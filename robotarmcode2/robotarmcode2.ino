#include <cvzone.h>

SerialData serialData(1,1);
int objects[8];
int signalout[2];
int signalin[1];

void setup() {
serialData.begin();
pinMode(13,INPUT);
pinMode(12,OUTPUT);
pinMode(11,OUTPUT);
pinMode(10,OUTPUT);
pinMode(9,OUTPUT);
pinMode(8,OUTPUT);
pinMode(7,OUTPUT);
digitalWrite(12,HIGH);
}

void loop() {
if (digitalRead(13)==1)
{
  signalout[0]=9;
  serialData.Send(signalout);
  int counter=0;
  int position=1;
  signalout[0]=8;
  while(counter<3)
    {
    while(1)
    {
      serialData.Get(signalin);
      if(signalin[0]==1+counter)
        signalin[0]=0;
        {break;}
    }
    delay(3000);
    digitalWrite(12-position+1,LOW);
    digitalWrite(12-position,HIGH);
    serialData.Send(signalout);
    position++;
    counter++;
    }
    serialData.Send(signalout);
    
}
}


