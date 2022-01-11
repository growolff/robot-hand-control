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

int led_st = 0;

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

  switch (command) {
    case 0: // si no hay comando el primer byte recibido es el comando
      command = c;
      SPDR = 0;  // devuelve 0 que no lee nadie
      break;
    case 0xAA: // si llega este comando primero rellena ref y envia data
      buf[pos] = c; // rellena el buffer con los datos que lleguen a continuacion
      SPDR = data.bytes[pos];
      pos++;
      break;
  }
  if (pos >= msgSize) {
    process_it  = true;
  }
}  // end of interrupt service routine (ISR) SPI_STC_vect

void loop (void) {

  if (process_it) {
    for (size_t i = 0; i < msgSize; i++)
    {
      ref.bytes[i] = buf[i];
    }
    pos = 0;
    newMessage = true;
    process_it = false;
  }

  if (newMessage) {
    led_st = ref.d1;
    data.d1 = ref.d1;
    data.d2 = ref.d2;
    data.d3 = ref.d3;
    data.d4 = ref.d4;
    analogWrite(LED_PIN, led_st);
    newMessage = false;
  }

  if (digitalRead (SS) == HIGH) {
    command = 0;
  }
}  // end of loop
