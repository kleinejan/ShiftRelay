#include "classes.h"
#include <Wire.h>
#include "PCF85063TP.h"
#include <TimeLib.h>
#include <RotaryEncoder.h>
#include "rgb_lcd.h"
rgb_lcd lcd;

#define ROTARYSTEPS 1
#define ROTARYMIN 0
#define ROTARYMAX 256

unsigned long previousMillis = 0; // last time update
long interval = 500; // interval at which to do something (milliseconds)
const int buttonPin = 6;     // the number of the pushbutton pin

long cNumber;


// Variables will change:
int buttonPushCounter = 0;   // counter for the number of button presses
int buttonState = 0;         // current state of the button
int lastButtonState = 0;     // previous state of the button

PCD85063TP clock;//define a object of PCD85063TP class
RotaryEncoder encoder(4, 5);
// Last known rotary position.
int lastPos = -1;

void setup()
{
  Serial.begin(9600);
  clock.begin();
  pinMode(buttonPin, INPUT);
  //clock.setcalibration(1, 32767.2);  // Setting offset by clock frequency
  uint8_t ret = clock.calibratBySeconds(0, -0.000041);
  Serial.print("offset value: ");
  Serial.print("0x");
  Serial.println(ret, HEX);
  setLocalTime();
  encoder.setPosition(0 / ROTARYSTEPS); // start with the value of 10.

  lcd.begin(16, 2);

  lcd.setRGB(255, 255, 0);
  lcd.print("hello, world!");

}

void setLocalTime() {
  clock.getTime();
  int hr = clock.hour;
  int min = clock.minute;
  int sec = clock.second;
  int month = clock.month;
  int day = clock.dayOfMonth;
  int yr = clock.year + 2000;
  setTime(hr, min, sec, day, month, yr);

}



void loop()
{
  buttonState = digitalRead(buttonPin);
  unsigned long currentMillis = millis();
  encoder.tick();

  if (currentMillis - previousMillis > interval) {
    previousMillis = currentMillis;
    countDown();
  }

  // compare the buttonState to its previous state
  if (buttonState != lastButtonState) {
    // if the state has changed, increment the counter
    if (buttonState == HIGH) {
      // if the current state is HIGH then the button went from off to on:
      buttonPushCounter++;
      Serial.println("on");
      Serial.print("number of button pushes: ");
      Serial.println(buttonPushCounter);
    } else {
      // if the current state is LOW then the button went from on to off:
      Serial.println("off");
    }
    // Delay a little bit to avoid bouncing
    delay(50);
  }
  // save the current state as the last state, for next time through the loop
  lastButtonState = buttonState;


  // turns on the LED every four button pushes by checking the modulo of the
  // button push counter. the modulo function gives you the remainder of the
  // division of two numbers:
  if (buttonPushCounter >=4) {
    buttonPushCounter=0;
  } 
  
 
  // get the current physical position and calc the logical position
  int newPos = encoder.getPosition() * ROTARYSTEPS;

  if (newPos < ROTARYMIN) {
    encoder.setPosition(ROTARYMIN / ROTARYSTEPS);
    newPos = ROTARYMIN;
    cNumber = newPos ;

  } else if (newPos > ROTARYMAX) {
    encoder.setPosition(ROTARYMAX / ROTARYSTEPS);
    newPos = ROTARYMAX;
    cNumber = newPos ;
  } // if

  if (lastPos != newPos) {
    //    Serial.print(newPos);
    //    Serial.println();
    lastPos = newPos;
    cNumber = newPos;
    countDown();
  }

}
/*Function: Display time on the serial monitor*/




void countDown() {
  clock.getTime();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(returnDigits(clock.dayOfMonth));
  lcd.print("/");
  lcd.print(returnDigits(clock.month));
  lcd.print("/");
  lcd.print(clock.year + 2000);
  lcd.print(":");
  lcd.print(clock.second);

  lcd.setCursor(0, 1);
  long extraTime = 86400 * cNumber;

  lcd.print(returnDigits(day(now() + extraTime)));
  lcd.print("/");
  lcd.print(returnDigits(month(now() + extraTime)));
  lcd.print("/");
  lcd.print(year(now() + extraTime));
  lcd.print("|");

  lcd.print(returnDigits(cNumber, 2));
  //  delay(100);
}





