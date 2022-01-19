/*
 Started with example code written by Nathan Seidle from SparkFun Electronics and added
 LCD output with gram and ounce values.

 Setup your scale and start the sketch WITHOUT a weight on the scale
 Once readings are displayed place the weight on the scale
 Press +/- or a/z to adjust the calibration_factor until the output readings match the known weight
 Arduino pin 6 -> HX711 CLK
 Arduino pin 5 -> HX711 DOUT
 Arduino pin 5V -> HX711 VCC
 Arduino pin GND -> HX711 GND

 The HX711 board can be powered from 2.7V to 5V so the Arduino 5V power should be fine.

 The HX711 library can be downloaded from here: https://github.com/bogde/HX711
*/

#include "HX711.h"

#define DOUT 5
#define CLK  6

HX711 scale;

float calibration_factor = 940;
float units;
float ounces;

unsigned long t_print = 0;

void setup() {
  Serial.begin(115200);

  scale.begin(DOUT, CLK);

  scale.set_scale();
  scale.tare();  //Reset the scale to 0

  long zero_factor = scale.read_average(); //Get a baseline reading
  //Serial.print("Zero factor: "); //This can be used to remove the need to tare the scale. Useful in permanent scale projects.
  //Serial.println(zero_factor);
  scale.set_scale(calibration_factor); //Adjust to this calibration factor

  t_print = millis();
}

void loop() {


  //Serial.print("Reading: ");
  units = scale.get_units(), 10;
  if (units < 0) {
    units = 0.00;
  }
  //ounces = units * 0.035274;

  if(millis() - t_print > 10) {
      Serial.println(units);
      t_print = millis();
  }
  //Serial.print(" grams");
  //Serial.print(" calibration_factor: ");
  //Serial.print(calibration_factor);
  //Serial.println();
/*
  if(Serial.available())
  {
    char temp = Serial.read();
    if(temp == '+' || temp == 'a')
      calibration_factor += 1;
    else if(temp == '-' || temp == 'z')
      calibration_factor -= 1;
  }
  */
}
