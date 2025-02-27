package com.bananz0.wattsapp.data.model

data class PowerData(
    val solarPower: Float? = null,
    val batteryLevel: Float? = null,
    val gridUsage: Float? = null,
    val homeUsage: Float? = null
)