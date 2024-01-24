#include <Servo.h>

const int trigPin = 8;
const int echoPin = 9;
const int servoPin = 11;
Servo servo;
int angle = 0;
int duration = 0;
int distance = 0;
char valueReceived;
int min_sweep = 0;
int max_sweep = 180;
int sweep_rate = 40;
unsigned long previousMillis = 0;
bool increasing = true;

int calculateDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;
  return distance;
}

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  servo.attach(servoPin);
  Serial.begin(9600);
}

void loop() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= sweep_rate) {

    if (angle < max_sweep && increasing) {
      servo.write(angle);
      calculateDistance();
      Serial.print(angle);
      Serial.print(",");
      Serial.println(distance);
      angle++;
    } else {
      increasing = false;
    }

    if (angle >= min_sweep && !increasing) {
      servo.write(angle);
      calculateDistance();
      Serial.print(angle);
      Serial.print(",");
      Serial.println(distance);
      angle--;

      if (angle <= min_sweep) {
        increasing = true;
      }
    }
    previousMillis = currentMillis;
  }

  buttons(); 
  calculateDistance();
}


void buttons() {
  if (Serial.available() > 0) {
    valueReceived = Serial.read();
    switch (valueReceived) {
      case 'r':
        sweep_rate = 7;
        break;
      case 't':
        sweep_rate = 20;
        break;
      case 'b':
        sweep_rate = 40;
        break;
      case 'e':
        sweep_rate = 80;
        break;
      case 'y':
        min_sweep = 0;
        max_sweep = 180;
        break;
      case 'u':
        min_sweep = 30;
        max_sweep = 150;
        break;
      case 'i':
        min_sweep = 60;
        max_sweep = 120;
        break;
      case 'o':
        min_sweep = 70;
        max_sweep = 110;
        break;
      case 'p':
        min_sweep = 84;
        max_sweep = 96;
        break;
      default:
        break;
    }
    delay(100);
  } else {
  }
}
