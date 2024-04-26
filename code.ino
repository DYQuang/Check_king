#include <Servo.h>
#include <MFRC522.h>
#include <ESP8266WiFi.h>
#include <WiFiManager.h>
#include <SPI.h>

#define RST_PIN D3
#define SS_PIN D4
#define GAS_SENSOR_PIN A0
#define fan_pin D1
#define fire_pin D0
#define buzz_pin D2

MFRC522 mfrc522(SS_PIN, RST_PIN);
Servo SERVO1;
Servo SERVO2;

bool isFirstSwipe = true;
bool isDoorOpen = false;

void setup()
{
  Serial.begin(115200);
  SPI.begin();
  mfrc522.PCD_Init();
  SERVO1.attach(D8);
  SERVO2.attach(3);
  pinMode(fan_pin, OUTPUT);
  pinMode(buzz_pin, OUTPUT);
  pinMode(fire_pin, INPUT);
  digitalWrite(buzz_pin, LOW);
  digitalWrite(fan_pin, LOW);
  SERVO1.write(0);
  SERVO2.write(0);
  Serial.println("Đưa thẻ RFID vào phạm vi đọc...");

  WiFiManager wifiManager;
  if (!wifiManager.autoConnect("Hệ Thống PCCC", "11111111")) 
  {
    Serial.println("Failed to connect and hit timeout");
    delay(3000);
    ESP.reset();
    delay(5000);
  }
  Serial.println("Connected to WiFi!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  SPI.begin();
  Serial.println("Kết nối WiFi thành công!");

  // Hiển thị thông tin tên và mật khẩu WiFi
  Serial.print("WiFi SSID: ");
  Serial.println(WiFi.SSID());
  Serial.print("WiFi Password: ");
  Serial.println(WiFi.psk());
}

void loop()
{
  rfid_check();
  MQ5();
  check_fire();
  delay(500);
}

void rfid_check()
{
  if (!mfrc522.PICC_IsNewCardPresent()) {
    return;
  }

  if (!mfrc522.PICC_ReadCardSerial()) {
    return;
  }

  unsigned long uidDec = 0;
  Serial.println("===================================");
  Serial.println("Card UID: ");
  
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    uidDec = (uidDec * 256) + mfrc522.uid.uidByte[i];
  }
  Serial.print(uidDec);
  
  if (uidDec == 2736406578)
  {
    if (isFirstSwipe) 
    {
      SERVO1.write(180);
      isFirstSwipe = false;
      isDoorOpen = true;
    } 
    else 
    {
      SERVO1.write(0);
      isFirstSwipe = true;
      isDoorOpen = false;
    }
  }

  mfrc522.PICC_HaltA();
}

void MQ5()
{
  int gasValue = analogRead(GAS_SENSOR_PIN);
  int mappedValue = map(gasValue, 0, 1023, 0, 225);
  
  Serial.print("Gas Sensor Value: ");
  Serial.println(mappedValue);

  if (mappedValue > 100)
  {
    digitalWrite(fan_pin, HIGH);
  }
  else
  {
    digitalWrite(fan_pin, LOW);
  }
  
  delay(1000);
}

void check_fire()
{
  int fireValue = digitalRead(fire_pin);
  
  if (fireValue == LOW)
  {
    Serial.println("Phát hiện lửa!");
    digitalWrite(buzz_pin, HIGH);
    SERVO1.write(180);
    SERVO2.write(180);
  }
  else 
  { 
    Serial.println("Bình thường");
    digitalWrite(buzz_pin, LOW);
    SERVO1.write(0);
    SERVO2.write(0);
  }
  
  delay(1000);
}