#include <SPI.h>
#include <SD.h>

File root;
String fileindex[4] = {"No Files.", "-", "-", "-"};
String selectedfile = "notafile.abc";

const int vernum = 0;
float x=0;
enum{ SPLASH, FILES, PLAYING};
int uistate = SPLASH;
int olduistate = SPLASH;

void setup() {
  serialsetup();
  drawsplash(vernum);
  SD.begin(4);
  root = SD.open("/");
  scansdcard(root);  
  delay(1000);
  uistate = FILES;
}

void loop() {
  switch(uistate) {
    case SPLASH:
    break;
    case FILES:
    break;
    case PLAYING:
      drawprogressbar(x);
    break;
  }
  tryuistatechange();
  x+=0.05;
  if (x>1){
    x = 0;
  }
  delay(1000);
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

void serialsetup(){
  Serial1.begin(9600,SERIAL_8N2);
  Serial1.write(0x13);             // Turn backlight on
  Serial1.print('\f');
}

void drawfiles(){
  Serial1.print('\f');
  for (int line = 0; line < 4; line++){
    movelcdcursortoline(line);
    Serial1.print("> ");
    Serial1.println(fileindex[line]);
  }
}

void drawnowplaying(){
  Serial1.print("\f    Now Playing:    ");
  movelcdcursortoline(1);
  Serial1.print("    ");
  Serial1.print(selectedfile);
}

void scansdcard(File dir){
  int i = 0;
  while(i<255) {
     File entry =  dir.openNextFile();
     if (! entry) {
       //fileindex[0] = "No Files!";
       break;
     }
     fileindex[i] = entry.name();
     entry.close();
     i++;
   }
}

void movelcdcursortoline(int line){
  Serial1.write(0x02);
  Serial1.write(1+(line*20));
}

void tryuistatechange(){
  if (uistate != olduistate){
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

