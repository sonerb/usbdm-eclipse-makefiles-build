/**
 * @file     MMA845x.h
 * @brief    Interface for MMA845x accelerometer
 *
 * @version  V4.11.1.70
 * @date     18 June 2015
 */

#ifndef MMA845X_H_
#define MMA845X_H_

#include <stdint.h>
#include "I2C.h"

/**
 * @addtogroup MMA845x_Group MMA845x 3-axis accelerometer
 * @brief C++ Class providing interface to MMA845x
 * @{
 */

#define MMA845x_CTRL_REG1_ACTIVE_MASK         (1<<0)
#define MMA845x_CTRL_REG1_F_READ_MASK         (1<<1)
#define MMA845x_CTRL_REG1_LNOISE_MASK         (1<<2)
#define MMA845x_CTRL_REG1_DR_OFF              (3)
#define MMA845x_CTRL_REG1_DR_MASK             (0x7<<MMA845x_CTRL_REG1_DR_OFF)
#define MMA845x_CTRL_REG1_DR(x)               (((x)<<MMA845x_CTRL_REG1_DR_OFF)&MMA845x_CTRL_REG1_DR_MASK)
#define MMA845x_CTRL_REG1_ASLP_RATE_OFF       (6)
#define MMA845x_CTRL_REG1_ASLP_RATE_MASK      (0x3F<<MMA845x_CTRL_REG1_ASLP_RATE_OFF)
#define MMA845x_CTRL_REG1_ASLP_RATE(x)        (((x)<<MMA845x_CTRL_REG1_ASLP_RATE_OFF)&MMA845x_CTRL_REG1_ASLP_RATE_MASK)

#define MMA845x_CTRL_REG2_MODS_OFF            (0)
#define MMA845x_CTRL_REG2_MODS_MASK           (0x3<<MMA845x_CTRL_REG1_DR_OFF)
#define MMA845x_CTRL_REG2_MODS(x)             (((x)<<MMA845x_CTRL_REG1_DR_OFF)&MMA845x_CTRL_REG1_DR_MASK)
#define MMA845x_CTRL_REG2_SLPE_MASK           (1<<2)
#define MMA845x_CTRL_REG2_SMODS_OFF           (3)
#define MMA845x_CTRL_REG2_SMODS_MASK          (0x3<<MMA845x_CTRL_REG1_DR_OFF)
#define MMA845x_CTRL_REG2_SMODS(x)            (((x)<<MMA845x_CTRL_REG1_DR_OFF)&MMA845x_CTRL_REG1_DR_MASK)
#define MMA845x_CTRL_REG2_RST_MASK            (1<<6)
#define MMA845x_CTRL_REG2_ST_MASK             (1<<7)

#define MMA845x_CTRL_REG3_PP_OD_MASK          (1<<0)
#define MMA845x_CTRL_REG3_IPOL_MASK           (1<<1)
#define MMA845x_CTRL_REG3_WAKE_A_VECM_MASK    (1<<2)
#define MMA845x_CTRL_REG3_WAKE_FFMT_MASK      (1<<3)
#define MMA845x_CTRL_REG3_WAKE_PULSE_MASK     (1<<4)
#define MMA845x_CTRL_REG3_WAKE_INDPRT_MASK    (1<<5)
#define MMA845x_CTRL_REG3_WAKE_TRANS_MASK     (1<<6)
#define MMA845x_CTRL_REG3_FIFO_GATE_MASK      (1<<7)

#define MMA845x_CTRL_REG4_INT_EN_DRDY_MASK    (1<<0)
#define MMA845x_CTRL_REG4_INT_EN_A_VECM_MASK  (1<<1)
#define MMA845x_CTRL_REG4_INT_EN_EN_FFMT_MASK (1<<2)
#define MMA845x_CTRL_REG4_INT_EN_PULSE_MASK   (1<<3)
#define MMA845x_CTRL_REG4_INT_EN_INDPRT_MASK  (1<<4)
#define MMA845x_CTRL_REG4_INT_EN_TRANS_MASK   (1<<5)
#define MMA845x_CTRL_REG4_INT_EN_FIFO_MASK    (1<<6)
#define MMA845x_CTRL_REG4_INT_EN_ASLP_MASK    (1<<7)

#define MMA845x_XYZ_DATA_CFG_FS_OFF           (0)
#define MMA845x_XYZ_DATA_CFG_FS_MASK          (0x03<<MMA845x_XYZ_DATA_CFG_FS_OFF)
#define MMA845x_XYZ_DATA_CFG_FS(x)            (((x)<<MMA845x_XYZ_DATA_CFG_FS_OFF)&MMA845x_XYZ_DATA_CFG_FS_MASK)
#define MMA845x_XYZ_DATA_CFG_HPF_OUT_MASK     (1<<4)

#define MMA845x_STATUS_ZYXDR_MASK 	(1<<3)


/**
 * @brief Class representing an interface for MMA845x 3-axis accelerometer over I2C
 *
 * <b>Example</b>
 * @code
 *  // Instantiate interface
 *  I2C *i2c = new I2C_0();
 *  MMA845x *accelerometer = new MMA845x(i2c, MMA845x::ACCEL_2Gmode);
 *
 *  uint8_t id = accelerometer->readID();
 *  printf("Device ID = 0x%02X\n", id);
 *
 *  printf("Before simple calibration (make sure the device is level!)\n");
 *  accelerometer->calibrateAccelerometer();
 *
 *  printf("After calibration\n");
 *  for(;;) {
 *     int accelStatus;
 *     int16_t accelX,accelY,accelZ;
 *
 *     accelerometer->readAccelerometerXYZ(&accelStatus, &accelX, &accelY, &accelZ);
 *     printf("s=0x%02X, aX=%10d, aY=%10d, aZ=%10d\n", accelStatus, accelX, accelY, accelZ);
 *     waitMS(400);
 *  }
 *
 * @endcode
 */
class MMA845x {

private:
   I2C *i2c;
   static const uint8_t DEVICE_ADDRESS = 0x1D<<1;  // SA0 pin : 0=>1C, 1=>1D
   static const uint8_t WHO_AM_I_VALUE = 0x1A;

   /**
    * Read Accelerometer register
    *
    * @param regNum  - Register number
    */
   uint8_t readReg(uint8_t regNum);
   /**
    * Write Accelerometer register
    *
    * @param regNum  - Register number
    * @param value   - Value to write
    */
   void    writeReg(uint8_t regNum, uint8_t value);
   /**
    * Reset Accelerometer
    */
   void    reset(void);
   bool    failedInit;

public:

   enum AccelerometerMode {
      ACCEL_2Gmode      = MMA845x_XYZ_DATA_CFG_FS(0),                                  // 2g Full-scale, no high-pass filter
      ACCEL_4Gmode      = MMA845x_XYZ_DATA_CFG_FS(1),                                  // 4g Full-scale, no high-pass filter
      ACCEL_8Gmode      = MMA845x_XYZ_DATA_CFG_FS(2),                                  // 8g Full-scale, no high-pass filter
      ACCEL_2G_HPF_mode = MMA845x_XYZ_DATA_CFG_FS(0)|MMA845x_XYZ_DATA_CFG_HPF_OUT_MASK, // 2g Full-scale, high-pass filter
      ACCEL_4G_HPF_mode = MMA845x_XYZ_DATA_CFG_FS(1)|MMA845x_XYZ_DATA_CFG_HPF_OUT_MASK, // 4g Full-scale, high-pass filter
      ACCEL_8G_HPF_mode = MMA845x_XYZ_DATA_CFG_FS(2)|MMA845x_XYZ_DATA_CFG_HPF_OUT_MASK, // 8g Full-scale, high-pass filter
   } ;

   /**
    * Constructor
    *
    * @param i2c  - The I2C interface to use
    * @param mode - Mode of operation (gain and filtering)
    */
   MMA845x(I2C *i2c, AccelerometerMode mode);
   /**
    * Put accelerometer into Standby mode
    */
   void standby();
   /**
    * Put accelerometer into Active mode
    */
   void active();
   /**
    * Obtains measurements from the accelerometer
    *
    * @param status  - Indicates status of x, y & z measurements
    * @param x       - X axis value
    * @param y       - Y axis value
    * @param z       - Z axis value
    */
   void readAccelerometerXYZ(int *status, int16_t *x, int16_t *y, int16_t *z);
   /**
    * Set accelerometer mode (gain and filtering)
    *
    * @param mode - one of ACCEL_2Gmode etc.
    */
   void setAccelerometerMode(AccelerometerMode mode);
   /**
    * Read ID from accelerometer
    *
    * @return ID value as 8-bit number (0x1A for MMA8451Q)
    */
   uint32_t readID();
   /**
    * Calibrate accelerometer
    * Assumes accelerometer is on a flat surface
    * The accelerometer mode will need to be reset after this operation
    */
   void calibrateAccelerometer();
};

/**
 * @}
 */
#endif /* MMA845X_H_ */
