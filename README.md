# MP3_Player

#### This project was inspired from the IEEE Student Branch at UCI. This project consists of an ESP32, a  DFPlayer Mini MP3 Player, and a simple two port speaker. The mp3 player allows music that is put on a micro SD card to be played through the press of two buttons. One for pause/play, and one for skipping to the next song. There is no autoskip. I have also created my own schematic in LTspice based off of the original schematic of the club. Feel free to copy or change anything needed. All files are provided. Please be aware that the DFPlayer Mini library needs to be downloaded.

#### Circuit Schematic:

![Diagram](assets/mp3_circuit.png)

## How to run
I ran this code on a WeAct Studio esp32c3 board.

If using the Arduino IDE, make sure the WeAct Studio esp32c3 board is the correct selected one and enable CDC on boot.

If running the script in CLI, it will prompt you for your board's serial port that it is connected to, so you need to find it and mention it. The serial port is usually /dev/ttyACM0 or /dev/ttyUSB0 but it could also be different. Also, if the shell script is not executable, chmod it.

If you prefer a manual/edited compilation and upload, either edit my shell script or simply copy them one at a time with custom flags, note that CDC on boot is required for the serial monitor to work.

Below is the easy script method in CLI.

### 1. Find Connected Serial Port
#### On Linux
```bash
ls /dev/tty*
```   

#### With Arduino CLI (recommended method)
```bash
arduino-cli board list
```

### 2. Run the program in CLI
```bash
./run.sh
```