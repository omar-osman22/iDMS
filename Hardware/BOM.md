# iDMS Bill of Materials (BOM)

## Overview
Complete component list for the Industrial Monitoring System (iDMS) PCB assembly. Components are organized by functional category with specifications, quantities, and recommended suppliers.

## Summary
- **Total Components**: 156 items
- **Estimated Cost**: $85-120 USD (quantity 1-10)
- **Assembly Time**: 3-4 hours (experienced technician)
- **PCB Size**: 100mm x 80mm, 4-layer

## Core Components

### Microcontroller and Support
| Ref | Description | Value/Part Number | Package | Qty | Unit Cost | Supplier |
|-----|-------------|------------------|---------|-----|-----------|----------|
| U1 | ATmega128-16AU | 16MHz, 128KB Flash | TQFP-64 | 1 | $12.50 | Microchip Direct |
| Y1 | Crystal Oscillator | 16MHz ±20ppm | HC-49/S | 1 | $0.85 | Abracon |
| C1, C2 | Load Capacitors | 22pF ±5% | 0805 | 2 | $0.10 | Murata |
| R1 | Pull-up Resistor | 10kΩ ±5% | 0805 | 1 | $0.05 | Yageo |

### Power Management
| Ref | Description | Value/Part Number | Package | Qty | Unit Cost | Supplier |
|-----|-------------|------------------|---------|-----|-----------|----------|
| U2 | Linear Regulator | LM7805CT | TO-220 | 1 | $0.75 | Texas Instruments |
| U3 | Linear Regulator | AMS1117-3.3 | SOT-223 | 1 | $0.45 | Advanced Monolithic |
| U4 | Switching Regulator | LM2596S-5.0 | TO-263 | 1 | $2.20 | Texas Instruments |
| U7 | GSM Power Regulator | LM2596S-ADJ | TO-263 | 1 | $2.20 | Texas Instruments |
| L1 | Inductor | 100μH, 3A | 1210 | 1 | $1.50 | Coilcraft |
| L2 | Inductor | 47μH, 3A | 1210 | 1 | $1.35 | Coilcraft |
| D1, D2 | Schottky Diodes | 1N5819 | DO-41 | 2 | $0.25 | Vishay |
| F1 | Fuse | 3A Fast-blow | 6.3x32mm | 1 | $0.30 | Littelfuse |
| FB1 | Ferrite Bead | BLM18PG221SN1D | 0603 | 1 | $0.15 | Murata |

### Power Supply Capacitors
| Ref | Description | Value | Package | Qty | Unit Cost | Supplier |
|-----|-------------|-------|---------|-----|-----------|----------|
| C3 | Input Filter | 1000μF 25V | Radial | 1 | $1.20 | Panasonic |
| C4, C5 | Output Filter | 470μF 16V | Radial | 2 | $0.85 | Panasonic |
| C6-C11 | Decoupling | 0.1μF 50V | 0805 | 6 | $0.08 | Murata |
| C12, C13 | Low-noise Filter | 22μF 16V | 1206 | 2 | $0.45 | Taiyo Yuden |
| C14-C25 | MCU Decoupling | 0.1μF 50V | 0603 | 12 | $0.06 | Samsung |

## Analog Front-End

### Temperature Sensor Interface
| Ref | Description | Value/Part Number | Package | Qty | Unit Cost | Supplier |
|-----|-------------|------------------|---------|-----|-----------|----------|
| U5 | Instrumentation Amplifier | INA118P | DIP-8 | 1 | $4.85 | Texas Instruments |
| U5A | Precision Op-Amp | OPA2277P | DIP-8 | 1 | $3.20 | Texas Instruments |
| R2 | Reference Resistor | 1000Ω ±0.1% | 0805 | 1 | $0.85 | Vishay |
| R3 | Gain Resistor | 499Ω ±0.1% | 0805 | 1 | $0.75 | Vishay |
| R4, R5 | Filter Resistors | 1.6kΩ ±1% | 0805 | 2 | $0.12 | Yageo |
| C26, C27 | Filter Capacitors | 100nF ±5% | 0805 | 2 | $0.10 | Murata |
| C28 | Filter Capacitor | 10nF ±5% | 0805 | 1 | $0.08 | Murata |

### Current Sensor Interface
| Ref | Description | Value/Part Number | Package | Qty | Unit Cost | Supplier |
|-----|-------------|------------------|---------|-----|-----------|----------|
| U6 | Dual Op-Amp | MCP6002-I/P | DIP-8 | 1 | $0.65 | Microchip |
| R6 | Burden Resistor | 22Ω 5W | Through-hole | 1 | $1.20 | Ohmite |
| R7, R8 | Bias Resistors | 10kΩ ±1% | 0805 | 2 | $0.08 | Yageo |
| R9, R10 | Feedback Resistors | 10kΩ ±1% | 0805 | 2 | $0.08 | Yageo |
| R11 | Gain Resistor | 1kΩ ±1% | 0805 | 1 | $0.08 | Yageo |
| C29 | AC Coupling | 1μF 250V | Film | 1 | $0.85 | WIMA |
| C30, C31 | Filter Capacitors | 1nF ±5% | 0805 | 2 | $0.08 | Murata |
| D3, D4 | Protection Diodes | SMBJ5.0CA | SMB | 2 | $0.35 | Littelfuse |

## Interface Connectors

### External Connections
| Ref | Description | Specification | Mounting | Qty | Unit Cost | Supplier |
|-----|-------------|---------------|----------|-----|-----------|----------|
| J1 | Power Input | 2-pin 5.08mm | Screw Terminal | 1 | $1.25 | Phoenix Contact |
| J2 | PT100 Sensor | 3-pin 3.81mm | Screw Terminal | 1 | $1.45 | Phoenix Contact |
| J3 | CT Sensor | 2-pin 5.08mm | Screw Terminal | 1 | $1.25 | Phoenix Contact |
| J4 | LCD Interface | 4-pin 2.54mm | Pin Header | 1 | $0.25 | Samtec |
| J5 | GSM Module | 6-pin 2.54mm | Socket Header | 1 | $0.35 | Samtec |
| J6 | GSM Antenna | SMA Female | Edge Mount | 1 | $2.85 | Amphenol |
| J7 | SIM Card | Micro SIM Socket | Push-Pull | 1 | $3.25 | Molex |

### Programming and Debug
| Ref | Description | Specification | Mounting | Qty | Unit Cost | Supplier |
|-----|-------------|---------------|----------|-----|-----------|----------|
| J8 | ISP Programming | 2x3 pin 2.54mm | Pin Header | 1 | $0.18 | Samtec |
| J9 | JTAG Debug | 2x5 pin 2.54mm | Pin Header | 1 | $0.28 | Samtec |
| J10 | Serial Debug | 4-pin 2.54mm | Pin Header | 1 | $0.22 | Samtec |

## Digital I/O and Indicators

### Status Indicators
| Ref | Description | Value/Color | Package | Qty | Unit Cost | Supplier |
|-----|-------------|-------------|---------|-----|-----------|----------|
| LED1 | Power Indicator | Green 3mm | T-1 | 1 | $0.15 | Kingbright |
| LED2 | Status Indicator | Blue 3mm | T-1 | 1 | $0.18 | Kingbright |
| LED3 | Alarm Indicator | Red 3mm | T-1 | 1 | $0.15 | Kingbright |
| LED4 | Communication | Yellow 3mm | T-1 | 1 | $0.16 | Kingbright |
| R12-R15 | LED Resistors | 330Ω ±5% | 0805 | 4 | $0.05 | Yageo |

### User Interface
| Ref | Description | Specification | Package | Qty | Unit Cost | Supplier |
|-----|-------------|---------------|---------|-----|-----------|----------|
| SW1 | Reset Button | SPST Momentary | 6x6mm | 1 | $0.85 | C&K Components |
| SW2 | User Button | SPST Momentary | 6x6mm | 1 | $0.85 | C&K Components |
| SW3 | Mode Select | SPDT Toggle | Through-hole | 1 | $1.25 | E-Switch |
| BZ1 | Alarm Buzzer | 5V, 85dB | 12mm | 1 | $2.45 | Murata |

## Passive Components

### Resistors (0805 package unless noted)
| Value | Tolerance | Power | Quantity | Unit Cost | Total Cost |
|-------|-----------|-------|----------|-----------|------------|
| 10Ω | ±5% | 1/8W | 2 | $0.05 | $0.10 |
| 22Ω | ±5% | 1/8W | 4 | $0.05 | $0.20 |
| 100Ω | ±5% | 1/8W | 6 | $0.05 | $0.30 |
| 330Ω | ±5% | 1/8W | 8 | $0.05 | $0.40 |
| 1kΩ | ±5% | 1/8W | 12 | $0.05 | $0.60 |
| 2.2kΩ | ±5% | 1/8W | 4 | $0.05 | $0.20 |
| 4.7kΩ | ±5% | 1/8W | 6 | $0.05 | $0.30 |
| 10kΩ | ±5% | 1/8W | 15 | $0.05 | $0.75 |
| 47kΩ | ±5% | 1/8W | 4 | $0.05 | $0.20 |
| 100kΩ | ±5% | 1/8W | 3 | $0.05 | $0.15 |

### Capacitors
| Value | Voltage | Type | Package | Quantity | Unit Cost | Total Cost |
|-------|---------|------|---------|----------|-----------|------------|
| 10pF | 50V | C0G | 0603 | 2 | $0.08 | $0.16 |
| 22pF | 50V | C0G | 0603 | 4 | $0.08 | $0.32 |
| 100pF | 50V | C0G | 0603 | 6 | $0.08 | $0.48 |
| 1nF | 50V | X7R | 0805 | 8 | $0.08 | $0.64 |
| 10nF | 50V | X7R | 0805 | 6 | $0.08 | $0.48 |
| 100nF | 50V | X7R | 0805 | 20 | $0.08 | $1.60 |
| 1μF | 50V | X7R | 1206 | 4 | $0.25 | $1.00 |
| 10μF | 25V | X7R | 1210 | 2 | $0.45 | $0.90 |

## Test Points and Hardware

### Test Points
| Ref | Description | Type | Quantity | Unit Cost | Total Cost |
|-----|-------------|------|----------|-----------|------------|
| TP1-TP10 | Test Points | Loop 1mm | 10 | $0.12 | $1.20 |

### Mechanical Hardware
| Description | Specification | Quantity | Unit Cost | Total Cost |
|-------------|---------------|----------|-----------|------------|
| Heat Sink | TO-220, 10°C/W | 2 | $1.85 | $3.70 |
| Thermal Pad | 1mm Silicone | 2 | $0.45 | $0.90 |
| Standoffs | M3x10mm | 4 | $0.25 | $1.00 |
| Screws | M3x6mm | 8 | $0.15 | $1.20 |

## PCB Specifications

### Board Requirements
| Parameter | Specification | Notes |
|-----------|---------------|-------|
| Board Size | 100mm x 80mm | Standard Eurocard format |
| Layer Count | 4 layers | Signal/Ground/Power/Signal |
| Board Thickness | 1.6mm | Standard thickness |
| Copper Weight | 1oz/2oz | 1oz signal, 2oz power |
| Via Size | 0.2mm drill | Standard process |
| Minimum Trace | 0.1mm (4 mil) | For dense routing |
| Minimum Space | 0.1mm (4 mil) | Between traces |
| Surface Finish | HASL or ENIG | Lead-free compatible |
| Solder Mask | Green LPI | Standard color |
| Silkscreen | White | Component labels |

### Cost Breakdown (Quantity 1-10)
| Category | Cost Range | Percentage |
|----------|------------|------------|
| PCB Fabrication | $25-35 | 25-30% |
| Core Components | $35-45 | 35-40% |
| Analog Components | $15-20 | 15-18% |
| Connectors | $8-12 | 8-10% |
| Passives | $5-8 | 5-7% |
| Mechanical | $3-5 | 3-4% |
| **Total** | **$91-125** | **100%** |

## Supplier Information

### Primary Suppliers
- **Microchip Direct**: Microcontrollers, development tools
- **Digi-Key**: General electronic components, fast shipping
- **Mouser Electronics**: Full component selection, good availability
- **Arrow Electronics**: Volume pricing, design support
- **Newark/Farnell**: European distributor, good for EU orders

### PCB Fabrication
- **JLCPCB**: Low-cost prototypes, 4-layer from $15
- **PCBWay**: Good quality, competitive pricing
- **OSH Park**: USA-based, excellent quality, slower delivery
- **Seeed Studio**: Integrated assembly service available

### Specialized Components
- **Phoenix Contact**: Terminal blocks and industrial connectors
- **Amphenol**: RF connectors and antennas
- **Murata**: Inductors, filters, and ceramic components
- **Texas Instruments**: Power management and precision analog

## Assembly Notes

### Component Placement Order
1. **Power components first**: Regulators, large capacitors
2. **Core digital**: Microcontroller, crystal, decoupling
3. **Analog circuits**: Sensor interfaces, op-amps
4. **Connectors**: All external interfaces
5. **Indicators**: LEDs, test points
6. **Final check**: Programming and testing

### Critical Assembly Points
- **ESD precautions**: Use anti-static mat and wrist strap
- **Component orientation**: Verify IC pin 1 markings
- **Solder joint quality**: Inspect all connections
- **Power sequencing**: Test supplies before powering sensitive ICs
- **Calibration**: Perform sensor calibration after assembly

### Required Tools
- **Soldering**: Temperature-controlled station (300-350°C)
- **Inspection**: 10x magnifying glass or microscope
- **Testing**: Digital multimeter, oscilloscope
- **Assembly**: Flux, solder wick, isopropyl alcohol
- **Mechanical**: Screwdrivers, hex keys, wire strippers

## Quality Control

### Inspection Checklist
- [ ] All components placed correctly
- [ ] No solder bridges or cold joints
- [ ] Proper component orientations
- [ ] Clean flux residue removed
- [ ] Continuity test on power rails
- [ ] Short circuit test before power-on
- [ ] Visual inspection under magnification
- [ ] Test point accessibility verified

### Acceptance Testing
1. **Power supply test**: All voltages within specification
2. **Programming test**: Successful firmware upload
3. **Sensor test**: ADC readings respond to inputs
4. **Communication test**: UART and GSM functionality
5. **Safety test**: Alarm and protection circuits
6. **Environmental test**: Temperature and humidity exposure
7. **Burn-in test**: 24-hour continuous operation

## Revision History

| Version | Date | Changes | Author |
|---------|------|---------|--------|
| 1.0 | 2024-01-XX | Initial BOM release | Hardware Team |
| 1.1 | 2024-01-XX | Updated component values | Engineering |
| 1.2 | 2024-01-XX | Cost optimization | Procurement |

## Support and Documentation

### Related Documents
- [PCB Layout Design](PCB_Layout.md)
- [Hardware Integration Guide](Hardware_Integration.md)
- [Assembly Instructions](Assembly_Guide.md)
- [Test Procedures](Test_Procedures.md)

### Contact Information
- **Hardware Design**: hardware@company.com
- **Procurement**: procurement@company.com
- **Technical Support**: support@company.com
- **Documentation**: docs@company.com