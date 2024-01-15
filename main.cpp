#include <ESP32Servo.h>

Servo myservo;
const int photoResistorPin = 34;
const int threshold = 300;
int currentServoPosition = 0;  // aktualna pozycja serwomechanizmu

// Piny diod LED
const int ledPins[] = {13, 15, 27, 26, 25};
const int numLEDs = sizeof(ledPins) / sizeof(ledPins[0]);

// Deklaracja funkcji moveToPosition
void moveToPosition(int position);

void openFlower();
void closeFlower();

void moveToPosition(int position) {
  // Płynne przesuwanie serwomechanizmu do żądanej pozycji
  int speed = 3;  // Zmniejszenie prędkości dla wolniejszego ruchu
  int positionDiff = abs(position - currentServoPosition);

  for (int i = 0; i <= positionDiff; i += speed) {
    int nextPosition = map(i, 0, positionDiff, currentServoPosition, position);
    myservo.write(nextPosition);
    delay(20);  // Zwiększenie przerwy dla płynności
  }

  // Aktualizacja pozycji serwomechanizmu
  currentServoPosition = position;
}

void setup() {
  myservo.attach(23);
  pinMode(photoResistorPin, INPUT);
  myservo.write(0);  // początkowa pozycja serwomechanizmu
  currentServoPosition = 0;

  // Ustawienie pinów diod LED jako wyjścia
  for (int i = 0; i < numLEDs; i++) {
    pinMode(ledPins[i], OUTPUT);
    digitalWrite(ledPins[i], LOW);  // Wyłączenie diod LED na początku
  }
}

void loop() {
  int lightValue = analogRead(photoResistorPin);

  if (lightValue < threshold) {
    // Jeśli światło jest naświetlone, otwórz kwiat i włącz diody LED
    openFlower();

    // Poczekaj, aż światło zgaśnie
    while (analogRead(photoResistorPin) < threshold) {
      delay(100);
    }

    // Zamknij kwiat i wyłącz diody LED
    closeFlower();
  }

  // krótsza przerwa, aby uniknąć zbyt szybkiego odczytu światła
  delay(100);
}

void openFlower() {
  int openPosition = 120;
  moveToPosition(openPosition);

  // Włącz diody LED
  for (int i = 0; i < numLEDs; i++) {
    digitalWrite(ledPins[i], HIGH);
  }
}

void closeFlower() {
  // Wyłącz diody LED
  for (int i = 0; i < numLEDs; i++) {
    digitalWrite(ledPins[i], LOW);
  }

  int closePosition = 0;
  moveToPosition(closePosition);
}
