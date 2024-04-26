#define BLYNK_TEMPLATE_ID "TMPL2BE2K7LO2"
#define BLYNK_TEMPLATE_NAME "vuon thong minh"
#define BLYNK_AUTH_TOKEN "3JrlG_6jRYRd5uYDjMkViqU5mSgP9ApP"
#define BLYNK_WIFI_SSID "123123123"
#define BLYNK_WIFI_PASS "DuyQuang"
#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <WiFiManager.h>
#include <BlynkSimpleEsp8266.h>
#include <DHTesp.h>
#include <Wire.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

#define i2c_Address 0x3c
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET -1   //   QT-PY / XIAO
#define DHTPIN D5    // Digital pin connected to the DHT sensor (temperature and humidity)
#define DHTTYPE DHT11 // Type of DHT sensor

#define relay_bom D6
#define relay_led D7
#define light_PIN D0
#define MOISTURE_PIN A0
int mode = 0;  // nếu là 1: chế độ tự động, nếu là 0: chế độ thủ công

Adafruit_SH1106G display = Adafruit_SH1106G(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
WiFiManager wifimanager;
DHTesp dht;

     
void setup() 
{
  Serial.begin(115200);
  dht.setup(DHTPIN, DHTesp::DHT11);
  pinMode(relay_led,OUTPUT);
  pinMode(relay_bom,OUTPUT);
  pinMode(light_PIN,INPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(relay_led,LOW);
  digitalWrite(relay_bom,LOW);
  digitalWrite(LED_BUILTIN, LOW);
  display.begin(i2c_Address, true);
  display.display();
  display.clearDisplay();
  display.begin(i2c_Address, true);
  display.display();
  display.clearDisplay();
  display.setTextColor(SH110X_WHITE);
  display.setTextSize(2);
  display.setCursor(10, 0);
  display.println("Thong Tin");
  display.setTextSize(1);
  display.setCursor(0, 20);
  display.println("Temp: ");
  display.setCursor(0, 30);
  display.println("Humi: ");
  display.setCursor(0, 40);
  display.println("Soil Moisture: ");
  
  display.setTextSize(1);
  display.setCursor(60, 20);
  display.println("*C");
  display.setCursor(60, 30);
  display.println("%");
  display.setCursor(120, 40);
  display.println("%");
  wifimanager.autoConnect("config wifi");
  Blynk.begin(BLYNK_AUTH_TOKEN, WiFi.SSID().c_str(), WiFi.psk().c_str());
  digitalWrite(LED_BUILTIN, HIGH);
  
}

void loop() 
{
  Blynk.run();
  float humidity = dht.getHumidity();
  float temperature = dht.getTemperature();
  int light_value = digitalRead(light_PIN);
  int MOISTUREStatus = analogRead(MOISTURE_PIN);
  int moisturePercentage = map(MOISTUREStatus, 0, 1023, 0, 100);


  if(light_value == LOW)
  {
    Serial.println("DayTIME");
    digitalWrite(relay_led,HIGH);
  }
  else
  {
    Serial.println("NIGHT");
    digitalWrite(relay_led,LOW);
  }

  if(mode == 1) 
  { // Chế độ auto
    int MOISTUREStatus = analogRead(MOISTURE_PIN);
    int moisturePercentage = map(MOISTUREStatus, 0, 1023, 0, 100);
    if (moisturePercentage < 50) 
    {
      digitalWrite(relay_bom, HIGH);
    } 
    else
    {
      digitalWrite(relay_bom, LOW);
    }
  }
  Serial.println(MOISTUREStatus);


  delay(1000);
  // Gửi nhiệt độ và độ ẩm lên ứng dụng Blynk
  Blynk.virtualWrite(V2, humidity);
  Blynk.virtualWrite(V3, temperature);
  Blynk.virtualWrite(V4, moisturePercentage);
  display.setCursor(30, 20);
  display.println(temperature);
  display.setCursor(30, 30);
  display.println(humidity);
  display.setCursor(100, 40);
  display.println(moisturePercentage);
  display.display();
}

BLYNK_WRITE(V0) 
{
  int check_auto = param.asInt();
  if(check_auto == 1)
  {
    mode = 1;
  }
  else
  {
    mode = 0;
  }
}

BLYNK_WRITE(V1)
{
  int value = param.asInt();
  digitalWrite(relay_bom,value);
}
