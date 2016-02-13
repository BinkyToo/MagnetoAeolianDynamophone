
int ding = 2;  //possibly dong
int dong = 3;  //possibly ding

int dingTime = 40; //38 seems to be the minimum time to get a ding but will be quieter
int space = 1000;
void setup() 
{
pinMode(ding,OUTPUT);
pinMode(dong,OUTPUT);
}

void loop() 
{
digitalWrite(ding,HIGH); //Does this really need a comment?

delay(dingTime);
digitalWrite(ding,LOW);
delay(space-dingTime);
digitalWrite(dong,HIGH);
delay(dingTime);
digitalWrite(dong,LOW);
delay(space-dingTime);
}
