/*
Source code taken from:
http://robotakeover.com/post/33537620873/arduino-with-single-potentiometer-and-common-anode
http://www.youtube.com/watch?v=QgxJQatc6mo
Used Common Anode RGB LED
 
 * Program: rgbled.ino
 * Author: Julian White
 * Created: 10/13/2012
 * Description: Use single 10K potentiometer to cycle through all
 * hues in the color wheel. The RGB LED is wired as common anode
 * (+5V). 330 ohm resistor wired in series with LED anode.
*/
 
// DIP LED
int redPin = 9; // goes to RED LED cathode through a 1kOhm resistor
int greenPin = 10; // goes to GREEN LED cathode through a 1kOhm resistor
int bluePin = 11;  // goes to BLUE LED cathode through a 1kOhm resistor
 
// 5050 LED
//int redPin = 10; // goes to RED LED cathode through a 1kOhm resistor
//int greenPin = 9; // goes to GREEN LED cathode through a 1kOhm resistor
//int bluePin = 11;  // goes to BLUE LED cathode through a 1kOhm resistor
 
// RGB LED Common Anode goes to +5V
int ledAnode = 8;
 
// 1 and 3 pot pins connected to GND and +5V respectively
int potPin = A0; // goes to 2 pin (niddle)
 
int val = 0;
int val1 = 0;
 
void setup()
{
    pinMode(redPin, OUTPUT);
    pinMode(greenPin, OUTPUT);
    pinMode(bluePin, OUTPUT);
    pinMode(ledAnode, OUTPUT);
 
    digitalWrite(ledAnode, HIGH);
 
    Serial.begin(115200);
}
 
void loop()
{
//    Read the potentiometer (color wheel).
//    val = analogRead(potPin); // I use UART console for sending, not Potentiometer.
//    Serial.println(val);
    if(Serial.available()) val1 = Serial.parseInt();  // Receive value from serial to a tempp variable
    if(val1 > 1023) {delay(1000); val1 = 0;};
    val = val1++;                                     // inc value every cicle (only for testing)
//    val = val1;
    
    valToColor(val);
    
    delay(10);  //Sampling rate.
}
    
    
    
    
    
    
    
    
void valToColor(uint16_t value)
{
//    if(value > 850) value = 850;
    
    uint8_t redVal;
    uint8_t greenVal;
    uint8_t blueVal;
    
      //Red-orange-yellow-green spectrum
    if ( value <= 341 )
    {
        //We now know the val is between 0 and 341.
        //Map the val to 0-255 for the LED.
        value = map(val, 0, 341, 0, 255);
        
        //Higher val turns red down.
        //Lower val turns red up (common anode).
        redVal = value;
        
        //Write the complimentary amount to the green pin.
        //The mixed color values add up to 255.
        //This keeps the LED at constant brightness.
        greenVal = 255-value;
        
        //Turn blue pin off.
        blueVal = 255;
    }
    
    //Green-teal-blue spectrum
    if (value > 341 && value <= 683)
    {
        //We now know the val is between 341 and 683.
        //Map the val to 0-255 for the LED.
        value = map(value, 341, 683, 0, 255);
        
        //Higher val turns green down.
        //Lower val turns green up (common anode).
        greenVal = value;
        
        //Write the complimentary amount to the blue pin.
        //The mixed color values add up to 255.
        //This keeps the LED at constant brightness.
        blueVal = 255-value;
        
        //Turn red pin off.
        redVal = 255;
    }
    
    //Blue-indigo-purple-red spectrum.
    if (value > 683 && value <= 850)
    {
        //We now know the val is between 683 and 1023.
        //Map the val to 0-255 for the LED.
        value = map(value, 683, 850, 0, 255);
        
        //Higher val turns blue down.
        //Lower val turns blue up (common anode).
        blueVal = value/3;
    
        //Write the complimentary amount to the red pin.
        //The mixed color values add up to 255.
        //This keeps the LED at constant brightness.
        redVal = 255 - value/2;
    
        //Turn green pin off.
        greenVal = 255;
    }
    
    if (value > 850)
    {
        //We now know the val is between 683 and 1023.
        //Map the val to 0-255 for the LED.
        value = map(value, 850, 1023, 0, 255);
        
        //Higher val turns blue down.
        //Lower val turns blue up (common anode).
        blueVal = 85; //255 - value*2/3;
    
        //Write the complimentary amount to the red pin.
        //The mixed color values add up to 255.
        //This keeps the LED at constant brightness.
        redVal = 127; //value/3;
    
        //Turn green pin off.
        greenVal = 255 - value*2/3;
    }
    
    analogWrite(redPin, redVal);
    analogWrite(greenPin, greenVal);
    analogWrite(bluePin, blueVal);
}

void valToColor1(uint16_t value)
{
  float arg, fnc;
  uint8_t redVal;
  uint8_t greenVal;
  uint8_t blueVal;
  
  if (value <= 255)
  {
    arg = ((float)(value)/255)*PI;
    fnc = ((cos(arg)+1)/2)*255;
//    Serial.println(fnc);
    redVal = fnc;   // 255 -> 0
    greenVal = 255 - fnc;  // 0 -> 255
    blueVal = 0; // 0
  }

  if (value > 255 && value <= 511)
  {
    arg = ((float)((value)-255)/255)*PI;
    fnc = ((cos(arg)+1)/2)*255;
    redVal = 0; // 0
    greenVal = fnc; // 255 -> 0
    blueVal = 255 - fnc; // 0 -> 255
  }
  
  if (value > 511 && value <= 639)
  {
    // val between 512 and 639
    arg = ((float)((value)-511)/255)*PI;
    fnc = ((cos(arg)+1)/2)*255;
    redVal = 255 - fnc; // 0 -> 255
    greenVal = 0;  // 0
    blueVal = fnc;  // 255 -> 0
  }
  
  if (value > 639 && value <= 767)
  {
    arg = ((float)((value)-639)/255)*PI;
    fnc = ((cos(2*arg)+1)/2)*255;
    redVal = 85 + fnc/6;   // 127 -> 85
    greenVal = 85 - fnc/3;   // 0 -> 85
    blueVal = 85 + fnc/6;   // 127 -> 85
  }
  
//    invert for PWM with a LED with common anode -> 0% PWM is max brightness
    redVal = 255-redVal;
    greenVal = 255-greenVal;
    blueVal = 255-blueVal;
    
    analogWrite(redPin, redVal);
    analogWrite(greenPin, greenVal);
    analogWrite(bluePin, blueVal);
  
}
