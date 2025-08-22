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

# Measure

## Setup Instructions communication frequency measurement
Make sure `libserial-dev` is installed. e.g. with
```bash
$ apt list libserial-dev 
    Listing... Done
    libserial-dev/now 1.0.0-9build1 amd64 [installed,local]
```
If not install with `sudo apt install libserial-dev`.

### Build
```bash
g++ serial_freq.cpp -o serial_freq -lserial -std=c++17
```

### Run 
```bash
./serial_freq
```