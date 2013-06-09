/*
 * Cl7ck
 * Author : op414
 * http://op414.net
 * License : CC BY-NC-SA http://creativecommons.org/licenses/by-nc-sa/3.0/
 * ------------
 * Display.h : header file for Display class
 * Created 11 may 2013
 */
 
#ifndef DEF_DISPLAY
#define DEF_DISPLAY

#if defined(ARDUINO) && ARDUINO >= 100
  #include <Arduino.h>
#else
  #include "WProgram.h"
#endif

#include "font.h"

#define BUFFER_SIZE 20

// Definition of MAX7219 registers
#define MAX_REG_NOOP           0x00
#define MAX_REG_DECODEMODE     0x09
#define MAX_REG_INTENSITY      0x0A
#define MAX_REG_SCANLIMIT      0x0B
#define MAX_REG_SHUTDOWN       0x0C
#define MAX_REG_DISPLAYTEST    0x0F

class Display
{
  public:
    Display(prog_char fontSet[]);
    void setDigit(int dig, int val);
    void clearBuffer();
    void setDP(int dig, boolean dp);
    void sendCommand(byte reg, byte val);
    byte getChar(char c);
    void setString(int pos, char* str);
    void display();
  
  private:
    byte m_buffer[BUFFER_SIZE];
    //void sendCommand(byte reg, byte val);
    prog_char *m_fontSet PROGMEM;
  };

#endif
