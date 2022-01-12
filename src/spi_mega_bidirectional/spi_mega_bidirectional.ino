
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

#define msgSize 4

union { // receive data structure
  char bytes[msgSize];
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

union { // send data structure
    char bytes [msgSize];
    struct {
      uint8_t d1;
      uint8_t d2;
      uint8_t d3;
      uint8_t d4;
    };
} ref;

byte transferAndWait (const byte what) {
    byte a = SPI.transfer (what);
    delayMicroseconds (10);
    return a;
} // end of transferAndWait

void sendMsg (int ss) {
  digitalWrite(ss, LOW);
  transferAndWait(0xAA); // comando para que el nano comience a rellenar buffer
  data.d1 = transferAndWait(ref.d1); // primer valor enviado es el comando
  data.d2 = transferAndWait(ref.d2);
  data.d3 = transferAndWait(ref.d3);
  data.d4 = transferAndWait(ref.d4);
  //data.d4 = transferAndWait(0xFF);
  delay(10);
  digitalWrite(ss, HIGH);
} // end of sendMsg

int in = 0;
unsigned long t_print = 0;

void setup (void)
{
  Serial.begin (115200);
  Serial.println ("SPI Mega Master");

  pinMode(LED_PWM,OUTPUT);

  // SS = slave select, built in AVR output pin reference
  digitalWrite(SS1, HIGH);  // ensure SS stays high for now

  // Put SCK, MOSI, SS pins into output mode
  // also put SCK, MOSI into LOW state, and SS into HIGH state.
  // Then put SPI hardware into Master mode and turn SPI on
  SPI.begin ();

  // Slow down the master a bit
  //SPI.setClockDivider(SPI_CLOCK_DIV4);
  // use 2 Mbps decided by testing. 4 Mbps has too many bit errors over
  // jumper wires. Single-ended signals are not well suited for high-speed over wires
  SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE0));
/*
  ref.d1 = 0; // position control
  ref.d2 = 0;
  ref.d3 = 0;
  ref.d4 = 0;
  sendMsg(SS1); // send first msg */
}  // end of setup

void loop (void)
{
  if (Serial.available()) {
    in = Serial.parseInt();
    int aux = Serial.parseInt(); // quita el 0 que se lee siempre despues de un parseInt
    Serial.print("in: ");
    ref.d1 = in; // position control
    ref.d2 = in+1;
    ref.d3 = in;
    ref.d4 = in-1;
    Serial.println(in);
    sendMsg(SS1);
    analogWrite(LED_PWM,ref.d3);
  }

  if (millis() - t_print > 2000) {
    printRow(ref.d1, ref.d2, ref.d3, ref.d4);
    printRow(data.d1, data.d2, data.d3, data.d4);
    Serial.println("---");
    t_print = millis();
  }

}  // end of loop

void printRow(int a, int b, int c, int d){
  Serial.print("d1: ");
  Serial.print(a,DEC);
  Serial.print("\td2: ");
  Serial.print(b,DEC);
  Serial.print("\td3: ");
  Serial.print(c,DEC);
  Serial.print("\td4: ");
  Serial.println (d,DEC);
}
