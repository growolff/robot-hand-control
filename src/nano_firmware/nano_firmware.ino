
#include <Motor.h>

#define M1_HA 2
#define M1_HB A1
#define M1_DIR 7
#define M1_EN 8
#define M1_PWM 6

#define M2_HA 3
#define M2_HB A3
#define M2_DIR 4
#define M2_EN A2
#define M2_PWM 5

#define LED_PIN 9
#define msgSize 6
#define FORCE_PIN A6

#define SET_CONTROL     0x83
#define SET_POS_REF     0x01
#define SET_TENS_REF    0x02
#define GET_MOTOR_POS   0xEE
#define ENABLE_MOTOR_F  0xAF
#define ENABLE_MOTOR_E  0xAE
#define DISABLE_MOTOR_F 0xBF
#define DISABLE_MOTOR_E 0xBE

#define POSITION_CONTROL 0xDA
#define TENSION_CONTROL 0xDB

#define LED_DEBUG       0xCC

union { // data out structure
  char bytes [msgSize];
  struct {
    uint8_t d1;
    uint8_t d2;
    uint8_t d3;
    uint8_t d4;
    uint8_t d5;
    uint8_t d6;
  };
  struct {
    int16_t pos0;
    int16_t pos1;
    uint16_t sensor;
  };
} data;

union { // data in structure
  char bytes [msgSize];
  struct
  {
    uint8_t d1;
    uint8_t d2;
    uint8_t d3;
    uint8_t d4;
    uint8_t d5;
    uint8_t d6;
  };
} ref;

char buf[100];
volatile byte pos = 0;
volatile byte command = 0;
volatile boolean process_it;

bool newMessage = false;

int led_pwm = 0;

Motor* m1;
Motor* m2;
uint8_t m1_int = M1_HA;
uint8_t m2_int = M2_HA;

int16_t ref1 = 0;
int16_t ref2 = 0;
float kp = 3.0;
float ki = 2.0;

float tkp = 1.2;

int16_t t_ref = 0;

unsigned long t_pos = 0;
unsigned long t_tens = 0;

#define M1_MAX_POS 160
#define M2_MAX_POS 160

byte control_mode = POSITION_CONTROL;

float tension;
uint16_t adcReading1 = 0;
static int32_t avg1 = 0;
int sensor1 = 0;

double K = 0.011159057404310219;

#define DSP_EMA_I32_ALPHA(x) ( (uint16_t)(x * 65535) )
int32_t dsp_ema_i32(int32_t in, int32_t average, uint16_t alpha) {
  int64_t tmp0;
  tmp0 = (int64_t)in * (int64_t)(alpha) + (int64_t)average * (int64_t)(65536 - alpha);
  return (int32_t)((tmp0 + 32768) / 65536);
}


void m1_isr() {
  m1->readCounter();
}

void m2_isr() {
  m2->readCounter();
}

void setup (void)
{
  // motor 1 flector
  m1 = new Motor(M1_HA, M1_HB, M1_DIR, M1_EN, M1_PWM);
  m1->setupInterruptHandler(m1_isr);
  m1->setPositionPID(kp, ki, 0);
  m1->setTensionPID(tkp,0,0);

  // motor 2 extensor
  m2 = new Motor(M2_HA, M2_HB, M2_DIR, M2_EN, M2_PWM);
  m2->setupInterruptHandler(m2_isr);
  m2->setPositionPID(kp, ki, 0);
  //m2->setInitPosition(0);

  pinMode(LED_PIN, OUTPUT);
  analogWrite(LED_PIN, led_pwm);

  // have to send on master in, *slave out*
  pinMode(MISO, OUTPUT);
  digitalWrite(MISO, LOW); //ensure is low to start

  // turn on SPI in slave mode
  SPCR |= _BV(SPE);

  // turn on interrupts
  SPCR |= _BV(SPIE);

  t_pos = micros();
  t_tens = millis();

}  // end of setup


// SPI interrupt routine
ISR (SPI_STC_vect)
{
  byte c = SPDR; // received char

  switch (command) {
    case 0: // si no hay comando el primer byte recibido es el comando 0xAA
      command = c;
      SPDR = 0;  // devuelve 0 que no lee nadie
      break;
    case 0xAA: // el primer mensaje ref.d1 llega aca
      buf[pos] = c; // rellena el buffer con los datos que lleguen a continuacion
      SPDR = data.bytes[pos]; // devuelve estructura de datos
      pos++;
      break;
  }
  if (pos >= msgSize) {
    process_it  = true;
    command = 0;
  }
}  // end of interrupt service routine (ISR) SPI_STC_vect

void loop (void)
{
  if (process_it) {
    buf[pos] = 0;
    // copy msg from buffer
    for (size_t i = 0; i < msgSize; i++) { ref.bytes[i] = buf[i]; }
    pos = 0;
    newMessage = true;
    process_it = false;
    // clean buffer
    for (size_t i = 0; i < msgSize; i++) { buf[i] = 0; }
  }

  if (newMessage) {
    switch (ref.d2) {
      case SET_CONTROL:
        control_mode = ref.d5;
      case GET_MOTOR_POS: // pide info de los sensores
        data.pos0 = m1->getPosition();
        data.pos1 = m2->getPosition();
        data.sensor = (int16_t)tension;
        break;
      case SET_POS_REF: // entrega referencias de posicion
        ref1 = ref.d3;
        ref2 = ref.d4;
        data.pos0 = m1->getPosition();
        data.pos1 = m2->getPosition();
        data.sensor = (int16_t)tension;
        break;
      case SET_TENS_REF:
        t_ref = ref.d3;
        ref2 = ref.d4;
      case ENABLE_MOTOR_F: // enable motor F
        m1->enable();
        break;
      case ENABLE_MOTOR_E: // enable motor E
        m2->enable();
        break;
      case DISABLE_MOTOR_F: // enable motor F
        m1->disable();
        break;
      case DISABLE_MOTOR_E: // enable motor E
        m2->disable();
        break;
      case LED_DEBUG:
        led_pwm = ref.d3;
    }
    analogWrite(LED_PIN, led_pwm);
    newMessage = false;
  }

  adcReading1 = analogRead(FORCE_PIN);
  avg1 = dsp_ema_i32( (int32_t)adcReading1 << 21, avg1, DSP_EMA_I32_ALPHA(0.2));
  sensor1 = byte(avg1 >> 23 ); // shift down to single byte

  if (millis() - t_tens > 10){  // 100Hz loop
      tension = K * sensor1;
      if(control_mode == TENSION_CONTROL){
          m1->moveToTension(t_ref,sensor1);
          m2->moveToPosition(ref2 > M2_MAX_POS ? M2_MAX_POS : ref2);
      }
      t_tens = millis();
  }

  if (micros() - t_pos > 1000) { // 1kHz loop
      if(control_mode != TENSION_CONTROL){
          m1->moveToPosition(ref1 > M1_MAX_POS ? M1_MAX_POS : ref1);
          m2->moveToPosition(ref2 > M2_MAX_POS ? M2_MAX_POS : ref2);
      }
      t_pos = micros();
  }
  if (digitalRead (SS) == HIGH) {
    command = 0;
  }
}  // end of loop
