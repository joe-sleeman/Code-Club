/*
 *  Greg Neilson 2016.
 *  
 *  Dunedin, New Zealand.
 *  
 *  This project has been made using an arduino and a glove. Buttons pressed by the glove
 *  can form to make combinations based on binary.
 *  
 *  You are free to use this code as you please as long as you credit Greg Neilson in the code.
 *  
 *  Just copy/paste this comment.
 */
#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12,11,5,4,3,2);

#define LCD_LIGHT_PIN A4
// Constants that represent the physical board itself. Mapping buttons to slots on Arduino.
const int oneButtonPin = 6; 
const int twoButtonPin = 7;   
const int threeButtonPin = 8;   
const int fourButtonPin = 9; 
const int fiveButtonPin = 10;

// Initialize button states to 0.
int button1State = 0;
int button2State = 0;
int button3State = 0;
int button4State = 0;
int button5State = 0;

// Initialize output number to 0.
int output = 0;
// set sentance size - size of array. Change for longer sentances.
const int sentSize = 20;
//Init char array that will build up to form a word/sentance. Displays on top line LCD display.
char chosenSentance[sentSize];
//Init position to the starting position. 
int position = 0;

void setup() {
  Serial.begin(9600);
    // Setup the number of columns and rows that are available on the LCD. 
  lcd.begin(16, 2);
  lcd.noDisplay();
  
  // Set the button pins as an input.
  pinMode(oneButtonPin, INPUT);
  pinMode(twoButtonPin, INPUT);
  pinMode(threeButtonPin, INPUT);
  pinMode(fourButtonPin, INPUT);
  pinMode(fiveButtonPin, INPUT);
   // Set the LCD display backlight pin as an output.
  pinMode(LCD_LIGHT_PIN, OUTPUT);

  // Turn off the LCD backlight.
  digitalWrite(LCD_LIGHT_PIN, LOW);
}

void loop() { 
  readState();
  // Array that has characters in the alphabet. Only have up to p for now as only 4 fingers currently.
  char alphabet[16] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p'};
  char chosen;  // Chosen char.
  // Choosing the letter.
  button5State = digitalRead(fiveButtonPin);
  // if output is >= 1 then we have chosen a letter.
  if (output >=1){

    lcd.setCursor(0,1);
    // Need to - 1 because arrays are 0 based.
    lcd.print(alphabet[output-=1]);
    //lcd.print(output);
    // Grabbing letter out of alphabet array and displaying on the second line of LED display.
    digitalWrite(LCD_LIGHT_PIN, HIGH);
    lcd.display();
    // You have pressed a button and you want to confirm that as part of your sentance.
    // Now we set our char chosen to equal the letter we have selected and we add it to
    // the chosenSentance array.
    if (button5State == HIGH){
      chosen = (alphabet[output]);
      lcd.setCursor(position,0);
      lcd.print(chosen);
      //Add letter to sentence
      chosenSentance[position++]=chosen;
      Serial.println(chosenSentance);
      delay(500);
    }
    // Reset output.
    output = 0;
  }
  // Once we have confirmed a sentence, we need to move across.
  if (position>=sentSize){
    position = 0;
  }  
}

int readState(){ 
  //Read buttons
  button1State = digitalRead(oneButtonPin);
  button2State = digitalRead(twoButtonPin);
  button3State = digitalRead(threeButtonPin);
  button4State = digitalRead(fourButtonPin);
  
  //Encode to output
  if (button1State == HIGH){
    output+=1;
  }
  if (button2State == HIGH){
    output+=2;
  }
  if (button3State == HIGH){
    output+=4;
  }
  if (button4State == HIGH){
    output+=8;
  } 
  return output;
}

// Clear stuff.
void clear(){
    lcd.clear();
    lcd.setCursor(0, 0);
}
