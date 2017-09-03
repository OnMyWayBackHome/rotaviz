/****************************************************************
 * Use the ht16k33 library to turn on and off LEDs for the 
 * persistence of vision lightshow project.
 */
#include "ht16k33.h"
// Define the class
HT16K33 HT;

int numOfImages = 6;
int sizeOfImages = 32;
int delayBetweenColumns = 10;
int delayAtEnds = 100;

// Array of images to display. Each image is made up of 16 longs; one long per column. C0R1 is most sig. bit. 
unsigned long images[6][32] = {
// a.bmp
{3, 15, 31, 255, 4095, 65535, 131071, 1048568, 8388576, 134217216, 536870400, 536813056, 1073487360, 4290780672, 4261420544, 4293926400, 1073683968, 536845824, 268434944, 33554176, 4194300, 1048572, 65535, 16383, 2047, 511, 31, 7, 1, 0, 0, 0, }, 
// heart.bmp
{268369920, 1073725440, 2114973696, 4026660864, 4026563584, 3758104320, 3221229440, 3221227456, 3221226432, 3758096864, 3758096624, 4026531960, 2013265976, 1006633020, 520093724, 260046862, 260046862, 520093726, 1006633020, 2013265976, 4026531960, 3758096624, 3758096864, 3221226432, 3221227456, 3221229440, 3758104320, 3758128640, 4026660864, 2080894976, 1073725440, 268369920, }, 
// smiley.bmp
{1044480, 8388096, 16776960, 66592704, 130024416, 251658480, 503316600, 469762108, 939524124, 2015232030, 1883177998, 1883177486, 4030661391, 3760062727, 3758096647, 3758096647, 3758096647, 3758096647, 3758096647, 4028498703, 1883177486, 1883178510, 2017394718, 941490204, 469762108, 503316600, 251658480, 130024416, 66592704, 33554304, 8388096, 1044480, }, 
// test6x6.bmp
{3758153728, 1610670080, 536928256, 24576, 8192, 3758153728, 1610670080, 536928256, 24576, 8192, 3758153728, 1610670080, 536928256, 24576, 8192, 3758153728, 1610670080, 536928256, 24576, 8192, 3758153728, 1610670080, 536928256, 24576, 8192, 3758153728, 1610670080, 536928256, 24576, 8192, 0, 0, }, 
// tmp32x32.bmp
{0, 0, 1556496, 267415776, 402657664, 536875520, 1614549504, 1105596928, 2399371776, 2550144512, 2415919104, 2684354560, 2692743168, 2692743168, 2692808704, 3758161920, 3758694400, 2688105472, 2694390784, 2558551040, 2264974336, 3252782080, 1614742528, 1060897792, 31458304, 1536, 512, 768, 0, 0, 0, 0, }, 
};
/****************************************************************/
void setup() {
  Serial.begin(57600);
  Serial.println(F("ht16k33 light test v0.01"));
  Serial.println();
  // initialize everything, 0x00 is the i2c address for the first one (0x70 is added in the class).
  HT.begin(0x00);
}

/****************************************************************/
void loop() {
  uint8_t led;

  Serial.println(F("Turn on all LEDs"));
  // first light up all LEDs
  for (led=0; led<32; led++) {
    HT.setLedNow(led);
    delay(10);
  } // for led

  Serial.println(F("Clear all LEDs"));
  //Next clear them
  for (led=0; led<32; led++) {
    HT.clearLedNow(led);
    delay(10);
  } // for led

  //Now do one by one, slowly, and print out which one
  for (led=0; led<32; led++) {
    HT.setLedNow(led);
    Serial.print(F("Led no "));
    Serial.print(led,DEC);
    Serial.print(F(": On"));
    delay(1000);
    HT.clearLedNow(led);
    Serial.println(F(" Off"));
  } // for led

  for (int i=0; i<5; i++) {

    displayImage(images[4]);
  }
} // loop  

void displayImage(unsigned long image[]) {  
  // Display the image on the LEDs forward
  for (int col=0; col<32; col++) {
    displayColumn(images[4][col]);
  }
  delay(delayAtEnds);

  // Display the image backward
  for (int col=31; col>=0; col++) {
    displayColumn(images[4][col]);
  }
  delay(delayAtEnds);
}

void displayColumn(unsigned long column) {
  // Send the first 16 bits to the HT16K33
  int com = 0; // the first WORD is on COM0 pin
  unsigned long bitPointer = 1; 
  for (int i=1; i<32; i++) {
    bitPointer = bitPointer << 1; //This will be used to check each bit in the LONG
  }
  for (com=0; com<2; com++) {  //The first word is on COM0 pin, the second is on COM1 pin
    for (int row=0; row<16; row++) {
      if (column & bitPointer) {  //bitwise AND the LONG with the pointer. If there is a 1 in the long at the bitPointer position, it will evaluate to  True
        HT.setLed(row*8 + com);
      } else {
        HT.clearLed(row*8 + com);
      }  
      bitPointer  = bitPointer >> 1;   
    } //end for each row
  } //end for each WORD
  HT.sendLed(); //turn on the LEDs for that column
  delay(delayBetweenColumns); //leave LEDs on for the set time.
} //end function     

