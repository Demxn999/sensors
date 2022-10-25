int LedPin = 3;            
int ButPin = 4;             
int status = 0;               

void setup()
{
  pinMode(LedPin, OUTPUT);  
}  

void loop()
{
  if(digitalRead(ButPin) == HIGH && status == 0)      
    {                                              
      digitalWrite(LedPin, !digitalRead(LedPin));
      status = 1;
    }
  
  if(digitalRead(ButPin) == LOW && status == 1)
    {
      status = 0;
    }  
}
