
String notes[12]  = {"A","Bb","B","C","C#","D","Eb","E","F","F#","G","G#"}; //lowercase for sharps
int pinNumber[12] = { 2 ,   3, 4 ,  5,   6,  7,   8,  9, 10,  11, 12, 13};
boolean states[12];
boolean changeFlag = 0;
unsigned long timer;
int pointer = 0;
int timeRemaining[12] = {0,0,0,0,0,0,0,0,0,0,0,0};

const String address = "orgn1";   // Address used for recognising incoming commands
String addressstring = "";          // Initialise strings which will be populated by processing incomign commands
String pitchstring = "";
String timestring = "";


void setup() 
{
  Serial.begin(9600);
  for(int i=3; i<14; i++)
  {
    pinMode(i,OUTPUT);
    digitalWrite(i,HIGH);
    delay(70);
    digitalWrite(i,LOW);
  }

}

void loop() 
{
  while(Serial.available()) 
  {
    //Serial.print("hello");
    addressstring = Serial.readStringUntil(':');
    pitchstring = Serial.readStringUntil(',');
    timestring = Serial.readStringUntil('\n');
    
    if(addressstring == address)
    {
      changeFlag = 1;
      pointer = returnIndex(pitchstring);
      if(pointer != -1)
      timeRemaining[pointer] = timestring.toInt();
    }
  }
  
  
  
  if((millis() - timer) >=5)
  {
    //Serial.println("hello");
    for(int i=0; i<12; i++)
    {
      if(timeRemaining[i] <= 0)
      {
        states[i] = 0;
      }
      
      else
      {
        states[i] = 1;
        timeRemaining[i] -= 5;
        if(timeRemaining[i] <= 0)
        {
        changeFlag = 1;
        states[i] = 0;
        }
      }
    }
    timer = millis();
  }
  
  if(changeFlag)    //equivalent to if(changeFlag == 1)
  {
    changeFlag = 0;
    setPins();
  }

}

int returnIndex(String note)
{
  int index =-1;
  for(int i=0; i<12; i++)
  {
    if(note == notes[i])
    {
      index = i;
      break;
    }
  }
  return index;
}

void setPins()
{
  for(int i=0; i<12; i++)
  {
    digitalWrite(pinNumber[i],states[i]);
  }
}
