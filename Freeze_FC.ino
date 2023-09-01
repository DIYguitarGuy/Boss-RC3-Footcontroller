#include <MIDI.h>

int mode = 0;
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
unsigned long button0Millis = 0;
unsigned long button1Millis = 0;
unsigned long button2Millis = 0;
unsigned long button3Millis = 0;
unsigned long sendMidi = 0;
unsigned long latchMillis = 0;
unsigned long sampleTime = 0;
unsigned long switchTime = 0;
unsigned long ledMillis = 0;
unsigned long slowTipMillis = 0;
unsigned long slowRingMillis = 0;
int lastbutton0State = 0;
int lastbutton1State = 0;
int lastbutton2State = 0;
int lastbutton3State = 0;
int rampCc0 = 20;                                           // CC number for controlling ramp on receiving device.
int rampCc1 = 21;                                           // CC number for controlling ramp on receiving device.
int rampCc2 = 22;                                           // CC number for controlling ramp on receiving device.
int polySustainCc = 30;                                     // CC number to switch the Poly Sustain bypass on the Line6 Helix
int tipSendCc = 40;                                         // CC number for controlling the Tips Send block bypass in Infinite mode
int tipReturnCc = 41;                                       // CC number for controlling the Tips Return block return level in Infinite mode
int ringSendCc = 42;                                        // CC number for controlling the Rings Send block bypass in Infinite mode
int ringReturnCc = 43;                                      // CC number for controlling the Rings Return block return level in Infinite mode
float rampVal0 = 0;
float rampVal1 = 0;
float rampVal2 = 0;
int rampRound0 = 0;
int rampRound1 = 0;
int rampRound2 = 0;
float rampOnSpeed = .1;                                     // Ramp on speed: enter value up to 127, decimals allowed.
int rampMax0 = 3;
int rampMax1 = 3;
int rampMax2 = 3;
int volCc0 = 23;                                            // CC number for controlling the Freezes volume.
int volCc1 = 24;                                            // CC number for controlling the Freezes volume.
int volCc2 = 25;                                            // CC number for controlling the Freezes volume.
float volVal0 = 127;
float volVal1 = 127;
float volVal2 = 127;
int volRound0 = 127;
int volRound1 = 127;
int volRound2 = 127;
float dropVolumeSpeed = .001;                               // Speed of the volume drop: enter value up to 127, decimals allowed.
int volMax0 = 3;
int volMax1 = 3;
int volMax2 = 3;
int tipReturnVal = 0;
int ringReturnVal = 0;
int tipReturnMax = 3;
int ringReturnMax = 3;
int midiTxChannel = 1;                                      // Midi channel of the device you want to control.
int midiRxChannel = 16;                                     // Midi channel this device wil listen to. 
int message1Sent = 5;
int slowAttack = 10;                                        // Attack speed of Slow Mode: Enter value between 1 and 127.
int slowDecay = 1;                                          // Decay speed of Slow Mode: Enter value between 1 and 127.
int tapTime0 = 500;
int tapTime1 = 0;
int layer = 0;
bool initTip = false;
bool initRing = false;
bool turnOffLeft = false;
bool turnOnLeft = false;
bool turnOffRight = false;
bool turnOnRight = false;
bool dropVolume0 = false;
bool dropVolume1 = false;
bool dropVolume2 = false;
bool slowTipOn = false;
bool slowTipOff = false;
bool slowTipReset = false;
bool slowRingOn = false;
bool slowRingOff = false;
bool slowRingReset = false;
bool latchTip = false;
bool slowTip = false;
bool latchRing = false;
bool slowRing = false;
bool sampling = false;
bool switchReturn = false;
bool stopAll = false;

MIDI_CREATE_DEFAULT_INSTANCE();

void setup() {
  MIDI.begin(midiRxChannel);
  MIDI.turnThruOn();
  MIDI.setThruFilterMode(3);
  //Serial.begin(9600);
  pinMode(tipPin, OUTPUT);
  digitalWrite(tipPin, HIGH);
  pinMode(ringPin, OUTPUT);
  digitalWrite(ringPin, HIGH);
  pinMode(button0Pin, INPUT_PULLUP);
  pinMode(button1Pin, INPUT_PULLUP);
  pinMode(button2Pin, INPUT_PULLUP);
  pinMode(button3Pin, INPUT_PULLUP);
  pinMode(led0, OUTPUT);
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  button0State = digitalRead(button0Pin);
  button1State = digitalRead(button1Pin);
  button2State = digitalRead(button2Pin);
  button3State = digitalRead(button3Pin);
  if (button0State == LOW && button1State == HIGH && button2State == HIGH && button3State == HIGH){
    mode = 1;
    digitalWrite(led0, HIGH);
    delay(100);
    digitalWrite(led0, LOW);
    delay(100);
    digitalWrite(led0, HIGH);
    delay(100);
    digitalWrite(led0, LOW);
    delay(100);
  }
  if (button1State == LOW && button0State == HIGH && button2State == HIGH && button3State == HIGH){
    mode = 2;
    digitalWrite(led1, HIGH);
    delay(100);
    digitalWrite(led1, LOW);
    delay(100);
    digitalWrite(led1, HIGH);
    delay(100);
    digitalWrite(led1, LOW);
    delay(100);
  }
  if (button2State == LOW && button0State == HIGH && button1State == HIGH && button3State == HIGH){
    mode = 3;
    digitalWrite(led2, HIGH);
    delay(100);
    digitalWrite(led2, LOW);
    delay(100);
    digitalWrite(led2, HIGH);
    delay(100);
    digitalWrite(led2, LOW);
    delay(100);
  }
  if (button3State == LOW && button0State == HIGH && button1State == HIGH && button2State == HIGH){
    mode = 4;
    digitalWrite(led3, HIGH);
    delay(100);
    digitalWrite(led3, LOW);
    delay(100);
    digitalWrite(led3, HIGH);
    delay(100);
    digitalWrite(led3, LOW);
    delay(100);
  }
  if (button0State == HIGH && button1State == HIGH && button2State == HIGH && button3State == HIGH){
    mode = 0;
    digitalWrite(led0, HIGH);
    digitalWrite(led1, HIGH);
    digitalWrite(led2, HIGH);
    digitalWrite(led3, HIGH);
    delay(100);
    digitalWrite(led0, LOW);
    digitalWrite(led1, LOW);
    digitalWrite(led2, LOW);
    digitalWrite(led3, LOW);
    delay(100);
    digitalWrite(led0, HIGH);
    digitalWrite(led1, HIGH);
    digitalWrite(led2, HIGH);
    digitalWrite(led3, HIGH);
    delay(100);
    digitalWrite(led0, LOW);
    digitalWrite(led1, LOW);
    digitalWrite(led2, LOW);
    digitalWrite(led3, LOW);
  }
}

void loop() {
  MIDI.read();
  unsigned long currentMillis = millis();
  button0State = digitalRead(button0Pin);
  button1State = digitalRead(button1Pin);
  button2State = digitalRead(button2Pin);
  button3State = digitalRead(button3Pin);

  if (MIDI.getChannel() == midiRxChannel && MIDI.getType() == 0xC0 && MIDI.getData1() <= 4 && MIDI.getData1() != mode){
    switch (mode){
      case 0:
        MIDI.sendControlChange (volCc0,127,midiTxChannel);
        MIDI.sendControlChange (volCc1,127,midiTxChannel);
        break;
      case 1:
        MIDI.sendControlChange (volCc0,127,midiTxChannel);
        MIDI.sendControlChange (volCc1,127,midiTxChannel);
        MIDI.sendControlChange (volCc2,127,midiTxChannel);
        MIDI.sendControlChange (rampCc0,0,midiTxChannel);
        MIDI.sendControlChange (rampCc1,0,midiTxChannel);
        MIDI.sendControlChange (rampCc2,0,midiTxChannel);
        break;
      case 3:
        MIDI.sendControlChange (polySustainCc,0,midiTxChannel);
        break;
      case 4:
        MIDI.sendControlChange (tipSendCc,0,midiTxChannel);
        MIDI.sendControlChange (ringSendCc,0,midiTxChannel);
        MIDI.sendControlChange (tipReturnCc,0,midiTxChannel);
        MIDI.sendControlChange (ringReturnCc,0,midiTxChannel);
        break;
    }
    ledMillis = currentMillis;
    mode =  MIDI.getData1();
    digitalWrite(tipPin, HIGH);
    digitalWrite(ringPin, HIGH);
    tipReturnVal = 0;
    ringReturnVal = 0;
    tipReturnMax = 3;
    ringReturnMax = 3;
    initTip = false;
    initRing = false;
    turnOffLeft = false;
    turnOnLeft = false;
    turnOffRight = false;
    turnOnRight = false;
    dropVolume0 = false;
    dropVolume1 = false;
    dropVolume2 = false;
    slowTipOn = false;
    slowTipOff = false;
    slowRingOn = false;
    slowRingOff = false;
    latchTip = false;
    slowTip = false;
    latchRing = false;
    slowRing = false;
    sampling = false;
    switchReturn = false;
    stopAll = false;
    layer = 0;
    volRound0 = 127;
    volRound1 = 127;
    volRound2 = 127;
    volVal0 = 127;
    volVal1 = 127;
    volVal2 = 127;
    rampMax0 = 3;
    rampMax1 = 3;
    rampMax2 = 3;
    volMax0 = 3;
    volMax1 = 3;
    volMax2 = 3;
  }
  if (currentMillis - ledMillis <= 100 || currentMillis - ledMillis >= 200 && currentMillis - ledMillis <= 300 || currentMillis - ledMillis >= 400 && currentMillis - ledMillis <= 500){
    switch (mode){
      case 0:
        digitalWrite(led0, HIGH);
        digitalWrite(led1, HIGH);
        digitalWrite(led2, HIGH);
        digitalWrite(led3, HIGH);
        break;
      case 1:
        digitalWrite(led0, HIGH);
        break;
      case 2:
        digitalWrite(led1, HIGH);
        break;
      case 3:
        digitalWrite(led2, HIGH);
        break;
      case 4:
        digitalWrite(led3, HIGH);
        break;
    }
  }
  if (currentMillis - ledMillis >= 100 && currentMillis - ledMillis <= 200 || currentMillis - ledMillis >= 300 && currentMillis - ledMillis <= 400|| currentMillis - ledMillis >= 500 && currentMillis - ledMillis <= 600){
    digitalWrite(led0, LOW);
    digitalWrite(led1, LOW);
    digitalWrite(led2, LOW);
    digitalWrite(led3, LOW);
  }

  if (rampVal0 <= 0) {
    rampVal0 = 0;
  }
  if (rampVal0 >= 127) {
    rampVal0 = 127;
  }
  if (rampVal1 <= 0) {
    rampVal1 = 0;
  }
  if (rampVal1 >= 127) {
    rampVal1 = 127;
  }
  if (rampVal2 <= 0) {
    rampVal2 = 0;
  }
  if (rampVal2 >= 127) {
    rampVal2 = 127;
  }
  if (volVal0 <= 0) {
    volVal0 = 0;
  }
  if (volVal0 >= 127) {
    volVal0 = 127;
  }
  if (volVal1 <= 0) {
    volVal1 = 0;
  }
  if (volVal1 >= 127) {
    volVal1 = 127;
  }
  if (volVal2 <= 0) {
    volVal2 = 0;
  }
  if (volVal2 >= 127) {
    volVal2 = 127;
  }
  if (mode == 0){                                                       //All Freeze Modes
    if (currentMillis - sendMidi >= 10){
      if (slowTipOn){
        MIDI.sendControlChange (volCc0,volVal0,midiTxChannel);
        MIDI.sendControlChange (volCc0,volVal0,midiTxChannel);
        MIDI.sendControlChange (volCc0,volVal0,midiTxChannel);
        volVal0 = volVal0+slowAttack;
        if (volVal0 >= 127){
          volMax0++;
        }
        if (volMax0 >= 3){
          slowTipOn = false;
        }
      }
      if (slowTipOff){
        MIDI.sendControlChange (volCc0,volVal0,midiTxChannel);
        MIDI.sendControlChange (volCc0,volVal0,midiTxChannel);
        MIDI.sendControlChange (volCc0,volVal0,midiTxChannel);
        volVal0 = volVal0-slowDecay;
        if (volVal0 <= 0){
          volMax0++;
        }
        if (volMax0 >= 3){
          slowTipOff = false;
          slowTip = false;
          latchTip = false;
          digitalWrite(tipPin, HIGH);
          digitalWrite(led0, LOW);
          digitalWrite(led2, LOW);
          slowTipMillis = currentMillis;
          slowTipReset = true;
        }
      }
      if (slowTipReset && currentMillis - slowTipMillis > 50){
        MIDI.sendControlChange (volCc0,127,midiTxChannel);
        MIDI.sendControlChange (volCc0,127,midiTxChannel);
        MIDI.sendControlChange (volCc0,127,midiTxChannel);
        slowTipReset = false;
      }
      if (slowRingOn){
        MIDI.sendControlChange (volCc1,volVal1,midiTxChannel);
        MIDI.sendControlChange (volCc1,volVal1,midiTxChannel);
        MIDI.sendControlChange (volCc1,volVal1,midiTxChannel);
        volVal1 = volVal1+slowAttack;
        if (volVal1 >= 127){
          volMax1++;
        }
        if (volMax1 >= 3){
          slowRingOn = false;
        }
      }
      if (slowRingOff){
        MIDI.sendControlChange (volCc1,volVal1,midiTxChannel);
        MIDI.sendControlChange (volCc1,volVal1,midiTxChannel);
        MIDI.sendControlChange (volCc1,volVal1,midiTxChannel);
        volVal1 = volVal1-slowDecay;
        if (volVal1 <= 0){
          volMax1++;
        }
        if (volMax1 >= 3){
          slowRingOff = false;
          slowRing = false;
          latchRing = false;
          digitalWrite(ringPin, HIGH);
          digitalWrite(led1, LOW);
          digitalWrite(led3, LOW);
          slowRingMillis = currentMillis;
          slowRingReset = true;
        }
      }
      if (slowRingReset && currentMillis - slowRingMillis > 50){
        MIDI.sendControlChange (volCc1,127,midiTxChannel);
        MIDI.sendControlChange (volCc1,127,midiTxChannel);
        MIDI.sendControlChange (volCc1,127,midiTxChannel);
        slowRingReset = false;
      }
      sendMidi = currentMillis;
    }
  
    if (currentMillis - latchMillis >= 20){
      if (latchTip){
        digitalWrite(tipPin, LOW);
        digitalWrite(led2, HIGH);
      }
      if (latchRing){
        digitalWrite(ringPin, LOW);
        digitalWrite(led3, HIGH);
      }
      latchMillis = currentMillis;
    }
  
    // Button 2: Latch Tip
    if ((currentMillis - button2Millis >= 50) && (button2State != lastbutton2State)) {
      if (button2State == LOW) {
        if (!slowTip|| latchTip){
          digitalWrite(tipPin, HIGH);
          MIDI.sendControlChange (volCc0,127,midiTxChannel);
          MIDI.sendControlChange (volCc0,127,midiTxChannel);
          MIDI.sendControlChange (volCc0,127,midiTxChannel);
        }
        latchTip = true;
        tapTip();
      }
      button2Millis = currentMillis;
      lastbutton2State = button2State;
    }
  
    // Button 3: Latch Ring
    if ((currentMillis - button3Millis >= 50) && (button3State != lastbutton3State)) {
      if (button3State == LOW) {
        if (!slowRing || latchRing){
          digitalWrite(ringPin, HIGH);
          MIDI.sendControlChange (volCc1,127,midiTxChannel);
          MIDI.sendControlChange (volCc1,127,midiTxChannel);
          MIDI.sendControlChange (volCc1,127,midiTxChannel);
        }
        latchRing = true;
        tapRing();
      }
      button3Millis = currentMillis;
      lastbutton3State = button3State;
    }
  
    // Button 0: Slow Tip
    if ((currentMillis - button0Millis >= 50) && (button0State != lastbutton0State)) {
      if (button0State == LOW) {
        initTip = true;
        digitalWrite(led0, HIGH);
        digitalWrite(tipPin, LOW);
        if (!latchTip){
          volMax0 = 0;
          slowTip = true;
          slowTipOn = true;
        }
        else{
          volVal0 = 127;
          volMax0 = 0;
          slowTipOn = false;
          slowTipOff = true;
        }
      }
      else if (initTip && !latchTip){
        volMax0 = 0;
        slowTipOn = false;
        slowTipOff = true;
      }
      button0Millis = currentMillis;
      lastbutton0State = button0State;
    }
  
    // Button 1: Slow Ring
    if ((currentMillis - button1Millis >= 50) && (button1State != lastbutton1State)) {
      if (button1State == LOW) {
        initRing = true;
        digitalWrite(led1, HIGH);
        digitalWrite(ringPin, LOW);
        if (!latchRing){
          volMax1 = 0;
          slowRing = true;
          slowRingOn = true;
        }
        else{
          volVal1 = 127;
          volMax1 = 0;
          slowRingOn = false;
          slowRingOff = true;
        }
      }
      else if (initRing && !latchRing){
        volMax1 = 0;
        slowRingOn = false;
        slowRingOff = true;
      }
      button1Millis = currentMillis;
      lastbutton1State = button1State;
    }
  }

  if (mode == 1){                                                                       // Freeze & mute input for solos
    if (currentMillis - sendMidi >= 10){
      if (turnOnLeft) {
        rampRound0 = round(rampVal0);
        rampRound1 = round(rampVal1);
        MIDI.sendControlChange (rampCc0,rampRound0,midiTxChannel);
        MIDI.sendControlChange (rampCc1,rampRound1,midiTxChannel);
        if (rampRound0 >= 127){
          rampMax0++;
        }
        if (rampMax0 >=3){
          turnOnLeft = false;
        }
      }
      if (turnOffLeft) {
        rampRound0 = round(rampVal0);
        rampRound1 = round(rampVal1);
        MIDI.sendControlChange (rampCc0,rampRound0,midiTxChannel);
        MIDI.sendControlChange (rampCc1,rampRound1,midiTxChannel);
        volRound0 = round(volVal0);
        volRound1 = round(volVal1);
        MIDI.sendControlChange (volCc0,volRound0,midiTxChannel);
        MIDI.sendControlChange (volCc1,volRound1,midiTxChannel);
        if (rampRound0 <= 0){
          rampMax0++;
        }
        if (rampMax0 >=3){
          turnOffLeft = false;
        }
      }
      if (turnOnRight) {
        rampRound0 = round(rampVal0);
        rampRound2 = round(rampVal2);
        MIDI.sendControlChange (rampCc0,rampRound0,midiTxChannel);
        MIDI.sendControlChange (rampCc2,rampRound2,midiTxChannel);
        if (rampRound2 >= 127){
          rampMax2++;
        }
        if (rampMax2 >=3){
          turnOnRight = false;
        }
      }
      if (turnOffRight) {
        rampRound0 = round(rampVal0);
        rampRound2 = round(rampVal2);
        MIDI.sendControlChange (rampCc0,rampRound0,midiTxChannel);
        MIDI.sendControlChange (rampCc2,rampRound2,midiTxChannel);
        volRound0 = round(volVal0);
        MIDI.sendControlChange (volCc0,volRound0,midiTxChannel);
        if (rampRound2 <= 0){
          rampMax2++;
        }
        if (rampMax2 >=3){
          turnOffRight = false;
        }
      }
      if (dropVolume0) {
        volRound0 = round(volVal0);
        MIDI.sendControlChange (volCc0,volRound0,midiTxChannel);
        if (volRound0 <= 0){
          volMax0++;
        }
        if (volMax0 >=3){
          dropVolume0 = false;
        }
      }
      if (dropVolume1) {
        volRound1 = round(volVal1);
        MIDI.sendControlChange (volCc1,volRound1,midiTxChannel);
        if (volRound1 <= 0){
          volMax1++;
        }
        if (volMax1 >=3){
          dropVolume1 = false;
        }
      }
      if (dropVolume2) {
        volRound2 = round(volVal2);
        MIDI.sendControlChange (volCc2,volRound2,midiTxChannel);
        if (volRound2 <= 0){
          volMax2++;
        }
        if (volMax2 >=3){
          dropVolume2 = false;
        }
      }
      sendMidi = currentMillis;
    }
  
    // Button 2: Momentary Freeze Left
    if ((currentMillis - button2Millis >= 50) && (button2State != lastbutton2State)) {
      if (button2State == LOW) {
        digitalWrite(led2, HIGH);
        initTip = true;
      }
      else{
        digitalWrite(led2, LOW);
      }
      if (initTip) {
        if (digitalRead(tipPin) == 1){
          sample();
          turnOnLeft = true;
          turnOffLeft = false;
          dropVolume0 = true;
          dropVolume1 = true;
          rampMax0 = 0;
          rampMax1 = 0;
          volMax0 = 0;
          volMax1 = 0;
        }
        else {
          turnOffLeft = true;
          turnOnLeft = false;
          dropVolume0 = false;
          dropVolume1 = false;
          rampMax0 = 0;
          rampMax1 = 0;
          volMax0 = 0;
          volMax1 = 0;
        }
      }
      button2Millis = currentMillis;
      lastbutton2State = button2State;
    }
  
    // Button 3: Momentary Freeze Right
    if ((currentMillis - button3Millis >= 50) && (button3State != lastbutton3State)) {
      if (button3State == LOW) {
        digitalWrite(led3, HIGH);
        initRing = true;
      }
      else{
        digitalWrite(led3, LOW);
      }
      if (initRing) {
        if (digitalRead(tipPin) == 1){
          sample();
          turnOnRight = true;
          turnOffRight = false;
          dropVolume0 = true;
          rampMax2 = 0;
          volMax0 = 0;
        }
        else {
          turnOffRight = true;
          turnOnRight = false;
          dropVolume0 = false;
          rampMax2 = 0;
          volMax0 = 0;
        }
      }
      button3Millis = currentMillis;
      lastbutton3State = button3State;
    }
  
    // Button 0: Latching Freeze Left
    if ((currentMillis - button0Millis >= 50) && (button0State != lastbutton0State)) {
      if ((button0State == LOW) && (digitalRead(tipPin) == 1) && (digitalRead(led2) == LOW)) {
        sample();
        digitalWrite(led0, HIGH);
        rampMax0 = 0;
        turnOnLeft = true;
        turnOffLeft = false;
        dropVolume0 = true;
        dropVolume1 = true;
        volMax0 = 0;
        volMax1 = 0;
      }
      else if ((button0State == LOW) && (digitalRead(tipPin) == LOW) && (digitalRead(led2) == LOW)) {
        digitalWrite(led0, LOW);
        rampRound0 = 0;
        rampMax0 = 0;
        turnOffLeft = true;
        turnOnLeft = false;
        dropVolume0 = false;
        dropVolume1 = false;
        volMax0 = 0;
        volMax1 = 0;
      }
      button0Millis = currentMillis;
      lastbutton0State = button0State;
    }
  
    // Button 1: Latching Freeze Right
    if ((currentMillis - button1Millis >= 50) && (button1State != lastbutton1State)) {
      if ((button1State == LOW) && (digitalRead(tipPin) == 1) && (digitalRead(led3) == LOW)) {
        sample();
        digitalWrite(led1, HIGH);
        rampMax2 = 0;
        turnOnRight = true;
        turnOffRight = false;
        dropVolume0 = true;
        volMax0 = 0;
      }
      else if ((button1State == LOW) && (digitalRead(tipPin) == LOW) && (digitalRead(led3) == LOW)) {
        digitalWrite(led1, LOW);
        rampRound0 = 0;
        rampRound2 = 0;
        rampMax2 = 0;
        turnOffRight = true;
        turnOnRight = false;
        dropVolume0 = false;
        volMax0 = 0;
      }
      button1Millis = currentMillis;
      lastbutton1State = button1State;
    }
  
    if (turnOnLeft) {
      turnOffLeft = false;
      digitalWrite(tipPin, 0);
      digitalWrite(ringPin, 0);
      if (currentMillis - sampleTime >= 50){
        rampVal0 = rampVal0+rampOnSpeed;
        rampVal1 = rampVal1+rampOnSpeed;
      }
    }
    
    if (turnOffLeft) {
      turnOnLeft = false;
      rampVal0 = 0;
      rampVal1 = 0;
      volVal0 = 127;
      volVal1 = 127;
      digitalWrite(tipPin, 1);
      digitalWrite(ringPin, 1);
    }
  
    if (turnOnRight) {
      turnOffRight = false;
      if (currentMillis - sampleTime >= 50){
        rampVal0 = rampVal0+rampOnSpeed;
        rampVal2 = rampVal2+rampOnSpeed;
      }
      digitalWrite(tipPin, 0);
    }
    
    if (turnOffRight) {
      turnOnRight = false;
      rampVal0 = 0;
      rampVal2 = 0;
      volVal0 = 127;
      digitalWrite(tipPin, 1);
    }
  
    if (dropVolume0) {
      volVal0 = volVal0-dropVolumeSpeed;
    }
    if (dropVolume1) {
      volVal1 = volVal1-dropVolumeSpeed;
    }
    if (dropVolume2) {
      volVal2 = volVal2-dropVolumeSpeed;
    }
  }

  if (mode == 2){                                                                                 // Dual Layer
    // Button 2: Stop
    if ((currentMillis - button2Millis >= 50) && (button2State != lastbutton2State)) {
      if (button2State == LOW) {
        digitalWrite(led2, HIGH);
        digitalWrite (tipPin, HIGH);
        digitalWrite (ringPin, HIGH);
        layer = 0;
      }
      else{
        digitalWrite(led2, LOW);
      }
      button2Millis = currentMillis;
      lastbutton2State = button2State;
    }
  
    // Button 3: Auto next
    if ((currentMillis - button3Millis >= 50) && (button3State != lastbutton3State)) {
      if (button3State == LOW) {
        initTip = true;
        digitalWrite(led3, HIGH);
        layer++;
        if (layer > 2){
          layer = 1;
        }
        if (layer == 1){
          digitalWrite (tipPin, HIGH);
        }
        if (layer == 0 || layer == 2){
          digitalWrite (ringPin, HIGH);
        }
      }
      else if (initTip) {
        digitalWrite(led3, LOW);
        if (layer == 1){
          digitalWrite (tipPin, LOW);
        }
        if (layer == 0 || layer == 2){
          digitalWrite (ringPin, LOW);
        }
      }
      button3Millis = currentMillis;
      lastbutton3State = button3State;
    }
  
    // Button 0: Manual change layer Tip
    if ((currentMillis - button0Millis >= 50) && (button0State != lastbutton0State)) {
      if ((button0State == LOW)) {
        initTip = true;
        layer = 1;
        digitalWrite (tipPin, HIGH);
        digitalWrite (led0, HIGH);
      }
      else if (initTip) {
        digitalWrite (tipPin, LOW);
        digitalWrite (led0, LOW);
      }
      button0Millis = currentMillis;
      lastbutton0State = button0State;
    }
  
    // Button 1: Manual change layer Ring
    if ((currentMillis - button1Millis >= 50) && (button1State != lastbutton1State)) {
      if (button1State == LOW){
        initRing = true;
        layer = 2;
        digitalWrite (ringPin, HIGH);
        digitalWrite (led1, HIGH);
      }
      else if (initRing) {
        digitalWrite (ringPin, LOW);
        digitalWrite (led1, LOW);
      }
      button1Millis = currentMillis;
      lastbutton1State = button1State;
    }
  }

  if (mode == 3){                                                                         // Triple Layer
    // Botton 2: Manual change layer Poly Sustain
    if ((currentMillis - button2Millis >= 50) && (button2State != lastbutton2State)) {
      if (button2State == LOW) {
        initTip = true;
        layer = 3;
        MIDI.sendControlChange (polySustainCc,0,midiTxChannel);
        MIDI.sendControlChange (polySustainCc,0,midiTxChannel);
        MIDI.sendControlChange (polySustainCc,0,midiTxChannel);
        digitalWrite (led2, HIGH);
      }
      else if (initTip) {
        MIDI.sendControlChange (polySustainCc,127,midiTxChannel);
        MIDI.sendControlChange (polySustainCc,127,midiTxChannel);
        MIDI.sendControlChange (polySustainCc,127,midiTxChannel);
        digitalWrite (led2, LOW);
      }
      button2Millis = currentMillis;
      lastbutton2State = button2State;
    }
  
    // Button 3: Auto next, double tap for stop
    if ((currentMillis - button3Millis >= 50) && (button3State != lastbutton3State)) {
      if (button3State == LOW) {
        initTip = true;
        digitalWrite(led3, HIGH);
        layer++;
        tap();
        if (layer > 3){
          layer = 1;
        }
        if (layer == 1){
          digitalWrite (tipPin, HIGH);
        }
        if (layer == 2){
          digitalWrite (ringPin, HIGH);
        }
        if (layer == 3){
          MIDI.sendControlChange (polySustainCc,0,midiTxChannel);
          MIDI.sendControlChange (polySustainCc,0,midiTxChannel);
          MIDI.sendControlChange (polySustainCc,0,midiTxChannel);
        }
      }
      else if (initTip) {
        digitalWrite(led3, LOW);
        if (layer == 1){
          digitalWrite (tipPin, LOW);
        }
        if (layer == 2){
          digitalWrite (ringPin, LOW);
        }
        if (layer == 3){
          MIDI.sendControlChange (polySustainCc,127,midiTxChannel);
          MIDI.sendControlChange (polySustainCc,127,midiTxChannel);
          MIDI.sendControlChange (polySustainCc,127,midiTxChannel);
        }
      }
      button3Millis = currentMillis;
      lastbutton3State = button3State;
    }
  
    // Button 0: Manual change layer Tip
    if ((currentMillis - button0Millis >= 50) && (button0State != lastbutton0State)) {
      if ((button0State == LOW)) {
        initTip = true;
        layer = 1;
        digitalWrite (tipPin, HIGH);
        digitalWrite (led0, HIGH);
      }
      else if (initTip) {
        digitalWrite (tipPin, LOW);
        digitalWrite (led0, LOW);
      }
      button0Millis = currentMillis;
      lastbutton0State = button0State;
    }
  
    // Button 1: Manual change layer Ring
    if ((currentMillis - button1Millis >= 50) && (button1State != lastbutton1State)) {
      if (button1State == LOW){
        initRing = true;
        layer = 2;
        digitalWrite (ringPin, HIGH);
        digitalWrite (led1, HIGH);
      }
      else if (initRing) {
        digitalWrite (ringPin, LOW);
        digitalWrite (led1, LOW);
      }
      button1Millis = currentMillis;
      lastbutton1State = button1State;
    }
  }

  if (mode == 4){                                                                           // Infinite mode
    // Button 2: Stop all
    if ((currentMillis - button2Millis >= 50) && (button2State != lastbutton2State)) {
      if (button2State == LOW) {
        digitalWrite(led2, HIGH);
        digitalWrite (tipPin, HIGH);
        digitalWrite (ringPin, HIGH);
        layer = 0;
        stopAll = true;
        tipReturnMax = 0;
        ringReturnMax = 0;
        MIDI.sendControlChange (tipSendCc,0,midiTxChannel);
        MIDI.sendControlChange (ringSendCc,0,midiTxChannel);
      }
      else{
        digitalWrite(led2, LOW);
      }
      button2Millis = currentMillis;
      lastbutton2State = button2State;
    }
  
    // Button 3: Auto next
    if ((currentMillis - button3Millis >= 50) && (button3State != lastbutton3State)) {
      if (button3State == LOW) {
        initTip = true;
        digitalWrite(led3, HIGH);
        layer++;
        tipReturnMax = 0;
        ringReturnMax = 0;
        if (layer > 2){
          layer = 1;
        }
        if (layer == 1){
          MIDI.sendControlChange (tipSendCc,127,midiTxChannel);
          sample();
        }
        if (layer == 2){
          MIDI.sendControlChange (ringSendCc,127,midiTxChannel);
          sample();
        }
      }
      else if (initTip) {
        digitalWrite(led3, LOW);
      }
      button3Millis = currentMillis;
      lastbutton3State = button3State;
    }
  
    // Button 0: Manual change layer Tip
    if ((currentMillis - button0Millis >= 50) && (button0State != lastbutton0State)) {
      if ((button0State == LOW)) {
        initTip = true;
        layer = 1;
        digitalWrite (tipPin, HIGH);
        digitalWrite (led0, HIGH);
      }
      else if (initTip) {
        digitalWrite (tipPin, LOW);
        digitalWrite (led0, LOW);
      }
      button0Millis = currentMillis;
      lastbutton0State = button0State;
    }
  
    // Button 1: Manual change layer Ring
    if ((currentMillis - button1Millis >= 50) && (button1State != lastbutton1State)) {
      if (button1State == LOW){
        initRing = true;
        layer = 2;
        digitalWrite (ringPin, HIGH);
        digitalWrite (led1, HIGH);
      }
      else if (initRing) {
        digitalWrite (ringPin, LOW);
        digitalWrite (led1, LOW);
      }
      button1Millis = currentMillis;
      lastbutton1State = button1State;
    }
  
    if (currentMillis - sampleTime >= 200){
      if (layer == 1 && sampling){
        digitalWrite (tipPin, LOW);
      }
      if (layer == 2 && sampling){
        digitalWrite (ringPin, LOW);
      }
    }
    
    if (currentMillis - sampleTime >= 400){
      if (layer == 1 && sampling){
        //digitalWrite (ringPin, HIGH);
        MIDI.sendControlChange (tipSendCc,0,midiTxChannel);
        sampling = false;
        switchReturn = true;
      }
      if (layer == 2 && sampling){
        //digitalWrite (tipPin, HIGH);
        MIDI.sendControlChange (ringSendCc,0,midiTxChannel);
        sampling = false;
        switchReturn = true;
      }
    }
  
    if (currentMillis - sendMidi >= 10){
      if (switchReturn){
        if (layer == 1){
          MIDI.sendControlChange (ringReturnCc,ringReturnVal,midiTxChannel);
          MIDI.sendControlChange (tipReturnCc,tipReturnVal,midiTxChannel);
          ringReturnVal = ringReturnVal/1.6;
          tipReturnVal = tipReturnVal*1.6;
          if (ringReturnVal <= 1){
            ringReturnVal = 0;
            ringReturnMax++;
          }
          if (tipReturnVal >= 127){
            tipReturnVal = 127;
            tipReturnMax++;
          }
          if (ringReturnMax >= 3  && tipReturnMax >= 3){
            switchReturn = false;
            digitalWrite (ringPin, HIGH);
          }
        }
        if (layer == 2) {
          MIDI.sendControlChange (tipReturnCc,tipReturnVal,midiTxChannel);
          MIDI.sendControlChange (ringReturnCc,ringReturnVal,midiTxChannel);
          tipReturnVal = tipReturnVal/1.6;
          ringReturnVal = ringReturnVal*1.6;
          if (tipReturnVal <= 1){
            tipReturnVal = 0;
            tipReturnMax++;
          }
          if (ringReturnVal >= 127){
            ringReturnVal = 127;
            ringReturnMax++;
          }
          if (tipReturnMax >= 3 && ringReturnMax >= 3){
            switchReturn = false;
            digitalWrite (tipPin, HIGH);
          }
        }
        round(ringReturnVal);
        round(tipReturnVal);
      }
      if (stopAll){
        if (tipReturnVal >= 0){
          MIDI.sendControlChange (tipReturnCc,tipReturnVal,midiTxChannel);
          tipReturnVal = tipReturnVal-5;
        }
        if (ringReturnVal >= 0){
          MIDI.sendControlChange (ringReturnCc,ringReturnVal,midiTxChannel);
          ringReturnVal = ringReturnVal-5;
        }
        if (tipReturnVal <= 0){
          tipReturnVal = 0;
          tipReturnMax++;
        }
        if (ringReturnVal <= 0){
          ringReturnVal = 0;
          ringReturnMax++;
        }
        if (tipReturnMax >= 3 && ringReturnMax >= 3){
          switchReturn = false;
          stopAll = false;
        }
      }
      sendMidi = currentMillis;
    }
  }
}

void tapTip(){
  unsigned long currentMillis = millis();
  if (initTip){
    tapTime1 = tapTime0;
    tapTime0 = currentMillis;
    if (tapTime0 - tapTime1 <= 500){
      latchTip = false;
      digitalWrite(tipPin, HIGH);
      digitalWrite(led2, LOW);
      digitalWrite(led0, LOW);
    }
  }
  initTip = true;
}

void tapRing(){
  unsigned long currentMillis = millis();
  if (initRing){
    tapTime1 = tapTime0;
    tapTime0 = currentMillis;
    if (tapTime0 - tapTime1 <= 500){
      latchRing = false;
      digitalWrite(ringPin, HIGH);
      digitalWrite(led3, LOW);
      digitalWrite(led1, LOW);
    }
  }
  initRing = true;
}

void sample() {
  unsigned long currentMillis = millis();
  sampleTime = currentMillis;
  switchTime = currentMillis;
  sampling = true;
  if (layer == 1){
    tipReturnVal = 2;
  }
  if (layer == 2) {
    ringReturnVal = 2;
  }
}

void tap(){
  unsigned long currentMillis = millis();
  tapTime1 = tapTime0;
  tapTime0 = currentMillis;
  if (tapTime0 - tapTime1 <= 500){
    digitalWrite (tipPin, HIGH);
    digitalWrite (ringPin, HIGH);
    MIDI.sendControlChange (polySustainCc,0,midiTxChannel);
    MIDI.sendControlChange (polySustainCc,0,midiTxChannel);
    MIDI.sendControlChange (polySustainCc,0,midiTxChannel);
    layer = 0;
  }
}
