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
  m_time.hours = 0U;
  m_time.mins = 0U;
  m_time.secs = 0U;
  m_time.DOM = 0U;
  m_time.DOW = 0U;
  m_time.month = 0U;
  m_time.year = 0U;

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
    m_time.secs = (unsigned int)bcdToDec(Wire.read());
    m_time.mins = (unsigned int)bcdToDec(Wire.read());
    m_time.hours = (unsigned int)bcdToDec(Wire.read());
    m_time.DOW = (unsigned int)bcdToDec(Wire.read());
    m_time.DOM = (unsigned int)bcdToDec(Wire.read());
    m_time.month = (unsigned int)bcdToDec(Wire.read());
    m_time.year = (unsigned int)bcdToDec(Wire.read());
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

DateTime TimeHandler::getTime()
{
  return m_time;
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

void TimeHandler::setTime(DateTime time)
{
  m_time = time;
  
  // Validate the data (a bit special because we use unsigned ints)
  /* ----> Note : Date integrity (leap year and so on) is verified in TimeHandler::setRTCTime()
                  because when setting the date, you set the DOM first, so you might have a invalid
                  date a some point, which is okay...
  */              
  if(m_time.hours == 65535)
    m_time.hours = 23;
  if(m_time.hours > 23)
    m_time.hours = 0;
  
  if(m_time.mins == 65535)
    m_time.mins = 59;
  if(m_time.mins > 59)
    m_time.mins = 0;
  
  if(m_time.secs == 65535)
    m_time.secs = 59;
  if(m_time.secs > 59)
    m_time.secs = 0;
  
  if(m_time.DOM == 65535)
    m_time.DOM = 31;
  if(m_time.DOM > 31)
    m_time.DOM = 1;
  
  if(m_time.month == 65535)
    m_time.month = 12;
  if(m_time.month > 12)
    m_time.month = 1;
    
  if(m_time.year == 65535)
    m_time.month = 99;
  if(m_time.year > 99)
    m_time.year = 0;
}

// Used to write time to the RTC
int TimeHandler::setRTCTime()
{
  
  // Check date integrity
  boolean dateOK = true;
  
  // February
  boolean leapYear = (m_time.year % 4) == 0 ? true : false; // This is not exactly the real rule, but It's valid until 2099
  
  if(m_time.month == 2U && (m_time.DOM == 30U || m_time.DOM == 31 || (m_time.DOM == 29U && leapYear == false)))
    dateOK = false;
        
  // Check for number of days
  if(m_time.DOM == 31U && (m_time.month == 4U || m_time.month == 6U || m_time.month == 9U || m_time.month == 11U))
     dateOK = false;
  
  
  if(!dateOK)
  {
    return 1;
  }
  else
  {
    Wire.beginTransmission(DS3231S_ADDR);
    Wire.write((byte)0x00); // Start at register 0x00 (seconds)
  
    // Write the data
    Wire.write(decToBcd((byte)m_time.secs));
    Wire.write(decToBcd((byte)m_time.mins));
    Wire.write(decToBcd((byte)m_time.hours));
    Wire.write(decToBcd((byte)m_time.DOW));
    Wire.write(decToBcd((byte)m_time.DOM));
    Wire.write(decToBcd((byte)m_time.month));
    Wire.write(decToBcd((byte)m_time.year));
    
    Wire.endTransmission();
    
    return 0;
  }
}

// --------- Debuging functions ---------
#ifdef DEBUG

// Used to print the current time to the serial port
void TimeHandler::printTime() 
{
#ifdef MODE_DMY
  Serial.println(String(m_time.DOM) + "/" + String(m_time.month) + "/" + String(m_time.year) + " (DOW : " + String(m_time.DOW) + ")");
#else
  Serial.println(String(m_time.month) + "/" + String(m_time.DOM) + "/" + String(m_time.year) + " (DOW : " + String(m_time.DOW) + ")");
#endif

  Serial.println(String(m_time.hours) + ":" + String(m_time.mins) + ":" + String(m_time.secs));
}
#endif
