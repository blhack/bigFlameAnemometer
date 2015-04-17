/* instructions for using the 8x7 segment display were adapted from: https://playground2014.wordpress.com/arduino/3461bs/
This is a common, cheap display module that is available on amazon.
The anemometer is available from adafruit
*/

const int clock = 7; //SCK
const int latch = 5; //RCK 
const int data = 6;  //DIO

int gust = 0;
int instant = 0;

byte value[] ={ B11000000, // 0
                B11111001, // 1
                B10100100, // 2
                B10110000, // 3
                B10011001, // 4
                B10010010, // 5
                B10000010, // 6
                B11111000, // 7
                B10000000, // 8
                B10010000, // 9
                B11111111};// display nothing

byte digit[] ={ B00000001, // left segment
                B00000010,
                B00000100,
                B00001000,
                B00010000,
                B00100000,
                B01000000,
                B10000000}; // right segment
void setup() {
  Serial.begin(9600);
  pinMode(clock, OUTPUT);
  pinMode(latch, OUTPUT);
  pinMode(data, OUTPUT);
}

void clearDigits() {
  digitalWrite(latch,LOW);
  shiftOut(data,clock,MSBFIRST,B11111111); // select all segments
  shiftOut(data,clock,MSBFIRST,B11111111); // display nothing
  digitalWrite(latch,HIGH); 
}

void showDigit(int index, char integer) {
  digitalWrite(latch,LOW);
  // select the fourth segment from left
  shiftOut(data,clock,MSBFIRST,digit[index]);  
  // show the digit "5" ( the array starts with 0 to count!)
  shiftOut(data,clock,MSBFIRST,value[integer]);
  digitalWrite(latch,HIGH);
}

void showGust() {
  clearDigits();
  String _gust = String(gust);
  char charBuffer[_gust.length()];
  _gust.toCharArray(charBuffer,_gust.length());
  for (int i=0; i<_gust.length(); i++) {
    showDigit(i+4, String(_gust[i]).toInt());     
  } 
}

void showInstant() {
  clearDigits();
  String _instant = String(instant);
  char charBuffer[_instant.length()];
  _instant.toCharArray(charBuffer,_instant.length());
  for (int i=0; i<_instant.length(); i++) {
    showDigit(i, String(_instant[i]).toInt());     
  }  
}


void loop() {
  int sensVal = analogRead(A0);                //the value on A0, the direct reading from anem.
  float voltage= sensVal * (5.0 / 1023.0);     //convert this value to a voltage
  float speed = ((voltage - 0.4)/2.0) * 70;    //convert the voltage to a speed.  The anem gives 0.4-2.0v for 0-70mph wind.
  if (speed < 0) {
    speed = 0;                                 //ignore negative numbers (-0.1)
  }
  instant = int(speed);                        //we only care about wind speeds in integers
  if (instant > gust) {
    gust = instant;                            //keep track of gusts
  }
  showInstant();                               //display the data
  showGust();                                  //display the data
}
