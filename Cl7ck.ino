/*
 * Cl7ck
 * Author : op414
 * http://op414.net
 * License : CC BY-NC-SA http://creativecommons.org/licenses/by-nc-sa/3.0/
 * ------------
 * Cl7ck.ino : Main file
 * Created 11 may 2012
 */

#include <SPI.h>
#include <Wire.h>
#include <avr/pgmspace.h>

#include "settings.h"
#include "Display.h"
#include "font.h"
#include "TimeHandler.h"
#include "InputsHandler.h"

prog_char fontSet[FONT_SET_SIZE] PROGMEM = {
  f_0,f_1,f_2,f_3,f_4,f_5,f_6,f_7,f_8,f_9,
  f_A,f_B,f_C,f_D,f_E,f_F,f_G,f_H,f_I,f_J,f_K,f_L,f_M,f_N,f_O,f_P,f_Q,f_R,f_S,f_T,f_U,f_V,f_W,f_X,f_Y,f_Z,
  f_a,f_b,f_c,f_d,f_e,f_f,f_g,f_h,f_i,f_j,f_k,f_l,f_m,f_n,f_o,f_p,f_q,f_r,f_s,f_t,f_u,f_v,f_w,f_x,f_y,f_z};
  
// Lambda enumeration for mode keeping
enum{TIME = 0, DATE = 1};

Display disp(fontSet);
TimeHandler rtc;
InputsHandler inputs;

int mode = TIME;


void setup()
{
  Serial.begin(9600);

  rtc.initializeRTC();

  pinMode(PIN_MINUS, OUTPUT);
  pinMode(PIN_BUZZER, OUTPUT);
  //tone(PIN_BUZZER, 2100);
  //delay(500);
  //noTone(PIN_BUZZER);

  disp.clearBuffer();
  disp.setString(0, "Hello");
  disp.display();
  delay(1000);
  disp.clearBuffer();
}

void loop()
{
  inputs.updateButtonStates();
  
  // ChangeMode
  if(inputs.pullButtonPress(OK))
  {
    disp.clearBuffer();
    mode = !mode;
  }
  
  
  // Update display
  if(mode == TIME)
  {
    if(rtc.updateTime())
    {
      DateTime t = rtc.getTime();
      
      disp.setString(0, t.hours, 2);
      disp.setString(3, t.mins, 2);
      disp.setString(6, t.secs, 2);
      disp.display();
    }
  }
  else if(mode == DATE)
  {
    if(rtc.updateTime())
    {
      DateTime t = rtc.getTime();
      
      disp.setString(0, t.DOM, 2);
      disp.setString(3, t.month, 2);
      disp.setString(6, t.year, 2);
      disp.display();
    }
  }
}

