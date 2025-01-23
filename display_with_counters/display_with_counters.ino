// Segment patterns for numbers 0–9 (a, b, c, d, e, f, g)
byte segValue[10][7] = {
   {0,0,0,0,0,0,1}, // 0
   {1,0,0,1,1,1,1}, // 1
   {0,0,1,0,0,1,0}, // 2
   {0,0,0,0,1,1,0}, // 3
   {1,0,0,1,1,0,0}, // 4
   {0,1,0,0,1,0,0}, // 5
   {0,1,0,0,0,0,0}, // 6
   {0,0,0,1,1,1,1}, // 7
   {0,0,0,0,0,0,0}, // 8
   {0,0,0,0,1,0,0}  // 9  
};

int score = 0;
bool scoreUpdated = false;  // Flag to indicate score change

volatile bool increasePressed = false;
volatile bool decreasePressed = false;

// Segment pins (a to g and decimal point)
byte segPin[10] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
byte digitPin[2] = {A0, A1};  // Control which display is active

void setup() {
  Serial.begin(9600);

  // Attach interrupts for increase and decrease buttons
  attachInterrupt(digitalPinToInterrupt(segPin[0]), InterruptIncrease, FALLING);
  attachInterrupt(digitalPinToInterrupt(segPin[1]), InterruptDecrease, FALLING);
  

  pinMode(segPin[0], INPUT_PULLUP);  // Increase button
  pinMode(segPin[1], INPUT_PULLUP);  // Decrease button
 
  
  // Set segment pins as OUTPUT
  for (int i = 2; i < 10; i++) {
    pinMode(segPin[i], OUTPUT);
  }

  // Set digit control pins as OUTPUT
  pinMode(digitPin[0], OUTPUT);
  pinMode(digitPin[1], OUTPUT);

  // Turn off both displays initially
  digitalWrite(digitPin[0], LOW);
  digitalWrite(digitPin[1], LOW);
}

void loop() {
  // Handle score changes based on button presses
  if (increasePressed) {
    score = min(score + 1, 99);  // Clamp score to a maximum of 99
    increasePressed = false;
    scoreUpdated = true;
  }
  if (decreasePressed) {
    score = max(score - 1, 0);  // Clamp score to a minimum of 0
    decreasePressed = false;
    scoreUpdated = true;
  }

  // Only print and update display if the score changed
  if (scoreUpdated) {
    Serial.println(score);
    scoreUpdated = false;
  }

  // Continuously display the current score
  display_N(score);
  delay(5);  // Small delay to prevent flickering
}

// Interrupt Service Routine to detect button presses
void InterruptIncrease() {
  increasePressed = true;
}

void InterruptDecrease() {
  decreasePressed = true;
}


void display_N(int num) {
  int units = num % 10;            // Extract units digit
  int tens = (num / 10) % 10;       // Extract tens digit

  // Display both digits with dynamic refresh
  segOutput(1, units);  // Display units (without decimal point)
  segOutput(0, tens);   // Display tens (without decimal point)
  delay(2);  // Small delay for stability
}

void segClear() {
  // Turn off all segment LEDs
  for (int i = 2; i < 10; i++) {
    digitalWrite(segPin[i], LOW);
    digitalWrite(segPin[9], LOW);
  }
}

void segOutput(int d, int number) {
  segClear();  // Clear previous segments
  digitalWrite(digitPin[d], HIGH);  // Activate the selected display

  // Write segment values for the given number
  for (int i = 2; i < 9; i++) {
    digitalWrite(segPin[i], segValue[number][i - 2]);
    digitalWrite(segPin[9], LOW);
  }


  delayMicroseconds(1000);  // Small delay for stabilization
  digitalWrite(digitPin[d], LOW);  // Deactivate the display
}





