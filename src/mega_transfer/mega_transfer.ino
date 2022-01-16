
// commands for routing messages to nano
#define TO_SS1          0xC1
#define TO_SS2          0xC2
#define TO_SS3          0xC3
#define SEND_DATA_TRUE  0xD1
#define SEND_DATA_FALSE 0xD0

#include <SPI.h>
/*
   in arduino mega
   sck 52
   miso 50
   mosi 51
*/
#define SS1 40 // ss for nano 1
#define SS2 53 // ss for nano 2
#define SS3 41 // ss for nano 3

#define SENSOR_0 A0
int l_val = 0;        // value read from the sensor
int r_val = 0;
float l_force = 0;
float r_force = 0;
float c_factor = 0.2908;

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
  delay(5);
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
    case TO_SS3:
      sendMsg(SS3);
      break;
    case SEND_DATA_TRUE:
      sendData_st = true;
      break;
    case SEND_DATA_FALSE:
      sendData_st = false;
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
  pinMode(SS1,OUTPUT);
  pinMode(SS2,OUTPUT);
  pinMode(SS3,OUTPUT);
  digitalWrite(SS1, HIGH);  // ensure SS stays high for now
  digitalWrite(SS2, HIGH);  // ensure SS stays high for now
  digitalWrite(SS3, HIGH);  // ensure SS stays high for now

  // Put SCK, MOSI, SS pins into output mode
  // also put SCK, MOSI into LOW state, and SS into HIGH state.
  // Then put SPI hardware into Master mode and turn SPI on
  SPI.begin ();

  // Slow down the master a bit
  //SPI.setClockDivider(SPI_CLOCK_DIV4);
  // use 2 Mbps decided by testing. 4 Mbps has too many bit errors over
  // jumper wires. Single-ended signals are not well suited for high-speed over wires
  SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE0));
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
      //data.sensor = readForceSensor(SENSOR_0);
      writePacket();
    }
    t_write = millis();
  }
  //Serial.println(readForceSensor(SENSOR_0));
}  // end of loop

uint16_t readForceSensor(int sensor){
  // read ADC values
  l_val = smooth(sensor);
  l_force = l_val * 5.0/1024.0*c_factor;
  // using the ecuation for equilibrum state
  float Df = 0.017; // distance in meters
  float de = 0.0085;
  float alpha = atan(0.003/de) * 3.14159 / 180; // in radians
  float beta = atan(0.003/Df) * 3.14159 / 180; // in radians
  float l_tens = Df*l_force*cos(alpha) / (de * cos(beta));

  return l_tens*1000;
}

int smooth(const int sensor){
  int i;
  int value = 0;
  int numReadings = 10;

  for (i = 0; i < numReadings; i++){
    // Read light sensor data.
    value = value + analogRead(sensor);

    // 1ms pause adds more stability between reads.
    delay(1);
  }
  // Take an average of all the readings.
  value = value / numReadings;
  // Scale to 8 bits (0 - 255).
  // value = value / 4;
  return value;
}
