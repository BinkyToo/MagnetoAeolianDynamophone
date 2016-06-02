
void lcdclear(){
  //Serial1.begin(9600,SERIAL_8N2);  // two stop bits (not default)
  //Serial1.begin(2400);  // two stop bits (not default)
  //Serial1.write(0x13);             // Turn backlight on
  //Serial1.write(0x04);             // Turn cursor off
  //Serial1.print('\f');             // Form feed (return to top left?)
  //Serial1.write(254);
  //Serial1.write(128);
  OLED.clearScreen();
}

void movelcdcursortochar(int place){
  //Serial1.write(0x02);                    // Next byte is a screen position
  //Serial1.write(1+place);                 // FIXME are we using 1 or 0 indexing?
  OLED.cursorRight(place);
  
}

void movelcdcursortoline(int line){       // Pretty much like above
  //Serial1.write(0x02);
  //Serial1.write(1+(line*20));
  if (line==1){
    OLED.cursorPosition(1,0);
  }
  else if (line==2){
    OLED.cursorPosition(2,0);
  }
  else if (line==3){
    OLED.cursorPosition(1,20);
  }
  else if (line==4){
    OLED.cursorPosition(2,20);
  }
}

