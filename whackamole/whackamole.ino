const int ledPins[9] = {2}; // LED Pins
const int buttonPins[9] = {13}; //Button pins
const unsigned long debounceDelay = 50; // Debounce time (ms)
const unsigned long gameDuration = 90000; // 90 seconds in ms

int currentLED = -1; // Index of currently lit LED
int score = 0; // Player score

unsigned long lastDebounceTime[9] = {0}; // For debouncing
bool lastButtonState[9] = {false}; // Previous button states

unsigned long gameStartTime;
bool gameOver = false;

void setup() {
  Serial.begin(9600);

  // Initialize LEDs
  for (int i = 0; i < 9; i++) {
    pinMode(ledPins[i], OUTPUT); // Button to GND
    digitalWrite(ledPins[i], LOW);
  }

  //Initialize buttons with pull-up resistors
  for (int i = 0; i < 9; i++) {
    pinMode(buttonPins[i], INPUT_PULLUP);
  }

  // Seed random generator
  randomSeed(analogRead(A0));

  // Start game
  gameStartTime = millis();
  pickNewLED();
}

void loop() {
  if (gameOver) return; // Stop game after time limit

  // Check time limit
  if (millis() - gameStartTime >= gameDuration) {
    endGame();
    return;
  }
  // Check all buttons
  for (int i = 0; i < 9; i++) {
    bool reading = !digitalRead(buttonPins[i]); // Active LOW 
    
    if (reading != lastButtonState[i]) {
      lastDebounceTime[i] = millis();
    }

    if ((millis() - lastDebounceTime[i]) > debounceDelay) {
      if (reading && !lastButtonState[i]) { // Button just pressed
      if (i == currentLED){
        score++;
        Serial.print("Correct! Score: ");
        Serial.println(score);
        pickNewLED(); // Immediately pick a new LED
      } else {
        score--;
        Serial.print("Wrong! Score: ");
        Serial.println(score);
      }
    }
  }
  lastButtonState[i] = reading;
  }
}

void pickNewLED() {
  // Turn off current LED
  if (currentLED != -1) {
    digitalWrite(ledPins[currentLED], LOW);
  }

  // Pick a new random LED (different from last one)
  int newLED;
  do {
    newLED = random(0, 9);
  } while (newLED == currentLED);

  currentLED = newLED;
  digitalWrite(ledPins[currentLED], HIGH);
}

void endGame() {
  gameOver = true;

  // Turn off all LEDs
  for (int i = 0; i < 9; i++) {
    digitalWrite(ledPins[i], LOW);
  }
  Serial.println("=== GAME OVER===");
  Serial.print("Final Score: ");
  Serial.println(score);
}


// this is a test
