#include <_5watch.h>
#include <Arduino.h>
#include <avr/sleep.h>

short int _5watch::rowPins[] = {4, A2, 5};
short int _5watch::colPins[] = {A6, A7, A0, A1};
short int _5watch::buttonPins[] = {2, 3};

static volatile short int seconds = 0;
static volatile short int minutes = 0;
static volatile short int hours = 0;

static volatile bool dispFlag = false;

void scheduleDisp() {
  dispFlag = true;
}

_5watch::_5watch() {
}

void _5watch::init() {
  Serial.begin(9600);


  //set_sleep_mode(SLEEP_MODE_IDLE);
  //set_sleep_mode(SLEEP_MODE_ADC);
  set_sleep_mode(SLEEP_MODE_PWR_SAVE);
  //set_sleep_mode(SLEEP_MODE_STANDBY);
  //set_sleep_mode(SLEEP_MODE_PWR_DOWN);

  initTimerTick();
  initDisplay();
}

void _5watch::initTimerTick() {
  // Configure timer to tick every second
  TCCR2A = 0x00;  // overflow
  TCCR2B = 0x05;  // 5 gives 1 sec prescale
  TIMSK2 = 0x01;  // enabled timer2A overflow interrupt
  ASSR = 0x20;    // enable sync mode

  sei();
}

void _5watch::initDisplay() {
  // Disable all rows
  for (short int r = 0; r < 3; r++) {
    pinMode(rowPins[r], OUTPUT);
    digitalWrite(rowPins[r], HIGH);
  }

  // Disable all columns
  for (short int c = 0; c < 4; c++) {
    pinMode(colPins[c], OUTPUT);
    digitalWrite(colPins[c], LOW);
  }
}

void _5watch::initButtons(){
  // Set up buttons as inputs
  pinMode(buttonPins[0], INPUT);
  pinMode(buttonPins[1], INPUT);
}

void _5watch::loop() {
  if (dispFlag) {
    Serial.print(getHours());
    Serial.print(":");
    Serial.print(getMinutes());
    Serial.print(":");
    Serial.println(getSeconds());
    //delay(100);

    for (int i = 0; i < 10; i++) {
      setSingleLed(i*4 + 2);
      delay(50);
    }

    for (int i = 0; i < 12; i++) {
      setLed(i);
    }
    delay(500);


    clearLeds();
  }

  sleep();
}

void _5watch::clearLeds() {
  for (short int r = 0; r < 3; r++) {
    digitalWrite(rowPins[r], HIGH);
  }

  for (short int c = 0; c < 4; c++) {
    digitalWrite(colPins[c], LOW);
  }
}

void _5watch::setLed(short int ledNum) {
  ledNum = ledNum % 12;

  short int r = ledNum / 4;
  short int c = ledNum % 4;

  digitalWrite(rowPins[r], LOW);
  digitalWrite(colPins[c], HIGH);
}

void _5watch::setSingleLed(short int ledNum) {
  clearLeds();
  setLed(ledNum);
}

bool _5watch::checkButton(short int buttonNum) {
  return digitalRead(buttonPins[buttonNum]);
}

void _5watch::sleep() {
  clearLeds();
  //set_sleep_mode(SLEEP_MODE_PWR_SAVE);
  sleep_enable();
  dispFlag = false;
  attachInterrupt(0, scheduleDisp, RISING);
  sleep_mode();
  //sleep_disable();
  detachInterrupt(0);
}

short int _5watch::getSeconds() {
  return seconds;
}

short int _5watch::getMinutes() {
  return minutes;
}

short int _5watch::getHours() {
  return hours;
}


// Increment time each second
ISR(TIMER2_OVF_vect) {
  if (++seconds == 60) {
    seconds = 0;
    if (++minutes == 60) {
      minutes = 0;
      if (++hours == 24) {
        hours = 0;
      }
    }
  }
}