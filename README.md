# WattsApp Smart Meter

## Project Description

The WattsApp Smart Meter is designed to monitor and manage energy consumption in real-time, optimizing energy usage by integrating renewable energy sources, battery storage, and mains power. This system provides real-time insights into energy generation, consumption, and system status, allowing for informed decisions and reduced costs.

## Key Features

* **Real-time Energy Monitoring:** Tracks power generation and consumption from various sources.
* **Intelligent Load Management:** Automatically controls electrical loads to optimize energy usage based on available power.
* **Renewable Energy Integration:** Seamlessly integrates solar panels and wind turbines to maximize the use of renewable energy.
* **Battery Storage Management:** Intelligently charges and discharges a battery to provide backup power and stabilize the grid.
* **Emergency Handling:** Triggers an emergency screen on error or alerts.
* **Optional WiFi Connectivity:** Enables remote monitoring and control.
* **InfluxDB Integration:** Sends data to InfluxDB for storage and analysis.
* **Grafana Visualization:** Provides visual dashboards of system performance using Grafana.

## Hardware Components

* **Microcontroller:** [ATmega644P](https://www.microchip.com/en-us/product/atmega644p)
* **Display:** [ILI9341 TFT LCD](https://www.amazon.co.uk/Serial-Touch-Panel-Display-Module/dp/B0749N3S33)
* **Analog Inputs:** [Current and Voltage Sensors - Used for Busbar Monitoring]
* **Analog Output:** [PWM Module for Mains Control]
* **Digital Inputs/Outputs:** [Load Calls and Switches]
* **WiFi Module (Optional):** [ESP8266](https://www.espressif.com/en/products/socs/esp8266)

## Software Components

* **Arduino IDE:** For ESP8266 Development
* **CLion 2024.3:** For ATMega644P Development
* **Libraries:**
  * [Pictor](https://github.com/TBTerra/pictor/)
  * [arduino-core](https://github.com/arduino/ArduinoCore-avr/tree/master)
  * [InfluxDB_Arduino_Client](https://github.com/tobiasschuerg/InfluxDB-Client-for-Arduino)
  * [Checksum](https://github.com/bolderflight/checksum)
* **Grafana:** [v]

## Getting Started

1. **Hardware Setup:**
    * Connect the hardware components according to the provided schematic.
    * Ensure all connections are secure and properly wired.
2. **Software Setup:**
    * Install the Arduino IDE or CLion.
    * Install the required libraries (see "Software Components").
    * Upload the code to the microcontroller.
3. **Configuration:**
    * Configure the WiFi settings in the code (if applicable).
    * Configure the InfluxDB settings in the code (URL, Token, Org, Bucket).
4. **Running the Smart Meter:**
    * Power on the system.
    * Monitor the display for real-time data and alerts.
    * Use the manual switches to control electrical loads (if desired).

## Using with InfluxDB and Grafana

1. **InfluxDB Setup:**
    * Create an InfluxDB Cloud account or set up a local InfluxDB instance.
    * Create a new organization and bucket for storing the smart meter data.
    * Generate an API token with read and write permissions.
2. **Grafana Setup:**
    * Create a Grafana Cloud account or set up a local Grafana instance.
    * Add InfluxDB as a data source.
    * Create dashboards to visualize the smart meter data (e.g., power generation, consumption, battery level, load status).

## License

This project is licensed under the **GNU General Public License, Version 3** (GPLv3), as published by the Free Software Foundation. See [LICENSE](./LICENSE) for details.

## Acknowledgments

This project uses code from other open-source projects. See [ATTRIBUTION.md](./ATTRIBUTION.md) for details.
