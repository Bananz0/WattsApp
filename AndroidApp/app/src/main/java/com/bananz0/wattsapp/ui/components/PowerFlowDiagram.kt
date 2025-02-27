package com.bananz0.wattsapp.ui.components

import androidx.compose.foundation.Canvas
import androidx.compose.foundation.layout.Box
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.foundation.layout.padding
import androidx.compose.runtime.Composable
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.draw.clipToBounds
import androidx.compose.ui.geometry.Offset
import androidx.compose.ui.geometry.Size
import androidx.compose.ui.graphics.Color
import androidx.compose.ui.graphics.drawscope.DrawScope
import androidx.compose.ui.graphics.drawscope.Stroke
import androidx.compose.ui.unit.dp
import com.bananz0.wattsapp.data.model.PowerData

@Composable
fun PowerFlowDiagram(powerData: PowerData?) {
    Box(
        modifier = Modifier
            .fillMaxSize()
            .padding(16.dp)
            .clipToBounds(),
        contentAlignment = Alignment.Center
    ) {
        if (powerData != null) {
            Canvas(modifier = Modifier.fillMaxSize()) {
                val solar = Offset(size.width * 0.2f, size.height * 0.2f)
                val battery = Offset(size.width * 0.5f, size.height * 0.5f)
                val home = Offset(size.width * 0.5f, size.height * 0.8f)
                val grid = Offset(size.width * 0.8f, size.height * 0.2f)

                val maxPower = 5000f // Adjust this based on your expected maximum power value
                val solarPowerRatio = (powerData.solarPower ?: 0f) / maxPower
                val homeUsageRatio = (powerData.homeUsage ?: 0f) / maxPower
                val gridUsageRatio = (powerData.gridUsage ?: 0f) / maxPower

                // Function to draw lines with dynamic stroke width
                fun DrawScope.drawLineWithWidth(start: Offset, end: Offset, color: Color, ratio: Float) {
                    val strokeWidth = (4.dp.toPx() * ratio).coerceIn(1f, 12.dp.toPx()) // Limit the stroke width

                    drawLine(
                        color = color,
                        start = start,
                        end = end,
                        strokeWidth = strokeWidth
                    )
                }

                drawLineWithWidth(solar, battery, Color.Green, solarPowerRatio)
                drawLineWithWidth(battery, home, Color.Yellow, homeUsageRatio)
                drawLineWithWidth(grid, home, Color.Red, gridUsageRatio)

                // Draw arcs instead of circles to indicate direction
                val arcSize = 360f // The size of the arc (full circle)
                val radius = 24f

                drawArc(
                    color = Color.Green,
                    startAngle = 0f, // Start drawing from the top
                    sweepAngle = arcSize, // Draw a full circle
                    useCenter = false, // Don't connect the edges to the center
                    topLeft = Offset(solar.x - radius, solar.y - radius), // Calculate the top-left corner of the bounding box
                    size = Size(radius * 2, radius * 2), // The size of the bounding box
                    style = Stroke(width = 4.dp.toPx()) // Style for the arc (stroke)
                )

                drawArc(
                    color = Color.Yellow,
                    startAngle = 0f,
                    sweepAngle = arcSize,
                    useCenter = false,
                    topLeft = Offset(battery.x - radius, battery.y - radius),
                    size = Size(radius * 2, radius * 2),
                    style = Stroke(width = 4.dp.toPx())
                )

                drawArc(
                    color = Color.Blue,
                    startAngle = 0f,
                    sweepAngle = arcSize,
                    useCenter = false,
                    topLeft = Offset(home.x - radius, home.y - radius),
                    size = Size(radius * 2, radius * 2),
                    style = Stroke(width = 4.dp.toPx())
                )

                drawArc(
                    color = Color.Red,
                    startAngle = 0f,
                    sweepAngle = arcSize,
                    useCenter = false,
                    topLeft = Offset(grid.x - radius, grid.y - radius),
                    size = Size(radius * 2, radius * 2),
                    style = Stroke(width = 4.dp.toPx())
                )
            }
        } else {
            // Show a loading indicator or a message that data is not available
            // This is important because powerData is a State<PowerData?>
            // so it can be null while loading or if there's an error.
            // Example:
            // CircularProgressIndicator()
        }
    }
}