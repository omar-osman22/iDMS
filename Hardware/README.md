# iDMS Hardware Documentation

## Overview
Complete hardware design documentation for the Industrial Monitoring System (iDMS). This system provides real-time monitoring of temperature and current with SMS-based alerting capabilities for industrial applications.

## System Specifications

### Key Features
- **Temperature Monitoring**: PT100 RTD sensor (-50°C to +200°C)
- **Current Monitoring**: CT sensor (0-50A primary, configurable)
- **Display Interface**: TopWay LCD with UART communication
- **Communication**: SIM800L GSM module for SMS and voice calls
- **Safety Systems**: Configurable alarm thresholds and emergency shutdown
- **Data Storage**: Phone list management with EEPROM storage
- **Power Management**: Multi-rail power supply from 12V DC input

### Technical Specifications
| Parameter | Specification | Notes |
|-----------|---------------|-------|
| **Input Voltage** | 12V DC ±20% | 10-15V acceptable range |
| **Power Consumption** | <3A @ 12V | Maximum with GSM transmission |
| **Temperature Range** | -50°C to +200°C | ±1°C accuracy |
| **Current Range** | 0-50A primary | ±2% accuracy |
| **Operating Environment** | -20°C to +70°C | 10-90% RH non-condensing |
| **PCB Dimensions** | 100mm x 80mm | 4-layer design |
| **Communication** | GSM 850/900/1800/1900MHz | Global coverage |

## Documentation Structure

### 📄 Core Documentation Files

#### [PCB_Layout.md](PCB_Layout.md)
Comprehensive PCB design specifications including:
- Component placement and routing guidelines
- Layer stack-up configuration (4-layer)
- Power distribution and grounding strategy
- Signal integrity considerations
- Design for manufacturing (DFM) requirements
- Testing and validation procedures

#### [Hardware_Integration.md](Hardware_Integration.md)
Detailed hardware integration guide covering:
- Complete pin configuration for ATmega128
- Step-by-step connection procedures
- Signal conditioning circuit design
- Communication interface setup
- Calibration procedures for sensors
- System integration testing

#### [PCB_Schematic.md](PCB_Schematic.md)
Visual schematic representation including:
- ASCII art system block diagram
- Component interconnection details
- Power supply distribution
- Critical signal routing
- Layer stack-up visualization
- Physical component placement map

#### [BOM.md](BOM.md)
Complete Bill of Materials featuring:
- Detailed component specifications
- Supplier information and part numbers
- Cost analysis and quantity requirements
- PCB fabrication specifications
- Quality control requirements
- Assembly time estimates

#### [Assembly_Guide.md](Assembly_Guide.md)
Step-by-step assembly instructions including:
- Required tools and equipment
- SMT and through-hole assembly sequence
- Soldering techniques and profiles
- Quality control checkpoints
- Programming and initial configuration
- Troubleshooting procedures

#### [Safety_Guidelines.md](Safety_Guidelines.md)
Comprehensive safety documentation covering:
- Electrical safety procedures
- Chemical handling requirements
- Thermal management considerations
- Emergency response procedures
- Training and competency requirements
- Regulatory compliance guidelines

## Quick Start Guide

### 1. Hardware Assembly
```bash
# Review all documentation before starting
1. Read Safety_Guidelines.md completely
2. Follow Assembly_Guide.md step-by-step
3. Verify against BOM.md component list
4. Use PCB_Schematic.md for reference
5. Test according to integration guide
```

### 2. Initial Testing
```bash
# Power supply verification
1. Check all voltage rails: +12V, +5V, +3.3V, +3.7V
2. Verify current consumption <200mA unloaded
3. Test crystal oscillator: 16MHz operation
4. Check reset circuit: Clean reset signal
```

### 3. Programming and Configuration
```bash
# Firmware upload using ISP
make clean && make debug
make upload

# Verify operation
- Serial debug output at 9600 baud
- LCD initialization sequence
- GSM module registration
- Sensor baseline readings
```

## Hardware Components

### Central Processing
- **Microcontroller**: ATmega128 (TQFP-64)
- **Crystal**: 16MHz ±20ppm
- **Memory**: 128KB Flash, 4KB EEPROM, 4KB SRAM
- **Programming**: ISP and JTAG interfaces

### Power Management
- **Input**: 12V DC, up to 3A
- **Regulators**: LM7805 (+5V), AMS1117-3.3 (+3.3V), LM2596 (adjustable)
- **Protection**: Fuses, TVS diodes, thermal shutdown
- **Filtering**: Multi-stage capacitive and inductive filtering

### Sensor Interfaces
- **Temperature**: PT100 RTD with INA118P instrumentation amplifier
- **Current**: CT sensor with burden resistor and MCP6002 conditioning
- **ADC**: 10-bit internal ADC with 3.3V reference
- **Protection**: TVS diodes and input filtering

### Communication
- **LCD**: TopWay HMI via UART0 (9600 baud)
- **GSM**: SIM800L module via UART1 (9600 baud)
- **Antenna**: SMA connector for external antenna
- **SIM Card**: Micro SIM socket with card detection

### User Interface
- **LEDs**: Power, status, alarm, communication indicators
- **Switches**: Reset, user input, mode selection
- **Buzzer**: 85dB alarm notification
- **Emergency Stop**: Hardware interrupt safety circuit

## Installation Requirements

### Environmental
- **Temperature**: -20°C to +70°C operating
- **Humidity**: 10-90% RH non-condensing
- **Vibration**: <2G, 10-200Hz
- **EMI**: Industrial environment compatibility
- **Enclosure**: IP54 minimum for outdoor installation

### Electrical
- **Power**: 12V DC supply, 3A minimum capacity
- **Grounding**: Proper earth ground connection required
- **Protection**: GFCI recommended for safety
- **Isolation**: Galvanic isolation for current measurements

### Mechanical
- **Mounting**: Standard DIN rail or panel mount
- **Clearance**: 50mm minimum on all sides for ventilation
- **Access**: Front panel access for indicators and controls
- **Cable Entry**: Sealed glands for outdoor installation

## Sensor Connections

### PT100 RTD Temperature Sensor
```
Connector: J2 (3-pin screw terminal)
Pin 1: RTD+ (Red wire)
Pin 2: RTD- (White wire)  
Pin 3: Shield (Connected to ground)

Cable: Shielded twisted pair
Length: Maximum 100 meters
Resistance: <1Ω per conductor
```

### CT Current Sensor
```
Connector: J3 (2-pin screw terminal)
Pin 1: CT Secondary + (S1 marking)
Pin 2: CT Secondary - (S2 marking)

CT Ratio: 1000:1 to 2000:1 typical
Primary Current: Up to 50A
Secondary: 25-50mA at full scale
```

## Calibration Procedures

### Temperature Calibration
1. **Ice Point**: 0°C ±0.1°C reference
2. **Boiling Point**: 100°C ±0.5°C reference  
3. **Span Adjustment**: Software offset and gain
4. **Verification**: ±1°C accuracy across range

### Current Calibration
1. **Zero Current**: Disconnect CT primary
2. **Known Current**: Apply calibrated AC current
3. **Gain Adjustment**: Software calibration constant
4. **Verification**: ±2% accuracy across range

## Troubleshooting

### Common Issues

#### Power Supply Problems
- **No +5V**: Check input fuse, regulator orientation
- **Noisy supplies**: Add filtering, check ground loops
- **Overheating**: Verify heat sink installation

#### Sensor Interface Issues
- **Temperature errors**: Check PT100 wiring, recalibrate
- **Current instability**: Verify CT connections, check burden resistor
- **ADC noise**: Improve filtering, check reference voltage

#### Communication Failures
- **LCD not responding**: Check UART settings, power supply
- **GSM no network**: Verify antenna, check SIM card
- **Programming failure**: Check ISP connections, crystal operation

## Quality Assurance

### Testing Requirements
- **Electrical testing**: All voltage rails, signal integrity
- **Functional testing**: Sensor accuracy, communication
- **Environmental testing**: Temperature, humidity, vibration
- **Safety testing**: Isolation, protection circuits
- **EMC testing**: Emissions and immunity compliance

### Certification
- **RoHS**: Lead-free compliance for all components
- **CE Marking**: EMC and safety directive compliance
- **FCC**: Radio frequency emission certification
- **UL**: Safety standard compliance for industrial use

## Maintenance

### Routine Maintenance
- **Annual calibration**: Verify sensor accuracy
- **Connection inspection**: Check terminal tightness
- **Cleaning**: Remove dust from heat sinks
- **Software updates**: Install latest firmware

### Spare Parts
- **Critical components**: Keep spare regulators, MCU
- **Wear items**: Fuses, connectors, cables
- **Calibration standards**: Reference thermometer, current source
- **Test equipment**: Multimeter, oscilloscope access

## Support Resources

### Technical Support
- **Hardware Design**: Complete design files available
- **Software Source**: Commented source code provided
- **Application Notes**: Sensor interface design details
- **Test Procedures**: Validation and calibration worksheets

### Training Materials
- **Assembly Training**: Video guides and hands-on workshops
- **Safety Training**: Electrical safety and emergency procedures
- **Operation Training**: System configuration and monitoring
- **Maintenance Training**: Calibration and troubleshooting

### Documentation Updates
This documentation is maintained with the hardware design. Check for updates:
- Version history in each document
- Change notifications via project repository
- Technical bulletins for field issues
- User feedback incorporation

## Related Documentation
- **Software Guide**: `/Software/README.md`
- **System Integration**: `/System_Integration.md`
- **User Manual**: `/Documentation/User_Manual.md`
- **Regulatory Documentation**: `/Compliance/`

## Contact Information
- **Hardware Design**: hardware-team@company.com
- **Technical Support**: support@company.com
- **Documentation**: docs@company.com
- **Emergency Support**: Available 24/7 for critical systems

---

**Document Version**: 1.0  
**Last Updated**: January 2024  
**Review Date**: June 2024  
**Approved By**: Hardware Engineering Team