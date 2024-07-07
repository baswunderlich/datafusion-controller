Here is the updated README with an additional setup step to start the "NanoToApi.py" script:

# Wireless Robot Controller

This project is designed to control a robot wirelessly using an Arduino Nano. The robot is from Manchester Robotics, and the setup involves running a Python script on an Nvidia Jetson board and configuring the firewall to ensure the API is reachable from outside. The Arduino Nano is flashed with a program to send the controller data via WiFi and control the robot accordingly.

## Table of Contents

- [Prerequisites](#prerequisites)
- [Setup Instructions](#setup-instructions)
  - [1. Flashing the Arduino](#1-flashing-the-arduino)
  - [2. Setting up the Nvidia Jetson Board](#2-setting-up-the-nvidia-jetson-board)
  - [3. Configuring the Firewall](#3-configuring-the-firewall)
  - [4. Configuring the Firewall](#3-configuring-the-firewall)
- [Usage](#usage)
- [Contributing](#contributing)
- [License](#license)

## Prerequisites

Before you begin, ensure you have the following:

- Arduino Nano
- Arduino IDE
- Python on your laptop/computer
- Manchester Robotics Puzzlebot Jackson Edition

## Setup Instructions

### 1. Flashing the Arduino

This is supposed to be done with the Arduino IDE

1. Connect your Arduino Nano to your computer via USB.
2. Open the Arduino IDE.
3. Open the crazyDatafusionStuff.ino file located in the crazyDatafusionStuff directory of this repository.
4. Select the correct board and port from the Tools menu.
5. Click the upload button to flash the Arduino Nano.

### 2. Setting up the Nvidia Jetson Board

1. Ensure Python 3.x is installed on your Nvidia Jetson board.
2. Clone this repository to your Nvidia Jetson board:
    

    ```bash
    git clone https://github.com/yourusername/datafusion-controller.git
    ```

3. Navigate to the project directory:
    

    ```bash
    cd datafusion-controller
    ```

4. Install the required Python packages:
    

    ```bash
    pip install -r requirements.txt
    ```

5. Start the main Python script to handle the API:
    

    ```bash
    hypercorn robot_go_brrr:app -b <IP>:8000
    ```

### 3. Configuring the Firewall

Ensure that the API running on the Nvidia Jetson board is reachable from outside your local network:

1. Open the firewall settings on your Nvidia Jetson board.
2. Allow incoming connections on the port used by robot_go_brrr.py (default is 8000).
3. Save the firewall settings and ensure the changes take effect.

### 4. Starting communication between laptop, nano and Puzzlebot 

For that you only need to start the NanoToApi.py script, make sure to edit the script, so that the right port for the nano and IP-adresse of the Jackson is used:
    

    ```bash
    python NanoToApi.py
    ```


## Usage

Once the setup is complete, you can control the robot wirelessly. The controller data is sent via WiFi to the Arduino Nano, which in turn controls the robot's movements. Ensure the Nvidia Jetson board and the Arduino Nano are both connected to the same WiFi network. It should be noted that the current version deals with some delay between control and actual reaction of the robot.

## License

This project is licensed under the MIT License.