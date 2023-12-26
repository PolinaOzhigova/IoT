//SEND

#define LOW_SIGNAL LOW
#define HIGH_SIGNAL HIGH
#define SIGNAL_PIN 9
#define TU 100

const char* MORSE[] = {
    ".-",
    "-...",
    "-.-.",
    "-..",
    ".",
    "..-.",
    "--.",
    "....",
    "..",
    ".---",
    "-.-",
    ".-..",
    "--",
    "-.",
    "---",
    ".--.",
    "--.-",
    ".-.",
    "...",
    "-",
    "..-",
    "...-",
    ".--",
    "-..-",
    "-.--",
    "--.."
};

const char ALPHABET[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
const int alph_len = sizeof(ALPHABET) - 1;

void send_morse(String code) {
  Serial.println(code);
  int code_len = code.length(); 
  for (int i = 0; i < code_len; i++){
     char get_char = code[i];
     if (get_char == '.'){
       digitalWrite(SIGNAL_PIN, LOW_SIGNAL);
       delay(TU);
      }
      else{
        digitalWrite(SIGNAL_PIN, LOW_SIGNAL);
        delay(3 * TU);
      }
      digitalWrite(SIGNAL_PIN, HIGH_SIGNAL);
      delay(TU);
   }
   delay(3 * TU);
}

void setup() {

  Serial.begin(9600);
  pinMode(SIGNAL_PIN, OUTPUT);
  digitalWrite(SIGNAL_PIN, HIGH_SIGNAL);

}

void loop() {
  if (Serial.available() > 0) {
    char symbol = Serial.read();
    symbol = toupper(symbol);

    for (int i = 0; i < alph_len; i++) {
      if (symbol == ALPHABET[i]) {
        const char* code = MORSE[i];
        send_morse(code);
        break;
      }
    }

  }
}

//GET

#define LOW_SIGNAL LOW
#define HIGH_SIGNAL HIGH
#define SIGNAL_PIN 2
#define TU 100

const char* MORSE[] = {
    ".-",
    "-...",
    "-.-.",
    "-..",
    ".",
    "..-.",
    "--.",
    "....",
    "..",
    ".---",
    "-.-",
    ".-..",
    "--",
    "-.",
    "---",
    ".--.",
    "--.-",
    ".-.",
    "...",
    "-",
    "..-",
    "...-",
    ".--",
    "-..-",
    "-.--",
    "--.."
};

const char ALPHABET[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
const int alph_len = sizeof(ALPHABET) - 1;

long dataTime, spaceTime;

long symbolTime[50];
bool dotOrLine[50];

int index = 0;
int previousSignal = HIGH_SIGNAL;
bool isEnd = false;
String message = "";


void decode_morse(){
  bool flag = false;

  for (int i = 0; i < index; i++){

    if (symbolTime[i] >= 3 and dotOrLine[i] == false){
      String morse = "";

      for (int j = 0; j < i; j++){

        if (symbolTime[j] == 3 and dotOrLine[j]){morse += '-';}
        else if (symbolTime[j] == 1 and dotOrLine[j]){morse += '.';}

        symbolTime[j] = 0;

      }

      symbolTime[i] = 0;

      for (int k = 0; k < alph_len; k++) {

        if (morse == MORSE[k]){
          flag = true;
          message += ALPHABET[k];
          Serial.print(message);
        }

      }

    }

    //end
    if (flag && isEnd){
      Serial.print(".\n");
      index = 0;
      isEnd = false;
    }

  }

  //reset
  if (flag){
    index = 0;
    message = "";
  }

}

void get_string(){

  int currentSignal = digitalRead(SIGNAL_PIN);

  if (currentSignal == LOW_SIGNAL && previousSignal == HIGH_SIGNAL){

    dataTime = millis();
    symbolTime[index] = int((millis() - spaceTime + 0.5 * TU) / TU);

    dotOrLine[index] = false;
    index++;
  }
  else if (currentSignal == HIGH_SIGNAL && previousSignal == LOW_SIGNAL){

    isEnd = false;

    spaceTime = millis();
    symbolTime[index] = int((millis() - dataTime + 0.5 * TU) / TU);

    dotOrLine[index] = true;
    index++;

  }
  else if (isEnd == false && currentSignal == HIGH_SIGNAL && previousSignal == HIGH_SIGNAL){

    long temp = millis() - spaceTime;

    if (temp > TU * 7){
      isEnd = true;
      symbolTime[index] = 7;
      index++;
    }

  }

  previousSignal = currentSignal;

}

void setup() {
  Serial.begin(9600);
  pinMode(SIGNAL_PIN, INPUT);
}

void loop() {
  get_string();
  decode_morse();
}