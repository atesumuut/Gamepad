#include "Keyboard.h"
#include "Mouse.h"
#define BUTTON_UP 2
#define BUTTON_RIGHT 3
#define BUTTON_DOWN 4
#define BUTTON_LEFT 5
#define BUTTON_E 6
#define BUTTON_F 7

int aKeyPressCount = 0;
int dKeyPressCount = 0;
bool spaceKeyPressed = false;
bool rKeyPressed = false;
int lastJoystickX = 0;
int lastJoystickY = 0;
const int deadzone = 3;

void setup() {
  Keyboard.begin();
  Mouse.begin();
  Serial.begin(9600);
  pinMode(BUTTON_UP, INPUT_PULLUP);
  pinMode(BUTTON_RIGHT, INPUT_PULLUP);
  pinMode(BUTTON_DOWN, INPUT_PULLUP);
  pinMode(BUTTON_LEFT, INPUT_PULLUP);
  pinMode(BUTTON_E, INPUT_PULLUP);
  pinMode(BUTTON_F, INPUT_PULLUP);
}

void loop() {
  bool buttonUpState = (digitalRead(BUTTON_UP) == LOW);
  bool buttonRightState = (digitalRead(BUTTON_RIGHT) == LOW);
  bool buttonDownState = (digitalRead(BUTTON_DOWN) == LOW);
  bool buttonLeftState = (digitalRead(BUTTON_LEFT) == LOW);
  bool buttonEState = (digitalRead(BUTTON_E) == LOW);
  bool buttonFState = (digitalRead(BUTTON_F) == LOW);

  if (buttonUpState) {
    Keyboard.press('w');
  } else {
    Keyboard.release('w');
  }

  if (buttonRightState) {
    Keyboard.press('d');
    if (++dKeyPressCount == 2) {
      rKeyPressed = true;
      dKeyPressCount = 0;
    }
  } else {
    Keyboard.release('d');
  }

  if (buttonDownState) {
    Keyboard.press('s');
  } else {
    Keyboard.release('s');
  }

  if (buttonLeftState) {
    Keyboard.press('a');
    if (++aKeyPressCount == 2) {
      spaceKeyPressed = true;
      aKeyPressCount = 0;
    }
  } else {
    Keyboard.release('a');
  }

  if (buttonEState) {
    Mouse.click(MOUSE_RIGHT);
  }

  if (buttonFState) {
    Mouse.click(MOUSE_LEFT);
  }

  if (!buttonUpState && !buttonRightState && !buttonDownState && !buttonLeftState) {
    if (spaceKeyPressed) {
      Keyboard.write(' ');
      spaceKeyPressed = false;
    }
    if (rKeyPressed) {
      Keyboard.write('r');
      rKeyPressed = false;
    }
  }

  int joystickX = analogRead(A0);
  int joystickY = analogRead(A1);

  joystickX = map(joystickX, 0, 1023, -20, 20);
  joystickY = map(joystickY, 0, 1023, -20, 20);

  if (abs(joystickX) > deadzone || abs(joystickY) > deadzone) {
    Mouse.move(joystickX, -joystickY, 0);
    lastJoystickX = joystickX;
    lastJoystickY = joystickY;
  } else {
    Mouse.move(0, 0, 0);
  }

  delay(10);
}
