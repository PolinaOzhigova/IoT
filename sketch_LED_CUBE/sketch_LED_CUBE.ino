int ledpins[] = { 4, 5, 6, 7 };
int groundpins[] = { 2, 3 };

void turn_led_1();
void turn_led_2();
void turn_led_3();
void turn_led_4();
void turn_led_5();
void turn_led_6();
void turn_led_7();
void turn_led_8();

void (*turn_led_functions[8])() = {
  turn_led_1, turn_led_2, turn_led_3, turn_led_4,
  turn_led_5, turn_led_6, turn_led_7, turn_led_8
};

unsigned long lastMillis = 0;
String currentInput = "";

void setup() {
  for (int i = 0; i < 4; i++) {
    pinMode(ledpins[i], OUTPUT);
  }
  for (int i = 0; i < 2; i++) { 
    pinMode(groundpins[i], OUTPUT); 
  }

  Serial.begin(9600);
  Serial.println("Input any LEDs from 1 to 8 in any combination");
}

void loop() {
  if (Serial.available() > 0) {
    currentInput = Serial.readStringUntil('\n');
  }

  unsigned long currentMillis = millis();
  if (currentMillis - lastMillis >= 100) {
    lastMillis = currentMillis;
    handleInput(currentInput);
  }
}

void handleInput(String input) {
  for (int i = 0; i < input.length(); i++) {
    char c = input.charAt(i);
    if (c >= '1' && c <= '8') {
      int ledNumber = c - '1'; // Adjust to zero-index
      turn_led_functions[ledNumber]();
    } else {
      Serial.println("Incorrect LED input. Enter LEDs from 1 to 8.");
    }
  }
}

void turnFloorUP() {
  digitalWrite(groundpins[0], HIGH);
}

void turnFloorDOWN() {
  digitalWrite(groundpins[1], HIGH);
}

void turn_led_1() {
  Serial.println("Turning LED 1");
  turnFloorUP();
  digitalWrite(ledpins[2], LOW);
}

void turn_led_2() {
  Serial.println("Turning LED 2");
  turnFloorUP();
  digitalWrite(ledpins[0], LOW);
}

void turn_led_3() {
  Serial.println("Turning LED 3");
  turnFloorUP();
  digitalWrite(ledpins[1], LOW);
}

void turn_led_4() {
  Serial.println("Turning LED 4");
  turnFloorUP();
  digitalWrite(ledpins[3], LOW);
}

void turn_led_5() {
  Serial.println("Turning LED 5");
  turnFloorDOWN();
  digitalWrite(ledpins[2], LOW);
}

void turn_led_6() {
  Serial.println("Turning LED 6");
  turnFloorDOWN();
  digitalWrite(ledpins[0], LOW);
}

void turn_led_7() {
  Serial.println("Turning LED 7");
  turnFloorDOWN();
  digitalWrite(ledpins[1], LOW);
}

void turn_led_8() {
  Serial.println("Turning LED 8");
  turnFloorDOWN();
  digitalWrite(ledpins[3], LOW);
}