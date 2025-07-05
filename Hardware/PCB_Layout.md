# iDMS PCB Layout Design

## Overview
This document describes the Printed Circuit Board (PCB) layout for the Industrial Monitoring System (iDMS). The system monitors temperature and current with safety thresholds and provides GSM-based alerting capabilities.

## System Architecture

### Core Components
- **Microcontroller**: ATmega128 (100-pin TQFP package)
- **Temperature Sensor**: PT100 RTD with signal conditioning
- **Current Sensor**: CT (Current Transformer) with burden resistor
- **Display**: TopWay LCD with UART interface
- **Communication**: SIM800L GSM module
- **Power Supply**: Multi-rail power management
- **Storage**: External EEPROM (optional, ATmega128 has internal EEPROM)

## PCB Layout Specifications

### Board Dimensions
- **Size**: 100mm x 80mm (4-layer PCB recommended)
- **Thickness**: 1.6mm standard
- **Copper Weight**: 1oz (35μm) for signal layers, 2oz (70μm) for power layers

### Layer Stack-up (4-Layer)
1. **Layer 1 (Top)**: Component placement and signal routing
2. **Layer 2 (GND)**: Ground plane
3. **Layer 3 (Power)**: Power planes (+5V, +3.3V, analog supplies)
4. **Layer 4 (Bottom)**: Signal routing and component placement

## Component Placement

### Central Processing Unit (U1)
```
ATmega128 (TQFP-100)
Position: Center of board
Orientation: Pin 1 at top-left
Decoupling: 0.1μF ceramics near VCC pins
Crystal: 16MHz, placed close to XTAL1/XTAL2
```

### Power Management Section (Top-Left)
```
Input Connector: J1 (12V DC input)
Voltage Regulators:
- U2: LM7805 (5V, 1A) with heat sink
- U3: AMS1117-3.3 (3.3V, 800mA)
- U4: LM2596 (adjustable for analog supplies)

Filtering:
- C1: 1000μF/25V electrolytic (input)
- C2-C7: 0.1μF ceramic decoupling
- C8, C9: 22μF tantalum (output filtering)
```

### Analog Input Section (Right Side)

#### Temperature Sensor Interface (U5)
```
PT100 RTD Conditioning Circuit:
- Op-amp: INA118P (instrumentation amplifier)
- Reference resistor: 1kΩ 0.1% precision
- Gain setting: 100 (Rg = 499Ω)
- Filter: 2nd order Sallen-Key (fc = 10Hz)
- ADC Connection: PA0 (Channel A0)

PT100 Connector: J2 (3-pin screw terminal)
- Pin 1: RTD+
- Pin 2: RTD- 
- Pin 3: Shield/Ground
```

#### Current Sensor Interface (U6)
```
CT Current Conditioning:
- Burden resistor: 22Ω 5W wire-wound
- Protection: TVS diodes (bidirectional)
- Amplifier: MCP6002 dual op-amp
- Gain: 10x with offset bias (Vcc/2)
- Filter: RC low-pass (fc = 1kHz)
- ADC Connection: PA1 (Channel A1)

CT Connector: J3 (2-pin screw terminal)
- Pin 1: CT Secondary +
- Pin 2: CT Secondary -
```

### Communication Section (Bottom)

#### UART LCD Interface (J4)
```
TopWay LCD Connector: 4-pin header
- Pin 1: +5V
- Pin 2: GND
- Pin 3: TX (PD1)
- Pin 4: RX (PD0)

Level shifting: Direct 5V connection
```

#### GSM Module Interface (J5)
```
SIM800L Module Connection: 6-pin header
- Pin 1: +3.7V (from dedicated regulator U7)
- Pin 2: GND
- Pin 3: TX (PE1)
- Pin 4: RX (PE0)
- Pin 5: RESET (PC0)
- Pin 6: STATUS (PC1)

Power: Dedicated 3.7V/2A regulator for GSM
Antenna: SMA connector (J6)
SIM Card: Push-pull socket (J7)
```

### Programming and Debug (Left Side)
```
ISP Header (J8): 2x3 pin header
- Pin 1: MISO (PB3)
- Pin 2: VCC
- Pin 3: SCK (PB1)
- Pin 4: MOSI (PB2)
- Pin 5: RESET
- Pin 6: GND

JTAG Header (J9): 2x5 pin header (optional)
Serial Debug: USB-to-Serial (U8: FT232RL)
```

## Electrical Connections

### Power Distribution
```
Input Power: 12V DC @ 2A minimum
│
├── U2 (LM7805) → +5V Digital Rail
│   ├── ATmega128 VCC pins
│   ├── LCD Interface
│   └── Digital Logic
│
├── U3 (AMS1117-3.3) → +3.3V Rail  
│   ├── Analog Reference (AREF)
│   └── Logic Level interfaces
│
├── U4 (LM2596) → +5V Analog Rail
│   ├── Sensor conditioning circuits
│   └── Op-amp supplies
│
└── U7 (Dedicated GSM) → +3.7V @ 2A
    └── SIM800L Module
```

### Ground System
```
Digital Ground (DGND)
│
├── Microcontroller digital pins
├── Digital ICs
└── Communication interfaces

Analog Ground (AGND)  
│
├── Sensor conditioning circuits
├── ADC reference
└── Op-amp circuits

Connection: Single point star ground at power input
Isolation: Ferrite bead between DGND and AGND
```

### Signal Routing Guidelines

#### Critical Signals
- **Crystal oscillator**: Guard with ground, minimize trace length
- **ADC inputs**: Differential routing where possible, away from digital switching
- **Power supply feedback**: Keep switching regulator loops tight
- **GSM antenna**: 50Ω controlled impedance, no vias in RF path

#### Digital Signals  
- **UART**: 100Ω differential if high speed, otherwise single-ended
- **SPI/ISP**: Star topology from MCU, series termination resistors
- **GPIO**: Standard digital routing practices

## Design for Manufacturing (DFM)

### Via Specifications
- **Standard vias**: 0.2mm drill, 0.4mm pad
- **Micro vias**: 0.1mm drill, 0.25mm pad (if needed)
- **Thermal vias**: 0.3mm drill under high-power components

### Trace Specifications
- **Minimum trace width**: 0.1mm (4 mil)
- **Minimum spacing**: 0.1mm (4 mil)  
- **Power traces**: 0.5mm minimum for 1A current
- **High-current traces**: 1.0mm for >2A current

### Component Specifications
- **Minimum component size**: 0603 for passives
- **Preferred sizes**: 0805 for ease of assembly
- **Connector pitch**: 2.54mm for field connections
- **IC packages**: TQFP/SOIC preferred over BGA

## Testing and Validation

### Built-in Test Points
- **TP1-TP4**: Power supply voltages (+12V, +5V, +3.3V, +3.7V)
- **TP5-TP6**: Analog sensor inputs (pre and post conditioning)
- **TP7-TP8**: UART communication lines
- **TP9**: Crystal oscillator
- **TP10**: Reset signal

### Test Procedures
1. **Power-on test**: Verify all supply voltages
2. **Oscillator test**: Check 16MHz crystal operation
3. **Programming test**: ISP interface functionality
4. **Sensor test**: ADC readings with known inputs
5. **Communication test**: UART and GSM functionality

## Safety Considerations

### Electrical Safety
- **Isolation**: Current sensor inputs isolated from mains
- **Protection**: TVS diodes on all external connections
- **Fusing**: Input fuse and overcurrent protection
- **ESD**: ESD protection on communication interfaces

### Thermal Management
- **Heat sinks**: Required for linear regulators
- **Thermal vias**: Under high-power components
- **Airflow**: Consider enclosure ventilation
- **Component derating**: 20% safety margin on power ratings

## Documentation References
- [Hardware Integration Guide](Hardware_Integration.md)
- [Component Specifications](BOM.md) 
- [Assembly Instructions](Assembly_Guide.md)
- [Test Procedures](Test_Procedures.md)