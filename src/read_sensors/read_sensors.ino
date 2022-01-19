
#define DSP_EMA_I32_ALPHA(x) ( (uint16_t)(x * 65535) )

#define SI A0  // Analog input pin that the left sensor is attached to
#define SP A1
#define SM A2

float I_force = 0;
float M_force = 0;
float P_force = 0;

float c_factor = 0.2908;

unsigned long t_print = 0;
unsigned long t_read_celda = 0;

uint16_t adcReading1 = 0;
uint16_t adcReading2 = 0;
uint16_t adcReading3 = 0;
static int32_t avg1 = 0;
static int32_t avg2 = 0;
static int32_t avg3 = 0;

int sensor1 = 0;
int sensor2 = 0;
int sensor3 = 0;

#include "HX711.h"

#define DOUT 5
#define CLK  6

HX711 scale;

float calibration_factor = 940500;
float units;

double K = 0.011159057404310219;

int32_t dsp_ema_i32(int32_t in, int32_t average, uint16_t alpha) {
  int64_t tmp0;
  tmp0 = (int64_t)in * (int64_t)(alpha) + (int64_t)average * (int64_t)(65536 - alpha);
  return (int32_t)((tmp0 + 32768) / 65536);
}

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
  t_read_celda = millis();
}

void loop() {
  // read ADC values
  adcReading1 = analogRead(SI);
  avg1 = dsp_ema_i32( (int32_t)adcReading1 << 21, avg1, DSP_EMA_I32_ALPHA(0.2));
  sensor1 = byte(avg1 >> 23 ); // shift down to single byte

  adcReading2 = analogRead(SP);
  avg2 = dsp_ema_i32( (int32_t)adcReading2 << 21, avg2, DSP_EMA_I32_ALPHA(0.2));
  sensor2 = byte(avg2 >> 23 ); // shift down to single byte

  adcReading3 = analogRead(SM);
  avg3 = dsp_ema_i32( (int32_t)adcReading3 << 21, avg3, DSP_EMA_I32_ALPHA(0.2));
  sensor3 = byte(avg3 >> 23 ); // shift down to single byte


  if(millis() - t_read_celda > 50){
    units = scale.get_units(1);
    if (units < 0) {
      units = 0.00;
    }
    t_read_celda = millis();
  }

  // converts the voltaje value to force unit [kg] (see calibration guide)
  I_force = sensor1 * 5.0/256.0*c_factor;
  P_force = sensor2 * 5.0/256.0*c_factor;
  M_force = sensor3 * 5.0/256.0*c_factor;

  // using the ecuation for equilibrum state
  float Df = 0.0167; // distance in meters
  float de = 0.0085;

  float alpha = atan(0.003/de) * 3.14159 / 180; // in radians
  float beta = atan(0.003/Df) * 3.14159 / 180; // in radians

  float I_tens = K * sensor1;// = Df*I_force*cos(alpha) / (de * cos(beta));
  float P_tens = K * sensor2;// = Df*P_force*cos(alpha) / (de * cos(beta));
  float M_tens = K * sensor3;// = Df*M_force*cos(alpha) / (de * cos(beta));

/*
  // formatea la info para mostrarla en el serial plotter
  Serial.print("l_flex: ");
  Serial.print(l_tens*1000);
  Serial.print("\tr_flex: ");
  Serial.print(l_force*1000);

  Serial.println("");
  delay(10);
  */
  if(millis() - t_print > 50){
    Serial.print(I_tens*1000);
    Serial.print(",");
    Serial.print(P_tens*1000);
    Serial.print(",");
    Serial.print(M_tens*1000);
    Serial.print(",");
    Serial.print(units*1000);
    Serial.println();
    t_print = millis();
  }
}
