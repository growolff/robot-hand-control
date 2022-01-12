#define LED_PIN 9
#define msgSize 4

union { // data out structure
  char bytes [msgSize];
  struct {
    uint8_t d1;
    uint8_t d2;
    uint8_t d3;
    uint8_t d4;
  };
  struct {
    uint8_t pos0;
    uint8_t pos1;
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
  };
} ref;

char buf[100];
volatile byte pos = 0;
volatile byte command = 0;
volatile boolean process_it;

bool newMessage = false;

int led_pwm = 0;

void setup (void)
{
  pinMode(LED_PIN, OUTPUT);
  analogWrite(LED_PIN,led_pwm);

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
      SPDR = data.bytes[pos]; // devuelve 0 que no lee nadie
      pos++;
      //command = 0xBB;
      break;
    case 0XBB:
      buf[pos] = c;
      SPDR = data.bytes[pos-1];
      pos++;
      break;
  }
  if (pos >= msgSize) {
    process_it  = true;
    command = 0;
  }
}  // end of interrupt service routine (ISR) SPI_STC_vect

void loop (void) {
  buf[pos] = 0;
  if (process_it) {
    for (size_t i = 0; i < msgSize; i++)
    {
      ref.bytes[i] = buf[i];
      data.bytes[i] = ref.bytes[i];
    }
    pos = 0;
    newMessage = true;
    process_it = false;
    for (size_t i = 0; i < msgSize; i++)
    {
      buf[i] = 0;
    }
  }

  if (newMessage) {
    led_pwm = ref.d3;
    analogWrite(LED_PIN, led_pwm);
    newMessage = false;
  }

  if (digitalRead (SS) == HIGH) {
    command = 0;
  }
}  // end of loop
