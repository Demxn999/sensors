#define pin3 3
#define pin4 4
#define pin5 5
#define pin6 6
int d3 = 0;
int d4 = 0;
String result;

void setup() {
  Serial.begin(9600);
  pinMode(pin3, INPUT);
  pinMode(pin4, INPUT);
  pinMode(pin5, OUTPUT);
  pinMode(pin6, OUTPUT);
}

void loop() {
  pinMode(pin5, OUTPUT);
  pinMode(pin6, INPUT);
  result = "";
  digitalWrite(pin5, HIGH);

  d3 = digitalRead(pin3);
  d4 = digitalRead(pin4);
  
  if (d3 == 1 && d4 == 1) {
    result = result + "12";
  }
  else if (d3 == 1 && d4 == 0) {
    result = result + "1";
  }
  else if (d3 == 0 && d4 == 1) {
    result = result + "2";
  }
  
  pinMode(pin6, OUTPUT);
  pinMode(pin5, INPUT);
  digitalWrite(pin6, HIGH);
  
  d3 = digitalRead(pin3);
  d4 = digitalRead(pin4);
  
  if (d3 == 1 && d4 == 1) {
    result = result + "34";
  }
  else if (d3 == 1 && d4 == 0) {
    result = result + "3";
  }
  else if (d3 == 0 && d4 == 1) {
    result = result + "4";
  }
  Serial.println(result);
}
