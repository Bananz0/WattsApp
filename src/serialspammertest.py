import serial
import time
import random

ser = serial.Serial("COM12", 115200, timeout=1) 

def generate_data():
    data_values = [
        #Renewable Stats
        random.uniform(0, 10),#windTurbineCapacity (float)
        random.uniform(0, 5), #pvCapacity (float)
        random.uniform(0, 15),  #totalRenewableCapacity (float)
        #Battery Stores
        random.randint(0, 100),  #batteryCapacity (int)
        #Mains Capacity
        random.uniform(0, 10),#mainsCapacity (float)
        # BusbarStuff
        random.uniform(320, 340), #busbarVoltage (float)
        random.uniform(2, 4), #busbarCurent (float)
        random.uniform(0, 50), #averagePower (float)
        random.uniform(0, 100), #otalEnergy (float)
        # Load Statuses
        random.randint(0, 1),#currentLoadStatus1 (bool/int)
        random.randint(0, 1), #currentLoadtatus2 (bool/int)
        random.randint(0, 1),#currentLoadStatus3 (bool/int)
        # Current Requests
        random.randint(0, 1),#curentLoad1Call (bool/int)
        random.randint(0, 1), #currentLoad2Call (bool/int)
        random.randint(0, 1), #currentLoad3Call (bool/int)
        # Load Overrides
        random.randint(0, 1), #oad1Override (bool/int)
        random.randint(0, 1),#load2Override (bool/int)
        random.randint(0, 1), #load3Ovrride (bool/int)
        # Total Capacity Stuff
        random.uniform(0, 20),#totalLoadDeficit (float)
        random.uniform(150, 200),#totalLoadCapacity (float)
        # Current Day Status
        random.randint(1, 24) #dayCount (int)
    ]
    return ",".join(f"{v:.2f}" if isinstance(v, float) else str(v) for v in data_values)

for i in range(1000):
    data = generate_data()
    if i % 2 == 0:
        delay = random.uniform(1800, 2000) / 1000
    else:
        delay = random.uniform(2000, 2200) / 1000
    ser.write((data + "\n").encode())  # Send the data
    print(f"Sent: {data} (Delay: {delay*1000:.1f}ms)")

    time.sleep(delay)

ser.close()