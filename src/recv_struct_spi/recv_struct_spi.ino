#define LED_PIN 5
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

uint16_t adcReading = 0;
static int32_t avg1 = 0;

void setup (void)
{
  pinMode(LED_PIN, OUTPUT);
  analogWrite(LED_PIN,led_st);

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
}  // end of interrupt service routine (ISR) SPI_STC_vect

fingerMsg recvMsg;

void loop (void)
{
  if (newMessage) {
    led_st = recvMsg.val2;
    analogWrite(LED_PIN, led_st);
    newMessage = false;
  }
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
}  // end of loop
