
#define WRITE_RATE_HZ 5

#define FF 0xff
#define OUT_MSG_LEN 8
#define IN_MSG_LEN 10

// motor cmds
#define F_SET_POS_REF         1
#define SET_FORCE_REF         2
#define REQ_PID_VALUES        22
#define SET_PID_VALUES        23
#define SET_CONTROL_MODE      24
#define DISABLE_MOTOR         25
#define ENABLE_MOTOR          26
#define F_DISABLE_MOTOR       25
#define F_ENABLE_MOTOR        26

#define F_SEND_DATA_TRUE      40
#define F_SEND_DATA_FALSE     41
#define F_UPDATE_PLOT         100

#define LED_PWM 9

uint8_t led_pwm = 0;
bool sendData_st = false;
uint8_t led_st = 0;
bool process_it = false;

// each Rx command consists of: <char command><string value><CR>
typedef struct inMsg {           // anonimous structure
  uint8_t cmd;     // 1-byte command: Write, Read, Control, get info, set pid, etc.
  uint8_t id;      // motor id
  int16_t ref;    // control ref
  int16_t p;      //
  int16_t i;
  int16_t d;
} iM;

union setupMsg {
  char   bytes[IN_MSG_LEN];   // received Rx packet (to hold individual command message)
  inMsg msg;
} RB;

union vizMsg {
  char   bytes[OUT_MSG_LEN];   // buffer (to hold individual command message)
  struct {            // anonimous structure
    uint8_t cmd;
    uint8_t motor;     // 1-byte command: Write, Read, Control, get info, set pid, etc.
    int16_t ref;    // pid control reference
    int16_t cur;    // pid control current value
    int16_t val;
  };
} WB;

void readPacket() {
  if (Serial.available() == IN_MSG_LEN) {
    // do nothing and wait if serial buffer doesn't have enough bytes to read
    Serial.readBytes(RB.bytes, IN_MSG_LEN);
    // read numBytes bytes from serial buffer and store them at a union called “RB”
    process_it = true;
  }
}

bool receive(inMsg* table)
{
  return (Serial.readBytes((char*)table, sizeof(iM)) == sizeof(iM));
}

void writePacket(uint8_t cmd, uint8_t motor, int16_t v1, int16_t v2, int16_t v3) {

  int len = sizeof(WB.bytes) / sizeof(*WB.bytes);
  WB.cmd = cmd;
  WB.motor = motor;
  WB.ref = v1;
  WB.cur = v2;
  WB.val = v3;

  Serial.write(WB.bytes, len);
}

void parseCommand() {

  uint8_t cmd =  RB.msg.cmd;
  uint8_t id = RB.msg.id;
  uint16_t ref = RB.msg.ref;
  uint16_t p = RB.msg.p;
  uint16_t i = RB.msg.i;
  uint16_t d = RB.msg.d;

  switch (cmd) {
    case F_SEND_DATA_TRUE:
      sendData_st = true;
      break;
    case F_SEND_DATA_FALSE:
      sendData_st = false;
      break;
    case F_SET_POS_REF:
      led_pwm = ref;
      break;
  }
  // clear data structure
  for (int i = 0; i < IN_MSG_LEN; i++) {
    RB.bytes[i] = 0;
  }
  Serial.flush();
}


unsigned long t_write = 0;
uint16_t write_rate = 1000 / WRITE_RATE_HZ;

void setup() {
  Serial.begin(500000);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(LED_PWM, OUTPUT);

  t_write = millis();
}

inMsg* recvMsg;

void loop() {

  readPacket();
  if (process_it) {
    parseCommand();
    process_it = false;
  }

  if (millis() - t_write > write_rate) {
    digitalWrite(LED_BUILTIN, led_st);
    led_st = !led_st;
    if(sendData_st == true){
      writePacket(led_pwm, t_write, sendData_st, 2, 3);
    }
    t_write = millis();
  }

  analogWrite(LED_PWM, led_pwm);
}

void blinkLed(int t) {
  digitalWrite(LED_BUILTIN, HIGH);
  delay(t / 2);
  digitalWrite(LED_BUILTIN, LOW);
  delay(t / 2);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(t / 2);
  digitalWrite(LED_BUILTIN, LOW);
  delay(t / 2);
}

void sendCMD(){

  
}
