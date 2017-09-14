unsigned long timex = 0;
const byte ledPin = 13;
const byte interruptPin = 2;
volatile byte state = LOW;
volatile byte flag = 0;
int oldtime = 0;

void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(interruptPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPin), blink, CHANGE);
  Serial.begin(9600);
}


void loop() {
  /*Serial.print("Time: "); 
  time = millis();
  Serial.println(time); */
  delay(10);
  digitalWrite(ledPin, state);
   if (flag == 1)
  {
    flag = 0;
   
    Serial.print("Time: ");
    int timex = millis();
    int newtime = timex - oldtime;
    Serial.println(newtime);
    oldtime = timex;

  }
  
 
}

void blink() {
  state = !state; 
  flag = 1;
}


