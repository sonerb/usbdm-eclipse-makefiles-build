/*****************************************************************
 * @file segment_lcd.h (derived from segment_lcd.c)
 *
 * Routines to interface to Segment LCD on FRDM-KL46Z
 *
 * Based on:
 * https://eewiki.net/display/microcontroller/Using+the+Segment+LCD+Controller+on+the+Kinetis+KL46
 *
 * @revision 1.0 EPH Initial version
 * @date     8/20/2014
 * @author   Ethan Hettwer
 *
 *****************************************************************/

#include <string.h>
#include "derivative.h"
#include "hardware.h"
#include "segment_lcd.h"
/*
 * *****************************
 * *** DO NOT EDIT THIS FILE ***
 * *****************************
 *
 * This file is generated automatically.
 * Any manual changes will be lost.
 */
namespace USBDM {

/*
 *     s401 Pin           | KL46 Pin        | KL43 Pin
 * ===============================================================
 *     1   (COM0)         | PTD0   LCD_P40  | PTE20  LCD_P59
 *     2   (COM1)         | PTE4   LCD_P52  | PTE21  LCD_P60
 *     3   (COM2)         | PTB23  LCD_P19  | PTB18  LCD_P14
 *     4   (COM3)         | PTB22  LCD_P18  | PTB19  LCD_P15
 *                        |                 |
 *     5   (1D/1E/1G/1F)  | PTC17  LCD_P37  | PTC0   LCD_P20
 *     6   (DP1/1C/1B/1A) | PTB21  LCD_P17  | PTC4   LCD_P24
 *     7   (2D/2E/2G/2F)  | PTB7   LCD_P7   | PTC6   LCD_P26
 *     8   (DP2/2C/2B/2A) | PTB8   LCD_P8   | PTC7   LCD_P27
 *     9   (3D/3E/3G/3F)  | PTE5   LCD_P53  | PTD0   LCD_P40
 *    10   (DP3/3C/3B/3A) | PTC18  LCD_P38  | PTD2   LCD_P42
 *    11   (4D/4E/4G/4F)  | PTB10  LCD_P10  | PTD3   LCD_P43
 *    12   (COL/4C/4B/4A) | PTB11  LCD_P11  | PTD4   LCD_P44
 */

$(/LCD/lcdPins)
// Each segment
static constexpr uint8_t  LCD_SEG_D        = 0x11;
static constexpr uint8_t  LCD_SEG_E        = 0x22;
static constexpr uint8_t  LCD_SEG_G        = 0x44;
static constexpr uint8_t  LCD_SEG_F        = 0x88;
static constexpr uint8_t  LCD_SEG_DECIMAL  = 0x11;
static constexpr uint8_t  LCD_SEG_C        = 0x22;
static constexpr uint8_t  LCD_SEG_B        = 0x44;
static constexpr uint8_t  LCD_SEG_A        = 0x88;
static constexpr uint8_t  LCD_CLEAR        = 0x00;

/*
 * Configure LCD_PENn - Pin Enable Register
 *
 * Controls which of the possible LCD pins are used
 * PEN[0] contains bits 0-31, while PEN[1] contains bits 32-63.
 *
 * @param pinMap  Map of logical pin numbers to physical LCD pins
 * @param pinNum  Logical pin number
 */
static void pen(const int8_t pinMap[], uint32_t pinNum) {
   const int lcdPin = pinMap[pinNum];
   LCD->PEN[lcdPin>>5] |= 1UL<<((uint32_t)(lcdPin&0x1F));
};

/*
 * Configure LCD_SEG_BPENn - Back Plane Enable Register
 *
 * Controls which pins in PEN are Back Plane (commons)
 * BPEN[0] contains bits 0-31, while BPEN[1] contains bits 32-63.
 *
 * @param pinMap  Map of logical pin numbers to physical LCD pins
 * @param pinNum  Logical pin number
 */
static void bpen(const int8_t pinMap[], uint32_t pinNum) {
   const int lcdPin = pinMap[pinNum];
   LCD->BPEN[lcdPin>>5] |= 1UL<<((uint32_t)(lcdPin&0x1F));
};

/**
 * Initialises all components of the SLCD on the FRDM-KL46Z
 *
 * Assumes use of 32kHz OSCERCLK clock
 */
void SegmentLcd::enable(void) {

   // Enable Clock to SegLCD Peripheral
   SIM->SCGC5 |=  SIM_SCGC5_SLCD_MASK;

   // Set PADSAFE to disable LCD while configuring
   lcd->GCR   |=  LCD_GCR_PADSAFE_MASK;

   // Clear LCDEN (LCD Enable) while configuring
   lcd->GCR   &= ~LCD_GCR_LCDEN_MASK;

   /*
    * Configure pins.
    * From Reference manual, set pins to MUX 0 for normal LCD display operation, only use MUX 7 if using LCD fault detection
    */
   LcdInfo::initPCRs();

   /*
    * Configure LCD_GCR - General Control Register, controls most options in LCD Peripheral
    *
    * Use safe value while being configured
    */
   lcd->GCR = (LcdInfo::gcr | LCD_GCR_PADSAFE_MASK) & ~LCD_GCR_LCDEN_MASK;

   /*
    * Configure LCD_SEG_AR  - Auxiliary Register, controls blinking of LCD
    */
   lcd->AR =
         LCD_AR_BLINK(0)   | // Disable SLCD blinking.  Enable to make LCD Blink.
         LCD_AR_ALT(0)     | // If enabled LCD back plane sequencer changes to an alternate display.
         //                  // Only functional if DUTY[2:0] is less than 100(binary).
         //                  // This allows a blink screen that is not blank.
         LCD_AR_BLANK(0)   | // Asserting bit clears all segments in LCD.
         LCD_AR_BMODE(0)   | // If enabled displays alternate display during blink period instead of blank.
         LCD_AR_BRATE(0x00); // Frequency of blink is determined by LCD clock/(2^(12 + BRATE))

   /*
    * Configure LCD_SEG_FDCR - Fault Detect Control Register, controls use of Fault Detect features of SLCD.
    *
    * Clear all bits. Fault detection not used.
    */
   lcd->FDCR = 0x00000000;

   // Enable front-pane pins
   for (unsigned int index=0; index<sizeof(frontplanePins)/sizeof(frontplanePins[0]); index++) {
      pen(frontplanePins, index);
   }

   // Enable back-pane pins
   for (unsigned int index=0; index<sizeof(backplanePins)/sizeof(backplanePins[0]); index++) {
      // Enable
      pen(backplanePins, index);
      // Set as backplane
      bpen(backplanePins, index);
   }

   // This applies for both disabled & off segments
   memset((uint8_t*)(lcd->WF), 0x00, sizeof(lcd->WF));

   lcd->WF[backplanePins[3]] = 0x88; // (COM3) is enabled on Phases D and H
   lcd->WF[backplanePins[2]] = 0x44; // (COM2) is enabled on Phases C and G
   lcd->WF[backplanePins[1]] = 0x22; // (COM1) is enabled on Phases B and F
   lcd->WF[backplanePins[0]] = 0x11; // (COM0) is enabled on Phases A and E

   // Clear PADSAFE to unlock LCD pins
   lcd->GCR &= ~LCD_GCR_PADSAFE_MASK;

   // Set LCDEN to enable operation of LCD
   lcd->GCR |= LCD_GCR_LCDEN_MASK;
}

/**
 * Set Decimal point/colon on or off
 *
 * @param digit 0-2 => DP, 3 => colon
 * @param value true/false => on/off
 *
 */
void SegmentLcd::setDPs(int digit, int value) {
   if (value) {
      lcd->WF[frontplanePins[2*digit+1]]  |=  LCD_SEG_DECIMAL;
   }
   else {
      lcd->WF[frontplanePins[2*digit+1]]  &=  ~LCD_SEG_DECIMAL;
   }
}

/**
 * Displays a hex value in a specified position on the LCD.  \n
 * Will display error if position is outside of range.
 *
 * @param value    Value to display (0-F), -1 => blank
 * @param position Digit position (left=1 - 4=right)
 */
void SegmentLcd::set(int value, int position) {

   typedef struct {
      uint8_t m1;
      uint8_t m2;
   } SegmentData;

   static const SegmentData segmentData[] = {
         /* 0 */ { (LCD_SEG_D | LCD_SEG_E |LCD_SEG_F),                (LCD_SEG_A | LCD_SEG_B | LCD_SEG_C) },
         /* 1 */ { (LCD_CLEAR),                                       (LCD_SEG_B | LCD_SEG_C) },
         /* 2 */ { (LCD_SEG_D | LCD_SEG_E | LCD_SEG_G),               (LCD_SEG_A | LCD_SEG_B) },
         /* 3 */ { (LCD_SEG_D | LCD_SEG_G),                           (LCD_SEG_A | LCD_SEG_B | LCD_SEG_C) },
         /* 4 */ { (LCD_SEG_F | LCD_SEG_G),                           (LCD_SEG_B | LCD_SEG_C) },
         /* 5 */ { (LCD_SEG_D | LCD_SEG_F | LCD_SEG_G),               (LCD_SEG_A | LCD_SEG_C) },
         /* 6 */ { (LCD_SEG_D | LCD_SEG_E | LCD_SEG_F | LCD_SEG_G),   (LCD_SEG_A | LCD_SEG_C) },
         /* 7 */ { (LCD_CLEAR),                                       (LCD_SEG_A | LCD_SEG_B | LCD_SEG_C) },
         /* 8 */ { (LCD_SEG_D | LCD_SEG_E | LCD_SEG_F | LCD_SEG_G),   (LCD_SEG_A | LCD_SEG_B | LCD_SEG_C) },
         /* 9 */ { (LCD_SEG_F | LCD_SEG_G),                           (LCD_SEG_A | LCD_SEG_B | LCD_SEG_C) },
         /* A */ { (LCD_SEG_E | LCD_SEG_F | LCD_SEG_G),               (LCD_SEG_A | LCD_SEG_B | LCD_SEG_C) },
         /* B */ { (LCD_SEG_D | LCD_SEG_E | LCD_SEG_F | LCD_SEG_G),   (LCD_SEG_C) },
         /* C */ { (LCD_SEG_D | LCD_SEG_E | LCD_SEG_F),               (LCD_SEG_A) },
         /* D */ { (LCD_SEG_D | LCD_SEG_E | LCD_SEG_G),               (LCD_SEG_B | LCD_SEG_C) },
         /* E */ { (LCD_SEG_D | LCD_SEG_E | LCD_SEG_F | LCD_SEG_G),   (LCD_SEG_A) },
         /* F */ { (LCD_SEG_E | LCD_SEG_F | LCD_SEG_G),               (LCD_SEG_A) },
   };

   if (position > 4){
      // Display "Err" if trying to access a digit that does not exist
      SegmentLcd::displayError(0x01);
   }
   else if (value<0) {
      lcd->WF[frontplanePins[((2*position)-2)]] = 0;
      lcd->WF[frontplanePins[((2*position)-1)]] = 0;
   }
   else {
      lcd->WF[frontplanePins[((2*position)-2)]] = segmentData[value].m1;
      lcd->WF[frontplanePins[((2*position)-1)]] = segmentData[value].m2;
   }
}

/**
 * Displays a 4 digit decimal number
 *
 * @param value Value to display (0000-9999)
 */
void SegmentLcd::displayDecimal(uint16_t value){

   if (value > 9999) {
      // Display "Err" if value is greater than 4 digits
      displayError(0x10);
   }
   else{
      set(value/1000,     1);
      set((value/100)%10, 2);
      set((value/10)%10,  3);
      set(value%10,       4);
   }
}

/**
 * Displays a 4 digit decimal number leading zero suppression
 *
 * @param value Value to display (0-9999)
 */
void SegmentLcd::displayDecimalLz(uint16_t value){

   if (value > 9999) {
      // Display "Err" if value is greater than 4 digits
      displayError(0x10);
   }
   else{
      bool leadingZero = true;
      auto lzero = [&](int digit) {
         if (digit>0) {
            leadingZero = false;
         }
         if (!leadingZero) {
            return digit;
         }
         return -1;
      };
      set(lzero(value/1000),     1);
      set(lzero((value/100)%10), 2);
      set(lzero((value/10)%10),  3);
      set(value%10,              4);
   }
}

/**
 * Displays a 4 Digit hex number
 *
 * @param value Value to display (0x0000-0xFFFF)
 */
void SegmentLcd::displayHex(uint16_t Value){

   set((Value>>12) & 0xF, 1);
   set((Value>>8)  & 0xF, 2);
   set((Value>>4)  & 0xF, 3);
   set( Value      & 0xF, 4);
}


/**
 * Displays two numbers as hour:minute i.e. separated by colon
 *
 * @param hour Hours value to display (0-99)
 * @param minutes Minutes value to display (0-59)
 *
 */
void SegmentLcd::displayTime(uint8_t hour, uint8_t minutes) {
   if ((hour > 99) | (minutes > 99)){
      // Display "Err" if either value is greater than 2 digits
      displayError(0x10);
   }
   else{
      set(hour/10, 1);
      set(hour % 10, 2);
      set(minutes/10, 3);
      set(minutes % 10, 4);
      setDPs(3, 1);
   }
}

/**
 * Display error number
 *
 * @param ErrorNum Error number value 0-F.\n
 *        If ErrorNum is outside of that range, just displays Err
 */
void SegmentLcd::displayError(uint8_t ErrorNum){

   lcd->WF[frontplanePins[0]] = (LCD_SEG_D | LCD_SEG_E | LCD_SEG_F | LCD_SEG_G);
   lcd->WF[frontplanePins[1]] = (LCD_SEG_A);
   lcd->WF[frontplanePins[2]] = (LCD_SEG_E | LCD_SEG_G);
   lcd->WF[frontplanePins[3]] = (LCD_CLEAR);
   lcd->WF[frontplanePins[4]] = (LCD_SEG_E | LCD_SEG_G);
   lcd->WF[frontplanePins[5]] = (LCD_CLEAR);
   if (ErrorNum <= 0xF) {
      // Display ErrorNum if within valid range.
      set(ErrorNum,4);
   }
   else{
      // If not, leave blank.
      lcd->WF[frontplanePins[6]] = (LCD_CLEAR);
      lcd->WF[frontplanePins[7]] = (LCD_CLEAR);
   }
}

}  // End namespace USBDM

