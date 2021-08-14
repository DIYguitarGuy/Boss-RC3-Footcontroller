#include <MIDI.h>

int button0Pin = 2;
int button1Pin = 4;
int button2Pin = 7;
int button3Pin = 12;
int led0 = 3;
int led1 = 5;
int led2 = 6;
int led3 = 9;
int tipPin = A0;
int ringPin =  A1;
int button0State = 0;
int button1State = 0;
int button2State = 0;
int button3State = 0;
int rxPin = 0;
unsigned long ringMillis = 0;
unsigned long shiftUpMillis = 0;
unsigned long shiftDownMillis = 0;
unsigned long button0Millis = 0;
unsigned long button1Millis = 0;
unsigned long button2Millis = 0;
unsigned long button3Millis = 0;
int lastbutton0State = 0;
int lastbutton1State = 0;
int lastbutton2State = 0;
int lastbutton3State = 0;
int ringTime = 0;
int shiftingUp = 0;
int shiftingDown = 0;
int tapMidi = 0;
int tapped = 0;
int counter = 0;
byte midi_start = 0xfa;
byte midi_stop = 0xfc;
byte midi_clock = 0xf8;
byte midi_continue = 0xfb;
byte data;

void setup() {
  Serial.begin(31250);
  pinMode(tipPin, OUTPUT);
  pinMode(ringPin, OUTPUT);
  pinMode(button0Pin, INPUT_PULLUP);
  pinMode(button1Pin, INPUT_PULLUP);
  pinMode(button2Pin, INPUT_PULLUP);
  pinMode(button3Pin, INPUT_PULLUP);
  pinMode(led0, OUTPUT);
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  digitalWrite(led0, HIGH);
  delay(200);
  digitalWrite(led0, LOW);
  digitalWrite(led1, HIGH);
  delay(200);
  digitalWrite(led1, LOW);
  digitalWrite(led2, HIGH);
  delay(200);
  digitalWrite(led2, LOW);
  digitalWrite(led3, HIGH);
  delay(200);
  digitalWrite(led3, LOW);
}

void loop() {
  unsigned long currentMillis = millis();
  button0State = digitalRead(button0Pin);
  button1State = digitalRead(button1Pin);
  button2State = digitalRead(button2Pin);
  button3State = digitalRead(button3Pin);
  
//Stop/manual tap tempo button
  
  if ((tapMidi == 0) && (currentMillis - button2Millis >= 50) && (button2State != lastbutton2State)) {
    if (button2State == LOW) {
      digitalWrite(tipPin, HIGH);
      digitalWrite(led2, HIGH);
    }
    else {
      digitalWrite(tipPin, LOW);
      digitalWrite(led2, LOW);
    }
    button2Millis = currentMillis;
    lastbutton2State = button2State;
  }

//Convert incoming MIDI to tap tempo

  if ((data == midi_clock) && (tapMidi == 0) && (currentMillis - button0Millis >= 50) && (button0State != lastbutton0State)) {
    if (button0State == LOW) {
      tapMidi = 1;
    }
    else {
      tapMidi = 0;
    }
    button0Millis = currentMillis;
    lastbutton0State = button0State;
  }
  if(Serial.available() > 0) {
    data = Serial.read();
    Serial.write(data);
    if(data == midi_clock) {
      Sync();
    }
    if(data == midi_start || data == midi_stop){
      counter = 23;
    }
  }
  if(tapped >= 5) {
    tapped = 0;
    tapMidi = 0;
    digitalWrite(tipPin, LOW);
  }
  
//Shift up by pressing button1
  if ((shiftingDown == 0) && (currentMillis - button1Millis >= 50) && (button1State != lastbutton1State)) {
    if (button1State == LOW) {
      shiftingUp = 1;
      shiftUp();
    }
    else {
      digitalWrite(ringPin, LOW);
      digitalWrite(led1, LOW);
      shiftingUp = 0;
    }
    button1Millis = currentMillis;
    lastbutton1State = button1State;
  }
  if ((shiftingUp == 1) && (currentMillis - shiftUpMillis >= 50) && (currentMillis - shiftUpMillis <= 2000)){
    digitalWrite(ringPin, LOW);
  }
  if ((shiftingUp >= 2) && (currentMillis - shiftUpMillis >= 50) && (currentMillis - shiftUpMillis <= 500)){
    digitalWrite(ringPin, LOW);
  }
  if ((shiftingUp == 1) && (currentMillis - shiftUpMillis >= 2000)){
    shiftUp();
    shiftingUp = 2;
  }
  if ((shiftingUp > 1) && (currentMillis - shiftUpMillis >= 500)){
    shiftUp();
  }
  
//Stop shifting down by pressing button1
  if ((shiftingDown == 1) && (currentMillis - button1Millis >= 50) && (button1State != lastbutton1State)) {
    if (button1State == LOW) {
      digitalWrite(ringPin, LOW);
      digitalWrite(led3, LOW);
      shiftingDown = 0;
      ringTime = 0;
    }
    button1Millis = currentMillis;
    lastbutton1State = button1State;
  }

//Shift down by pressing button 3
  if ((shiftingUp == 0) && (currentMillis - button3Millis >= 50) && (button3State != lastbutton3State)) {
    if (button3State == LOW) {
      digitalWrite(ringPin, HIGH);
      digitalWrite(led3, HIGH);
      shiftDown();
    }
    else if (currentMillis - shiftDownMillis <= 2000){
      shiftingDown = 1;
      ringTime = ringTime + 500;
    }
    else {
      digitalWrite(ringPin, LOW);
      digitalWrite(led3, LOW);
    }
    button3Millis = currentMillis;
    lastbutton3State = button3State;
  }
  if (shiftingDown == 1) {
    if ((currentMillis - ringMillis >= 1500 + ringTime) && (button3State == HIGH)){
      digitalWrite(ringPin, LOW);
      digitalWrite(led3, LOW);
      shiftingDown = 0;
      ringMillis = currentMillis;
    }
  }
  if (shiftingDown == 0) {
    ringMillis = currentMillis;
    ringTime = 0;
  }
}

void Sync() {
  if(counter == 23) {
    counter = 0;
    if(tapMidi == 0) {
      digitalWrite(led0, HIGH);
    }
    else if(tapMidi == 1) {
      digitalWrite(tipPin, HIGH);
      digitalWrite(led0, LOW);
      tapped++;
    }
  }
  else if(counter < 23) {
    counter = counter + 1;
    if(tapMidi == 0) {
      digitalWrite(led0, LOW);
    }
    else if(tapMidi == 1) {
      digitalWrite(tipPin, LOW);
      digitalWrite(led0, HIGH);
    }
  }
  else {
    counter = counter + 1;
  }
}

void shiftUp() {
  unsigned long currentMillis = millis();
  digitalWrite(led1, HIGH);
  digitalWrite(ringPin, HIGH);
  shiftUpMillis = currentMillis;
}

void shiftDown() {
  unsigned long currentMillis = millis();
  shiftDownMillis = currentMillis;
}
