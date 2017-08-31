/**
 * @file     smc.h (180.ARM_Peripherals/Project_Headers/smc.h)
 * @brief    System Management Controller
 *
 * @version  V4.12.1.80
 * @date     13 April 2016
 */
#ifndef SMC_H_
#define SMC_H_
/*
 * *****************************
 * *** DO NOT EDIT THIS FILE ***
 * *****************************
 *
 * This file is generated automatically.
 * Any manual changes will be lost.
 */
#include "hardware.h"

namespace USBDM {

/**
 * @addtogroup SMC_Group SMC, System Mode Controller
 * @brief Abstraction for System Mode Controller
 * @{
 */

/**
 * Determines if VLPR, VLPW, and VLPS modes are enabled
 */
enum SmcVeryLowPower {
   SmcVeryLowPower_Disable  = SMC_PMPROT_AVLP(0),   //!< Disallow VLPR, VLPW, and VLPS modes
   SmcVeryLowPower_Enable   = SMC_PMPROT_AVLP(1),   //!< Allow VLPR, VLPW, and VLPS modes
};

/**
 * Determines if any LLSx modes are enabled
 */
enum SmcLowLeakageStop {
   SmcLowLeakageStop_Disable  = SMC_PMPROT_ALLS(0),   //!< Disallow Any LLSx mode
   SmcLowLeakageStop_Enable   = SMC_PMPROT_ALLS(1),   //!< Allow Any LLSx mode
};

/**
 * Determines if any VLLSx modes are enabled
 */
enum SmcVeryLowLeakageStop {
   SmcVeryLowLeakageStop_Disable  = SMC_PMPROT_AVLLS(0),   //!< Disallow Any VLLSx mode
   SmcVeryLowLeakageStop_Enable   = SMC_PMPROT_AVLLS(1),   //!< Allow Any VLLSx mode
};

/**
 * Determines if HSRUN mode is enabled
 *
 * @note Not supported on all processors
 */
enum SmcHighSpeedRun {
#ifdef SMC_PMPROT_AHSRUN
   SmcHighSpeedRun_Disable  = SMC_PMPROT_AHSRUN(0),   //!< Disallow HSRUN mode
   SmcHighSpeedRun_Enable   = SMC_PMPROT_AHSRUN(1),   //!< Allow HSRUN mode
#else
   SmcHighSpeedRun_Disable  = 0, //!< HSRUN Not supported
#endif
};

/**
 * Whether to exit VLP to Run mode on interrupt
 *
 * @note Not supported on all processors
 */
enum SmcExitVeryLowPowerOnInt {
#ifdef SMC_PMCTRL_LPWUI
   SmcExitVeryLowPowerOnInt_Disable = SMC_PMCTRL_LPWUI(0),  //!< Remain in VLP mode on interrupt
   SmcExitVeryLowPowerOnInt_Enable  = SMC_PMCTRL_LPWUI(1),  //!< Exit to RUN mode on interrupt
#else
   SmcExitVeryLowPowerOnInt_Disable = 0, //!< LPWUI not supported
#endif
};

/**
 * Sets Run mode
 *
 * @note Not all modes are supported on all processors
 */
enum SmcRunMode {
   SmcRunMode_Normal       = SMC_PMCTRL_RUNM(0),   //!< Normal run mode (RUN)
   SmcRunMode_VeryLowPower = SMC_PMCTRL_RUNM(2),   //!< Very low power run mode (VLPR)
#ifdef SMC_PMPROT_AHSRUN
   SmcRunMode_HighSpeed    = SMC_PMCTRL_RUNM(3),   //!< High Speed Run mode (HSRUN)
#endif
};

/**
 * Sets Stop mode
 */
enum SmcStopMode {
   SmcStopMode_NormalStop         = SMC_PMCTRL_STOPM(0), //!< Normal Stop (STOP)
   SmcStopMode_VeryLowPowerStop   = SMC_PMCTRL_STOPM(2), //!< Very-Low-Power Stop (VLPS)
   SmcStopMode_LowLeakageStop     = SMC_PMCTRL_STOPM(3), //!< Low-Leakage Stop (LLSx)
   SmcStopMode_VeryLowLeakageStop = SMC_PMCTRL_STOPM(4), //!< Very-Low-Leakage Stop (VLLSx)
};

/**
 *  Partial Stop Option\n
 *  This field controls whether a Partial Stop mode is entered when STOPM=STOP\n
 *
 * @note Not supported on all processors
 */
enum SmcPartialStopMode {
#ifdef SMC_STOPCTRL_PSTOPO
   SmcPartialStopMode_Normal   = SMC_STOPCTRL_PSTOPO(0), //!< Normal stop mode
   SmcPartialStopMode_Partial1 = SMC_STOPCTRL_PSTOPO(1), //!< Partial Stop with both system and bus clocks disabled
   SmcPartialStopMode_Partial2 = SMC_STOPCTRL_PSTOPO(2), //!< Partial Stop with system clock disabled and bus clock enabled
#else
   SmcPartialStopMode_Normal   = 0, //!< Partial STOP options not supported
#endif
};

/**
 *  POR Power Option\n
 *  This bit controls whether the POR detect circuit is enabled in VLLS0 mode.
 */
enum SmcPowerOnReset {
   SmcPowerOnReset_Disable = SMC_STOPCTRL_PORPO(0),   //!< Power on reset in STOP disabled
   SmcPowerOnReset_Enable  = SMC_STOPCTRL_PORPO(1),   //!< Power on reset in STOP enabled
};

/**
 *  POR Power Option\n
 *  This bit controls whether the POR detect circuit is enabled in VLLS0 mode.
 */
enum SmcSleepOnExit {
   SmcSleepOnExit_Disable = false,  //!< Processor does not enter SLEEP/DEEPSLEEP mode
   SmcSleepOnExit_Enable  = true,   //!< Processor enters SLEEP/DEEPSLEEP mode on completion of interrupts
};

/**
 *  VLS or VLLS Mode Control\n
 *  This field controls which LLS/VLLS sub-mode to enter if STOPM=LLS/VLLS\n
 *
 * @note Not all modes are supported on all processors
 */
enum SmcLowLeakageStopMode {
#ifdef SMC_STOPCTRL_VLLSM
   SmcLowLeakageStopMode_VLLS0 = SMC_STOPCTRL_VLLSM(0),  //!< Enter VLLS0 in VLLSx mode
   SmcLowLeakageStopMode_VLLS1 = SMC_STOPCTRL_VLLSM(1),  //!< Enter VLLS1 in VLLSx mode
   SmcLowLeakageStopMode_VLLS2 = SMC_STOPCTRL_VLLSM(2),  //!< Enter VLLS2 in VLLSx mode, LLS2 in LLSx mode
   SmcLowLeakageStopMode_VLLS3 = SMC_STOPCTRL_VLLSM(3),  //!< Enter VLLS3 in VLLSx mode, LLS3 in LLSx mode
#else
   SmcLowLeakageStopMode_VLLS0 = (0),  //!< Not supported
   SmcLowLeakageStopMode_VLLS1 = (0),  //!< Not supported
   SmcLowLeakageStopMode_VLLS2 = (0),  //!< Not supported
   SmcLowLeakageStopMode_VLLS3 = (0),  //!< Not supported
#endif
   SmcLowLeakageStopMode_LLS2  = SMC_STOPCTRL_LLSM(2),   //!< Enter VLLS2 in VLLSx mode, LLS2 in LLSx mode
   SmcLowLeakageStopMode_LLS3  = SMC_STOPCTRL_LLSM(3),   //!< Enter VLLS3 in VLLSx mode, LLS3 in LLSx mode
};

/**
 *  Indicates the current stop mode
 */
enum SmcStatus {
   // Run modes
   SmcStatus_hsrun  = SMC_PMSTAT_PMSTAT(1<<7),    //!< Processor is in High Speed Run mode
   SmcStatus_run    = SMC_PMSTAT_PMSTAT(1<<0),    //!< Processor is in Normal Run mode
   SmcStatus_vlpr   = SMC_PMSTAT_PMSTAT(1<<2),    //!< Processor is in Very Low Power Run mode

   // Sleep = Wait modes
   SmcStatus_vlpw   = SMC_PMSTAT_PMSTAT(1<<3),    //!< Processor is in Very Low Power Wait mode

   // Stop = DeepSleep modes
   SmcStatus_stop   = SMC_PMSTAT_PMSTAT(1<<1),    //!< Processor is in Stop mode
   SmcStatus_vlps   = SMC_PMSTAT_PMSTAT(1<<4),    //!< Processor is in Very Low Power Stop mode
   SmcStatus_lls    = SMC_PMSTAT_PMSTAT(1<<5),    //!< Processor is in Low Leakage Stop mode
   SmcStatus_vlls   = SMC_PMSTAT_PMSTAT(1<<6),    //!< Processor is in Very Low Leakage Stop mode
};

/**
 *  POR Power Option\n
 *  This bit controls whether the POR detect circuit is enabled in VLLS0 mode.
 */
enum SmcPowerOption {
#ifdef SMC_VLLSCTRL_PORPO
   SmcPowerOption_Disable = SMC_VLLSCTRL_PORPO(0),   //!< Disable POR detect in VLLS0
   SmcPowerOption_Enable  = SMC_VLLSCTRL_PORPO(1),   //!< Enable POR detect in VLLS0
#else
   SmcPowerOption_Disable = (0),   //!< Not supported
   SmcPowerOption_Enable  = (0),   //!< Not supported
#endif
};

/**
 * @brief Template class representing the System Mode Controller (SMC)
 *
 * Partially based on Freescale Application note AN4503\n
 * Support for Kinetis Low Power operation.
 *
 * @image html KinetisPowerModes.png
 */
template <class Info>
class SmcBase_T {

protected:
   static constexpr volatile SMC_Type *smc = Info::smc;

public:

   /**
    * Configure with settings from <b>Configure.usbdmProject</b>.
    */
   static void defaultConfigure() {
      smc->PMPROT   = Info::pmprot;
      smc->STOPCTRL = Info::stopctrl;
   }

   /**
    * Enable the given power modes.
    * A mode must be enabled before it can be entered.
    *
    * @param[in] smcVeryLowPower        Allows VLPR, VLPW, and VLPS modes
    * @param[in] smcLowLeakageStop      Allows LLSx modes
    * @param[in] smcVeryLowLeakageStop  Allows VLLSx modes
    * @param[in] smcHighSpeedRun        Allows HSRUN mode (if supported)
    *
    * @note This is a write-once-after-reset operation
    */
   static ErrorCode enablePowerModes(
         SmcVeryLowPower         smcVeryLowPower,
         SmcLowLeakageStop       smcLowLeakageStop       = SmcLowLeakageStop_Disable,
         SmcVeryLowLeakageStop   smcVeryLowLeakageStop   = SmcVeryLowLeakageStop_Disable,
         SmcHighSpeedRun         smcHighSpeedRun         = SmcHighSpeedRun_Disable ) {

      uint8_t mask = smcVeryLowPower|smcLowLeakageStop|smcVeryLowLeakageStop|smcHighSpeedRun;
      smc->PMPROT = mask;
      return E_NO_ERROR;
   }

   /**
    * Allows the detailed operation in STOP mode to be controlled.
    *
    * @param[in] smcLowLeakageStopMode  Controls which LLS/VLLS sub-mode to enter if STOPM=LLS/VLLS
    * @param[in] smcPowerOnReset        Controls whether the POR detect circuit is enabled in VLLS0 mode
    * @param[in] smcPartialStopMode     Controls whether a Partial Stop mode is entered when STOPM=STOP (is supported)
    */
   static void setStopOptions(
         SmcLowLeakageStopMode   smcLowLeakageStopMode,
         SmcPowerOnReset         smcPowerOnReset,
         SmcPartialStopMode      smcPartialStopMode=SmcPartialStopMode_Normal) {

      smc->STOPCTRL = smcPartialStopMode|smcPowerOnReset|smcLowLeakageStopMode;
   }

   /**
    * Get current power status
    *
    * @return SmcStatus value indicating operating mode
    */
   static SmcStatus getPowerStatus() {

      return (SmcStatus)(smc->PMSTAT);
   }

   /**
    * Enter Run Mode.
    *
    * This may be used to change between supported RUN modes (RUN, VLPR, HSRUN).\n
    * Only the following transitions are allowed: VLPR <-> RUN <-> HSRUN.
    *
    * @param[in] smcRunMode  Mode to enter
    *
    * @return E_NO_ERROR                 No error
    * @return E_ILLEGAL_POWER_TRANSITION Cannot transition to smcRunMode from current run mode
    */
   static ErrorCode enterRunMode(SmcRunMode smcRunMode) {
#ifdef SMC_PMPROT_AHSRUN
      SmcStatus smcStatus = getPowerStatus();
#endif
      switch(smcRunMode) {
         case SmcRunMode_Normal:
            smc->PMCTRL = (smc->PMCTRL&~SMC_PMCTRL_RUNM_MASK)|smcRunMode;
            while (getPowerStatus() != SmcStatus_run) {
               __asm__("nop");
            }
            break;
#ifdef SMC_PMPROT_AHSRUN
         case SmcRunMode_HighSpeed:
            if (smcStatus != SmcStatus_run) {
               // Can only transition from RUN mode
               return setErrorCode(E_ILLEGAL_POWER_TRANSITION);
            }
            smc->PMCTRL = (smc->PMCTRL&~SMC_PMCTRL_RUNM_MASK)|smcRunMode;
            while (getPowerStatus() != SmcStatus_hsrun) {
               __asm__("nop");
            }
            break;
#endif
         case SmcRunMode_VeryLowPower:
#ifdef SMC_PMPROT_AHSRUN
            if (smcStatus != SmcStatus_run) {
               // Can only transition from RUN mode
               return setErrorCode(E_ILLEGAL_POWER_TRANSITION);
            }
#endif
            smc->PMCTRL = (smc->PMCTRL&~SMC_PMCTRL_RUNM_MASK)|smcRunMode;
            while (getPowerStatus() != SmcStatus_vlpr) {
               __asm__("nop");
            }
            break;
         default:
            return setErrorCode(E_ILLEGAL_PARAM);
      }
      return E_NO_ERROR;
   }

   /**
    * Set Stop Mode to enter on Sleep-Now or Sleep-On-Exit
    *
    * @param[in]  smcStopMode Stop mode to set
    */
   static void setStopMode(SmcStopMode smcStopMode) {
      smc->PMCTRL = (smc->PMCTRL&~SMC_PMCTRL_STOPM_MASK)|smcStopMode;
      // Make sure write completes
      __DSB();
   }

   /**
    * Enter Stop Mode (STOP, VLPS, LLSx, VLLSx)
    * (ARM core DEEPSLEEP mode)
    *
    * The processor will stop execution and enter the current STOP mode.\n
    * Peripherals affected will depend on the stop mode selected.\n
    * The stop mode to enter may be set by setStopMode().
    */
   static void enterStopMode() {
      SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;
      // Make sure write completes
      __DSB();
      __WFI();
   }

   /**
    * Enter Stop Mode (STOP, VLPS, LLSx, VLLSx)
    * (ARM core DEEPSLEEP mode)
    *
    * The processor will stop execution and enter the given STOP mode.\n
    * Peripherals affected will depend on the stop mode selected.
    *
    * @param[in] smcStopMode Stop mode to set
    */
   static void enterStopMode(SmcStopMode smcStopMode) {
      setStopMode(smcStopMode);
      enterStopMode();
   }

   /**
    * Enter Deep Sleep mode
    *
    * See enterStopMode();
    */
   static void deepSleep() {
      enterStopMode();
   }

   /**
    * Enter Wait Mode (WAIT, VLPW)\n
    * (ARM core SLEEP mode)
    *
    * The processor will stop execution and enter SLEEP mode.\n
    * In this mode the core clock is disabled (no code executing),
    * but bus clocks are enabled (peripheral modules are operational).
    *
    * This function can be used to enter normal WAIT mode or VLPW mode
    * depending upon current run mode.
    *
    * Mode transitions:
    * - RUN  -> WAIT
    * - VLPR -> VLPW
    *
    * WAIT mode is exited using any enabled interrupt or RESET.
    *
    * For Kinetis K:
    * If in VLPW mode, the statue of the SMC_PMCTRL[LPWUI] bit
    * determines if the processor exits to VLPR or RUN mode.\n
    * Use setExitVeryLowPowerOnInterrupt() to modify this action.
    *
    * For Kinetis L:
    * LPWUI does not exist.\n
    * Exits with an interrupt from VLPW will always be back to VLPR.\n
    * Exits from an interrupt from WAIT will always be back to RUN.
    *
    * @note Some modules include a programmable option to disable them in wait mode.\n
    * If those modules are programmed to disable in wait mode, they will not be able to
    * generate interrupts to wake the core.
    */
   static void enterWaitMode() {
      SCB->SCR &= ~SCB_SCR_SLEEPDEEP_Msk;
      // Make sure write completes
      __DSB();
      __WFI();
   }

   /**
    * Enter SLEEP mode
    *
    * See enterWaitMode();
    */
   static void sleep() {
      enterWaitMode();
   }

#ifdef SMC_PMCTRL_LPWUI_MASK
   /**
    * Select VLP action on interrupt when in VLP modes (VLPR, VLPW or VLPS).
    *
    * @param[in]  smcExitVeryLowPowerOnInt Whether to exit from any VLP mode to RUN mode on interrupt
    */
   static void setExitVeryLowPowerOnInterrupt(SmcExitVeryLowPowerOnInt smcExitVeryLowPowerOnInt) {
      if (smcExitVeryLowPowerOnInt) {
         smc->PMCTRL |= SMC_PMCTRL_LPWUI_MASK;
      }
      else {
         smc->PMCTRL &= ~SMC_PMCTRL_LPWUI_MASK;
      }
      // Make sure write completes
      (void)smc->PMCTRL;
   }
#else
   /**
    * Set action on interrupt when in VLP modes (VLPR, VLPW or VLPS).
    *
    * @note Not supported
    */
   static void setExitVeryLowPowerOnInterrupt(SmcExitVeryLowPowerOnInt) {
   }

#endif

   /**
    * Set Sleep-on-exit action
    *
    * If enabled, when the processor completes the execution of all exception handlers it
    * returns to Thread mode and immediately enters WAIT/STOP mode (ARM core SLEEP/DEEPSLEEP mode).\n
    * Use this mechanism in applications that only require the processor to run when
    * an exception occurs.
    *
    * @param[in] smcSleepOnExit Determines action on completion of all exception handlers
    */
   static void setSleepOnExit(SmcSleepOnExit smcSleepOnExit=SmcSleepOnExit_Enable) {
      if (smcSleepOnExit) {
         SCB->SCR |= SCB_SCR_SLEEPONEXIT_Msk;
      }
      else {
         SCB->SCR &= ~SCB_SCR_SLEEPONEXIT_Msk;
      }
      // Make sure write completes
      __DSB();
   }
};

#ifdef USBDM_SMC_IS_DEFINED
/**
 * Class representing SMC
 */
using Smc = SmcBase_T<SmcInfo>;

#endif

/**
 * @}
 */

} // End namespace USBDM

#endif /* SMC_H_ */
