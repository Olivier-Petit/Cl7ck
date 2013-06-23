/*
 * Cl7ck
 * Author : op414
 * http://op414.net
 * License : CC BY-NC-SA http://creativecommons.org/licenses/by-nc-sa/3.0/
 * ------------
 * InputsHandler.cpp : Implements InputsHandler class : handles button presses
 * Created 22 june 2013
 */

#if defined(ARDUINO) && ARDUINO >= 100
  #include <Arduino.h>
#else
  #include "WProgram.h"
#endif

#include "settings.h"
#include "InputsHandler.h"

InputsHandler::InputsHandler()
{
  // Initialize the buttons
  for(int i = 0 ; i < 4 ; i++)
  {
    pinMode(m_buttonPins[i], OUTPUT);
    m_lastDebounceTime[i] = 0UL;
    m_buttonState[i] = LOW;
    m_lastButtonState[i] = LOW;
    m_buttonPulled[i] = false;
    m_buttonTimePulled[i] = false;
  }
}

const int InputsHandler::m_buttonPins[4] = {PIN_BACK, PIN_MINUS, PIN_OK, PIN_PLUS};

void InputsHandler::updateButtonStates()
{
  for(int i = 0 ; i < 4 ; i++)
  {
    boolean reading = digitalRead(m_buttonPins[i]);
    
    if(reading != m_lastButtonState[i])
    {
      m_lastDebounceTime[i] = millis();
    }
    else if(millis() - m_lastDebounceTime[i] >= DEBOUNCE_DELAY)
    {
      if(m_buttonState[i] == LOW)
      {
        m_buttonPulled[i] = false;
        m_buttonTimePulled[i] = false;
      }
      m_buttonState[i] = reading;
    }
      
    m_lastButtonState[i] = reading;
  }
}

boolean InputsHandler::pullButtonPress(int button)
{
  if(m_buttonState[button] == HIGH && m_buttonPulled[button] == false)
  {
    m_buttonPulled[button] = true;
    return true;
  }
  
  return false;
}

boolean InputsHandler::pullButtonPress(int button, int time)
{
  if(m_buttonState[button] == HIGH && m_buttonTimePulled[button] == false && (millis() - m_lastDebounceTime[button]) > time)
  {
    m_buttonTimePulled[button] = true;
    return true;
  }
  
  return false;
}

boolean InputsHandler::getButtonState(int button)
{
  return m_buttonState[button];
}

