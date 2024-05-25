#include <SoftwareSerial.h>

#define DIR_1        4 
#define SPEED_1      5 
#define SPEED_2      6 
#define DIR_2        7 

#define CAR_SPEED 255 
#define RX_PIN 10
#define TX_PIN 11

#define START_CMD 'A'
#define PAUSE_CMD 'P'
#define SAVE_CMD 'S'
#define CHANGE_CMD 'C'
#define SPEED_CMD 'X'

#define FORWARD_CMD 'F'
#define BACKWARD_CMD 'B'
#define ROTATE_LEFT_CMD 'L'
#define ROTATE_RIGHT_CMD 'R'
#define END_CMD '0'

const int numRows = 4;
const int numCols = 2;
int currentIndex = 0; 

int states[numRows][numCols] = {
    {LOW, LOW},
    {LOW, HIGH},
    {HIGH, LOW},
    {HIGH, HIGH}
};

int resultsIndices[] = {0, 0, 0, 0};

SoftwareSerial androidDeviceSerial(RX_PIN, TX_PIN);
bool isCalibrating = false; 
char movementCommand = 'P';
char commands[] = {FORWARD_CMD, BACKWARD_CMD, ROTATE_LEFT_CMD, ROTATE_RIGHT_CMD};
bool anyConfigured = false;
bool fullyConfigured = false;
bool speedConf = false;

bool confRight = false;
bool confLeft = false;
bool confTop = false;
bool confBot = false;

int leftWheelSpeed = CAR_SPEED;
int rightWheelSpeed = CAR_SPEED;

enum CalibrationState {
    NOT_CALIBRATING,
    CALIBRATING_LEFT_WHEEL,
    CALIBRATING_RIGHT_WHEEL
};

CalibrationState calibrationState = NOT_CALIBRATING;

void moveCart(bool dirFirstState, bool dirSecondState){
  digitalWrite(DIR_1, dirFirstState);
  digitalWrite(DIR_2, dirSecondState);
  analogWrite(SPEED_1, leftWheelSpeed);
  analogWrite(SPEED_2, rightWheelSpeed);
}

void setup() {
  Serial.begin(57600);
  androidDeviceSerial.begin(9600);
  for (int i = 4; i < 8; i++) {
    pinMode(i, OUTPUT);
  }
}

bool isMovementCommand(char inpCommand){
    return inpCommand == FORWARD_CMD || inpCommand == BACKWARD_CMD || inpCommand == ROTATE_LEFT_CMD || inpCommand == ROTATE_RIGHT_CMD;
}

void stop() {
  analogWrite(SPEED_1, 0);
  analogWrite(SPEED_2, 0);
}

int indexOfCommand(char command){
  for (int i = 0; i < 4; i++){
    if (command == commands[i]){
      return i;
    }
  }
  return 0; 
}

bool areAllConfigurated(){
  return confBot && confTop && confLeft && confRight;
}

bool handleCommand(char command){
  if (command == START_CMD && !isCalibrating){
    Serial.println("Calibration has been started!");
    isCalibrating = true;
    return true;
  }
  if (command == PAUSE_CMD){
    Serial.println("Calibration has been paused!");
    stop();
    return true;
  }

  if (fullyConfigured && isMovementCommand(command) && !speedConf){
        int index = indexOfCommand(command);
        int dirIndex = resultsIndices[index];
        moveCart(states[dirIndex][0], states[dirIndex][1]);
        return true;
  }

  if (command == SPEED_CMD) {
    speedConf = true;
    switch (calibrationState) {
      case NOT_CALIBRATING:
        Serial.println("Calibrating left wheel. Use F to increase speed, B to decrease speed.");
        calibrationState = CALIBRATING_LEFT_WHEEL;
        break;
      case CALIBRATING_LEFT_WHEEL:
        Serial.println("Calibrating right wheel. Use F to increase speed, B to decrease speed.");
        calibrationState = CALIBRATING_RIGHT_WHEEL;
        break;
      case CALIBRATING_RIGHT_WHEEL:
        Serial.println("Calibration completed and saved.");
        calibrationState = NOT_CALIBRATING;
        speedConf = false;
        break;
    }
    return true;
  }

  if (calibrationState == CALIBRATING_LEFT_WHEEL) {
    if (command == FORWARD_CMD) {
      leftWheelSpeed = min(255, leftWheelSpeed + 10);
      Serial.print("Left wheel speed increased to ");
      Serial.println(leftWheelSpeed);
    } else if (command == BACKWARD_CMD) {
      leftWheelSpeed = max(0, leftWheelSpeed - 10);
      Serial.print("Left wheel speed decreased to ");
      Serial.println(leftWheelSpeed);
    }
    return true;
  }

  if (calibrationState == CALIBRATING_RIGHT_WHEEL) {
    if (command == FORWARD_CMD) {
      rightWheelSpeed = min(255, rightWheelSpeed + 10);
      Serial.print("Right wheel speed increased to ");
      Serial.println(rightWheelSpeed);
    } else if (command == BACKWARD_CMD) {
      rightWheelSpeed = max(0, rightWheelSpeed - 10);
      Serial.print("Right wheel speed decreased to ");
      Serial.println(rightWheelSpeed);
    }
    return true;
  }

  if (isCalibrating){

    if (isMovementCommand(command)){
      movementCommand = command; 
      return true; 
    } else {
      if (movementCommand == 'P' && !anyConfigured){
        Serial.println("Unable to change the configuration: direction not set!");
        return true;
      } else {
        if (command == CHANGE_CMD){
          Serial.println("Changing dirrection");
          currentIndex++;
          if (currentIndex > 3){
            currentIndex = 0;
          }
          moveCart(states[currentIndex][0], states[currentIndex][1]);
          return true; 
        } else if (command == SAVE_CMD){
            Serial.print("Direction for command ");
            Serial.print(movementCommand);
            Serial.println(" has been saved");
            resultsIndices[indexOfCommand(movementCommand)] = currentIndex;
            currentIndex = 0; 
            stop();
            
            if (movementCommand == 'F') confTop = true;
            if (movementCommand == 'B') confBot = true;
            if (movementCommand == 'L') confLeft = true;
            if (movementCommand == 'R') confRight = true;
           
            movementCommand = 'P';
            anyConfigured = true;
            if (areAllConfigurated()){
              Serial.println("All the directions have been configured! You can use the cart!");
              fullyConfigured = true;
            }
            return true; 
        }
      }
    }
  }
  return false;
}

void loop() {
  if (androidDeviceSerial.available()) {
    char commandReal = androidDeviceSerial.read();
    if (commandReal != END_CMD){
      bool h = handleCommand(commandReal);
    }
  }
}
