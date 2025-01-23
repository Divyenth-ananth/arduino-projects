#include <tcs3200.h>
#include <Servo.h>
#include <LiquidCrystal.h>
char pick = 'r';
char drop = 'r';
int red, green, blue, white;
int redTH=30,blueTH=20,greenTH=20;
tcs3200 tcs(8,9,11,12,10); // (S0, S1, S2, S3, output pin)  //
LiquidCrystal lcd(2,3,4,8,12,13);

Servo bigMotor;
const int bigMotorPin = 5;
Servo smallMotor;
const int smallMotorPin = 6;
int redLed = 13;
int greenLed =12;
int blueLed = 11;
int RLed = 3;
int BLed = 6;
int GLed =5;
int pickThreshold;
String pickColorName;
int pickLed;
int dropThreshold;
String dropColorName;
int dropLed;
int pickColor;
int dropColor;
int t=0;
  // int pickColor = tcs.colorRead('r');



 int proximityPin = A4;
  void setMotors(){
    bigMotor.attach(bigMotorPin); // Re-attach the servo to continue its movement
    // Continue sweeping by moving the servo back and forth
    for (int angle = 0; angle >= -90; angle--){
      bigMotor.write(angle);
      delay(15);
    }
    bigMotor.detach();
    smallMotor.attach(smallMotorPin);
    for (int angle = 0; angle >= -90; angle--){
      smallMotor.write(angle);
      delay(15);
    }
    smallMotor.detach();
  }

void setup() {
  
  lcd.begin(16, 2);


  setMotors();
  lcd.print("Hello World");
  Serial.begin(9600);
  pinMode(redLed,OUTPUT);
  pinMode(blueLed,OUTPUT);
  pinMode(greenLed,OUTPUT);

}
void loop() {
  t=0;
  digitalWrite(redLed,LOW);
  
  int pickColor;
  if(pick == 'r'){
    pickColorName = "RED";
    pickThreshold = redTH;
    pickLed = redLed;
  }
  else if(pick == 'b'){
    pickColorName = "BLUE";
    pickThreshold = blueTH;
    pickLed = blueLed;
  }
  else {
    pickColorName = "GREEN";
    pickThreshold = greenLed;
    pickLed = greenLed;
  }
  int dropColor;
  if(drop == 'r'){
    dropColorName = "RED";
    dropThreshold = redTH;
    dropLed = redLed;
  }
  else if(drop == 'b'){
    dropColorName = "BLUE";
    dropThreshold = blueTH;
    dropLed = blueLed;
  }
  else {
    dropColorName = "Green";
    dropThreshold = greenTH;
    dropLed = greenLed;
  }
  Serial.println(pickColorName);
  Serial.println(pickThreshold);
  Serial.println(dropColorName);
  Serial.println(dropThreshold);
  findPick();
  bigMotor.attach(bigMotorPin); // Re-attach the servo to continue its movement
  bigMotor.write(0);
  bigMotor.detach();
  t=0;
  findDrop();
  bigMotor.attach(bigMotorPin); // Re-attach the servo to continue its movement
  bigMotor.write(0);
  bigMotor.detach();


  // If sensor value is below a threshold, an obstacle is detected
  // Adjust threshold value as needed


  red = tcs.colorRead('r');   //reads color value for red
  Serial.print("R= ");
  Serial.print(red);
  Serial.print("    ");
  
  green = tcs.colorRead('g');   //reads color value for green
  Serial.print("G= ");
  Serial.print(green);
  Serial.print("    ");

  blue = tcs.colorRead('b');    //reads color value for blue
  Serial.print("B= ");
  Serial.print(blue);
  Serial.print("    ");

  white = tcs.colorRead('c');    //reads color value for white(clear)
  Serial.print("W(clear)= ");
  Serial.print(white);
  Serial.print("    ");
  Serial.println();



  delay(200);

}
    void findPick(){
    bigMotor.attach(bigMotorPin);
    while(true){
      for(int angle=0;angle<=180;angle++){
        bigMotor.write(angle);
        delay(25);
        pickColor = tcs.colorRead(pick);
        if(pickColor>pickThreshold){
          Serial.println("found picking place");
          lcd.clear();
          lcd.print("pick : ");
          lcd.print(pickColorName);
          take();
          digitalWrite(pickLed,HIGH);
          t=1;
          break;
        }
      }
      if(t==1){break;}
    }
    bigMotor.detach();
  }
  void findDrop(){
    bigMotor.attach(bigMotorPin);
    while(true){
    for(int angle=0;angle<=180;angle++){
      bigMotor.write(angle);
      delay(25);
      dropColor = tcs.colorRead(drop);
      if(dropColor>dropThreshold){
        Serial.println("found dropping place");
        lcd.clear();
        lcd.print("drop : ");
        lcd.print(dropColorName);
        put();
        digitalWrite(dropLed,HIGH);
        t=1;
        break;
      }
    }
    if(t==1){break;}

    }
    bigMotor.detach();

  }
  void take(){
    smallMotor.attach(smallMotorPin);
    for(int angle=0;angle<=180;angle++){
      smallMotor.write(angle);
      int proximityValue = analogRead(proximityPin); // Read sensor value
      if(proximityValue<=200){
        Serial.println("found object");
        for(int back=angle;back>=90;back--){
          smallMotor.write(back);
          delay(15);
        }
        break;
      }
      delay(25);
    }
    smallMotor.detach();
  }
  void put(){
    smallMotor.attach(smallMotorPin);
    for(int angle=0;angle<=180;angle++){
      smallMotor.write(angle);
      int proximityValue = analogRead(proximityPin); // Read sensor value
      if(proximityValue<=200){
        Serial.println("dropped object");
        digitalWrite(dropLed,LOW);
        for(int back=angle;back>=0;back--){
          smallMotor.write(back);
          delay(15);
        }
        break;
      }
      delay(25);
    }
    smallMotor.detach();
  }
