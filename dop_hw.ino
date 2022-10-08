#define led_pin 4
#define sensor_pin A2

unsigned long previousMillis = 0;       
int interval = 100;           
int ledState = LOW;

void setup() {
  Serial.begin(9600);
  pinMode(led_pin, OUTPUT);
}

void loop() {
  unsigned long currentMillis = millis();
  int val = analogRead(sensor_pin);
  Serial.println(val);
  if (val>100){
      interval=40;
   }
  if (val<100){
    interval=200;
   }
  if(currentMillis - previousMillis > interval) {
    previousMillis = currentMillis; 
    if (ledState == LOW)
      ledState = HIGH;
    else
      ledState = LOW;
    digitalWrite(led_pin, ledState);
  }

}