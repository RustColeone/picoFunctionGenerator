#ifndef PWM_CONTROLLER_H
#define PWM_CONTROLLER_H

#include "hardware/pwm.h"
#include "pico/stdlib.h"
#include "hardware/clocks.h"
#include "hardware/pll.h"
#include <vector>

class PWMController {
public:
    PWMController(uint pin, float frequency, float dutyCycle, float phaseDelay, bool isInverting, uint8_t resolutionBits = 16);

    void setDutyCycle(float newDutyCycle);
    void setFrequency(float newFrequency);
    void setPhaseDelay(float newPhaseDelay);
    void setIsInverting(bool newIsInverting);
    void setResolutionBits(uint8_t newResolutionBits);
    bool getIsInverting() const;

    static void enableAll();
    static void disableAll();

private:
    uint pin;
    float frequency;
    float dutyCycle;
    float phaseDelay;
    bool isInverting;
    uint8_t resolutionBits;
    uint slice_num;  // PWM slice number
    uint chan;       // PWM channel (either A or B)

    void initPWM();
    void updatePWM();

    static std::vector<PWMController*> controllers;  // Keeps track of all PWMController instances
};

#endif  // PWM_CONTROLLER_H
