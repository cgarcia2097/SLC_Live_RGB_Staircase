#include <Adafruit_NeoPixel.h>

// Define pins used here
#define TRIG A15
#define ECHO A14
#define PIX A16
#define NUM_READINGS 3
#define TRIP_DISTANCE 200

// Initialize some variables
int i=0;
const long trip = TRIP_DISTANCE;
long avgDistance, currentDistance[NUM_READINGS];

// Create your Neopixel object
Adafruit_NeoPixel strip = Adafruit_NeoPixel(150, PIX, NEO_GRB + NEO_KHZ800);

void setup() {
  //Set your pins up here:
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);

  // Set up your Neopixel object
  strip.begin();
  strip.show();
  
  Serial.begin(9600); 
  
}

void loop() {
  // Take Ultrasonic readings and average them
  for(i=0; i<NUM_READINGS; i++){
    currentDistance[i] = getDistance(TRIG,ECHO);
  }

  avgDistance = average(currentDistance, NUM_READINGS);

  // Create trip scenario with a trip distance
    if(avgDistance < trip){
      Serial.println("Tripped");

        // Start doing effects here:
          theaterChaseRainbow(20);
          colorWipe(strip.Color(0,0,0), 50);
    }
    else{
      Serial.print("Distance: ");
      Serial.print(avgDistance);
      Serial.print("cm \n");
    }
  
}

/*
***********************************************************
*Functions go here
***********************************************************
*/
// Ultrasonic Sensor functions
int getDistance(int trigPin, int echoPin){
  unsigned long distance = 0;
  
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  delayMicroseconds(1);
  
  distance = pulseIn(echoPin, HIGH)/58.4;

  if(distance > 400){
    distance = 400;
    Serial.print("Out of range!");
  }
  else if(distance < 5){
    distance = 5;
    Serial.print("Too close!");
  }
  
  delay(50);
  return(distance);
}

// Average function for smoothing purposes
int average(long *buff, int num_elements){
  int i = 0;
  long total = 0, average = 0;
    while(i < num_elements){
      total = total + buff[i];
      i++;
    }
  average = total / num_elements;
  return(average);
}


// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}

void rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i+j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

//Theatre-style crawling lights.
void theaterChase(uint32_t c, uint8_t wait) {
  for (int j=0; j<10; j++) {  //do 10 cycles of chasing
    for (int q=0; q < 3; q++) {
      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, c);    //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(uint8_t wait) {
  for (int j=0; j < 256; j++) {     // cycle all 256 colors in the wheel
    for (int q=0; q < 3; q++) {
      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, Wheel( (i+j) % 255));    //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}

