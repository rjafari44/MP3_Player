#include "myheader.h"

// main setup that sets up everything to run in loop, returns nothing
void setup() {
    bool initialized{}; // variable for tracking successful initialization of DFPlayer

    // begin the serial monitor at baudrate of 115200
    Serial.begin(115200);
    delay(200);

    Serial.println("Starting...");

    // uart setup
    mySerial.begin(9600, SERIAL_8N1, MP3_RX, MP3_TX);
    delay(1000); 

    // set the pin buttons of the button with a pull to ensure they don't float
    pinMode(BUTTON_TOGGLE, INPUT_PULLUP);
    pinMode(BUTTON_SKIP, INPUT_PULLUP);

    // random seed for esp, used for the random track play on first press
    randomSeed(esp_random());

    // Retry DFPlayer initialization
    for (int i = 0; i < 3 && !initialized; i++) {
        if (player.begin(mySerial)) {
            initialized = true;
        } else {
            Serial.println("DFPlayer init failed, retrying...");
            delay(200);
        }
    }

    // if DFPlayer initialization attempts fail
    if (!initialized) {
        Serial.println("Failed to connect to DFPlayer Mini. Check wiring/SD card.");
        while(true);  // stop here
    }

    // let monitor know initialization was successful
    Serial.println("DFPlayer initialized successfully!");
    player.volume(20);
}

// main loop that performs all of the mp3 player actions, returns nothing
void loop() {
    static bool isPlaying{false};         // variable to track if a track is playing or not
    static bool firstPress{true};         // variable to track if its the first press of the loop
    static int currentTrack{1};           // variable to play the current track
    static unsigned long timeLastPlay{};  // variable that keeps track of time since last press of toggle button press
    static unsigned long timeLastSkip{};  // variable that keeps track of time since last press of skip button press
    static bool lastToggleState{HIGH};    // variable for debounce protection of the last toggle button press
    static bool lastSkipState{HIGH};      // variable for debounce protection of the last skip button press
    unsigned long timeNow{};              // variable for getting the time now depending on when in the loop
    bool currentToggleState{};            // variable for debounce protection of the current toggle button press
    bool currentSkipState{};              // variable for debounce protection of the current skip button press
    static bool trackFinished{};          // variable to track if song finished naturally

    timeNow = millis();

    // assign the current button states to the current values beeing read by the micro controller.
    currentToggleState = digitalRead(BUTTON_TOGGLE);
    currentSkipState = digitalRead(BUTTON_SKIP);

    // toggle pause/play button, the condition ensures you can't just hold the button down and have it skip forever
    // also gives a 0.3 second delay for each press to ensure circuit protection
    if (lastToggleState == HIGH && currentToggleState == LOW && timeNow - timeLastPlay > 300) {
        if (firstPress) {
            playRandomTrack(currentTrack, isPlaying);
            firstPress = false;
        } else {
            togglePlayPause(isPlaying, trackFinished, currentTrack);
        }
        timeLastPlay = timeNow; // set the time to now for the last toggle press for condition checking
    }

    // skip button, the condition ensures you can't just hold the button down and have it skip forever
    // also gives a 0.3 second delay for each press to ensure circuit protection
    if (lastSkipState == HIGH && currentSkipState == LOW && timeNow - timeLastSkip > 300) {
        skipTrack(currentTrack, isPlaying);
        timeLastSkip = timeNow; // set the time to now for the last skip press for condition checking
    }
    // assign the current toggle state to be used for the next iteration of the loop
    lastToggleState = currentToggleState; 
    lastSkipState = currentSkipState;

    remindFinished(currentTrack, isPlaying, trackFinished); // function that lets the serial monitor know track is finished
}
