/**
 * @file     pcr.h (180.ARM_Peripherals/Project_Headers/pcr-MK.h)
 * @brief    Port Control Register interface
 *
 * @version  V4.12.1.80
 * @date     13 April 2016
 */
#ifndef HEADER_PCR_H
#define HEADER_PCR_H
/*
 * *****************************
 * *** DO NOT EDIT THIS FILE ***
 * *****************************
 *
 * This file is generated automatically.
 * Any manual changes will be lost.
 */
#include <stddef.h>
#include <assert.h>
#include "derivative.h"
#include "bitband.h"

/*
 * Default port information
 */
namespace USBDM {

enum Polarity {
   ActiveLow=false,  //!< Signal is active low i.e. Active => Low level, Inactive => High level
   ActiveHigh=true   //!< Signal is active high i.e. Active => High level, Inactive => Low level
};

/*
 * PORT clock masks
 */
#ifdef SIM_SCGC5_PORTA_MASK
constexpr   uint32_t PORTA_CLOCK_MASK         = SIM_SCGC5_PORTA_MASK;
#endif

#ifdef SIM_SCGC5_PORTB_MASK
constexpr   uint32_t PORTB_CLOCK_MASK         = SIM_SCGC5_PORTB_MASK;
#endif

#ifdef SIM_SCGC5_PORTC_MASK
constexpr   uint32_t PORTC_CLOCK_MASK         = SIM_SCGC5_PORTC_MASK;
#endif

#ifdef SIM_SCGC5_PORTD_MASK
constexpr   uint32_t PORTD_CLOCK_MASK         = SIM_SCGC5_PORTD_MASK;
#endif

#ifdef SIM_SCGC5_PORTE_MASK
constexpr   uint32_t PORTE_CLOCK_MASK         = SIM_SCGC5_PORTE_MASK;
#endif

#ifdef SIM_SCGC5_PORTF_MASK
constexpr   uint32_t PORTF_CLOCK_MASK         = SIM_SCGC5_PORTF_MASK;
#endif

extern volatile uint32_t SystemMcgffClock;
extern volatile uint32_t SystemMcgOutClock;
extern volatile uint32_t SystemMcgFllClock;
extern volatile uint32_t SystemMcgPllClock;
extern volatile uint32_t SystemCoreClock;
extern volatile uint32_t SystemBusClock;
extern volatile uint32_t SystemLpoClock;

/*
 * Enable clock to ports
 *
 * @param[in] mask Mask for PORTs to enable
 */
static inline __attribute__((always_inline)) void enablePortClocks(uint32_t clockMask) {
   SIM->SCGC5 |= clockMask;
   __DMB();
};

/*
 * Disable clock to ports
 *
 * @param[in] mask Mask for PORTs to disable
 */
static inline __attribute__((always_inline)) void disablePortClocks(uint32_t clockMask) {
   SIM->SCGC5 &= ~clockMask;
   __DMB();
};

/**
 * @addtogroup PeripheralPinTables Peripheral Information Classes
 * @brief Provides information about pins used by a peripheral
 * @{
 */

/** Pin number indicating the function has a fixed mapping to a pin */
constexpr   int8_t FIXED_NO_PCR         = 0x00;

/** Pin number indicating the function doesn't exist. Note: -ve value*/
constexpr   int8_t INVALID_PCR          = 0xA5;

/** Pin number indicating the function is not currently mapped to a pin. Note: -ve value */
constexpr   int8_t UNMAPPED_PCR         = 0xA4;

/**
 * Struct for Pin Control Register information
 * Information required to configure the PCR for a particular function
 */
struct PcrInfo {
   uint32_t clockMask;   //!< Clock mask for PORT
   uint32_t pcrAddress;  //!< PCR register array address
   uint32_t gpioAddress; //!< Address of GPIO hardware associated with pin
   int8_t   gpioBit;     //!< Bit number of pin in GPIO (-ve indicates unmapped/invalid)
   uint32_t pcrValue;    //!< PCR value including MUX value to select this function
};

#ifndef PORT_PCR_DSE
/**
 * Some devices don't have DSE function on pin
 */
#define PORT_PCR_DSE(x) (0)
#endif

#ifndef PORT_PCR_ODE
/**
 * Some devices don't have ODE function on pin
 * The open-drain mode is automatically selected when I2C function is selected for the pin
 */
#define PORT_PCR_ODE(x) 0
#endif

#ifndef PORT_PCR_PE
/**
 * Some devices don't have PE function on pin
 */
#define PORT_PCR_PE(x) 0
#endif

#ifndef PORT_PCR_PS
/**
 * Some devices don't have PS function on pin
 */
#define PORT_PCR_PS(x) 0
#endif

#ifndef PORT_PCR_SRE
/**
 * Some devices don't have PS function on pin
 */
#define PORT_PCR_SRE(x) 0
#endif

#ifndef PORT_PCR_PFE
/**
 * Some devices don't have PFE function on pin
 */
#define PORT_PCR_PFE(x) 0
#endif

/**
 * Pull device modes
 */
enum PinPull {
   PinPull_None = PORT_PCR_PE(0),                //!< No pull device
   PinPull_Up   = PORT_PCR_PE(1)|PORT_PCR_PS(1), //!< Weak pull-up
   PinPull_Down = PORT_PCR_PE(1)|PORT_PCR_PS(0), //!< Weak pull-down
};

/**
 * Pin drive strengths
 * Few pins support this function
 */
enum PinDriveStrength {
   PinDriveStrength_Low  = PORT_PCR_DSE(0), //!< Low drive strength
   PinDriveStrength_High = PORT_PCR_DSE(1), //!< High drive strength
};

/**
 * Pin drive mode
 * Not all pins support this function
 */
enum PinDriveMode {
   PinDriveMode_PushPull      = PORT_PCR_ODE(0), //!< Push-pull output
   PinDriveMode_OpenDrain     = PORT_PCR_ODE(1), //!< Open-drain output
   PinDriveMode_OpenCollector = PinDriveMode_OpenDrain,       //!< For oldies like me
};

/**
 * Pin Slew rate control
 * Not all pins support this function
 */
enum PinSlewRate {
   PinSlewRate_Slow = PORT_PCR_SRE(1),  //!< Slow slew rate on output
   PinSlewRate_Fast = PORT_PCR_SRE(0),  //!< Fast slew rate on output
};

/**
 * Pin filter mode
 * Not all pins support this function
 */
enum PinFilter {
   PinFilter_None      = PORT_PCR_PFE(0), //!< No pin filter
   PinFilter_Passive   = PORT_PCR_PFE(1), //!< Pin filter enabled
};

/**
 * Pin Multiplexor setting
 */
enum PinMux {
   PinMux_None      = PORT_PCR_MUX(0), //!< Used for default/no mapping
   PinMux_Analogue  = PORT_PCR_MUX(0), //!< Analogue function
   PinMux_Analog    = PinMux_Analogue,  //!< Analogue function
   PinMux_Tsi       = PinMux_Analogue,  //!< Touch Sense function is analogue
   PinMux_Gpio      = PORT_PCR_MUX(1), //!< Gpio function
   PinMux_2         = PORT_PCR_MUX(2), //!< Mux 2
   PinMux_3         = PORT_PCR_MUX(3), //!< Mux 3
   PinMux_4         = PORT_PCR_MUX(4), //!< Mux 4
   PinMux_5         = PORT_PCR_MUX(5), //!< Mux 5
   PinMux_6         = PORT_PCR_MUX(6), //!< Mux 6
   PinMux_7         = PORT_PCR_MUX(7), //!< Mux 7
};

/**
 * Pin interrupt/DMA modes
 */
enum PinIrq {
   PinIrq_None     = PORT_PCR_IRQC(0),   //!< No interrupt or DMA function

   PinDma_Rising   = PORT_PCR_IRQC(1),   //!< Generate DMA request on rising edge
   PinDma_Falling  = PORT_PCR_IRQC(2),   //!< Generate DMA request on falling edge
   PinDma_Either   = PORT_PCR_IRQC(3),   //!< Generate DMA request on either edge

   PinIrq_Low      = PORT_PCR_IRQC(8),   //!< Generate IRQ request when low
   PinIrq_Rising   = PORT_PCR_IRQC(9),   //!< Generate IRQ request on rising edge
   PinIrq_Falling  = PORT_PCR_IRQC(10),  //!< Generate IRQ request on falling edge
   PinIrq_Either   = PORT_PCR_IRQC(11),  //!< Generate IRQ request on either edge
   PinIrq_High     = PORT_PCR_IRQC(12),  //!< Generate IRQ request when high
};

/** Type for PCR value used by PCR functions */
using PcrValue = uint32_t;

/**
 * Construct PCR value from individual bit masks
 *
 * @param[in] pinPull          One of PinPull_None, PinPull_Up, PinPull_Down (defaults to PinPull_None)
 * @param[in] pinDriveStrength One of PinDriveStrength_Low, PinDriveStrength_High (defaults to PinDriveLow)
 * @param[in] pinDriveMode     One of PinDriveMode_PushPull, PinDriveMode_OpenDrain (defaults to PinPushPull)
 * @param[in] pinIrq           One of PinIrq_None, etc (defaults to PinIrq_None)
 * @param[in] pinFilter        One of PinFilter_None, PinFilter_Passive (defaults to PinFilter_None)
 * @param[in] pinSlewRate      One of PinSlewRate_Slow, PinSlewRate_Fast (defaults to PinSlewRate_Fast)
 * @param[in] pinMux           Multiplexor setting (defaults to 0)
 *
 * @return PCR value constructed from individual flags
 */
static __attribute__((always_inline)) constexpr PcrValue pcrValue(
      PinPull           pinPull           = PinPull_None,
      PinDriveStrength  pinDriveStrength  = PinDriveStrength_Low,
      PinDriveMode      pinDriveMode      = PinDriveMode_PushPull,
      PinIrq            pinIrq            = PinIrq_None,
      PinFilter         pinFilter         = PinFilter_None,
      PinSlewRate       pinSlewRate       = PinSlewRate_Fast,
      PinMux            pinMux            = PinMux_None
      ) {
   return pinPull|pinDriveStrength|pinDriveMode|pinIrq|pinFilter|pinSlewRate|pinMux;
}

/**
 * Construct PCR value from existing PcrValue and extra flags
 *
 * @param[in] original      Original value
 * @param[in] extraFlags    Extra flags
 *
 * @return PCR value constructed from individual flags
 */
static __attribute__((always_inline)) constexpr PcrValue pcrValue(PcrValue original, uint32_t extraFlags) {
   return original|extraFlags;
}

/**
 * Construct PCR value from flags
 *
 * @param[in] flags Extra flags
 *
 * @return PCR value constructed from individual flags
 */
static __attribute__((always_inline)) constexpr PcrValue pcrValue(uint32_t flags) {
   return flags;
}

/**
 * Default PCR setting for pins (excluding multiplexor value)
 * High drive strength + Pull-up
 */
static constexpr PcrValue DEFAULT_PCR = pcrValue(PinPull_Up, PinDriveStrength_High);

/**
 * Default PCR value for pins used as GPIO (including multiplexor value)
 * High drive strength + Pull-up + GPIO_MUX value
 */
static constexpr PcrValue GPIO_DEFAULT_PCR = pcrValue(PinPull_Up, PinDriveStrength_High, PinDriveMode_PushPull, PinIrq_None, PinFilter_None, PinSlewRate_Fast, PinMux_Gpio);

/**
 * Default PCR setting for I2C pins (excluding multiplexor value)
 * High drive strength + Pull-up + Open-drain (if available)
 */
static constexpr PcrValue I2C_DEFAULT_PCR =  pcrValue(PinPull_Up, PinDriveStrength_High, PinDriveMode_OpenDrain);

/**
 * Default PCR setting for (E)XTAL pins (excluding multiplexor value)
 */
static constexpr PcrValue XTAL_DEFAULT_PCR = pcrValue(PinPull_None, PinDriveStrength_Low, PinDriveMode_PushPull);

/**
 * Type definition for PORT interrupt call back.
 * This callback is shared by all port pins
 *
 * @param[in] status 32-bit value from ISFR (each bit indicates a pin interrupt source)
 */
typedef void (*PinCallbackFunction)(uint32_t status);

/**
 * Common PORT features shared by all port pins
 *
 * tparam pcrAddress Base address of PCR register array
 */
template<uint32_t pcrAddress>
class PcrBase_T {

private:
   /**
    * This class is not intended to be instantiated
    */
   PcrBase_T() = delete;
   PcrBase_T(const PcrBase_T&) = delete;
   PcrBase_T(PcrBase_T&&) = delete;

protected:
   /** Callback to catch unhandled interrupt */
   static void unhandledCallback(uint32_t) {
      setAndCheckErrorCode(E_NO_HANDLER);
   }

private:
   static constexpr volatile PORT_Type *port = reinterpret_cast<volatile PORT_Type *>(pcrAddress);

   /** Callback functions for ISRs */
   static PinCallbackFunction fCallback;

public:
   /**
    * Interrupt handler -  Calls callback
    */
   static void irqHandler() {
      // Capture interrupt flags
      uint32_t status = port->ISFR;

      // Clear flags
      port->ISFR = status;

      fCallback(status);
   }
   /**
    * Set callback for Pin IRQ
    *
    * @note There is a single callback function for all pins on the related port.
    *
    * @param[in] callback The function to call on Pin interrupt. \n
    *                     nullptr to indicate none
    */
   static __attribute__((always_inline)) void setCallback(PinCallbackFunction callback) {
      if (callback == nullptr) {
         fCallback = unhandledCallback;
         return;
      }
      fCallback = callback;
   }
};

template<uint32_t pcrAddress>
PinCallbackFunction USBDM::PcrBase_T<pcrAddress>::fCallback = PcrBase_T::unhandledCallback;

/**
 * @brief Template representing a Pin Control Register (PCR)
 *
 * Code examples:
 * @code
 * // Create PCR type
 * using PortC_3 = USBDM::Pcr_T<SIM_SCGC5_PORTC_MASK, PORTC_BasePtr, 3, USBDM::DEFAULT_PCR>;
 *
 * // Configure PCR
 * PortC_3::setPCR(PinPull_Up,PinDriveStrength_High,PinDriveMode_PushPull,PinIrq_None,PinFilter_None,PinSlewRate_Fast,PinMux_3);
 *
 * // Disable Port clock
 * PortC_3::disableClock();
 * @endcode
 *
 * @tparam clockMask       Mask for SIM clock register associated with this PCR
 * @tparam pcrAddress      PORT to be manipulated e.g. PORTA (PCR array)
 * @tparam bitNum          Bit number e.g. 3
 * @tparam defPcrValue     Default value for PCR (including MUX value)
 */
template<uint32_t clockMask, uint32_t pcrAddress, int bitNum, PcrValue defPcrValue>
class Pcr_T : public PcrBase_T<pcrAddress> {

#ifdef DEBUG_BUILD
   static_assert((bitNum != UNMAPPED_PCR), "Pcr_T: Signal is not mapped to a pin - Modify Configure.usbdm");
   static_assert((bitNum != INVALID_PCR),  "Pcr_T: Non-existent signal");
   static_assert((bitNum == UNMAPPED_PCR)||(bitNum == INVALID_PCR)||((bitNum >= 0)&&(bitNum <= 31)), "Pcr_T: Illegal bit number");
#endif

public:
   static constexpr int      BITNUM  = bitNum;
   static constexpr uint32_t BITMASK = (1<<bitNum);

private:

   static constexpr volatile uint32_t *pcrReg = reinterpret_cast<volatile uint32_t *>(pcrAddress+offsetof(PORT_Type,PCR[bitNum]));

public:
   using PcrBase = PcrBase_T<pcrAddress>;

   using PcrBase_T<pcrAddress>::setCallback;

   /**
    * Enable/disable clock associated with PORT
    *
    * @param[in] enable true => enable, false => disable
    */
   static __attribute__((always_inline)) void enableClock(bool enable=true) {
      if (enable) {
         enablePortClocks(clockMask);
      }
      else {
         disablePortClocks(clockMask);
      }
   }

   /**
    * Set pin PCR value
    * The clock to the port will be enabled before changing the PCR
    *
    * @param[in] pcrValue PCR value constructed using pcrValue() including MUX value.\n
    *                     Defaults to template value.
    */
   static __attribute__((always_inline)) void setPCR(PcrValue pcrValue=defPcrValue) {
      if (pcrAddress != 0) {
         enablePortClocks(clockMask);

         // Pointer to PCR register for pin
         *pcrReg = pcrValue;
      }
   }
   /**
    * Set Pin Control Register (PCR) value
    *
    * @param[in] pinPull          One of PinPull_None, PinPull_Up, PinPull_Down
    * @param[in] pinDriveStrength One of PinDriveStrength_Low, PinDriveStrength_High (defaults to PinDriveLow)
    * @param[in] pinDriveMode     One of PinDriveMode_PushPull, PinDriveMode_OpenDrain (defaults to PinPushPull)
    * @param[in] pinIrq           One of PinIrq_None, etc (defaults to PinIrq_None)
    * @param[in] pinFilter        One of PinFilter_None, PinFilter_Passive (defaults to PinFilter_None)
    * @param[in] pinSlewRate      One of PinSlewRate_Slow, PinSlewRate_Fast (defaults to PinSlewRate_Fast)
    * @param[in] pinMux           One of PinMux_Analogue, PinMux_Gpio etc (defaults to template value)
    */
   static __attribute__((always_inline)) void setPCR(
         PinPull           pinPull,
         PinDriveStrength  pinDriveStrength  = PinDriveStrength_Low,
         PinDriveMode      pinDriveMode      = PinDriveMode_PushPull,
         PinIrq            pinIrq            = PinIrq_None,
         PinFilter         pinFilter         = PinFilter_None,
         PinSlewRate       pinSlewRate       = PinSlewRate_Fast,
         PinMux            pinMux            = (PinMux)(defPcrValue&PORT_PCR_MUX_MASK)
         ) {
      if (pcrAddress != 0) {
         enablePortClocks(clockMask);

         // Pointer to PCR register for pin
         *pcrReg = pinPull|pinDriveStrength|pinDriveMode|pinIrq|pinFilter|pinSlewRate|pinMux;
      }
   }
   /**
    * Set pin PCR.MUX value
    * Assumes clock to the port has already been enabled
    *
    * @param[in] pinMux PCR MUX value [0..7]
    */
   static __attribute__((always_inline)) void setMux(PinMux pinMux) {
      *pcrReg = (*pcrReg&~PORT_PCR_MUX_MASK) | pinMux;
   }
   /**
    * Sets pin interrupt/DMA mode
    * Assumes clock to the port has already been enabled
    *
    * @param[in] pinIrq Interrupt/DMA mode
    */
   static __attribute__((always_inline)) void setIrq(PinIrq pinIrq) {
      *pcrReg = (*pcrReg&~PORT_PCR_IRQC_MASK) | pinIrq;
   }

   /**
    * Clear interrupt flag
    * Assumes clock to the port has already been enabled
    */
   static __attribute__((always_inline)) void clearIrqFlag() {
      *pcrReg |= PORT_PCR_ISF_MASK;
   }

#ifdef PORT_PCR_PE_MASK
   /**
    * Set pull device on pin
    * Assumes clock to the port has already been enabled
    *
    *  @param[in] pinPull Pull selection mode
    */
   static __attribute__((always_inline)) void setPullDevice(PinPull pinPull) {
      *pcrReg = (*pcrReg&~PORT_PCR_PD_MASK) | pinPull;
   }
#else
   /**
    * Not supported
    */
   static __attribute__((always_inline)) void setPullDevice(PinPullMode) {
   }
#endif

#ifdef PORT_PCR_ODE_MASK
   /**
    * Set drive mode on pin
    * Assumes clock to the port has already been enabled
    *
    *  @param[in] pinDriveMode Drive mode
    */
   static __attribute__((always_inline)) void setDriveMode(PinDriveMode pinDriveMode) {
      *pcrReg = (*pcrReg&~PORT_PCR_ODE_MASK) | pinDriveMode;
   }
#else
   /**
    * Not supported
    */
   static __attribute__((always_inline)) void setDriveMode(PinDriveMode) {
   }
#endif

#ifdef PORT_PCR_SRE_MASK
   /**
    * Set slew rate on pin
    * Assumes clock to the port has already been enabled
    *
    *  @param[in] pinSlewRate Slew rate. Either PinSlewRate_Slow or PinSlewRate_Fast
    */
   static __attribute__((always_inline)) void setSlewRate(PinSlewRate  pinSlewRate) {
      *pcrReg = (*pcrReg&~PORT_PCR_SRE_MASK) | pinSlewRate;
   }
#else
   /**
    * Not supported
    */
   static __attribute__((always_inline)) void setSlewRate(PinSlewRate) {
   }
#endif

#ifdef PORT_PCR_PFE_MASK
   /**
    * Set filter on pin
    * Assumes clock to the port has already been enabled
    *
    *  @param[in] pinFilter Pin filter option. Either PinFilter_None or PinFilter_Passive
    */
   static __attribute__((always_inline)) void setFilter(PinFilter pinFilter) {
      *pcrReg = (*pcrReg&~PORT_PCR_PFE_MASK) | pinFilter;
   }
#else
   /**
    * Not supported
    */
   static __attribute__((always_inline)) void setFilter(PinFilter) {
   }
#endif

#if defined(PORT_PCR_DSE_MASK)
   /**
    * Set drive strength on pin
    * Assumes clock to the port has already been enabled
    *
    *  @param[in] pinDriveStrength Drive strength to set
    */
   static __attribute__((always_inline)) void setDriveStrength(PinDriveStrength pinDriveStrength) {
      *pcrReg = (*pcrReg&~PORT_PCR_DSE_MASK) | pinDriveStrength;
   }
#else
   /**
    * Not supported
    */
   static __attribute__((always_inline)) void setDriveStrength(PinDriveStrength) {
   }
#endif

#if defined(PORT_PCR_LK_MASK)
   /**
    * Locks most of the PCR properties e.g. drive strength, pull-device etc.
    * Assumes clock to the port has already been enabled
    * The pin properties remains locked until the next reset
    * Not supported on all devices
    */
   static __attribute__((always_inline)) void lock() {
      *pcrReg |= PORT_PCR_LK_MASK;
   }
#else
   /**
    * Not supported
    */
   static __attribute__((always_inline)) void lock() {
   }
#endif

   /**
    * Enable/disable Pin interrupts in NVIC
    *
    * @param[in] enable true => enable, false => disable
    */
   static __attribute__((always_inline)) void enableNvicInterrupts(bool enable=true) {

      constexpr IRQn_Type irqNum = (IRQn_Type)(PORTA_IRQn+((pcrAddress-PORTA_BasePtr)/(PORTB_BasePtr-PORTA_BasePtr)));

      if (enable) {
         // Enable interrupts
         NVIC_EnableIRQ(irqNum);

         // Set priority level
         NVIC_SetPriority(irqNum, 3);
      }
      else {
         // Disable interrupts
         NVIC_DisableIRQ(irqNum);
      }
   }
};

/**
 * @brief Template function to set a PCR to the default value
 *
 * @tparam  Last PCR to modify
 */
template<typename Last>
void processPcrs() {
   Last::setPCR();
}
/**
 * @brief Template function to set a collection of PCRs to the default value
 *
 * @tparam  Pcr1 PCR to modify
 * @tparam  Pcr2 PCR to modify
 * @tparam  Rest Remaining PCRs to modify
 */
template<typename Pcr1, typename  Pcr2, typename  ... Rest>
void processPcrs() {
   processPcrs<Pcr1>();
   processPcrs<Pcr2, Rest...>();
}
/**
 * @brief Template function to set a PCR to a given value
 *
 * @param[in]   pcrValue PCR value to set
 *
 * @tparam  Last PCR to modify
 */
template<typename Last>
void processPcrs(uint32_t pcrValue) {
   Last::setPCR(pcrValue);
}

/**
 * @brief Template function to set a collection of PCRs to a given value
 *
 * @param[in] pcrValue PCR value to set
 *
 * @tparam  Pcr1 PCR to modify
 * @tparam  Pcr2 PCR to modify
 * @tparam  Rest Remaining PCRs to modify
 */
template<typename Pcr1, typename  Pcr2, typename  ... Rest>
void processPcrs(uint32_t pcrValue) {
   processPcrs<Pcr1>(pcrValue);
   processPcrs<Pcr2, Rest...>(pcrValue);
}

/**
 * @brief Template representing a Pin Control Register (PCR)
 * Makes use of a configuration class
 *
 * Code examples:
 * @code
 * // Create PCR type
 * Pcr_T<spiInfo, 3> SpiMOSI;
 *
 * // Configure PCR
 * SpiMOSI::setPCR(PinPull_Up,PinDriveStrength_High,PinDriveMode_PushPull,PinIrq_None,PinFilter_None,PinSlewRate_Fast,PinMux_3);
 *
 * // Disable clock to associated PORT
 * SpiMOSI::disableClock();
 *
 * // Alternatively the PCR may be manipulated directly
 * Pcr_T<spiInfo, 3>::setPCR(PORT_PCR_DSE_MASK|PORT_PCR_PE_MASK|PORT_PCR_PS_MASK);
 * @endcode
 *
 * @tparam info          Configuration class
 * @tparam index         Index of pin in configuration table
 */
template<class Info, uint8_t index> using PcrTable_T =
      Pcr_T<Info::info[index].clockMask, Info::info[index].pcrAddress, Info::info[index].gpioBit, Info::info[index].pcrValue>;

/**
 * @}
 ** PeripheralPinTables
 */
} // End namespace USBDM

#endif /* HEADER_PCR_H */

