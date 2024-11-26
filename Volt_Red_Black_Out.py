import serial
import time

def Volt_Red_Black_Out(chan1, v1, chan2, v2):
    # Assuming you have already opened the serial connection
    ser = serial.Serial('COM3', 19200)
    time.sleep(0.5)

    # Format the data as a string
    data_to_send = f"{chan1} {v1} {chan2} {v2}\n"

    # Send the data
    for i in data_to_send:
        time.sleep(0.005)
        ser.write(i.encode())
    time.sleep(0.5)

    v1_out, v2_out = str(ser.readline().decode()).split()
    return float(v1_out), float(v2_out)