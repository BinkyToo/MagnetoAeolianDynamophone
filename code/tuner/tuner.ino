// Project 19 Tune Player

int step1 = 2;
int step2 = 3;
int step3 = 4;
int step4 = 5;
long duration = 0;
long bpm = 60;
long cycles = 0;
long cyclesDone = 0;
int slide = 0;
int slideEnd = 1703;
int x = 0;
int pause = 0;
boolean forwards = 0;
int lowTonedurations[] = {1136, 1012, 956, 902, 851, 804, 758, 638 };
//                         a      b     c     d     e     f     g  eflat/h      

int highTonedurations[] = {2273, 2025, 1911, 1703, 1517, 1432, 1276, 1607, 2408, 1204, 758, 2025 };
//                            A    B     C     D     E     F     G  eflat/H  I   J/Ab   k/     L/B

     

void setup()
{
  pinMode(step1, OUTPUT);
  pinMode(step2, OUTPUT);
  pinMode(step3, OUTPUT);
  pinMode(step4,OUTPUT);
  Serial.begin(9600);
  delay(3000);
  duration = 30000000/bpm;
  pause = 30000/bpm;
}

void loop()
{
  char ch;
  if(Serial.available())
  {
ch = Serial.read();
  {
    if(ch == 't')
      {
        if(forwards == 0)
          { 
            step1 = 5;
            step2 = 4;
            step3 = 3;
            step4 = 2;
            forwards = 1;
          }
        else if(forwards == 1)
          { 
            step1 = 2;
            step2 = 3;
            step3 = 4;
            step4 = 5;
            forwards = 0;
           }
      }
    if (ch == 'W')
    {
      //Serial.println("hi");
      slide = 1607;
      x = slide + slideEnd;
      x = x/2;
      cycles = 113;
      cyclesDone = 0;
      while (cycles >= cyclesDone)
      {
        digitalWrite(step1,HIGH);
        delayMicroseconds(slide);
        digitalWrite(step1,LOW);
        
        digitalWrite(step2,HIGH);
        delayMicroseconds(slide);
        digitalWrite(step2,LOW);
        
        digitalWrite(step3,HIGH);
        delayMicroseconds(slide);
        digitalWrite(step3,LOW);
        
        digitalWrite(step4,HIGH);
        delayMicroseconds(slide);
        digitalWrite(step4,LOW);
        slide ++;
        cyclesDone++;
      }
    }
    else if (ch == ' ')
    {
      delay(pause);
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
}
void playNote(long timePeriod)
{

  cycles =  duration/timePeriod;
  cycles = cycles/4;
  timePeriod = timePeriod - 20;    //it takes 20 us to do the lines.
/*  Serial.print("duration = ");
  Serial.println(duration);
  Serial.print("cycles = ");
  Serial.println(cycles);
  Serial.print("timePeriod = ");
  Serial.println(timePeriod);
  Serial.println();

   */
  
  while (cyclesDone < cycles)
  {
 // Serial.println(cyclesDone);
digitalWrite(step1, HIGH);
delayMicroseconds(timePeriod);
digitalWrite(step1, LOW);

digitalWrite(step2, HIGH);
delayMicroseconds(timePeriod);
digitalWrite(step2, LOW);

digitalWrite(step3, HIGH);
delayMicroseconds(timePeriod);
digitalWrite(step3, LOW);

digitalWrite(step4, HIGH);
delayMicroseconds(timePeriod);
digitalWrite(step4, LOW);
cyclesDone++;
}
cyclesDone = 0;
}



