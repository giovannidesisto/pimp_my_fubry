# pimp_my_fubry

![Furby ESP32](https://img.shields.io/badge/ESP32-Furby_Modernization-blue) ![License](https://img.shields.io/badge/License-MIT-green) ![Version](https://img.shields.io/badge/Version-1.0-orange)

## 📖 Table of Contents
- [Project Overview](#project-overview)
- [Hardware Requirements](#hardware-requirements)
- [Wiring Diagram](#wiring-diagram)
- [Software Libraries](#software-libraries)
- [Installation](#installation)
- [Project Structure](#project-structure)
- [Operational Modes](#operational-modes)
- [Serial Commands](#serial-commands)
- [OTA Updates](#ota-updates)
- [FTP Server](#ftp-server)
- [Game Engine](#game-engine)
- [Troubleshooting](#troubleshooting)
- [License](#license)

## 🚀 Project Overview

This project modernizes a vintage Furby animatronic toy using an ESP32 microcontroller. The system adds advanced features like MP3 playback, WiFi connectivity, OTA updates, and a sophisticated game engine while maintaining the original Furby's charm.

**Key Features:**
- 🎵 MP3 Audio Playback with I2S
- 📚 Interactive Storytelling Mode
- 🎮 Extensible Game Engine
- 🎵 Music Player Mode
- 📡 WiFi with OTA Updates
- 🗂️ FTP File Management
- 🎨 NeoPixel Status Indicators
- 🔧 Serial Debug Interface

## 🔧 Hardware Requirements

### Components:
- **ESP32 Board** (LOLIN D32)
- **MicroSD Card Module** (SPI)
- **I2S Audio Amplifier** (MAX98357A or similar)
- **NeoPixel LED** (WS2812B)
- **DC Motor** with Driver (L298N or similar)
- **Rotary Encoder** + CAM Sensor
- **Photoresistor** (Front Sensor)
- **Buttons** (Front, Rear, Tongue)
- **5V Power Supply**

## 🔌 Wiring Diagram
┌─────────────────────────────────────────────────────────────┐
│ LOLIN D32                                                   │
│ │                                                           |
│ ├─ SD CARD MODULE (SPI) │                                   │
│ │ CS ──── GPIO5         │                                   │
│ │ MOSI ──── GPIO23      │                                   │
│ │ MISO ──── GPIO19      │                                   │
│ │ SCK ──── GPIO18       │                                   │
│ │                                                           │
│ ├─ AUDIO I2S (MAX98357A)      │                             |
│ │ DIN ──── GPIO25 (I2S_DOUT)  │                             │
│ │ BCLK ──── GPIO27 (I2S_BCLK) │                             |
│ │ LRCLK ──── GPIO26 (I2S_LRC) │                             |
│ │                                                           |
│ ├─ MOTORE DC + ENCODER      │                               |
│ │ MOTORE+ ──── GPIO4 (FWD)  │                               |
│ │ MOTORE- ──── GPIO16 (BKW) │                               |
│ │                                                           │
│ │ ENCODER ──── GPIO36 (Analog)                              │
│ │ CAM ──────── GPIO2 (BTN_CAM)                              |
│ │ LED ──────── GPIO17 (ENC_LED)                             |
│ │                                                           │
│ ├─ PULSANTI                                                 |
│ │ FRONT ──── GPIO32 (BTN_FRONT)                             |
│ │ REAR ──── GPIO15 (BTN_REAR)                               │
│ │ TONGUE ──── GPIO39 (BTN_TONGUE)                           │
│ │ IR ──── GPIO35 (BTN_IR)                                   │
│ │                                                           |
│ ├─ SENSORI                                                  |
│ │ FOTO ───── GPIO34 (PhotoFront - Analog)                   │
│ │                                                           |
│ ├─ NEO-PIXEL                                                |
│ │ DATA ───── GPIO33                                         │
│ │                                                           |
│ └─ WIFI ENABLE                                              │
│ EN ───── GPIO21 (WIFI_EN - legacy reset button)             |
|                                                             |
└─────────────────────────────────────────────────────────────┘




## 📚 Software Libraries

### Bord version
ESP32		         1.0.6     # By Espressif System
### Required Libraries:
```ini
Adafruit BusIO           1.17.1+   # I2C/SPI abstraction
Adafruit NeoPixel        1.15.2+   # WS2812B LED control
ESP32 AnalogWrite        0.1+      # analogWrite() polyfill
ESP8266Audio             1.9.5+    # I2S audio playback
SimpleFTPServer          Latest    # FTP server functionality
Time                     1.6.1     -         user     -
Timezone                 1.2.5     1.2.6     user     Arduino library to facilitate time zo...




## Installation:

    Open Arduino IDE

    Go to Tools → Manage Libraries

    Search and install each library listed above

    Restart Arduino IDE

## 📁 Project Structure

```ini
FurbyESP32/
├── Babo.ino                 # Main application file
├── Furby.h                  # Core Furby class
├── FurbyConstants.h         # Pin definitions & constants
├── Libraries/
│   ├── Services/
│   │   ├── AudioManager.h   # MP3 playback management
│   │   ├── InputManager.h   # Button & sensor handling
│   │   ├── LedManager.h     # NeoPixel control
│   │   ├── EngineManager.h  # Motor control system
│   │   ├── TalesTeller.h    # Story mode logic
│   │   ├── GameManager.h    # Game engine framework
│   │   ├── FunctionEvaluator.h # Conditional logic
│   │   ├── FurbyWifi.h      # WiFi connectivity
│   │   ├── Ota.h            # Over-the-air updates
│   │   └── FurbyFtpServer.h # FTP file management
│   └── Objects/
│       ├── Game.h           # Game data structure
│       └── Function.h       # Function evaluation structure
└── SD Card Contents/
    ├── audio/               # System audio files
    ├── tales/               # Story folders (0/, 1/, etc.)
    ├── games/               # Game configurations
    └── mp3/                 # Music files (1.mp3, 2.mp3, etc.)





## 🎮 Operational Modes
### 1. 📚 Story Mode (OP_MODE_TALES)

    Activation: Front button (2-second press)

    Function: Plays interactive stories from SD card

    LED Indicator: Yellow when active, Rainbow during playback

    Controls:

        Front button (1s): Accept/Continue story

        Rear button (1s): Skip to next story

        Front double-click: Volume down

        Rear double-click: Volume up

### 2. 🎵 Music Mode (OP_MODE_MP3)

    Activation: Mode button

    Function: MP3 music player

    LED Indicator: Cyan when idle, Rainbow during playback

    Controls:

        Front button (1s): Next track / Stop playback

        Rear button (1s): Previous track / Stop playback

        Front double-click: Volume down

        Rear double-click: Volume up

### 3. 🎮 Game Mode (OP_MODE_PLAY)

    Activation: Mode button

    Function: Interactive games with audio + motor

    LED Indicator: Magenta

    Controls:

        Tongue button: Special interaction sounds

        Light sensor: Environmental triggers ("cucu" sounds)

        Front/Rear: Game-specific actions

        Front double-click: Volume down

        Rear double-click: Volume up

## ⌨️ Serial Commands
Audio Control
bash

audio@/path/file.mp3    # Play specific audio file
+                       # Increase volume (5%)
-                       # Decrease volume (5%)

System Control
bash

mode                    # Cycle through operational modes
i                       # Print input sensor status

LED Control
bash

l@ON                    # Turn NeoPixel green
l@OFF                   # Turn NeoPixel off

Input Simulation (Testing)
bash

# Front button simulation
fn, fc, f1, f2, f3      # NOT_CLICKING, CLICKING, 1s, 2s, 3s

# Rear button simulation  
rn, rc, r1, r2, r3      # NOT_CLICKING, CLICKING, 1s, 2s, 3s

Configuration
bash

x@500                   # Set encoder threshold value

# 📡 OTA Updates
Activation:

Hardware Method: Connect GPIO21 to 3.3V during boot
Usage:

    Ensure WiFi is enabled (blue LED)

    Check serial monitor for IP address

    In Arduino IDE: Tools → Port → Network Ports → Select Furby IP

    Upload sketch normally

# WiFi Configuration:

Edit FurbyWifi.h:
cpp

const char* ssid = "Your_WiFi_SSID";
const char* password = "Your_WiFi_Password";

# 📁 FTP Server
## Access:

    URL: ftp://[FURBY_IP]/

    Username: furby

    Password: furby

    Port: 21 (default)

## Features:

    Upload/download files to SD card

    Remote file management

    Real-time transfer status

## 🎯 Game Engine
### Configuration Files:

The game engine uses CSV files for configuration:

game.csv format:
csv

ID,COMMAND,AUDIO_FILE,ENGINE_ROUTINE,NEXT_STEP,FUNCTION
0,0,1,0,1,0        # Play audio 1, go to step 1
1,4,2,-1,-1,1      # Play audio 2 + evaluate function block 1

functions.csv format:
csv

ID,FUNCTION,NEXT_STEP
0,"1_=_1_-1_&;2_=_-1_3_|",2    # If condition true, go to step 2

### Command Types:
cpp

GAME_TYPE_AUDIO = 0             // Audio only
GAME_TYPE_AUDIO_ENGINE = 1      // Audio + motor
GAME_TYPE_ENGINE = 2            // Motor only
GAME_TYPE_EVAL_INPUT = 3        // Evaluate input
GAME_TYPE_EVAL_INPUT_AUDIO = 4  // Audio + evaluate input

# 🛠️ Troubleshooting
Common Issues:

    Audio Not Playing

        Verify file exists on SD card

        Check file extension (.mp3 vs .MP3)

        Verify I2S connections

        Check volume level

    WiFi/OTA Not Working

        Ensure GPIO21 is HIGH during boot

        Verify WiFi credentials

        Check serial monitor for connection status

    Buttons Not Responding

        Use i command to check button states

        Verify wiring connections

        Check button pull-up/down configuration

    Motor Not Moving

        Verify motor driver connections

        Check encoder and CAM sensor readings

        Ensure power supply is adequate

# Debug Tips:

    Enable serial monitor at 115200 baud

    Use i command for real-time sensor status

    Check NeoPixel colors for mode indications

    Verify SD card file structure

# Button Input States

The system recognizes these button press durations:

    CLICKING: < 1 second

    CLICK_1_SEC: 1-2 seconds

    CLICK_2_SEC: 2-3 seconds

    CLICK_3_SEC: 3-4 seconds

    NOT_CLICKING: >4 seconds or released

# 📄 License

This project is open source under MIT License. Feel free to modify and distribute according to your needs.
# 🤝 Contributing

Contributions are welcome! Please feel free to submit pull requests or open issues for bugs and feature requests.

Happy Modernizing! 🎉 Your Furby is about to get a 21st-century upgrade!

