/*
 * Cl7ck
 * Author : op414
 * http://op414.net
 * License : CC BY-NC-SA http://creativecommons.org/licenses/by-nc-sa/3.0/
 * ------------
 * TempSensor.h : TempSensor class definition
 * Created 23 june 2013
 */
 
#ifndef DEF_TEMPSENSOR
#define DEF_TEMPSENSOR

#include <OneWire.h>

#define DS18B20_START_CONVERSION 0x44
#define DS18B20_READ_SCRATCHPAD 0xBE

#define TEMP_CONVERSION_DELAY 1000UL
#define TEMP_CHECK_INTERVAL 5000UL

class TempSensor
{
  public:
    TempSensor();
    boolean updateTemp();
    void displayTemp();
    int getWhole();
    int getFract();
  
  private:
    int m_tempWhole;
    int m_tempFract;
    
    OneWire m_sensor;
    boolean m_conversionAsked;
    unsigned long m_conversionStartTime;
    
    void startConversion();
};

#endif
