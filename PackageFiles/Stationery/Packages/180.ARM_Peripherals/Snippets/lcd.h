/****************************************************************************************************//**
 * @file     lcd.h
 *
 * @brief    Abstraction layer for Lcd interface
 *
 * @version  V1.0
 * @date     2015/06
 *
 *******************************************************************************************************/

#ifndef INCLUDES_LCD_H_
#define INCLUDES_LCD_H_
 /*
 * *****************************
 * *** DO NOT EDIT THIS FILE ***
 * *****************************
 *
 * This file is generated automatically.
 * Any manual changes will be lost.
 */
#include <stdint.h>
#ifdef __CMSIS_RTOS
#include "cmsis.h"
#endif

#include "delay.h"
#include "spi.h"
#include "fonts.h"
#include "formatted_io.h"

namespace USBDM {

//-------- <<< Use Configuration Wizard in Context Menu >>> -----------------

// Lcd Options ==============================
//
//   <o> Lcd PWM back-light support
//   <i> Enables the use of PWM to adjust the Lcd back-light
//   <i> This ties up one of the FTMs so is rather expensive to provide.
//     <0=> Back-light on/off only
//     <1=> Back-light adjustable
//     <0=> Default
/**
 * Controls if PWM feature is available for back-light
 */
#define LCD_BACKLIGHT_PWM_FEATURE 0

//------------- <<< end of configuration section >>> -----------------------

/**
 * Indicates the code should be for a ELEC FREAKS version of the Lcd
 */
#define ELEC_FREAKS

/**
 * @addtogroup LCD_Group Elecfreaks Liquid Crystal Display
 * @brief C++ Class allowing access Lcd
 * @{
 */

//********************************************************************
//
//   Lcd Dimension Definitions
//
//********************************************************************
constexpr int ROW_LENGTH       = 132;
constexpr int COL_HEIGHT       = 132;
constexpr int ENDPAGE          = 132;
constexpr int ENDCOL           = 130;

// Usable area
constexpr int LCD_X_MIN        = 1;
constexpr int LCD_X_MAX        = 130;
constexpr int LCD_Y_MIN        = 1;
constexpr int LCD_Y_MAX        = 130;

/**
 * Calculates colour as 16-bit value in 4:4:4 format
 *
 * @param r Red
 * @param g Green
 * @param b Blue
 */
constexpr uint16_t colour444(uint8_t r, uint8_t g, uint8_t b) {
   return ((r&0xF)<<8)|((g&0xF)<<4)|(b&0xF);
}
using Colour = uint16_t;

//*******************************************************
//   12-Bit Colour Definitions
//*******************************************************
constexpr Colour BLACK            = 0x000;
constexpr Colour NAVY             = 0x008;
constexpr Colour BLUE             = 0x00F;
constexpr Colour TEAL             = 0x088;
constexpr Colour EMERALD          = 0x0C5;
constexpr Colour GREEN            = 0x0F0;
constexpr Colour CYAN             = 0x0FF;
constexpr Colour SLATE            = 0x244;
constexpr Colour INDIGO           = 0x408;
constexpr Colour TURQUOISE        = 0x4ED;
constexpr Colour OLIVE            = 0x682;
constexpr Colour MAROON           = 0x800;
constexpr Colour PURPLE           = 0x808;
constexpr Colour GRAY             = 0x888;
constexpr Colour SKYBLUE          = 0x8CE;
constexpr Colour BROWN            = 0xB22;
constexpr Colour CRIMSON          = 0xD13;
constexpr Colour ORCHID           = 0xD7D;
constexpr Colour RED              = 0xF00;
constexpr Colour MAGENTA          = 0xF0F;
//constexpr Colour ORANGE           = 0xF40;
constexpr Colour PINK             = 0xF6A;
constexpr Colour CORAL            = 0xF75;
constexpr Colour SALMON           = 0xF87;
constexpr Colour ORANGE           = 0xFA0;
constexpr Colour GOLD             = 0xFD0;
constexpr Colour YELLOW           = 0xFF0;
constexpr Colour WHITE            = 0xFFF;

//*******************************************************
//       Circle Definitions
//*******************************************************
constexpr int SECTOR_0_45      = 0x01;
constexpr int SECTOR_45_90     = 0x02;
constexpr int SECTOR_90_135    = 0x04;
constexpr int SECTOR_135_180   = 0x08;
constexpr int SECTOR_180_225   = 0x10;
constexpr int SECTOR_225_270   = 0x20;
constexpr int SECTOR_270_315   = 0x40;
constexpr int SECTOR_315_360   = 0x80;

constexpr int QUAD_NORTH_EAST  = (SECTOR_0_45|SECTOR_45_90);
constexpr int QUAD_NORTH_WEST  = (SECTOR_90_135|SECTOR_135_180);
constexpr int QUAD_SOUTH_WEST  = (SECTOR_180_225|SECTOR_225_270);
constexpr int QUAD_SOUTH_EAST  = (SECTOR_270_315|SECTOR_315_360);

constexpr int FULLCIRCLE       = (QUAD_NORTH_EAST|QUAD_NORTH_WEST|QUAD_SOUTH_EAST|QUAD_SOUTH_WEST);
constexpr int OPENSOUTH        = (QUAD_NORTH_EAST|QUAD_NORTH_WEST);
constexpr int OPENNORTH        = (QUAD_SOUTH_EAST|QUAD_SOUTH_WEST);
constexpr int OPENEAST         = (QUAD_NORTH_WEST|QUAD_SOUTH_WEST);
constexpr int OPENWEST         = (QUAD_NORTH_EAST|QUAD_SOUTH_EAST);
constexpr int OPENNORTHEAST    = (QUAD_NORTH_WEST|QUAD_SOUTH_EAST|QUAD_SOUTH_WEST);
constexpr int OPENNORTHWEST    = (QUAD_NORTH_EAST|QUAD_SOUTH_EAST|QUAD_SOUTH_WEST);
constexpr int OPENSOUTHEAST    = (QUAD_NORTH_EAST|QUAD_NORTH_WEST|QUAD_SOUTH_WEST);
constexpr int OPENSOUTHWEST    = (QUAD_NORTH_EAST|QUAD_NORTH_WEST|QUAD_SOUTH_EAST);

constexpr int DEFAULT_BACKGROUND = BLACK;
constexpr int DEFAULT_FOREGROUND = WHITE;

constexpr int DEFAULT_LCD_CONTRAST = 65;

/**
 * @brief Class representing an LCD
 *
 * <b>Example</b>
 * @code
 * // Instantiate interface
 *	Lcd *lcd = new Lcd(new SPI_0());
 *
 * lcd.clear(RED);
 * lcd.drawCircle(65, 65, 20, WHITE);
 * lcd.drawCircle(65, 65, 30, WHITE);
 * lcd.drawCircle(65, 65, 40, WHITE);
 * lcd.putStr("Some Circles", 30, 10, Fonts::FontSmall, WHITE, RED);
 *
 *  @endcode
 */
class LcdBase : public FormattedIO {

protected:
   /** SPI interface used to communicate with LCD */
   Spi      &spi;

   /** SPI communication configuration */
   SpiConfig  spiConfig;

#ifdef __CMSIS_RTOS
   /** Mutex to protect access to LCD */
   CMSIS::Mutex mutex;

   void lock() {
      mutex.lock();
   }
   void unlock() {
      mutex.unlock();
   }
#else
   void lock() {
   }
   void unlock() {
   }
#endif

   static constexpr uint16_t  DATA_FLAG = 0x100;

   /**
    * Reset LCD
    */
   virtual void reset() = 0;

   /**
    * Send a single command byte to the display
    *
    * @param command Command byte to send
    */
   virtual void txCommand(uint8_t command) = 0;

   /**
    * Send a single data byte to the display
    *
    * @param data Data byte to send
    */
   virtual void txData(uint8_t data) = 0;

   /**
    * Send two pixels of a single colour to the display
    *
    * @param colour Colour to send
    */
   virtual void txTwoPixels(Colour colour) = 0;

   /**
    * Send a single colour block to the display
    *
    * @param colour Colour to send
    * @param size   Size of colour block
    */
   virtual void txColourBlock(Colour colour, int size) = 0;

   /** Sets the Row and Column addresses
    *
    * @param x Row address (0 .. 131)
    * @param y Column address (0 .. 131)
    */
   void setXY(int x, int y);

   /**
    *  Initialises the LCD
    */
   void init();

public:
   /**
    *  Constructor
    *
    *  @param spi   SPI interface to use to communicate with LCD
    */
   LcdBase(Spi &spi) : spi(spi) {
   }

   virtual ~LcdBase() {
   }
#ifdef ELEC_FREAKS
   /**
    * Set backlight level
    *
    * @param  level  Level back-light level as percentage
    *
    * @note     Requires @ref LCD_BACKLIGHT_PWM_FEATURE to be fully implemented.\n
    * Otherwise it falls back to basic on/off
    *
    * Note : Only of elecfreaks version of shield
    */
   virtual void backlightSetLevel(int level) = 0;
   /**
    * Turn LCD back-light on
    */
   void backlightOn(void) { backlightSetLevel(100); }
   /**
    * Turn LCD back-light off
    */
   void backLightOff(void) { backlightSetLevel(0); }
#endif

   /** Set LCD contrast - range varies with device
    *
    *  @param setting Contrast level (0..127) ?
    */
   virtual void setContrast(uint8_t setting);

   /** This function will clear the screen to the given colour.
    *
    * @param colour   12-bit colour value rrrrggggbbbb
    */
   void clear(Colour colour=DEFAULT_BACKGROUND);

   /** Lights a single pixel in the specified colour at the specified x and y addresses
    *
    * @param  x      Row address (0 .. 131)
    * @param  y      Column address (0 .. 131)
    * @param  colour 12-bit colour value rrrrggggbbbb
    *
    * @note See lcd.h for some sample colour settings
    */
   void drawPixel(int x, int y, Colour colour=DEFAULT_FOREGROUND);

   /** Draws a line in the specified colour from (x0,y0) to (x1,y1)
    *
    * @param  x0      Row address (0 .. 131)
    * @param  y0      Column address (0 .. 131)
    * @param  x1      Row address (0 .. 131)
    * @param  y1      Column address (0 .. 131)
    * @param  colour  12-bit colour value rrrrggggbbbb
    */
   void drawLine(int x0, int y0, int x1, int y1, Colour colour=DEFAULT_FOREGROUND);

   /** Draws a rectangle in the specified colour from (x1,y1) to (x2,y2)\n
    *  Rectangle can be filled with a colour if desired
    *
    * @param  x0      Row address (0 .. 131)
    * @param  y0      Column address (0 .. 131)
    * @param  x1      Row address (0 .. 131)
    * @param  y1      Column address (0 .. 131)
    * @param  fill    Indicates if the rectangle will be filled
    * @param  colour  12-bit colour value rrrrggggbbbb
    */
   void drawRect(int x0, int y0, int x1, int y1, int fill, Colour colour=DEFAULT_FOREGROUND);

   /** Draws a line circle in the specified colour at center (x0,y0) with radius
    *
    * @param centreX    Row address (0 .. 131)
    * @param centreY    Column address (0 .. 131)
    * @param radius     Radius in pixels
    * @param colour     12-bit colour value rrrrggggbbbb
    * @param circleType Controls which segments of the circle are drawn
    */
   void drawCircle(int centreX, int centreY, int radius, Colour colour, int circleType=FULLCIRCLE);
   /** Writes the entire LCD screen from a bmp file
    *
    * @param bmp - bitmap to display
    *
    * @author Olimex, James P Lynch July 7, 2007
    *
    * @note Use Olimex BmpToArray.exe utility to create bitmap
    */
   void drawBitmap(uint8_t bmp[131*131]);
   /** Draws an ASCII character at the specified (x,y) address and colour
    *
    * @param c         Character to be displayed
    * @param x         Row address (0 .. 131)
    * @param y         Column address (0 .. 131)
    * @param font      Font to use
    * @param fColour   12-bit foreground colour value
    * @param bColour   12-bit background colour value
    */
   void putChar(char c, int x, int y, Font &font=smallFont, Colour fColour=DEFAULT_FOREGROUND, Colour bColour=DEFAULT_BACKGROUND);

   /** Draws a null-terminated character string at the specified (x,y) address and colour
    *
    * @param str      Pointer to character string to be displayed
    * @param x        Row address (0 .. 131)
    * @param y        Column address (0 .. 131)
    * @param font     Font pitch (Lcd::FontSmall, Lcd::FontMedium, Lcd::FontLarge)
    * @param fColour  12-bit foreground colour value rrrrggggbbbb
    * @param bColour  12-bit background colour value rrrrggggbbbb
    *
    * @code{.c}
    *       // Here's an example to display "Hello World!" at address (20,20)
    *
    *       lcd_putStr("Hello World!", 20, 20, WHITE, BLACK);
    * @endcode
    *
    * @author James P Lynch, August 30, 2007 \n
    *         Edited by Peter Davenport on August 23, 2010
    *
    * @note For more information on how this code does it's thing look at this \n
    *       "http://www.sparkfun.com/tutorial/Nokia%206100%20LCD%20Display%20Driver.pdf"
    */
   void putStr(const char *str, int x, int y, Font &font=smallFont, Colour fColour=DEFAULT_FOREGROUND, Colour bColour=DEFAULT_BACKGROUND);

   int _readChar() override { return -1;}
   bool _isCharAvailable() override { return false; }
   void flushInput() override {}
   void flushOutput() override {}

private:
   int fX,fY;
   Font *fFont;
   Colour fForeground;
   Colour fBackground;

public:
   LcdBase &setForeground(Colour foreground) {
      fForeground = foreground;
      return *this;
   }

   LcdBase &setBackground(Colour background) {
      fBackground = background;
      return *this;
   }

   LcdBase &moveXY(int x, int y) {
      fX = x;
      fY = y;
      return *this;
   }

   LcdBase &setFont(Font &font) {
      fFont = &font;
      return *this;
   }

   void _writeChar(char ch) override {
      putChar(ch, fX, fY, *fFont, fForeground, fBackground);
      fX += fFont->width;
      if (fX >= LCD_X_MAX) {
         fX = LCD_X_MIN;
         fY += fFont->height;
         if (fY >= LCD_Y_MAX) {
            fY = LCD_Y_MIN;
         }
      }
   }
};

/**
 * Template for LCD
 *
 * tparam SpiCS_n   Display CS     (D9 on Arduino)
 * tparam Reset_n   Display reset  (D8 on Arduino)
 * tparam BackLight Display back-light control (may be PWM) (D10 on Arduino)
 */
template<typename SpiCS_n, typename Reset_n, typename BackLight>
class Lcd_T : public LcdBase {

public:
   /**
    *  Constructor
    *
    *  Initialises the LCD
    *
    *  @param spi The SPI interface to use to communicate with LCD
    */
   Lcd_T(Spi &spi) : LcdBase(spi) {
      // Chip select pin
      SpiCS_n::setOutput(pcrValue(PinPull_None, PinDriveStrength_High));
      SpiCS_n::high();         // Set idle high

      // LCD Reset pin
      Reset_n::setOutput(pcrValue(PinPull_None, PinDriveStrength_High));
      Reset_n::high();

      init();
   }

   /**
    * Reset LCD
    */
   virtual void reset()override  {
      // Reset display
//      SpiCS_n::low();
      Reset_n::low();
      waitMS(2);
      Reset_n::high();
      SpiCS_n::high();
      waitMS(20);
   }
   /**
    * Send a single command byte to the display
    *
    * @param command Command byte to send
    */
   virtual void txCommand(uint8_t command) override {
      spi.startTransaction(spiConfig);
      SpiCS_n::low();
      spi.txRx(command);
      SpiCS_n::high();
      spi.endTransaction();
   }

   /**
    * Send a single data byte to the display
    *
    * @param data Data byte to send
    */
   virtual void txData(uint8_t data) override {
      spi.startTransaction(spiConfig);
      SpiCS_n::low();
      spi.txRx(DATA_FLAG|data);
      SpiCS_n::high();
      spi.endTransaction();
   }

   /**
    * Send two pixels of a single colour to the display
    *
    * @param colour Colour to send
    */
   virtual void txTwoPixels(Colour colour) override {
      // Construct RRRRGGGG|BBBBRRRR|GGGGBBBB in 3 bytes = 2 pixels
      uint16_t data1 = DATA_FLAG|(colour >> 4);
      uint16_t data2 = DATA_FLAG|(((colour & 0xF) << 4) | ((colour >> 8) & 0xF));
      uint16_t data3 = DATA_FLAG|(colour & 0xFF);

      spi.startTransaction(spiConfig);
      SpiCS_n::low();
      spi.txRx(data1);
      spi.txRx(data2);
      spi.txRx(data3);
      SpiCS_n::high();
      spi.endTransaction();
   }

   /**
    * Send a single colour multiple times to the display (multiple of 2 pixels)
    *
    * @param colour Colour to send
    * @param size   Size of colour block (even)
    */
   virtual void txColourBlock(Colour colour, int size) override {
      // Construct RRRRGGGG|BBBBRRRR|GGGGBBBB in 3 bytes = 2 pixels
      uint16_t data1 = DATA_FLAG|(colour >> 4);
      uint16_t data2 = DATA_FLAG|(((colour & 0xF) << 4) | ((colour >> 8) & 0xF));
      uint16_t data3 = DATA_FLAG|(colour & 0xFF);

      spi.startTransaction(spiConfig);
      SpiCS_n::low();
      size = (size+1)/2;
      while(size-->0) {
         spi.txRx(data1);
         spi.txRx(data2);
         spi.txRx(data3);
      }
      SpiCS_n::high();
      spi.endTransaction();
   }

#ifdef ELEC_FREAKS
   /**
    * Set back-light level
    *
    * @param level 0-100 back-light level as percentage
    */
   virtual void backlightSetLevel(int level) {
#if LCD_BACKLIGHT_PWM_FEATURE
      BackLight::setMode(1000, tmr_leftAlign);
      if (level>100) {
         level = 100;
      }
      if (level<0) {
         level = 0;
      }
      BackLight::setDutyCycle(level);
#else
      BackLight::setOutput(pcrValue(PinPull_None, PinDriveStrength_High));
      BackLight::write(level>0);
#endif
   }
#endif

};

/**
 * @brief Class representing an LCD\n
 * Convenience type for typical Arduino shield LCD
 *
 * <b>Example</b>
 * @code
 * // Instantiate interface
 * Lcd *lcd = new Lcd(new SPI_0());
 *
 * lcd.clear(RED);
 * lcd.drawCircle(65, 65, 20, WHITE);
 * lcd.drawCircle(65, 65, 30, WHITE);
 * lcd.drawCircle(65, 65, 40, WHITE);
 * lcd.putStr("Some Circles", 30, 10, Fonts::FontSmall, WHITE, RED);
 *
 *  @endcode
 */
#if LCD_BACKLIGHT_PWM_FEATURE
// PWM control for back-light brightness
using Lcd = Lcd_T<USBDM::gpio_D9, USBDM::gpio_D8, USBDM::ftm_D10>;
#else
// On/Off control of back-light
using Lcd = Lcd_T<USBDM::$(demo.lcd.elecfreaks.cs:gpio_D9), USBDM::$(demo.lcd.elecfreaks.reset:gpio_D8), USBDM::$(demo.lcd.elecfreaks.backlight:gpio_D10)>;
#endif

/**
 * @}
 */

} // End namespace USBDM

#endif /* INCLUDES_LCD_H_ */
