#include <RedBot.h>

RedBotMotors motors;
int LeftValue;
int MiddleValue;
int RightValue;
bool left;
bool mid;
bool right;
bool StopBot;
int rightPower;
int leftPower;
int basePower_R = 48;
int basePower_L = 40;
int deltaLR = 15;

RedBotSensor LeftSensor = RedBotSensor(A1);
RedBotSensor MiddleSensor = RedBotSensor(A6);
RedBotSensor RightSensor = RedBotSensor(A7);

int bg = 750;

void setup() {
  Serial.begin(9600);
  StopBot = false;
  
 
}

void loop() { 
  LeftValue = LeftSensor.read();
  MiddleValue = MiddleSensor.read();
  RightValue  = RightSensor.read();
  Serial.print("Left: "); Serial.print(LeftValue); Serial.print("  ");
  Serial.print("Mid: "); Serial.print(MiddleValue); Serial.print("  ");
  Serial.print("Right: "); Serial.print(RightValue); Serial.print("  ");
  Serial.println(StopBot);

  if(!StopBot){
    lineFollowing();
    motors.leftDrive(leftPower);
    motors.rightDrive(rightPower);
    delay(50);
  } else{
    motors.brake();
    delay(200);
    StopBot = false;
  }
  Serial.print(left); Serial.print(mid); Serial.println(right);
  
  //straight();
}

void lineFollowing(){
  //Serial.println("yes");
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


  if(left && mid && right){ //Impossible.
    StopBot = true;
  }
  
  if(left && mid && !right){ //To far right, gently turn left.
    rightPower = basePower_R;
    leftPower = basePower_L + deltaLR;
    Serial.print("little left");
  }
  
  if(left && !mid && right){  //Impossible.
    StopBot = true;
  }
  
  if(left && !mid && !right){ //To far right, harder turn left.
    rightPower = basePower_R + deltaLR;
    leftPower = basePower_L - deltaLR;   
    Serial.print("hard left"); 
  }
  
  if(!left && mid && right){  //To far left, gently turn right.
    rightPower = basePower_R - deltaLR;
    leftPower = basePower_L;
    Serial.print("little right");
  }
  
  if(!left && mid && !right){ //Perfect, Straigt ahead.
    rightPower = basePower_R;
    leftPower = basePower_L;
  }
  
  if(!left && !mid && right){ //To far left, harder turn right.
    rightPower = basePower_R - deltaLR;
    leftPower = basePower_L + deltaLR;
    Serial.print("hard right");
  }
  
  if(!left && !mid && !right){  //Lost
    StopBot = true;
  }  
}

void square(){
  straight();
  delay(1000);
  motors.brake();
  delay(300);
  motors.pivot(-100,500);
  motors.brake();
  delay(300);  
}

void zigzag(){
  straight();
  delay(1000);
  motors.brake();
  delay(300);
  motors.pivot(-100,450);
  motors.brake();
  delay(300);
  straight();
  delay(1000);
  motors.brake();
  delay(300);
  motors.pivot(100,450);
  motors.brake();
  delay(300);
}

void straight(){
  motors.leftDrive(-108);
  motors.rightDrive(-100);
}

