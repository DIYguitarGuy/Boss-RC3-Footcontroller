#include <SPI.h>
#include <MIDI.h>
byte address = 0x11;
byte midi_start = 0xfa;
byte midi_stop = 0xfc;
byte midi_clock = 0xf8;
byte midi_continue = 0xfb;
byte data;
int CS = 10;
int i = 0;
float val = 0;
float changeRate = 3.2;
float factor = 2.68;
bool reverse = false;
bool reversed = false;
bool rise = true;
int button0Pin = 2;
int button1Pin = 4;
int button2Pin = 7;
int button3Pin = 12;
int led0 = 3;
int led1 = 5;
int led2 = 6;
int led3 = 9;
int rxPin = 0;
int counter = 0;
int cycleDivision = 1;
int waveShape = 0;
int beat = 0;
int button0State = 0;
int button1State = 0;
int button2State = 0;
int button3State = 0;
int lastbutton0State = 0;
int lastbutton1State = 0;
int lastbutton2State = 0;
int lastbutton3State = 0;
unsigned long button0Millis = 0;
unsigned long button1Millis = 0;
unsigned long button2Millis = 0;
unsigned long button3Millis = 0;
unsigned long updateMillis = 0;
unsigned long clockMillis = 0;

void setup() {
  Serial.begin(31250);
  pinMode (CS, OUTPUT);
  SPI.begin();
  digitalPotWrite(0x00);
  pinMode(button0Pin, INPUT_PULLUP);
  pinMode(button1Pin, INPUT_PULLUP);
  pinMode(button2Pin, INPUT_PULLUP);
  pinMode(button3Pin, INPUT_PULLUP);
  pinMode(led0, OUTPUT);
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
}

void loop() {
  unsigned long currentMillis = millis();
  button0State = digitalRead(button0Pin);
  button1State = digitalRead(button1Pin);
  button2State = digitalRead(button2Pin);
  button3State = digitalRead(button3Pin);
  float sensorValue = analogRead(A5);

  //Behaviour of the buttons: selecting waveshape and reversing waveshape

  if ((currentMillis - button0Millis >= 50) && (button0State != lastbutton0State)) {
    if (button0State == HIGH && reversed == false) {
      waveShape = 0;
    }
    button0Millis = currentMillis;
    lastbutton0State = button0State;
    reversed = false;
  }
  if ((currentMillis - button1Millis >= 50) && (button1State != lastbutton1State)) {
    if ((button0State == HIGH) && button1State == LOW) {
      if (waveShape == 1) {
        if (cycleDivision < 4) {
          cycleDivision = cycleDivision * 2;
        }
        else {
          cycleDivision = 1;
        }
      }
      else {
        waveShape = 1;
      }
    }
    if ((button0State == LOW) && (button1State == LOW)) {
      waveShape = 1;
      reverse = !reverse;
      reversed = true;
    }
    button1Millis = currentMillis;
    lastbutton1State = button1State;
  }
  if ((currentMillis - button2Millis >= 50) && (button2State != lastbutton2State)) {
    if ((button0State == HIGH) && button2State == LOW) {
      if (waveShape == 2) {
        if (cycleDivision < 4) {
          cycleDivision = cycleDivision * 2;
        }
        else {
          cycleDivision = 1;
        }
      }
      else {
        waveShape = 2;
      }
    }
    if ((button0State == LOW) && (button2State == LOW)) {
      waveShape = 2;
      reverse = !reverse;
      reversed = true;
    }
    button2Millis = currentMillis;
    lastbutton2State = button2State;
  }
  if ((currentMillis - button3Millis >= 50) && (button3State != lastbutton3State)) {
    if ((button0State == HIGH) && button3State == LOW) {
      if (waveShape == 3) {
        if (cycleDivision < 4) {
          cycleDivision = cycleDivision * 2;
        }
        else {
          cycleDivision = 1;
        }
      }
      else {
        waveShape = 3;
      }
    }
    if ((button0State == LOW) && (button3State == LOW)) {
      waveShape = 3;
      reverse = !reverse;
      reversed = true;
    }
    button3Millis = currentMillis;
    lastbutton3State = button3State;
  }

  //Behaviour of the waveshapes with midi clock present.
  //Waveshapes: 0=Static (expresion input=expression output), 1=Saw, 2=Triangle, 3=Square.
  //Reverse waveshape is available on modes 1,2 and 3 with midi clock, witchout midi clock only on mode 2 (Saw).
  //All incoming midi messages will be transmitted out of the midi output.

  if (Serial.available() > 0) {
    data = Serial.read();
    Serial.write(data);
    if (data == midi_clock) {
      Sync();
      clockMillis = currentMillis;
    }
    if (data == midi_start || data == midi_stop) {
      counter = 95;
    }
    if (waveShape == 0) {
      val = sensorValue / 4;
      analogWrite(led0, pow((val / 16), 2));
    }
    if (waveShape == 1) {
      if (cycleDivision == 1) {
        factor = 2.68;
        beat = 0;
      }
      if (cycleDivision == 2) {
        factor = 5.43;
        if (counter < 48) {
          beat = 0;
        }
        else {
          beat = 48;
        }
      }
      if (cycleDivision == 4) {
        factor = 11.09;
        if (counter > -1 && counter < 24) {
          beat = 0;
        }
        if (counter > 24 && counter < 48) {
          beat = 24;
        }
        if (counter > 48 && counter < 72) {
          beat = 48;
        }
        if (counter > 72 && counter < 96) {
          beat = 72;
        }
      }
      if (reverse == false) {
        val = (counter - beat) * factor;
      }
      else {
        val = 255 - ((counter - beat) * factor);
      }
      analogWrite(led1, (pow(val, 2) / 256));
    }
    if (waveShape == 2) {
      if (cycleDivision == 1) {
        factor = 5.43;
        if (counter < 48) {
          beat = 0;
          rise = true;
        }
        else {
          beat = 48;
          rise = false;
        }
      }
      if (cycleDivision == 2) {
        factor = 11.09;
        if (counter >= 0 && counter < 24) {
          beat = 0;
          rise = true;
        }
        if (counter >= 24 && counter < 48) {
          beat = 24;
          rise = false;
        }
        if (counter >= 48 && counter < 72) {
          beat = 48;
          rise = true;
        }
        if (counter >= 72 && counter < 96) {
          beat = 72;
          rise = false;
        }
      }
      if (cycleDivision == 4) {
        factor = 23.18;
        if (counter >= 0 && counter < 12) {
          beat = 0;
          rise = true;
        }
        if (counter >= 12 && counter < 24) {
          beat = 12;
          rise = false;
        }
        if (counter >= 24 && counter < 36) {
          beat = 24;
          rise = true;
        }
        if (counter >= 36 && counter < 48) {
          beat = 36;
          rise = false;
        }
        if (counter >= 48 && counter < 60) {
          beat = 48;
          rise = true;
        }
        if (counter >= 60 && counter < 72) {
          beat = 60;
          rise = false;
        }
        if (counter >= 72 && counter < 84) {
          beat = 72;
          rise = true;
        }
        if (counter >= 84 && counter < 96) {
          beat = 84;
          rise = false;
        }
      }
      if (reverse == false) {
        if (rise == true) {
          val = (counter - beat) * factor;
        }
        else {
          val = 255 - ((counter - beat) * factor);
        }
      }
      else {
        if (rise == false) {
          val = (counter - beat) * factor;
        }
        else {
          val = 255 - ((counter - beat) * factor);
        }
      }
      analogWrite(led2, (pow(val, 2) / 256));
    }
    if (waveShape == 3) {
      if (cycleDivision == 1) {
        if (counter <= 47) {
          if (reverse == false) {
            val = 0;
          }
          else {
            val = 255;
          }
        }
        else {
          if (reverse == false) {
            val = 255;
          }
          else {
            val = 0;
          }
        }
      }
      if (cycleDivision == 2) {
        if (counter < 24 || (counter > 47 && counter < 72 )) {
          if (reverse == false) {
            val = 0;
          }
          else {
            val = 255;
          }
        }
        else {
          if (reverse == false) {
            val = 255;
          }
          else {
            val = 0;
          }
        }
      }
      if (cycleDivision == 4) {
        if (counter < 12 || (counter > 23 && counter < 36 ) || (counter > 47 && counter < 60 ) || (counter > 71 && counter < 84 )) {
          if (reverse == false) {
            val = 0;
          }
          else {
            val = 255;
          }
        }
        else {
          if (reverse == false) {
            val = 255;
          }
          else {
            val = 0;
          }
        }
      }
      analogWrite(led3, val);
    }
    digitalPotWrite(val);
  }

  //Without the presence of midi clock, expression input controlls the rate of the LFOs in mode 1, 2, and 3.

  if (currentMillis - clockMillis >= 500) {
    if (waveShape == 0) {
      val = sensorValue / 4;
      analogWrite(led0, pow((val / 16), 2));
      analogWrite(led1, 0);
      analogWrite(led2, 0);
      analogWrite(led3, 0);
      digitalPotWrite(val);
    }
    if (waveShape == 1) {
      analogWrite(led0, 0);
      analogWrite(led2, 0);
      analogWrite(led3, 0);
      changeRate = sqrt(sensorValue) * 0.38 + 0.64;
      if (currentMillis - updateMillis >= 10) {
        if (reverse == false) {
          val = val + changeRate;
          if (val >= 255) {
            val = 0;
          }
        }
        else {
          val = val - changeRate;
          if (val <= 0) {
            val = 255;
          }
        }
        updateMillis = currentMillis;
      }
      analogWrite(led1, pow((val / 16), 2));
      digitalPotWrite(val);
    }
    if (waveShape == 2) {
      analogWrite(led0, 0);
      analogWrite(led1, 0);
      analogWrite(led3, 0);
      changeRate = sqrt(sensorValue) * 0.76 + 1.28;
      if (currentMillis - updateMillis >= 10) {
        if (rise == true) {
          val = val + changeRate;
          if (val > 255) {
            val = 255;
            rise = false;
          }
        }
        if (rise == false) {
          val = val - changeRate;
          if (val < 0) {
            val = 0;
            rise = true;
          }
        }
        updateMillis = currentMillis;
      }
      digitalPotWrite(val);
      analogWrite(led2, pow((val / 16), 2));
    }
    if (waveShape == 3) {
      analogWrite(led0, 0);
      analogWrite(led1, 0);
      analogWrite(led2, 0);
      changeRate = sqrt(sensorValue) * 0.76 + 1.28;
      if (currentMillis - updateMillis >= 10) {
        if (rise == true) {
          val = val + changeRate;
          if (val >= 255) {
            rise = false;
          }
          digitalPotWrite(0);
          analogWrite(led3, 0);
        }
        if (rise == false) {
          val = val - changeRate;
          if (val <= 0) {
            rise = true;
          }
          digitalPotWrite(255);
          analogWrite(led3, 255);
        }
        updateMillis = currentMillis;
      }
    }
  }
}

//Program for updating the DigiPot

int digitalPotWrite(int value) {
  digitalWrite(CS, LOW);
  SPI.transfer(address);
  SPI.transfer(value);
  digitalWrite(CS, HIGH);
}

//Program for counting midi clock signals and flashing LEDS on beats

void Sync() {
  counter++;
  if (counter >= 96) {
    counter = 0;
  }
  if (counter == 0 || counter == 23 || counter == 47 || counter == 71) {
    analogWrite(led0, 255);
    analogWrite(led1, 255);
    analogWrite(led2, 255);
    analogWrite(led3, 255);
  }
  else {
    analogWrite(led0, 0);
    analogWrite(led1, 0);
    analogWrite(led2, 0);
    analogWrite(led3, 0);
  }
}
