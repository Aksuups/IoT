/*------------------------------------------------------------------------------*/
  /*Infared receiver and sender. Sensors repurposed from old dvd-player and remote.
  /*Aleksi Jokinen*/
  /*
  * Third party library used: IRremote.h
  * Dev: https://github.com/Arduino-IRremote/Arduino-IRremote
  * Initially coded 2009 Ken Shirriff http://www.righto.com Copyright (c) 2016 Rafi Khan Copyright (c) 2020-2021 Armin Joachimsmeyer
  */
/*------------------------------------------------------------------------------*/
/*Libraries used by the program*/
#include <IRremote.h>
#include <LiquidCrystal.h>

const int buttonPin = 7;
const int RECV_PIN = 10;
IRrecv irrecv(RECV_PIN);
decode_results results;
int buttonState = 0;
int lastButtonState = 0;
unsigned long timesSend = 0;
int testrange = 2774140000;

const int RS = 6, ENABLE = 8, D4 = 9, D5 = 11, D6 = 12, D7 = 5;
LiquidCrystal lcd(RS, ENABLE, D4, D5, D6, D7);



IRsend irsend; //IR led needs to be on PWM 3 on UNO. Can be changed if library itself edited. 
  
void setup() {
  Serial.begin(9600);
  irrecv.enableIRIn();
  irrecv.blink13(true);
  pinMode(buttonPin, INPUT);
  lcd.begin(16, 2);
  lcd.clear();
}

void loop() {
      results.value = testrange;
      irsend.sendNEC(results.value, 32);
      irrecv.enableIRIn();
      Serial.print("Sending IR signal, HEX value: 0x");
      Serial.print(results.value, HEX);
      Serial.println();
      lcd.setCursor(0,0);
      lcd.print("Current signal:");
      lcd.setCursor(0, 1);
      lcd.print("0x");
      lcd.setCursor(2, 1);
      lcd.print(results.value, HEX);
      timesSend++;
      lcd.setCursor(11, 1);
      lcd.print(timesSend);
      testrange++;
}
