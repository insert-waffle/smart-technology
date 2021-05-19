/*
 * RoomADM
 * A project by Jens Maes & Jonas Lembrechts
 * Arduino: Arduino UNO
 * IR remote: LAFVIN
 */
#include <dht.h>
#include <IRremote.h>
#include <LiquidCrystal_I2C.h>

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
#define LED_RED 10 
#define LED_BLUE 11 
#define LED_GREEN 12 
#define LED_YELLOW 6

/* Sound sensor */
#define SSPIN A0

/* Temp sensor */
#define DHT11_PIN 7



dht DHT;


LiquidCrystal_I2C lcd(0x27, 16, 2);
int RECV_PIN = 8;

decode_results results;

unsigned long result_value;
bool sentTempWarning = false;

void setup()
{
  IrReceiver.begin(RECV_PIN, ENABLE_LED_FEEDBACK);
  lcd.init();
  lcd.clear();
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("Placeholder");
  pinMode(LED_RED,OUTPUT);
  pinMode(LED_BLUE,OUTPUT);
  pinMode(LED_GREEN,OUTPUT);
  Serial.begin(9600);
}

void loop() {
  remoteControl();
  getRoomTemp(); 
  getRoomHumid();  
  soundSensor();
  serialMonitor();
}

void changeFanSpeed(String server, String fspeed) {
  Serial.println(server+fspeed+"_");
}

void serialMonitor() {
  if (Serial.available() > 0) {
    lcd.clear();
    String temp = Serial.readStringUntil('\n');
    String cpuID, cpuTemp;
    cpuID = temp.substring(3,4);
    cpuTemp = temp.substring(4,7);
    lcd.setCursor(0,0);
    lcd.print("Server "+cpuID);
    lcd.setCursor(0,1);
    lcd.print("Temp: "+cpuTemp+" C");
    delay(10000);
    lcd.clear();
  }
}

void getRoomTemp() {
  int chk = DHT.read11(DHT11_PIN);
  int temperature = DHT.temperature;

  lcd.setCursor(0,0);
  lcd.print("Temp: ");
  lcd.print(DHT.temperature);
  lcd.print((char)223);
  lcd.print("C");

  if(temperature > 25 && temperature < 45) {
    statusRed();
    if(!sentTempWarning) {
      Serial.println("MAILTEMP_");
    }
    sentTempWarning = true;
  } else {
    statusGreen();
    sentTempWarning = false;
  }
}

void getRoomHumid() {
  int chk = DHT.read11(DHT11_PIN);

  lcd.setCursor(0,1);
  lcd.print("Humidity: ");
  lcd.print(DHT.humidity);
  lcd.print("%");
}


void statusGreen() {
  digitalWrite(LED_GREEN,HIGH);
  digitalWrite(LED_RED,LOW);
  digitalWrite(LED_BLUE,LOW);
}

void statusRed() {
  digitalWrite(LED_GREEN,LOW);
  digitalWrite(LED_RED,HIGH);
  digitalWrite(LED_BLUE,LOW);
}

void statusBlue() {
}

void soundSensor() {
  int tmp = analogRead(A0);

  if(tmp > 720) {
    digitalWrite(LED_YELLOW,HIGH);
  } else {
    digitalWrite(LED_YELLOW,LOW);
  }
  
}

void getTemp(String server) {
  Serial.println(server+"GET_");
  lcd.print("Temperature:");
}

void remoteControl() {
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
                IrReceiver.resume(); // Receive the next value
                lcd.clear();
                lcd.print("DFC enable S1"); 
                delay(5000);
                lcd.clear();
                break;
              case IR_DOWN:
                changeFanSpeed("S1","0xB");
                IrReceiver.resume(); // Receive the next value 
                lcd.clear();
                lcd.print("DFC disable S1"); 
                delay(5000);
                lcd.clear();
                break;
            }
            break;
        }
        break;
        IrReceiver.resume(); // Receive the next value 
      case IR_STAR:
        IrReceiver.resume();
        while(!IrReceiver.decode()) {
          // We're just waiting for the next input tbh...  
        }
        switch(IrReceiver.decodedIRData.command) {
          case IR_1:
            getTemp("S1");
            IrReceiver.resume();
            break;
        }
        IrReceiver.resume(); // Receive the next value 
        break;
    }
    lcd.clear();
    IrReceiver.resume();
  }
}
