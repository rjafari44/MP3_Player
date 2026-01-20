#include "myheader.h"

void setup() {

    Serial.begin(115200);

    mySerial.begin(9600, SERIAL_8N1, MP3_RX, MP3_TX);

    pinMode(BUTTON_TOGGLE, INPUT_PULLUP);
    pinMode(BUTTON_SKIP, INPUT_PULLUP);

    delay(1000);

    if (!player.begin(mySerial)) {
        Serial.println("Connecting to DFPlayer Mini failed!");
    }

    player.volume(15);
}

void loop() {
    static bool isPlaying{false};
    static bool firstPress{true};
    static int currentTrack;
    static unsigned long timeLastPlay{};
    static unsigned long timeLastSkip{};
    unsigned long timeNow{};

    timeNow = millis();

    if (digitalRead(BUTTON_TOGGLE) == LOW && timeNow - timeLastPlay > 200) {
        if (firstPress) {
            playRandomTrack();
            isPlaying = true;
            firstPress = false;
        } else {
            togglePlayPause(isPlaying);
        }
        timeLastPlay = timeNow;
    }

    if (digitalRead(BUTTON_SKIP) == LOW && timeNow - timeLastSkip > 200) {
        skipTrack(currentTrack);
        isPlaying = true;
        timeLastSkip = timeNow;
    }

    if (player.available() && player.readState() == 0) {
        skipTrack(currentTrack);
        isPlaying = true;
    }


}

