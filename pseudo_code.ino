// Github Signatures Here:

// Daniel Richardson 6/17/2024
// Emmy Ploskina 6/17/2024
// Lily Grech 6/19/2024

// DEFINE PINS

// input
// 3 distinctly different inputs
const int coinInput = 12;
const int pullInput = 11;
const int spinInput = 10;
// start new game button
const int startGameInput = 9;

// output
// will actually consist of multiple output pins for LCD Display and Speaker
const int displayOutput = 8;
const int speakerOutput = 7;

// DEFINE CONSTANTS

// our 3 options to randomly pick from
const int choices[] = {1, 2, 3}; // to get a choice: choice = choices[random(0, 3)]

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

// State variables for start button (for edge detection)
int buttonState = 0;
int lastButtonState = 0;

// Setup I/O
void setup()
{
  pinMode(coinInput, INPUT);
  pinMode(pullInput, INPUT);
  pinMode(spinInput, INPUT);
  pinMode(startGameInput, INPUT);

  pinMode(displayOutput, OUTPUT);
  pinMode(speakerOutput, OUTPUT);

  // interupts could be setup here
}

// Main Game Loop
void loop()
{
  currentMillis = millis();

  checkStartGame(); // check start game to either start game or reset
  if (runGame)
  {
    // If input matches choice, successful
    // TODO
    // If input does not match choice, unsuccessful
    // TODO
    // If time runs out, unsuccessful
    if (currentMillis - previousMillis > currentRoundPeriod)
    {
      gameOverLose();
    }
  }
}

// Prompt user for input
void promptUser()
{
  choice = choices[random(0, 3)];
  // output sound cue of random choice
}

// Correct Button is Pressed within required time
void successfulAttempt()
{
  currentRound += 1;
  currentRoundPeriod -= 20.2;
  previousMillis = millis();

  if (currentRound == 100)
  {
    gameOverWin();
  }

  promptUser();
}

void checkStartGame()
{
  // read the pushbutton input pin:
  buttonState = digitalRead(startGameInput);
  // compare the buttonState to its previous state
  if (buttonState != lastButtonState)
  {
    // if the state has changed, increment the counter
    if (buttonState == HIGH)
    {
      // if the current state is HIGH then the button went from off to on:
      resetGame();
      runGame = true;
      promptUser(); // first promptUser
    }
    // Delay a little bit to avoid bouncing
    delay(50);
  }
  // save the current state as the last state, for next time through the loop
  lastButtonState = buttonState;
}

void gameOverWin()
{
  // output winning sound
  // make sure final score is still displayed
}

void gameOverLose()
{
  // output erroneous sound
  // make sure final score is still displayed
}

void resetGame()
{
  runGame = false;           // stop game
  currentRound = 0;          // reset round
  currentRoundPeriod = 3000; // reset round period
  previousMillis = millis();
}
