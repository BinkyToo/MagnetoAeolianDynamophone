/*
Doesn't read full ABC notation yet e.g aabbbcc not a2b3c2
't' changes direction of the stepper motor
' ' is a rest
*/

int stepNo1 = 9; //1st pin of the stepper motor
int stepNo2 = 5; //1st pin of other stpeer motor
int duration1 = 0;  //duration of note calculated from bpm
int duration2 = 0;
unsigned long oldMicros1 = 0;//this may get confused after 4294 seconds
unsigned long oldMicros2 = 0;
unsigned long oldMillis1 = 0;
unsigned long oldMillis2 = 0;
int bpm = 120;
int noteTime1 = 0;  //Time taken through note so far
int noteTime2 = 0;  //Time taken through note so far
int waveTime1= 0;   //Time taken to next step so far
int waveTime2= 0;   //Time taken to next step so far
int forwards1= 1;   //1 for forawards, -1 for backwards
int forwards2= 1;   //1 for forawards, -1 for backwards
int timePeriod = 0;
boolean done1=0;    //One motor may finish the note before the other has finished. Both need to have finished before it recieves more info
boolean done2=0;    
char ch = 'a';    //Used to be in the main loop
int lowTonedurations[] = {1136, 1012, 956, 902, 851, 804, 758, 638 };
//                      1136      b     c     d     e     f     g  eflat/h      

int highTonedurations[] = {2273, 2025, 1911, 1703, 1517, 1432, 1276, 1607, 2408, 1204, 758, 2025 };
//                            A    B     C     D     E     F     G  eflat/H  I   J/Ab   k/     L/B

void setup()
{
  pinMode(9, OUTPUT); 
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(12,OUTPUT);
  Serial.begin(9600);
  duration1 = 60000/bpm; //minutes in milliSeconds
}

void loop()
{
  if(Serial.available() and done1 == 1)
  {
ch = Serial.read();  
    if(ch == 't')
      {
        forwards1= forwards1- 2*forwards1; //changes forwards between -1 and 1
      }
    else if (ch == ' ')
    {
      delay(duration1);
    }
    if (ch >= 'a' and ch <= 'g' and done1 == 1)
    {
      timePeriod = lowTonedurations[ch - 'a'];
      done1 = 0;
      noteTime1 = 0;
      oldMillis1 = millis();
    oldMicros1 = micros();
    }
    else if (ch >= 'A' and ch <= 'G' and done1 == 1)
    {
      timePeriod = highTonedurations[ch - 'A'];
      done1 = 0;
      noteTime1 = 0;
      oldMillis1 = millis();
    oldMicros1 = micros();
     // Serial.println(ch);
    }
        
  }
  if(done1 == 0 || done2 == 0)
  {
    //delay(5);
    //Serial.println(noteTime1);
    if(noteTime1 >= duration1)
    {
      done1 = 1;
      //noteTime1 = 0;
      digitalWrite(stepNo1,LOW);
    }
    if(waveTime1>= timePeriod && done1 == 0) //if it is time to step the motor one more step
    {
      digitalWrite(stepNo1,LOW);    //turn off last pin
      stepNo1 = stepNo1 + forwards1;  //+1 or -1 from pin number
      if(stepNo1 == 8) stepNo1 = 12; //unless it goes past 8 or 13
      if(stepNo1 == 13) stepNo1 = 9;
      oldMicros1 = micros();        //reset oldMicros1 for next step
      digitalWrite(stepNo1,HIGH);   //turn on new pin
    }
    
    if(noteTime2 >= duration2)
    {
      done2 = 1;
      //noteTime2 = 0;
      digitalWrite(stepNo2,LOW);
    }
    if(waveTime2>= timePeriod && done2 == 0) //if it is time to step the motor one more step
    {
      digitalWrite(stepNo2,LOW);    //turn off last pin
      stepNo2 = stepNo2 + forwards2;  //+1 or -1 from pin number
      if(stepNo1 == 4) stepNo1 = 8; //unless it goes past 8 or 13
      if(stepNo1 == 9) stepNo1 = 5;
      oldMicros2 = micros();        //reset oldMicros1 for next step
      digitalWrite(stepNo2,HIGH);   //turn on new pin
    }
    
    
    waveTime1= micros() - oldMicros1;  //difference between now and new step in micros
    noteTime1 = millis() - oldMillis1;  //length of time of note already passed 
    waveTime2= micros() - oldMicros2;  //difference between now and new step in micros
    noteTime2 = millis() - oldMillis2;  //length of time of note already passed 
  }
}




