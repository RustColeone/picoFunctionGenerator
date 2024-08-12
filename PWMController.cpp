#include "PWMController.h"
#include <algorithm>

// Leo: You might want to check the reference on exactly how these works:
// https://www.i-programmer.info/programming/148-hardware/14849-the-pico-in-c-basic-pwm.html?start=1

std::vector<PWMController*> PWMController::controllers;

PWMController::PWMController(uint pin, float frequency, float dutyCycle, float phaseDelay, bool isInverting, uint8_t resolutionBits)
    : pin(pin), frequency(frequency), dutyCycle(dutyCycle), phaseDelay(phaseDelay), isInverting(isInverting), resolutionBits(resolutionBits) {
    initPWM();
    controllers.push_back(this);  // Add this instance to the list of controllers
}

void PWMController::setDutyCycle(float newDutyCycle) {
    dutyCycle = newDutyCycle;
    updatePWM();
}

void PWMController::setFrequency(float newFrequency) {
    frequency = newFrequency;
    updatePWM();
}

void PWMController::setPhaseDelay(float newPhaseDelay) {
    phaseDelay = newPhaseDelay;
    updatePWM();
}

void PWMController::setIsInverting(bool newIsInverting) {
    isInverting = newIsInverting;
    updatePWM();
}

void PWMController::setResolutionBits(uint8_t newResolutionBits) {
    resolutionBits = newResolutionBits;
    updatePWM();
}

bool PWMController::getIsInverting() const {
    return isInverting;
}

void PWMController::initPWM() {
    gpio_set_function(pin, GPIO_FUNC_PWM);  // Configure the pin to use its PWM function

    // Retrieve the PWM slice and channel for the specified pin
    slice_num = pwm_gpio_to_slice_num(pin);  // Get the PWM slice number for this pin
    chan = pwm_gpio_to_channel(pin);         // Get the PWM channel for this pin (A or B)

    updatePWM();
}

void PWMController::updatePWM() {
    uint32_t clock_freq = clock_get_hz(clk_sys);  // Get the current system clock frequency
    uint32_t max_count = (1 << resolutionBits) - 1;
    float divider_ratio = (float)clock_freq / (frequency * max_count / 2);

    pwm_set_clkdiv(slice_num, divider_ratio);  // Set the clock divider for the PWM slice
    pwm_set_wrap(slice_num, max_count);        // Set the counter wrap value (top value)

    uint16_t level = (uint16_t)((max_count + 1) * (dutyCycle / 100.0) + 0.5);
    pwm_set_chan_level(slice_num, chan, level);  // Set the duty cycle level for the channel

    uint16_t delay = (uint16_t)((max_count + 1) * (phaseDelay / 100.0) + 0.5);
    pwm_set_counter(slice_num, delay);  // Set the counter value for phase delay

	// Get if the other channel is inverting
	bool isOtherChannelInverting = false;
	for (auto* controller : controllers) {
        if (controller->slice_num == slice_num) {
            isOtherChannelInverting = controller->getIsInverting();
            break;
        }
    }
	if (chan == PWM_CHAN_A) {
        pwm_set_output_polarity(slice_num, isInverting, isOtherChannelInverting);  // Set if channel A is inverting
    } else {
        pwm_set_output_polarity(slice_num, isOtherChannelInverting, isInverting);  // Set if channel B is inverting
    }

    //pwm_set_enabled(slice_num, true);  // Enable the PWM slice
}

void PWMController::enableAll() {
    for (auto* controller : controllers) {
        pwm_set_enabled(controller->slice_num, true);  // Enable each PWM slice
    }
}

void PWMController::disableAll() {
    for (auto* controller : controllers) {
        pwm_set_enabled(controller->slice_num, false);  // Disable each PWM slice
    }
}
