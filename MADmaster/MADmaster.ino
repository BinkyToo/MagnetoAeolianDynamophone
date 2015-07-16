#include <SPI.h>
#include <SD.h>

const int vernum = 0;

String fileindex[10] = {};
String selectedfile = "notafile.abc";
File sequence;
int playedsofar = 0;
int selectedfilenum = 0;
int numberoffiles = 0;
int scrolloffset = 0;

float progress = 0;
enum{ SPLASH, FILES, PLAYING};
int uistate = SPLASH;
int olduistate = SPLASH;


void setup() {
  //begindebug
  Serial.begin(9600);
  //enddebug

  Serial2.begin(9600);
  
  lcdsetup();
  drawsplash(vernum);
 
  scansdcard();  
  delay(1000);
  uistate = FILES;
}


void loop() {
  handlecommands();
  switch(uistate) {
    case SPLASH:
    break;
    case FILES:
    break;
    case PLAYING:
      drawprogressbar(progress);
    break;
  }
  if (sequence.available()) {
    //Serial.write(sequence.read());
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
    playedsofar = 0;
  }
  tryuistatechange();
}

void scansdcard(){
  SD.begin(4);
  File root;
  root = SD.open("/");
  numberoffiles = 0;
  for(int i=0; i<10; i++){
    fileindex[i] = "";
  }
  int i = 0;
  while(i<10) {
     File entry =  root.openNextFile();
     if (! entry) {
       break;
     }
     fileindex[i] = entry.name();
     numberoffiles++;
     entry.close();
     i++;
   }
   root.close();
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
    }
  olduistate = uistate;
  }
}

void handlecommands(){
  while (Serial.available()){
    char command = Serial.read();
    switch (command){
      case '+':
        if ((selectedfilenum+1) < numberoffiles){selectedfilenum++;}
        if ((selectedfilenum-scrolloffset)>=4){ scrolloffset++; }
        drawfiles();
      break;
      case '-':
        if (selectedfilenum > 0){selectedfilenum--;}
        if ((selectedfilenum-scrolloffset)<0){ scrolloffset--; }
        drawfiles();
      break;
      case 'p':
        uistate = PLAYING;
        selectedfile = fileindex[selectedfilenum];
        sequence = SD.open(selectedfile);
      break;
      case 'f':
        sequence.close();
        uistate = FILES;
      break;
      case 'r':
        scansdcard();
        drawfiles();
      break;
    }
  }
}

