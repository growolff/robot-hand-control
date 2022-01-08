/* creado el 01-01-2021 por Gonzalo Olave
 * Control de posicion de un motor BLDC con sensores de efecto hall
 */

#define M1_HA 3// 2
#define M1_HB 6
#define M1_DIR 8
#define M1_EN 9
#define M1_PWM 10

int16_t M1_counter = 0;
int16_t ref = 0;

int16_t pwm_out = 0;
float kp = 3;
unsigned long t = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(M1_HB, INPUT);
  pinMode(M1_HA, INPUT_PULLUP);
  pinMode(M1_DIR, OUTPUT);
  pinMode(M1_EN, OUTPUT);
  pinMode(M1_PWM, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(M1_HA), HA_ISR, RISING);

  Serial.begin(115200);

  digitalWrite(M1_EN, HIGH);
  digitalWrite(M1_DIR, LOW);

  t = millis();
}

void loop() {
  if (Serial.available()) {
    ref = Serial.parseInt();
    int16_t aux = Serial.parseInt(); // quita el 0 que se lee siempre despues de un parseInt
  }
  int16_t err = ref - M1_counter;
  pwm_out =  (int16_t) err * kp;

  if (pwm_out > 255) pwm_out = 255;
  if (pwm_out < -255) pwm_out = -255;
  
  if (pwm_out < 0) {
    digitalWrite(M1_DIR, LOW);
    pwm_out = pwm_out * -1;
  }
  else if (pwm_out > 0) digitalWrite(M1_DIR, HIGH);
  analogWrite(M1_PWM, pwm_out);


  if (millis() - t > 500) {
    Serial.print("Ref: ");
    Serial.print(ref);
    Serial.print("\tMes: ");
    Serial.print(M1_counter);
    Serial.print("\tPWM: ");
    Serial.println(pwm_out);
    //digitalWrite(M1_PWM,HIGH);

    t = millis();
  }
}

void HA_ISR() {
  int8_t hb = digitalRead(M1_HB);
  M1_counter += hb ? -1 : 1;
}
