/**
 * @file    ftfe.cpp (180.ARM_Peripherals/Sources/ftfe.cpp)
 * @brief   Flash support code
 *
 *  Created on: 10/1/2016
 *      Author: podonoghue
 */
#include <stdio.h>
#include <string.h>

#include "system.h"
#include "derivative.h"
#include "hardware.h"
#include "delay.h"
#include "ftfe.h"

namespace USBDM {

// Flash commands
//static constexpr uint8_t  F_RD1BLK      =  0x00;
//static constexpr uint8_t  F_RD1SEC      =  0x01;
//static constexpr uint8_t  F_PGMCHK      =  0x02;
static constexpr uint8_t  F_RDRSRC      =  0x03;
static constexpr uint8_t  F_PGM8        =  0x07;
//static constexpr uint8_t  F_ERSBLK      =  0x08;
static constexpr uint8_t  F_ERSSCR      =  0x09;
//static constexpr uint8_t  F_PGMSEC      =  0x0B;
//static constexpr uint8_t  F_RD1ALL      =  0x40;
//static constexpr uint8_t  F_RDONCE      =  0x41;
//static constexpr uint8_t  F_PGMONCE     =  0x43;
static constexpr uint8_t  F_ERSALL      =  0x44;
//static constexpr uint8_t  F_VFYKEY      =  0x45;
static constexpr uint8_t  F_PGMPART     =  0x80;
//static constexpr uint8_t  F_SETRAM      =  0x81;

/** A23 == 0 => indicates PROGRAM flash */
//static constexpr uint32_t PROGRAM_ADDRESS_FLAG = (0<<23);

/** A23 == 1 => indicates DATA flash */
static constexpr uint32_t DATA_ADDRESS_FLAG    = (1<<23);

/**
 * Launch & wait for Flash command to complete
 *
 * @note This routine is copied to the stack (RAM) for execution
 */
void Flash::executeFlashCommand_asm() {
   __asm__ volatile (
         "    .equ   FTFE_FSTAT,0x40020000        \n"
         "    .equ   FTFE_FSTAT_ERROR_MASK,0x70   \n" // = FTFE_FSTAT_RDCOLERR_MASK|FTFE_FSTAT_ACCERR_MASK|FTFE_FSTAT_FPVIOL_MASK
         "    .equ   FTFE_FSTAT_CCIF_MASK,0x80    \n"

         "     ldr   r1,=FTFE_FSTAT               \n" // Point R1 @FTFE_FSTAT

         "     movs  r2,#FTFE_FSTAT_ERROR_MASK    \n" // Clear previous errors
         "     strb  r2,[r1,#0]                   \n" // FTFE_FSTAT = FTFE_FSTAT_ERROR_MASK

         "     movs  r2,#FTFE_FSTAT_CCIF_MASK     \n" // Start command
         "     strb  r2,[r1,#0]                   \n" // FTFE_FSTAT = FTFE_FSTAT_CCIF_MASK

         "loop:                                   \n"
         "     ldrb  r3,[r1,#0]                   \n" // Wait for completion
#if (__CORTEX_M == 4)
         "     ands  r3,r2                        \n" // while ((FTFE->FSTAT & FTFE_FSTAT_CCIF_MASK) == 0) {
#else
         "     and   r3,r2                        \n" // while ((FTFE->FSTAT & FTFE_FSTAT_CCIF_MASK) == 0) {
#endif
         "     beq   loop                         \n" // }

         ::: "r1", "r2", "r3"
   );
}

/**
 * Launch & wait for Flash command to complete
 *
 * @note This routine must be placed in ROM immediately following executeFlashCommand_asm()
 */
FlashDriverError_t Flash::executeFlashCommand() {
   uint8_t space[50]; // Space for RAM copy of executeFlashCommand_asm()
   FlashDriverError_t (*fp)() = (FlashDriverError_t (*)())((uint32_t)space|1);

   volatile uint32_t source     = (uint32_t)executeFlashCommand_asm&~1;
   volatile uint32_t source_end = (uint32_t)executeFlashCommand&~1;
   volatile uint32_t size       = source_end-source;

   assert(size<sizeof(space));

   // Copy routine to RAM (stack)
   memcpy(space, (uint8_t*)(source), size);

   // Call executeFlashCommand_asm() on the stack with interrupts disabled
   disableInterrupts();
   (*fp)();
   enableInterrupts();

   // Handle any errors
   if ((FTFE->FSTAT & FTFE_FSTAT_FPVIOL_MASK ) != 0) {
      return FLASH_ERR_PROG_FPVIOL;
   }
   if ((FTFE->FSTAT & FTFE_FSTAT_ACCERR_MASK ) != 0) {
      return FLASH_ERR_PROG_ACCERR;
   }
   if ((FTFE->FSTAT & FTFE_FSTAT_MGSTAT0_MASK ) != 0) {
      return FLASH_ERR_PROG_MGSTAT0;
   }
   if ((FTFE->FSTAT & FTFE_FSTAT_RDCOLERR_MASK ) != 0) {
      return FLASH_ERR_PROG_RDCOLERR;
   }
   return FLASH_ERR_OK;
}

/**
 * Read Flash Resource (IFR etc)
 * This command reads 4 bytes from the selected flash resource
 *
 * @param[in]  resourceSelectCode 00 => IFR, 01 => Version ID
 * @param[in]  address            Address in IFR etc, A23=0 => Program flash, A23=1 => Data flash
 * @param[out] data               Buffer for data returned
 *
 * @return Error code, 0 => no error
 */
FlashDriverError_t Flash::readFlashResource(uint8_t resourceSelectCode, uint32_t address, uint8_t *data) {
   FTFE->FCCOB0 = F_RDRSRC;
   FTFE->FCCOB1 = address>>16;
   FTFE->FCCOB2 = address>>8;
   FTFE->FCCOB3 = address;
   FTFE->FCCOB8 = resourceSelectCode;
   FlashDriverError_t rc = executeFlashCommand();
   if (rc != FLASH_ERR_OK) {
      return rc;
   }
   data[0] = FTFE->FCCOB4;
   data[1] = FTFE->FCCOB5;
   data[2] = FTFE->FCCOB6;
   data[3] = FTFE->FCCOB7;

   return FLASH_ERR_OK;
}

/**
 * Program EEPROM Data Size Code and FlexNVM Partition Code
 *
 * See device reference manual for the meaning of the following parameters
 *
 * @param[in]  eeprom     EEPROM Data Size value
 * @param[in]  partition  FlexNVM Partition value
 *
 * @return Error code, 0 => no error
 */
FlashDriverError_t Flash::partitionFlash(uint8_t eeprom, uint8_t partition) {
   FTFE->FCCOB0 = F_PGMPART;
   FTFE->FCCOB1 = 0x00;
   FTFE->FCCOB2 = 0x00;
   FTFE->FCCOB3 = 0x00;
   FTFE->FCCOB4 = eeprom;
   FTFE->FCCOB5 = partition;
   FlashDriverError_t rc = executeFlashCommand();
   if (rc != FLASH_ERR_OK) {
      USBDM::setErrorCode(E_FLASH_INIT_FAILED);
   }
   return rc;
}

/**
 * Program phrase to Flash memory
 *
 * @param[in]  data       Location of data to program
 * @param[out] address    Memory address to program - must be phrase boundary
 *
 * @return Error code
 */
FlashDriverError_t Flash::programPhrase(const uint8_t *data, uint8_t *address) {
   FTFE->FCCOB0 = F_PGM8;
   FTFE->FCCOB1 = (uint8_t)(((uint32_t)address)>>16);
   FTFE->FCCOB2 = (uint8_t)(((uint32_t)address)>>8);
   FTFE->FCCOB3 = (uint8_t)(((uint32_t)address));
   FTFE->FCCOB7 = *data++;
   FTFE->FCCOB6 = *data++;
   FTFE->FCCOB5 = *data++;
   FTFE->FCCOB4 = *data++;
   FTFE->FCCOBB = *data++;
   FTFE->FCCOBA = *data++;
   FTFE->FCCOB9 = *data++;
   FTFE->FCCOB8 = *data++;
   FlashDriverError_t rc = executeFlashCommand();
   return rc;
}

/**
 * Program a range of bytes to Flash memory
 *
 * @param[in]  data       Location of data to program
 * @param[out] address    Memory address to program - must be phrase boundary
 * @param[in]  size       Size of range (in bytes) to program - must be multiple of phrase size
 *
 * @return Error code
 */
FlashDriverError_t Flash::programRange(const uint8_t *data, uint8_t *address, uint32_t size) {
   unsigned phraseSize;
   if ((uint32_t)address >= 0x10000000) {
      // DFLASH
      address = (uint8_t*)((uint32_t)address | DATA_ADDRESS_FLAG);
      phraseSize = dataFlashPhraseSize;
   }
   else {
      // PFLASH
      phraseSize = programFlashPhraseSize;
   }
   assert((((uint32_t)address)&(phraseSize-1)) == 0);
   assert((size&(phraseSize-1)) == 0);

   while (size>0) {
      FlashDriverError_t rc = programPhrase(data, address);
      if (rc != FLASH_ERR_OK) {
         return rc;
      }
      data    += phraseSize;
      address += phraseSize;
      size    -= phraseSize;
   }
   return FLASH_ERR_OK;
}

/**
 * Erase sector in Flash memory
 *
 * @param[in]  address    Memory address to erase - must be phrase boundary
 *
 * @return Error code
 */
FlashDriverError_t Flash::eraseSector(uint8_t *address) {
   FTFE->FCCOB0 = F_ERSSCR;
   FTFE->FCCOB1 = (uint8_t)(((uint32_t)address)>>16);
   FTFE->FCCOB2 = (uint8_t)(((uint32_t)address)>>8);
   FTFE->FCCOB3 = (uint8_t)(((uint32_t)address));
   FlashDriverError_t rc = executeFlashCommand();
   return rc;
}

/**
 * Program a range of bytes to Flash memory
 *
 * @param[in]  address    Memory address to start erasing - must be sector boundary
 * @param[in]  size       Size of range (in bytes) to erase - must be multiple of sector size
 *
 * @return Error code
 */
FlashDriverError_t Flash::eraseRange(uint8_t *address, uint32_t size) {
   unsigned sectorSize;
   if ((uint32_t)address >= 0x10000000) {
      // DFLASH
      address = (uint8_t*)((uint32_t)address | DATA_ADDRESS_FLAG);
      sectorSize = dataFlashSectorSize;
   }
   else {
      // PFLASH
      sectorSize = programFlashSectorSize;
   }
   assert((((uint32_t)address)&(sectorSize-1)) == 0);
   assert((size&(sectorSize-1)) == 0);

   while (size>0) {
      FlashDriverError_t rc = eraseSector(address);
      if (rc != FLASH_ERR_OK) {
         return rc;
      }
      address += sectorSize;
      size    -= sectorSize;
   }
   return FLASH_ERR_OK;
}

/**
 * Mass erase entire Flash memory
 */
void Flash::eraseAll() {
   FTFE->FCCOB0 = F_ERSALL;
   FlashDriverError_t rc = executeFlashCommand();
   (void)rc;
   // Don't expect it to get here as flash is erased!!!!
   for(;;) {
      __asm__("nop");
   }
}

}
