
#include <TFT_eSPI.h> // Hardware-specific library
#include <SPI.h>

TFT_eSPI tft = TFT_eSPI();                   // Invoke custom library with default width and height

#define LTRED     0xFD34
#define BLUE      0x001F
#define GREEN     0x07E0
#define CYAN      0x07FF
#define RED       0xF800
#define MAGENTA   0xF81F
#define YELLOW    0xFFE0
#define ORANGE    0xFC00
#define PINK      0xF81F
#define PURPLE    0x8010
#define GREY      0xC618
#define WHITE     0xFFFF
#define BLACK     0x0000
#define LTGREY    0xE71C

//Global Variables
boolean display1 = true;
boolean update1 = true;
int w = 240;
int h = 135;
int scaleY= 26;
int scaleX = 1;
//for bpm
int bpm = 0;
int beat_new = 0;
int beat_old = 0;
const int rrTime = 20 ;
float beats[rrTime];  // Used to calculate average BPM
int beatIndex = 0;
float threshold = 2480.0;  //Threshold at which BPM calculation occurs
boolean belowThreshold = true;
String hr = "bpm";


void setup() {
//screen
  tft.begin();
  tft.setRotation(1);
  delay(2000);
  tft.setTextSize(2);
  tft.setTextColor(GREEN);
  drawInfo();

//AD8232
  pinMode(15, INPUT); // Setup for leads off detection LO +
  pinMode(13, INPUT); // Setup for leads off detection LO -
  pinMode(2, OUTPUT); // Setup SDN enable pin HIGH to enable
  digitalWrite(2,HIGH);
                      //analog pin 12 INPUT
//Button input
  pinMode(0, INPUT);   //Left button
  pinMode(35, INPUT);  //Right button

//BT communication
  Serial.begin(9600);

//bpm setup
  for (int i = 0; i < rrTime; i++){
    beats[i] = 60; //avg
    }
  beat_old = millis();
}   


// loop global variable
int px =0, py =0;
int x =0, y=0;
int d = 0;
int frameRate = 0;

void loop(void) {
  delay(10); //sample rate
  if((digitalRead(13) == 1)||(digitalRead(15) == 1)){
    d = 2048;
  }
  else{
    d = analogRead(12); //read data from AD8232
  }
  //plot
  frameRate++;
  if(frameRate >= 0){
    frameRate = 0;
    px = x;
    py = y;
    x += scaleX;
    y = 135-(d/scaleY);
    tft.drawLine(px,py,x,y,BLUE);
    if (x>=w){
      drawInfo();
      px =0;
      x =0;
    }
  }

//check left button if pressed then change scaleY amplitude
  if(!digitalRead(0)){
    delay(100);
    if(!digitalRead(0)){
      scaleY -=2;
    }
  }
  if (scaleY <= 22){
    scaleY = 26; 
    }
//check right button if pressed then change scaleX time
  if(!digitalRead(35)){
    delay(100);
    if(!digitalRead(35)){
      scaleX += 1;
    }
  }
  if (scaleX > 3){
    scaleX = 1; 
  }

//check bpm
  if (d > threshold && belowThreshold == true)
      {
        calculateBPM();
        belowThreshold = false;
      }
      else if(d < threshold)
      {
        belowThreshold = true;
      }
}

void drawInfo(){ //clear screen
  tft.fillScreen(BLACK);
  //tft.drawLine(0, h/2, w, h/2, LTGREY);
  //tft.drawLine(w/2, 0, w/2, h, LTGREY);
  tft.drawString("bmp: " + String(bpm), 5, 5); //draw bmp
  tft.drawString(String(scaleX)+"," + String(scaleY), w-80, 5); //draw scale x y
  
  }

void calculateBPM () {  
  int beat_new = millis();    // get the current millisecond
  int diff = beat_new - beat_old;    // find the time between the last two beats
  float currentBPM = 60000 / diff;    // convert to beats per minute
  beats[beatIndex] = currentBPM;  // store to array to convert the average
  float total = 0.0;
  for (int i = 0; i < rrTime; i++){
    total += beats[i];
    }
  bpm = int(total / rrTime);
  beat_old = beat_new;
  beatIndex = (beatIndex + 1) % rrTime;  // cycle through the array instead of using FIFO queue
  if (bpm>300){
    for (int i = 0; i < rrTime; i++){
    beats[i] = 60; //avg
    }
  }
  }
