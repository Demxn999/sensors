#define sensor_pin A2
#define led_pin 3 

int val_max = 0;
int val_min = 1024;

void setup() {
  Serial.begin(9600);
  pinMode(led_pin, OUTPUT);
}

void loop() {
  int val = analogRead(sensor_pin);
  Serial.println(val);
  delay(100);

  if (val > val_max)
  {
    val_max = val;
  }

  if (val < val_min)
  {
    val_min = val;
  }

  if (val < (val_max + val_min)/2) 
  {
    digitalWrite(led_pin, HIGH); 
  }
  else 
  {
    digitalWrite(led_pin, LOW);   
  }
}