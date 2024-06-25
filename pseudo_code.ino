// Github Signatures Here:

// Daniel Richardson 6/17/2024
// Emmy Ploskina 6/17/2024
// Lily Grech 6/19/2024

#include <pitches.h>

// DEFINE PINS

// input
// 3 distinctly different inputs
const int coinInput = 13;
const int pullInput = 12;
const int spinInput = 11;
// start new game button
const int startGameInput = 10;

// output
const int speakerOutput = 9;
const int displayOutput1 = 8;
const int displayOutput2 = 7;
const int displayOutput3 = 6;
const int displayOutput4 = 5;
const int displayOutput5 = 4;
const int displayOutput6 = 3;
const int displayOutput7 = 2;
const int displayOutput8 = 1;


// DEFINE CONSTANTS

// our 3 options to randomly pick from 
const int choices[] = {1, 2, 3};  // to get a choice: choice = choices[random(0, 3)]

// DEFINE GLOBAL VARIABLES

// Should the game be running?
bool runGame = false;

// Our choice needs to be a global for easy access across functions
int choice;

// The current time in ms
// We will constantly check this rather than use delays
unsigned long currentMillis;
// The time up until the start of the current round
unsigned long previousMillis = 0;

// The current round count, should start at 0 and not exceed 99
// Incremented by 1 every successful attempt, reset every time game ends
unsigned int currentRound = 0;

// The per-round delay, must decrement every correct attempt, reset every time game ends
// Good goal: 3 seconds on first round -> 1 second on 99th round (20.2 ms decrements)
unsigned long currentRoundPeriod = 3000;

// State variables for edge detection
int startButtonState = 0;
int lastStartButtonState = 0;

int coinButtonState = 0;
int lastCoinButtonState = 0;

int pullButtonState = 0;
int lastPullButtonState = 0;

int spinButtonState = 0;
int lastSpinButtonState = 0;

// global player input
int playerInput = -1;  // -1 means we are waiting on an input


// Setup I/O
void setup() {
  pinMode(coinInput, INPUT);
  pinMode(pullInput, INPUT);
  pinMode(spinInput, INPUT);
  pinMode(startGameInput, INPUT);

  pinMode(displayOutput, OUTPUT);
  pinMode(speakerOutput, OUTPUT);

  // interupts could be setup here
}



// Main Game Loop
void loop() {
  currentMillis = millis();
 
  checkStartGame();  // check start game to either start game or reset
  if(runGame){
    checkInput();
    if(playerInput != -1){
      if(playerInput == choice){  // successful action
        succesfulAttempt();
      }
      else if(playerInput != choice){  // unsuccessful action
        gameOverLose();
      }
      playerInput = -1;  // reset input
    }
  // If time runs out, unsuccessful
    if(currentMillis - previousMillis > currentRoundPeriod){
      gameOverLose();
    }
  }

}



// Prompt user for input
void promptUser() {
  choice = choices[random(0, 3)];
  // output sound cue of random choice
}

// Correct Button is Pressed within required time
void successfulAttempt() {
  currentRound += 1;
  currentRoundPeriod -= 20.2;
  previousMillis = millis();

  if(currentRound == 100){
    gameOverWin();
  }

  promptUser();
}

void checkStartGame() {
  // read the pushbutton input pin:
  startButtonState = digitalRead(startGameInput);
  // compare the buttonState to its previous state
  if (startButtonState != lastStartButtonState) {
    // if the state has changed, increment the counter
    if (startButtonState == HIGH) {
      // if the current state is HIGH then the button went from off to on:
      resetGame();
      runGame = true;
      promptUser();  // first promptUser 
    }
    // Delay a little bit to avoid bouncing
    delay(50);
  }
  // save the current state as the last state, for next time through the loop
  lastStartButtonState = startButtonState;
}

// Note: choice 1 = coin, 2 = pull, 3 = spin
void checkInput() {
  // read inputs
  coinButtonState = digitalRead(coinButtonInput);
  pullButtonState = digitalRead(pullButtonInput);
  spinButtonState = digitalRead(spinButtonInput);
  // check for changes, if HIGH then return input
  if (coinButtonState != lastCoinButtonState){
    if(coinButtonState == HIGH){
      playerInput = 1;
    }
    delay(50);  // for debouncing
  }
  if (pullButtonState != lastPullButtonState){
    if(pullButtonState == HIGH){
      playerInput = 2;
    }
    delay(50);  // for debouncing
  }
  if (spinButtonState != lastSpinButtonState){
    if(spinButtonState == HIGH){
      playerInput = 3;
    }
    delay(50);  // for debouncing
  }
  // save the current state as the last state, for next time through the loop
  lastCoinButtonState = coinButtonState;
  lastPullButtonState = pullButtonState
  lastSpinButtonState = spinButtonState
}

void gameOverWin(){
  // output winning sound
  // make sure final score is still displayed
}

void gameOverLose(){
  // output erroneous sound
  // make sure final score is still displayed
}


void resetGame(){
  runGame = false;  // stop game
  currentRound = 0;  // reset round
  currentRoundPeriod = 3000;  // reset round period
  previousMillis = millis();
}



// ********************
//
// AUDIO
//
// ********************



int melody[] = {
  NOTE_E5, NOTE_E5, REST, NOTE_E5, REST, NOTE_C5, NOTE_E5,
  NOTE_G5, REST, NOTE_G4, REST, 
  NOTE_C5, NOTE_G4, REST, NOTE_E4,
  NOTE_A4, NOTE_B4, NOTE_AS4, NOTE_A4,
  NOTE_G4, NOTE_E5, NOTE_G5, NOTE_A5, NOTE_F5, NOTE_G5,
  REST, NOTE_E5,NOTE_C5, NOTE_D5, NOTE_B4,
  NOTE_C5, NOTE_G4, REST, NOTE_E4,
  NOTE_A4, NOTE_B4, NOTE_AS4, NOTE_A4,
  NOTE_G4, NOTE_E5, NOTE_G5, NOTE_A5, NOTE_F5, NOTE_G5,
  REST, NOTE_E5,NOTE_C5, NOTE_D5, NOTE_B4,
};

int durations[] = {
  8, 8, 8, 8, 8, 8, 8,
  4, 4, 8, 4, 
  4, 8, 4, 4,
  4, 4, 8, 4,
  8, 8, 8, 4, 8, 8,
  8, 4,8, 8, 4,
  4, 8, 4, 4,
  4, 4, 8, 4,
  8, 8, 8, 4, 8, 8,
  8, 4,8, 8, 4,
  
  
  4, 8, 8, 8, 4, 8,
  8, 8, 8, 8, 8, 8, 8, 8,
  4, 4, 8, 4,
  2, 2,
  
  4, 8, 8, 8, 4, 8,
  8, 8, 8, 8, 8, 8, 8, 8,
  4, 4, 8, 4,
  2, 2,
  
  8, 4, 8, 8, 8, 4,
  8, 4, 8, 2,
  
  8, 4, 8, 8, 8, 8, 8,
  1, 
  8, 4, 8, 8, 8, 4,
  8, 4, 8, 2,
  8, 8, 8, 8, 8, 8, 4,
  4, 4, 4, 4, 
  4, 8, 4, 4,
  
  4, 4, 8, 4,
  8, 8, 8, 4, 8, 8,
  8, 4, 8, 8, 4,
  
  4, 8, 4, 4,
  4, 4, 8, 4,
  8, 8, 8, 4, 8, 8,
  8, 4, 8, 8, 4,
  
  8, 4, 8, 4, 4,
  8, 4, 8, 2,
  8, 8, 8, 8, 8, 8,
  
  8, 4, 8, 2,
  8, 4, 8, 4, 4,
  8, 4, 8, 2,
  8, 4, 8, 8, 8, 8,
  8, 4, 8, 2,
  
  8, 4, 8, 4, 4,
  8, 4, 8, 2,
  8, 8, 8, 8, 8, 8,
  
  8, 4, 8, 2,
  8, 4, 8, 4, 4,
  8, 4, 8, 2,
  8, 4, 8, 8, 8, 8,
  8, 4, 8, 2,
  8, 4, 8, 8, 8, 8, 8,
  1,
  
  8, 4, 8, 8, 8, 4,
  8, 4, 8, 2,
  8, 8, 8, 8, 8, 8, 4,
  4, 4, 4, 4, 
  8, 4, 8, 4, 4,
  8, 4, 8, 2,
  8, 8, 8, 8, 8, 8,
  
  8, 4, 8, 2,
  8, 4, 8, 4, 4,
  8, 4, 8, 2,
  8, 4, 8, 8, 8, 8,
  8, 4, 8, 2,
  
  //game over sound
  4, 4, 4,
  8, 8, 8, 8, 8, 8,
  8, 8, 2
};

void playWinAudio()
{
  int size = sizeof(durations) / sizeof(int);

  for (int note = 0; note < size; note++) {
    //to calculate the note duration, take one second divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int duration = 1000 / durations[note];
    tone(speakerOutput, melody[note], duration);

    //to distinguish the notes, set a minimum time between them.
    //the note's duration + 30% seems to work well:
    int pauseBetweenNotes = duration * 1.30;
    delay(pauseBetweenNotes);

    //stop the tone playing:
    noTone(speakerOutput);
  }
}

void playSuccessAudio()
{
  // Yes this is the mario coin sound
  tone(speakerOutput,NOTE_B5,100);
  delay(100);
  tone(speakerOutput,NOTE_E6,750);
  delay(700);
  noTone(speakerOutput);
}

void playFailAudio()
{
  tone(speakerOutput,NOTE_E6,100);
  delay(100);
  tone(speakerOutput,NOTE_B5,750);
  delay(700);
  noTone(speakerOutput);
}

void playPromptAudio(int x)
{
  if(x == 1){
    tone(speakerOutput,NOTE_B4,100);
    delay(300);
    noTone(speakerOutput);
  }
  else if(x == 2){
    tone(speakerOutput,NOTE_B4,100);
    delay(300);
    tone(speakerOutput,NOTE_B5,100);
    delay(300);
    noTone(speakerOutput);
  }
  else{
    tone(speakerOutput,NOTE_B4,100);
    delay(300);
    tone(speakerOutput,NOTE_B5,100);
    delay(300);
    tone(speakerOutput,NOTE_B6,100);
    delay(300);
    noTone(speakerOutput);
  }
}
