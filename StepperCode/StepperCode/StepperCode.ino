#include <AccelStepper.h>
#include <Servo.h>
// Define the stepper motor and the pins that is connected to
AccelStepper StepperX(1, 2, 5); // (Type of driver: with 2 pins, STEP, DIR)
AccelStepper StepperY(1, 3, 6);
AccelStepper StepperZ(1, 4, 7);
Servo Gripper;
#define LimitX 9
#define LimitY 10
#define LimitZ A2
const int maxspeed = 400;
const int accel = 400;
const float ratio = (800/101.25);
int grip = 90;
int HomeX = 0;
int HomeY = 0;
int HomeZ = 0;

void setup() {
  pinMode(LimitX,INPUT_PULLUP);
  pinMode(LimitY,INPUT_PULLUP);
  pinMode(LimitZ,INPUT_PULLUP);
  StepperX.setMaxSpeed(maxspeed);
  StepperX.setAcceleration(accel);
  StepperX.setCurrentPosition(0);
  StepperY.setMaxSpeed(maxspeed);
  StepperY.setAcceleration(accel);
  StepperY.setCurrentPosition(0);
  StepperZ.setMaxSpeed(maxspeed);
  StepperZ.setAcceleration(accel);
  StepperZ.setCurrentPosition(0);
  Gripper.attach(A3);
  Gripper.write(grip);
  Serial.begin(9600);
  delay(5000);
}

void loop() {
  Home();
}

void Home(){
  StepperX.setSpeed(200);
  StepperY.setSpeed(-400);
  StepperZ.setSpeed(400);
  while(1){
  if(digitalRead(LimitX)==0){
    HomeX = 1;
  }
  if(digitalRead(LimitY)==0){
    HomeY = 1;
  }
  if(digitalRead(LimitZ)==0){
    HomeZ = 1;
  }
  if (HomeX==0){
    StepperX.runSpeed();
  }
  else if (HomeX==1){
    StepperX.stop();
  }
  if (HomeY==0){
    StepperY.runSpeed();
  }
  else if (HomeY==1){
    StepperY.stop();
  }
  if (HomeZ==0 && HomeX==1 && HomeY==1){
    StepperZ.runSpeed();
  }
  else if (HomeZ==1){
    StepperZ.stop();
  }
  if(HomeX==1 && HomeY==1 && HomeZ==1){
    break;
  }
  }
  StepperX.setCurrentPosition(0);
  StepperY.setCurrentPosition(0);
  StepperZ.setCurrentPosition(0);
  StepperX.moveTo(-ceil(40*ratio));
  StepperY.moveTo(ceil(67*ratio));
  while(StepperX.currentPosition()!=-ceil(40*ratio) || StepperY.currentPosition()!=ceil(67*ratio)){
    StepperX.run();
    StepperY.run();
  }
  StepperX.setCurrentPosition(0);
  StepperY.setCurrentPosition(0);
}

void Actuate(int xangle, int yangle, int zangle){
  int x;
  int y;
  int z;
  if(xangle>=0){
    x=ceil(xangle*ratio);
    StepperX.moveTo(x);
  }
  else{
    x=-ceil(abs(xangle*ratio));
    StepperX.moveTo(x);
  }
  if(yangle>=0){
    y=ceil(yangle*ratio);
    StepperY.moveTo(y);
  }
  else{
    y=-ceil(abs(yangle*ratio));
    StepperY.moveTo(y);
  }
  if(zangle>=0){
    z=ceil(zangle*ratio);
    StepperZ.moveTo(z);
  }
  else{
    z=-ceil(abs(zangle*ratio));
    StepperZ.moveTo(z);
  }
  while(StepperX.currentPosition()!=x || StepperY.currentPosition()!=y || StepperZ.currentPosition()!=z){
    StepperX.run();
    StepperY.run();
    StepperZ.run();
  }
}

void ActuateRelative(int xangle, int yangle, int zangle){
  int x;
  int y;
  int z;
  if(xangle>=0){
    x=ceil(xangle*ratio);
    StepperX.move(x);
  }
  else{
    x=-ceil(abs(xangle*ratio));
    StepperX.move(x);
  }
  if(yangle>=0){
    y=ceil(yangle*ratio);
    StepperY.move(y);
  }
  else{
    y=-ceil(abs(yangle*ratio));
    StepperY.move(y);
  }
  if(zangle>=0){
    z=ceil(zangle*ratio);
    StepperZ.move(z);
  }
  else{
    z=-ceil(abs(zangle*ratio));
    StepperZ.move(z);
  }
  while(StepperX.currentPosition()!=x || StepperY.currentPosition()!=y || StepperZ.currentPosition()!=z){
    StepperX.run();
    StepperY.run();
    StepperZ.run();
  }
}

void Grip(){
    for (grip; grip <= 170; grip += 1) {
    Gripper.write(grip);
    delay(3);
  }
}

void UnGrip(){
    for (grip; grip >= 85; grip -= 1) {
    Gripper.write(grip);              
    delay(3);                       
  }
}

