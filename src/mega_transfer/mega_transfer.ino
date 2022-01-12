
// commands for routing messages to nano
#define TO_SS1 11
#define TO_SS2 12
#define SEND_DATA_TRUE 40
#define SEND_DATA_FALSE 41
#define SET_POS_REF 1

#include <SPI.h>
/*
   in arduino mega
   sck 52
   miso 50
   mosi 51
*/
#define SS1 53 // ss for nano 1
#define SS2 40 // ss for nano 1

#define LED_PWM 9
#define WRITE_RATE_HZ 10

#define msgSize 6
union outMsg { // receive data structure
  char bytes[msgSize];
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
union { // send data structure
    char bytes [msgSize];
    struct {
      uint8_t d1;
      uint8_t d2;
      uint8_t d3;
      uint8_t d4;
      uint8_t d5;
      uint8_t d6;
    };
} ref;

unsigned long t_write = 0;

uint8_t led_pwm = 0;
uint8_t led_st = 0;
bool sendData_st = false;
bool process_it = false;

int in = 0;
unsigned long t_print = 0;

// functions for spi communication
byte transferAndWait (const byte what) {
    byte a = SPI.transfer (what);
    delayMicroseconds (10);
    return a;
} // end of transferAndWait

void sendMsg (int ss) {
  digitalWrite(ss, LOW);
  transferAndWait(0xAA); // comando para que el nano comience a rellenar buffer
  byte a = transferAndWait(ref.d1); // primer valor enviado es el comando
  data.d1 = transferAndWait(ref.d2);
  data.d2 = transferAndWait(ref.d3);
  data.d3 = transferAndWait(ref.d4);
  data.d4 = transferAndWait(ref.d5);
  data.d5 = transferAndWait(ref.d6);
  data.d6 = transferAndWait(0xFF);
  delay(10);
  digitalWrite(ss, HIGH);
} // end of sendMsg

// functions for serial comunication
void readPacket() {
  if (Serial.available() >= msgSize) {
    // read if serial buffer have all the bytes to read
    Serial.readBytes(ref.bytes, msgSize);
    // read numBytes bytes from serial buffer and store them at a union called “ref”
    process_it = true;
  }
} // end of readPacket

void writePacket() {
    int len = sizeof(data.bytes) / sizeof(*data.bytes);
    Serial.write(data.bytes, len);
}

void parseCommand() {
  uint8_t d1 = ref.d1;
  uint8_t d2 = ref.d2;
  uint8_t d3 = ref.d3;
  uint8_t d4 = ref.d4;
  uint8_t d5 = ref.d5;
  uint8_t d6 = ref.d6;

  led_pwm = d3;

  switch (d1) {
    case TO_SS1:
      sendMsg(SS1);
      break;
    case TO_SS2:
      sendMsg(SS2);
      break;
    case SEND_DATA_TRUE:
      sendData_st = true;
      break;
    case SEND_DATA_FALSE:
      sendData_st = false;
      break;
    case SET_POS_REF:
      led_pwm = d3;
      break;
  }
  /*
  // clear data structure
  for (int i = 0; i < msgSize; i++) {
    ref.bytes[i] = 0;
  }*/
}

void setup (void)
{
  Serial.begin (500000);
  //Serial.println ("Mega main router");

  // SS = slave select, built in AVR output pin reference
  digitalWrite(SS1, HIGH);  // ensure SS stays high for now
  digitalWrite(SS2, HIGH);  // ensure SS stays high for now

  // Put SCK, MOSI, SS pins into output mode
  // also put SCK, MOSI into LOW state, and SS into HIGH state.
  // Then put SPI hardware into Master mode and turn SPI on
  SPI.begin ();

  // Slow down the master a bit
  //SPI.setClockDivider(SPI_CLOCK_DIV4);
  // use 2 Mbps decided by testing. 4 Mbps has too many bit errors over
  // jumper wires. Single-ended signals are not well suited for high-speed over wires
  SPI.beginTransaction(SPISettings(2000000, MSBFIRST, SPI_MODE0));
  //for (size_t i = 0; i < msgSize; i++) { ref.bytes[i] = 0 }
  //sendMsg(SS1); // send first msg

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(LED_PWM, OUTPUT);

  t_write = millis();
}  // end of setup

void loop (void)
{
  readPacket();
  if (process_it) {
    parseCommand();
    analogWrite(LED_PWM, led_pwm);
    process_it = false;
  }

  if (millis() - t_write > 1000 / WRITE_RATE_HZ) {
    digitalWrite(LED_BUILTIN, led_st);
    led_st = !led_st;
    if(sendData_st == true){
      writePacket();
    }
    t_write = millis();
  }
}  // end of loop
