#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// OLED 1 for ECG display (address 0x3C)
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display1(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// OLED 2 for heart rhythm analysis (address 0x3D)
Adafruit_SSD1306 display2(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

const int ECG_PIN = A0;
const int LO_PLUS = 2;
const int LO_MINUS = 3;

// Variables for ECG analysis
int previousX = 0;
int previousY = SCREEN_HEIGHT / 2;
unsigned long previousBeatTime = 0;
float heartRate = 0;
const int bpmThresholdLow = 60;
const int bpmThresholdHigh = 100;

// Debouncing variables
unsigned long lastLeadOffTime = 0;
const unsigned long debounceDelay = 100;

// Function to detect heartbeats
bool detectHeartbeat(int ecgValue) {
  static int threshold = 512;
  static bool aboveThreshold = false;

  if (ecgValue > threshold && !aboveThreshold) {
    aboveThreshold = true;
    return true;
  } else if (ecgValue < threshold) {
    aboveThreshold = false;
  }
  return false;
}

void setup() {
  Serial.begin(9600);

  // Initialize first OLED display (0x3C)
  if (!display1.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("OLED 1 allocation failed"));
    for (;;);
  }
  display1.clearDisplay();
  display1.display();

  // Initialize second OLED display (0x3D)
  if (!display2.begin(SSD1306_SWITCHCAPVCC, 0x3D)) {
    Serial.println(F("OLED 2 allocation failed"));
    for (;;);
  }
  display2.clearDisplay();
  display2.display();

  pinMode(LO_PLUS, INPUT);
  pinMode(LO_MINUS, INPUT);
}

void loop() {
  // Check for lead off
  if ((digitalRead(LO_PLUS) == 1 || digitalRead(LO_MINUS) == 1)) {
    unsigned long currentTime = millis();
    if (currentTime - lastLeadOffTime > debounceDelay) {
      display1.clearDisplay();
      display1.setTextSize(1);
      display1.setTextColor(WHITE);
      display1.setCursor(0, 0);
      display1.println("Leads Off!");
      display1.display();
      display2.clearDisplay();
      display2.setTextSize(1);
      display2.setTextColor(WHITE);
      display2.setCursor(0, 0);
      display2.println("Leads Off!");
      display2.display();
      Serial.println("Leads Off!");
      lastLeadOffTime = currentTime;
      delay(500);
      return;
    }
  }

  // Read ECG data
  int ecgValue = analogRead(ECG_PIN);
  int yPos = map(ecgValue, 300, 700, 0, SCREEN_HEIGHT - 1);
  static int xPos = 0;

  // Display ECG waveform on OLED 1
  display1.drawLine(previousX, previousY, xPos, SCREEN_HEIGHT - yPos, WHITE);
  display1.display();
  previousX = xPos;
  previousY = SCREEN_HEIGHT - yPos;

  xPos++;
  if (xPos >= SCREEN_WIDTH) {
    xPos = 0;
    display1.clearDisplay();
  }

  // Heartbeat detection
  if (detectHeartbeat(ecgValue)) {
    unsigned long currentTime = millis();
    unsigned long interval = currentTime - previousBeatTime;
    previousBeatTime = currentTime;

    if (interval > 300 && interval < 2000) {
      heartRate = 60000.0 / interval;
    }
  }

  // Determine heart rhythm and display on OLED 2
  display2.clearDisplay();
  display2.setTextSize(1);
  display2.setTextColor(WHITE);
  display2.setCursor(0, 0);
  display2.print("Heart Rate: ");
  display2.print(heartRate);
  display2.println(" BPM");

  if (heartRate < bpmThresholdLow) {
    display2.println("Bradycardia");
  } else if (heartRate > bpmThresholdHigh) {
    display2.println("Tachycardia");
  } else {
    display2.println("Normal Rhythm");
  }
  display2.display();

  delay(10);
}
