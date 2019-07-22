#include <boarddefs.h>
#include <IRremote.h>
#include <IRremoteInt.h>
#include <ir_Lego_PF_BitStreamEncoder.h>
int RECV_PIN = A3; //define input pin on Arduino
int cds = A2;
IRrecv irrecv(RECV_PIN);
decode_results results;
#define DHT_PIN 5
#define DHT11_RETRY_DELAY 1000

unsigned long last_read_time = 0;
byte hI;
byte hF;
byte tI;
byte tF;

void setup() {

  //stick
  Serial.begin(9600);
  //pinSet
  pinMode(8, INPUT_PULLUP);
  //IRs
  irrecv.enableIRIn();
}
void loop() {
  int err, photo;
  float temp;
  byte humid;
  //stick
  //X축 방향값
  if (analogRead(A0) <= 300) {
    Serial.println('a');
  }
  else if (analogRead(A0) >= 700) {
    Serial.println('d');
  }
  //Y축 방향값
  if (analogRead(A1) <= 300) {
    Serial.println('w');
  }
  else if (analogRead(A1) >= 700) {
    Serial.println('s');
  }
  err = read(humid, temp);
  if (err == 0)
  {

    Serial.print("Hum : ");
    Serial.println(humid);
    Serial.print("Tem : ");
    Serial.println(temp);
    int cdsValue = analogRead(cds);
    Serial.print("cds : ");
    Serial.println(cdsValue);

  }
  else
  {
  }

  //ir
  if (irrecv.decode(&results)) {
    Serial.println(results.value, HEX);
    switch (results.value) {
      case 0xFF30CF:

        break;
      case 0xFF18E7://1번눌렀을때

        break;//1버튼이 눌리면 LED OFF
    }
    irrecv.resume(); // Receive the next value
  }
  delay(20);
}

int read(byte& humidity, float& temperature) {
  long temp;

  if ((millis() - last_read_time < DHT11_RETRY_DELAY) && last_read_time != 0)  return -1;
  pinMode(DHT_PIN, OUTPUT);
  digitalWrite(DHT_PIN, LOW);
  delay(18);
  digitalWrite(DHT_PIN, HIGH);
  pinMode(DHT_PIN, INPUT);

  if ((temp = waitFor(LOW, 40)) < 0)  return 1; //waiting for DH11 ready
  if ((temp = waitFor(HIGH, 90)) < 0) return 1; //waiting for first LOW signal(80us)
  if ((temp = waitFor(LOW, 90)) < 0)  return 1; //waiting for first HIGH signal(80us)

  hI = readByte();
  hF = readByte();
  tI = readByte();
  tF = readByte();
  byte cksum = readByte();
  if (hI + hF + tI + tF != cksum)
    return 4;

  humidity = (float)hI + (((float)hF) / 100.0F);
  temperature = (float)tI + (((float)tF) / 100.0F);
  last_read_time = millis();
  return 0;
}

unsigned long waitFor(uint8_t target, unsigned long time_out_us) {
  unsigned long start = micros();
  unsigned long time_out = start + time_out_us;
  while (digitalRead(DHT_PIN) != target)
  {
    if (time_out < micros()) return -1;
  }
  return micros() - start;
}

void waitFor(uint8_t target) {
  while (digitalRead(DHT_PIN) != target);
}

byte readByte() {
  int i = 0;
  byte ret = 0;
  for (i = 7; i >= 0; i--)
  {
    waitFor(HIGH); //wait for 50us in LOW status
    delayMicroseconds(30); //wait for 30us
    if (digitalRead(DHT_PIN) == HIGH) //if HIGH status lasts for 30us, the bit is 1;
    {
      ret |= 1 << (i);
      waitFor(LOW); //wait for rest time in HIGH status.
    }
  }
  return ret;
}
