# picoFunctionGenerator

Welcome to the **picoFunctionGenerator**! This repository contains the code for a simple function generator using a Raspberry Pi Pico. Currently, the only function available is a square wave generator. The frequency of the generated square wave is determined by dividing the Pico's system clock frequency by a desired amount. When the Pico is overclocked to 200MHz, it can generate square waves up to 12.5MHz with 4-bit resolution in duty cycle.

## Table of Contents

- [Features](#features)
- [Requirements](#requirements)
- [Installation](#installation)
- [Usage](#usage)
- [PWM Slice and Channel Assignments](#pwm-slice-and-channel-assignments)
- [Notes](#notes)
- [License](#license)

## Features

- The pico lacks an onboard DAC so currently there's only square wave available
- Square wave generation with configurable frequency, duty cycle, and phase delays.

## Requirements

- Arduino IDE
- Raspberry Pi Pico

## Installation

1. **Clone the Repository:**
   ```
   git clone https://github.com/RustColeone/picoFunctionGenerator
   ```
2. **Open the Code in Arduino IDE:**
   - Open the `picoFunctionGenerator.ino` file in the Arduino IDE.

3. **Select the Raspberry Pi Pico Board:**
   - Go to `Tools > Board > Raspberry Pi RP2040 Boards` and select `Raspberry Pi Pico`.

4. **Upload the Code:**
   - Connect your Raspberry Pi Pico to your computer.
   - Click the upload button in the Arduino IDE to upload the code to the Pico.

## Usage

### Setting Frequency and Duty Cycle

A simple math to follow

$$\frac{f_{sys}} {f_{desired} \times 2^{bit}} > 1$$
min resolution 2 bits

Once the code is uploaded, the Pico generates a square wave on the specified pins as defined. For example:
```
// Requirement: clk_feq / (feq * resolution_in_int) > 1, min resolution 2 bits
// Maximum Working Frequency: 16MHz @ 2-bit resolution
// They cannot have different frequency and resolution if they are on the same channel
PWMController pwm1(6, 0.2*1000*1000.0, 45, 0, false, 8); // Pin 6, 200 kHz, 45% duty cycle, no phase delay, 8-bit resolution
PWMController pwm2(7, 0.2*1000*1000.0, 45, 0, false, 8); // Pin 7, 200 kHz, 45% duty cycle, no phase delay, 8-bit resolution

```
The 10% duty cycle not necessarily translates to 10%, but rather an approximation to the nearest bit will be calculated instead.

### Example Usage

1. Serial Communication not working as expected right now, it appears to have some conflicts over some pins.

## PWM Slice and Channel Assignments

The table in the previous response does not include all possible PWM slice and channel assignments for the Raspberry Pi Pico. Below is the complete table for the PWM slice and channel assignments.

## PWM Slice and Channel Assignments

This is all copied and I have no idea if its true or bs

| Pin | Slice | Channel |
|-----|-------|---------|
| 0   | 0     | A       |
| 1   | 0     | B       |
| 2   | 1     | A       |
| 3   | 1     | B       |
| 4   | 2     | A       |
| 5   | 2     | B       |
| 6   | 3     | A       |
| 7   | 3     | B       |
| 8   | 4     | A       |
| 9   | 4     | B       |
| 10  | 5     | A       |
| 11  | 5     | B       |
| 12  | 6     | A       |
| 13  | 6     | B       |
| 14  | 7     | A       |
| 15  | 7     | B       |
| 16  | 0     | A       |
| 17  | 0     | B       |
| 18  | 1     | A       |
| 19  | 1     | B       |
| 20  | 2     | A       |
| 21  | 2     | B       |
| 22  | 3     | A       |
| 23  | 3     | B       |
| 24  | 4     | A       |
| 25  | 4     | B       |
| 26  | 5     | A       |
| 27  | 5     | B       |
| 28  | 6     | A       |
| 29  | 6     | B       |

Note: The PWM slices and channels are repeated for different pins to allow for multiple PWM outputs. Each slice has two channels (A and B), and you can configure different parameters for each channel. However, keep in mind that channels A and B within the same slice share the same clock divider and counter, so they will have the same frequency and phase but can have different duty cycles and output polarity.

### Notes

- Each PWM slice shares the same clock divider and counter, so channels A and B within the same slice will have identical frequencies and phase.
- If you want to generate complementary waveforms, set one channel to invert the waveform by using the `setIsInverting(true)` method.