/****************************************************************************************************//**
 * @file     formatted_io.h (180.ARM_Peripherals/Project_Headers/formatted_io.h)
 * @brief    Formatted I/O
 *
 * @version  V0.0
 * @date     2015/06
 *
 *******************************************************************************************************/

#ifndef INCLUDE_USBDM_FORMATTED_IO_H_
#define INCLUDE_USBDM_FORMATTED_IO_H_
/*
 * *****************************
 * *** DO NOT EDIT THIS FILE ***
 * *****************************
 *
 * This file is generated automatically.
 * Any manual changes will be lost.
 */
#include <stdint.h>
#include <cstdio>       // snprintf()
#include <ctype.h>      // isspace() etc
#include "hardware.h"

namespace USBDM {

/**
 * @addtogroup FORMATTED_IO Formatted Input/Output
 * @brief C++ Class allowing input and output of basic types as character streams
 * @{
 */

/**
 * Enumeration selecting radix for integer types with << or >> operators
 */
enum Radix : uint8_t {
   Radix_2       = 2,         //!< Convert as binary number
   Radix_8       = 8,         //!< Convert as octal number
   Radix_10      = 10,        //!< Convert as decimal number
   Radix_16      = 16,        //!< Convert as hexadecimal number
   Radix_Default = Radix_10,  //!< Default radix (10)
};

enum WhiteSpaceType {
   /**
    * With operator<< Discard input white-space characters
    */
   WhiteSpace
};

enum EndOfLineType {
   /**
    * With operator<< Discard input until end-of-line \n
    * With operator>> Write end-of-line
    */
   EndOfLine
};

enum EchoMode {
   /*
    * For use with operator<< and operator>>
    */
   EchoMode_Off = false, //!< Turn echo off
   EchoMode_On  = true,  //!< Turn echo on
};

enum FlushType {
   /**
    * With operator<< Discard queued input \n
    * With operator>> Wait until queued data is transmitted.
    */
   Flush
};

/**
 * Virtual Base class for formatted IO
 */
class FormattedIO {

protected:
   /**
    * Current radix for << and >> operators
    */
   Radix fRadix = Radix_10;

   /**
    * Indicate in error state
    */
   bool inErrorState = false;

   /**
    * Control echo of input characters
    */
   EchoMode echo = EchoMode_Off;

   /**
    * One character look-ahead
    */
   int16_t lookAhead = -1;

   /**
    * Construct formatter interface
    */
   FormattedIO() {
   }

   /**
    * Destructor
    */
   virtual ~FormattedIO() {
   }

   /**
    * Convert character to digit in given radix
    *
    * @param[in] ch    The character to convert
    * @param[in] radix The radix to use
    *
    * @return >=0 Digit in range 0 - (radix-1)
    * @return <0  Invalid character for radix
    */
   static int NOINLINE_DEBUG convertDigit(int ch, Radix radix) {
      unsigned digit = ch - '0';
      if (digit<10) {
         return (digit<radix)?digit:-1;
      }
      digit = ch-'a'+10;
      if (digit<radix) {
         return digit;
      }
      digit = ch-'A'+10;
      if (digit<radix) {
         return digit;
      }
      return -1;
   }

protected:
   /**
    * Check if character is available
    *
    * @return true  Character available i.e. _readChar() will not block
    * @return false No character available
    */
   virtual bool _isCharAvailable() = 0;

   /**
    * Receives a character (blocking)
    *
    * @return Character received
    */
   virtual int _readChar() = 0;

   /**
    * Writes a character (blocking)
    *
    * @param[in]  ch - character to send
    */
   virtual void _writeCh(char ch) = 0;

public:
   /**
    * Peek at lookahead (non-blocking).
    *
    * @return <0   No character available
    * @return >=0  Character available
    */
   int NOINLINE_DEBUG peek() {
      if (lookAhead>0) {
         return lookAhead;
      }
      if (!_isCharAvailable()) {
         return -1;
      }
      lookAhead = _readChar();
      if (lookAhead == (uint8_t)'\r') {
         lookAhead = '\n';
      }
      if (echo) {
         _writeCh(lookAhead);
      }
      return lookAhead;
   }

   /**
    * Push a value to the look-ahead buffer
    *
    * @param[in] ch Character to push
    */
   void NOINLINE_DEBUG pushBack(char ch) {
      lookAhead = (uint8_t)ch;
   }

   /**
    * Writes a character (blocking)
    *
    * @param[in]  ch - character to send
    */
   void NOINLINE_DEBUG writeCh(char ch) {
      _writeCh(ch);
   }

   /**
    * Receives a single character
    *
    * @return Character received
    */
   int NOINLINE_DEBUG readChar() {
      int ch;
      do {
         ch = peek();
      } while (ch < 0);
      lookAhead = -1;
      return ch;
   }

   /**
    * Converts an unsigned long to a string
    *
    * @param[in] ptr   Buffer to write result (at least 12 characters)
    * @param[in] value Unsigned long to convert
    * @param[in] radix Radix for conversion [2..16]
    *
    * @return Pointer to '\0' null character at end of converted number\n
    *         May be used for incrementally writing to a buffer.
    */
   static NOINLINE_DEBUG char *ultoa(char *ptr, unsigned long value, Radix radix=Radix_10) {
#ifdef DEBUG_BUILD
      if (ptr == nullptr) {
         __BKPT();
      }
      if ((radix<2)||(radix>16)) {
         __BKPT();
      }
#endif
      // Save beginning for reversal
      char *beginPtr = ptr;
      // Convert backwards
      do {
         *ptr++ = "0123456789ABCDEF"[value % radix];
         value /= radix;
      } while (value != 0);
      // Terminate and leave ptr at last digit
      *ptr = '\0';
      // Reverse digits
      char *endPtr = ptr-1;
      while (beginPtr < endPtr) {
         char t = *beginPtr;
         *beginPtr++ = *endPtr;
         *endPtr-- = t;
      }
      return ptr;
   }

   /**
    * Converts a long to a string
    *
    * @param[in] ptr   Buffer to write result (at least 12 characters)
    * @param[in] value Long to convert
    * @param[in] radix Radix for conversion [2..16]
    *
    * @return Pointer to '\0' null character at end of converted number\n
    *         May be used for incrementally writing to a buffer.
    */
   static NOINLINE_DEBUG char *ltoa(char *ptr, long value, Radix radix=Radix_10) {
      if (value<0) {
         *ptr++ = '-';
         value = -value;
      }
      return ultoa(ptr, value, radix);
   }

   /**
    * Copies a C string including terminating '\0' character
    *
    * @param[out] dst  Where to copy string
    * @param[in]  src  Source to copy from
    *
    * @return Pointer to '\0' null character at end of concatenated string.\n
    *         May be used for incrementally writing to a buffer.
    */
   static NOINLINE_DEBUG char *strcpy(char *dst, const char *src) {
#ifdef DEBUG_BUILD
      if (dst == nullptr) {
         __BKPT();
      }
#endif
      do {
         *dst++ = *src;
      } while (*src++ != '\0');
      return dst-1;
   }

   /**
    * Transmit data
    *
    * @param[in]  data     Data to transmit
    * @param[in]  size     Size of transmission data
    */
   void NOINLINE_DEBUG transmit(const uint8_t data[], uint16_t size) {
      while (size-->0) {
         writeCh(*data++);
      }
   }

   /**
    * Receive data
    *
    * @param[out] data     Data buffer for reception
    * @param[in]  size     Size of data to receive
    */
   void NOINLINE_DEBUG receive(uint8_t data[], uint16_t size) {
      while (size-->0) {
         *data++ = readChar();
      }
   }

   /**
    * Receive string until terminator character\n
    * The terminating character is discarded and the string '\0' terminated
    *
    * @param[out] data       Data buffer for reception
    * @param[in]  size       Size of data buffer (including '\0')
    * @param[in]  terminator Terminating character
    *
    * @return number of characters read (excluding terminator)
    *
    * Usage
    * @code
    *    char buff[100];
    *    int numChars = gets(buff, sizeof(buff));
    * @endcode
    */
   int NOINLINE_DEBUG gets(char data[], uint16_t size, char terminator='\n') {
      char *ptr = data;
      while (size-->1) {
         char ch = readChar();
         if (ch == terminator) {
            break;
         }
         *ptr++ = ch;
      }
      *ptr = '\0';
      return ptr-data;
   }

   /**
    * Transmit a character
    *
    * @param[in]  ch - character to send
    *
    * @return Reference to self
    */
   FormattedIO NOINLINE_DEBUG &write(char ch) {
      writeCh(ch);
      return *this;
   }

   /**
    * Transmit an end-of-line
    *
    * @return Reference to self
    */
   FormattedIO NOINLINE_DEBUG &writeln() {
      return write('\n');
   }

   /**
    * Transmit a character with newline
    *
    * @param[in]  ch - character to send
    *
    * @return Reference to self
    */
   FormattedIO  NOINLINE_DEBUG &writeln(char ch) {
      write(ch);
      return write('\n');
   }

   /**
    * Transmit a C string
    *
    * @param[in]  str String to print
    *
    * @return Reference to self
    */
   FormattedIO NOINLINE_DEBUG &write(const char *str) {
      while (*str != '\0') {
         write(*str++);
      }
      return *this;
   }

   /**
    * Transmit a C string with new line
    *
    * @param[in]  str String to print
    *
    * @return Reference to self
    */
   FormattedIO NOINLINE_DEBUG &writeln(const char *str) {
      write(str);
      return write('\n');
   }

   /**
    * Transmit a boolean value
    *
    * @param[in]  b Boolean to print
    *
    * @return Reference to self
    */
   FormattedIO NOINLINE_DEBUG &write(bool b) {
      return write(b?"true":"false");
   }

   /**
    * Transmit a boolean value with new line
    *
    * @param[in]  b Boolean to print
    *
    * @return Reference to self
    */
   FormattedIO NOINLINE_DEBUG &writeln(bool b) {
      write(b);
      return write('\n');
   }

   /**
    * Transmit an unsigned long integer
    *
    * @param[in]  value Unsigned long to print
    * @param[in]  radix Radix for conversion [2..16]
    *
    * @return Reference to self
    */
   FormattedIO NOINLINE_DEBUG &write(unsigned long value, Radix radix=Radix_10) {
      static char buff[35];
      ultoa(buff, value, radix);
      return write(buff);
   }

   /**
    * Transmit an unsigned long integer with newline
    *
    * @param[in]  value Unsigned long to print
    * @param[in]  radix Radix for conversion [2..16]
    *
    * @return Reference to self
    */
   FormattedIO NOINLINE_DEBUG &writeln(unsigned long value, Radix radix=Radix_10) {
      write(value, radix);
      return write('\n');
   }

   /**
    * Transmit an pointer value
    *
    * @param[in]  value Pointer value to print
    * @param[in]  radix Radix for conversion [2..16]
    *
    * @return Reference to self
    */
   FormattedIO NOINLINE_DEBUG &write(const void *value, Radix radix=Radix_10) {
      return write((unsigned long) value, radix);
   }

   /**
    * Transmit an pointer value with newline
    *
    * @param[in]  value Pointer value to print
    * @param[in]  radix Radix for conversion [2..16]
    *
    * @return Reference to self
    */
   FormattedIO NOINLINE_DEBUG &writeln(const void *value, Radix radix=Radix_10) {
      return writeln((unsigned long) value, radix);
   }

   /**
    * Transmit a long integer
    *
    * @param[in]  value Long to print
    * @param[in]  radix Radix for conversion [2..16]
    *
    * @return Reference to self
    */
   FormattedIO NOINLINE_DEBUG &write(long value, Radix radix=Radix_10) {
      if (value<0) {
         write('-');
         value = -value;
      }
      return write((unsigned long) value, radix);
   }

   /**
    * Transmit a long integer with newline
    *
    * @param[in]  value Long to print
    * @param[in]  radix Radix for conversion [2..16]
    *
    * @return Reference to self
    */
   FormattedIO NOINLINE_DEBUG &writeln(long value, Radix radix=Radix_10) {
      write(value, radix);
      return write('\n');
   }

   /**
    * Transmit an unsigned integer
    *
    * @param[in]  value Unsigned to print
    * @param[in]  radix Radix for conversion [2..16]
    *
    * @return Reference to self
    */
   FormattedIO NOINLINE_DEBUG &write(unsigned value, Radix radix=Radix_10) {
      return write((unsigned long)value, radix);
   }

   /**
    * Transmit an unsigned integer with newline
    *
    * @param[in]  value Unsigned to print
    * @param[in]  radix Radix for conversion [2..16]
    *
    * @return Reference to self
    */
   FormattedIO NOINLINE_DEBUG &writeln(unsigned value, Radix radix=Radix_10) {
      return writeln((unsigned long)value, radix);
   }

   /**
    * Transmit an integer
    *
    * @param[in]  value Integer to print
    * @param[in]  radix Radix for conversion [2..16]
    *
    * @return Reference to self
    */
   FormattedIO NOINLINE_DEBUG &write(int value, Radix radix=Radix_10) {
      return write((long)value, radix);
   }

   /**
    * Transmit an integer with newline
    *
    * @param[in]  value Integer to print
    * @param[in]  radix Radix for conversion [2..16]
    *
    * @return Reference to self
    */
   FormattedIO NOINLINE_DEBUG  &writeln(int value, Radix radix=Radix_10) {
      return writeln((long)value, radix);
   }

   /**
    * Transmit a double
    *
    * @param[in]  value Double to print
    *
    * @return Reference to self
    *
    * @note Uses snprintf() which is large.
    * @note To use this function it is necessary to enable floating point printing\n
    *       in the linker options (Support %f format in printf -u _print_float)).
    */
   FormattedIO NOINLINE_DEBUG &write(double value) {
      char buff[20];
      snprintf(buff, sizeof(buff), "%f", value);
      return write(buff);
   }

   /**
    * Transmit a double with newline
    *
    * @param[in]  value Double to print
    *
    * @return Reference to self
    *
    * @note Uses snprintf() which is large.
    * @note To use this function it is necessary to enable floating point printing\n
    *       in the linker options (Support %f format in printf -u _print_float).
    */
   FormattedIO NOINLINE_DEBUG &writeln(double value) {
      write(value);
      return write('\n');
   }

   /**
    * Transmit a float
    *
    * @param[in]  value Float to print
    *
    * @return Reference to self
    *
    * @note Uses snprintf() which is large.
    * @note To use this function it is necessary to enable floating point printing\n
    *       in the linker options (Support %f format in printf -u _print_float).
    */
   FormattedIO NOINLINE_DEBUG &write(float value) {
      return write((double)value);
   }

   /**
    * Transmit a float with newline
    *
    * @param[in]  value Float to print
    *
    * @return Reference to self
    *
    * @note Uses snprintf() which is large.
    * @note To use this function it is necessary to enable floating point printing\n
    *       in the linker options (Support %f format in printf -u _print_float)).
    */
   FormattedIO NOINLINE_DEBUG &writeln(float value) {
      return writeln((double)value);
   }

   /**
    * Transmit a character
    *
    * @param[in]  ch Character to print
    *
    * @return Reference to self
     */
   FormattedIO NOINLINE_DEBUG &operator <<(char ch) {
      return write(ch);
   }

   /**
    * Transmit a boolean value
    *
    * @param[in]  b Boolean to print
    *
    * @return Reference to self
     */
   FormattedIO NOINLINE_DEBUG &operator <<(bool b) {
      return write(b);
   }

   /**
    * Transmit a C string
    *
    * @param[in]  str String to print
    *
    * @return Reference to self
    */
   FormattedIO NOINLINE_DEBUG &operator <<(const char *str) {
      return write(str);
   }

   /**
    * Transmit an unsigned long integer
    *
    * @param[in]  value Unsigned long to print
    *
    * @return Reference to self
    */
   FormattedIO NOINLINE_DEBUG &operator <<(unsigned long value) {
      return write(value, fRadix);
   }

   /**
    * Transmit a long integer
    *
    * @param[in]  value Long to print
    *
    * @return Reference to self
    */
   FormattedIO NOINLINE_DEBUG &operator <<(long value) {
      return write(value, fRadix);
   }

   /**
    * Transmit an unsigned integer
    *
    * @param[in]  value Unsigned to print
    *
    * @return Reference to self
    */
   FormattedIO NOINLINE_DEBUG &operator <<(unsigned int value) {
      return write(value, fRadix);
   }

   /**
    * Transmit an integer
    *
    * @param[in]  value Integer to print
    *
    * @return Reference to self
    */
   FormattedIO NOINLINE_DEBUG &operator <<(int value) {
      return write(value, fRadix);
   }

   /**
    * Transmit a pointer value
    *
    * @param[in]  value Pointer value to print
    *
    * @return Reference to self
    */
   FormattedIO NOINLINE_DEBUG &operator <<(const void *value) {
      return write((unsigned long)value, fRadix);
   }

   /**
    * Transmit a float
    *
    * @param[in]  value Float to print
    *
    * @return Reference to self
    *
    * @note Uses snprintf() which is large.
    * @note To use this function it is necessary to enable floating point printing\n
    *       in the linker options (Support %f format in printf -u _print_float)).
    */
   FormattedIO NOINLINE_DEBUG &operator <<(float value) {
      return write((double)value);
   }

   /**
    * Transmit a double
    *
    * @param[in]  value Double to print
    *
    * @return Reference to self
    *
    * @note Uses snprintf() which is large.
    * @note To use this function it is necessary to enable floating point printing\n
    *       in the linker options (Support %f format in printf -u _print_float)).
    */
   FormattedIO NOINLINE_DEBUG &operator <<(double value) {
      return write(value);
   }

   /**
    * Sets the conversion radix for integer types
    *
    * @param[in] radix Radix to set
    *
    * @return Reference to self
    *
    * @note Only applies for operator<< methods
    */
   FormattedIO NOINLINE_DEBUG &operator <<(Radix radix) {
      fRadix = radix;
      return *this;
   }

   /**
    * Write end-of-line
    *
    * @return Reference to self
    */
   FormattedIO NOINLINE_DEBUG &operator <<(EndOfLineType) {
      write('\n');
      return *this;
   }

   /**
    * Enable/Disable echoing of input characters
    *
    * @return Reference to self
    */
   FormattedIO NOINLINE_DEBUG &operator <<(EchoMode echoMode) {
      return setEcho(echoMode);
   }

   /**
    * Enable/Disable echoing of input characters
    *
    * @return Reference to self
    */
   FormattedIO NOINLINE_DEBUG &operator >>(EchoMode echoMode) {
      return setEcho(echoMode);
   }

   /**
    * Flush output data
    *
    * @return Reference to self
    */
   FormattedIO NOINLINE_DEBUG &operator <<(FlushType) {
      flushOutput();
      return *this;
   }

   /**
    * Discard white-space from the input
    *
    * @return Reference to self
    */
   FormattedIO NOINLINE_DEBUG &skipWhiteSpace() {
      int ch;
      do {
         ch = readChar();
      } while (isspace(ch));
      pushBack(ch);
      return *this;
   }

   /**
    * Discard input until end-of-line (inclusive)
    *
    * @return Reference to self
    */
   FormattedIO NOINLINE_DEBUG &readln() {
      while (readChar() != '\n') {
         __asm__("nop");
      }
      return *this;
   }

   /**
    * Read a character from the input
    *
    * @param[out] ch Where to place character read
    *
    * @return Reference to self
    */
   FormattedIO NOINLINE_DEBUG &read(char &ch) {
      ch = readChar();
      return *this;
   }

   /**
    * Get and clear error state
    *
    * @return false No error
    * @return true  Operation failed since last checked e.g. illegal digit at start of number
    */
   bool NOINLINE_DEBUG isError() {
      bool t = inErrorState;
      inErrorState = false;
      return t;
   }

   /**
    * Receives an unsigned long
    *
    * @param[out] value Where to place value read
    * @param[in]  radix The radix to use
    *
    * @return Reference to self
    *
    * @note Skips leading whitespace
    */
   FormattedIO NOINLINE_DEBUG &read(unsigned long &value, Radix radix=Radix_10) {
      // Skip white space
      int ch;
      do {
         ch = readChar();
      } while (isspace(ch));

      // Check if sign character
      bool negative = (ch == '-');
      if (negative) {
         // Discard  '-'
         ch = readChar();
      }

      // Parse number
      value = 0;
      int digitCount = 0;
      do {
         int digit = convertDigit(ch, radix);
         if (digit<0) {
            break;
         }
         digitCount++;
         value *= radix;
         value += digit;
         ch = readChar();
      } while (true);

      // Must have at least 1 digit
      inErrorState = (digitCount<=0);

      // Push back 1st non-digit
      pushBack(ch);
      if (negative) {
         value = -value;
      }
      return *this;
   }

   /**
    * Controls echoing of input characters
    *
    * @param echoMode
    *
    * @return Reference to self
    */
   FormattedIO NOINLINE_DEBUG &setEcho(EchoMode echoMode=EchoMode_On) {
      echo = echoMode;
      return *this;
   }
   /**
    * Receives an unsigned long and then discards characters until end of line.
    *
    * @param[out] value Where to place value read
    * @param[in]  radix The radix to use
    *
    * @return Reference to self
    *
    * @note Skips leading whitespace
    */
   FormattedIO NOINLINE_DEBUG &readln(unsigned long &value, Radix radix=Radix_10) {
      read(value, radix);
      return readln();
   }

   /**
    * Receives a long
    *
    * @param[out] value Where to place value read
    * @param[in]  radix The radix to use
    *
    * @return Reference to self
    *
    * @note Skips leading whitespace
    */
   FormattedIO NOINLINE_DEBUG &read(long &value, Radix radix=Radix_10) {
      unsigned long temp;
      read(temp, radix);
      value = temp;
      return *this;
   }

   /**
    * Receives a long and then discards characters until end of line.
    *
    * @param[out] value Where to place value read
    * @param[in]  radix The radix to use
    *
    * @return Reference to self
    *
    * @note Skips leading whitespace
    */
   FormattedIO NOINLINE_DEBUG &readln(long &value, Radix radix=Radix_10) {
      read(value, radix);
      return readln();
   }

   /**
    * Receives an unsigned integer
    *
    * @param[out] value Where to place value read
    * @param[in]  radix The radix to use
    *
    * @return Reference to self
    *
    * @note Skips leading whitespace
    */
   FormattedIO NOINLINE_DEBUG &read(unsigned int &value, Radix radix=Radix_10) {
      unsigned long temp;
      read(temp, radix);
      value = temp;
      return *this;
   }

   /**
    * Receives an unsigned integer and then discards characters until end of line.
    *
    * @param[out] value Where to place value read
    * @param[in]  radix The radix to use
    *
    * @return Reference to self
    *
    * @note Skips leading whitespace
    */
   FormattedIO NOINLINE_DEBUG &readln(unsigned &value, Radix radix=Radix_10) {
      read(value, radix);
      return readln();
   }

   /**
    * Receives an integer
    *
    * @param[out] value Where to place value read
    * @param[in]  radix The radix to use
    *
    * @return Reference to self
    *
    * @note Skips leading whitespace
    */
   FormattedIO NOINLINE_DEBUG &read(int &value, Radix radix=Radix_10) {
      long temp;
      read(temp, radix);
      value = temp;
      return *this;
   }

   /**
    * Receives an integer and then discards characters until end of line.
    *
    * @param[out] value Where to place value read
    * @param[in]  radix The radix to use
    *
    * @return Reference to self
    *
    * @note Skips leading whitespace
    */
   FormattedIO NOINLINE_DEBUG &readln(int &value, Radix radix=Radix_10) {
      read(value, radix);
      return readln();
   }

   /**
    * Discard white-space from the input
    *
    * @return Reference to self
    */
   FormattedIO NOINLINE_DEBUG &operator >>(WhiteSpaceType) {
      return skipWhiteSpace();
   }

   /**
    * Discard input until end-of-line
    *
    * @return Reference to self
    */
   FormattedIO NOINLINE_DEBUG &operator >>(EndOfLineType) {
      while (readChar() != '\n') {
         __asm__("nop");
      }
      return *this;
   }

   /**
    * Flush input data
    *
    * @return Reference to self
    */
   FormattedIO NOINLINE_DEBUG &operator >>(FlushType) {
      flushInput();
      return *this;
   }

   /**
    * Sets the conversion radix for integer types
    *
    * @param[in]  radix Radix to set
    *
    * @return Reference to self
    *
    * @note Only applies for operator<< methods
    */
   FormattedIO NOINLINE_DEBUG &operator >>(Radix radix) {
      fRadix = radix;
      return *this;
   }

   /**
    * Receives a single character
    *
    * @param[out] ch Where to place character read
    *
    * @return Reference to self
    */
   FormattedIO NOINLINE_DEBUG &operator >>(char &ch) {
      ch = readChar();
      return *this;
   }

   /**
    * Receives an unsigned long
    *
    * @param[out] value Where to place value read
    *
    * @return Reference to self
    *
    * @note Skips leading whitespace
    */
   FormattedIO NOINLINE_DEBUG &operator >>(unsigned long &value) {
      return read(value, fRadix);
   }

   /**
    * Receives a long
    *
    * @param[out] value Where to place value read
    *
    * @return Reference to self
    *
    * @note Skips leading whitespace
    */
   FormattedIO NOINLINE_DEBUG &operator >>(long &value) {
      return read(value, fRadix);
   }

   /**
    * Receives an unsigned long
    *
    * @param[out] value Where to place value read
    *
    * @return Reference to self
    *
    * @note Skips leading whitespace
    */
   FormattedIO NOINLINE_DEBUG &operator >>(unsigned int &value) {
      return read(value, fRadix);
   }

   /**
    * Receives an integer
    *
    * @param[out] value Where to place value read
    *
    * @return Reference to self
    *
    * @note Skips leading whitespace
    */
   FormattedIO NOINLINE_DEBUG &operator >>(int &value) {
      return read(value, fRadix);
   }

   /**
    * Get conversion radix for given base
    *
    * @param[in]  radix Base to convert to radix [2..16]
    *
    * @return Radix corresponding to base
    */
   static constexpr Radix NOINLINE_DEBUG radix(unsigned radix) {
      return (Radix)radix;
   }

   /**
    *  Flush output data
    */
   virtual void flushOutput() = 0;

   /**
    *  Flush input data
    */
   virtual void flushInput() = 0;

};

/**
 * @}
 */

} // End namespace USBDM

#endif /* INCLUDE_USBDM_FORMATTED_IO_H_ */
