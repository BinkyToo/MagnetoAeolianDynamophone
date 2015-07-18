#include <SPI.h>
#include <SD.h>

const int vernum = 0;

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
enum{ SPLASH, FILES, PLAYING, ERR};
enum {NONE, NOFILES};
int uistate = SPLASH;
int olduistate = SPLASH;
int error = NONE;


void setup() {
  Serial.begin(9600);
  Serial2.begin(9600);
  
  lcdsetup();
  drawsplash(vernum);
 
  scansdcard();  
  delay(1000);
  uistate = FILES;
  Serial.println("Finished setup");
}


void loop() {
  handlecommand();
  switch(uistate) {
    case SPLASH:
    break;
    case FILES:
    break;
    case PLAYING:
      drawprogressbar(progress);
    break;
    case ERR:
    break;
  }
  if (sequence.available()) {
    playedsofar++;
    switch (sequence.read()){
      case '*':
        Serial2.print("percussion:*,\n");
        delay(100);
      break;
      case '_':
        delay(100);
      break;
    }
    progress = ((float)playedsofar/(playedsofar+sequence.available()));
  }
  else{
    sequence.close();
    uistate = FILES;
  }
  tryuistatechange();
}

void scansdcard(){
  Serial.println("Scanning SD card...");
  SD.begin(4);
  File root;
  root = SD.open("/");
  int oldnumberoffiles = numberoffiles;
  numberoffiles = 0;
  for(int i=0; i<10; i++){
    fileindex[i] = "";
  }
  while(numberoffiles<10) {
     File entry =  root.openNextFile();
     if (! entry) {
       break;
     }
     fileindex[numberoffiles] = entry.name();
     Serial.println("\t" + fileindex[numberoffiles]);
     numberoffiles++;
     entry.close();
   }
   root.close();
   Serial.println("Done");
   if (numberoffiles!=oldnumberoffiles){
     Serial.println("Number of files on SD card has changed!");
     Serial.print("\tWas "); Serial.print(oldnumberoffiles); Serial.print(", now "); Serial.println(numberoffiles);
   }
}

void movelcdcursortochar(int place){
  Serial1.write(0x02);
  Serial1.write(1+place);
}

void movelcdcursortoline(int line){
  Serial1.write(0x02);
  Serial1.write(1+(line*20));
}

void lcdsetup(){
  Serial1.begin(9600,SERIAL_8N2);
  Serial1.write(0x13);             // Turn backlight on
  Serial1.write(0x04);             // Turn cursor off
  Serial1.print('\f');
}

void tryuistatechange(){
  if (uistate != olduistate){
    Serial1.print('\f');
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

void handlecommand(){
  if (Serial.available()){
    char command = Serial.read();
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
          sequence = SD.open(selectedfile);
        }
        else{
          Serial.println("File seems to have disappeared! Not playing.");
          uistate = ERR;
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
}

