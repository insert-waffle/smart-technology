/*
 * RoomADM
 * A project by Jens Maes & Jonas Lembrechts
 * Arduino: Arduino UNO
 * IR remote: LAFVIN
 */

#define IR_UP 0x46
#define IR_DOWN 0x15
#define IR_RIGHT 0x43
#define IR_LEFT 0x44
#define IR_OK 0x40
#define IR_1 0x16
#define IR_2 0x19
#define IR_3 0xD
#define IR_4 0xC
#define IR_5 0x18
#define IR_6 0x5E
#define IR_7 0x8
#define IR_8 0x1C
#define IR_9 0x5A
#define IR_0 0x52
#define IR_HASHTAG 0x4A
#define IR_STAR 0x42

#include <IRremote.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);
int RECV_PIN = 8;

decode_results results;

unsigned long result_value;

void setup()
{
  Serial.begin(9600);
  // In case the interrupt driver crashes on setup, give a clue
  // to the user what's going on.
  Serial.println("Enabling IRin");
  IrReceiver.begin(RECV_PIN, ENABLE_LED_FEEDBACK);
  Serial.println("Enabled IRin");
  Serial.println("Enabling screen");
  lcd.init();
  lcd.clear();
  lcd.backlight();
}

void loop() {
  lcd.setCursor(0,0);
  lcd.print("Placeholder");
  
  if (Serial.available() > 0) {
    lcd.setCursor(0,1);
    String temp = Serial.readStringUntil('\n');
    String cpuID, cpuTemp;
    cpuID = temp.substring(3,4);
    cpuTemp = temp.substring(4,7);
    lcd.print("S"+cpuID+": "+cpuTemp+" C");
    delay(100);
  }
  
  if (IrReceiver.decode()) {
    int result = IrReceiver.decodedIRData.command;
    delay(100);
    switch (IrReceiver.decodedIRData.command) {
      case IR_HASHTAG:
        IrReceiver.resume();
        while(!IrReceiver.decode()) {
          // We're just waiting for the next input tbh...  
        }
        switch(IrReceiver.decodedIRData.command) {
          case IR_1:
            IrReceiver.resume();
            while(!IrReceiver.decode()) {
              // We're just waiting for the next input tbh...  
            }
            switch(IrReceiver.decodedIRData.command) {
              case IR_UP:
                changeFanSpeed("S1","0xA");
                break;
              case IR_DOWN:
                changeFanSpeed("S1","0xB");
                break;
            }
            break;
        }
        break;
      case IR_STAR:
        IrReceiver.resume();
        while(!IrReceiver.decode()) {
          // We're just waiting for the next input tbh...  
        }
        switch(IrReceiver.decodedIRData.command) {
          case IR_1:
            getSpeed("S1");
            break;
        }
    IrReceiver.resume(); // Receive the next value 
    }
  }
}

void changeFanSpeed(String server, String fspeed) {
  Serial.println(server+"$"+fspeed+"_");
}

void getRoomTemp() {
  
}

void getSpeed(String server) {
  Serial.println(server+"GET_");
}
