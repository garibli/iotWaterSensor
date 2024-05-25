/*************************************
      Öğrenci adı: Fuad Garibli
      Öğrenci numara: G201210558
      Şube: 2A
      Ders: Nesnelerin Interneti
      Proje: Su seviyye tespit sensörü
**************************************/
#include <LiquidCrystal_I2C.h> 
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <BlynkSimpleEsp8266.h> //Blynk tanımlayalım
#define BLYNK_AUTH_TOKEN "c2LYOs3ADPQWgK9AUJTlIENS0mfKWGg4"
#define BLYNK_PRINT Serial
WiFiServer server(80);
WiFiClientSecure client;
//Board'da kullanacağımız elemanların pinlerini tanımlayalım
#define TRIG_PIN D7
#define ECHO_PIN D8
#define RED D0 //D0 LED1
#define GREEN1 D3  //D3 LED2
#define GREEN2 D4 //D4 LED3
#define GREEN3 D5 //D5 LED4
#define BUZZ D6 //D6 LED5
#define PUMP 3 //S1 PUMP (change to D3 to match the variable name)
//LCD Ekranı tanımlayalım
LiquidCrystal_I2C lcd(0x27, 16, 2);
char auth[] = "c2LYOs3ADPQWgK9AUJTlIENS0mfKWGg4"; // blynk token
char ssid[] = "Fd";
char pass[] = "MarxEngelsss";
//Blynk uygulaması butonundan gelen veriyi RX pinine yazdıralım
  BLYNK_WRITE(V0) 
  {
    digitalWrite(PUMP, param.asInt());
  }
//Setp menüsü
void setup() {
  Serial.begin(9600);
  configTime(0, 0, "pool.ntp.org");
  lcd.init();
  lcd.backlight();
  // pinMode setupu
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(RED, OUTPUT);
  pinMode(GREEN1, OUTPUT);
  pinMode(GREEN2, OUTPUT);
  pinMode(GREEN3, OUTPUT);
  pinMode(BUZZ, OUTPUT);
  pinMode(PUMP, OUTPUT);
  //lamasını başlat
  Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);
  //wifiye bağlan
  Serial.print("Connecting Wifi: ");
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print("*");
    delay(500);
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  //LCD ekranı başlat
  lcd.setCursor(0, 0);
  lcd.print("<Water Level");
  lcd.setCursor(3, 1);
  lcd.print("Monitoring>");
  delay(6000);
  lcd.clear();
}
void loop() {
  lcd.setCursor(0, 0);
  lcd.print("Level:");
  delayMicroseconds(4);
  long duration, distance;
  // Send a pulse to the trigger pin
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  // Measure the duration of the echo pulse
  duration = pulseIn(ECHO_PIN, HIGH);
  // Calculate the distance in centimeters
  distance = (duration * 0.0343) / 2;
  int blynkmeter=22-distance;
  Blynk.virtualWrite(V1, blynkmeter);
  // Control the pump using Blynk V0
  Blynk.run();
    // Print the distance to the Serial Monitor
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");
  // Add a delay between measurements
  delay(500);
  if (distance <= 25 && 20 <= distance) {
    lcd.setCursor(6, 1);
    lcd.print("EMPTY");
    lcd.print("               ");
    digitalWrite(RED, HIGH);
    digitalWrite(GREEN1, LOW);
    digitalWrite(GREEN2, LOW);
    digitalWrite(GREEN3, LOW);
    digitalWrite(BUZZ, HIGH);
    delay(300);
    digitalWrite(BUZZ, LOW);
    delay(300);
  } 
  else if (distance < 20 && 18 <= distance) 
  {
    lcd.setCursor(6, 1);
    lcd.print("VERY LOW");
    lcd.print("               ");
    digitalWrite(RED, LOW);
    digitalWrite(GREEN1, HIGH);
    delay(200);
    digitalWrite(GREEN1, LOW);
    delay(200);
    digitalWrite(GREEN2, LOW);
    digitalWrite(GREEN3, LOW);
    digitalWrite(BUZZ, LOW);
  } 
  else if (distance < 18 && 16 <= distance) 
  {
    lcd.setCursor(6, 1);
    lcd.print("LOW");
    lcd.print("               ");
    digitalWrite(RED, LOW);
    digitalWrite(GREEN1, HIGH);
    digitalWrite(GREEN2, LOW);
    digitalWrite(GREEN3, LOW);
    digitalWrite(BUZZ, LOW);
  } 
  else if (distance < 16 && 11 <= distance) 
  {
    lcd.setCursor(6, 1);
    lcd.print("MEDIUM");
    lcd.print("               ");
    digitalWrite(RED, LOW);
    digitalWrite(GREEN1, HIGH);
    digitalWrite(GREEN2, HIGH);
    delay (200);
    digitalWrite(GREEN2, LOW);
    delay(200);
    digitalWrite(GREEN3, LOW);
    digitalWrite(BUZZ, LOW);
  } 
  else if (distance < 11 && 7 <= distance) 
  {
    lcd.setCursor(6,1);
    lcd.print("MID-HIGH");
    lcd.print("               ");
    digitalWrite(RED, LOW);
    digitalWrite(GREEN1, HIGH);
    digitalWrite(GREEN2, HIGH);
    digitalWrite(GREEN3, LOW);
    digitalWrite(BUZZ, LOW);
  }
    else if (distance < 7 && 4 <= distance) 
    {
    lcd.setCursor(6,1);
    lcd.print("HIGH");
    lcd.print("               ");
    digitalWrite(RED, LOW);
    digitalWrite(GREEN1, HIGH);
    digitalWrite(GREEN2, HIGH);
    digitalWrite(GREEN3, HIGH);
    delay(50);
    digitalWrite(GREEN3, LOW);
    delay(50);
    digitalWrite(BUZZ, LOW);
    }
    else if(distance<=3)
    {
    lcd.setCursor(6,1);
    lcd.print("FULL");
    lcd.print("               ");
    digitalWrite(RED, LOW);
    digitalWrite(GREEN1, HIGH);
    digitalWrite(GREEN2, HIGH);
    digitalWrite(GREEN3, HIGH);
  }
}