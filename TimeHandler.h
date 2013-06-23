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

typedef struct DateTime DateTime;

struct DateTime
{
  unsigned int hours;
  unsigned int mins;
  unsigned int secs;
  unsigned int DOM;
  unsigned int DOW;
  unsigned int month;
  unsigned int year;
};

class TimeHandler
{
  public:
    TimeHandler();
    void initializeRTC();
    boolean updateTime();
    DateTime getTime();
    void setTime(DateTime time);
    int setRTCTime();
    
    DateTime getAlarmTime();
    void setAlarmTime(DateTime time);
    void getRTCAlarmTime();
    void setRTCAlarmTime();
    void setAlarm(boolean on);
    boolean getAlarm();
    boolean checkAlarm();
    void stopAlarm();
    void ring();
    
    #ifdef DEBUG
      void printTime();
    #endif
    
  private:
    DateTime m_time;
    unsigned long m_lastRTCCheck;
    
    DateTime m_alarmTime;
    boolean m_alarmOn;
    boolean m_ringing;
    unsigned long m_lastRTCAlarmCheck;
    unsigned long m_lastRing;
    
    byte decToBcd(byte val);
    byte bcdToDec(byte val);
    void getRTCTime();
};

#endif
