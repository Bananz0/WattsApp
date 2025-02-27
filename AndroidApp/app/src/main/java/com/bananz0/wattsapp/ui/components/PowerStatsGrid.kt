package com.bananz0.wattsapp.ui.components

import androidx.compose.foundation.layout.*
import androidx.compose.material3.Text
import androidx.compose.runtime.Composable
import androidx.compose.ui.Modifier
import androidx.compose.ui.unit.dp
import androidx.compose.foundation.lazy.grid.GridCells
import androidx.compose.foundation.lazy.grid.LazyVerticalGrid
import androidx.compose.ui.tooling.preview.Preview
import com.bananz0.wattsapp.data.model.PowerData

@Composable
fun PowerStatsGrid(powerData: PowerData?) {
    LazyVerticalGrid(
        columns = GridCells.Fixed(2), // Two columns
        contentPadding = PaddingValues(horizontal = 8.dp, vertical = 8.dp),
        modifier = Modifier.fillMaxWidth()
    ) {
        item { PowerStatItem(label = "Solar Power", value = powerData?.solarPower?.toString() ?: "N/A") }
        item { PowerStatItem(label = "Battery Level", value = powerData?.batteryLevel?.toString() ?: "N/A") }
        item { PowerStatItem(label = "Grid Usage", value = powerData?.gridUsage?.toString() ?: "N/A") }
        item { PowerStatItem(label = "Home Usage", value = powerData?.homeUsage?.toString() ?: "N/A") }
    }
}

@Composable
fun PowerStatItem(label: String, value: String) {
    Column(
        modifier = Modifier
            .padding(8.dp)
            .fillMaxWidth()
    ) {
        Text(text = label)
        Text(text = value)
    }
}

@Preview(showBackground = true)
@Composable
fun PreviewPowerStatsGrid() {
    PowerStatsGrid(
        powerData = PowerData(
            solarPower = 1500f,
            batteryLevel = 75f,
            gridUsage = 200f,
            homeUsage = 1700f
        )
    )
}