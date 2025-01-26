

#include <Arduino.h>
// ------------------------------------------------------------------ Pins
const int ledPin = 15; // Pino do LED (no Arduino UNO, é o pino 13)
const int interruptPin = 4; // Pino da chave


// -------------------------------------------- Variáveis para interrupção
unsigned long lastDebounceTime = 0; // Último tempo de debounce
unsigned long debounceDelay = 10; // Tempo de debounce em milissegundos
volatile bool start = false;


int state = 0; // 0 -   WAIT_FOR_PRESS, 1 -  LED_ON, 2 - LED_OFF, 3 - LED_BLINK
void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(interruptPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPin), switchInterrupt, RISING);
}


void loop() {
  switch (state) {
    case 0:
      if (start) {
        start = false; // Limpa a flag da chave
        state = 1;
      }
      break;


    case 1:
      digitalWrite(ledPin, HIGH); // Passo 2: Acende o LED
      delay(1000);
      state = 2;
      break;


    case 2:
      digitalWrite(ledPin, LOW); // Passo 3: Apaga o LED
      delay(2000);
      state = 3;
      break;


    case 3:
      digitalWrite(ledPin, HIGH);
      delay(250);
      digitalWrite(ledPin, LOW);
      delay(250);
      digitalWrite(ledPin, HIGH);
      delay(250);
      digitalWrite(ledPin, LOW);
      delay(250);
      state = 0;
      start = false;
      break;
  }
}


void switchInterrupt() {
  unsigned long currentMillis = millis();


  if (currentMillis - lastDebounceTime >= debounceDelay) {
    start = true;
  }


  lastDebounceTime = currentMillis;
}
