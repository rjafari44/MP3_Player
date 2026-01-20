#pragma once

#include <HardwareSerial.h>
#include <DFRobotDFPlayerMini.h>
#include <Arduino.h>

// Pins
constexpr int MP3_RX{20};
constexpr int MP3_TX{21};
constexpr int BUTTON_TOGGLE{4};
constexpr int BUTTON_SKIP{5};

// Objects
inline HardwareSerial mySerial(1);
inline DFRobotDFPlayerMini player;

// Functions
void playRandomTrack(int &currentTrack, bool &isPlaying);
void togglePlayPause(bool &isPlaying);
void skipTrack(int &currentTrack, bool &isPlaying);
void remindFinished(int currentTrack, bool &isPlaying);