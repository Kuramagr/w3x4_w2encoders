//This is my final code for Davinci resolve using a teensy 2.0 and the custom keyboard with 3x4 and 2 encoders
//The macro keyboard is handwired and im using every switch with a Pin and the other switch to the ground of the board.
//You can add more cases to change Layouts. I use only one because i need it only for editing videos.
//i hope it helps at least to start from somewhere.
//PS. the keyboard.press() is for mac you can have to change it for windows and for your shortcuts on davinci resolve.


#include <Bounce2.h>
#include <Keyboard.h>
#include <Encoder.h>


//Set up the button grid
const int numButtons = 12;
const int buttonPins[numButtons] = {3,6,15,18,4,7,14,17,5,8,13,16}; //Array of button pins in an order 
int state = 0;
//Set up all the buttons as bounce objects
Bounce buttons[] = {Bounce(buttonPins[0],10),Bounce(buttonPins[1],10),Bounce(buttonPins[2],10),Bounce(buttonPins[3],10),Bounce(buttonPins[4],10),Bounce(buttonPins[5],10),Bounce(buttonPins[6],10),Bounce(buttonPins[7],10),Bounce(buttonPins[8],10),Bounce(buttonPins[9],10),Bounce(buttonPins[10],10),Bounce(buttonPins[11],10)};

// Define pins for the first encoder and button
#define ENC1_A 0
#define ENC1_B 1
#define ENC1_BTN 2

// Define pins for the second encoder and button
#define ENC2_A 3
#define ENC2_B 4
#define ENC2_BTN 5

// Define the encoder objects and variables for the button states
Encoder encoder1(ENC1_A, ENC1_B);
Encoder encoder2(ENC2_A, ENC2_B);
Bounce button1 = Bounce(ENC1_BTN, 10);
Bounce button2 = Bounce(ENC2_BTN, 10);
int button1State = 0;
int lastButton1State = 0;
int button2State = 0;
int lastButton2State = 0;

// Define variables for the timeline and media library control
int timelinePos = 0; // start at beginning of timeline
int libraryPos = 0; // start at beginning of media library
bool playing = true;

void setup() {
  Serial.begin(9600);
  Keyboard.begin(); //Start the Keyboard object
  for(int i = 0; i < numButtons; i++){
     pinMode(buttonPins[i], INPUT_PULLUP);
  }// Set up the button pins as inputs and enable pullup resistors
  pinMode(ENC1_BTN, INPUT_PULLUP);
  pinMode(ENC2_BTN, INPUT_PULLUP);
  // Initialize the Keyboard library
  Keyboard.begin();
}


void loop() {
  //check all buttons
  for(int j = 0; j < numButtons; j++){
    if(buttons[j].update()){
      if(buttons[j].fallingEdge()){
        //Serial.write("Button");
       

        //use the current state and the button number to find the command needed and send it.
        switch (state) {
          case 0: //Layout 1
            switch (j) {
              case 0: 
                Keyboard.press('M');
                delay(100);
                Keyboard.releaseAll();
                break;
              case 1: 
              Keyboard.press(KEY_RIGHT_SHIFT);
                Keyboard.press('J');
                delay(100);
                Keyboard.releaseAll();
                break;
              case 2: 
              Keyboard.press('K');
                delay(100);
                Keyboard.releaseAll();
               break;
              case 3:
                Keyboard.press(KEY_RIGHT_SHIFT);
                Keyboard.press('L');
                delay(100);
                Keyboard.releaseAll();
                break;
              case 4: 
              Keyboard.press(KEY_RIGHT_GUI);
                Keyboard.press('V');
                delay(100);
                Keyboard.releaseAll();
                break;
              case 5:
              Keyboard.press(KEY_RIGHT_GUI);
                Keyboard.press('C');
                delay(100);
                Keyboard.releaseAll();
                break;
              case 6: 
                Keyboard.press('O');
                delay(100);
                Keyboard.releaseAll();
                break;
              case 7:
                Keyboard.press('I');
                delay(100);
                Keyboard.releaseAll();
                break;
              case 8:
              Keyboard.press(KEY_RIGHT_GUI);
                Keyboard.press('E');
                delay(100);
                Keyboard.releaseAll();
                break;
              case 9:
              Keyboard.press(KEY_RIGHT_GUI);
                Keyboard.press('A');
                delay(100);
                Keyboard.releaseAll();
                break;
              case 10:
              Keyboard.press(KEY_RIGHT_ALT);
                Keyboard.press('S');
                delay(100);
                Keyboard.releaseAll();
                break;
              case 11:
              Keyboard.press(KEY_RIGHT_GUI);
                Keyboard.press('X');
                delay(100);
                Keyboard.releaseAll();
                break;
             }
            break;

    }
  }
  // Read the button states
  button1.update();
  button1State = button1.read();
  button2.update();
  button2State = button2.read();

  // If the first button state has changed from high to low, toggle the play/pause state
  if (button1State == LOW && lastButton1State == HIGH) {
    playing = !playing;
    if (playing) {
      // Send play command
      Keyboard.press(KEY_MEDIA_PLAY_PAUSE);
    } else {
      // Send pause command
      Keyboard.press(KEY_MEDIA_PLAY_PAUSE);
    }
  }

  // If the second button state has changed from high to low, play the selected media
  if (button2State == LOW && lastButton2State == HIGH) {
    // Send select command to move to the selected media in the library
    for (int i = 0; i < libraryPos; i++) {
      Keyboard.press(KEY_MEDIA_NEXT_TRACK);
      delay(50);
      Keyboard.release(KEY_MEDIA_NEXT_TRACK);
      delay(50);
    }
    // Send play command to play the selected media
    Keyboard.press(KEY_MEDIA_PLAY_PAUSE);
  }

  // Read the encoder values and adjust the timeline or library position accordingly
  int delta1 = encoder1.read();
  int delta2 = encoder2.read();
  if (delta1 != 0) {
    timelinePos += delta1;
    if (timelinePos < 0) {
      timelinePos = 0;
    }
    // Send timeline command
    Keyboard.press(KEY_LEFT_GUI);
    Keyboard.press(KEY_LEFT_ALT);
    Keyboard.press(KEY_LEFT_SHIFT);
    Keyboard.press('M');
    delay(50);
    Keyboard.releaseAll();
  }
  if (delta2 != 0) {
    libraryPos += delta2;
    if (libraryPos < 0) {
      libraryPos = 0;
   }
// Send media library command
Keyboard.press(KEY_LEFT_GUI);
Keyboard.press(KEY_LEFT_ALT);
Keyboard.press(KEY_LEFT_SHIFT);
Keyboard.press('B');
delay(50);
Keyboard.releaseAll();
}

// Save the button states for the next loop iteration
lastButton1State = button1State;
lastButton2State = button2State;

// Reset the encoder positions to prevent overflow
encoder1.write(0);
encoder2.write(0);
}
