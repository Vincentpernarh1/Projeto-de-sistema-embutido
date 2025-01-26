// States
enum State {WAIT_PRESS, WAIT_RELEASE, LED_SEQUENCE, PISCA_LED,ON};
State currentState = ON;

// Define the pin numbers
const int buttonPin = 4;    // GPIO pin for button
const int ledPin = 15;      // GPIO pin for LED

// Debounce delay in milliseconds
const long debounceDelay = 10;
unsigned long lastDebounceTime = 0; // Último tempo de debounce

// Button states
bool buttonstate_pres = LOW;
bool buttonstate_unpres = HIGH;

void setup() {
  // Configure the button pin with an internal pull-up resistor
  pinMode(buttonPin, INPUT_PULLUP);

  // Configure the LED pin as output
  pinMode(ledPin, OUTPUT);

  // Initialize the LED to OFF
  digitalWrite(ledPin, LOW);
  attachInterrupt(digitalPinToInterrupt(buttonPin), switchInterrupt, RISING);
  // Initialize serial for debugging
  Serial.begin(115200);
}

void loop() {
  switch (currentState) {
    case WAIT_PRESS:
      // Wait for the button to be pressed
      buttonstate_pres = HIGH;
      currentState = WAIT_RELEASE;  // Move to next state
      break;

    case WAIT_RELEASE:
      // Wait for the button to be released
      buttonstate_unpres = LOW;
      currentState = LED_SEQUENCE;  // Move to LED sequence

      break;

    case LED_SEQUENCE:
      // Turn LED ON for 1 second
      digitalWrite(ledPin, HIGH);
      Serial.println("Button pressed and released");
      func_delay(1000);

      // Turn LED OFF
      digitalWrite(ledPin, LOW);
      func_delay(2000);
      currentState =PISCA_LED;
      break;

    case PISCA_LED:
      // Turn LED ON for 250 ms (indicating the end of the sequence)
      digitalWrite(ledPin, HIGH);
      func_delay(250);
      digitalWrite(ledPin, LOW);

      func_delay(250);
      digitalWrite(ledPin, HIGH);
      func_delay(250);
      digitalWrite(ledPin, LOW);
      
         // Return to initial state/ bloking state.
      currentState = ON;
      break;

    default:
      currentState = ON;
      break;
  }
}


//Implementing debounce function
void switchInterrupt() {
  unsigned long currentMillis = millis();


  if (currentMillis - lastDebounceTime >= debounceDelay ) {
   currentState = WAIT_PRESS;
  }


  lastDebounceTime = currentMillis;
}


//implementing delay function

void func_delay(unsigned long timer) {
  unsigned long startMillis = millis();  // Record the start time
  while (millis() - startMillis < timer) {
    // Keep looping until the desired time has passed
  }
}