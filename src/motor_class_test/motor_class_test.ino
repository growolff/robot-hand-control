/* creado el 02-01-2021 por Gonzalo Olave
   Control de posicion de un motor BLDC con sensores de efecto hall
   utilizando la clase Motor definida en Motor.h
*/

#include <Motor.h>

#define M2_HA 2// 2
#define M2_HB 9
#define M2_DIR 5
#define M2_EN 4
#define M2_PWM 11

#define M1_HA 3
#define M1_HB 6
#define M1_DIR 7
#define M1_EN 8
#define M1_PWM 10

int16_t ref = 0;
int16_t ref1 = 0;
int16_t ref2 = 0;

int amplitude = 200;

float th = 0;

int16_t pwm_out = 0;
float kp = 3.0;
unsigned long t = 0, t_pos = 0, t_sweep = 0;

Motor* m1;
Motor* m2;
uint8_t m1_int = M1_HA;
uint8_t m2_int = M2_HA;

void m1_isr() {
  m1->readCounter();
}

void m2_isr() {
  m2->readCounter();
}

void setup() {
  Serial.begin(115200);

  m1 = new Motor(M1_HA, M1_HB, M1_DIR, M1_EN, M1_PWM);
  m1->setupInterruptHandler(m1_isr);
  m1->setPositionPID(kp, 0, 0);

  m2 = new Motor(M2_HA, M2_HB, M2_DIR, M2_EN, M2_PWM);
  m2->setupInterruptHandler(m2_isr);
  m2->setPositionPID(kp, 0, 0);

  m2->setInitPosition(amplitude);

  t = millis();
  t_pos = millis();
  t_sweep = millis();
}

void loop() {
  /*
    if (Serial.available()) {
    ref = Serial.parseInt();
    int16_t aux = Serial.parseInt(); // quita el 0 que se lee siempre despues de un parseInt
    }*/

  /*
     sweep loop
  */
  if (millis() - t_sweep > 14) { // time delay
    th += 0.04; // step size
    ref1 = amplitude * sin(th);
    ref2 = amplitude * cos(th);
    t_sweep = millis();
  }

  if (millis() - t_pos > 1) { // 1kHz loop
    m1->moveToPosition(ref1);
    m2->moveToPosition(ref2);
    t_pos = millis();
  }

  if (millis() - t > 100) {
    Serial.print("Ref1: ");
    Serial.print(ref1);
    Serial.print("\tRef2: ");
    Serial.println(ref2);

    /*
      Serial.print("Ref: ");
      Serial.print(ref);
      Serial.print("\tMes: ");
      Serial.println(m2->getPosition());
    */
    t = millis();
  }
}
