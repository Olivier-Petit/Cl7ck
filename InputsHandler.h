/*
 * Cl7ck
 * Author : op414
 * http://op414.net
 * License : CC BY-NC-SA http://creativecommons.org/licenses/by-nc-sa/3.0/
 * ------------
 * Inputs Handler.h : InputsHandler class definition
 * Created 22 june 2013
 */


#ifndef DEF_INPUTSHANDLER
#define DEF_INPUTSHANDLER

// Lambda enumeration for button names
enum{BACK = 0, MINUS = 1, OK = 2, PLUS = 3};

#define DEBOUNCE_DELAY 50UL

class InputsHandler
{
  public:
    InputsHandler();
    void updateButtonStates();
    boolean pullButtonPress(int button);
    boolean pullButtonPress(int button, int time);
    boolean getButtonState(int button);
  
  private:
    unsigned long m_lastDebounceTime[4];
    boolean m_buttonState[4];
    boolean m_lastButtonState[4];
    boolean m_buttonPulled[4];
    static const int m_buttonPins[4];
};

#endif

