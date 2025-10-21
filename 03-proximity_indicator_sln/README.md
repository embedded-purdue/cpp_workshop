# Proximity Indicator with Ultrasonic Sensor

This project demonstrates how to create a proximity indicator using an ultrasonic sensor (HC-SR04) and RGB LED on an ESP32 microcontroller. The LED changes color based on the measured distance to provide visual feedback about nearby objects.

## Features

- **Ultrasonic Distance Measurement**: Accurate distance readings using HC-SR04 sensor
- **Visual Proximity Indication**: RGB LED changes color based on distance ranges
- **Multiple Units Support**: Distance can be measured in centimeters, millimeters, or inches
- **Averaged Measurements**: Reduces noise by averaging multiple readings
- **Error Handling**: Comprehensive error detection and reporting
- **Range Detection**: Easy object-in-range detection with configurable thresholds

## Hardware Requirements

### Components
- ESP32 Development Board (NodeMCU-32S)
- HC-SR04 Ultrasonic Sensor
- RGB LED (Common Anode)
- Resistors (220Ω - 1kΩ for LED current limiting)
- Breadboard and jumper wires

### Wiring Connections

#### RGB LED (Common Anode)
- **Red Pin**: GPIO 27
- **Green Pin**: GPIO 26  
- **Blue Pin**: GPIO 25
- **Common Anode**: 3.3V or 5V (through appropriate resistor)

#### HC-SR04 Ultrasonic Sensor
- **VCC**: 5V (or 3.3V for 3.3V variants)
- **GND**: Ground
- **Trig**: GPIO 5
- **Echo**: GPIO 17

## Distance Indication Ranges

The RGB LED provides visual feedback based on measured distance:

| Distance Range | LED Color | Indication |
|----------------|-----------|------------|
| < 10 cm        | Red       | Very Close (Danger) |
| 10-20 cm       | Yellow    | Close (Warning) |
| 20-50 cm       | Green     | Medium Distance (Safe) |
| > 50 cm        | Blue      | Far/Clear |
| Error          | Purple    | Measurement Error |

## Software Architecture

### UltrasonicSensor Class

The `UltrasonicSensor` class provides a complete driver for HC-SR04 and similar ultrasonic sensors.

#### Key Features:
- **Distance Measurement**: Single and averaged measurements
- **Multiple Units**: Centimeters, millimeters, inches
- **Range Detection**: Boolean proximity checking
- **Error Handling**: Timeout, out-of-range, and error detection
- **Configurable Timeout**: Adjustable maximum measurement time

#### Basic Usage:

```cpp
#include "ultrasonic.h"

// Initialize sensor with trigger and echo pins
UltrasonicSensor sensor(GPIO_NUM_23, GPIO_NUM_22);

// Initialize the sensor
if (sensor.initialize() == UltrasonicSensor::Status::Success) {
    float distance;
    
    // Single measurement
    if (sensor.measure_distance(distance) == UltrasonicSensor::Status::Success) {
        std::cout << "Distance: " << distance << " cm" << std::endl;
    }
    
    // Averaged measurement (3 samples)
    if (sensor.measure_distance_avg(distance, 3) == UltrasonicSensor::Status::Success) {
        std::cout << "Average distance: " << distance << " cm" << std::endl;
    }
    
    // Check if object is within range
    if (sensor.is_object_in_range(30.0f)) {
        std::cout << "Object detected within 30cm!" << std::endl;
    }
}
```

#### Advanced Usage:

```cpp
// Use different units
float distance_mm, distance_in;
sensor.measure_distance(distance_mm, UltrasonicSensor::Unit::Millimeters);
sensor.measure_distance(distance_in, UltrasonicSensor::Unit::Inches);

// Convert between units
float cm_value = UltrasonicSensor::convert_units(distance_in, 
                                                UltrasonicSensor::Unit::Inches,
                                                UltrasonicSensor::Unit::Centimeters);

// Adjust timeout for longer range detection
sensor.set_timeout(50000);  // 50ms timeout for ~8.5m max range
```

## Technical Specifications

### HC-SR04 Sensor Specifications
- **Operating Voltage**: 5V DC (3.3V variants available)
- **Operating Current**: 15mA
- **Frequency**: 40KHz
- **Range**: 2cm - 400cm
- **Accuracy**: ±3mm
- **Measuring Angle**: 15°
- **Trigger Input**: 10μs TTL pulse
- **Echo Output**: TTL level signal, proportional to distance

### Timing Characteristics
- **Trigger Pulse**: 10μs high pulse
- **Echo Response**: High pulse duration proportional to distance
- **Measurement Cycle**: Minimum 60ms between measurements
- **Default Timeout**: 30ms (≈5.1m maximum range)

### Distance Calculation
Distance is calculated using the speed of sound:
- **Speed of Sound**: 343 m/s (at 20°C)
- **Formula**: Distance = (Echo_Duration × Speed_of_Sound) / 2
- **Division by 2**: Sound travels to object and back

## Build and Flash

1. **Install PlatformIO**: Make sure PlatformIO is installed and configured
2. **Open Project**: Open this directory in PlatformIO
3. **Build**: Run `pio run` to build the project
4. **Flash**: Run `pio run --target upload` to flash to ESP32
5. **Monitor**: Run `pio device monitor` to view serial output

## Troubleshooting

### Common Issues:

1. **No Distance Readings**
   - Check wiring connections
   - Verify power supply to sensor
   - Ensure GPIO pins are correctly configured

2. **Inconsistent Readings**
   - Use averaged measurements
   - Ensure stable mounting of sensor
   - Check for electromagnetic interference

3. **Timeout Errors**
   - Increase timeout value
   - Check for obstacles blocking sound waves
   - Verify echo pin connection

4. **LED Not Working**
   - Check LED polarity (Common Anode configuration)
   - Verify resistor values for current limiting
   - Test individual LED colors

### Serial Output Example:
```
Proximity Indicator Starting...
Ultrasonic sensor initialized successfully
Distance ranges:
  < 10cm  : Red (Very Close)
  < 20cm  : Yellow (Close)
  < 50cm  : Green (Medium)
  >= 50cm : Blue (Far/Clear)
========================================
Distance: 25.4 cm [GREEN - Medium]
Distance: 15.2 cm [YELLOW - Close]
Distance: 8.7 cm [RED - Very Close]
```

## Future Enhancements

- **Temperature Compensation**: Adjust speed of sound based on temperature
- **Multiple Sensors**: Support for sensor arrays
- **Data Logging**: Store measurement history
- **Web Interface**: Remote monitoring via WiFi
- **Sound Alerts**: Audio feedback for proximity warnings
- **Calibration**: User-adjustable distance thresholds

## License

This project is part of the CPP Workshop and is provided for educational purposes.