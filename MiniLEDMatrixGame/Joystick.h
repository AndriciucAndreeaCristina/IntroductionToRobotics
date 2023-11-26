#ifndef JOYSTICK_H
#define JOYSTICK_H
#include <Arduino.h>

class Joystick {
private:
  int xAxisPin;
  int yAxisPin;
  int buttonPin;
  int xPos;
  int yPos;
  bool buttonState;
  bool joyMovedX;
  bool joyMovedY;
  static Joystick* instance;
  const int minThreshold = 400;
  const int maxThreshold = 600;
  unsigned long lastDebounceTime;
  bool lastButtonReading;
  const int debounce = 50;

  Joystick::Joystick(int xPin, int yPin, int bPin)
    : xAxisPin(xPin), yAxisPin(yPin), buttonPin(bPin), xPos(0), yPos(0), joyMovedX(false), joyMovedY(false), buttonState(false), lastButtonReading(false) {
    pinMode(buttonPin, INPUT_PULLUP);
    pinMode(xAxisPin, INPUT);
    pinMode(yAxisPin, INPUT);
  }



public:
  Joystick(Joystick& other) = delete;
  Joystick operator=(const Joystick&) = delete;
  static Joystick* getInstance(int xPin, int yPin, int bPin);
  int readInput(int pin) {
    int value = analogRead(pin);
    if (value <= minThreshold) {
      return -1;
    } else if (value >= maxThreshold) {
      return 1;
    }
    return 0;
  }
  int getXPosition() {
    return readInput(xAxisPin);
  }
  int getYPosition() {
    return readInput(yAxisPin);
  }
  bool getButtonState() {
    int reading = digitalRead(buttonPin);
    if (reading != lastButtonReading) {
      lastDebounceTime = millis();
    }
    if ((millis() - lastDebounceTime) > debounce) {
      if (reading != buttonState) {
        buttonState = reading;
      }
    }
    lastButtonReading = reading;
    return buttonState;
  }
};

Joystick* Joystick::instance = nullptr;

Joystick* Joystick::getInstance(int xPin, int yPin, int bPin) {
  if (instance == nullptr) {
    instance = new Joystick(xPin, yPin, bPin);
  }
  return instance;
}

#endif
