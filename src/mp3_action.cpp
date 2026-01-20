#include "myheader.h"

void playRandomTrack() {
    int total{};
    int track{};
    static bool noFilesFound{false};

    total = player.readFileCounts();

    if (total > 0) { 
        track = random(1, total + 1);
        player.play(track);
        Serial.println("playing: ");
        Serial.print(track);

        noFilesFound = false;
    } else if (!noFilesFound) {
        Serial.println("No audio files found!");
        noFilesFound = true;
    }
}

void togglePlayPause(bool &isPlaying) {
    if (isPlaying) {
        player.pause();
        Serial.println("Track Paused");
        isPlaying = true;
    } else {
        player.start();
        Serial.println("Track Resumed");
        isPlaying = true;
    }
}

void skipTrack(int &currentTrack) {
    int total{};
    
    player.next();

    total = player.readFileCounts();
    
    currentTrack++;

    if (currentTrack > total) {
        currentTrack = 1;
    }

    Serial.println("Now Playing: ");
    Serial.println(currentTrack);

}