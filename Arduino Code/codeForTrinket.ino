// LED STRIP SETUP
#include <Adafruit_NeoPixel.h>
#define PIN 4
Adafruit_NeoPixel strip = Adafruit_NeoPixel(15, PIN, NEO_GRB + NEO_KHZ800);

//LEF ANIMATION SETUP
int runner = 1;
boolean bTurnOn = true;
int delayForBlinking = 0;

//PIEZO SETUP
#define piezoPin 0
int length = 10; // the number of notes 'c', 'd', 'e', 'f', 'g', 'a', 'b', 'C' 

char notes[] = "Cddefgade "; // a space represents a rest
int beats[] = { 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 2, 4 };
int tempo = 300;
int timer = 0;
int counter = 1;

//DUST SENSOR SETUP
#define dustLedPin 2
#define dustPin 3
int delayTime=280;
int delayTime2=40;
int offTime=9680;
int dustVal=0;

//switch
#define button 1


void setup() {
  
  
  strip.begin();
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, strip.Color(0, 0, 0));
  }
  pinMode(dustLedPin,OUTPUT);
  pinMode(piezoPin, OUTPUT); 
  pinMode(button, INPUT); 
  
  strip.show();

}

void loop() {

  dustSensorRead();
  int num = map(dustVal,0,1024,0,4);
  num == 2;
  
  if (num < 2){
     colorRun (50, strip.Color(0, 255, 0));
  }
   else
  {
    int workTime = 10;
    int stopTime = -50;
    uint32_t c;
     if(num == 2){
         tempo = 300;
         c = strip.Color(255, 20, 0);
      }else if(num == 3){
         tempo = 200;
         c = strip.Color(255, 0, 0);
     }else{
         tempo = 100;
         c = strip.Color(255, 0, 0);
     }
   
     for (int i = 0; i < length; i++) {
        if (notes[i] == ' ') {
            delay(beats[i] * tempo); 
        } else {
            playNote(notes[i], beats[i] * tempo, c);
        }
     }
  }
    
    
}


void colorRun(uint8_t wait, uint32_t c){
  
  int runner0 = runner -1;
  int runner1 = runner +1;
  
 
  
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    if(i == runner0 || i == runner1 || i == runner){
         strip.setPixelColor(i, c);
    }else{
         strip.setPixelColor(i, strip.Color(0, 0, 0));
    }
  }
  strip.show();
  runner--;
  delay(wait);

  if(runner < 1){
    delay(500);
    runner = strip.numPixels()-1;
  }
  

}

void colorBlinking(uint32_t c){
  if(bTurnOn){
    for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    }
    bTurnOn = false;
  }else{
    
   for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, strip.Color(0, 0, 0));
    }
    bTurnOn = true;
  }
  strip.show();
  
}



void dustSensorRead(){
  
  digitalWrite(dustLedPin,LOW); // power on the LED
  delayMicroseconds(delayTime);
  dustVal=analogRead(dustPin); // read the dust value via pin 5 on the sensor
  delayMicroseconds(delayTime2);
  digitalWrite(dustLedPin,HIGH); // turn the LED off
  delayMicroseconds(offTime);
  
}


void playTone(int tone, int duration) {
  for (long i = 0; i < duration * 1000L; i += tone * 2) {
    digitalWrite(piezoPin, HIGH);
    delayMicroseconds(tone);
    digitalWrite(piezoPin, LOW);
    delayMicroseconds(tone);
  }
}

void playNote(char note, int duration,uint32_t c) {
  char names[] = { 'c', 'd', 'e', 'f', 'g', 'a', 'b', 'C' };
  int tones[] = { 1915*2, 1700*2, 1519*2, 1432*2, 1275*2, 1136*2, 1014*2, 956*2 };

  // play the tone corresponding to the note name
  for (int i = 0; i < 8; i++) {
    if (names[i] == note) {
      playTone(tones[i], duration);
      colorRun(0,c);
    }
  }
}

