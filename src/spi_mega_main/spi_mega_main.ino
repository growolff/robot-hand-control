
#include <SPI.h>
//#include "SPI_anything.h"

/*
   in arduino mega
   sck 52
   miso 50
   mosi 51

*/

#define SS_1 53 // ss for nano 1

typedef struct fingerMsg {
  uint8_t cmd;
  uint8_t val;
} fM;

typedef union fingerMsgUnion {
  unsigned char bytes[sizeof(fM)];
  fM msg;
} fU;

byte sendMsg (fingerMsg table, int ss){
  fingerMsgUnion msgU;
  msgU.msg = table;
  byte a = 0; // variable to store SPI input data from Nano

  digitalWrite(ss, LOW);
  for (size_t i = 0; i < sizeof(fM); i++)
  {
    a = SPI.transfer(msgU.bytes[i]);
  }
  delay(1);
  digitalWrite(ss, HIGH);

  /*
  digitalWrite(ss, LOW);
  SPI_writeAnything(table);
  
  delay(10);
  digitalWrite(ss, HIGH);
  */
  return a;
}

byte in = 0;

unsigned long t_print = 0;

void setup (void)
{
  Serial.begin (115200);
  Serial.println ("SPI demo");

  // SS = slave select, built in AVR output pin reference
  digitalWrite(SS_1, HIGH);  // ensure SS stays high for now

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

fingerMsg msg;
byte a = 0; // variable to store SPI input data from Nano

void loop (void)
{
  //msg.cmd = 1; // position control
  if (Serial.available()) {
    in = Serial.read();
    int aux = Serial.parseInt(); // quita el 0 que se lee siempre despues de un parseInt
    Serial.print("in: ");
    Serial.println(in);
    //msg.val = in;
  }
  
  // get a value from the SPI - we're the master so have to ask for it from slave
  // enable Slave Select
  digitalWrite(SS_1, LOW);
  a = SPI.transfer (in);
  // disable Slave Select
  delay(10);
  digitalWrite(SS_1, HIGH);
  
  if (millis() - t_print > 100) {
    Serial.println (a, DEC);
    t_print = millis();
  }


}  // end of loop
