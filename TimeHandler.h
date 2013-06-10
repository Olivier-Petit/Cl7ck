/*
 * Cl7ck
 * Author : op414
 * http://op414.net
 * License : CC BY-NC-SA http://creativecommons.org/licenses/by-nc-sa/3.0/
 * ------------
 * TimeHandler.h : Header for the TimeHandler class
 * Created June 9th 2013
 */

#ifndef DEF_TIMEHANDLER
#define DEF_TIMEHANDLER

#define DS3231S_ADDR 0x68

#include "settings.h"

#define RTC_CHECK_INTERVAL 100UL

#define MODE_DMY // Comment for MDY mode

typedef struct Time Time;

class TimeHandler
{
  public:
    TimeHandler();
    void initializeRTC();
    boolean updateTime();
    unsigned int getHours();
    unsigned int getMins();
    unsigned int getSecs();
    
    void setRTCTime();
    
    #ifdef DEBUG
      void printTime();
    #endif
    
  private:
    unsigned int m_hours;
    unsigned int m_mins;
    unsigned int m_secs;
    unsigned int m_DOM;
    unsigned int m_DOW;
    unsigned int m_month;
    unsigned int m_year;
    
    unsigned long m_lastRTCCheck;
    
    byte decToBcd(byte val);
    byte bcdToDec(byte val);
    void getRTCTime();
};

#endif
