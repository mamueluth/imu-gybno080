## Setup Instructions

## 1. Create virtual environment
```bash
python3 -m venv .venv
source .venv/bin/activate
pip install -r requirements.txt
```

## Run Instructions

```bash
python3 visualize.py
```

# Measure IMU Update Frequency

## Setup Instructions communication frequency measurement
Make sure `libserial-dev` is installed. e.g. with
```bash
$ apt list libserial-dev 
    Listing... Done
    libserial-dev/now 1.0.0-9build1 amd64 [installed,local]
```
If not install with `sudo apt install libserial-dev`.

### Build
Change to `measure_frequency` folder. If there is no build folder create one with: `mkdir build`. After you create the build folder change into it and run:
```bash
cmake ..
make
```
there should now be a executable called `measure_serial_freq`.

### Run
From within your build folder (`~/workspace/src/imu-gybno080/tools/measure_frequency/build`) run: 
```bash
./measure_serial_freq
```
Note however, that the port and baud rate are currently hardcoded as:
```cpp
// Open and configure serial port
serial_port.Open("/dev/ttyUSB0");
serial_port.SetBaudRate(LibSerial::BaudRate::BAUD_230400);
```
You can check [https://github.com/crayzeewulf/libserial/blob/master/src/libserial/SerialPortConstants.h](https://github.com/crayzeewulf/libserial/blob/master/src/libserial/SerialPortConstants.h) for available baud rates. Don't forget to update the baud rates ind [main.cpp](../src/main.cpp) and [platformio.ini](../platformio.ini) 