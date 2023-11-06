#include <ezButton.h>

const int pinSW = 2;  // Digital pin connected to switch output
const int pinX = A0;  // Analog pin connected to X output
const int pinY = A1;  // Analog pin connected to Y output

const int pinA = 12;
const int pinB = 10;
const int pinC = 9;
const int pinD = 8;
const int pinE = 7;
const int pinF = 6;
const int pinG = 5;
const int pinDP = 4;
const int segSize = 8;

bool commonAnode = false;

int interval = 300;
int stateSegments[segSize] = { LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW };
int isOnByButtonPress[segSize] = { LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW };
int currentSegment = pinDP;  // Initialize with the decimal point
int minThreshold = 400;
int maxThreshold = 600;
int lastDebounceTime = 0;
int debounceDelay = 50;
ezButton button(2);

const int SHORT_PRESS_TIME = 500;
const int LONG_PRESS_TIME = 1000;
int lastStateButton = LOW;
int currentStateButton;
unsigned long pressedTime = 0;
unsigned long releasedTime = 0;
int segments[segSize] = {
  pinA, pinB, pinC, pinD, pinE, pinF, pinG, pinDP
};
bool joyMoved = false;
unsigned long lastMoveTime = 0;
void setup() {
  for (int i = 0; i < segSize; i++) {
    pinMode(segments[i], OUTPUT);
  }
  pinMode(pinSW, INPUT_PULLUP);
  if (commonAnode) {
    for (int i = 0; i < segSize; i++) {
      stateSegments[i] = HIGH;
    }
  }
  button.setDebounceTime(50);
  Serial.begin(9600);
}

void loop() {
  button.loop();
  blinkSegment(currentSegment);
  checkButton();
  int xValue = analogRead(pinX);
  int yValue = analogRead(pinY);

  if (millis() - lastMoveTime >= debounceDelay) {
    if (xValue < minThreshold && joyMoved == false) {
      moveLeft();
      joyMoved = true;
      lastMoveTime = millis(); 
    } else if (xValue > maxThreshold && joyMoved == false) {
      moveRight();
      joyMoved = true;
      lastMoveTime = millis();  
    } else if (yValue < minThreshold && joyMoved == false) {
      moveUp();
      joyMoved = true;
      lastMoveTime = millis(); 
    } else if (yValue > maxThreshold && joyMoved == false) {
      moveDown();
      joyMoved = true;
      lastMoveTime = millis(); 
    }
  }

  if (xValue >= minThreshold && xValue <= maxThreshold && yValue >= minThreshold && yValue <= maxThreshold) {
    joyMoved = false;
  }
  
  for (int i = 0; i < segSize; i++) {
    if (i != getIndexFromPin(currentSegment)) {
      digitalWrite(segments[i], isOnByButtonPress[i]);
    }
  }
}

void moveLeft() {
  Serial.println("Left move on Oy axis");
  int index = getIndexFromPin(currentSegment);
  stateSegments[index] = LOW;
  switch (currentSegment) {
    case pinA:
      {
        currentSegment = pinF;
        break;
      }
    case pinB:
      {
        currentSegment = pinF;
        break;
      }
    case pinC:
      {
        currentSegment = pinE;
        break;
      }
    case pinD:
      {
        currentSegment = pinE;
        break;
      }
    case pinDP:
      {
        currentSegment = pinC;
        break;
      }
  }
  digitalWrite(segments[index], stateSegments[index]);
  Serial.print("CurrentSegment after move: ");
  Serial.println(getIndexFromPin(currentSegment));
}

void moveRight() {
  Serial.println("Right move on Oy axis");
  int index = getIndexFromPin(currentSegment);
  switch (currentSegment) {
    case pinA:
      currentSegment = pinB;
      break;
    case pinC:
      currentSegment = pinDP;
      break;
    case pinD:
      currentSegment = pinC;
      break;
    case pinE:
      currentSegment = pinC;
      break;
    case pinF:
      currentSegment = pinB;
      break;
  }
  digitalWrite(segments[index], stateSegments[index]);
  Serial.print("CurrentSegment after move: ");
  Serial.println(getIndexFromPin(currentSegment));
}

void moveUp() {
  Serial.println("Up move on Ox axis");
  int index = getIndexFromPin(currentSegment);
  switch (currentSegment) {
    case pinB:
      currentSegment = pinA;
      break;
    case pinC:
      currentSegment = pinG;
      break;
    case pinD:
      currentSegment = pinG;
      break;
    case pinE:
      currentSegment = pinG;
      break;
    case pinF:
      currentSegment = pinA;
      break;
    case pinG:
      currentSegment = pinA;
      break;
  }
  digitalWrite(segments[index], stateSegments[index]);
  Serial.print("CurrentSegment after move: ");
  Serial.println(getIndexFromPin(currentSegment));
}

void moveDown() {
  Serial.println("down move on Ox axis");
  int index = getIndexFromPin(currentSegment);
  switch (currentSegment) {
    case pinA:
    case pinB:
    case pinF:
      currentSegment = pinG;
      break;
    case pinC:
    case pinE:
    case pinG:
      currentSegment = pinD;
      break;
  }
  digitalWrite(segments[index], stateSegments[index]);
  Serial.print("CurrentSegment after move: ");
  Serial.println(getIndexFromPin(currentSegment));
}

void blinkSegment(int pin) {
  unsigned long currentMillis = millis();
  static unsigned long previousMillis = 0;
  int index = getIndexFromPin(pin);

  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    stateSegments[index] = !stateSegments[index];
    digitalWrite(pin, stateSegments[index]);
  }
}

bool checkButton() {
  if (button.isPressed())
    pressedTime = millis();

  if (button.isReleased()) {
    releasedTime = millis();

    long pressDuration = releasedTime - pressedTime;

    if (pressDuration < SHORT_PRESS_TIME) {
      Serial.println("A short press is detected");
      int index = getIndexFromPin(currentSegment);
      isOnByButtonPress[index] = !isOnByButtonPress[index];
      digitalWrite(segments[index], isOnByButtonPress[index]);
    }
    if (pressDuration > LONG_PRESS_TIME) {
      Serial.println("A long press is detected");
      reset();
    }
  }
}

int getIndexFromPin(int pin) {
  for (int i = 0; i < segSize; i++) {
    if (segments[i] == pin)
      return i;
  }
}

void reset() {
  currentSegment = pinDP;
  for (int i = 0; i < segSize; i++) {
    stateSegments[i] = LOW;
    isOnByButtonPress[i] = LOW;
    digitalWrite(segments[i], stateSegments[i]);
  }
}
