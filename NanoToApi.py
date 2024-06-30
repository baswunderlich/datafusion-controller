import serial
import requests
import sys

url = 'http://192.168.5.46:8000/instruction'
port = '3'  # Default COM port is 3

if len(sys.argv) > 1:  # Set COM port with command line args
    port = sys.argv[1]

print(f'Using COM port: {port}')

def main():
    try:
        serialCon = serial.Serial(f'COM{port}', 9600, timeout=1)
    except serial.SerialException as e:
        print(f"Failed to open serial port COM{port}: {e}")
        return

    requestCounter = 0
    try:
        while True:
            line = serialCon.readline().decode('utf-8').strip()
            if line:
                try:
                    # Assuming the line format is "velocityR,velocityL"
                    velocityR, velocityL = map(float, line.split(','))

                    data = {
                        'left': velocityL,
                        'right': velocityR
                    }
                    requestCounter+=1
                    if requestCounter >= 30:
                        requestCounter = 0
                        response = requests.post(url, json=data)
                        if response.status_code != 200:
                            print(f"Failed to send data: {response.status_code} - {response.text}")

                except ValueError:
                    print(f"Invalid data received: {line}")
                except requests.RequestException as e:
                    print(f"HTTP request failed: {e}")
    except KeyboardInterrupt:
        print("Program terminated by user.")
    finally:
        serialCon.close()

if __name__ == '__main__':
    main()
