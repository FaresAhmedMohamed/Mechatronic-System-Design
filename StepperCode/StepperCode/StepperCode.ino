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
  /*StepperX.moveTo(200);
  StepperY.moveTo(-300);
  StepperZ.moveTo(400);
  while(StepperX.currentPosition()!=200 || StepperY.currentPosition()!=-300 || StepperZ.currentPosition()!=400){
    StepperX.run();
    StepperY.run();
    StepperZ.run();
  }
  delay(2000);
    for (grip; grip <= 170; grip += 1) {
    Gripper.write(grip);
    delay(3);
  }
  delay(2000);
  StepperX.moveTo(-200);
  StepperY.moveTo(150);
  StepperZ.moveTo(-400);
  while(StepperX.currentPosition()!=-200 || StepperY.currentPosition()!=150 || StepperZ.currentPosition()!=-400){
    StepperX.run();
    StepperY.run();
    StepperZ.run();
  }
  delay(2000);
  for (grip; grip >= 85; grip -= 1) {
    Gripper.write(grip);              
    delay(3);                       
  }
  delay(2000);*/
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
  StepperX.moveTo(-ceil(10*ratio));
  while(StepperX.currentPosition()!=-ceil(7*ratio)){
    StepperX.run();
  }
  for (grip; grip <= 170; grip += 1) {
    Gripper.write(grip);
    delay(3);
  }
  StepperZ.moveTo(-ceil(90*ratio));
  while(StepperZ.currentPosition()!=-ceil(90*ratio)){
    StepperZ.run();
  }
  for (grip; grip >= 85; grip -= 1) {
    Gripper.write(grip);              
    delay(3);                       
  }
  delay(100000);
}
