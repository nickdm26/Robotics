#include <RedBot.h>

RedBotMotors motors;
RedBotAccel accel;
int basePower_R = 140;
int basePower_L = 144;
boolean bump = true;
boolean turning = false;
boolean driveing = true;
int bumpthresh = 35;

int bg = 750;
bool left;
bool mid;
bool right;
int LeftValue;
int MiddleValue;
int RightValue;
RedBotSensor LeftSensor = RedBotSensor(A1);
RedBotSensor MiddleSensor = RedBotSensor(A6);
RedBotSensor RightSensor = RedBotSensor(A7);

void setup() {
  Serial.begin(9600);
  accel.x = 0;
  accel.y = 0;
  accel.z = 0;
  accel.enableBump();
  accel.setBumpThresh(bumpthresh);
  randomSeed(random(100));

}

void loop() {
  accel.read();
  bump = accel.checkBump();
  LeftValue = LeftSensor.read();
  MiddleValue = MiddleSensor.read();
  RightValue  = RightSensor.read();  

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
  
//  if(bump){
//    Serial.println("Bumped");
//    motors.brake();
//    delay(300);
//    int ran = random(2000);
//    motors.pivot(basePower_L, ran);
//    motors.brake();
//    delay(300);
//  }else{
//    Serial.println("Nope");
//  }

  //Serial.print(left); Serial.print(mid); Serial.println(right);

  if(bump){
    turning = true;
    driveing = false;
    Serial.println("Bumping");
  }else if(left || mid || right){
    turning = true;
    driveing = false;
    Serial.println("light");
  }else{
    turning = false;
    driveing = true;
    Serial.println("drive");
  }

  if(driveing){
    straight();
  }else if(turning){
    motors.brake();
    delay(300);
    int ran = random(2000);
    motors.pivot(basePower_L, ran);
    motors.brake();
    delay(300);
    turning = false;   
  }

  
  //Serial.print("X: ");
  //Serial.print(accel.x);
  //Serial.print(" Y: ");
  //Serial.print(accel.y);
  //Serial.print(" Z: ");
  //Serial.println(accel.z);
  //delay(100);
  

}

void straight(){
  motors.leftDrive(basePower_L);
  motors.rightDrive(basePower_R);
}
