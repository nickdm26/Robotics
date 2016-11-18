#include <RedBot.h>

RedBotMotors motors;
RedBotAccel accel;

RedBotSensor LeftSensor = RedBotSensor(A1);
RedBotSensor MiddleSensor = RedBotSensor(A6);
RedBotSensor RightSensor = RedBotSensor(A7);
int trigPin = A3;
int echoPin = A2;

int basePower_R = 80;
int basePower_L = 84;
int bumpthresh = 35;
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
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
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
  
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  float duration = pulseIn(echoPin, HIGH);
  distance = duration / 58;
}


void UpdateState(){
  switch(State){
    case 1:
        if(left || mid || right || bump || distance < SonarRange){
      State = 2;
    }
    break;
    case 2:
    State = 1;
    break;
  }
}

void PerformAction(){
  switch(State){
    case 1: //Drive
    straight();
    break;
    case 2: //Turn
    Recover();
    break;
  }
}

void Recover(){
  motors.stop();
  delay(200);
  reverse();
  delay(200);
  motors.stop();
  delay(200);
  motors.pivot(basePower_L, 400);
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
