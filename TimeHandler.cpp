/*
 * Cl7ck
 * Author : op414
 * http://op414.net
 * License : CC BY-NC-SA http://creativecommons.org/licenses/by-nc-sa/3.0/
 * ------------
 * TimeHandler.cpp : implements the TimeHandler class to interract with the DS3231S RTC
 * Created June 9th 2013
 */

#if defined(ARDUINO) && ARDUINO >= 100
#include <Arduino.h>
#else
#include "WProgram.h"
#endif

#include <Wire.h>

#include "TimeHandler.h"
#include "settings.h"

// Constructor
TimeHandler::TimeHandler()
{
  m_hours = 0U;
  m_mins = 0U;
  m_secs = 0U;
  m_DOM = 0U;
  m_DOW = 0U;
  m_month = 0U;
  m_year = 0U;

  m_lastRTCCheck = 0UL;
}

// Used to get the time from the chronodot
void TimeHandler::getRTCTime()
{
  Wire.beginTransmission(DS3231S_ADDR);
  Wire.write((byte)0x00); // Start at register 0
  Wire.endTransmission();
  Wire.requestFrom(DS3231S_ADDR, 7); // Request seven bytes (seconds, minutes, hours, dow, dom, month, year)

  while(Wire.available()) // Wait for the data to come
  {
    // Read the data
    m_secs = (unsigned int)bcdToDec(Wire.read());
    m_mins = (unsigned int)bcdToDec(Wire.read());
    m_hours = (unsigned int)bcdToDec(Wire.read());
    m_DOW = (unsigned int)bcdToDec(Wire.read());
    m_DOM = (unsigned int)bcdToDec(Wire.read());
    m_month = (unsigned int)bcdToDec(Wire.read());
    m_year = (unsigned int)bcdToDec(Wire.read());
  }
}

// Used to update the time.
boolean TimeHandler::updateTime()
{
  if((millis() - m_lastRTCCheck) >= RTC_CHECK_INTERVAL || m_lastRTCCheck == 0)
  {
    getRTCTime();
    m_lastRTCCheck = millis();

    return true;
  }
  else
    return false;
}

unsigned int TimeHandler::getHours()
{
  return m_hours;
}

unsigned int TimeHandler::getMins()
{
  return m_mins;
}

unsigned int TimeHandler::getSecs()
{
  return m_secs;
}

// Used to initialize the RTC (Chronodot)
void TimeHandler::initializeRTC()
{
  Wire.begin(); // Initialize the Wire library

  // Clear /EOSC bit
  Wire.beginTransmission(DS3231S_ADDR);
  Wire.write((byte)0x0E); // Select register
  Wire.write((byte)B00011100); // Write register bitmap, bit 7 is /EOSC
  Wire.endTransmission();
}

// Helpers functions
byte TimeHandler::decToBcd(byte val)
{
  return ((val/10*16) + (val%10));
}

byte TimeHandler::bcdToDec(byte val)
{
  return ((val/16*10) + (val%16));
}

// Used to write time to the RTC
void TimeHandler::setRTCTime()
{
  Wire.beginTransmission(DS3231S_ADDR);
  Wire.write((byte)0x00); // Start at register 0x00 (seconnds)

  // Write the data
  Wire.write(decToBcd((byte)m_secs));
  Wire.write(decToBcd((byte)m_mins));
  Wire.write(decToBcd((byte)m_hours));
  Wire.write(decToBcd((byte)m_DOW));
  Wire.write(decToBcd((byte)m_DOM));
  Wire.write(decToBcd((byte)m_month));
  Wire.write(decToBcd((byte)m_year));
  
  /*Wire.write(decToBcd((byte)0));
  Wire.write(decToBcd((byte)52));
  Wire.write(decToBcd((byte)19));
  Wire.write(decToBcd((byte)7));
  Wire.write(decToBcd((byte)9));
  Wire.write(decToBcd((byte)6));
  Wire.write(decToBcd((byte)13));*/

  Wire.endTransmission();
}

// --------- Debuging functions ---------
#ifdef DEBUG

// Used to print the current time to the serial port
void TimeHandler::printTime() 
{
#ifdef MODE_DMY
  Serial.println(String(m_DOM) + "/" + String(m_month) + "/" + String(m_year) + " (DOW : " + String(m_DOW) + ")");
#else
  Serial.println(String(m_month) + "/" + String(m_DOM) + "/" + String(m_year) + " (DOW : " + String(m_DOW) + ")");
#endif

  Serial.println(String(m_hours) + ":" + String(m_mins) + ":" + String(m_secs));
}
#endif
