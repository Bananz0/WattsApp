//
// Created by glenm on 2/5/2025.
//

#include "PowerHandler.h"
#include <avr/sleep.h>
#include <avr/power.h>
#include "pictor.h"

PowerHandler::PowerHandler() {
    power_all_enable();  //All peripherals are enabled
}

PowerHandler::~PowerHandler() {
    //Make sure all devices are in normal state when exit
    exitSleep();
    manageBacklight(true);
    manageWiFiPower(false);
}

void PowerHandler::enterSleep() {
    // pre-sleep
    manageBacklight(false);  // turn off the backlight
    manageWiFiPower(false);  // turn off the WiFi module

    //disable all peripherals
    power_adc_disable();
    power_spi_disable();
    power_timer0_disable();
    power_timer1_disable();
    power_timer2_disable();
    power_twi_disable();

    //set sleep mode
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);
    sleep_enable();

    //sleep
    sleep_mode();
}

void PowerHandler::exitSleep() {
    sleep_disable();

    //enable all peripherals
    power_adc_enable();
    power_spi_enable();
    power_timer0_enable();
    power_timer1_enable();
    power_timer2_enable();
    power_twi_enable();

    // post-sleep
    manageBacklight(true);  // turn on the backlight
    manageWiFiPower(true);  // turn on the WiFi module

    _delay_ms(100);  // wait for the WiFi module to be ready
}

void PowerHandler::manageBacklight(bool backlight) {
    if (backlight) {
        pictorBacklightState(1);
    } else {
        pictorBacklightState(0);
    }
}

void PowerHandler::manageWiFiPower(bool enable) {

}