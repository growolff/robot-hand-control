
#include <SPI.h>


/*
   in arduino mega
   sck 52
   miso 50
   mosi 51

*/

#define SS1 53 // ss for nano 1

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

void sendMsg (fingerMsg table, int ss) {
  fingerMsgUnion msgU;
  msgU.msg = table;

  digitalWrite(ss, LOW);
  for (size_t i = 0; i < msgSize; i++)
  {
    SPI.transfer(msgU.bytes[i]);
  }
  delay(10);
  digitalWrite(ss, HIGH);
}

byte in = 0;

unsigned long t_print = 0;

void setup (void)
{
  Serial.begin (115200);
  Serial.println ("SPI demo");

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

fingerMsg msg;

void loop (void)
{
  msg.cmd = 1; // position control
  if (Serial.available()) {
    in = Serial.parseInt();
    int aux = Serial.parseInt(); // quita el 0 que se lee siempre despues de un parseInt
    Serial.print("in: ");
    msg.val = in;
    msg.val2 = msg.val;
    Serial.println(msg.val);
    sendMsg(msg, SS1);
  }

  if (millis() - t_print > 100) {
    //Serial.println (a, DEC);
    t_print = millis();
  }

}  // end of loop
