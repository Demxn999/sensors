#define CLOCK_PIN 2
#define RESET_PIN 3

String show_number = "";
int position = 0;
long prev = 0;

void resetNumber()
{
  digitalWrite(RESET_PIN, HIGH);
  digitalWrite(RESET_PIN, LOW);
}

void showNumber(int n)
{
  resetNumber();
  for (byte i=0; i < n; i++) {
    digitalWrite(CLOCK_PIN, HIGH);
    digitalWrite(CLOCK_PIN, LOW);
  }
}

void setup(){
  pinMode(RESET_PIN, OUTPUT);
  pinMode(CLOCK_PIN, OUTPUT);
  resetNumber();
  Serial.begin(9600);
}

void loop(){
  if (Serial.available()){
    show_number = "";
    position = 0;
    while (Serial.available()){
      show_number += char(Serial.read());
      delay(50);
    }
  }
  while (millis() - prev >= 1000){
    prev = millis();
    if (position == 0){
      position++;
      if (show_number.length() > 2){
        resetNumber();
        break;
      }
    }
    int show = (show_number[position - 1] - '0') * 10 + show_number[position] - '0';
    showNumber(show);
    position++;
    position %= show_number.length();
  }
}
