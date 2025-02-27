package com.bananz0.wattsapp.ui.viewmodel

import android.util.Log
import androidx.lifecycle.ViewModel
import androidx.lifecycle.viewModelScope
import com.bananz0.wattsapp.data.model.PowerData
import com.bananz0.wattsapp.data.repository.InfluxDB
import com.bananz0.wattsapp.data.repository.InfluxRepository
import kotlinx.coroutines.flow.MutableStateFlow
import kotlinx.coroutines.flow.StateFlow
import kotlinx.coroutines.flow.asStateFlow
import kotlinx.coroutines.flow.catch
import kotlinx.coroutines.flow.launchIn
import kotlinx.coroutines.flow.onEach

class PowerViewModel : ViewModel() {

    private val repository = InfluxRepository()  // Instantiate the repository

    private val _powerData = MutableStateFlow<PowerData?>(null)
    val powerData: StateFlow<PowerData?> = _powerData.asStateFlow()

    init {
        repository.getLivePowerData()
            .onEach { data ->
                _powerData.value = data
            }
            .catch { exception ->
                // Handle errors from the repository
                Log.e("PowerViewModel", "Error fetching power data", exception)
                // Optionally set a default value or an error state in _powerData
                _powerData.value = null // Or PowerData() with default values if appropriate
            }
            .launchIn(viewModelScope)
    }

    override fun onCleared() {
        super.onCleared()
        InfluxDB.close() // Close the InfluxDB client when the ViewModel is destroyed
    }
}