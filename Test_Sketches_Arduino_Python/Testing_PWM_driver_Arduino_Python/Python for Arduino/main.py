import serial
import time
import numpy as np
import matplotlib.pyplot as plt

# Open serial connection to Arduino
ser = serial.Serial('COM7', 9600)
time.sleep(1)  # Wait for Arduino to reset

# Sweep duty cycles from 0 to 255
duty_cycles = np.arange(0, 256)
voltages = []

# Send duty cycle values to Arduino, collect analog readings, and convert
for duty_cycle in duty_cycles:
    print(f"Sending duty cycle: {duty_cycle}")
    ser.write(f"{duty_cycle}\n".encode())  # Send duty cycle to Arduino
    time.sleep(1.5)  # Introduce a delay time (s)
    analog_value = ser.readline().decode().strip()  # Read analog value from Arduino
    voltage = (float(analog_value) / 1023.0) * 5.0  # Convert to voltage
    voltages.append(voltage)
    print(f"Analog reading: {analog_value}, Voltage: {voltage:.2f} V")

# Close serial connection
ser.close()

# Plotting the graph
plt.plot(duty_cycles, voltages)
plt.xlabel("Duty cycle (0-255)")
plt.ylabel("Voltage (V)")
plt.title("Voltage vs. Duty Cycle")
plt.grid(True)
plt.show()