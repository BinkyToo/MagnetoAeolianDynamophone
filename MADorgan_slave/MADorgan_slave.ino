
//String notes[12]  = {"A","Bb","B","C","C#","D","Eb","E","F","F#","G","G#"};  //mapping note to pin number
//int pinNumber[12] = { 2 ,   3, 4 ,  5,   6,  7,   8,  9, 10,  11, 12, 13};
String notes[12]  = {"C", "D", "E", "F", "G", "A", "B", "c", "d"};  //mapping note to pin number
int pinNumber[12] = { 12 ,  3,   4 ,  5,   6,  7,   8,   9,  10};
boolean states[12];      //each pin on or off
boolean changeFlag = 0;  //the changeFlag is set to 1 whenever an output changes so setPins is only called when needed
unsigned long timer;     //used to time things...
int pointer = 0;         //not a c++ style pointer to memory, just something to point to the correct value in the arrays
int timeRemaining[12] = {0,0,0,0,0,0,0,0,0,0,0,0};

const String address = "orgn1";   // Address used for recognising incoming commands
String addressstring = "";          // Initialise strings which will be populated by processing incomign commands
String pitchstring = "";
String timestring = "";


void setup() 
{
  Serial.begin(9600);
  Serial.setTimeout(5);   //if gibberish is sent then Serial.readStringUntil(':') only waits for 5ms before giving up
  for(int i=0; i<=8; i++)  //set all the pins to output and test all the recorders! 
  {                        //I've always liked the idea of the recorders going doing this
    pinMode(pinNumber[i],OUTPUT);     //when the module is first switched on!
    digitalWrite(pinNumber[i],HIGH);
    delay(500);
    digitalWrite(pinNumber[i],LOW);
  }

}

void loop() 
{
  while(Serial.available()) 
  {
    addressstring = Serial.readStringUntil(':');
    if (addressstring == "")
       break;
    pitchstring = Serial.readStringUntil(',');
    timestring = Serial.readStringUntil('\n');
    
    if(addressstring == address)
    {
      changeFlag = 1;            //the changeFlag is set to 1 whenever an output changes so setPins is only called when needed
      pointer = returnIndex(pitchstring);          //function returns -1 if no notes are found in pitchstring
      if(pointer != -1)                            //if there was a valid note in pitchstring then add the time to the timRemaining
      timeRemaining[pointer] = timestring.toInt(); 
    }
  }
  
  
  
  if((millis() - timer) >=5)    //I'm using 5ms blocks for all the serial read to happen because using millis() is more precise than delay() but wont work one ms at once
  {
    for(int i=0; i<12; i++)
    {
      if(timeRemaining[i] <= 0) //if no time left then switch the pin off
      {
        states[i] = 0;
      }
      
      else
      {
        states[i] = 1;            //switch the pin on
        timeRemaining[i] -= 5;    //remove 5 ms from time remaining
        if(timeRemaining[i] <= 0) //if by doing this the time goes to 0
        {
        changeFlag = 1;           //switch off the pin
        states[i] = 0;            //and set the changeFlag 
        }
      }
    }
    timer = millis();            //reset the timer
  }
  
  if(changeFlag)    //equivalent to if(changeFlag == 1)
  {
    changeFlag = 0; //obvious
    setPins();      //switch on/off pins
  }

}

int returnIndex(String note)
{
  int index =-1;          //make default -1
  for(int i=0; i<12; i++) 
  {
    if(note == notes[i]) //if the note is found in the array, return it's index
    {                    //This is so much easier in python!
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
    digitalWrite(pinNumber[i],states[i]); //go through all the pin numbers and switch on/off each pin
  }
}
