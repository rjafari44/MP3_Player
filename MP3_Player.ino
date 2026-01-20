#include "myheader.h"

void setup() {
    Serial.begin(115200);
    Serial.println("Starting...");

    mySerial.begin(9600, SERIAL_8N1, MP3_RX, MP3_TX);
    delay(1000); // Give DFPlayer time to power up

    pinMode(BUTTON_TOGGLE, INPUT_PULLUP);
    pinMode(BUTTON_SKIP, INPUT_PULLUP);

    // random seed for esp
    randomSeed(esp_random());

    // Retry DFPlayer initialization
    bool initialized = false;
    for (int i = 0; i < 3 && !initialized; ++i) {
        if (player.begin(mySerial)) {
            initialized = true;
        } else {
            Serial.println("DFPlayer init failed, retrying...");
            delay(200);
        }
    }

    if (!initialized) {
        Serial.println("Failed to connect to DFPlayer Mini. Check wiring/SD card.");
        while(true);  // stop here
    }

    Serial.println("DFPlayer initialized successfully!");
    player.volume(15);
}

void loop() {
    static bool isPlaying{false};
    static bool firstPress{true};
    static int currentTrack{1};
    static unsigned long timeLastPlay{};
    static unsigned long timeLastSkip{};
    static bool lastToggleState{HIGH};
    static bool lastSkipState{HIGH};
    unsigned long timeNow{};
    bool currentToggleState{};
    bool currentSkipState{};

    timeNow = millis();

    currentToggleState = digitalRead(BUTTON_TOGGLE);
    currentSkipState = digitalRead(BUTTON_SKIP);

    // ---- TOGGLE BUTTON (edge + debounce) ----
    if (lastToggleState == HIGH && currentToggleState == LOW && timeNow - timeLastPlay > 300) {
        if (firstPress) {
            playRandomTrack(currentTrack, isPlaying);
            firstPress = false;
        } else {
            togglePlayPause(isPlaying);
        }
        timeLastPlay = timeNow;
    }

    // ---- SKIP BUTTON (edge + debounce) ----
    if (lastSkipState == HIGH && currentSkipState == LOW && timeNow - timeLastSkip > 300) {
        skipTrack(currentTrack, isPlaying);
        timeLastSkip = timeNow;
    }

    lastToggleState = currentToggleState;
    lastSkipState = currentSkipState;

    remindFinished(currentTrack, isPlaying);
}
