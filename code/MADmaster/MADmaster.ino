#include <SPI.h>
#include <SD.h>
#include <AXE133Y.h>

const int vernum = 1;

String dummy[20] = {};
String fileindex[10] = {};
String alsoadummy[20] = {};
String selectedfile = "notafile.abc";
File sequence;
int playedsofar = 0;
int selectedfilenum = 0;
int numberoffiles = 0;
int scrolloffset = 0;

float progress = 0;
enum {SPLASH, FILES, PLAYING, ERR};
enum {NONE, NOFILES};
int uistate = SPLASH;
int olduistate = SPLASH;
int error = NONE;

String title = "";

AXE133Y OLED = AXE133Y(6);

const int upButton = 2;
const int downButton = 4;
const int leftButton = 3;
const int rightButton = 5;
bool buttonPressed = false;

void setup() {
  pinMode(upButton, INPUT_PULLUP);
  pinMode(downButton, INPUT_PULLUP);
  pinMode(leftButton, INPUT_PULLUP);
  pinMode(rightButton, INPUT_PULLUP);
  
  lcdclear();                         // Includes setup of serial to LCD
  Serial.begin(9600);                 // Serial over USB for debug
  Serial2.begin(9600);                // Serial to  slave arduino(s)
  
  drawsplash(vernum);
  uistate = FILES;                    // Should later draw the list of files to browse
  scansdcard();  
  delay(1000);
  Serial.println("\n---\nFinished setup");
}

void loop() {
  handlecommand();                    // Check if there is a command from the default serial connection, and process it
  
  switch(uistate) {                   // These are displayed material which changes spontaneously (e.g. by time)
    case SPLASH:
    break;
    case FILES:                       // Nothing here; file browser is kinda static
    break;
    case PLAYING:
      drawprogressbar(progress);
      if (sequence.available()) {
        playedsofar++;
        char ch = sequence.read();
        switch (ch){
          case '*':
            Serial2.print("drum:*,\n");   // This is a command packet using a (probably) unique structure. Needs better documentation.
            //delay(100);
          break;
          case 'x':                             // Play nothing, but still take up a note's worth of time
            //delay(100);
          break;
        }
        if ((ch >= 'A' and ch <= 'G') || (ch >= 'a' and ch <= 'g')) {
          Serial2.print("orgn0:"); Serial2.print(ch); Serial2.print(",");
          Serial.print("orgn0:");  Serial.print(ch);  Serial.print(",");
        }
        if (ch >= '0' and ch <='9'){
          Serial2.print(ch); Serial2.print("00\n");
          Serial.print(ch);  Serial.print("00\n");
          delay(150*(ch-48));
        }
        //delay(100);                // Wait for end device to process things? not sure. Ungly. FIXME
        progress = ((float)playedsofar/(playedsofar+sequence.available()));   // Progress through track from 0 to 1
      }
      else{
        sequence.close();                       // Have got to end of file now
        uistate = FILES;                        // Go back to file browser
      }
    break;
    case ERR:                               // Nothing here - errors are static
    break;
  }
  
  tryuistatechange();                       // Has the UI changed state? If so draw the new screen, once only
}

void scansdcard(){
  Serial.println("Scanning SD card...");
  SD.begin(4);
  File root;                                // Actually a directory
  root = SD.open("/");
  root.rewindDirectory();
  int oldnumberoffiles = numberoffiles;
  numberoffiles = 0;
  for(int i=0; i<10; i++){
    fileindex[i] = "";
  }
  while(numberoffiles<10) {
     File entry =  root.openNextFile();
     if (! entry) {
       break;                              // Leave the while loop, no more files
     }
     fileindex[numberoffiles] = entry.name(); // Add file name to an array of all the files on the SD card
     Serial.println("\t" + fileindex[numberoffiles]);
     numberoffiles++;
     entry.close();                       // Can't have too many files open at once
   }
   if (numberoffiles == 0){
     Serial.println("\tDidn't find any files");
     uistate = ERR;
     error = NOFILES;
   }
   root.close();                          // Ditto for dirs
   Serial.println("Done");                // This needed for clarification of jammed processing or poor no-files handling
   if (numberoffiles!=oldnumberoffiles){
     Serial.println("Number of files on SD card has changed!");
     Serial.print("\tWas "); Serial.print(oldnumberoffiles); Serial.print(", now "); Serial.println(numberoffiles);
   }
}

void tryuistatechange(){
  if (uistate != olduistate){
    Serial1.print('\f');          // If UI state has changed, definitely need to clear and redraw screen
    switch(uistate) {
    case SPLASH:
      drawsplash(vernum);
    break;
    case FILES:
      drawfiles();
    break;
    case PLAYING:
      drawnowplaying();
    break;
    case ERR:
      drawerror();
    break;
    }
  olduistate = uistate;
  }
}

void handlecommand(){             // Deal with any incomming commands on serial link
  char command = ' ';
  if (digitalRead(upButton) == false){
    command = '-';
    buttonPressed = true;
  }
  if (digitalRead(downButton) == false){
    command = '+';
    buttonPressed = true;
  }
  if (digitalRead(leftButton) == false){
    command = 'f';
    buttonPressed = true;
  }
  if (digitalRead(rightButton) == false){
    command = 'p';
    buttonPressed = true;
  }
  if (Serial.available() || buttonPressed){
    if (Serial.available()){
      command = Serial.read();
    }
    switch (command){
      case '+':
        Serial.println("Moving cursor down by one line");
        if ((selectedfilenum+1) < numberoffiles){selectedfilenum++;}
        if ((selectedfilenum-scrolloffset)>=4){ scrolloffset++; }
        drawfiles();
      break;
      case '-':
        Serial.println("Moving cursor up by one line");
        if (selectedfilenum > 0){selectedfilenum--;}
        if ((selectedfilenum-scrolloffset)<0){ scrolloffset--; }
        drawfiles();
      break;
      case 'p':
        Serial.print("Attempting to begin playback of ");
        selectedfile = fileindex[selectedfilenum];
        Serial.println(selectedfile);
        scansdcard();
        if (numberoffiles != 0) {
          Serial.println("Card still present; playing");
          playedsofar = 0;
          uistate = PLAYING;
          sequence.close();     // Avoid having lots of open files
          sequence = SD.open(selectedfile);
        }
        else{
          Serial.println("File seems to have disappeared! Not playing.");
          uistate = ERR;
          error = NOFILES;
        }
      break;
      case 'f':
        Serial.println("Entering file browser by user's request");
        sequence.close();
        uistate = FILES;
      break;
      case 'r':
        Serial.println("Rescanning SD card by user's request");
        scansdcard();
        drawfiles();
      break;
    }
  }
  buttonPressed = false;
}


