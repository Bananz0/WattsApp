package com.bananz0.wattsapp.data.repository

import com.influxdb.client.InfluxDBClient
import com.influxdb.client.InfluxDBClientFactory
import com.influxdb.client.QueryApi

import android.util.Log
import com.bananz0.wattsapp.BuildConfig
import com.bananz0.wattsapp.data.model.PowerData
import kotlinx.coroutines.flow.Flow
import kotlinx.coroutines.flow.flow
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.withContext

object InfluxDB {
    private val token = BuildConfig.INFLUXDB_TOKEN.toCharArray()
    private val org = BuildConfig.INFLUXDB_ORG
    private val bucket = BuildConfig.INFLUXDB_BUCKET
    private val influxDBClient: InfluxDBClient by lazy {
        InfluxDBClientFactory.create(
            BuildConfig.INFLUXDB_URL,
            token,
            org,
            bucket
        )
    }

    private val queryApi: QueryApi by lazy {
        influxDBClient.queryApi
    }

    fun queryPowerData(): Flow<PowerData?> = flow {
        try {
            val query = """
                from(bucket: "$bucket")
                |> range(start: -1m)
                |> filter(fn: (r) => r._measurement == "smart_meter")
                |> last()
            """.trimIndent()

            val rawResponse = withContext(Dispatchers.IO) {
                queryApi.queryRaw(query)
            }

            // Parse the raw response
            val lines = rawResponse.split("\n")
            var powerData: PowerData? = null

            // Find the line containing the data (skip headers, etc.)
            for (line in lines) {
                if (!line.startsWith("#") && line.isNotEmpty()) {
                    val values = line.split(",")
                    if (values.size > 7) { // Adjust based on expected number of columns
                        try {
                            val solarPower = values[4].toDoubleOrNull() // Adjust index based on column order
                            val batteryLevel = values[5].toDoubleOrNull() // Adjust index based on column order
                            val gridUsage = values[6].toDoubleOrNull() // Adjust index based on column order
                            val homeUsage = values[7].toDoubleOrNull() // Adjust index based on column order

                            powerData = PowerData(
                                solarPower = solarPower?.toFloat(),
                                batteryLevel = batteryLevel?.toFloat(),
                                gridUsage = gridUsage?.toFloat(),
                                homeUsage = homeUsage?.toFloat()
                            )
                            break // Stop after finding the first valid data line
                        } catch (e: NumberFormatException) {
                            Log.e("InfluxDB", "Error parsing values: ${e.message}")
                        }
                    }
                }
            }

            emit(powerData)

        } catch (e: Exception) {
            Log.e("InfluxDB", "Error querying InfluxDB: ${e.message}", e)
            emit(null)
        }
    }

    fun close() {
        influxDBClient.close()
    }
}