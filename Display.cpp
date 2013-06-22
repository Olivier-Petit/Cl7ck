/*
 * Cl7ck
 * Author : op414
 * http://op414.net
 * License : CC BY-NC-SA http://creativecommons.org/licenses/by-nc-sa/3.0/
 * ------------
 * Display.cpp : implements the Display class to communicate with the MAX7219
 * Created 11 may 2013
 */

#if defined(ARDUINO) && ARDUINO >= 100
  #include <Arduino.h>
#else
  #include "WProgram.h"
#endif

#include <SPI.h>

#include "settings.h"
#include "Display.h"
 
Display::Display(prog_char fontSet[])
{
  // Font set
  m_fontSet = fontSet;
  // Setting-up the load pin (revert logic)
  pinMode(PIN_DISP, OUTPUT);
  digitalWrite(PIN_DISP, HIGH);
  
  clearBuffer();
  
  // Setting-up the SPI library
  SPI.begin();
  SPI.setBitOrder(MSBFIRST);
  SPI.setClockDivider(SPI_CLOCK_DIV128);
  SPI.setDataMode(SPI_MODE0);
  
  sendCommand(MAX_REG_DECODEMODE, 0x00); // No decode
  sendCommand(MAX_REG_SCANLIMIT, 0x07); // Only digits 0->3
  setBrightness(0x0F); // Max Brightness
  sendCommand(MAX_REG_DISPLAYTEST, 0x00); // No test mode
  sendCommand(MAX_REG_SHUTDOWN, 0x01); // Leaving shutdown mode
  
}

void Display::sendCommand(byte reg, byte val)
{
  digitalWrite(PIN_DISP, LOW);
  
  SPI.transfer(reg);
  SPI.transfer(val);
  
  digitalWrite(PIN_DISP, HIGH);
}

void Display::clearBuffer()
{
  for(int i = 0 ; i < BUFFER_SIZE ; i++)
    m_buffer[i] = 0;
}

void Display::setDigit(int dig, int val)
{
  m_buffer[dig] = (val & B01111111) + (m_buffer[dig] & B10000000); // This way you do not modify the DP...
}

void Display::setDP(int dig, boolean dp)
{
  if(dp)
    m_buffer[dig] |= B10000000;
  else
    m_buffer[dig] &= B01111111;
}

byte Display::getChar(char c)
{
  if(c >= '0' && c <= '9')
    return (byte)pgm_read_byte_near(m_fontSet + (c - '0'));
  else if(c >= 'A' && c <= 'Z')
    return (byte)pgm_read_byte_near(m_fontSet + 10 + (c -'A'));
  else if(c >= 'a' && c <= 'z')
    return (byte)pgm_read_byte_near(m_fontSet + 36 + (c - 'a'));
  else if(c == '-')
    return (byte)0x01;
  
  return (byte)0;
}

void Display::setString(int pos, char* str)
{
  int i;
  boolean cont = true;
  
  while(cont)
  {
    if(str[i] != '\0' && (pos + i) < BUFFER_SIZE && pos >= 0)
    {
      setDigit(pos + i, getChar(str[i]));
      i++;
    }
    else
      cont = false;
  }
}

void Display::setString(int pos, char c)
{
  if(pos < BUFFER_SIZE && pos >= 0)
    setDigit(pos, getChar(c));
}

void Display::setString(int pos, int number, int length)
{
  for(int i = pos + (length - 1) ; i >= pos ; i--)
  {
    char dig = (char)(number % 10) + '0';
    number /= 10;
    
    if(pos < BUFFER_SIZE && pos >= 0)
    {
      setDigit(i, getChar(dig));
    }
  }
}

void Display::display()
{
  for(int i = 0 ; i < 8 ; i++)
  {
    sendCommand(i+1, m_buffer[i]);
  }
}

int Display::getBrightness()
{
  return (int)m_brightness;
}

void Display::setBrightness(int brightness)
{
  if(brightness < 0)
    m_brightness = 0;
  else if(brightness > 0x0F)
    m_brightness = 0x0F;
  else
    m_brightness = (byte)brightness;
  
  sendCommand(MAX_REG_INTENSITY, m_brightness);
}

void Display::printBuffer()
{
  for(int i = 0 ; i < BUFFER_SIZE ; i++)
  {
    Serial.print(m_buffer[i], HEX);
    Serial.print(" ");
  }
  Serial.println();
}
