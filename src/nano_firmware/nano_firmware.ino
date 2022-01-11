
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
#define msgSize 3

typedef struct fingerMsg {
  uint8_t cmd;
  uint8_t val;
  uint8_t val2;
} fM;

typedef union fingerMsgUnion {
  char bytes[msgSize];
  fingerMsg msg;
} fU;

char buf[100];
volatile byte pos;
volatile boolean process_it;

bool newMessage = false;

int led_st = 0;

Motor* m1;
Motor* m2;
uint8_t m1_int = M1_HA;
uint8_t m2_int = M2_HA;

int16_t ref1 = 0;
int16_t ref2 = 0;
float kp = 3.0;
int amplitude = 0;

float th = 0;

unsigned long t_pos = 0;

void m1_isr() {
  m1->readCounter();
}

void m2_isr() {
  m2->readCounter();
}

void setup (void)
{
  m1 = new Motor(M1_HA, M1_HB, M1_DIR, M1_EN, M1_PWM);
  m1->setupInterruptHandler(m1_isr);
  m1->setPositionPID(kp, 0, 0);

  m2 = new Motor(M2_HA, M2_HB, M2_DIR, M2_EN, M2_PWM);
  m2->setupInterruptHandler(m2_isr);
  m2->setPositionPID(kp, 0, 0);
  m2->setInitPosition(amplitude);

  t_pos = micros();

  pinMode(LED_PIN, OUTPUT);
  analogWrite(LED_PIN, led_st);

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

  buf[pos] = c;
  pos++;
  if (pos >= msgSize) {
    process_it  = true;
  }
  SPDR = m1->getPosition();
}  // end of interrupt service routine (ISR) SPI_STC_vect

fingerMsg recvMsg;

void loop (void)
{
  if (process_it)
  {
    buf[pos] = 0;
    fingerMsgUnion msgUnion;
    for (size_t i = 0; i < sizeof(fM); i++)
    {
      msgUnion.bytes[i] = buf[i];
    }
    recvMsg = msgUnion.msg;
    newMessage = true;
    pos = 0;
    process_it = false;
  }
  if (newMessage) {
    ref1 = recvMsg.val;
    ref2 = recvMsg.val2;
    analogWrite(LED_PIN, ref1);
    newMessage = false;
  }
  if (micros() - t_pos > 1000) { // 1kHz loop
      m1->moveToPosition(ref1);
      m2->moveToPosition(ref2);
      t_pos = micros();
  }
}  // end of loop
