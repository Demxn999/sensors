#define led_pin 9
#define sensor_pin A2
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(led_pin, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
if (Serial.available()>0){
    char message = Serial.read();
    //digitalWrite(led_pin, LOW HIGH);
    if (message == 'u') {
      digitalWrite(led_pin, HIGH);
    }else if (message == 'd'){
      digitalWrite (led_pin, LOW);
    }else if (message == 's'){
      int val = analogRead(sensor_pin);
      val=map(val,0,1023,100,999);
      Serial.print(val);
    } else {
      Serial.println("unknown message");
  }
 }
}