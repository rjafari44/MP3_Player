#include "myheader.h"

// function which plays a random track
// parameters: int &currentTrack -> will get the value of the current track and replace through the pass by reference
//             bool &isPlaying -> will get the condition of whether a track is playing or not, and replace it through the pass by reference
// returns: nothing
void playRandomTrack(int &currentTrack, bool &isPlaying) {
    int total{};
    int track{};

    // read number of files on SD card
    total = player.readFileCounts();

    // handle empty SD card
    if (total <= 0) {
        Serial.println("No audio files available! Cannot play random track.");
        isPlaying = false;
        return;
    } else {
    track = random(1, total + 1); // assign the track to be played to a random number between the availble track indexes
    player.play(track);           // play the chosen track
    currentTrack = track;
    isPlaying = true;          

    Serial.print("Playing random track: ");
    Serial.println(track);
    }
}

// function which skips the current track to the next one
// parameters: bool &isPlaying -> will get the condition of whether a track is playing or not, and replace it through the pass by reference
//             bool &trackFinished -> will see if the current track is finished
//             int currentTrack -> will play current track
// returns: nothing
void togglePlayPause(bool &isPlaying, bool &trackFinished, int currentTrack) {
    if (isPlaying) { // if song is playing, pause it
        player.pause();
        Serial.println("Track Paused");
        isPlaying = false;
    } else { // if song is paused, check two conditions
        player.start();
        if (trackFinished) { // if a song had already finished
            Serial.print("Now Playing: ");
            Serial.println(currentTrack);
            trackFinished = false; // reset flag for new playback
        } else { // if it is midway through the track
            Serial.println("Track Resumed");
        }
        isPlaying = true;
    }
}

// function which skips the current track to the next one
// parameters: int &currentTrack -> will get the value of the current track and replace through the pass by reference
//             bool &isPlaying -> will get the condition of whether a track is playing or not, and replace it through the pass by reference
// returns: nothing
void skipTrack(int &currentTrack, bool &isPlaying) {
    int total{};

    // read how many files are in the SD card
    total = player.readFileCounts();

    // if there is nothing, exit the function
    if (total <= 0) {
        Serial.println("No audio files available!");
        isPlaying = false;
        return;
    } 
    currentTrack++; // increment through the SD card to play the next track

    // if the track cycling reaches more than the availble amount of tracks, reset the music list again to the beginning
    if (currentTrack > total) {
        currentTrack = 1;
    }

    player.play(currentTrack);  // play current track (or next track in this case)
    isPlaying = true;
    Serial.print("Now Playing: ");
    Serial.println(currentTrack);
}

// function which reminds the serial when a track is finished
// parameters: int &currentTrack -> will get the value of the current track and replace through the pass by reference
//             bool &isPlaying -> will get the condition of whether a track is playing or not, and replace it through the pass by reference
//             bool &trackFinished -> will set the condition of whether the current track playing is finished
// returns: nothing
void remindFinished(int currentTrack, bool &isPlaying, bool &trackFinished) {
    static int lastFinishedTrack{}; // variable to track last finished track
    int type{};                     // variable to track current state of DFPlayer

    while (player.available()) {
        type = player.readType();

        if (type == DFPlayerPlayFinished && lastFinishedTrack != currentTrack) {
            Serial.print("Track ");
            Serial.print(currentTrack);
            Serial.println(" finished!");

            isPlaying = false;
            trackFinished = true;             // mark as naturally finished
            lastFinishedTrack = currentTrack; // set last finished track to current track, for preventing spam
        }
    }
}


