#include <ESP8266WiFi.h>
#include <SPI.h>
#include <MFRC522.h>
#include <WiFiClientSecure.h>

#define SS_PIN D2
#define RST_PIN D3

const char* ssid = "DUY QUANG";
const char* password = "123456789";
const char* host = "script.google.com";
const int httpsPort = 443;

String GAS_ID = "AKfycbwmdRqxlIsFdwIvOwUz2l84WyHPYmyND0No83r8cbujkHmdUmdpBZOTyt4s8lmLeYWLMA";

WiFiClientSecure client;

MFRC522 mfrc522(SS_PIN, RST_PIN);
unsigned long uidDec, uidDecTemp;
unsigned long ticketNumber;

void setup() {
  Serial.begin(115200);
  delay(500);

  WiFi.begin(ssid, password);
  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println();
  Serial.print("Successfully connected to: ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println();
  client.setInsecure();

  SPI.begin();
  mfrc522.PCD_Init();
  
  Serial.println("Đưa thẻ RFID vào phạm vi đọc...");
}

void loop() 
{
  if (!mfrc522.PICC_IsNewCardPresent()) {
    return;
  }

  if (!mfrc522.PICC_ReadCardSerial()) {
    return;
  }

  uidDec = 0;
  Serial.println("===================================");
  Serial.println("Card UID: ");
  
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    uidDecTemp = mfrc522.uid.uidByte[i];
    uidDec = uidDec * 256 + uidDecTemp;
  }
  Serial.println(uidDec);
  sendData(uidDec);
  delay(2000);
  
  
}

void sendData(int c) {
  Serial.print("Connecting to ");
  Serial.println(host);

  if (!client.connect(host, httpsPort)) {
    Serial.println("Connection failed");
    return;
  }

  String url = "/macros/s/" + GAS_ID + "/exec?name=" + String(c);
  Serial.println(url);
  client.print(String("GET ") + url + " HTTP/1.1\r\n" + "Host: " + host + "\r\n" + "User-Agent: BuildFailureDetectorESP8266\r\n" +"Connection: close\r\n\r\n");
 Serial.println("OK");
}