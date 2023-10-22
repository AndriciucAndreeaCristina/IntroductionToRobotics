const int pinBlueA1 = A1;
const int pinGreenA2 = A2;
const int pinRedA3 = A3;

const int ledCathodeBlue = 9;
const int ledCathodeGreen = 10;
const int ledCathodeRed = 11;

int resistenceValueRed, ledValueRed;
int resistenceValueBlue, ledValueBlue;
int resistenceValueGreen, ledValueGreen;

void setup() {
  
  pinMode(pinBlueA1, INPUT);
  pinMode(ledCathodeBlue, OUTPUT);

  pinMode(pinGreenA2, INPUT);
  pinMode(ledCathodeGreen, OUTPUT);

  pinMode(pinRedA3, INPUT);
  pinMode(ledCathodeRed, OUTPUT);

}

void loop() {

  resistenceValueGreen = analogRead(ledCathodeGreen);
  ledValueGreen = map(resistenceValueGreen, 0, 1023, 0, 255);
  analogWrite(ledCathodeGreen, ledValueGreen);
  
  resistenceValueBlue = analogRead(ledCathodeBlue);
  ledValueBlue = map(resistenceValueBlue, 0, 1023, 0, 255);
  analogWrite(ledCathodeBlue, ledValueBlue);

  resistenceValueRed = analogRead(ledCathodeRed);
  ledValueRed = map(resistenceValueRed, 0, 1023, 0, 255);
  analogWrite(ledCathodeRed, ledValueRed);
}

