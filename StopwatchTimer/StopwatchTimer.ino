#include <ezButton.h>

const int clockPin = 10;
const int latchPin = 11; 
const int dataPin = 12;

const int segD1 = 4;
const int segD2 = 5;
const int segD3 = 6;
const int segD4 = 7;

int displayDigits[] = { segD1, segD2, segD3, segD4 };
const int displayCount = 4; 

const int encodingsNumber = 10;
const int buttonPinStart = A0;
ezButton buttonStart(A0);
byte buttonStartState = 0;
const int buttonPinReset = A1;
ezButton buttonReset(A1);
byte buttonResetState = 0;
const int buttonPinSaveLap = A2;
ezButton buttonSaveLap(A2);
byte buttonSaveLapState = 0;
const int ledPinGreen = 13;
const int ledPinYellow = 9;
const int ledPinRed = 3;

unsigned long lastIncrement = 0;                               
unsigned long numberDigits[4] = { B11111100, B11111101, B11111100, B11111100 }; 
unsigned long number = 0;
const int debounceTime = 50;
enum generalTimerState { INITIAL,
                         COUNTING,
                         PAUSE,
                         STOP,
                         LAP };
generalTimerState timerState = INITIAL;
unsigned long startTime = 0;
int interval = 100;
byte byteEncodings[encodingsNumber] = {
  //A B C D E F G DP
  B11111100,  // 0
  B01100000,  // 1
  B11011010,  // 2
  B11110010,  // 3
  B01100110,  // 4
  B10110110,  // 5
  B10111110,  // 6
  B11100000,  // 7
  B11111110,  // 8
  B11110110,  // 9
};

byte byteEncodingsDP[encodingsNumber] = {
  //A B C D E F G DP
  B11111101,  // 0
  B01100001,  // 1
  B11011011,  // 2
  B11110011,  // 3
  B01100111,  // 4
  B10110111,  // 5
  B10111111,  // 6
  B11100001,  // 7
  B11111111,  // 8
  B11110111,  // 9
};

byte laps[4];
int lapNumber = 0;
int lapNumberTotal = 4;
int indexLap = 0;
unsigned long previousMillis = 0;
const int displayInterval = 5; 
int displayDigit = 3;
int index = 0;
bool delayInProgressInitial = false;
unsigned long delayStartMillis;

void setup() {
  // Initialize the pins connected to the shift register as outputs
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  // Initialize digit control pins and set them to LOW (off)
  for (int i = 0; i < displayCount; i++) {
    pinMode(displayDigits[i], OUTPUT);
    digitalWrite(displayDigits[i], LOW);
  }
  pinMode(buttonPinStart, INPUT_PULLUP);

  pinMode(buttonPinReset, INPUT_PULLUP);
  pinMode(buttonPinSaveLap, INPUT_PULLUP);
  pinMode(ledPinGreen, OUTPUT);
  pinMode(ledPinYellow, OUTPUT);
  pinMode(ledPinRed, OUTPUT);
  buttonStart.setDebounceTime(debounceTime);
  buttonReset.setDebounceTime(debounceTime);
  buttonSaveLap.setDebounceTime(debounceTime);
  Serial.begin(9600);
}

void loop() {
  handleStates();
  handleLeds();
  readButtons();
}

void writeReg(int digit) {
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, digit);
  digitalWrite(latchPin, HIGH);
}

void activateDisplay(int displayNumber) {
  for (int i = 0; i < displayCount; i++) {
    digitalWrite(displayDigits[i], HIGH);
  }
  digitalWrite(displayDigits[displayNumber], LOW);
}

void writeNumber(int number) {
  int currentNumber = number;
  int displayDigitWrite = 3;
  int lastDigit = 0;
  while (displayDigitWrite >= 0) {
    lastDigit = currentNumber % 10;
    activateDisplay(displayDigitWrite);
    if (displayDigitWrite == 2) {
      writeReg(byteEncodingsDP[lastDigit]);
    } else {
      writeReg(byteEncodings[lastDigit]);
    }
    displayDigitWrite--;
    currentNumber /= 10;
    writeReg(B00000000);
  }
}

void readButtons() {
  readButtonStart();
  readButtonReset();
  readButtonSaveLap();
}

void readButtonStart() {
  buttonStart.loop();
  if (buttonStart.isReleased() && (timerState == INITIAL || timerState == PAUSE || timerState == STOP)) {
    if (timerState == INITIAL || timerState == STOP) {
      number = 0;
    }
    timerState = COUNTING;
    startTime = millis();
    handleStates();
  }
  if (buttonStart.isReleased() && timerState == COUNTING) {
    timerState = PAUSE;
    handleStates();
  }
}

void readButtonReset() {
  buttonReset.loop();
  if (buttonReset.isReleased() && (timerState == STOP || timerState == LAP)) {
    timerState = INITIAL;
    for (int i = 0; i < lapNumberTotal; i++) {
      laps[i] = 0;
    }
    handleStates();
  }
  if (buttonReset.isReleased() && timerState != COUNTING && timerState != STOP) {
    timerState = STOP;
    handleStates();
  }
}

void readButtonSaveLap() {
  buttonSaveLap.loop();
  if (buttonSaveLap.isReleased() && timerState == COUNTING) {
    laps[lapNumber++] = number;
    if (lapNumber == 4) {
      lapNumber = 0;
    }
  }
  if (buttonSaveLap.isPressed() && (timerState == STOP || timerState == LAP)) {
    timerState = LAP;
    number = laps[indexLap++];
    Serial.print(indexLap - 1);
    Serial.print("  ");
    Serial.println(number);
    if (indexLap == 4) {
      indexLap = 0;
    }
    handleStates();
  }
}

void handleStates() {
  switch (timerState) {
    case COUNTING:
      {
        if (millis() - lastIncrement > interval) {
          number++;
          number %= 10000;
          lastIncrement = millis();
        }
        writeNumber(number);
        break;
      }
    case PAUSE:
      {
        writeNumber(number);
        break;
      }
    case STOP:
      {
        initialState();
        break;
      }
    case LAP:
      {
        writeNumber(number);
        break;
      }
    case INITIAL:
      {
        initialState();
        break;
      }
  }
  readButtons();
}

void initialState() {
  unsigned long currentMillis = millis();
  if (!delayInProgressInitial) {
    if (index < displayCount) {
      writeReg(B00000000);
      activateDisplay(displayDigit);
      writeReg(numberDigits[index]);
      delayInProgressInitial = true;
      delayStartMillis = currentMillis;
    } else {
      index = 0;
      displayDigit = 3;
    }
  } else {
    if (currentMillis - delayStartMillis >= displayInterval) {
      delayInProgressInitial = false;
      index++;
      displayDigit--;
    }
  }
}

void handleLeds() {
  if (timerState == INITIAL) {
    digitalWrite(ledPinGreen, HIGH);
    digitalWrite(ledPinYellow, LOW);
    digitalWrite(ledPinRed, LOW);
  }
  if (timerState == COUNTING) {
    digitalWrite(ledPinGreen, HIGH);
    digitalWrite(ledPinYellow, LOW);
    digitalWrite(ledPinRed, HIGH);
  }
  if (timerState == PAUSE) {
    digitalWrite(ledPinGreen, HIGH);
    digitalWrite(ledPinYellow, HIGH);
    digitalWrite(ledPinRed, LOW);
  }
  if (timerState == STOP) {
    digitalWrite(ledPinGreen, LOW);
    digitalWrite(ledPinYellow, HIGH);
    digitalWrite(ledPinRed, HIGH);
  }
  if (timerState == LAP) {
    digitalWrite(ledPinGreen, LOW);
    digitalWrite(ledPinYellow, HIGH);
    digitalWrite(ledPinRed, HIGH);
  }
}
