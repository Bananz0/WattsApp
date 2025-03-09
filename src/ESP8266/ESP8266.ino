#if defined(ESP32)
#include <WiFiMulti.h>
WiFiMulti wifiMulti;
#define DEVICE "ESP32"
#elif defined(ESP8266)
#include <ESP8266WiFiMulti.h>
ESP8266WiFiMulti wifiMulti;
#define DEVICE "ESP8266"
#endif

#include <InfluxDbClient.h>
#include <InfluxDbCloud.h>
#include "checksum.h"

// WiFi AP SSID
#define WIFI_SSID "Glen's XPS"
// WiFi password
#define WIFI_PASSWORD "eesp8266"

#define INFLUXDB_URL "http://xps.mshome.net:8086"
#define INFLUXDB_TOKEN "iiWf_PqMPyerqIJOLyHPS6pEHnzshhFhhL7bC9dSbpkOztgEQqOuBwL_F5_UIuzFpbanUMzLJsPadRC5Axk-Fg=="
#define INFLUXDB_ORG "e4d0432f0ad4939a"
#define INFLUXDB_BUCKET "wattsapp_streamed_data"

// Time zone info
#define TZ_INFO "UTC0"


// Declare InfluxDB client instance with preconfigured InfluxCloud certificate
InfluxDBClient client(INFLUXDB_URL, INFLUXDB_ORG, INFLUXDB_BUCKET, INFLUXDB_TOKEN, InfluxDbCloud2CACert);

// Declare Data point
Point sensor("wifi_status");
Point loadStatus("load_status");
Point sourcesStatus("sources_status");
Point generalStats("general_statuses");

String getValue(String data, char separator, int index) {
  int found = 0;
  int strIndex[] = { 0, -1 };
  int maxIndex = data.length() - 1;

  for (int i = 0; i <= maxIndex && found <= index; i++) {
    if (data.charAt(i) == separator || i == maxIndex) {
      found++;
      strIndex[0] = strIndex[1] + 1;
      strIndex[1] = (i == maxIndex) ? i + 1 : i;
    }
  }
  return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}

void parseCSVData(String data) {
  //Parse the CSV Data to Influx
  //Renewable Stats
  float windTurbineCapacity = getValue(data, ',', 0).toFloat();
  float pvCapacity = getValue(data, ',', 1).toFloat();
  float totalRenewableCapacity = getValue(data, ',', 2).toFloat();
  //Battery Stores
  int batteryCapacity = getValue(data, ',', 3).toInt();
  //Mains Capacity
  float mainsCapacity = getValue(data, ',', 4).toFloat();
  //BusbarStuff
  float busbarVoltage = getValue(data, ',', 5).toFloat();
  float busbarCurrent = getValue(data, ',', 6).toFloat();
  float averagePower = getValue(data, ',', 7).toFloat();
  float totalEnergy = getValue(data, ',', 8).toFloat();
  //Load Statuses
  //Current Load statuses
  bool currentLoadStatus1 = getValue(data, ',', 9).toInt();
  bool currentLoadStatus2 = getValue(data, ',', 10).toInt();
  bool currentLoadStatus3 = getValue(data, ',', 11).toInt();
  //Current Requests
  bool currentLoad1Call = getValue(data, ',', 12).toInt();
  bool currentLoad2Call = getValue(data, ',', 13).toInt();
  bool currentLoad3Call = getValue(data, ',', 14).toInt();
  //Load Overrides
  bool load1Override = getValue(data, ',', 15).toInt();
  bool load2Override = getValue(data, ',', 16).toInt();
  bool load3Override = getValue(data, ',', 17).toInt();
  //Total Capacity Stuff
  float totalLoadDeficit = getValue(data, ',', 18).toFloat();
  float totalLoadCapacity = getValue(data, ',', 19).toFloat();
  //Current Day Status (i could use this for plotting the against the days)
  int dayCount = getValue(data, ',', 20).toInt();

  // SourceData
  sourcesStatus.clearFields();
  sourcesStatus.addField("wind", windTurbineCapacity);
  sourcesStatus.addField("pv", pvCapacity);
  sourcesStatus.addField("voltage", busbarVoltage);
  sourcesStatus.addField("current", busbarCurrent);
  sourcesStatus.addField("renewable", totalRenewableCapacity);
  sourcesStatus.addField("battery", batteryCapacity);
  sourcesStatus.addField("mains", mainsCapacity);
  sourcesStatus.addField("deficit", totalLoadDeficit);
  sourcesStatus.addField("totalLoadCapacity", totalLoadCapacity);
  sourcesStatus.addField("averagePower", averagePower);
  sourcesStatus.addField("totalEnergy", totalEnergy);

  // Load Status Data 
  loadStatus.clearFields();
  loadStatus.addField("load1Status", currentLoadStatus1);
  loadStatus.addField("load2Status", currentLoadStatus2);
  loadStatus.addField("load3Status", currentLoadStatus3);
  loadStatus.addField("load1Override", load1Override);
  loadStatus.addField("load2Override", load2Override);
  loadStatus.addField("load3Override", load3Override);
  loadStatus.addField("load1Call", currentLoad1Call);
  loadStatus.addField("load2Call", currentLoad2Call);
  loadStatus.addField("load3Call", currentLoad3Call);

  // General Stats data points
  generalStats.clearFields();
  generalStats.addField("day", dayCount);
}

bool validateChecksum(String data) {
  int starIndex = data.indexOf('*');
  String dataPart;
  String checksumPart;
  uint16_t calculatedChecksum;
  unsigned long receivedChecksum = 0;
  uint16_t receivedChecksum_u16 = 0;
  bool checksumValid = false;

  if (starIndex == -1) {
    Serial.println("No checksum delimiter found");
    dataPart = data; // Treat the entire string as data for checksum calculation

    bfs::Fletcher16 chk;
    calculatedChecksum = chk.Compute(reinterpret_cast<const uint8_t*>(dataPart.c_str()), dataPart.length());

    Serial.print("Calculated checksum (no delimiter): ");
    Serial.println(calculatedChecksum, HEX);
    return false; // Indicate that the checksum could not be validated properly
  } else {
    dataPart = data.substring(0, starIndex);
    checksumPart = data.substring(starIndex + 1);

    bfs::Fletcher16 chk;
    calculatedChecksum = chk.Compute(reinterpret_cast<const uint8_t*>(dataPart.c_str()), dataPart.length());

    // Convert received checksum from HEX to uint16_t
    receivedChecksum = strtoul(checksumPart.c_str(), NULL, 16);
    receivedChecksum_u16 = (uint16_t)receivedChecksum;

    // Verify Checksum
    if (calculatedChecksum != receivedChecksum_u16) {
      Serial.print("Checksum mismatch: calculated = ");
      Serial.print(calculatedChecksum, HEX);
      Serial.print(", received = ");
      Serial.println(receivedChecksum_u16, HEX);
      checksumValid = false;
    } else {
      checksumValid = true;
    }

    if (!checksumValid)
    {
        return false;
    }

    return true;
  }
}

void setup() {
  Serial.begin(115200);
  Serial.setTimeout(50);

  // Setup wifi
  WiFi.mode(WIFI_STA);
  wifiMulti.addAP(WIFI_SSID, WIFI_PASSWORD);

  Serial.print("Connecting to wifi");
  while (wifiMulti.run() != WL_CONNECTED) {
    Serial.print(".");
    delay(100);
  }
  Serial.println();

  // Accurate time is necessary for certificate validation and writing in batches
  // We use the NTP servers in your area as provided by: https://www.pool.ntp.org/zone/
  // Syncing progress and the time will be printed to Serial.
  timeSync(TZ_INFO, "pool.ntp.org", "time.nis.gov");

  // Check server connection
  if (client.validateConnection()) {
    Serial.print("Connected to InfluxDB: ");
    Serial.println(client.getServerUrl());
  } else {
    Serial.print("InfluxDB connection failed: ");
    Serial.println(client.getLastErrorMessage());
  }

  //dd tags to the data point
  sensor.addTag("device", DEVICE);
  sensor.addTag("SSID", WiFi.SSID());

  //Add the loads stgatuses to the data points with their tags for easier viewing.
  loadStatus.addTag("POWERSTATS", "load");
  sourcesStatus.addTag("POWERSTATS", "source");
  generalStats.addTag("POWERSTATS", "general");
}

void processSerialData(String data) {
  data.trim();
  if (data.length() == 0) {
    Serial.println("Skipping empty line");
    return;
  }
  bool isChecksumValid = validateChecksum(data);

  if (isChecksumValid) {
    parseCSVData(data);
    // Send the data to InfluxDB
    if (!client.writePoint(sourcesStatus)) {
      Serial.print("InfluxDB write failed (sources): ");
      Serial.println(client.getLastErrorMessage());
      return;  // Don't continue if write fails
    }
    if (!client.writePoint(loadStatus)) {
      Serial.print("InfluxDB write failed (loads): ");
      Serial.println(client.getLastErrorMessage());
      return;  // Don't continue if write fails
    }
    if (!client.writePoint(generalStats)) {
      Serial.print("InfluxDB write failed (general): ");
      Serial.println(client.getLastErrorMessage());
      return;  // Don't continue if write fails
    }
    Serial.println("Sources, Load and General Stats sent");
  } else {
    Serial.println("Checksum validation failed, data not sent to InfluxDB.");
  }
}


void loop() {
  if (wifiMulti.run() != WL_CONNECTED) {
    Serial.println("Wifi connection lost");
  }

  if (Serial.available() > 0) {
    String data = Serial.readStringUntil('\n');  // Read until newline
    processSerialData(data);
  }

  // Send WiFi status periodically
  sensor.clearFields();
  sensor.addField("rssi", WiFi.RSSI());

  Serial.print("Writing: ");
  Serial.println(sensor.toLineProtocol());

  if (!client.writePoint(sensor)) {
    Serial.print("InfluxDB write failed: ");
    Serial.println(client.getLastErrorMessage());
  }

  delay(500);
}