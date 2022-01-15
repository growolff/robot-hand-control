
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

#define SET_POS_REF     0x01
#define GET_MOTOR_POS   0xEE
#define ENABLE_MOTOR_F  0xAF
#define ENABLE_MOTOR_E  0xAE
#define DISABLE_MOTOR_F 0xBF
#define DISABLE_MOTOR_E 0xBE

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

unsigned long t_pos = 0;

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

  // motor 2 extensor
  m2 = new Motor(M2_HA, M2_HB, M2_DIR, M2_EN, M2_PWM);
  m2->setupInterruptHandler(m2_isr);
  m2->setPositionPID(kp, ki, 0);
  //m2->setInitPosition(0);

  t_pos = micros();

  pinMode(LED_PIN, OUTPUT);
  analogWrite(LED_PIN, led_pwm);

  // have to send on master in, *slave out*
  pinMode(MISO, OUTPUT);
  digitalWrite(MISO, LOW); //ensure is low to start

  // turn on SPI in slave mode
  SPCR |= _BV(SPE);

  // turn on interrupts
  SPCR |= _BV(SPIE);

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
      case GET_MOTOR_POS: // pide info de los sensores
        data.pos0 = m1->getPosition();
        data.pos1 = m2->getPosition();
        break;
      case SET_POS_REF: // entrega referencias de posicion
        ref1 = ref.d3;
        ref2 = ref.d4;
        data.pos0 = m1->getPosition();
        data.pos1 = m2->getPosition();
        break;
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

  if (micros() - t_pos > 1000) { // 1kHz loop
      m1->moveToPosition(ref1);
      m2->moveToPosition(ref2);
      t_pos = micros();
  }
  if (digitalRead (SS) == HIGH) {
    command = 0;
  }
}  // end of loop
