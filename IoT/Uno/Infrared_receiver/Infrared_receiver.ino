#include <IRremote.h>
#include <LiquidCrystal.h>

const int buttonPin = 7;
const int RECV_PIN = 10;
IRrecv irrecv(RECV_PIN);
decode_results results;
int buttonState = 0;
int lastButtonState = 0; 

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
  if (irrecv.decode(&results)){
    Serial.print("0x");
    Serial.println(results.value, HEX);
    lcd.setCursor(0, 0);
    lcd.print("0x");
    lcd.setCursor(2, 0);
    lcd.print(results.value, HEX);
    lcd.setCursor(10, 0);
    lcd.print(" PRTCL");
    lcd.setCursor(10, 1);
            switch (results.decode_type){
            case NEC: Serial.println("NEC"); 
            lcd.print(" NEC  ");
            break ;
            case SONY: Serial.println("SONY");
            lcd.print(" SONY "); 
            break ;
            case RC5: Serial.println("RC5");
            lcd.print(" RC5 "); 
            break ;
            case RC6: Serial.println("RC6");
            lcd.print(" RC6 "); 
            break ;
            case DISH: Serial.println("DISH");
            lcd.print(" DISH "); 
            break ;
            case SHARP: Serial.println("SHARP"); 
            lcd.print(" SHARP");
            break ;
            case JVC: Serial.println("JVC"); 
            lcd.print(" JVC ");
            break ;
            case SANYO: Serial.println("SANYO"); 
            lcd.print(" SANYO");
            break ;
            case SAMSUNG: Serial.println("SAMSUNG");
            lcd.print(" SAMSUNG"); 
            break ;
            case LG: Serial.println("LG");
            lcd.print(" LG  "); 
            break ;
            case WHYNTER: Serial.println("WHYNTER");
            lcd.print(" WHYNTER"); 
            break ;
            case PANASONIC: Serial.println("PANASONIC");
            lcd.print(" PANASONIC"); 
            break ;
            case DENON: Serial.println("DENON");
            lcd.print(" DENON"); 
            break ;
          default:
            case UNKNOWN: Serial.println("UNKNOWN");
            lcd.print(" UNKNOWN");
            break ;
          }
    irrecv.resume();
    delay(50);
  }
    button();
}

void button(){
  buttonState = digitalRead(buttonPin);
  if (buttonState != lastButtonState) {
    if (buttonState == HIGH) {
      irsend.sendNEC(results.value, 32);
      irrecv.enableIRIn();
      Serial.print("Sending IR signal, HEX value: 0x");
      Serial.print(results.value, HEX);
      Serial.println();
      
      lcd.setCursor(0, 1);
      lcd.print("0x");
      lcd.setCursor(2, 1);
      lcd.print(results.value, HEX);
    } else {
      //do nothing.
    }
    delay(50);
  }
  lastButtonState = buttonState; 
}
