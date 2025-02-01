//
// Created by Jackson.Liu on 25-2-1.
//
#include "pin.h"
#include <avr/io.h>

// 模拟信号映射

// Mains Capacity (0-10V DC) 输出到 Test Bed (PB0 -> Timer0 PWM 输出)
Pin mainsCapacityPin(DDRB, PORTB, PINB, 0);

// input: Busbar Voltage (-4V ~ 4V AC)
Pin busbarVoltagePin(DDRA, PORTA, PINA, 0);

// Busbar Current (-10V ~ 10V AC)
Pin busbarCurrentPin(DDRA, PORTA, PINA, 1);

// Wind Turbine Capacity (0-5V DC)
Pin windTurbinePin(DDRA, PORTA, PINA, 2);

// PV Capacity (0-5V DC)
Pin pvCapacityPin(DDRA, PORTA, PINA, 3);

// 数字信号映射
// Call for Load 1 输入 (PD0 -> RXD0)
Pin callForLoad1Pin(DDRD, PORTD, PIND, 0);

// Call for Load 2 输入 (PD1 -> TXD0)
Pin callForLoad2Pin(DDRD, PORTD, PIND, 1);

// Call for Load 3 输入 (PD2 -> XCK0)
Pin callForLoad3Pin(DDRD, PORTD, PIND, 2);

// Switch on Load 1 输出 (PD3)
Pin switchLoad1Pin(DDRD, PORTD, PIND, 3);

// Switch on Load 2 输出 (PD4)
Pin switchLoad2Pin(DDRD, PORTD, PIND, 4);

// Switch on Load 3 输出 (PD5)
Pin switchLoad3Pin(DDRD, PORTD, PIND, 5);

// Charge Battery 输出 (PD6 -> INT0)
Pin chargeBatteryPin(DDRD, PORTD, PIND, 6);

// Discharge Battery 输出 (PD7 -> INT1)
Pin dischargeBatteryPin(DDRD, PORTD, PIND, 7);

//线性缩放工具函数
uint64_t mapToRange(uint64_t x, uint64_t in_min, uint64_t in_max, uint64_t out_min, uint64_t out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

// 初始化
void initPins() {
    // output (SM to Test Bed)
    mainsCapacityPin.setOutput();
    chargeBatteryPin.setOutput();
    dischargeBatteryPin.setOutput();
    switchLoad1Pin.setOutput();
    switchLoad2Pin.setOutput();
    switchLoad3Pin.setOutput();

    // input (Test Bed to SM)
    busbarVoltagePin.setInput();
    busbarCurrentPin.setInput();
    windTurbinePin.setInput();
    pvCapacityPin.setInput();

    callForLoad1Pin.setInputPullup();
    callForLoad2Pin.setInputPullup();
    callForLoad3Pin.setInputPullup();
}

