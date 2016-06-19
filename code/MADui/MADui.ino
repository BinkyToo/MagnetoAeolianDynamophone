#include <SPI.h>
#include <SD.h>

const int vernum = 0;

String fileindex[10] = {};
String selectedfile = "notafile.abc";
File sequence;
int playedsofar = 0;
int selectedfilenum = 2;
int numberoffiles = 0;
int scrolloffset = 0;

float x=0;
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
  SD.begin(4);
 
  scansdcard();  
  delay(1000);
  uistate = FILES;
}


void loop() {
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
    }
  }
  switch(uistate) {
    case SPLASH:
    break;
    case FILES:
    break;
    case PLAYING:
      drawprogressbar(x);
    break;
  }
  if (sequence.available()) {
    //Serial.write(sequence.read());
    switch (sequence.read()){
      playedsofar++;
      case '*':
        Serial2.print("percussion:*,\n");
        delay(100);
      break;
      case '_':
        delay(100);
      break;
    }
    x = (playedsofar / (playedsofar+sequence.available()));
  }
  else{
    sequence.close();
    uistate = FILES;
    int playedsofar = 0;
  }
  tryuistatechange();
  //x+=0.05;
  //if (x>1){
  //  x = 0;
  //}
  //delay(50);
}

void drawsplash(int vernum){      // Compose device splash screen
  String output = "\f";             // Form feed
  output += "                    ";
  output += "  Magneto-Aeolian   ";
  output += "  Dynamophone v";
  output += vernum;
  Serial1.print(output);
}

void drawprogressbar(float progress){
  String output = "";
  for (int i=0; i<(progress*20); i++) {
    output += "=";
  }
  for (int i=(progress*20); i<20; i++) {
    output += " ";
  }
  Serial1.write(0x02);
  Serial1.write(61);
  Serial1.print(output);
}

void lcdsetup(){
  Serial1.begin(9600,SERIAL_8N2);
  Serial1.write(0x13);             // Turn backlight on
  Serial1.write(0x04);             // Turn cursor off
  Serial1.print('\f');
}

void drawfiles(){
  Serial1.print('\f');
  for (int line = 0; line < 4; line++){
    movelcdcursortoline(line);
    if ((line+scrolloffset)==selectedfilenum){
      Serial1.print("> ");
    }
    else{
      Serial1.print("  ");
    }
    Serial1.println(fileindex[line+scrolloffset]);

  }
  movelcdcursortochar(77);
  Serial1.print(selectedfilenum+1);
  Serial1.print('/');
  Serial1.print(numberoffiles);
}

void drawnowplaying(){
  Serial1.print("\f    Now Playing:    ");
  movelcdcursortoline(1);
  Serial1.print("    ");
  Serial1.print(selectedfile);
}

void scansdcard(){
  File root;
  root = SD.open("/");
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

