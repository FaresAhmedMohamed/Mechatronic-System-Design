//variable for encoder 
#define  outputencA 2 //yellow  
#define outputencB A1 //green

//define pin num for motor
#define  input1 12
#define  input2 13
#define   PWM   11

//variable of postion
int postion =0;
float angle=0;
float increment = (360.0/512.0);
long time1=0;


//vaeiable fpr pid control
long previoustime =0;
float eprevious =0 ;
float eintegral =0;
int target = 0;




void setup() {
  Serial.begin(9600);
  pinMode(outputencA,INPUT);
  pinMode(outputencB,INPUT);
  pinMode(input1,OUTPUT);
  pinMode(input2,OUTPUT);
  pinMode(PWM,OUTPUT);
  //INterrupt for encoder
  attachInterrupt(digitalPinToInterrupt(outputencA),readEncoder,RISING);
}



void loop() {
   while(1){ //pid variable 
    float kp =5.0;
    float kd =1.0;
    float ki =0.001;
    float u = 1;
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

  Serial.println(target);
  Serial.print(", ");
  Serial.println(angle);
    delay(20);
  
    }

    delay(3000);
    target+=45;
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




//function to move motor
void movemotor (int dir , int pwmvalue, int pwm ,int in1 ,int in2 )
{
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
  digitalWrite(pwm,HIGH);
  digitalWrite(in1,HIGH);
  digitalWrite(in2,HIGH);
 }

}







float pidcontrol (int target ,float kp,float kd,float ki)
{

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
