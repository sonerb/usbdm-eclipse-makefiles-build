/**
 * @file     mma8491q.h
 * @brief    Interface for MMA8491Q accelerometer
 *
 * @version  V4.11.1.70
 * @date     18 June 2015
 */

#ifndef INCLUDE_USBDM_MMA8491Q_H_
#define INCLUDE_USBDM_MMA8491Q_H_

#include <stdint.h>
#include "i2c.h"

namespace USBDM {

/**
 * @addtogroup MMA8491Q_Group MMA8491Q 3-axis accelerometer
 * @brief C++ Class providing interface to MMA8491Q
 * @{
 */

/**
 * @brief Class representing an interface for MMA8491Q 3-axis accelerometer over I2C
 *
 * <b>Example</b>
 * @code
 *  // Instantiate interface
 *  I2c *i2c = new I2C_0();
 *  MMA8491Q *accelerometer = new MMA8491Q(i2c, MMA8491Q::ACCEL_2Gmode);
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
class MMA8491Q {
protected:
   // Accelerometer registers
   enum {
      STATUS,
      OUT_X_MSB,
      OUT_X_LSB,
      OUT_Y_MSB,
      OUT_Y_LSB,
      OUT_Z_MSB,
      OUT_Z_LSB,
   };

   static const uint8_t DEVICE_ADDRESS = 0x55<<1;

   USBDM::I2c *i2c;

   int16_t offsetX;
   int16_t offsetY;
   int16_t offsetZ;

   /**
    * Read Accelerometer register
    *
    * @param regNum  - Register number
    */
   uint8_t readReg(uint8_t regNum) {
      uint8_t command[1];
      i2c->txRx(DEVICE_ADDRESS, 1, &regNum, sizeof(command), command);
      return command[0];
   }
   /**
    * Write Accelerometer register
    *
    * @param regNum  - Register number
    * @param value   - Value to write
    */
   void writeReg(uint8_t regNum, uint8_t value) {
      uint8_t command[] = {regNum, value};
      i2c->transmit(DEVICE_ADDRESS, sizeof(command), command);
   }
   /**
    * Reset Accelerometer
    */
   void reset(void);

public:

   /**
    * Constructor
    *
    * @param i2c    - The I2C interface to use
    */
   MMA8491Q(USBDM::I2c *i2c) : i2c(i2c), offsetX(0), offsetY(0), offsetZ(0) {
   }

   /**
    * Put accelerometer into Standby mode
    */
   virtual void standby() = 0;
   /**
    * Put accelerometer into Active mode
    */
   virtual void active() = 0;
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
    */
   void calibrateAccelerometer();
};

template<class EnableGpio> class MMA8491QT : public MMA8491Q {
private:
   // Accelerometer registers
   enum {
      STATUS,
      OUT_X_MSB,
      OUT_X_LSB,
      OUT_Y_MSB,
      OUT_Y_LSB,
      OUT_Z_MSB,
      OUT_Z_LSB,
   };

public:
   EnableGpio enableGpio;

   /**
    * Constructor
    *
    * @param i2c    - The I2C interface to use
    */
   MMA8491QT(USBDM::I2c *i2c) : MMA8491Q(i2c) {
      enableGpio.setOutput();
   }

   /**
    * Put accelerometer into Standby mode
    */
   void standby() {
      enableGpio.clear();
   }
   /**
    * Put accelerometer into Active mode
    */
   void active() {
      enableGpio.set();
   }
};

/**
 * @}
 */

} // End namespace USBDM

#endif /* INCLUDE_USBDM_MMA8491Q_H_ */
