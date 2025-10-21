# Embedded C++ Workshop

[Slides](https://docs.google.com/presentation/d/1YdjXo4bXQZLM8vI7Et5yl3VB8MPnTD0fWMCR_XoaUjk/edit?usp=sharing)

## Overview
This hands-on workshop introduces embedded engineers to C++ in the context of bare-metal firmware development. You’ll start from familiar C patterns and incrementally explore how C++ can improve readability, safety, and scalability in low-level environments.

By the end, you’ll:

- Gain an intuition for why C++ can be better than C for embedded code
- Learn (or review) the basics of object-oriented programming
- Leverage C++ in writing a driver, app, and designing a system
- Get ideas on personal projects, interviews, and more!

## Setup
_Please do this before coming to the workshop!_

### Requirements
- [VS Code](https://code.visualstudio.com/)
- [PlatformIO Extension](https://marketplace.visualstudio.com/items?itemName=platformio.platformio-ide)

### Steps
1. Install the requirements
2. Clone this repository
3. Open the folder `01_hello-world` in VS Code

### Verify that it works
Please see the [hello world](01_hello-world/README.md) example to ensure that it builds. If it does, you are good to go for the workshop.

If not, send me a message on discord (.aa_battery).

## Exercises
- [01-hello_world](01-hello_world/README.md) - Make sure your environment is set up correctly
- [02-led_driver](02-led_driver/README.md) - Write a simple LED driver
- [03-proximity_indicator](03-proximity_indicator/README.md) - Write an app for a proximity sensor
- [04-data_logger](04-data_logger/README.md) - Design a data logger system
