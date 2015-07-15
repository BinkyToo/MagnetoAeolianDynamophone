#include <SPI.h>
#include <SD.h>

int timebase = 50;        //Duration of non-whitespace characters in tune string - larger is slower - this is the fastest that the drum works well at

void setup() {
  Serial.begin(9600);
  

}

void loop() {
  // put your main code here, to run repeatedly:

}

void sendcommand(address, note, duration){
  Serial.print
}

