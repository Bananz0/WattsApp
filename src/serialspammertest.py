import serial
import time
import random
ser = serial.Serial("COM12", 115200, timeout=1) 

def generate_data():
    data_values = [
        random.uniform(0, 100),#windTurbineCapacity
        random.uniform(0, 100), # pvCapacity
        random.uniform(100, 300),#busbarVoltage
        random.uniform(0, 20), #busbarCurrent
        random.uniform(100, 300),#totalRenewableCapacity
        random.randint(0, 100), #batteryCapacity
        random.randint(200, 300), #mainsCapacity
        random.randint(0, 5),  #dayCount
        random.uniform(150, 200), #totalLoadCapacity
        random.uniform(10, 20),#loadDeficit
        random.randint(0, 10),  #remainingDays
        random.randint(1, 10),  #utc
        random.randint(0, 1)   #load1Status
        #Will probably add the rest in the cctual code as this is only for testing.
    ]
    return ",".join(f"{v:.1f}" if isinstance(v, float) else str(v) for v in data_values)

for i in range(10):
    data = generate_data()
    if i % 2 == 0:
        delay = random.uniform(2000, 2000) / 1000
    else:
        delay = random.uniform(2000, 2000) / 1000
    ser.write((data + "\n").encode())  # Send the data
    print(f"Sent: {data} (Delay: {delay*1000:.1f}ms)")
    
    time.sleep(delay)
ser.close()