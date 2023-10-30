#include "Queue.h"

int elevatorFloor = 0;
int targetFloor;
enum ElevatorState {
  IDLE,
  MOVING
};
ElevatorState elevatorState = IDLE;

unsigned int lastDebounceTime[3] = { 0, 0, 0 };
unsigned int debounceDelay = 50;
unsigned int interval = 2000;
unsigned int currentMillis = 0;
unsigned int previousMillis = 0;

unsigned int ledOnTime[3] = { 0, 0, 0 };

const int firstButtonPin = 2;
const int secondButtonPin = 4;
const int thirdButtonPin = 6;
const int firstFloorLedPin = 7;
const int secondFloorLedPin = 10;
const int thirdFloorLedPin = 12;
const int stateLedPin = 13;

byte currentButtonState[3] = { LOW, LOW, LOW };
byte lastButtonState[3] = { LOW, LOW, LOW };
byte readingButtonState[3] = { LOW, LOW, LOW };
byte ledState[3] = { LOW, LOW, LOW };
byte buttonPressed[3] = { 0, 0, 0 };
byte stateLedState = HIGH;
Queue<int> floorQueue(10);

void setup() {

  pinMode(firstButtonPin, INPUT_PULLUP);
  pinMode(secondButtonPin, INPUT_PULLUP);
  pinMode(thirdButtonPin, INPUT_PULLUP);

  pinMode(firstFloorLedPin, OUTPUT);
  pinMode(secondFloorLedPin, OUTPUT);
  pinMode(thirdFloorLedPin, OUTPUT);
  pinMode(stateLedPin, OUTPUT);

  Serial.begin(9600);
}

void readButtonStates() {

  readingButtonState[0] = digitalRead(firstButtonPin);
  readingButtonState[1] = digitalRead(secondButtonPin);
  readingButtonState[2] = digitalRead(thirdButtonPin);
  for (int i = 0; i < 3; i++) {
    if (readingButtonState[i] != lastButtonState[i]) {
      lastDebounceTime[i] = millis();
    }
    if (millis() - lastDebounceTime[i] > debounceDelay) {
      if (readingButtonState[i] != currentButtonState[i]) {
        currentButtonState[i] = readingButtonState[i];
        if (currentButtonState[i] == HIGH) {
          floorQueue.push(i);
          buttonPressed[i] = 1;
          ledState[elevatorFloor] = HIGH;
          ledOnTime[elevatorFloor] = millis();
        }
      }
    }
  }

  if (floorQueue.count()) {
    targetFloor = floorQueue.peek();

    if (targetFloor == 0) {
      Serial.println("here");
      if (elevatorFloor == 0) {
        oneFloorMove(0, 0);
        elevatorFloor = 0;
      }
      if (elevatorFloor == 1) {
        oneFloorMove(1, 0);
        elevatorFloor = 0;
      }
      if (elevatorFloor == 2) {
        oneFloorMove(2, 1);
        elevatorFloor = 1;
        oneFloorMove(1, 0);
        elevatorFloor = 0;
      }
    }

    if (targetFloor == 1) {
      Serial.println("here1");
      if (elevatorFloor == 0) {
        digitalWrite(firstFloorLedPin, HIGH);
        ledState[0] = HIGH;
        ledOnTime[0] = millis();
        oneFloorMove(0, 1);
        elevatorFloor = 1;
      }
      if (elevatorFloor == 1) {
        oneFloorMove(1, 1);
        elevatorFloor = 1;
      }
      if (elevatorFloor == 2) {
        oneFloorMove(2, 1);
        elevatorFloor = 1;
      }
      digitalWrite(firstFloorLedPin, ledState[0]);
      digitalWrite(secondFloorLedPin, ledState[1]);
      digitalWrite(thirdFloorLedPin, ledState[2]);
    }

    if (targetFloor == 2) {
      Serial.println("here2");
      if (elevatorFloor == 0) {
        oneFloorMove(0, 1);
        Serial.println(elevatorFloor);
        elevatorFloor = 1;
        Serial.println(elevatorFloor);
        oneFloorMove(1, 2);
        Serial.println(elevatorFloor);
        elevatorFloor = 2;
        Serial.println(elevatorFloor);
      }
      if (elevatorFloor == 1) {
        oneFloorMove(1, 2);
        elevatorFloor = 2;
      }
      if (elevatorFloor == 2) {
        oneFloorMove(2, 2);
        elevatorFloor = 2;
      }
    }

    floorQueue.pop();
    if (elevatorFloor == targetFloor) {
      buttonPressed[elevatorFloor] = 0;
    }
  }

  digitalWrite(firstFloorLedPin, ledState[0]);
  digitalWrite(secondFloorLedPin, ledState[1]);
  digitalWrite(thirdFloorLedPin, ledState[2]);

  lastButtonState[0] = readingButtonState[0];
  lastButtonState[1] = readingButtonState[1];
  lastButtonState[2] = readingButtonState[2];
}

void oneFloorMove(int floor1, int floor2) {  // FLOOR1 = 2, FLOOR2 = 1
                                             //if (buttonPressed[floor2] == 1) {
  ledState[floor1] = HIGH;
  ledOnTime[floor1] = millis();
  //}
  if (floor1 == floor2) {
    if (ledState[floor1] == HIGH) {
      while (millis() - ledOnTime[floor1] < interval) {
      }
      //Serial.println(millis() - ledOnTime[floor1]);
      ledState[floor1] = LOW;
      elevatorState = IDLE;
      //manageElevatorState();
      elevatorFloor = floor1;
    }
  } else {
    //if (buttonPressed[floor2] == 1) {
    if (ledState[floor1] == HIGH) {
      while (millis() - ledOnTime[floor1] < interval) {
      }
      Serial.println("led stins");
      ledState[floor1] = LOW;
      elevatorState = MOVING;
      currentMillis = millis();

      // manageElevatorState();
      ledState[floor2] = HIGH;
      ledOnTime[floor2] = millis();
    }

    if (ledState[floor2] == HIGH) {
      while (millis() - ledOnTime[floor2] < interval) {
      }
      ledState[floor2] = LOW;
      Serial.println(ledState[floor2]);
      elevatorFloor = floor2;
      buttonPressed[floor2] = 0;
    }
    //}
  }
}


void manageElevatorState() {
  unsigned long currentMillis = millis();

  switch (elevatorState) {
    case IDLE:
      {
        digitalWrite(stateLedPin, HIGH);
        break;
      }
    case MOVING:
      {
        int blinkCount = 0;
        currentMillis = millis();
        if (currentMillis - previousMillis >= interval / 10) {
          previousMillis = currentMillis;
          if (stateLedState == LOW) {
            stateLedState = HIGH;
          } else {
            stateLedState = LOW;
          }
          digitalWrite(stateLedPin, stateLedState);

          if (stateLedState == HIGH) {
            blinkCount++;
          }

          if (blinkCount >= 5 && blinkCount <= 10) {
            digitalWrite(stateLedPin, LOW);
          }
        }
        break;
      }
  }
}

void loop() {

  manageElevatorState();

  digitalWrite(firstFloorLedPin, ledState[0]);
  digitalWrite(secondFloorLedPin, ledState[1]);
  digitalWrite(thirdFloorLedPin, ledState[2]);
  digitalWrite(stateLedPin, stateLedState);


  readButtonStates();

  lastButtonState[0] = readingButtonState[0];
  lastButtonState[1] = readingButtonState[1];
  lastButtonState[2] = readingButtonState[2];
}
