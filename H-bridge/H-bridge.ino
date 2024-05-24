#define PIN_L 3
#define PIN_R 5

void setup(){
  pinMode(PIN_L, OUTPUT);
  pinMode(PIN_R, OUTPUT);
  Serial.begin(9600);
}

void loop(){
  if (Serial.available()) {
    String input = Serial.readString();
    char command = input[0];
    input.remove(0, 1); 
    int speed = input.toInt();
    if (command == 'l') {
        analogWrite(PIN_L, speed); 
    	analogWrite(PIN_R, 0);
    } else if (command == 'r') {
        analogWrite(PIN_R, speed); 
    	analogWrite(PIN_L, 0);
    } else {
      Serial.print("Unknown command, input l or r ");
    }
  }
}