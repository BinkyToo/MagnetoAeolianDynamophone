const int vernum = 0;
float x=0;

void setup() {
  serialsetup();
  drawsplash(vernum);
  delay(1000);
}

void loop() {
  drawprogressbar(x);
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
  Serial1.write(0x02);
  Serial1.write(61);
  Serial1.print(output);
}

void serialsetup(){
  Serial1.begin(9600,SERIAL_8N2);
  Serial1.write(0x13);             // Turn backlight on
  Serial1.print('\f');
}

