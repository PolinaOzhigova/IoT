#define UP 8
#define DOWN 9
#define LEFT 10
#define RIGHT 11
#define PRESSED LOW

void setup()
{
  pinMode(UP, OUTPUT);
  pinMode(DOWN, OUTPUT);
  pinMode(LEFT, INPUT_PULLUP);
  pinMode(RIGHT, INPUT_PULLUP);
  
  Serial.begin(9600);
}

int button[4] = {0, 0, 0, 0};
bool flag = false;

void loop()
{
  digitalWrite(UP, LOW);
  digitalWrite(DOWN, HIGH);
  if(digitalRead(LEFT) == PRESSED){
    button[0] = 1;
  }
  if(digitalRead(RIGHT) == PRESSED){
    button[1] = 1;
  }
  delay(100);
  digitalWrite(UP, HIGH);
  digitalWrite(DOWN, LOW);
  if(digitalRead(LEFT) == PRESSED){
    button[2] = 1;
  }
  if(digitalRead(RIGHT) == PRESSED){
    button[3] = 1;
  }
  String b = "buttons ";
  for(int i=0; i<4; i++){
    if(button[i] == 1){
      b += i + 1;
      b += " ";
      button[i] = 0;
      flag = true;
    }
  }
  if(flag==true){
    Serial.println(b);
    flag = false;
  }
  delay(100);
}
