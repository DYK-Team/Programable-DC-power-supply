# Programmable DC power supply based on Arduino UNO WiFi Rev2

In the realm of test and measurement, the concept of software-defined instrumentation is revolutionizing how we design and implement laboratory equipment. Our programmable DC power supply, built on an Arduino UNO and a DAC, exemplifies this paradigm shift. By leveraging software-defined instrumentation principles, we deliver a versatile, cost-effective solution that integrates multiple functions into a single reconfigurable device. By simply uploading different Arduino’s sketches, users can easily reconfigure the device for various tasks, including integrating sensors. This approach enhances flexibility, simplifies maintenance, and ensures adaptability to evolving technological demands.

For detailed information about the project, refer to the report in the file "Programable DC power supply based on Arduino UNO.pdf". The report provides a detailed discussion of the applications of the programmable power supply for magneto-impedance (MI) measurements, along with the relevant references. The power supply was specifically designed for the MI measurements but can be easily reprogrammed for other laboratory tasks.

The Arduino sketch can be found in the "Voltage_Red_Black_Outputs" folder. The control program in Python, "main.py", uses the library function "Volt_Red_Black_Out.py". In addition, we have included the sketches and programs used during the debugging process of the device. They are located in the "Test_Sketches_Arduino_Python" folder. Photographs of the project stages are saved in the "Design gallery" folder.

Instructions for Using the Power Supply Control Program:
1.	Upload the sketch "Voltage_Red_Black_Outputs.ino" to the Arduino.
2.	Run the "main.py" program, which activates the Red or Black outputs and controls the voltage on them.
3.	The "main.py" program utilizes the library function "Volt_Red_Black_Out.py", included in the project folder. This library handles communication with the Arduino via the serial port.
4.	For projects involving measurement automation using Python, only the "main.py" script needs to be modified to adapt the functionality.

