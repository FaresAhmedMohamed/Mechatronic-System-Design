#include <cvzone.h>

SerialData serialData(1,1);
SerialData sequenceData(20,1);
int signalout[2];
int signalin[1];
int sequence[20];

void setup() {
serialData.begin();
pinMode(13,INPUT_PULLUP);
}

void loop() {
if (digitalRead(13)==0)
{
  signalout[0]=9;
  serialData.Send(signalout);
  int counter=0;
  int position=1;
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
    delay(2500);
    signalout[0]=8-counter;
    serialData.Send(signalout);
    position++;
    counter++;
    }
    signalout[0]=8-counter;
    serialData.Send(signalout);
    delay(100);
    /*int sequence[20]={0};
    int index=0;
    while(1)
    {
      signalin[0]=0;
      while(1)
      {
      serialData.Get(signalin);
      if(signalin[0]>0 && signalin[0]<10)
        {
          break;
        }
      }
      if(signalin[0]==9)
        {
        break;
        }
      sequence[index]=signalin[0];
      signalout[0]=(index%8);
      serialData.Send(signalout);
      delay(5);
      index++;
    }
    delay(5);
    signalout[0]=(index%8);
    serialData.Send(signalout);
    delay(1000);
    signalout[0]=sequence[0];
    serialData.Send(signalout);
    */
    int h=0;
    while(h<2000){
      serialData.Get(signalin);
      delay(1);
      h++;
    }
    while(sequence[19]==0)
    {
    sequenceData.Get(sequence);
    }
    delay(1000);
    int l=0;
    while(l<sizeof(sequence)){
    signalout[0]=sequence[l];
    serialData.Send(signalout);
    l++;
    delay(1000);
    }
    Serial.end();
}
}


