#include <Key.h>
#include <Keypad.h>
#include <LiquidCrystal.h> //Load the LCD library
#include <Servo.h>

#define blueLED A2
#define greenLED A1
#define redLED A0
#define servoPin 3
#define buzzPin 7

const byte ROWS = 4; 
const byte COLS = 4;

const int buttonPin = A3;     // the number of the pushbutton pin
const int buttonPin2 = A4;

// variables will change:
int buttonState = 0;
int buttonState2 = 0; 

char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'3', '4', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte rowPins[ROWS] = {9, 8}; 
byte colPins[COLS] = {5, 4}; 
String pin[7] = {"-1","-1","-1","-1","-1","-1","-1"};
String unlockPin[7];
String starPin = "";

Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);
LiquidCrystal LCD(10, 11, 6, 13, 12, 2);
Servo myPointer;

int servoPos = 0;
const int delayTime = 2000;
const int soundPin = A5; //sound sensor attach to A0 
int threshold = 800; //Set minimum threshold for LED lit
String alarmActivated = "no"; 

void setup(){
  Serial.begin(9600);
  LCD.begin(16,2);
  pinMode(buttonPin, INPUT);
  pinMode(buttonPin2, INPUT);
  pinMode(blueLED, OUTPUT);
  pinMode(greenLED, OUTPUT);
  pinMode(redLED, OUTPUT);
  pinMode(buzzPin, OUTPUT);
  analogWrite(blueLED, 1023);
  myPointer.attach(servoPin);
  myPointer.write(servoPos);
}
  
void loop(){
  
  if(pin[0]=="-1"){
    starPin = "";
    setNewPassword();
  }else{
    if(alarmActivated == "no"){
      starPin = "";
      LCD.clear();
      LCD.setCursor(0,0);
      LCD.print("PIN: ");
      for(int i=0; i<7; i++){
          char customKey = customKeypad.getKey();
          int res;
          while(!customKey){
            customKey = customKeypad.getKey();
            res = reset();
            if(pin[0]=="-1"){
              break;
            }
            if(res == 1){
              break;
            }
          }
          if(pin[0]=="-1"){
              break;
          }
          if(res == 1){
              break;
          }
          LCD.setCursor(0,1);
          starPin.concat("*");
          LCD.print(starPin);
          unlockPin[i] = customKey;
          if(i == 6){
            LCD.clear();
            LCD.setCursor(0,0);
            if(pin[0]==unlockPin[0] && pin[1]==unlockPin[1] && pin[2]==unlockPin[2] && pin[3]==unlockPin[3] && pin[4]==unlockPin[4] && pin[5]==unlockPin[5] && pin[6]==unlockPin[6]){
              LCD.print("Access Granted");
              analogWrite(greenLED, 1023);
              analogWrite(blueLED, 0);
              for (servoPos = 0; servoPos <= 90; servoPos += 1){
                myPointer.write(servoPos);
                delay(70);
              }
              delay(delayTime);
              for (servoPos = 90; servoPos >= 0; servoPos -= 1){
                myPointer.write(servoPos);
                delay(70);
              }
              analogWrite(greenLED, 0);
              analogWrite(blueLED, 1023);
              LCD.clear();
              LCD.setCursor(0,0);
              LCD.print("Alarm ON");
              alarmActivated = "yes";
            }else{
              LCD.print("Access Denied");
              analogWrite(redLED, 1023);
              analogWrite(blueLED, 0);
              delay(2000);
              analogWrite(redLED, 0);
              analogWrite(blueLED, 1023);
            }
            delay(1000);
          }  
      }
    }else{
      buttonState2 = digitalRead(buttonPin2);
      if (buttonState2 == HIGH) {
        Serial.println("Button 2 Pressed");
        starPin = "";
        LCD.clear();
        LCD.setCursor(0,0);
        LCD.print("Input your PIN:");
        for(int i=0; i<7; i++){
            char customKey = customKeypad.getKey();
            int res;
            while(!customKey){
              customKey = customKeypad.getKey();
            }
            LCD.setCursor(0,1);
            starPin.concat("*");
            LCD.print(starPin);
            unlockPin[i] = customKey;
            if(i == 6){
              LCD.clear();
              LCD.setCursor(0,0);
              if(pin[0]==unlockPin[0] && pin[1]==unlockPin[1] && pin[2]==unlockPin[2] && pin[3]==unlockPin[3] && pin[4]==unlockPin[4] && pin[5]==unlockPin[5] && pin[6]==unlockPin[6]){
                LCD.print("Alarm Disabled");
                analogWrite(greenLED, 1023);
                analogWrite(blueLED, 0);
                delay(delayTime);
                analogWrite(greenLED, 0);
                analogWrite(blueLED, 1023);
                alarmActivated = "no";
              }else{
                LCD.print("Disable Failed!");
                analogWrite(redLED, 1023);
                analogWrite(blueLED, 0);
                delay(2000);
                analogWrite(redLED, 0);
                analogWrite(blueLED, 1023);
                LCD.clear();
                LCD.setCursor(0,0);
                LCD.print("Alarm ON");
              }
            }  
        }
        
      }else{
        
        int value = analogRead(soundPin);//read the value of A0
        Serial.println(value);
        if(value > 810) //if the value is greater than 600 
        { 
          tone(buzzPin, 50);
          analogWrite(redLED, 1023);
          delay(2000);
          noTone(buzzPin);
          analogWrite(redLED, 0);
        } else { 
           
          
        }
      }
    }

  }
}

void blinkLED(){
  int blinkRate = 200;
  for(int i=0; i<5; i++){
    analogWrite(blueLED, 0);
    delay(blinkRate);
    analogWrite(blueLED, 1023);
    delay(blinkRate);
  }
}

int reset(){
  buttonState = digitalRead(buttonPin);
  if (buttonState == HIGH) {
    Serial.println("Pressed");
    starPin = "";
    LCD.clear();
    LCD.setCursor(0,0);
    LCD.print("Input old pin: ");
    for(int i=0; i<7; i++){
      char customKey = customKeypad.getKey();
        while(!customKey){
          customKey = customKeypad.getKey();
        }
        LCD.setCursor(0,1);
        starPin.concat("*");
        LCD.print(starPin);
        unlockPin[i] = customKey;
        if(i == 6){
          LCD.clear();
          LCD.setCursor(0,0);
          if(pin[0]==unlockPin[0] && pin[1]==unlockPin[1] && pin[2]==unlockPin[2] && pin[3]==unlockPin[3] && pin[4]==unlockPin[4] && pin[5]==unlockPin[5] && pin[6]==unlockPin[6]){
            pin[0] = "-1";
            pin[1] = "-1";
            pin[2] = "-1";
            pin[3] = "-1";
            pin[4] = "-1";
            pin[5] = "-1";
            pin[6] = "-1";
            for(int i=0; i<7; i++){
              Serial.println(pin[i]);  
            }
            return 0;
          }else{
            LCD.print("Try Again");
            analogWrite(redLED, 1023);
            analogWrite(blueLED, 0);
            delay(2000);
            analogWrite(redLED, 0);
            analogWrite(blueLED, 1023);
            return 1;
          }
        }  
    }    
  } else {
    return 2;
  }
}

void setNewPassword(){
     for(int i=0; i<7; i++){
      char customKey = customKeypad.getKey();
      if(pin[i] == "-1"){
        LCD.setCursor(0,0);
        LCD.print("Input new pin:");
        while(!customKey){
          customKey = customKeypad.getKey();
        }
        LCD.setCursor(0,1);
        starPin.concat("*");
        LCD.print(starPin);
        pin[i] = customKey;
        if(i == 6){
          LCD.clear();
          LCD.setCursor(0,0);
          LCD.print("New Password Set");
          blinkLED();
        }
      }else{
        break;
      }  
    }
}
