void drawnowplaying(){
  Serial1.print("\f    Now Playing:    ");
  movelcdcursortoline(1);
  Serial1.print("    ");
  Serial1.print(selectedfile);
}

void drawfiles(){
  //Serial1.write(0x0c);
  for (int line = 0; line < 4; line++){
    movelcdcursortoline(line);
    if ((line+scrolloffset)==selectedfilenum){
      Serial1.print("> ");
    }
    else{
      Serial1.print("  ");
    }
    Serial1.println(fileindex[line+scrolloffset]);
    Serial.println(fileindex[line+scrolloffset]);
  }
  movelcdcursortochar(77);
  Serial1.print(selectedfilenum+1);
  Serial1.print('/');
  Serial1.print(numberoffiles);
}


void drawprogressbar(float progress){
  String output = "";
  for (int i=0; i<(progress*20); i++) {
    output += "=";
  }
  for (int i=(progress*20); i<20; i++) {
    //output += " ";
  }
  Serial1.write(0x02);
  Serial1.write(61);
  Serial1.print(output);
}

void drawsplash(int vernum){      // Compose device splash screen
  String output = "\f";             // Form feed
  output += "                    ";
  output += "  Magneto-Aeolian   ";
  output += "  Dynamophone v";
  output += vernum;
  Serial1.print(output);
}

