/*
Doesn't read full ABC notation yet e.g aabbbcc not a2b3c2
't' changes direction of the stepper motor
' ' is a rest
*/

int stepNo = 9; //1st pin of the stepper motor
int duration = 0;  //duration of note calculated from bpm
unsigned long oldMicros = 0;//this may get confused after 4294 seconds
unsigned long oldMillis = 0;
int bpm = 120;
int noteTime = 0;  //Time taken through note so far
int waveTime = 0;  //Time taken to next step so far
int forwards = 1;  //1 for forawards, -1 for backwards
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
  duration = 60000/bpm; //minutes in milliSeconds
}

void loop()
{
  if(Serial.available())
  {
ch = Serial.read();  
    if(ch == 't')
      {
        forwards = forwards - 2*forwards; //changes forwards between -1 and 1
      }
    else if (ch == ' ')
    {
      delay(duration);
    }
    if (ch >= 'a' and ch <= 'g')
    {
      playNote(lowTonedurations[ch - 'a']);
    }
    else if (ch >= 'A' and ch <= 'G')
    {
      playNote(highTonedurations[ch - 'A']);
    }
  }
}
void playNote(long timePeriod)
{
  oldMillis = millis();
  oldMicros = micros();
  while(noteTime <= duration)
  {
    if(waveTime >= timePeriod) //if it is time to step the motor one more step
    {
      digitalWrite(stepNo,LOW);    //turn off last pin
      stepNo = stepNo + forwards;  //+1 or -1 from pin number
      if(stepNo == 8) stepNo = 12; //unless it goes past 8 or 13
      if(stepNo == 13) stepNo = 9;
      oldMicros = micros();        //reset oldMicros for next step
      digitalWrite(stepNo,HIGH);   //turn on new pin
    }
    waveTime = micros() - oldMicros;  //difference between now and new step in micros
    noteTime = millis() - oldMillis;  //length of time of note already passed 
  }
  noteTime = 0;  //resets the note time
 digitalWrite(stepNo,LOW); //turns pin off so if no more data recieved, the stepper isn't just shorting out.
}



