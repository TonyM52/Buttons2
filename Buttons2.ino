// This sketch is for LCDs that are directly controlled with Arduino pins.
// It is based on Bill Perry's variant of Hello World", which includes his modified library to 
// avoid the design flaw that exists in many (including mine) LCD shields.
//
// Sketch originally printed "Hello, World!" on the lcd - Intention is to modify the sketch
//to take input from the buttons on the LCD shield, and display the effects on the LCD.
//
// See below for configuring the Arduino pins used.
//
// While not all hd44780 use the same pinout, here is the one that most use:
// pin 1 is the pin closest to the edge of the PCB
//  1 - LCD gnd
//  2 - VCC (5v)
//  3 - Vo Contrast Voltage
//  4 - RS Register Select (rs)
//  5 - Read/Write
//  6 - Enable (en)
//  7 - Data 0 (db0) ----
//  8 - Data 1 (db1)     |-------- Not used in 4 bit mode
//  9 - Data 2 (db2)     |
// 10 - Data 3 (db3) ----
// 11 - Data 4 (db4)
// 12 - Data 5 (db5)
// 13 - Data 6 (db6)
// 14 - Data 7 (db7)
// 15 - Backlight Anode (+5v)
// 16 - Backlight Cathode (Gnd)
//
// ----------------------------------------------------------------------------
// LiquidCrystal compability:
// Since hd44780 is LiquidCrystal API compatible, most existing LiquidCrystal
// sketches should work with hd44780 hd44780_pinIO i/o class once the
// includes are changed to use hd44780 and the lcd object constructor is
// changed to use the hd44780_pinIO class.

#include <hd44780.h>
#include <hd44780ioClass/hd44780_pinIO.h> // Arduino pin i/o class header

// declare Arduino pins used for LCD functions
// and the lcd object

// This sketch does not attempt to use LCD backlight control:  
//(Bill's notes for using such control have therefore been removed.)

// without backlight control:
// The parameters used by hd44780_pinIO are the same as those used by
// the IDE bundled LiquidCrystal library
const int rs=8, en=9, db4=4, db5=5, db6=6, db7=7;
hd44780_pinIO lcd(rs, en, db4, db5, db6, db7);

// LCD geometry
const int LCD_COLS = 16;
const int LCD_ROWS = 2;

//Smiley:  pinched this, and the related lines to print it, from the tutorial page on createChar().
byte smiley[8] = {
  B00000,
  B10001,
  B00000,
  B00000,
  B10001,
  B01110,
  B00000,
};



void setup()
{
  Serial.begin(9600);
  
  // initialize LCD with number of columns and rows: 
  //
  // note:
  //  begin() will automatically turn on the backlight if backlight
  //  control is specified in the lcd object constructor
  //
  lcd.begin(LCD_COLS, LCD_ROWS);

  // if backlight control was specified, the backlight should be on now

  // Print a preliminary message to the LCD
  
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Buttons, by Tony");
  lcd.setCursor(0,1);
  lcd.print("Wait a mo... ");
  lcd.createChar(0,smiley);
  lcd.setCursor(14,1);
  lcd.write(byte(0));
  delay (5000);
  lcd.clear();
}

/*  Note to self:  the sketch compiles and runs OK thus far, giving the Hello World display on Line 1.

 Try changing to a longer message after the end of the setup section to see how the display responds - 
this one has 29 characters:  "Hello, World!   abcdefghijklm"
The lcd.print line on its own produces an error message 'lcd' does not name a type:
Repeating the lcd.begin line first just gives the same error in relation to both lines - now rem'd out:
 lcd.begin(LCD_COLS, LCD_ROWS);
 lcd.print("Hello, World!   abcdefghijklm");
Try the longer message within the original setup section:
Just gives the original "Hello World!" message - ignores the later bit.
However, if the spaces are removes, displays "Hello, World!abc" - i.e. fills the first row (16 chrs) 
but doesn't display the rest.
Finally got the expected 2-line output:  Note:  lcd.setCursor - first column is 0, not 1 - 
similarly first row is 0, not 1!
Then added the smiley - note that, again, setCursor assumes 0 for first char, hence char 15 needs 14 in the 
statement - and using 16 pushes it off the end of the display! 

Final step for today was to try to read the buttons - but didn't get anything on the serial monitor - 
so added the 5-second timer to clear the display.  This works - as does the reset button, which re-displays 
the initial message - but still nothing on the serial oputput.

Silly boy - didn't set up the serial connection!

Now, having added serial printing of x along with the state, and adjusted the definitions of directions 
to match the analogue values, everything seems to work except the "right" key, which is producing no 
x value and no state.  Not clear what's happening there.

Values are:

X=100 - Up
x=254 - Down
x=408 - Left
x=639 - Select

A meter check on the Right button confirms that it works OK - continuity when pressed.
The values are identical when feeding the board from the radio desk 12V (actually 13.77V) supply.

Sorted:  Changed the State numbers - increased by 1, rather than starting with Right = 0.
This was to let me see whether the state was actually being affected by pressing the Right button - and it was.  
However, Right actually gave an analogue read value of zero, and Up was borderline - now showing as 99.
Changing the if statement for Right to "if x < 50" avoided any confusion between Right and Up - and now everything works fine.
Having fixed that, it took a while to get the string handling right to update the LCD according to button pushes -
it's not quite perfect, it could do with saying "waiting for press" rather than x=1023 - No Press - and longer delays might help.
*/

int laststate = 0;


char laststatename;

void loop() {
  // put your main code here, to run repeatedly

  int state = 0;            //Sets state to 0 at start of every loop cycle.
String statename0 = "empty";
String statename1 = "Right";
String statename2 = "Up";
String statename3 = "Down";
String statename4 = "Left";
String statename5 = "Select";
String statename6 = "No Press";
String statename;




  int x= analogRead(0);     //  Reads the analog value produced by the button.
  // Note that, with no button pressed, the analogue read returns a value of 1023, 
  // so a statement is needed to catch values over 1000.

  // Check analog values against button title:

if (x<50) {  state = 1;  // - i.e. Right 
}

else if (x<200) {  state = 2; // state = Up
}

else if (x<400) {  state = 3;  //state = Down
}

else if (x<600) {  state = 4;  // state = Left
}

else if (x<800) {  state = 5;  // state = Select
}

else if (x>1000){ state = 6;  // state = No Press
}


  if (state != laststate) {

    
    Serial.print ("x = "); Serial.print (x); Serial.print ("  ");
if (state == 0) { Serial.println (statename0 + " - empty state");
statename = statename0;
delay(200);
}
else if (state == 1) { Serial.println (statename1 + " - Right"); 
statename = statename1;
  delay(200);
}  
else if (state == 2) { Serial.println (statename2 + " - Up");
statename = statename2;
    delay(200);
} else if (state == 3) {Serial.println (statename3 + " - Down");
statename = statename3;
    delay(200);
} else if (state == 4) {Serial.println (statename4 + " - Left");
statename = statename4;
    delay(200);
} else if (state == 5) {Serial.println (statename5 + " - Select");
statename = statename5;
    delay(200);
} else if (state ==6) {Serial.println (statename6 = " - No Press");
statename = statename6;
}
  

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("x = "); lcd.print(x);
  lcd.setCursor(0,1);
  //lcd.print("Button:  "); 
  lcd.print(statename);
  
  delay (500);
//  lcd.clear();
laststate = state;

  }
}

  
