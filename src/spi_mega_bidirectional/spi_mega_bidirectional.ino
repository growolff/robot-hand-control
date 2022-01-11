
#include <SPI.h>

/*
   in arduino mega
   sck 52
   miso 50
   mosi 51
*/

#define SS1 53 // ss for nano 1
#define SS2 40 // ss for nano 1

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
  transferAndWait(0xAA); // primer valor enviado es el comando
  data.d1 = transferAndWait(ref.d1);
  data.d2 = transferAndWait(ref.d2);
  data.d3 = transferAndWait(ref.d3);
  data.d4 = transferAndWait(ref.d4);
  //data.d4 = transferAndWait(0xFF); // ultimo valor enviado para cerrar comunicacion
  delay(10);
  digitalWrite(ss, HIGH);
}

byte in = 0;

unsigned long t_print = 0;

void setup (void)
{
  Serial.begin (115200);
  Serial.println ("SPI Mega Master");

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
  SPI.beginTransaction(SPISettings(2000000, MSBFIRST, SPI_MODE0));

}  // end of setup

void loop (void)
{
  if (Serial.available()) {
    in = Serial.parseInt();
    int aux = Serial.parseInt(); // quita el 0 que se lee siempre despues de un parseInt
    Serial.print("in: ");
    ref.d1 = in; // position control
    ref.d2 = in+1;
    ref.d3 = in-1;
    ref.d4 = in+10;
    Serial.println(in);
    sendMsg(SS1);
  }

  if (millis() - t_print > 1000) {
    Serial.print("d1: ");
    Serial.print(data.d1,DEC);
    Serial.print("\td2: ");
    Serial.print(data.d2,DEC);
    Serial.print("\td3: ");
    Serial.print(data.d3,DEC);
    Serial.print("\td4: ");
    Serial.println (data.d4,DEC);
    t_print = millis();
  }

}  // end of loop
