package com.bananz0.wattsapp.data.repository

import com.bananz0.wattsapp.data.model.PowerData
import kotlinx.coroutines.flow.Flow

class InfluxRepository {
    fun getLivePowerData(): Flow<PowerData?> {
        return InfluxDB.queryPowerData()
    }
}