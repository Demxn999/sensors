#define pin_0 11
#define pin_1 10

void setup() {
  Serial.begin(9600);
  pinMode(pin_0, OUTPUT);
  pinMode(pin_1, OUTPUT);
}

void switch_motor(bool vector = 0, int speed = 255){
  if (vector){
    analogWrite(pin_0, speed);
  }
  else if (!vector){
    analogWrite(pin_1, speed);
  }
}

void loop() {
  switch_motor(1, 200);
  digitalWrite(11, LOW); 
  delay(2000);
  digitalWrite(11, HIGH); 
  switch_motor(0, 100);
  delay(2000);
}
