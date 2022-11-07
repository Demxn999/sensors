#include <SoftwareSerial.h>

byte numbers[] = {
  0b11101110,  //0
  0b10100000,  //1
  0b11001101,  //2
  0b11101001,  //3
  0b10100011,  //4
  0b01101011,  //5
  0b01101111,  //6
  0b11100000,  //7
  0b11101111,  //8
  0b11101011,  //9
  0b11100111,  //A
  0b00101111,  //B
  0b01001110,  //C
  0b10101101,  //D
  0b01001111,  //E
  0b01000111   //F 
};

SoftwareSerial mySerial(2, 3);

void dataDisplay(int num){
  for (int i = 0; i < 8; i++){
    digitalWrite(13 - i, (num >> i) & 1);
  }
}

void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);
  for (int i = 6; i <= 13; i++){
    pinMode(i, OUTPUT);
  }
}

void loop() {
  if (Serial.available() >= 1){
    int position = 0;
    byte data = Serial.read();
    if (data >= '0' && data <= '9'){
      position = data - '0';
    }
    if (data >= 'A' && data <= 'F'){
      position = data - 'A' + 10;
    }
    dataDisplay(numbers[position]);
    delay(50);
    if (Serial.available() >= 1){
      while (Serial.available() >= 1){
        mySerial.write(Serial.read());
      }
    } else{
      mySerial.write(byte(0));
    }
  }
}
