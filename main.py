#
# Testing the function

from Volt_Red_Black_Out import Volt_Red_Black_Out

Red = 'ON'  # ON/OFF
Black = 'ON'  # ON/OFF
v1 = 1.0  # voltage on the Red channel
v2 = 3.0  # voltage on the Black channel
v1_out, v2_out = Volt_Red_Black_Out(Red, v1, Black, v2)

print("Red:", v1_out, "V\nBlack:", v2_out, "V")