#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// OLED display settings
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

const int ECG_PIN = A0;   // AD8232 output connected to A0
const int LO_PLUS = 2;    // LO+ pin connected to D2 (optional)
const int LO_MINUS = 3;   // LO- pin connected to D3 (optional)

// Variables to store previous point
int previousX = 0;
int previousY = SCREEN_HEIGHT / 2;

// Debouncing variables
unsigned long lastLeadOffTime = 0;
const unsigned long debounceDelay = 100; // 100 milliseconds

void setup() {
  Serial.begin(9600);
  
  // Initialize OLED display
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }
  display.clearDisplay();
  display.display();

  // Setup LO+ and LO- pins (optional)
  pinMode(LO_PLUS, INPUT);
  pinMode(LO_MINUS, INPUT);

  Serial.println("AD8232 ECG Monitor");
}

void loop() {
  // Debounce "Leads Off" detection
  if ((digitalRead(LO_PLUS) == 1 || digitalRead(LO_MINUS) == 1)) {
    unsigned long currentTime = millis();
    if (currentTime - lastLeadOffTime > debounceDelay) {
      display.clearDisplay();
      display.setTextSize(1);
      display.setTextColor(WHITE);
      display.setCursor(0, 0);
      display.println("Leads Off!");
      display.display();
      Serial.println("Leads Off!");
      lastLeadOffTime = currentTime;
      delay(500);
      return;
    }
  }

  // Read ECG data
  int sensorValue = analogRead(ECG_PIN);
  float voltage = sensorValue * (5.0 / 1023.0);

  // Map the sensor value to a larger display range
  // Adjust these values to make the ECG line larger
  int yPos = map(sensorValue, 300, 700, 0, SCREEN_HEIGHT - 1);
  
  static int xPos = 0;

  // Draw a line from the previous point to the current point
  display.drawLine(previousX, previousY, xPos, SCREEN_HEIGHT - yPos, WHITE);
  display.display();

  // Update previous coordinates
  previousX = xPos;
  previousY = SCREEN_HEIGHT - yPos;

  // Move to the next x position
  xPos++;
  if (xPos >= SCREEN_WIDTH) {
    xPos = 0;
    display.clearDisplay();
  }

  delay(10); // Adjust to change waveform speed
}


  
