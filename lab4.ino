void setup() {
#define GREEN_LED 12
#define RED_LED 11
#define BTN_1 5
#define BTN_2 4
#define BTN_3 3
#define BTN_4 2

const int passcodeLength = 4;
const int passcode[] = {BTN_4, BTN_2, BTN_1, BTN_4};
int input_passcode[passcodeLength] = {-1, -1, -1, -1};  // Initialize with invalid values

bool isLocked = true;

void setup() {
  pinMode(GREEN_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(BTN_1, INPUT_PULLUP);
  pinMode(BTN_2, INPUT_PULLUP);
  pinMode(BTN_3, INPUT_PULLUP);
  pinMode(BTN_4, INPUT_PULLUP);

  Serial.begin(9600);

  // Ensure system starts in locked state
  lockSystem();
}

void loop() {
  checkButtonPress(BTN_1);
  checkButtonPress(BTN_2);
  checkButtonPress(BTN_3);
  checkButtonPress(BTN_4);

  if (isLocked && isCorrectPasscode()) {
    Serial.println("Correct passcode!");
    unlockSystem();
  }

  delay(50);  // Small delay to debounce and slow down loop
}

void checkButtonPress(int button) {
  if (digitalRead(button) == LOW) {
    storeButtonPress(button);
    delay(200);  // Debounce delay
  }
}

void storeButtonPress(int button) {
  // FIFO mechanism
  for (int i = 1; i < passcodeLength; i++) {
    input_passcode[i - 1] = input_passcode[i];
  }
  input_passcode[passcodeLength - 1] = button;

  Serial.print("Button pressed: ");
  Serial.println(button);
}

bool isCorrectPasscode() {
  for (int i = 0; i < passcodeLength; i++) {
    if (input_passcode[i] != passcode[i]) {
      return false;  // Incorrect sequence found
    }
  }
  return true;  // All buttons matched
}

void unlockSystem() {
  isLocked = false;
  digitalWrite(RED_LED, LOW);
  digitalWrite(GREEN_LED, HIGH);
  delay(5000);  // Keep unlocked for 5 seconds
  lockSystem();
}

void lockSystem() {
  isLocked = true;
  resetInputPasscode();  // Important: Reset the input array when locking again
  digitalWrite(RED_LED, HIGH);
  digitalWrite(GREEN_LED, LOW);
}

void resetInputPasscode() {
  for (int i = 0; i < passcodeLength; i++) {
    input_passcode[i] = -1;  // Reset all entries to an invalid value
  }
}