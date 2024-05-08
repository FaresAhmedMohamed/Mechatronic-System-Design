#include <AccelStepper.h>
#include <Servo.h>
#include <cvzone.h>
// Define the stepper motor and the pins that is connected to
AccelStepper StepperX(1, A5, 5); // (Type of driver: with 2 pins, STEP, DIR)
AccelStepper StepperY(1, 3, 6);
AccelStepper StepperZ(1, 4, 7);
Servo Gripper;
#define LimitX 9
#define LimitY 10
#define LimitZ A2
#define  outputencA 2 //yellow  
#define outputencB A1 //green
#define LimitF A0
#define  input1 12
#define  input2 13
#define   PWM   11
#define button A4
SerialData serialData(1,1);
SerialData sequenceData(22,1);
int signalout[2];
int signalin[1];
int sequence[22];
int postion =0;
float angle=0;
float increment = (360.0/512.0);
long time1=0;
float kp =5.2;
float kd =1.0;
float ki =0.001;
long previoustime =0;
float eprevious =0 ;
float eintegral =0;
int targ = 45;
const int maxspeed = 400;
const int accel = 400;
const float ratio = (800/101.25);
int grip = 75;
int HomeX = 0;
int HomeY = 0;
int HomeZ = 0;
int assembly=0;
int storagesquare=0;
int storagecircle=0;

void setup() {
  pinMode(A4,INPUT_PULLUP);
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
  pinMode(outputencA,INPUT);
  pinMode(outputencB,INPUT);
  pinMode(input1,OUTPUT);
  pinMode(input2,OUTPUT);
  pinMode(PWM,OUTPUT);
  pinMode(LimitF,INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(outputencA),readEncoder,RISING);
  delay(5000);
  Home();
  HomeF();
  serialData.begin();
}

void loop() {
  if(digitalRead(button)==0){
  time1=micros();
  vision();
  delay(3000);
  Actuate(-26,4,0);
  Grip();
  Actuate(20,-15,0);
  Actuate(20,-15,-61);
  Actuate(-24,4,-61);
  UnGrip();
  Actuate(20,-15,-61);
  Actuate(20,-15,0);
  Actuate(-24,4,0);
  Grip();
  Actuate(-10,-58,0);
  Actuate(-10,-58,-61);
  Actuate(-4,-22,-61);
  UnGrip();
  Actuate(-24,4,-61);
  Grip();
  Actuate(5,-65,-61);
  Actuate(5,-65,-103);
  Actuate(-18,0,-103);
  UnGrip();
  delay(5000);
   }
}

void AvengersAssemble(int sequence[22]){
  int index=2;
  if(sequence[index]==0 && sequence[index+1]==0){
    base();
    assembly++;
    index+=2;
  }
  for(index;index<22;index++){
    if(index==6 || index==11 || index==16){
      if(sequence[index]==7){
        storesquare();
        index++;
        continue;
      }
      else if(sequence[index]==8){
        storecircle();
        index++;
        continue;
      }
    }
    if(sequence[index+1]==5){
      targ+=sequence[index]*45;
    }
    else if(sequence[index+1]==6){
      targ-=sequence[index]*45;
    }
    TurnFeed(targ);
    if(assembly%2==0){
      lid();
      assembly++;
      index+=2;
    }
    else if(assembly%2==1){
      base();
      assembly++;
      index+=2;
    }
  }
}

void base(){
  Actuate(-26,4,0);
  Grip();
  Actuate(20,-15,0);
  Actuate(20,-15,-61);
  Actuate(-24,4,-61);
  UnGrip();
  Actuate(20,-15,-61);
  Actuate(20,-15,0);
}

void lid(){
  Actuate(-24,4,0);
  Grip();
  Actuate(-10,-58,0);
  Actuate(-10,-58,-61);
  Actuate(-4,-22,-61);
  UnGrip();
}

void storesquare(){
  Actuate(-24,4,-61);
  Grip();
  Actuate(5,-65,-61);
  Actuate(5,-65,-103);
  Actuate(-18,0,-103);
  UnGrip();
}

void storecircle(){
}

void Home(){
  StepperX.setSpeed(200);
  StepperY.setSpeed(-300);
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
    for (grip; grip <= 195; grip += 1) {
    Gripper.write(grip);
    delay(3);
  }
}

void UnGrip(){
    for (grip; grip >= 75; grip -= 1) {
    Gripper.write(grip);              
    delay(3);                       
  }
}

void readEncoder(){
  int b =digitalRead(outputencB);
  if(b>0){
    postion++;
    angle+=increment;
  }
  else {
    postion--;
    angle-=increment;
  }
  
}

void movemotor (int dir , int pwmvalue, int pwm ,int in1 ,int in2 ){
  analogWrite(pwm,pwmvalue);
  if(dir == 1)
  {
  digitalWrite(in1,HIGH);
  digitalWrite(in2,LOW);
  }
 else if (dir == -1)
 {
  digitalWrite(in1,LOW);
  digitalWrite(in2,HIGH);
 }
 else if ( dir ==0) 
 {
  digitalWrite(in1,LOW);
  digitalWrite(in2,LOW);
 }

}

float pidcontrol (int target ,float kp,float kd,float ki){
  float u;
  //get delta t
  long currenttime = micros()-time1;
  float deltaT =((float)(abs(currenttime-previoustime))) /1.0e6;
  
  //calculate error ,derivative ,integral
  int e = (int)angle -target ;

  if(abs(e)<increment)
  {
    return 0;
  }
  float edrivative =(e-eprevious)/ deltaT ;
  eintegral = eintegral + e * deltaT ;
  
  // contrpal signal
  u = (kp * e) + (kd * edrivative) +( ki * eintegral);
  //update variable for next iteration
  previoustime =  currenttime ;
  eprevious =e ;
  return u ;
}

void HomeF(){
  int pwr=45;
  int dir=-1;
  while(1){
  movemotor(dir,pwr,PWM,input1,input2);
  if(digitalRead(LimitF)==0){
    int fix=0;
    while(fix<30){
    pwr=49;
    dir=1;
    movemotor(dir,pwr,PWM,input1,input2);
    delay(45);
    pwr=0;
    dir=0;
    movemotor(dir,pwr,PWM,input1,input2);
    delay(45);
    fix++;
    }
    movemotor(0,0,PWM,input1,input2);
    break;
  }
  }
    postion=0;
    angle=0;
    return;
}

void TurnFeed(int target){  
    float u = 1;//pid variable
    eintegral=0;
    while(u!=0 ){
    u = pidcontrol (target ,kp ,kd,ki);

 //motor power
    float pwr =fabs(u);
    if(pwr>235)
    {
      pwr=235;
    }
    else if(pwr<55)
    {
      pwr=55;
    }
    
 //motor direction
  int dir =1 ;
  if (u==0)
  {
    dir=0 ;
  }  
  else if (u<0)
  {
    dir=-1 ;
  }


  movemotor (dir,pwr,PWM,input1,input2);

    delay(25);
   }
   return;
}

void vision(){
  signalout[0]=9;
  serialData.Send(signalout);
  int counter=0;
  signalout[0]=8;
  while(counter<7)
    {
    while(1)
    {
      serialData.Get(signalin);
      if(signalin[0]==1+counter)
      {
        signalin[0]=0;
        break;
      }
    }
    TurnFeed(targ);
    delay(50);
    signalout[0]=8-counter;
    serialData.Send(signalout);
    counter++;
    targ+=45;
    }
    signalout[0]=8-counter;
    serialData.Send(signalout);
    delay(100);
    int h=0;
    while(h<2500){
      serialData.Get(signalin);
      delay(1);
      h++;
    }
    while(sequence[21]==0)
    {
    sequenceData.Get(sequence);
    }
    delay(1000);
    Serial.flush();
    int l=0;
    while(l<sizeof(sequence)){
    signalout[0]=sequence[l];
    serialData.Send(signalout);
    l++;
    delay(1000);
    }
    Serial.end();
}
