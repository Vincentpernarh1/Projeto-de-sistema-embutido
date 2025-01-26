const int ledGPIO = 21;
const int buttonGPIO = 15;

unsigned long debounceStartTime = 0;
const unsigned long debounceDuration = 20;

volatile bool stateChangeDetected = false;
volatile bool buttonCurrentState = HIGH;
volatile bool buttonIsPressed = false;

bool ledSequenceInProgress = false;
unsigned long pressStartTime = 0; // Time when the button was pressed
unsigned long pressDuration = 0; // Duration of the button press

enum States {
    WAIT_BUTTON_PRESS,
    WAIT_BUTTON_RELEASE,
    DEBOUNCE_RELEASE,
    LED_SEQUENCE,
    FINAL_BLINKS
};
int currentState = WAIT_BUTTON_PRESS;

void setup() {
    Serial.begin(115200);
    pinMode(ledGPIO, OUTPUT);
    pinMode(buttonGPIO, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(buttonGPIO), handleButtonInterrupt, CHANGE);
}

void loop() {
    unsigned long currentMillis = millis();

    switch (currentState) {
        case WAIT_BUTTON_PRESS:
            if (stateChangeDetected && buttonIsPressed) {
                stateChangeDetected = false;
                pressStartTime = currentMillis; // Record the press start time
                currentState = WAIT_BUTTON_RELEASE;
            }
            break;

        case WAIT_BUTTON_RELEASE:
            if (stateChangeDetected && !buttonIsPressed) {
                stateChangeDetected = false;
                pressDuration = currentMillis - pressStartTime; // Calculate press duration
                debounceStartTime = currentMillis;
                currentState = DEBOUNCE_RELEASE;
            }
            break;

        case DEBOUNCE_RELEASE:
            if ((currentMillis - debounceStartTime) > debounceDuration) {
                currentState = LED_SEQUENCE;
                ledSequenceInProgress = true;
            }
            break;

        case LED_SEQUENCE:
            if (pressDuration < 1000) {
                blinkLED(1, 500); // Blink 1 time, 0.5 seconds each
            } else if (pressDuration >= 1000 && pressDuration < 2000) {
                blinkLED(2, 500); // Blink 2 times, 0.5 seconds each
            } else if (pressDuration >= 3000) {
                blinkLED(3, 500); // Blink 3 times, 0.5 seconds each
            }
            currentState = FINAL_BLINKS; 
            break;

        case FINAL_BLINKS:
            blinkLED(2, 250); // Rapid blink 2 times (0.25s each)
            ledSequenceInProgress = false;
            currentState = WAIT_BUTTON_PRESS; // Return to waiting for button press
            break;
    }
}

void handleButtonInterrupt() {
    if (!ledSequenceInProgress) {
        buttonCurrentState = digitalRead(buttonGPIO);
        if ((millis() - debounceStartTime) > debounceDuration) { // Check debounce timing
            stateChangeDetected = true;
            buttonIsPressed = (buttonCurrentState == LOW);
        }
        debounceStartTime = millis();
    }
}

void blinkLED(int times, int duration) {
    for (int i = 0; i < times; i++) {
        digitalWrite(ledGPIO, HIGH);
        delay(duration / 2);
        digitalWrite(ledGPIO, LOW);
        delay(duration / 2);
    }
}
