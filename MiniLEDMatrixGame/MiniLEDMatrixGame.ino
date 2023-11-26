#include "LedControl.h"
#include <ezButton.h>

#define DIN 7
#define CS 6
#define CLK 5

LedControl lc = LedControl(DIN, CLK, CS, 1);
const int n = 8;
const int xPin = A0;
const int yPin = A1;
const int bPin = 2;
const int minThreshold = 400;
const int maxThreshold = 600;
const int debounceDelay = 100;
int blinkInterval = 0;
const int blinkIntervalPlayer = 300;
const int blinkIntervalBomb = 100;
int currentPositionRowPlayer = 0;
int currentPositionColPlayer = 0;
int bombPositionRow = 0;
int bombPositionCol = 0;
int xValue = 0;
int yValue = 0;
unsigned long lastMoveTime = 0;
unsigned long lastBlinkTime = 0;
unsigned long lastBlinkTimeBomb = 0;
bool blinkState = false;
bool blinkStateBomb = false;
const int bombTimer = 2000;
bool flashState = false;
bool bombActive = false;
unsigned long flashStartTime = 0;
unsigned long bombStartTime = 0;
const int flashInterval = 2000;
enum JoystickDirection {
  LEFT,
  RIGHT,
  UP,
  DOWN,
  CENTER
};
unsigned long pressedTime = 0;
unsigned long releasedTime = 0;
ezButton button(bPin);
const int SHORT_PRESS_TIME = 300;
const int LONG_PRESS_TIME = 1000;
const float wallProb = 0.5;
const byte matrixSize = 8;
byte matrixBrightness = 10;
bool fastBlink = false;
byte matrix[matrixSize][matrixSize] = {
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 }
};

byte endGame[matrixSize][matrixSize] = {
  { 1, 0, 0, 0, 0, 0, 0, 1 },
  { 0, 1, 0, 0, 0, 0, 1, 0 },
  { 0, 0, 1, 0, 0, 1, 0, 0 },
  { 0, 0, 0, 1, 1, 0, 0, 0 },
  { 0, 0, 0, 1, 1, 0, 0, 0 },
  { 0, 0, 1, 0, 0, 1, 0, 0 },
  { 0, 1, 0, 0, 0, 0, 1, 0 },
  { 1, 0, 0, 0, 0, 0, 0, 1 }
};
byte happyFace[matrixSize][matrixSize] = {
  { 0, 0, 1, 1, 1, 1, 0, 0 },
  { 0, 1, 0, 0, 0, 0, 1, 0 },
  { 1, 0, 1, 0, 0, 1, 0, 1 },
  { 1, 0, 0, 0, 0, 0, 0, 1 },
  { 1, 0, 1, 0, 0, 1, 0, 1 },
  { 1, 0, 0, 1, 1, 0, 0, 1 },
  { 0, 1, 0, 0, 0, 0, 1, 0 },
  { 0, 0, 1, 1, 1, 1, 0, 0 }
};
bool lost = false;
bool won = false;
int bricks = 0;

void generateMap() {
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      float r = random(0, 100) / 100.0;
      if (r < wallProb && i != currentPositionRowPlayer && j != currentPositionColPlayer) {
        matrix[i][j] = 1;
        bricks++;
      } else {
        matrix[i][j] = 0;
      }
    }
  }
}

JoystickDirection getJoystickDirection(int xValue, int yValue) {
  if (xValue < minThreshold) {
    return LEFT;
  } else if (xValue > maxThreshold) {
    return RIGHT;
  } else {
    if (yValue < minThreshold) {
      return UP;
    } else if (yValue > maxThreshold) {
      return DOWN;
    } else {
      return CENTER;
    }
  }
}

void moveDot(JoystickDirection direction) {
  switch (direction) {
    case LEFT:
      if (currentPositionRowPlayer != 0 && (matrix[(currentPositionRowPlayer - 1) % n][currentPositionColPlayer] == 0)) {
        currentPositionRowPlayer = (currentPositionRowPlayer - 1) % n;
      }
      break;
    case RIGHT:
      if (currentPositionRowPlayer != n - 1 && (matrix[(currentPositionRowPlayer + 1) % n][currentPositionColPlayer] == 0)) {
        currentPositionRowPlayer = (currentPositionRowPlayer + 1) % n;
      }
      break;
    case UP:
      if (currentPositionColPlayer != 0 && (matrix[currentPositionRowPlayer][(currentPositionColPlayer - 1) % n] == 0)) {
        currentPositionColPlayer = (currentPositionColPlayer - 1) % n;
      }
      break;
    case DOWN:
      if (currentPositionColPlayer != n - 1 && (matrix[currentPositionRowPlayer][(currentPositionColPlayer + 1) % n] == 0)) {
        currentPositionColPlayer = (currentPositionColPlayer + 1) % n;
      }
      break;
    case CENTER:
      break;
  }
}

void blinkDot(bool state, int row, int col) {
  lc.setLed(0, row, col, state);
}

void updateDisplay() {
  if (!lost && !won) {
    for (int row = 0; row < matrixSize; row++) {
      for (int col = 0; col < matrixSize; col++) {
        lc.setLed(0, row, col, matrix[row][col]);
      }
    }
    lc.clearDisplay(0);
    blinkDot(blinkState, currentPositionRowPlayer, currentPositionColPlayer);
    blinkDot(blinkStateBomb, bombPositionRow, bombPositionCol);
    if (bombTimerUp()) {
      flashBomb();
      clearWalls();
      bombActive = false;
      fastBlink = false;
      blinkInterval = blinkIntervalPlayer;
      blinkStateBomb = false;
      if (currentPositionRowPlayer == bombPositionRow || currentPositionColPlayer == bombPositionCol) {
        lost = true;
        lc.clearDisplay(0);
        for (int row = 0; row < matrixSize; row++) {
          for (int col = 0; col < matrixSize; col++) {
            lc.setLed(0, row, col, endGame[row][col]);
          }
        }
      }
    }
  }
  if (won) {
    for (int row = 0; row < matrixSize; row++) {
      for (int col = 0; col < matrixSize; col++) {
        lc.setLed(0, row, col, happyFace[row][col]);
      }
    }
  }
}

void initMatrix() {
  lc.shutdown(0, false);
  lc.setIntensity(0, matrixBrightness);
  lc.clearDisplay(0);
}

void setup() {
  Serial.begin(9600);
  currentPositionColPlayer = n / 2;
  currentPositionRowPlayer = n / 2;
  pinMode(bPin, INPUT_PULLUP);
  button.setDebounceTime(100);
  initMatrix();
  generateMap();
}

void loop() {
  button.loop();
  checkButton();
  xValue = analogRead(xPin);
  yValue = analogRead(yPin);
  if (millis() - lastMoveTime >= debounceDelay) {
    JoystickDirection direction = getJoystickDirection(xValue, yValue);
    moveDot(direction);
    lastMoveTime = millis();
  }
  if (millis() - lastBlinkTime >= blinkIntervalPlayer) {
    blinkState = !blinkState;
    lastBlinkTime = millis();
  }
  if (fastBlink) {
    if (millis() - lastBlinkTimeBomb >= blinkIntervalBomb) {
      blinkStateBomb = !blinkStateBomb;
      lastBlinkTimeBomb = millis();
    }
  }
  updateDisplay();
}

void checkButton() {
  if (button.isPressed())
    pressedTime = millis();

  if (button.isReleased() && !fastBlink) {
    releasedTime = millis();
    long pressDuration = releasedTime - pressedTime;
    if (pressDuration < SHORT_PRESS_TIME) {
      Serial.println("A short press is detected");
      fastBlink = !fastBlink;
      if (fastBlink) {
        bombPositionRow = currentPositionRowPlayer;
        bombPositionCol = currentPositionColPlayer;
        bombStartTime = millis();
        bombActive = true;
      } else {
        bombPositionRow = 0;
        bombPositionCol = 0;
        bombActive = false;
      }
    }
    if (pressDuration > LONG_PRESS_TIME) {
      Serial.println("A long press is detected");
      if (lost || won) {
        lost = false;
        won = false;
        bombPositionRow = 0;
        bombPositionCol = 0;
        currentPositionColPlayer = n / 2;
        currentPositionRowPlayer = n / 2;
        bricks = 0;
        initMatrix();
        generateMap();
      }
    }
  }
}

void blinkBomb(bool state) {
  lc.setLed(0, bombPositionRow, bombPositionCol, state);
}

bool flashIntervalUp() {
  if (millis() > flashStartTime + flashInterval) {
    return true;
  } else {
    return false;
  }
}

bool bombTimerUp() {
  if (bombActive && millis() > bombStartTime + bombTimer) {
    return true;
  } else {
    return false;
  }
}

void flashBomb() {
  flashStartTime = millis();
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      if (i == bombPositionRow || j == bombPositionCol) {
        lc.setLed(0, i, j, true);
      }
    }
  }
  flashState = true;
}


void clearWalls() {
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      if (i == bombPositionRow || j == bombPositionCol) {
        if (matrix[i][j])
          bricks--;
        matrix[i][j] = 0;
      }
    }
  }
  if (bricks == 0) {
    won = true;
  }
  lc.clearDisplay(0);
}
