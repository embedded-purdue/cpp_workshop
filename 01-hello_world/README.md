# Hello, World!

The purpose of this progam is to verify you can build and flash to the esp32. Please ensure that it builds before coming to the workshop.

To do so, click on the PlatformIO icon on the left sidebar (the alien), and click "Build". You should see it build in the terminal and end with something like `Successfully created esp32 image`.

## The environment we are working in

 Even though this workshop is for "bare metal C++", we are actually writing our programs in the [ESP-IDF](https://docs.espressif.com/projects/esp-idf/en/stable/esp32/get-started/index.html). This is a official SDK for programming ESP32 chips, and contains an entire embedded operating environment:
- bootloader (initializes the chip and load the program from flash)
- RTOS scheduler
- HAL (drivers for on-chip peripherals)
- Build system
...and more!

We are using it because it is natively supported in our chips, allows for a quick spin-up time for new developers, and gives us just enough hardware control so that we can focus on writing good code. It is even used in the production code of many IoT products!

We will not focus too much on the hardware elements of microcontrollers in this workshop. If you are interested in learning more about this, take ECE 362.