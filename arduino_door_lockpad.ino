// include libraries
#include <LiquidCrystal.h>

// set up lcd variables
const int rs = 12, en = 11, d4 = 10, d5 = 9, d6 = 8, d7 = 7;
LiquidCrystal lcd(rs,en,d4,d5,d6,d7);

//set up analog varialbes **assign pins later**
const int buzz = A0;
const int flipSwitch = A1;

// set up buttons, leds, and switch
const int button1 = 6;
const int button2 = 5;
const int button3 = 4;
const int button4 = 3;
const int greenLED = 2;
const int redLED = 1;

// button sequence to open
int openSequence[] = {1, 2, 3, 4};
// sequence that will test if it is = to the openSequence
int trySequence[4];
// variable to know what part of the sequence we are on
int tryInput = 0;

// state tracker for if we are trying to open the door or not
int tryState = 0; //if try state == 0, we are trying to open the door, if try state != 0, we have opened the door

// assign button pushes states
int button1Push;
int button2Push;
int button3Push;
int button4Push;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  lcd.begin(16,2);
  lcd.write("Door is locked");
  pinMode(buzz, OUTPUT);
  pinMode(flipSwitch, INPUT);
  pinMode(button1, INPUT);
  pinMode(button2, INPUT);
  pinMode(button3, INPUT);
  pinMode(button4, INPUT);
  pinMode(greenLED, OUTPUT);
  pinMode(redLED, OUTPUT);
  // we want the redLED to stay on at the beginning
  digitalWrite(redLED, HIGH);  
}

void loop() {
  // first check which state we are in, if we are in the try state, get the inputs, otherwise if we are open already, lock the door after 5 seconds
  if (tryState == 0) {
    // we want to first get try and get the correct button sequence. Everytime a player pushes a button, send that assigned value to the trySequence
    // assign button push variables the digitalread of their respective buttons
    button1Push = digitalRead(button1);
    button2Push = digitalRead(button2);
    button3Push = digitalRead(button3);
    button4Push = digitalRead(button4);
    // if a button is pressed, add to the trySequence, check tryInput location as well
    if(button1Push == HIGH) {
      if (tryInput != 4) {
        trySequence[tryInput] = 1;
        tryInput+=1;
        Serial.println("button1 pressed");
        delay(500);
      } else {
        checkTrySequence();
      }
    }
    if(button2Push == HIGH) {
      if (tryInput != 4) {
        trySequence[tryInput] = 2;
        tryInput+=1;
        Serial.println("button2 pressed");
        delay(500);
      } else {
        checkTrySequence();
      }
    }
    if(button3Push == HIGH) {
      if (tryInput != 4) {
        trySequence[tryInput] = 3;
        tryInput+=1;
        Serial.println("button3 pressed");
        delay(500);
      } else {
        checkTrySequence();
      }
    }
    if(button4Push == HIGH) {
      if (tryInput != 4) {
        trySequence[tryInput] = 4;
        tryInput+=1;
        Serial.println("button4 pressed");
        delay(500);
      } else {
        checkTrySequence();
      }
    }
    // check if we did all pressed the pin and automatically check if we are correct
  } else {
    delay(5000);
    digitalWrite(redLED, HIGH);
    digitalWrite(greenLED, LOW);
    tryState = 0;
    tryInput = 0;
  }
}

// method helpers are below here

/*
 * If the tryInput variable is at 3, run this function
 */
void checkTrySequence() {
  // compare the two sequences, if the sequence is correct, check the switch orientation
  // otherwise reset tryInput to 0 and send a serial alert
  if (compareSequence(trySequence, openSequence, 4, 4)){
    checkSwitchOrientation();
  } else {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Wrong Combo!");
    Serial.println("------ WARNING ---------");
    Serial.println("|Failed entrace attempt|");
    Serial.println("------------------------");
    tryInput = 0;
  }
}

/*
 * Function to compare our trySequence and openSequence
 */
 boolean compareSequence(int * tryS, int * openS, int tryS_len, int openS_len) {
  int i;
  if (tryS_len != openS_len) {
    return false;
  }
  for(i = 0; i < openS_len; i++) {
    if(tryS[i] != openS[i]) {
      return false;
    }
  }
  return true;
 }

 /*
  * Function check the switches orientation
  */
void checkSwitchOrientation() {
  // check if the flipSwitch is in the right orientation, if so, open the door
  // otherwise send a alert and reset tryInput to 0;
  int orientation = digitalRead(flipSwitch);
  if (orientation == HIGH) {
    openDoor();
  } else {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Wrong Combo!");
    Serial.println("------ WARNING ---------");
    Serial.println("|Failed entrace attempt|");
    Serial.println("------------------------");
    tryInput = 0;
  }
}

/*
 * Function to open door
 */
void openDoor() {
  tryState = 1;
  digitalWrite(redLED, LOW);
  digitalWrite(greenLED, HIGH);
  lcd.clear();
  lcd.print("Access Granted!");
  Serial.println("------ ALERT ------");
  Serial.println("|  Success entry  |");
  Serial.println("-------------------");
  tone(buzz, 1000);
  delay(1500);
  noTone(buzz);
}
