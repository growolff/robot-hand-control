/*
   Motor.cpp - Libreria para control de motores BLDC
   Creado por Gonzalo Olave el 02-01-2021
   publicado bajo licencia MIT
*/

#include "Arduino.h"
#include "Motor.h"

Motor::Motor(uint8_t ha, uint8_t hb, uint8_t dir, uint8_t en, uint8_t pwm)
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

  _pkp = 1.0;
  _pki = 1.0;

  _tkp = 1.0;

  iValue = new ILim(_pki,0.001f,255.0); // kI, Ts, Lim

  digitalWrite(p_en,LOW);
}

void Motor::enable()
{
  digitalWrite(p_en,HIGH);
}

void Motor::disable()
{
  digitalWrite(p_en,LOW);
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

  iValue->setKi(_pki);
}

void Motor::setTensionPID(float kp, float ki, float kd)
{
  _tkp = kp;
  _tki = ki;
  _tkd = kd;

  //iValue->setKi(_pki);
}

int16_t Motor::getPosition()
{
  return _counter;
}

void Motor::setInitPosition(int16_t pos)
{
  _counter = pos;
}

void Motor::moveToPosition(int16_t ref)
{
  _pos_err = ref - _counter;
  int16_t pwm_out =  (int16_t) _pos_err * _pkp + iValue->update(_pos_err);

  if (pwm_out > CTRL_MAX_VAL) pwm_out = CTRL_MAX_VAL;
  else if (pwm_out < -CTRL_MAX_VAL) pwm_out = -CTRL_MAX_VAL;

  if (pwm_out < 0) {
    digitalWrite(p_dir, LOW);
    pwm_out = pwm_out * -1;
  }
  else if (pwm_out > 0) digitalWrite(p_dir, HIGH);

  analogWrite(p_pwm, pwm_out);
}

void Motor::moveToTension(int16_t ref, int16_t sensor_mes)
{
    _tens_err = ref - sensor_mes;
    int16_t tens_out =  (int16_t) _tens_err * _tkp;

    // limita la referencia de posicion al maximo o minimo que se permita
    tens_out = tens_out > CTRL_MAX_VAL ? CTRL_MAX_VAL : (tens_out < -CTRL_MAX_VAL ? -CTRL_MAX_VAL : tens_out);

    if(tens_out < 0){ // cambia de dirección el motor dependiendo de si el error es positivo o negativo
        digitalWrite(p_dir, LOW);
        tens_out = tens_out * -1;
    }
    else if (tens_out > 0) digitalWrite(p_dir, HIGH);

    analogWrite(p_pwm, (uint8_t)tens_out);
}

void Motor::readCounter()
{
  _hb = digitalRead(p_hb);
  _counter += _hb ? 1 : -1;
}
