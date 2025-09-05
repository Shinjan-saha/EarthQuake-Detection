#include <Wire.h>
#include <I2Cdev.h>
#include <MPU6050.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <SPI.h>

MPU6050 mpu;
int16_t ax, ay, az, gx, gy, gz;


const int buzzerPin = 4;  
const int ledPin = 7;     


#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_DC     9
#define OLED_CS     10
#define OLED_RESET  8
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &SPI, OLED_DC, OLED_RESET, OLED_CS);


const unsigned long SAMPLE_INTERVAL_MS = 100; 

#define RMS_WINDOW 10   
float rmsBuffer[RMS_WINDOW];
int rmsIndex = 0;
bool rmsBufferFilled = false;
float rmsThreshold = 1.1; 


unsigned long lastSampleTime = 0;

void setup() {
  Serial.begin(115200);
  Wire.begin();
  mpu.initialize();

  pinMode(buzzerPin, OUTPUT);
  pinMode(ledPin, OUTPUT);
  digitalWrite(buzzerPin, LOW);
  digitalWrite(ledPin, LOW);

  if (!display.begin(SSD1306_SWITCHCAPVCC)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 20);
  display.println("System Init");
  display.display();
  delay(1000);
  display.clearDisplay();

  Serial.println("Earthquake detection ready (RMS).");
}

void loop() {
  unsigned long now = millis();
  if (now - lastSampleTime < SAMPLE_INTERVAL_MS) return;
  lastSampleTime = now;


  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
  float Ax = ax / 16384.0f;
  float Ay = ay / 16384.0f;
  float Az = az / 16384.0f;
  float mag = sqrt(Ax * Ax + Ay * Ay + Az * Az); 

  bool alarm = updateRMSAndCheck(mag);


  if (alarm) {
    digitalWrite(buzzerPin, HIGH);
    digitalWrite(ledPin, HIGH);
    Serial.println("!!! EARTHQUAKE DETECTED (RMS) !!!");
    showMessageOnOLED("EARTHQUAKE!", mag);
  } else {
    digitalWrite(buzzerPin, LOW);
    digitalWrite(ledPin, LOW);
    showMeasurementsOnOLED(Ax, Ay, Az, mag);
  }
}


bool updateRMSAndCheck(float mag) {
  rmsBuffer[rmsIndex++] = mag;
  if (rmsIndex >= RMS_WINDOW) {
    rmsIndex = 0;
    rmsBufferFilled = true;
  }
  if (!rmsBufferFilled) return false; 


  float sumSq = 0;
  for (int i = 0; i < RMS_WINDOW; ++i) sumSq += rmsBuffer[i] * rmsBuffer[i];
  float rms = sqrt(sumSq / RMS_WINDOW);
  Serial.print("RMS: "); Serial.println(rms);

 
  return (rms > rmsThreshold);
}


void showMeasurementsOnOLED(float Ax, float Ay, float Az, float mag) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0,0);
  display.print("Ax: "); display.println(Ax, 2);
  display.print("Ay: "); display.println(Ay, 2);
  display.print("Az: "); display.println(Az, 2);
  display.print("Mag: "); display.println(mag, 2);
  display.display();
}

void showMessageOnOLED(const char *msg, float mag) {
  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor(0,0);
  display.println(msg);
  display.setTextSize(1);
  display.setCursor(0,40);
  display.print("Mag: "); display.println(mag, 2);
  display.display();
}
