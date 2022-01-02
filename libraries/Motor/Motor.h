/*
 * Motor.h - Libreria para control de motores BLDC
 * Creado por Gonzalo Olave el 02-01-2021
 * publicado bajo licencia MIT
 */

#ifndef Motor_h
#define Motor_h

#include "Arduino.h"

class Motor
{
  public:
    Motor(uint8_t ha, uint8_t hb, uint8_t dir, uint8_t en, uint8_t pwm);
    void setupInterruptHandler(void (*ISR)(void));
    void ha_isr();

    int16_t getPosition();
    void setPositionPID(float kp, float ki, float kd);
    void moveToPosition(int16_t pos);


  private:
    uint8_t p_ha;
    uint8_t p_hb;
    uint8_t p_dir;
    uint8_t p_en;
    uint8_t p_pwm;

    float _pkp,_pki,_pkd;
    int16_t _pos_err;
    int16_t _counter;
    int _hb;
};

#endif
