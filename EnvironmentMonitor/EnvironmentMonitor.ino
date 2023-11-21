#include <EEPROM.h>

const int trigPin = 9;
const int echoPin = 10;
long trigDuration = 0;
long echoDuration = 0;
int photocellPin = 0;
int photocellValue;
int distance = 0;
const int ledCathodeGreen = 3;
const int ledCathodeBlue = 5;
const int ledCathodeRed = 7;
byte stateBlue = LOW;
byte stateGreen = LOW;
byte stateRed = LOW;
enum State { MAIN,
             MENU1,
             MENU2,
             MENU3,
             MENU4,
             MENU11,
             MENU12,
             MENU13,
             MENU21,
             MENU22,
             MENU31,
             MENU32,
             MENU33,
             MENU41,
             MENU42 };
State systemState = MAIN;
int currentInput = -1;
int currentState = -1; 
int ultrasoundSamplingRate = -1;
int LDRSamplingRate = -1;
int lastRead = 0;
const int debounce = 50;
int persistentUltrasoundSamplingRate = -1;
int persistentLDRSamplingRate = -1;
int ultrasonicThreshold = -1;
int persistentUltrasonicThreshold = -1;
int LDRThreshold = -1;
int persistentLDRThreshold = -1;
int adressStartEEPROM = 0;
int stop = -1;
unsigned long previousMillis = 0;
const int numberDataSaved = 10;
int savedDataUltrasonicSensor[numberDataSaved] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
int savedDataLDRSensor[numberDataSaved] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
int toPrintDataUltrasonicSensor[numberDataSaved] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
int toPrintDataLDRSensor[numberDataSaved] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
unsigned long currentMillisPrint = 0;
unsigned long previousMillisPrint = 0;
unsigned long currentMillis = 0;
unsigned long currentMicros = 0;
unsigned long currentMicros2 = 0;
unsigned long previousMicros = 0;
unsigned long previousPrintMicros2 = 0;
const int sizeInt = 2;
bool isHigh = false;
unsigned long previousPrintMicros = 0;
int indexUS = 0;
int indexLDR = 0;
bool ledAuto = false;
int red = -1;
int blue = -1;
int green = -1;

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(ledCathodeBlue, OUTPUT);
  pinMode(ledCathodeGreen, OUTPUT);
  pinMode(ledCathodeRed, OUTPUT);
  digitalWrite(ledCathodeBlue, stateBlue);
  digitalWrite(ledCathodeGreen, stateGreen);
  digitalWrite(ledCathodeRed, stateRed);
  Serial.begin(9600);
  Serial.flush();
  displayMainMenu();
}

void loop() {
  if (Serial.available() > 0 && currentInput < 0 && (millis() - lastRead > debounce)) {
    currentInput = Serial.parseInt();
    lastRead = millis();
  }

  if (currentInput >= 0 || currentState >= 0) {
    switch (systemState) {
      case MAIN:
        {
          switch (currentInput) {
            case 1:
              {
                systemState = MENU1;
                displayMenu1();
                currentInput = -1;
                currentState = currentInput;
                break;
              }
            case 2:
              {
                systemState = MENU2;
                displayMenu2();
                currentInput = -1;
                currentState = currentInput;
                break;
              }
            case 3:
              {
                systemState = MENU3;
                displayMenu3();
                currentInput = -1;
                currentState = currentInput;
                break;
              }
            case 4:
              {
                systemState = MENU4;
                displayMenu4();
                currentInput = -1;
                currentState = currentInput;
                break;
              }
            default:
              {
                systemState = MAIN;
                Serial.println("Invalid option! Please try again!");
                displayMainMenu();
                currentInput = -1;
                currentState = currentInput;
                break;
              }
          }
          break;
        }
      case MENU1:
        {
          switch (currentInput) {
            case 1:
              {
                systemState = MENU11;
                Serial.println("Enter sampling rate (1-10 seconds) for ultrasound and LDR sensors:");
                currentState = currentInput;
                break;
              }
            case 2:
              {
                systemState = MENU12;
                Serial.println("Enter threshold value for ultrasound sensors:");
                currentState = currentInput;
                break;
              }
            case 3:
              {
                systemState = MENU13;
                Serial.println("Enter threshold value for LDR sensors:");
                currentState = currentInput;
                break;
              }
            case 4:
              {
                systemState = MAIN;
                displayMainMenu();
                currentInput = -1;
                currentState = currentInput;
                break;
              }
            default:
              {
                systemState = MENU1;
                Serial.println("Invalid option! Please try again!");
                displayMenu1();
                currentInput = -1;
                currentState = currentInput;
                break;
              }
          }
          break;
        }
      case MENU11:
        {
          setSensorsSamplingInterval();
          break;
        }
      case MENU12:
        {
          setUltrasonicAlertThreshold();
          break;
        }
      case MENU13:
        {
          setLDRAlertThreshold();
          break;
        }
      case MENU2:
        {
          switch (currentInput) {
            case 1:
              {
                systemState = MENU21;
                Serial.println("Are you sure you want to delete all data? Press 1 for YES and 2 for NO.");
                currentState = currentInput;
                break;
              }
            case 2:
              {
                systemState = MENU22;
                currentState = currentInput;
                break;
              }
            default:
              {
                systemState = MENU2;
                Serial.println("Invalid option! Please try again!");
                displayMenu2();
                currentInput = -1;
                currentState = currentInput;
                break;
              }
          }
          break;
        }
      case MENU21:
        {
          resetLoggerData();
          break;
        }
      case MENU22:
        {

          Serial.println("Data not deleted.");
          systemState = MAIN;
          displayMainMenu();
          currentInput = -1;
          currentState = currentInput;
          break;
        }
      case MENU3:
        {
          switch (currentInput) {
            case 1:
              {
                systemState = MENU31;
                Serial.println("Real time sensor reading");
                Serial.println("Press 1 to start the reading");
                Serial.println("Press 0 to stop the reading");
                currentState = currentInput;
                break;
              }
            case 2:
              {
                systemState = MENU32;
                currentState = currentInput;
                break;
              }
            case 3:
              {
                systemState = MENU33;
                Serial.println("Displaying last 10 sensor readings:");
                currentState = currentInput;
                break;
              }
            case 4:
              {
                systemState = MAIN;
                displayMainMenu();
                currentInput = -1;
                currentState = currentInput;
                break;
              }
            default:
              {
                systemState = MENU3;
                Serial.println("Invalid option! Please try again!");
                displayMenu3();
                currentInput = -1;
                currentState = currentInput;
                break;
              }
          }
          break;
        }
      case MENU31:
        {
          currentSensorReadings();
          break;
        }
      case MENU32:
        {
          currentSensorSettings();
          break;
        }
      case MENU33:
        {
          displayLoggedData();
          break;
        }
      case MENU4:
        {
          switch (currentInput) {
            case 1:
              {
                systemState = MENU41;
                currentState = currentInput;
                break;
              }
            case 2:
              {
                systemState = MENU42;
                Serial.println("Enter the three colors (red, green, blue): ");
                currentState = currentInput;
                break;
              }
            case 3:
              {
                systemState = MAIN;
                displayMainMenu();
                currentInput = -1;
                currentState = currentInput;
                break;
              }
            default:
              {
                systemState = MENU4;
                Serial.println("Invalid option! Please try again!");
                displayMenu4();
                currentInput = -1;
                currentState = currentInput;
                break;
              }
          }
          break;
        }
      case MENU41:
        {
          manualColorControl();
          break;
        }
      case MENU42:
        {
          toggleLed();
          break;
        }
      default:
        {
          Serial.println("Invalid option! Please try again!");
          systemState = MAIN;
          currentInput = -1;
          currentState = currentInput;
          break;
        }
    }
  }
  checkSystem();
  digitalWrite(ledCathodeBlue, stateBlue);
  digitalWrite(ledCathodeGreen, stateGreen);
  digitalWrite(ledCathodeRed, stateRed);
}

void displayMainMenu() {
  Serial.println();
  Serial.println("Main menu:");
  Serial.println("  1. Sensor Settings");
  Serial.println("  2. Reset Logger Data");
  Serial.println("  3. System Status");
  Serial.println("  4. RGB LED Control");
}

void displayMenu1() {
  Serial.println("  1. Sensor Settings");
  Serial.println("    1.1 Sensors Sampling Interval");
  Serial.println("    1.2 Ultrasonic Alert Threshold");
  Serial.println("    1.3 LDR Alert Threshold");
  Serial.println("    1.4 Back");
}

void displayMenu2() {
  Serial.println("  2. Reset Logger Data");
  Serial.println("    2.1 Yes");
  Serial.println("    2.2 No");
}

void displayMenu3() {
  Serial.println("  3. System Status");
  Serial.println("    3.1 Current Sensor Readings");
  Serial.println("    3.2 Current Sensor Settings");
  Serial.println("    3.3 Display Logged Data");
  Serial.println("    3.4 Back");
}

void displayMenu4() {
  Serial.println("  4. RGB LED Control");
  Serial.println("    4.1 Manual Color Control");
  Serial.println("    4.2 LED: Toggle Automatic ON/OFF");
  Serial.println("    4.3 Back");
}

void setSensorsSamplingInterval() {
  if (Serial.available()) {
    if (ultrasoundSamplingRate < 0 && (millis() - lastRead > debounce)) {
      Serial.print("Setting ultrasoundSamplingRate: ");
      ultrasoundSamplingRate = Serial.parseInt();
      lastRead = millis();
      Serial.println(ultrasoundSamplingRate);
    }
    if (LDRSamplingRate < 0 && (millis() - lastRead > debounce) && ultrasoundSamplingRate > 0) {
      Serial.print("Setting LDRSamplingRate: ");
      LDRSamplingRate = Serial.parseInt();
      lastRead = millis();
      Serial.println(LDRSamplingRate);
    }
  }
  if (ultrasoundSamplingRate != -1 && LDRSamplingRate != -1) {
    if (ultrasoundSamplingRate >= 1 && ultrasoundSamplingRate <= 10 && LDRSamplingRate >= 1 && LDRSamplingRate <= 10) {
      Serial.println("Successfully set the sampling rate for the ultrasound and LDR sensors!");
      persistentLDRSamplingRate = ultrasoundSamplingRate;
      persistentUltrasoundSamplingRate = LDRSamplingRate;
      ultrasoundSamplingRate = -1;
      LDRSamplingRate = -1;
      systemState = MAIN;
      displayMainMenu();
      currentInput = -1;
      currentState = currentInput;
    } else {
      Serial.println("Invalid input. Sampling rate should be between 1 and 10 seconds.");
      Serial.println("Enter again the two values: ");
      ultrasoundSamplingRate = -1;
      LDRSamplingRate = -1;
    }
  }
}

void setUltrasonicAlertThreshold() {
  if (Serial.available()) {
    if (ultrasonicThreshold < 0 && (millis() - lastRead > debounce)) {
      Serial.print("Setting Ultrasonic sensor minimum threshold: ");
      ultrasonicThreshold = Serial.parseInt();
      lastRead = millis();
      Serial.println(ultrasonicThreshold);
    }
  }
  if (ultrasonicThreshold != -1) {
    if (ultrasonicThreshold >= 1) {
      Serial.println("Successfully set the threshold for the ultrasound sensor!");
      persistentUltrasonicThreshold = ultrasonicThreshold;
      ultrasonicThreshold = -1;
      systemState = MAIN;
      displayMainMenu();
      currentInput = -1;
      currentState = currentInput;
    }
  }
}

void setLDRAlertThreshold() {
  if (Serial.available()) {
    if (LDRThreshold < 0 && (millis() - lastRead > debounce)) {
      Serial.print("Setting LDR sensor minimum threshold: ");
      LDRThreshold = Serial.parseInt();
      lastRead = millis();
      Serial.println(LDRThreshold);
    }
  }
  if (LDRThreshold != -1) {
    if (LDRThreshold >= 1) {
      Serial.println("Successfully set the threshold for the LDR sensor!");
      persistentLDRThreshold = LDRThreshold;
      LDRThreshold = -1;
      systemState = MAIN;
      displayMainMenu();
      currentInput = -1;
      currentState = currentInput;
    }
  }
}

void currentSensorReadings() {
  if (persistentUltrasoundSamplingRate < 0 || persistentLDRSamplingRate < 0) {
    Serial.println("Set the sampling rates first!");
    systemState = MAIN;
    displayMainMenu();
    currentInput = -1;
    currentState = currentInput;
  }
  if (Serial.available()) {
    if (stop < 0 && (millis() - lastRead > debounce)) {
      stop = Serial.parseInt();
      lastRead = millis();
    }
  }
  if (stop == 0) {
    Serial.println("Printing stopped");
    for (indexUS = 0; indexUS < numberDataSaved; indexUS++) {
      EEPROM.put(adressStartEEPROM + indexUS * sizeInt, savedDataUltrasonicSensor[indexUS]);
    }
    for (indexLDR = 0; indexLDR < numberDataSaved; indexLDR++) {
      EEPROM.put(adressStartEEPROM + indexUS * sizeInt + indexLDR * sizeInt, savedDataLDRSensor[indexLDR]);
    }
    stop = -1;
    systemState = MAIN;
    displayMainMenu();
    currentInput = -1;
    currentState = currentInput;
  } else if (stop == 1) {
    currentMicros = millis();
    if (currentMicros - previousPrintMicros >= persistentUltrasoundSamplingRate * 1000) {
      if (!isHigh && currentMicros - previousMicros >= 2) {
        digitalWrite(trigPin, HIGH);
        previousMicros = currentMicros;
        isHigh = true;
      } else if (isHigh && currentMicros - previousMicros >= 10) {
        digitalWrite(trigPin, LOW);

        long duration = pulseIn(echoPin, HIGH);
        int distance = duration * 0.034 / 2;
        previousMicros = currentMicros;
        isHigh = false;
        Serial.print("Distance: ");
        Serial.println(distance);
        savedDataUltrasonicSensor[indexUS++] = distance;
        if (indexUS >= numberDataSaved) {
          indexUS = 0;
        }
        previousPrintMicros = currentMicros;
      }
    }
    currentMicros2 = millis();
    if (currentMicros2 - previousPrintMicros2 >= persistentLDRSamplingRate * 1000) {
      photocellValue = analogRead(photocellPin);
      Serial.print("Light: ");
      Serial.println(photocellValue);
      savedDataLDRSensor[indexLDR++] = photocellValue;
      if (indexLDR >= numberDataSaved) {
        indexLDR = 0;
      }
      previousPrintMicros2 = currentMicros2;
    }
    if (Serial.available()) {
      stop = Serial.parseInt();
      lastRead = millis();
    }
  }
}

void currentSensorSettings() {
  Serial.println("Ultrasonic sensor:");
  Serial.print("Sampling rate: ");
  Serial.println(persistentUltrasoundSamplingRate);
  Serial.print("Min threshold: ");
  Serial.println(persistentUltrasonicThreshold);
  Serial.println("LDR sensor:");
  Serial.print("Sampling rate: ");
  Serial.println(persistentLDRSamplingRate);
  Serial.print("Min threshold: ");
  Serial.println(persistentLDRThreshold);
  systemState = MAIN;
  displayMainMenu();
  currentInput = -1;
  currentState = currentInput;
}

void displayLoggedData() {
  Serial.println("Ultrasonic sensor:");
  for (indexUS = 0; indexUS < numberDataSaved; indexUS++) {
    EEPROM.get(adressStartEEPROM + indexUS * sizeInt, toPrintDataUltrasonicSensor[indexUS]);
    Serial.print(indexUS + 1);
    Serial.print(". ");
    Serial.println(toPrintDataUltrasonicSensor[indexUS]);
  }
  Serial.println("LDR sensor:");
  for (indexLDR = 0; indexLDR < numberDataSaved; indexLDR++) {
    EEPROM.get(adressStartEEPROM + indexUS * sizeInt + indexLDR * sizeInt, toPrintDataLDRSensor[indexLDR]);
    Serial.print(indexLDR + 1);
    Serial.print(". ");
    Serial.println(toPrintDataLDRSensor[indexLDR]);
  }
  systemState = MAIN;
  displayMainMenu();
  currentInput = -1;
  currentState = currentInput;
}

void resetLoggerData() {
  if (Serial.available()) {
    if (stop < 0 && (millis() - lastRead > debounce)) {

      stop = Serial.parseInt();
      lastRead = millis();
      if (stop == 1) {

        for (indexUS = 0; indexUS < numberDataSaved; indexUS++) {
          EEPROM.put(adressStartEEPROM + indexUS * sizeInt, 0);
          savedDataUltrasonicSensor[indexUS] = 0;
          toPrintDataUltrasonicSensor[indexUS] = 0;
        }
        for (indexLDR = 0; indexLDR < numberDataSaved; indexLDR++) {
          EEPROM.put(adressStartEEPROM + indexUS * sizeInt + indexLDR * sizeInt, 0);
          savedDataLDRSensor[indexLDR] = 0;
          toPrintDataLDRSensor[indexLDR] = 0;
        }
        persistentUltrasoundSamplingRate = -1;
        persistentUltrasonicThreshold = -1;
        persistentLDRSamplingRate = -1;
        persistentLDRThreshold = -1;
        Serial.println("All data deleted");
        stop = -1;
        systemState = MAIN;
        displayMainMenu();
        currentInput = -1;
        currentState = currentInput;
      } else {
        systemState = MENU22;
        stop = -1;
      }
    }
  }
}

void toggleLed() {
  if (ledAuto == false) {
    ledAuto = true;
    Serial.println("LED Mode: AUTO");
  } else {
    ledAuto = false;
    Serial.println("LED Mode: MANUAL");
  }
  systemState = MAIN;
  displayMainMenu();
  currentInput = -1;
  currentState = currentInput;
}

void manualColorControl() {
  if (!ledAuto) {
    if (Serial.available()) {
      if (red == -1) {
        red = Serial.parseInt();
        lastRead = millis();
        Serial.print("Red: ");
        Serial.println(red);
      }

      if (green == -1 && red != -1 && (millis() - lastRead > debounce)) {
        green = Serial.parseInt();
        lastRead = millis();
        Serial.print("Green: ");
        Serial.println(green);
      }

      if (blue == -1 && red != -1 && green != -1 && (millis() - lastRead > debounce)) {
        blue = Serial.parseInt();
        lastRead = millis();
        Serial.print("Blue: ");
        Serial.println(blue);
      }


      if (red != -1 && green != -1 && blue != -1) {
        if (red >= 0 && green >= 0 && blue >= 0) {
          stateRed = (red > 0) ? HIGH : LOW;
          stateGreen = (green > 0) ? HIGH : LOW;
          stateBlue = (blue > 0) ? HIGH : LOW;

          red = -1;
          green = -1;
          blue = -1;
          systemState = MAIN;
          displayMainMenu();
          currentInput = -1;
          currentState = currentInput;
        } else {
          Serial.println("Invalid input. Numbers should be non-negative values.");
          Serial.println("Enter the three values again: ");
          red = -1;
          green = -1;
          blue = -1;
        }
      }
    }
    digitalWrite(ledCathodeBlue, stateBlue);
    digitalWrite(ledCathodeGreen, stateGreen);
    digitalWrite(ledCathodeRed, stateRed);

  } else {
    Serial.println("Set the LED to MANUAL mode first!");
    systemState = MAIN;
    displayMainMenu();
    currentInput = -1;
    currentState = currentInput;
  }
}

void checkSystem() {
  if (ledAuto) {
    currentMicros = millis();
    if (currentMicros - previousPrintMicros >= persistentUltrasoundSamplingRate * 1000) {
      if (!isHigh && currentMicros - previousMicros >= 2) {
        digitalWrite(trigPin, HIGH);
        previousMicros = currentMicros;
        isHigh = true;
      } else if (isHigh && currentMicros - previousMicros >= 10) {
        digitalWrite(trigPin, LOW);
        long duration = pulseIn(echoPin, HIGH);
        distance = duration * 0.034 / 2;
        previousMicros = currentMicros;
        isHigh = false;
        previousPrintMicros = currentMicros;
      }
    }
    photocellValue = analogRead(photocellPin);

    if (distance < persistentUltrasonicThreshold || (photocellValue > persistentLDRThreshold && persistentLDRThreshold > 0)) {
      stateRed = HIGH;
      stateGreen = LOW;
      stateBlue = LOW;
    } else {
      stateRed = LOW;
      stateGreen = HIGH;
      stateBlue = LOW;
    }
  }
}
