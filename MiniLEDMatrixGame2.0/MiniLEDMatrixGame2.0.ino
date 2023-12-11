#include "LedControl.h"
#include <ezButton.h>
#include <LiquidCrystal.h>
#include <EEPROM.h>

const byte rs = 9;
const byte en = 8;
const byte d4 = 7;
const byte d5 = 6;
const byte d6 = 5;
const byte d7 = 4;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
#define DIN 12
#define CS 11
#define CLK 10

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
unsigned long lastMoveTime2 = 0;
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
int previousMillis = 0;
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
int interval = 100;
int row = 0;
int letter = 0;

enum States {
  INTRO,
  DISPLAY_MENU1,
  SETTINGS,
  SET1,
  SET2,
  ABOUT,
  ENDGAME_WIN,
  ENDGAME_LOSS,
  GAMEPLAY
};
States systemState = INTRO;
const long scrollInterval = 300;
const char* staticMessage = "Enjoy the Game!";
int scrollPosition = 0;
unsigned long previousMillisLCD = 0;
const char introMessage[] = "Press the joystick to continue!";

const uint8_t textStart[][8] = {
  { 0b00000000,
    0b00011000,
    0b00100100,
    0b00001100,
    0b00110000,
    0b00100100,
    0b00011000,
    0b00000000 },
  { 0b00000000,
    0b01111100,
    0b00010000,
    0b00010000,
    0b00010000,
    0b00010000,
    0b00010000,
    0b00000000 },
  { 0b00000000,
    0b00111100,
    0b00100100,
    0b00100100,
    0b00111100,
    0b00100100,
    0b00100100,
    0b00000000 },
  { 0b00000000,
    0b00111100,
    0b00100100,
    0b00111100,
    0b00001100,
    0b00010100,
    0b00100100,
    0b00000000 },
  { 0b00000000,
    0b01111100,
    0b00010000,
    0b00010000,
    0b00010000,
    0b00010000,
    0b00010000,
    0b00000000 }
};
byte doubleSquare[n] = {
  0b11111111,
  0b11000011,
  0b10111101,
  0b10100101,
  0b10100101,
  0b10111101,
  0b11000011,
  0b11111111
};
const int EEPROM_CONTRAST_ADDR = 10;
const int EEPROM_BRIGHTNESS_ADDR = 20;
const int contrastPin = 3;
int contrastValue;
int brightnessValue;
int stepBright = 1;
float percentContrast = 0.1;
const int textStartN = 5;
const char* settings = "Settings";
const char* brightnessText = "LCD brightness: ";
const char* brightnessLeds = "LED brightness: ";
const char* about = "About";
const char* start = "Start";
const char* aboutText = "Bomberman - GitHub: AndriciucAndreeaCristina ";
unsigned long previousMillisSettings = 0;
unsigned long previousMillisS1 = 0;
JoystickDirection direction = CENTER;


static int line = -1;
static int lineSettings = -1;

void blinkDot(bool state, int row, int col) {
  lc.setLed(0, row, col, state);
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

void updateDisplay() {
  switch (systemState) {
    case INTRO:
      {
        unsigned long currentMillis = millis();
        if (currentMillis - previousMillis >= interval) {
          previousMillis = currentMillis;
          if (letter < textStartN) {
            lc.setRow(0, row, textStart[letter][row]);
            row++;
            if (row >= n) {
              row = 0;
              letter++;
              lc.clearDisplay(0);
            }
          } else {
            letter = 0;
            row = 0;
          }
        }
        break;
      }
    case DISPLAY_MENU1:
    case SETTINGS:
      {
        lc.clearDisplay(0);
        for (int i = 0; i < n; i++) {
          lc.setRow(0, i, doubleSquare[i]);
        }
        break;
      }
    case SET1:
      {
        break;
      }
    case SET2:
      {
        break;
      }
    case ABOUT:
      {
        break;
      }
    case ENDGAME_WIN:
      {
        for (int row = 0; row < matrixSize; row++) {
          for (int col = 0; col < matrixSize; col++) {
            lc.setLed(0, row, col, happyFace[row][col]);
          }
        }
        break;
      }
    case ENDGAME_LOSS:
      {
        lost = true;
        lc.clearDisplay(0);
        for (int row = 0; row < matrixSize; row++) {
          for (int col = 0; col < matrixSize; col++) {
            lc.setLed(0, row, col, endGame[row][col]);
          }
        }
        break;
      }
    case GAMEPLAY:
      {
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
            systemState = ENDGAME_LOSS;
          }
        }
        if (won) {
          systemState = ENDGAME_WIN;
        }
        break;
      }
  }
}
int squareCount = 10;
int currentSquare = 5;
int currentSquareB = 5;
int startPosition = 0;
void handleDisplay() {
  switch (systemState) {
    case INTRO:
      {
        unsigned long currentMillis = millis();
        if (currentMillis - previousMillisLCD >= scrollInterval) {
          previousMillisLCD = currentMillis;
          lcd.setCursor(0, 1);
          lcd.print(introMessage + scrollPosition);
          scrollPosition++;
          if (scrollPosition >= strlen(introMessage)) {
            scrollPosition = 0;
          }
        }
        break;
      }
    case SETTINGS:
      {

        break;
      }
    case SET1:
      {
        unsigned long currentMillis = millis();
        if (currentMillis - previousMillisS1 >= interval) {
          previousMillisS1 = currentMillis;
          lcd.clear();
          lcd.setCursor(3, 0);
          for (int i = 0; i < currentSquare; i++) {
            lcd.print((char)255);
          }
          for (int i = currentSquare; i < squareCount; i++) {
            lcd.print((char)219);
          }
        }
        break;
      }
    case SET2:
      {
        unsigned long currentMillis = millis();
        if (currentMillis - previousMillisS1 >= interval) {
          previousMillisS1 = currentMillis;
          lcd.clear();
          lcd.setCursor(3, 0);
          for (int i = 0; i < currentSquareB; i++) {
            lcd.print((char)255);
          }
          for (int i = currentSquareB; i < squareCount; i++) {
            lcd.print((char)219);
          }
        }
        break;
      }
    case ABOUT:
      {
        unsigned long currentMillis = millis();
        static unsigned long previousMillisABT = 0;
        const long interval = 300;
        if (currentMillis - previousMillisABT >= interval) {
          previousMillisABT = currentMillis;
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print(aboutText + startPosition);
          startPosition++;
          if (startPosition > strlen(aboutText) - 16) {
            startPosition = 0;
          }
        }
        break;
      }
    case ENDGAME_WIN:
      {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("You won!");
        unsigned long currentMillis = millis();
        if (currentMillis - previousMillisLCD >= scrollInterval) {
          previousMillisLCD = currentMillis;
          lcd.setCursor(0, 1);
          lcd.print(introMessage + scrollPosition);
          scrollPosition++;
          if (scrollPosition >= strlen(introMessage)) {
            scrollPosition = 0;
          }
        }
        break;
      }
    case ENDGAME_LOSS:
      {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("You lost!");
        unsigned long currentMillis = millis();
        
        if (currentMillis - previousMillisLCD >= scrollInterval) {
          previousMillisLCD = currentMillis;
          lcd.setCursor(0, 1);
          lcd.print(introMessage + scrollPosition);
          scrollPosition++;
          if (scrollPosition >= strlen(introMessage)) {
            scrollPosition = 0;
          }
        }
        break;
      }
    case GAMEPLAY:
      {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Walls left: ");
        lcd.print(" ");
        lcd.print(bricks);
        scrollPosition = 0;
        break;
      }
  }
}

void checkButton() {
  if (button.isPressed())
    pressedTime = millis();

  if (button.isReleased() && !fastBlink) {
    releasedTime = millis();
    long pressDuration = releasedTime - pressedTime;
    if (pressDuration < SHORT_PRESS_TIME) {
      Serial.println("Short press");
      switch (systemState) {
        case INTRO:
          {
            systemState = DISPLAY_MENU1;
            break;
          }
        case DISPLAY_MENU1:
          {
            if (line == 0) {
              systemState = SETTINGS;
            } else if (line == 1) {
              systemState = ABOUT;
            } else if (line == 2) {
              systemState = GAMEPLAY;
            }
            break;
          }
        case SETTINGS:
          {
            if (lineSettings == 0) {
              systemState = SET1;
            } else if (lineSettings == 1) {
              systemState = SET2;
            } else if (lineSettings == 2) {
              line = -1;
              lineSettings = -1;
              systemState = DISPLAY_MENU1;
            }
            break;
          }
        case SET1:
          {
            line = -1;
            lineSettings = -1;
            analogWrite(contrastPin, contrastValue);
            EEPROM.put(EEPROM_CONTRAST_ADDR, contrastValue);
            systemState = DISPLAY_MENU1;
            break;
          }
        case SET2:
          {
            line = -1;
            lineSettings = -1;
            lc.setIntensity(0, brightnessValue);
            EEPROM.put(EEPROM_BRIGHTNESS_ADDR, brightnessValue);
            systemState = DISPLAY_MENU1;
            break;
          }
        case ABOUT:
          {
            line = -1;
            lineSettings = -1;
            systemState = DISPLAY_MENU1;
            break;
          }
        case ENDGAME_WIN:
          {
            break;
          }
        case ENDGAME_LOSS:
          {
            break;
          }
        case GAMEPLAY:
          {
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
            break;
          }
      }
    }
    if (pressDuration > LONG_PRESS_TIME) {
      switch (systemState) {
        case GAMEPLAY:
          {
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
            break;
          }
        case ENDGAME_WIN:
          {
            line = -1;
            lineSettings = -1;
            systemState = DISPLAY_MENU1;
            break;
          }
        case ENDGAME_LOSS:
          {
            line = -1;
            lineSettings = -1;
            systemState = DISPLAY_MENU1;
            break;
          }
        default:
          break;
      }
    }
  }
}

void initMatrix() {
  lc.shutdown(0, false);
  lc.setIntensity(0, brightnessValue);
  lc.clearDisplay(0);
}

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


void setup() {
  Serial.begin(9600);
  currentPositionColPlayer = n / 2;
  currentPositionRowPlayer = n / 2;
  pinMode(bPin, INPUT_PULLUP);
  button.setDebounceTime(100);
  pinMode(contrastPin, OUTPUT);
  EEPROM.get(EEPROM_CONTRAST_ADDR, contrastValue);
  EEPROM.get(EEPROM_BRIGHTNESS_ADDR, brightnessValue);
  initMatrix();
  generateMap();
  lcd.clear();
  lcd.noCursor();
  lcd.noAutoscroll();
  lcd.begin(16, 2);
  analogWrite(contrastPin, contrastValue);
  lcd.print("Enjoy the game!");
}

void loop() {
  button.loop();
  checkButton();
  handleDisplay();
  xValue = analogRead(xPin);
  yValue = analogRead(yPin);
  if (millis() - lastMoveTime >= debounceDelay) {
    direction = getJoystickDirection(xValue, yValue);
    if (systemState == GAMEPLAY) {
      moveDot(direction);
    } else if (systemState == DISPLAY_MENU1) {
      if (line == -1) {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("->");
        lcd.print(settings);
        lcd.setCursor(2, 1);
        lcd.print(about);
        line = 0;
      }
      displayMenu(direction);
    } else if (systemState == SETTINGS) {
      if (lineSettings == -1) {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("->");
        lcd.print(brightnessText);
        lcd.setCursor(2, 1);
        lcd.print(brightnessLeds);
        lineSettings = 0;
      }
      displaySettings(direction);
    } else if (systemState == SET1) {
      if (direction == LEFT) {
        contrastValue -= percentContrast * contrastValue;
        if (currentSquare > 0)
          currentSquare--;
      } else if (direction == RIGHT) {
        contrastValue += percentContrast * contrastValue;
        if (currentSquare < 10)
          currentSquare++;
      }
      analogWrite(contrastPin, contrastValue);
      EEPROM.put(EEPROM_CONTRAST_ADDR, contrastValue);
    } else if (systemState == SET2) {
      if (direction == LEFT) {
        brightnessValue -= stepBright;
        if (currentSquareB > 0)
          currentSquareB--;
      } else if (direction == RIGHT) {
        brightnessValue += stepBright;
        if (currentSquareB < 10)
          currentSquareB++;
      }
      lc.setIntensity(0, brightnessValue);
    }
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
void displayMenu(JoystickDirection direction) {
  //lcd.clear();
  switch (direction) {
    case UP:
      {
        Serial.println("UP");
        if (line == 1) {
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("->");
          lcd.print(settings);
          lcd.setCursor(2, 1);
          lcd.print(about);
          line = 0;
        } else if (line == 2) {
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("->");
          lcd.print(about);
          lcd.setCursor(2, 1);
          lcd.print(start);
          line = 1;
        }
        break;
      }
    case DOWN:
      {
        Serial.println("DOWN");
        if (line == 0) {
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("  ");
          lcd.print(settings);
          lcd.setCursor(0, 1);
          lcd.print("->");
          lcd.print(about);
          line = 1;
        } else if (line == 1) {
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("  ");
          lcd.print(about);
          lcd.setCursor(0, 1);
          lcd.print("->");
          lcd.print(start);
          line = 2;
        }
        break;
      }
    default:
      {
        break;
      }
  }
}

void displaySettings(JoystickDirection direction) {
  switch (direction) {
    case UP:
      {
        Serial.println("UP");
        if (lineSettings == 1) {
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("->");
          lcd.print(brightnessText);
          lcd.setCursor(2, 1);
          lcd.print(brightnessLeds);
          lineSettings = 0;
        } else if (lineSettings == 2) {
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("->");
          lcd.print(brightnessLeds);
          lcd.setCursor(2, 1);
          lcd.print("Back");
          lineSettings = 1;
        }
        break;
      }
    case DOWN:
      {
        Serial.println("DOWN");
        if (lineSettings == 0) {
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("  ");
          lcd.print(brightnessText);
          lcd.setCursor(0, 1);
          lcd.print("->");
          lcd.print(brightnessLeds);
          lineSettings = 1;
        } else if (lineSettings == 1) {
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("  ");
          lcd.print(brightnessLeds);
          lcd.setCursor(0, 1);
          lcd.print("->");
          lcd.print("Back");
          lineSettings = 2;
        }
        break;
      }
    default:
      {
        break;
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
