#include <RedBot.h>

RedBotMotors motors;
RedBotAccel accel;

RedBotSensor LeftSensor = RedBotSensor(A1);
RedBotSensor MiddleSensor = RedBotSensor(A0);
RedBotSensor RightSensor = RedBotSensor(A7);
int trig = A3;
int echo = A2;
int ledPin = 13;

int basePower_R = 100;
int basePower_L = 104;
int bumpthresh = 40;
int bg = 750;
bool left;
bool mid;
bool right;
bool bump;
int LeftValue;
int MiddleValue;
int RightValue;
float distance;
int State = 1;
int SonarRange = 6;

void setup() {
  Serial.begin(9600);
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
  pinMode(ledPin, OUTPUT);
  accel.x = 0;
  accel.y = 0;
  accel.z = 0;
  accel.enableBump();
  accel.setBumpThresh(bumpthresh);
  randomSeed(random(100));
}

void loop() {
  Input();
  UpdateState();
  PerformAction();
}

void Input(){
  accel.read();
  bump = accel.checkBump();
  LeftValue = LeftSensor.read();
  MiddleValue = MiddleSensor.read();
  RightValue  = RightSensor.read();  
  Serial.print(LeftValue); Serial.print("   ");
  Serial.print(MiddleValue); Serial.print("   ");
  Serial.println(RightValue);

  if(LeftValue < bg){
    left = false;
  }else{
    left = true;
  }

  if(MiddleValue < bg){
    mid = false;
  }else{
    mid = true;
  }

  if(RightValue < bg){
    right = false;
  }else{
    right = true;
  }
  
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);  
  float duration = pulseIn(echo, HIGH);
  distance = duration / 58;
  Serial.println(distance);
}


void UpdateState(){
  switch(State){
    case 1:
     if(left || mid || right || distance < SonarRange){
      State = 2;
     }else if(bump){
      State = 3;
     }
    break;
    case 2:
    State = 1;
    break;
    case 3:
    State = 3;
    break;
  }
}

void PerformAction(){
  switch(State){
    case 1: //Drive
    straight();
    break;
    case 2: //Turn
    RecoverLeft();
    break;
    case 3:
    bumped();
    break;
  }
}

void bumped(){
  motors.stop();
  digitalWrite(ledPin, HIGH);
  delay(1000);
  digitalWrite(ledPin, LOW);
}

void RecoverRight(){  
  motors.stop();
  delay(200);
  reverse();
  delay(200);
  motors.stop();
  delay(200);
  motors.pivot(- basePower_L, 800);
  motors.stop();
  delay(200);
}

void RecoverLeft(){  
  motors.stop();
  delay(200);
  reverse();
  delay(200);
  motors.stop();
  delay(200);
  motors.pivot(basePower_L, 250);
  motors.stop();
  delay(200);
}

void straight(){
  motors.leftDrive(basePower_L);
  motors.rightDrive(basePower_R);
}

void reverse(){
  motors.leftDrive(-basePower_L);
  motors.rightDrive(-basePower_R);
}
