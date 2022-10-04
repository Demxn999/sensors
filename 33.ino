#define PIN_LED 6
#define PIN_PHOTO_SENSOR A0
void setup() {
  Serial.begin (9600);
  pinMode(PIN_LED, OUTPUT);


}

void loop() {
  int val=analogRead (PIN_PHOTO_SENSOR);
  Serial.println(val);
  if (val < 5){
    digitalWrite (PIN_LED, LOW);

  } else {
    digitalWrite(PIN_LED, HIGH);
  }

}
