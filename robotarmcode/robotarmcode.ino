#include <cvzone.h>

SerialData serialData(1,1);
int valRec[1];
int signal[2];

void setup() {
serialData.begin();
pinMode(12,OUTPUT);
pinMode(10,OUTPUT);
pinMode(8,OUTPUT);
pinMode(6,OUTPUT);
pinMode(4,INPUT);
}

void loop() {
if (digitalRead(4)==1)
{
  signal[0]=9;
  serialData.Send(signal);
  delay(2000);
  serialData.Get(valRec);
  if (valRec[0]==1) cubeb();
  else if(valRec[0]==2) cubel();
  else if(valRec[0]==3) circleb();
  else if(valRec[0]==4) circlel();
}
}

void cubeb()
{
  digitalWrite(12,HIGH);
  digitalWrite(10,LOW);
  digitalWrite(8,LOW);
  digitalWrite(6,LOW);
}
void cubel()
{
  digitalWrite(12,LOW);
  digitalWrite(10,HIGH);
  digitalWrite(8,LOW);
  digitalWrite(6,LOW);
}
void circleb()
{
  digitalWrite(12,LOW);
  digitalWrite(10,LOW);
  digitalWrite(8,HIGH);
  digitalWrite(6,LOW);
}
void circlel()
{
  digitalWrite(12,LOW);
  digitalWrite(10,LOW);
  digitalWrite(8,LOW);
  digitalWrite(6,HIGH);
}
