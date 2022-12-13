#define DATA_PIN 2
#define LETTER_SEP 3
#define IDLE_TIME 10
#define DATA_LEVEL LOW

float TU = 1000;
long start = 0;
int timings[20];
int levels[20];
String ENCODE[] = {".-", "--."};
String LETTERS[] = {"A","Z"};
int current = 0;
int n_letters = 2;
bool check_letter = false;
bool is_idle = false;

void setup() {
  Serial.begin(9600);
  pinMode(DATA_PIN, INPUT);
  attachInterrupt(0, process_timing, CHANGE); 
}

void loop() {
  process_idle();
  if (check_letter){
    decode_letter();
    check_letter = false;
  }
}

void decode_letter(){
    if (timings[current-1] >= LETTER_SEP and levels[current-1] == !DATA_LEVEL) {
    String letter = "";
    for (int i = 0; i < current; i++){
      if (timings[i] == 1 and levels[i] == DATA_LEVEL){
        letter += ".";
      } else if (timings[i] == LETTER_SEP and levels[i] == DATA_LEVEL){
        letter += "-";
      }
    }
    current = 0;
    for (int iletter = 0; iletter < n_letters; iletter++){
      if(ENCODE[iletter] == letter) {
        Serial.println(LETTERS[iletter]);
      }
    }
  }
}

void process_timing() {
  if(not is_idle){
    timings[current] = round((millis()-start) / TU);
    levels[current] = !digitalRead(DATA_PIN);
    current++;
    check_letter = true;
  }
  start = millis();
  is_idle = false;
}

void process_idle() {
  if (round((millis()-start) / TU) >= IDLE_TIME and not is_idle){
    timings[current] = 7;
    levels[current] = 1;
    current++;
    start = millis();  
    check_letter = true;
    is_idle = true;
  }
}