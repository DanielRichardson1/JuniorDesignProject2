// Github Signatures Here:

// Daniel Richardson 6/17/2024
// Emmy Ploskina 6/17/2024
// Lily Grech 6/19/2024

#include <pitches.h>
#include <Bounce2.h>

// DEFINE PINS

// input
// 3 distinctly different inputs
const int coinInput = 6;  // 12
const int pullInput = 9;  // 15
const int spinInputA = 3;  // 5
const int spinInputB = 2;  // 4
// start new game button
const int startGameInput = 1;  // 3

// output
const int speakerOutput = 7;  // 13
const int ledOutput = 8;  // 14  
const int displayOutput1 = 10;  // 16
const int displayOutput2 = 13;  // 19
const int displayOutput1Clear = 11;  // 17
const int displayOutput2Clear = 12;  // 18



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
unsigned int currentRound = 1;

// The per-round delay, must decrement every correct attempt, reset every time game ends
// Good goal: 3 seconds on first round -> 1 second on 99th round (20.2 ms decrements)
unsigned long currentRoundPeriod = 3000;

// Bounce objects
Bounce startDebouncer = Bounce();
Bounce coinDebouncer = Bounce();
Bounce pullDebouncer = Bounce();
Bounce spinDebouncer = Bounce();

// State variables for edge detection (DELETE LATER)
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
  pinMode(spinInputA, INPUT);
  pinMode(spinInputB, INPUT);
  pinMode(startGameInput, INPUT);

  pinMode(displayOutput1, OUTPUT);
  pinMode(displayOutput1Clear, OUTPUT);
  pinMode(displayOutput2, OUTPUT);
  pinMode(displayOutput2Clear, OUTPUT);
  pinMode(speakerOutput, OUTPUT);
  pinMode(ledOutput, OUTPUT);

  // Attach debouncers to button pins
  startDebouncer.attach(startGameInput);
  startDebouncer.interval(25);
  
  coinDebouncer.attach(coinInput);
  coinDebouncer.interval(25);

  pullDebouncer.attach(pullInput);
  pullDebouncer.interval(25);

  spinDebouncer.attach(spinInputA);
  spinDebouncer.interval(25);

  // Set the display pins to LOW initially
  digitalWrite(displayOutput1, LOW);
  digitalWrite(displayOutput2, LOW);
  digitalWrite(displayOutput1Clear, LOW);
  digitalWrite(displayOutput2Clear, LOW);
  
  // clear counters
  clearDisplay('A');
  clearDisplay('B');
}



// Main Game Loop
void loop() {
  currentMillis = millis();
 
  checkStartGame();  // check start game to either start game or reset
  if(runGame){
    checkInput();
    if(playerInput != -1){
      if(playerInput == choice){  // successful action
        successfulAttempt();
      }
      else{  // unsuccessful action
        gameOverLose();
      }
      playerInput = -1;  // reset input
    }
  // If time runs out, unsuccessful
    // if(currentMillis - previousMillis > currentRoundPeriod){
    //   gameOverLose();
    // }
  }
}



// Prompt user for input
void promptUser() {
  previousMillis = millis();
  choice = choices[random(0, 3)];
  // output sound cue of random choice
  playPromptAudio(choice);
}

// Correct Button is Pressed within required time
void successfulAttempt() {
  // display
  if(currentRound%10){
    incrementDisplay('A');
  }else{
    incrementDisplay('A');
    delay(10);
    clearDisplay('A');
    delay(200);
    incrementDisplay('B');
    }

  // logic
  currentRound += 1;
  currentRoundPeriod -= 20.2;

  if(currentRound > 99){
    gameOverWin();
  }
  else{
    promptUser();
  }
}

void checkStartGame() {
  startDebouncer.update();
  if (startDebouncer.fell()) {
    resetGame();
    runGame = true;
    promptUser();  // first promptUser 
  }
}

// Note: choice 1 = coin, 2 = pull, 3 = spin
void checkInput() {
  coinDebouncer.update();
  pullDebouncer.update();
  spinDebouncer.update();

  if (coinDebouncer.fell()) {
    playerInput = 1;
  }
  if (pullDebouncer.fell()) {
    playerInput = 2;
  }
  if (spinDebouncer.fell()) {
    playerInput = 3;
  }
}

void gameOverWin(){
  // output winning sound
  playWinAudio();
  // end game
  runGame = false;
}

void gameOverLose(){
  // output erroneous sound
  playFailAudio();
  // end game
  runGame = false;
}

void resetGame(){
  // display
  clearDisplay('A');
  clearDisplay('B');
  // logic
  runGame = false;  // stop game
  currentRound = 1;  // reset round
  currentRoundPeriod = 3000;  // reset round period
}

// ********************
//
// DISPLAY
//
// ********************


void incrementDisplay(char display) {
  if (display == 'A') {
    digitalWrite(displayOutput1, LOW);
    delay(10);
    digitalWrite(displayOutput1, HIGH);
  } else if (display == 'B') {
    digitalWrite(displayOutput2, LOW);
    delay(10);
    digitalWrite(displayOutput2, HIGH);
  }
  delay(10);
}

void clearDisplay(char display) {
  // clear
  if(display == 'A'){
    digitalWrite(displayOutput1Clear, LOW);
    delay(10);
    digitalWrite(displayOutput1Clear, HIGH);
    incrementDisplay('A');
  }
  else{
    digitalWrite(displayOutput2Clear, LOW);
    delay(10);
    digitalWrite(displayOutput2Clear, HIGH);
    incrementDisplay('B');
  }
}

// ********************
//
// AUDIO
//
// ********************

void playWinAudio()
{
  for(int i = 0; i < 5; i++){
    playSuccessAudio();
    checkStartGame();
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

void playFailAudio()  // D# C F#
{
  tone(speakerOutput,NOTE_DS4,300);
  delay(300);
  tone(speakerOutput,NOTE_C4,300);
  delay(300);
  tone(speakerOutput,NOTE_FS3,1000);
  delay(300);
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
