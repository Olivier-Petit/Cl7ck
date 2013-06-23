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
#include "settings.h"

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
    byte getChar(char c);
    void setString(int pos, char* str);
    void setString(int pos, char c);
    void setString(int pos, int number, int length);
    void display();
    int getBrightness();
    void setBrightness(int brightness);
    
    #ifdef DEBUG
      void printBuffer();
    #endif
  
  private:
    byte m_buffer[BUFFER_SIZE];
    void sendCommand(byte reg, byte val);
    
    prog_char *m_fontSet PROGMEM;
    byte m_brightness;
  };

#endif
