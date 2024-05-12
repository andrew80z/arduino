
#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif
#include "RTClib.h"
#include <Adafruit_SSD1306.h>

#define OLED_RESET 4
#define LED_MODE 1


Adafruit_SSD1306 display(OLED_RESET);
// Clock
RTC_DS1307 RTC;
// Button and button LED
const int measureLedPin = 3;
const int measureButtonPin = 4;
// LED part
int measureLedState = LOW;
int ledFadeValue = 0;
int ledFadeStep = 5;
int ledFadeInterval = 5;           //milliseconds
const boolean breathMode = false;  // if or not the led lights as breath mode when it's on
int measureButtonState;              // the current reading from the input pin
int measureLastButtonState = HIGH;   // the previous reading from the input pin
unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 30;    // the debounce time; increase if the output flickers
unsigned long lastLedFadeTime = 0;

// Relay
const int relayPin = 8;

int tempEquiq = 0;
int maxVal = 682;
int minVal = 456;
const String soilWellMsg = "Watering not needed";
const String soilDryMsg = "Watering needed!";
String soilStateMsg;
int dryBorder = 30;



void setup(void) {
  pinMode(measureButtonPin, INPUT);
  pinMode(measureLedPin, OUTPUT);
  digitalWrite(measureLedPin, measureLedState);
  Serial.begin(9600);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  Wire.begin();
  RTC.begin();
  digitalWrite(relayPin, LOW);
   RTC.adjust(DateTime(F(__DATE__), F(__TIME__)));  // sync the RTC module
// Setup clock from PC if connected
  if (!RTC.isrunning()) {
    Serial.println("RTC is NOT running!");
    RTC.adjust(DateTime(F(__DATE__), F(__TIME__)));  // sync the RTC module
  };


  if (tempEquiq == 0) {
    Serial.println("Setting humidity");
    int setupTmpHum = getHumidity();
    humidityState(setupTmpHum);

    display.print("Humidity: ");
    display.print(setupTmpHum);
    display.print("%   ");

    tempEquiq = setupTmpHum;
  }
};

void loop(void) {
  // Reading button state
  int reading = digitalRead(measureButtonPin);

  // If the switch changed, due to noise or pressing:
  if (reading != measureButtonState) {
    measureButtonState = reading;
    // reset the debouncing timer
    lastDebounceTime = millis();

    if (measureButtonState == LOW) {  //button is pressed
      measureLedState = HIGH;
      ledFadeValue = 255;
      // read the input on analog pin 0:
      int tmpHum = getHumidity();
      humidityState(tmpHum);
      tempEquiq = tmpHum;

    } else {  //button is released
      measureLedState = LOW;
      ledFadeValue = 0;
    }
  }
  // set the LED:
  if (breathMode && measureLedState != LOW) {
    if (millis() - lastLedFadeTime > ledFadeInterval) {
      lastLedFadeTime = millis();
      analogWrite(measureLedPin, ledFadeValue);
      ledFadeValue += ledFadeStep;
      if (ledFadeValue > 255) {
        ledFadeValue = 255 - ledFadeStep;
        ledFadeStep = -ledFadeStep;
      } else if (ledFadeValue < 0) {
        ledFadeValue = 0;
        ledFadeStep = -ledFadeStep;
      }
    }
  } else {
    digitalWrite(measureLedPin, measureLedState);
  }
  measureLastButtonState = reading;

  //Check by time
  DateTime now = RTC.now();
  int sensorValue = analogRead(A0);

  if (now.minute() > 30) {
    // read the input on analog pin 0:
    int tmpHum = getHumidity();
    // print out the value you read:
    delay(3000);  // delay in between reads for stability
    humidityState(tmpHum);
    tempEquiq = tmpHum;
  }
  // for debugging needs
  Serial.print(now.year(), DEC);
  Serial.print('/');
  Serial.print(now.month(), DEC);
  Serial.print('/');
  Serial.print(now.day(), DEC);
  Serial.print(' ');
  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  Serial.print(now.second(), DEC);
  Serial.print(". Humidity is:");
  Serial.print(tempEquiq);
  Serial.println("%");
  delay(1000);
  //Display part
  display.setTextSize(1);
  display.setTextColor(WHITE, BLACK);
  display.setCursor(2, 2);
  display.print(soilStateMsg);
  display.setCursor(2, 10);
  display.print(now.year(), DEC);
  display.print('/');
  display.print(now.month(), DEC);
  display.print('/');
  display.print(now.day(), DEC);
  display.print(' ');
  display.print(now.hour(), DEC);
  display.print(':');
  display.print(now.minute(), DEC);
  display.setCursor(2, 20);
  display.print("Humidity: ");
  display.print(tempEquiq);
  display.print("%   ");
  display.display();
  display.clearDisplay();
}

int getHumidity() {
  delay(1000);
  int sensorValue = analogRead(A0);
  int myHumVal = (sensorValue - maxVal) / (minVal / 100) * -1;
  delay(1000);
  return myHumVal;
}

void humidityState(int humVal) {
  if (humVal < dryBorder) {
    soilStateMsg = soilDryMsg;
    digitalWrite(relayPin, HIGH);
    delay(300000);
    digitalWrite(relayPin, LOW);
  } else {
    soilStateMsg = soilWellMsg;
    digitalWrite(relayPin, LOW);
  }
}
