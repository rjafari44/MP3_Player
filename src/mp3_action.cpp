#include "myheader.h"

void playRandomTrack(int &currentTrack, bool &isPlaying) {
    int total{};
    int track{};

    total = player.readFileCounts();

    track = random(1, total + 1);
    player.play(track);
    currentTrack = track;   // sync track number
    isPlaying = true;       // <-- important
    Serial.print("Playing random track: ");
    Serial.println(track);
}

void togglePlayPause(bool &isPlaying) {
    if (isPlaying) {
        player.pause();
        Serial.println("Track Paused");
        isPlaying = false;
    } else {
        player.start();
        Serial.println("Track Resumed");
        isPlaying = true;
    }
}

void skipTrack(int &currentTrack, bool &isPlaying) {
    int total{};

    total = player.readFileCounts();
    if (total <= 0) {
        return;
    } 

    currentTrack++;
    if (currentTrack > total) {
        currentTrack = 1;
    }

    player.play(currentTrack);  // explicit control
    isPlaying = true;           // <-- FIX: track is now playing
    Serial.print("Now Playing: ");
    Serial.println(currentTrack);
}

void remindFinished(int currentTrack, bool &isPlaying) {
    static bool finishedReported = false; // ensure only once per track

    while (player.available()) {          // catch all pending DFPlayer events
        int type = player.readType();

        if (type == DFPlayerPlayFinished && !finishedReported) {
            Serial.print("Track ");
            Serial.print(currentTrack);
            Serial.println(" finished!");
            isPlaying = false;
            finishedReported = true;     // only report once
        }
    }
}

