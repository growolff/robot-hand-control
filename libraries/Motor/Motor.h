/*
 * Motor.h - Libreria para control de motores BLDC
 * Creado por Gonzalo Olave el 02-01-2021
 * publicado bajo licencia MIT
 */

#ifndef Motor_h
#define Motor_h

#include "Arduino.h"

#define CTRL_MAX_VAL 255

class ILim
{
  public:
    float a[2];
    float b[2];
    float state[1];
    float Ts;
    float lim;

  public:
    ILim(float ki, float Ts, float lim)
    {
      this->Ts = Ts;
      this->lim = lim;
      a[0] = 1.0f;
      a[1] = -1.0f;
      b[0] = Ts*ki/2.0f;
      b[1] = Ts*ki/2.0f;
    }
    setKi(float ki)
    {
      b[0] = Ts*ki/2.0f;
      b[1] = Ts*ki/2.0f;
    }

    float update(float in)
    {
        float out ;
        /* y [ n ] = b0 * x [ n ] + s [ n ] */
        out = this->b[0] * in + this->state[0];
        /* Limit output , conditional integration antiwindup */
        out = out > this->lim ? this-> lim : (out < -this->lim ? -this->lim : out);
        /* Update state */
        /* s [ n +1] = b1 * x [ n ] - a1 * y [ n ] */
        this->state[0] = this-> b[1] * in + out ; // a1 = -1.0 f ;
        /* Return I output */
        return (out);
    }
};

class Motor
{
  public:
    Motor(uint8_t ha, uint8_t hb, uint8_t dir, uint8_t en, uint8_t pwm);
    void setupInterruptHandler(void (*ISR)(void));
    void readCounter(); // isr

    void setInitPosition(int16_t pos);
    void setPositionPID(float kp, float ki, float kd);
    void setTensionPID(float kp, float ki, float kd);
    void moveToPosition(int16_t ref);
    void moveToTension(int16_t ref, int16_t sensor_mes);

    int16_t getPosition();
    void enable();
    void disable();


  private:
    uint8_t p_ha;
    uint8_t p_hb;
    uint8_t p_dir;
    uint8_t p_en;
    uint8_t p_pwm;

    ILim* iValue;
    float _pkp,_pki,_pkd;
    int16_t _pos_err;
    int16_t _counter;
    int _hb;

    float _tkp,_tki,_tkd;
    int16_t _tens_err;
};

#endif
