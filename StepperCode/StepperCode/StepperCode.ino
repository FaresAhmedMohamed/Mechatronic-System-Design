#include <AccelStepper.h>
#include <Servo.h>
// Define the stepper motor and the pins that is connected to
AccelStepper StepperX(1, 2, 5); // (Type of driver: with 2 pins, STEP, DIR)
AccelStepper StepperY(1, 3, 6);
AccelStepper StepperZ(1, 4, 7);
Servo Gripper;
int grip = 85;
const int maxspeed = 400;
const int accel = 400;

void setup() {
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
  StepperX.moveTo(200);
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
  delay(2000);
}