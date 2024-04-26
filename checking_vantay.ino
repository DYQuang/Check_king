#include <ESP8266WiFi.h>
#include <WiFiManager.h>
#include <WiFiClientSecure.h>
#include <Adafruit_Fingerprint.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

SoftwareSerial mySerial(14, 12);
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);
LiquidCrystal_I2C lcd(0x27, 16, 2);

const int maxUsers = 10;

const uint16_t fingerprintIDs[maxUsers] PROGMEM = {1, 2, 3, 4, 5, 6};
const char userNames[maxUsers][20] = {"1101", "1102", "1103", "1104", "1105", "1106"};
uint8_t scanCounts[maxUsers] = {0};

const char *host = "script.google.com";
const int httpsPort = 443;
String GAS_ID = "AKfycbyRtNlGcmVkywvgzJNKfeIUJjowvTjQPjnIILoC4-u7bBKP4mcsaU25zgEZzX8woX2H";

WiFiClientSecure client;

void setup()
{
  Serial.begin(115200);
  

  WiFiManager wifiManager;
  if (!wifiManager.autoConnect("AP Name", "11111111"))
  {
    Serial.print("Ket noi that bai");
    delay(300);
    ESP.reset();
    delay(500);
  }
  Serial.println("Ket noi thanh cong");
  Serial.print("IP ");
  Serial.println(WiFi.localIP());
  client.setInsecure();

  lcd.begin(16, 2);
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("User:");
  mySerial.begin(57600);
  if (finger.verifyPassword())
  {
    Serial.println("Cam bien van tay da duoc khoi tao thanh cong.");
  }
  else
  {
    Serial.println("Khong the ket noi voi cam bien van tay. Kiem tra ket noi va reset Arduino.");
    while (1)
      ;
  }
  Serial.println("Vui long dat van tay de nhan dien...");
}

void loop()
{
  int fingerprintID = getFingerprintID();
  if (fingerprintID > 0)
  {
    int i;
    // Tim xem ID van tay co trong danh sach khong
    for (i = 0; i < maxUsers; i++)
    {
      if (fingerprintID == pgm_read_word_near(fingerprintIDs + i))
      {
        scanCounts[i]++;
        Serial.print("Nhan dien van tay thanh cong cho: ");
        Serial.print(userNames[i]);
        Serial.print(", So lan quet: ");
        Serial.println(scanCounts[i]);

        // In thong tin len man hinh LCD
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("User:");
        lcd.print(userNames[i]);
        lcd.setCursor(0, 1);
        lcd.print("Scans: ");
        lcd.print(scanCounts[i]);

        delay(1000);
        lcd.clear();
        break;
      }
    }
    delay(1000);
    sendData(userNames[i], scanCounts[i]);
  }
  delay(100);
}

uint8_t getFingerprintID()
{
  uint8_t p = finger.getImage();

  switch (p)
  {
  case FINGERPRINT_OK:
    Serial.println("Anh van tay da duoc lay thanh cong. Dang kiem tra van tay...");
    break;
  case FINGERPRINT_NOFINGER:
    Serial.println("Dat van tay len cam bien...");
    return 0;
  case FINGERPRINT_PACKETRECIEVEERR:
    Serial.println("Loi truyen goi tin tu cam bien van tay. Vui long kiem tra lai ket noi.");
    return 0;
  case FINGERPRINT_IMAGEFAIL:
    Serial.println("Khong the lay duoc anh van tay. Hay thu lai.");
    return 0;
  default:
    Serial.println("Loi khong xac dinh. Hay thu lai.");
    return 0;
  }

  p = finger.image2Tz();
  switch (p)
  {
  case FINGERPRINT_OK:
    Serial.println("Xac dinh van tay thanh cong. Dang kiem tra van tay...");
    break;
  case FINGERPRINT_IMAGEMESS:
    Serial.println("Anh van tay qua mo. Hay thu lai.");
    return 0;
  case FINGERPRINT_PACKETRECIEVEERR:
    Serial.println("Loi truyen goi tin tu cam bien van tay. Vui long kiem tra lai ket noi.");
    return 0;
  case FINGERPRINT_FEATUREFAIL:
    Serial.println("Khong tim thay dac trung trong anh van tay. Hay thu lai.");
    return 0;
  case FINGERPRINT_INVALIDIMAGE:
    Serial.println("Anh van tay khong hop le. Hay thu lai.");
    return 0;
  default:
    Serial.println("Loi khong xac dinh. Hay thu lai.");
    return 0;
  }

  p = finger.fingerFastSearch();
  if (p == FINGERPRINT_OK)
  {
    return finger.fingerID;
  }
  else
  {
    Serial.println("Khong nhan dien duoc van tay. Thu lai hoac dang ky van tay moi.");
    return 0;
  }
}

void sendData(const char *c, int d)
{
  Serial.print("Connecting to ");
  Serial.println(host);

  if (!client.connect(host, httpsPort))
  {
    Serial.println("Connection failed");
    return;
  }

  String url = "/macros/s/" + GAS_ID + "/exec?ID=" + String(c) + "&solan=" + String(d);
  Serial.println(url);
  client.print(String("GET ") + url + " HTTP/1.1\r\n" + "Host: " + host + "\r\n" + "User-Agent: BuildFailureDetectorESP8266\r\n" + "Connection: close\r\n\r\n");
  Serial.println("OK");
}
