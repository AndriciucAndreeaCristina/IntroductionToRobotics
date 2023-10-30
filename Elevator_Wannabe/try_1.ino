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
        if (currentMillis - previousMillis >= interval) {
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

          if (blinkCount >= 5 && blinkCount <= 15) {
            digitalWrite(stateLedPin, LOW);
          }
        }

        break;
      }
  }
}

void loop() {

  readingButtonState[0] = digitalRead(firstButtonPin);
  readingButtonState[1] = digitalRead(secondButtonPin);
  readingButtonState[2] = digitalRead(thirdButtonPin);

  manageElevatorState();

  // the elevator is at the first floor
  if (elevatorFloor == 0) {

    // the first button is pushed
    if (readingButtonState[0] != lastButtonState[0]) {
      lastDebounceTime[0] == millis();
    }
    if (millis() - lastDebounceTime[0] > debounceDelay) {
      if (readingButtonState[0] != currentButtonState[0]) {
        currentButtonState[0] = readingButtonState[0];
        buttonPressed[0] = 1;
        if (currentButtonState[0] == HIGH) {
          ledState[0] = HIGH;
          ledOnTime[0] = millis();
        }
      }
    }
    if (buttonPressed[0] == 1) {
      if (ledState[0] == HIGH) {
        if (millis() - ledOnTime[0] >= interval) {
          ledState[0] = LOW;
          elevatorState = MOVING;
          currentMillis = millis();

          buttonPressed[0] = 0;
        }
      }
    }
    //the second button is pushed
    if (readingButtonState[1] != lastButtonState[1]) {
      lastDebounceTime[1] == millis();
    }
    if (millis() - lastDebounceTime[1] > debounceDelay) {
      if (readingButtonState[1] != currentButtonState[1]) {
        currentButtonState[1] = readingButtonState[1];
        if (currentButtonState[1] == HIGH) {
          buttonPressed[1] = 1;
          //the first led is on
          ledState[0] = HIGH;
          ledOnTime[0] = millis();
        }
      }
    }
    if (buttonPressed[1] == 1) {
      if (ledState[0] == HIGH) {
        if (millis() - ledOnTime[0] >= interval) {
          ledState[0] = LOW;
          elevatorState = MOVING;
          manageElevatorState();
          int now = millis();
          while (millis() - now < interval) {
            int blinkCount = 0;
            currentMillis = millis();
            if (currentMillis - previousMillis >= interval) {
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

              if (blinkCount >= 5 && blinkCount <= 15) {
                digitalWrite(stateLedPin, LOW);
              }
            }
          }
          ledState[1] = HIGH;
          ledOnTime[1] = millis();
        }
      }

      if (ledState[1] == HIGH) {
        if (millis() - ledOnTime[1] >= interval) {
          ledState[1] = LOW;
          elevatorState = IDLE;
          elevatorFloor = 1;
          buttonPressed[1] = 0;
        }
      }
    }

    // the third button is pushed
    if (readingButtonState[2] != lastButtonState[2]) {
      lastDebounceTime[2] == millis();
    }
    if (millis() - lastDebounceTime[2] > debounceDelay) {
      if (readingButtonState[2] != currentButtonState[2]) {
        currentButtonState[2] = readingButtonState[2];
        buttonPressed[2] = 1;
        if (currentButtonState[2] == HIGH) {
          ledState[0] = HIGH;
          ledOnTime[0] = millis();
        }
      }
    }
    if (buttonPressed[2] == 1) {
      if (ledState[0] == HIGH) {
        if (millis() - ledOnTime[0] >= interval) {
          ledState[0] = LOW;
          elevatorState = MOVING;
          manageElevatorState();
          int now = millis();
          while (millis() - now < interval) {
            int blinkCount = 0;
            currentMillis = millis();
            if (currentMillis - previousMillis >= interval) {
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

              if (blinkCount >= 5 && blinkCount <= 15) {
                digitalWrite(stateLedPin, LOW);
              }
            }
          }
          ledState[1] = HIGH;
          ledOnTime[1] = millis();
        }
      }

      if (ledState[1] == HIGH) {
        if (millis() - ledOnTime[1] >= interval) {
          ledState[1] = LOW;
          elevatorState = MOVING;
          manageElevatorState();
          int now = millis();
          while (millis() - now < interval) {
            int blinkCount = 0;
            currentMillis = millis();
            if (currentMillis - previousMillis >= interval) {
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

              if (blinkCount >= 5 && blinkCount <= 15) {
                digitalWrite(stateLedPin, LOW);
              }
            }
          }
          ledState[2] = HIGH;
          ledOnTime[2] = millis();
        }
      }

      if (ledState[2] == HIGH) {
        if (millis() - ledOnTime[2] >= interval) {
          ledState[2] = LOW;
          elevatorState = IDLE;
          elevatorFloor = 2;
          buttonPressed[2] = 0;
        }
      }
    }
  }

  if (elevatorFloor == 1) {
    // the first button is pushed
    if (readingButtonState[0] != lastButtonState[0]) {
      lastDebounceTime[0] == millis();
    }
    if (millis() - lastDebounceTime[0] > debounceDelay) {
      if (readingButtonState[0] != currentButtonState[0]) {
        currentButtonState[0] = readingButtonState[0];
        buttonPressed[0] = 1;
        if (currentButtonState[0] == HIGH) {
          ledState[1] = HIGH;
          ledOnTime[1] = millis();
        }
      }
    }
    if (buttonPressed[0] == 1) {
      if (ledState[1] == HIGH) {
        if (millis() - ledOnTime[1] >= interval) {
          ledState[1] = LOW;
          elevatorState = MOVING;
          manageElevatorState();
          int now = millis();
          while (millis() - now < interval) {
            int blinkCount = 0;
            currentMillis = millis();
            if (currentMillis - previousMillis >= interval) {
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

              if (blinkCount >= 5 && blinkCount <= 15) {
                digitalWrite(stateLedPin, LOW);
              }
            }
          }
          ledState[0] = HIGH;
          ledOnTime[0] = millis();
        }
      }

      if (ledState[0] == HIGH) {
        if (millis() - ledOnTime[0] >= interval) {
          ledState[0] = LOW;
          elevatorState = IDLE;
          elevatorFloor = 0;
          buttonPressed[0] = 0;
        }
      }
    }

    //the second button is pushed
    if (readingButtonState[1] != lastButtonState[1]) {
      lastDebounceTime[1] == millis();
    }
    if (millis() - lastDebounceTime[1] > debounceDelay) {
      if (readingButtonState[1] != currentButtonState[1]) {
        currentButtonState[1] = readingButtonState[1];
        buttonPressed[1] = 1;
        if (currentButtonState[1] == HIGH) {
          ledState[1] = HIGH;
          ledOnTime[1] = millis();
        }
      }
    }
    if (buttonPressed[1] == 1) {
      if (ledState[1] == HIGH) {
        if (millis() - ledOnTime[1] >= interval) {
          ledState[1] = LOW;
          elevatorState = IDLE;
          buttonPressed[1] = 0;
        }
      }
    }

    // the third button is pushed
    if (readingButtonState[2] != lastButtonState[2]) {
      lastDebounceTime[2] == millis();
    }
    if (millis() - lastDebounceTime[2] > debounceDelay) {
      if (readingButtonState[2] != currentButtonState[2]) {
        currentButtonState[2] = readingButtonState[2];
        buttonPressed[2] = 1;
        if (currentButtonState[2] == HIGH) {
          ledState[1] = HIGH;
          ledOnTime[1] = millis();
        }
      }
    }
    if (buttonPressed[2] == 1) {
      if (ledState[1] == HIGH) {
        if (millis() - ledOnTime[1] >= interval) {
          ledState[1] = LOW;
          elevatorState = MOVING;
          manageElevatorState();
          int now = millis();
          while (millis() - now < interval) {
            int blinkCount = 0;
            currentMillis = millis();
            if (currentMillis - previousMillis >= interval) {
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

              if (blinkCount >= 5 && blinkCount <= 15) {
                digitalWrite(stateLedPin, LOW);
              }
            }
          }
          ledState[2] = HIGH;
          ledOnTime[2] = millis();
        }
      }


      if (ledState[2] == HIGH) {
        if (millis() - ledOnTime[2] >= interval) {
          ledState[2] = LOW;
          elevatorState = IDLE;
          elevatorFloor = 2;
          buttonPressed[2] = 0;
        }
      }
    }
  }

  if (elevatorFloor == 2) {

    // the first button is pushed
    if (readingButtonState[0] != lastButtonState[0]) {
      lastDebounceTime[0] == millis();
    }
    if (millis() - lastDebounceTime[0] > debounceDelay) {
      if (readingButtonState[0] != currentButtonState[0]) {
        currentButtonState[0] = readingButtonState[0];
        buttonPressed[0] = 1;
        if (currentButtonState[0] == HIGH) {
          ledState[2] = HIGH;
          ledOnTime[2] = millis();
        }
      }
    }
    if (buttonPressed[0] == 1) {
      if (ledState[2] == HIGH) {
        if (millis() - ledOnTime[2] >= interval) {
          ledState[2] = LOW;
          elevatorState = MOVING;
          manageElevatorState();
          int now = millis();
          while (millis() - now < interval) {
            int blinkCount = 0;
            currentMillis = millis();
            if (currentMillis - previousMillis >= interval) {
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

              if (blinkCount >= 5 && blinkCount <= 15) {
                digitalWrite(stateLedPin, LOW);
              }
            }
          }
          ledState[1] = HIGH;
          ledOnTime[1] = millis();
        }
      }

      if (ledState[1] == HIGH) {
        if (millis() - ledOnTime[1] >= interval) {
          ledState[1] = LOW;
          elevatorState = MOVING;
          manageElevatorState();
          int now = millis();
          while (millis() - now < interval) {
            int blinkCount = 0;
            currentMillis = millis();
            if (currentMillis - previousMillis >= interval) {
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

              if (blinkCount >= 5 && blinkCount <= 15) {
                digitalWrite(stateLedPin, LOW);
              }
            }
          }
          ledState[0] = HIGH;
          ledOnTime[0] = millis();
        }
      }

      if (ledState[0] == HIGH) {
        if (millis() - ledOnTime[0] >= interval) {
          ledState[0] = LOW;
          elevatorState = IDLE;
          elevatorFloor = 0;
          buttonPressed[0] = 0;
        }
      }
    }

    //the second button is pushed
    if (readingButtonState[1] != lastButtonState[1]) {
      lastDebounceTime[1] == millis();
    }
    if (millis() - lastDebounceTime[1] > debounceDelay) {
      if (readingButtonState[1] != currentButtonState[1]) {
        currentButtonState[1] = readingButtonState[1];
        if (currentButtonState[1] == HIGH) {
          buttonPressed[1] = 1;
          //the third led is on
          ledState[2] = HIGH;
          ledOnTime[2] = millis();
        }
      }
    }
    if (buttonPressed[1] == 1) {
      if (ledState[2] == HIGH) {
        if (millis() - ledOnTime[2] >= interval) {
          ledState[2] = LOW;
          elevatorState = MOVING;
          manageElevatorState();
          int now = millis();
          while (millis() - now < interval) {
            int blinkCount = 0;
            currentMillis = millis();
            if (currentMillis - previousMillis >= interval) {
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

              if (blinkCount >= 5 && blinkCount <= 15) {
                digitalWrite(stateLedPin, LOW);
              }
            }
          }
          ledState[1] = HIGH;
          ledOnTime[1] = millis();
        }
      }

      if (ledState[1] == HIGH) {
        if (millis() - ledOnTime[1] >= interval) {
          ledState[1] = LOW;
          elevatorState = IDLE;
          elevatorFloor = 1;
          buttonPressed[1] = 0;
        }
      }
    }

    // the third button is pushed
    if (readingButtonState[2] != lastButtonState[2]) {
      lastDebounceTime[2] == millis();
    }
    if (millis() - lastDebounceTime[2] > debounceDelay) {
      if (readingButtonState[2] != currentButtonState[2]) {
        currentButtonState[2] = readingButtonState[2];
        buttonPressed[2] = 1;
        if (currentButtonState[2] == HIGH) {
          ledState[2] = HIGH;
          ledOnTime[2] = millis();
        }
      }
    }
    if (buttonPressed[2] == 1) {
      if (ledState[2] == HIGH) {
        if (millis() - ledOnTime[2] >= interval) {
          ledState[2] = LOW;
          elevatorState = IDLE;
          buttonPressed[2] = 0;
        }
      }
    }
  }
  Serial.print("Current floor: ");
  Serial.println(elevatorFloor);
  Serial.print("ElevatorState: ");
  Serial.println(elevatorState);

  digitalWrite(firstFloorLedPin, ledState[0]);
  digitalWrite(secondFloorLedPin, ledState[1]);
  digitalWrite(thirdFloorLedPin, ledState[2]);

  lastButtonState[0] = readingButtonState[0];
  lastButtonState[1] = readingButtonState[1];
  lastButtonState[2] = readingButtonState[2];
}
