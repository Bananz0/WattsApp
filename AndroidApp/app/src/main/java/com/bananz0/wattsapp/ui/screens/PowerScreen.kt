package com.bananz0.wattsapp.ui.screens

import androidx.compose.foundation.border
import androidx.compose.foundation.layout.*
import androidx.compose.foundation.shape.RoundedCornerShape
import androidx.compose.material3.*
import androidx.compose.runtime.Composable
import androidx.compose.runtime.collectAsState
import androidx.compose.runtime.getValue
import androidx.compose.ui.Modifier
import androidx.compose.ui.unit.dp
import androidx.lifecycle.viewmodel.compose.viewModel
import androidx.compose.ui.Alignment
import com.bananz0.wattsapp.ui.components.PowerFlowDiagram
import com.bananz0.wattsapp.ui.components.PowerStatsGrid
import com.bananz0.wattsapp.ui.viewmodel.PowerViewModel
import com.bananz0.wattsapp.data.model.PowerData
import androidx.compose.ui.tooling.preview.Preview

@Composable
fun PowerScreen(
    viewModel: PowerViewModel = viewModel(),
    testData: PowerData? = null // Add this parameter
) {
    val powerData by viewModel.powerData.collectAsState(initial = null) // Collect as State with initial value

    // Use testData if it's provided, otherwise use the data from the ViewModel
    val displayData = testData ?: powerData

    Column(
        modifier = Modifier
            .fillMaxSize()
            .padding(16.dp)
    ) {
        Text(
            "Power Flow",
            style = MaterialTheme.typography.headlineMedium,
            modifier = Modifier.padding(bottom = 16.dp)
        )

        Box(
            modifier = Modifier
                .weight(1f)
                .border(1.dp, MaterialTheme.colorScheme.outline, RoundedCornerShape(8.dp)),
            contentAlignment = Alignment.Center // Center content within the Box
        ) {
            if (displayData != null) {
                PowerFlowDiagram(displayData)
            } else {
                // Show loading indicator or error message
                CircularProgressIndicator() // Replace with your loading UI
                // Text("Loading...")
            }
        }

        Spacer(modifier = Modifier.height(16.dp))

        // Numerical stats
        PowerStatsGrid(displayData)
    }
}


@Preview(showBackground = true)
@Composable
fun PowerScreenPreview() {
    val fakeData = PowerData(
        solarPower = 2000f,
        batteryLevel = 80f,
        gridUsage = 100f,
        homeUsage = 2100f
    )
    PowerScreen(testData = fakeData)
}