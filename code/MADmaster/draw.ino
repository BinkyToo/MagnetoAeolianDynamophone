void drawnowplaying(){                        // Draws the static parts of a now-playing screen
  lcdclear();
  Serial.println("Drawing now playing screen");
  OLED.print("    Now Playing:    ");
  movelcdcursortoline(2);
  movelcdcursortochar(4);
  OLED.print(selectedfile);
}

void drawfiles(){
  Serial.println("Drawing file list");
  if (numberoffiles > 0){
    lcdclear();
    for (int line = 1; line <= 4; line++){
      movelcdcursortoline(line);
      if ((-1+line+scrolloffset)==selectedfilenum){
        OLED.print("> ");
      }
      else{
        OLED.print("  ");
      }
      OLED.print(fileindex[-1+line+scrolloffset]);
    }
    movelcdcursortoline(4);
    movelcdcursortochar(15);
    OLED.print(String(selectedfilenum+1));
    OLED.print("/");
    OLED.print(String(numberoffiles));
  }
  else{
    error = NOFILES;
    uistate = ERR;
  }
}


void drawprogressbar(float progress){
  //Serial.println("Drawing track play progress bar");
  String output = "";
  for (int i=0; i<(progress*20); i++) {
    output += "=";
  }
  for (int i=(progress*20); i<20; i++) {
    //output += " ";
  }
  //Serial1.write(0x02);
  //Serial1.write(61);
  movelcdcursortoline(4);
  OLED.print(output);
}

void drawsplash(int vernum){      // Compose device splash screen
  Serial.println("Drawing splash screen");
  String output = "";
  movelcdcursortoline(2);
  OLED.print("  Magneto-Aeolian   ");
  movelcdcursortoline(3);
  output += "  Dynamophone v";
  output += vernum;
  OLED.print(output);
}



void drawerror(){
  Serial.println("Drawing error message");
  //Serial1.write(0x0c);              // Clear display
  switch(error){
    case NOFILES:
      movelcdcursortoline(2);
      movelcdcursortochar(3);
      OLED.print("No files found");
      movelcdcursortoline(3);
      OLED.print("Please check SD card");
      Serial.println("\tNo files?");
    break;
    default:
      movelcdcursortoline(2);
      movelcdcursortochar(7);
      Serial1.print("Whoops");
      movelcdcursortoline(3);
      movelcdcursortochar(1);
      Serial1.print("unclassified error");
      Serial.print("\t???");
  }
}

