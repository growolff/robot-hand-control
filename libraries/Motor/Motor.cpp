/*
   Motor.cpp - Libreria para control de motores BLDC
   Creado por Gonzalo Olave el 02-01-2021
   publicado bajo licencia MIT
*/

#include "Arduino.h"
#include "Motor.h"

Motor::Motor(uint8_t ha, uint8_t hb, uint8_t dir, uint8_t en, uint8_t pwm)
: p_ha{p_ha}
, p_hb{p_hb}
, p_dir{p_dir}
, p_en{p_en}
, p_pwm{p_pwm}
{
  pinMode(ha, INPUT_PULLUP);
  pinMode(hb, INPUT);
  pinMode(dir, OUTPUT);
  pinMode(en, OUTPUT);
  pinMode(pwm, OUTPUT);
  p_ha = ha;
  p_hb = hb;
  p_dir = dir;
  p_en = en;
  p_pwm = pwm;

  _hb = 0;
  _counter = 0;
  _pos_err = 0;

  digitalWrite(en,HIGH);
}

void Motor::setupInterruptHandler(void (*ISR)(void))
{
  attachInterrupt(digitalPinToInterrupt(p_ha), ISR, RISING);
}

void Motor::setPositionPID(float kp, float ki, float kd)
{
  _pkp = kp;
  _pki = ki;
  _pkd = kd;
}

int16_t Motor::getPosition()
{
  return _counter;
}

void Motor::moveToPosition(int16_t pos)
{
  _pos_err = pos - _counter;
  int16_t pwm_out =  (int16_t) _pos_err * _pkp;

  if (pwm_out > 255) pwm_out = 255;
  else if (pwm_out < -255) pwm_out = -255;

  if (pwm_out < 0) {
    digitalWrite(p_dir, LOW);
    pwm_out = pwm_out * -1;
  }
  else if (pwm_out > 0) digitalWrite(p_dir, HIGH);

  analogWrite(p_pwm, pwm_out);
}

void Motor::ha_isr()
{
  _hb = digitalRead(p_hb);
  _counter += _hb ? -1 : 1;
}
