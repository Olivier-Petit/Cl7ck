/*
 * Cl7ck
 * Author : op414
 * http://op414.net
 * License : CC BY-NC-SA http://creativecommons.org/licenses/by-nc-sa/3.0/
 * ------------
 * TempSensor.cpp : Implements the TempSensor class to interact with the DS18B20
 * Created 23 june 2013
 */
 
 #if defined(ARDUINO) && ARDUINO >= 100
  #include <Arduino.h>
#else
  #include "WProgram.h"
#endif

#include <OneWire.h>

#include "TempSensor.h"
#include "Display.h"
#include "settings.h"

// Constructor
TempSensor::TempSensor() : m_sensor(PIN_TEMP)
{
  m_tempWhole = 0;
  m_tempFract = 0;
  m_conversionAsked = false;
}

// Used to ask the sensor to start computing the temperature
void TempSensor::startConversion()
{
  m_sensor.reset(); // Reset bus
  m_sensor.skip(); // Skip the device selection (only one sensor is on the bus)
  m_sensor.write(DS18B20_START_CONVERSION); // Ask to start conversion
  
  m_conversionStartTime = millis();
}
  
// Used to update the temp
boolean TempSensor::updateTemp()
{
  if(millis() - m_conversionStartTime >= TEMP_CHECK_INTERVAL || m_conversionStartTime == 0) // Begin a new conversion
  {
    startConversion();
    m_conversionStartTime = millis();
    m_conversionAsked = true;
    
    return false; // Temp did not changed
  }
  else if(millis() - m_conversionStartTime >= TEMP_CONVERSION_DELAY && m_conversionAsked == true) // Read the temp
  {
    byte data[9];
    
    m_sensor.reset();
    m_sensor.skip();
    m_sensor.write(DS18B20_READ_SCRATCHPAD); // Ask to read scratchpad
    
    for(int i = 0 ; i < 9 ; i++) // Read 9 bytes
      data[i] = m_sensor.read();
    
    int lowByte = data[0];
    int highByte = data[1];
    
    int tReading = (highByte << 8) + lowByte; // Make a twelve bit int
    int signBit = tReading & 0x8000; // Test the most significant bit
    
    if(signBit) // Negative
      tReading = (tReading ^ 0xFFFF) + 1; // 2's comp
      
    int tc100 = (6 * tReading) + tReading / 4; // Multiply by (100 * 0.0625) or 6.25
    
    m_tempWhole = tc100 / 100; // Get the whole portion
    m_tempFract = tc100 % 100; // Get the fractionnal portion
    
    m_conversionAsked = false; // We do not want to read the scratchpad before asking for a new conversion
    
    return true; // Temp changed
  }
  
  return false; // Temp did no changed
}

int TempSensor::getWhole()
{
  return m_tempWhole;
}

int TempSensor::getFract()
{
  return m_tempFract;
}
