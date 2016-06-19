// Project 19 Tune Player

int step1 = 2;
int step2 = 3;
int step3 = 4;
int step4 = 5;
int chorde = 6;
int chordd = 7;
int chordg = 5;
long duration = 0;
long bpm = 180;
long cycles = 0;
long cyclesDone = 0;
int pause = 0;
int chordRepeats = 0;
boolean tied = 0;
boolean forwards = 0;
char ch2 = 'a';
char chord = 'a';
//int highTonedurations[] = {1911, 1703, 1517, 1351, 1276, 1136, 1012};
int highTonedurations[] = {3822, 3405, 3034, 2703, 2551, 2273, 2025};
//                         c   |  d  |  e  |  f# |  g  |  a  |  b  |  

//int lowTonedurations[] = {3822, 3405, 3034, 2703, 2551, 2273, 2025};
int lowTonedurations[] = {7644, 6810, 6060, 5406, 5102, 4546, 4050};

//                          C |  D  |  E  |  F# |  G  |  A  |  B

char* song = "B3B3B2|ABdeABde|B3B3B2|ABdeABde|gg2g2g2g2g2(ee2)e2e2 d d2 d2 f3 g e3 e2          ";
char* chords="                                ee e e  e e  ee  e e  e e  e  e  g d  e";

//                    |       |       |       

 
void setup()
{
  pinMode(step1, OUTPUT);
  pinMode(step2, OUTPUT);
  pinMode(step3, OUTPUT);
  pinMode(step4,OUTPUT);
  pinMode(chorde,OUTPUT);
  pinMode(chordd,OUTPUT);
  pinMode(chordg,OUTPUT);
  digitalWrite(chorde,LOW);
  digitalWrite(chordd,LOW);
  digitalWrite(chordg,LOW);
  Serial.begin(9600);
  pause = 30000/bpm;        //calculates the duration in milliseconds for each quaver
  delay(2000);
}

void loop()
{
  int i = 0;              // i is the number that tracks the character in the array
  char ch = song[0];      //makes a char variable called ch and makes ch = the first term in the array 'song'
  while (ch != 0)         //play the song whilst the array is still valid
  {
    duration = 30000000/bpm;//calculates the duration in microseconds for each quaver
    if(chordRepeats >= 1)
      {
        digitalWrite(chorde,HIGH);
        chordRepeats--;
      }
    
    if(ch == '(')  //if there is an open bracket then allow the notes to be tied
      {
        tied = 1;
        i = i + 1;
        ch = song[i];
      }
    
    if(ch == ')') //if there iis a closed bracket then don't allow the notes to be tied
      {
        tied = 0;
        i = i + 1;
        ch = song[i];
      }
   
      
    i = i + 1;              
    ch = song[i];           //reads the next value of i
    if(ch >= 48 and ch <= 57)
  {
            ch = ch - 48;
      duration = duration * ch;

  }
    i = i - 1;
    ch = song[i];
    chord = chords[i];
    if(chord == 'e')
    {
      i = i + 1;              
      ch = song[i];           //reads the next value of i
      if(ch >= 48 and ch <= 57)
  {
            chordRepeats = ch - 49;
            i = i - 1;
      ch = song[i];
  }
      digitalWrite(chorde,HIGH);
    }
    

    
    if (ch == 'z')
    {
     delay(pause);
    }
     
    if (ch >= 'C' and ch <= 'G')
    {
      playNote(lowTonedurations[ch - 'C']);
    }
    if (ch >= 'c' and ch <= 'g')
    {
      playNote(highTonedurations[ch - 'c']);
    }
    if ( ch >= 'A' and ch <= 'B')
    {
        playNote(lowTonedurations[ch - 60]);
    }
    if (ch >= 'a' and ch<'b')
    {
      playNote(highTonedurations[ch - 92]);
    }

    ch2 = song[i];
    if(ch2 == 'x' || ch2 == 'z' || ch2 == '|' || ch2 == ' ' || ch2 <= 57)
      {        
        i = i - 1;
        ch2 = song[i];
        if(ch2 == 'x' || ch2 == 'z' || ch2 == '|' || ch2 == ' ' || ch2 <= 57)
          {
            i = i - 1;
            ch2 = song[i];
            i = i + 1;
          }
        i = i + 1;
      }
     
    i = i + 1;
    ch = song[i];
   /* 
    Serial.print(ch2);
    Serial.print("\t");
    Serial.println(ch);
    */
    if(ch == ch2 && tied == 0)
  {
    if(forwards == 0)
          { 
            step1 = 2;
            step2 = 3;
            step3 = 4;
            step4 = 5;
            forwards = 1;
          }
        else if(forwards == 1)
          { 
            step1 = 5;
            step2 = 4;
            step3 = 3;
            step4 = 2;
            forwards = 0;
           }  
  }
   // i = i + 1;
  }
 delay(2000);
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
  while (cyclesDone < cycles)
    {
      //Serial.println(cyclesDone);
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

