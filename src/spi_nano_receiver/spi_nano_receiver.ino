
#define DSP_EMA_I32_ALPHA(x) ( (uint16_t)(x * 65535) )

#define LED_PIN 4

volatile byte command = 0;
volatile byte out1 = 0;

int led_st = 1;

uint16_t adcReading = 0;
static int32_t avg1 = 0;

void setup (void)
{
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH);

  // have to send on master in, *slave out*
  pinMode(MISO, OUTPUT);
  digitalWrite(MISO, LOW); //ensure is low to start

  // turn on SPI in slave mode
  SPCR |= _BV(SPE);

  // turn on interrupts
  SPCR |= _BV(SPIE);

}  // end of setup

int32_t dsp_ema_i32(int32_t in, int32_t average, uint16_t alpha) {
  int64_t tmp0;
  tmp0 = (int64_t)in * (int64_t)(alpha) + (int64_t)average * (int64_t)(65536 - alpha);
  return (int32_t)((tmp0 + 32768) / 65536);
}

// SPI interrupt routine
ISR (SPI_STC_vect)
{
  byte c = SPDR; // received char

  /*
    buf[pos] = c;
    pos++;
    if (pos >= sizeof(fM)) {
    haveData = true;
    }
  */
  switch (command)
  {
    case 0:
      command = c;
      break;
    case '1':
      led_st = HIGH;
      break;
    case '2':
      led_st = LOW;
      break;
  } // end of switch

  SPDR = out1;
}  // end of interrupt service routine (ISR) SPI_STC_vect


void loop (void)
{
  /*
  if (haveData) {
    buf[pos] = 0;
    fingerMsgUnion msgU;
    for (size_t i = 0; i < sizeof(fM); i++)
    {
      msgU.bytes[i] = buf[i];
    }

    recvMsg = msgU.msg;
    pos = 0;
    haveData = false;
  }*/

  if(digitalRead(SS) == HIGH){
    command = 0;
  }

  digitalWrite(LED_PIN, led_st);

  adcReading = analogRead(A0);
  avg1 = dsp_ema_i32( (int32_t)adcReading << 21, avg1, DSP_EMA_I32_ALPHA(0.1));
  out1 = byte(avg1 >> 23 ); // shift down to single byte

}  // end of loop
