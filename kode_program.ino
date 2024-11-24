#define BLYNK_TEMPLATE_ID " " //
#define BLYNK_TEMPLATE_NAME "Inkubator Telur"
#define BLYNK_AUTH_TOKEN " " //

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <BlynkSimpleEsp32.h>
#include <WiFi.h>

#define SCREEN_WIDTH 128 
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

#define BLYNK_PRINT Serial
char auth[] = " ";
char ssid[] = " ";  // type your wifi name
char pass[] = " ";  // type your wifi password

#define DHTPIN 23
#define DHTTYPE    DHT11 

DHT dht(DHTPIN, DHTTYPE);
int led = 18;
BlynkTimer timer;
int inkubator = V3;

void setup() {
  Serial.begin(115200);
  Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);
  dht.begin();

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  delay(2000);
  display.clearDisplay();
  display.setTextColor(WHITE);
  pinMode(led, OUTPUT);

  timer.setInterval(2500L, sendSensor);
  pinMode(inkubator, OUTPUT);
  //Inisialisai saklar
  digitalWrite(inkubator, LOW);
}

void loop(){
  Blynk.run();
  timer.run();
}

void sendSensor(){
  float h = dht.readHumidity();
  float t = dht.readTemperature(); // or dht.readTemperature(true) for Fahrenheit
  if (!isnan(h) && !isnan(t)) {
    Serial.print("Temperature: ");
    Serial.print(t);
    Serial.println(" *C");
    Serial.print("Humidity: ");
    Serial.print(h);
    Serial.print(" %");
  }
  // clear display
  display.clearDisplay();
  
  // display temperature
  display.setTextSize(1);
  display.setCursor(0,0);
  display.print("Temperature: ");
  display.setTextSize(2);
  display.setCursor(0,10);
  display.print(t);
  display.print(" ");
  display.setTextSize(1);
  display.cp437(true);
  display.write(167);
  display.setTextSize(2);
  display.print("C");
  
  // display humidity
  display.setTextSize(1);
  display.setCursor(0, 35);
  display.print("Humidity: ");
  display.setTextSize(2);
  display.setCursor(0, 45);
  display.print(h);
  display.print(" %"); 
  
  display.display();

  Blynk.virtualWrite(V1, t);
  Blynk.virtualWrite(V2, h);
  
  Serial.print("Suhu : ");
  Serial.print(t);
  Serial.print("|| Kelembapan : ");
  Serial.println(h);

  if ( t <=35 ){
    digitalWrite(led, LOW);
  } 
  else {
    digitalWrite(led, HIGH);
  }
  // delay(500); //memberi jeda waktu baca selama 500 detik
}

BLYNK_WRITE(V3) {
  // Mendapatkan status tombol switch di Blynk
  int switchStatus = param.asInt();

  if (switchStatus == LOW) {
    digitalWrite(led, LOW); // Menyalakan lampu jika switch diaktifkan
  } else {
    digitalWrite(led, HIGH); // Mematikan lampu jika switch dimatikan
  }
  delay(5000);
}