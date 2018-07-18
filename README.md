# Smart City Research Project

## Pre-Requirements

* Arduino Studio
* NodeJS v9
* Node Package Manager (npm) v5
* Bluetooth Enabled Device (Raspberry Pi, Mac OSX) **Untested on Windows**

## Embedded Code

### Installation

1. Open `bascode.ino` under `arduino/bascode/` folder with *Arduino Studio*
2. Connect embedded device to host machine
3. Verify code and upload code to embedded device (when uploading, make sure that the Bluetooth module is unplugged)
4. Disconnect embedded device from host machine and restart the device

### Functionality

The embedded code itself contain codes to move the device, use ultrasonic to detect object, and line tracking feature. The idea is to only have core function on embedded device (acknowledge to its memory limitation) and move more logic toward the controlling machine. With that being, said embedded device will also constantly send over its sensor data over BlueTooth in the form of `${sensorName} ${sensorValue}`

## Main Application Script

### Initialization

1. Change directory to `javascript` folder
2. Install dependencies (`npm install`)

### Execution

1. From the `javascript` folder, execute project (`npm run start`)

### Feature

As the moment of execution, the script will find a specific BT device name (currently hardcoded). Once found, it will automatically establish a connection. Once connected, the host machine will constantly receive embedded device's sensor data, as well as the ability to control the device using character command (For now, you can use keyboard to execute those command). Below are the list of character as well their function:

```javascript
    case 'w': changeSpeed(carSpeedState); forward();  break; //Move Forward
    case 's': changeSpeed(carSpeedState); backward();  break; //Move Backward
    case 'a': changeSpeed(FAST); turnLeft();  break; // Turn left
    case 'd': changeSpeed(FAST); turnRight(); break; // Turn Right
    case 'x': immediateStop();  break;
    case 'f': changeSpeed(SLOW); break; //change speed to slow (100)
    case 'g': changeSpeed(AVERAGE); break; //change speed to average (128)
    case 'h': changeSpeed(FAST); break; //change speed to fast (255)
    case 'z': updateObstacleMeasurement(); break; //initiate object detection ahead of device
    case 'c': toggleLineTracking(); break; //toggle line tracking feature
```