// _5watch.h
// author: Kurt Mohler
//
// This library is meant to be used with the 5watch hardware
// with the ATMega328 breadboard bootloader already burned
// to it so that the ATMega chip can run off of its internal
// 8 MHz clock.
//
// Most of the time, the chip is in power save mode waiting
// for interrupts to wake it up.
//
// Timer2, which is driven by the 32.768 kHz watch crystal,
// triggers each second to increment the time.
//
// Int0 is triggered by one of the buttons to turn on the
// display for several seconds.


#if !defined(_5WATCH_h)
#define _5WATCH_h

class _5watch {
public:
  _5watch();
  void init();
  void loop();
  void clearLeds();
  void setLed(short int ledNum);
  void setSingleLed(short int ledNum);
  bool checkButton(short int buttonNum);

  short int getSeconds();
  short int getMinutes();
  short int getHours();

private:
  void initTimerTick();
  void initDisplay();
  void initButtons();
  void sleep();

  static short int rowPins[3];
  static short int colPins[4];
  static short int buttonPins[2];
};

#endif