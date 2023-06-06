#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Keyboard.h>

#define SCREEN_WIDTH 128  // OLED display width, in pixels
#define SCREEN_HEIGHT 32  // OLED display height, in pixels

#define OLED_RESET -1        // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C  ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

unsigned long start, match, delta, low, high, bad;
uint16_t val, measurements_nr;

const uint16_t repeats = 15;

uint16_t measurements[repeats];

void setup() {
  bad = 0;
  pinMode(A1, INPUT);
  Wire.setSDA(20);
  Wire.setSCL(21);
  Serial.begin(9600);

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ;  // Don't proceed, loop forever
  }

  Keyboard.begin();
  display.setTextColor(SSD1306_WHITE);  // Draw white text
  display.setTextSize(1);               // Draw 2X-scale text
  display.ssd1306_command(SSD1306_SETCONTRAST);
  display.ssd1306_command(0x8F);

  analogReadResolution(12);
}

uint16_t medianMeasurement() {

  uint16_t temp = 0;
  /* sorting */
  for (uint8_t i = 0; i < measurements_nr - 1; i++) {
    for (uint8_t j = 0; j < measurements_nr - i - 1; j++) {
      if (measurements[j] < measurements[j + 1]) {
        temp = measurements[j];
        measurements[j] = measurements[j + 1];
        measurements[j + 1] = temp;
      }
    }
  }

  if (measurements_nr % 2 == 0)
    return (measurements[(measurements_nr / 2) - 1] + measurements[(measurements_nr / 2)]) / 2;
  else
    return measurements[(measurements_nr / 2)];
}
void updateScreen() {
  display.setCursor(0, 0);  // Start at top-left corner
  display.clearDisplay();
  display.print("brightness ");
  display.println(val);
  display.print("low ");
  display.print(low);

  display.print("   "); // 3 spaces = len(median) - len(low)
  display.print(" high ");
  display.println(high);

  display.print("median ");
  display.print(medianMeasurement());

  display.print(" bad ");
  display.print(bad);
  display.display();
}
int measure() {
  // 40-79 with median at 59 INcluding keypress in measurement
  // 39-60 with median at 59 EXcluding keypress in measurement
  Keyboard.press(' ');
  start = millis();

  val = analogRead(A1);

  while (val < 900) {  // DARK, TODO: calibration
    delay(1);
    updateScreen();
    val = analogRead(A1);
    if ((millis() - start) > 200) {  // failed measurement, waited 200 ms for a new value
      Keyboard.release(' ');
      bad++;
      return 0;
    }
  }

  long end = millis();
  Keyboard.release(' ');

  while (val > 900) {  // wait for screen to go dark again
    delay(15);
    updateScreen();
    val = analogRead(A1);
    Keyboard.release(' ');
  }
  return end - start;
}
void loop() {
  val = analogRead(A1);

  if (BOOTSEL) {
    while (BOOTSEL) {}
    bad = 0;
    memset(measurements, 0, sizeof(measurements) / sizeof(measurements[0]));
    low = 899;
    high = 0;
    for (uint8_t i = 0; i < repeats; i++) {
      delta = measure();
      if (delta > 2) {
        high = max(high, delta);
        low = min(low, delta);
        measurements[i] = delta;
        measurements_nr = i;
      } else {
        i--;  // bad measurement
      }
      delay(50);
      updateScreen();
    }

    delay(100);
  }
  updateScreen();
  delay(10);
}
