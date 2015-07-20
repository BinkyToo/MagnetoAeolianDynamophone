void movelcdcursortochar(int place){
  Serial1.write(0x02);                    // Next byte is a screen position
  Serial1.write(1+place);                 // FIXME are we using 1 or 0 indexing?
}

void movelcdcursortoline(int line){       // Pretty much like above
  Serial1.write(0x02);
  Serial1.write(1+(line*20));
}

void lcdsetup(){
  Serial1.begin(9600,SERIAL_8N2);  // two stop bits (not default)
  Serial1.write(0x13);             // Turn backlight on
  Serial1.write(0x04);             // Turn cursor off
  Serial1.print('\f');             // Form feed (return to top left?)
}
