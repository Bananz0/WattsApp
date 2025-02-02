//
// Created by glenm on 2/2/2025.
//
#pragma once

#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

void signOfLife();

void LED(uint8_t lightStatus);

void testLight(const uint8_t time_delay);

void testOutputPin(char portName, const uint8_t pin);

