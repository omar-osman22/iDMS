# iDMS Hardware Integration Guide

## Overview
This guide provides step-by-step instructions for integrating hardware components of the Industrial Monitoring System (iDMS). Follow these procedures to ensure proper connections and system functionality.

## System Block Diagram

```
    [12V DC Input] ─────────────────────────────────┐
           │                                        │
    [Power Management]                               │
      │    │    │    │                              │
    +5V  +3.3V AVCC +3.7V                          │
      │    │    │    │                              │
      ├────┼────┼────┼─── [ATmega128 MCU] ──────────┤
      │    │    │    │         │                    │
      │    │    │    │         ├─ PA0 ← [PT100 RTD] │
      │    │    │    │         ├─ PA1 ← [CT Sensor] │
      │    │    │    │         ├─ PD0/PD1 ↔ [GSM]   │
      │    │    │    │         └─ PE0/PE1 ↔ [LCD]   │
      │    │    │    │                               │
      └────┼────┼────┼─── [LCD Display]              │
           │    │    │                               │
           │    │    └─── [SIM800L GSM] ─────────────┘
           │    │                                     
           │    └──────── [Sensor Conditioning]       
           │                                          
           └───────────── [Digital Logic]             
```

## Pin Configuration

### ATmega128 Pin Assignments

#### Port A (ADC Inputs)
```
PA0 (ADC0)  → Temperature sensor input (PT100 conditioned)
PA1 (ADC1)  → Current sensor input (CT conditioned)  
PA2 (ADC2)  → Reserved for future analog input
PA3 (ADC3)  → Reserved for future analog input
PA4 (ADC4)  → Battery voltage monitoring (optional)
PA5 (ADC5)  → Supply voltage monitoring 
PA6 (ADC6)  → Reserved
PA7 (ADC7)  → Reserved
```

#### Port B (SPI and Digital I/O)
```
PB0 (T0)    → External timer input (optional)
PB1 (T1)    → ISP SCK (In-System Programming)
PB2 (AIN0)  → ISP MOSI  
PB3 (AIN1)  → ISP MISO
PB4 (OC0)   → PWM output (optional cooling fan)
PB5 (OC1A)  → Status LED 1
PB6 (OC1B)  → Status LED 2
PB7 (OC2)   → Alarm buzzer output
```

#### Port C (Digital I/O and Control)
```
PC0         → GSM module RESET control
PC1         → GSM module STATUS input
PC2         → Relay control 1 (alarm output)
PC3         → Relay control 2 (fault output)
PC4         → External interrupt input
PC5         → System enable/disable input
PC6         → Reserved
PC7         → Emergency stop input (active low)
```

#### Port D (UART0 and External Interrupts)
```
PD0 (RXD0)  → UART0 RX (GSM/SIM800L communication)
PD1 (TXD0)  → UART0 TX (GSM/SIM800L communication)
PD2 (INT0)  → External interrupt 0 (emergency stop)
PD3 (INT1)  → External interrupt 1 (user input)
PD4 (ICP1)  → Input capture (frequency measurement)
PD5 (XCK0)  → External clock for UART0
PD6 (T1)    → Reserved
PD7 (T2)    → Reserved
```

#### Port E (UART1 and Control)
```
PE0 (RXD1)  → UART1 RX (LCD/TopWay communication)
PE1 (TXD1)  → UART1 TX (LCD/TopWay communication)
PE2 (XCK1)  → External clock for UART1
PE3         → SD card chip select (future expansion)
PE4         → SPI chip select 1
PE5         → SPI chip select 2
PE6         → Reserved
PE7         → Reserved
```

#### Port F (ADC and JTAG)
```
PF0 (ADC0)  → Alternate ADC channel
PF1 (ADC1)  → Alternate ADC channel
PF2 (ADC2)  → Alternate ADC channel
PF3 (ADC3)  → Alternate ADC channel
PF4 (TCK)   → JTAG clock (debugging)
PF5 (TMS)   → JTAG mode select
PF6 (TDO)   → JTAG data output
PF7 (TDI)   → JTAG data input
```

#### Port G (Additional I/O)
```
PG0         → Crystal driver output
PG1         → Crystal driver input  
PG2         → Watchdog reset output
PG3         → User button input
PG4         → Status indicator output
```

## Hardware Connection Details

### 1. Power Supply Connections

#### Primary Power Input (J1)
```
Connector: 2-pin terminal block (5.08mm pitch)
Pin 1: +12V DC input (10-15V acceptable range)
Pin 2: Ground return

Wire Gauge: 18 AWG minimum for 2A current
Protection: 3A fast-blow fuse at input
```

#### Internal Power Distribution
```
+12V Input → LM7805 → +5V Digital (1A capacity)
           → AMS1117-3.3 → +3.3V Logic (800mA capacity)  
           → LM2596 → +5V Analog (500mA capacity)
           → LM2596-ADJ → +3.7V GSM (2A capacity)

Decoupling Requirements:
- 1000μF/25V bulk capacitor at +12V input
- 470μF/16V at each regulator output
- 0.1μF ceramic at each IC power pin
- 22μF tantalum for low-noise analog supplies
```

### 2. Temperature Sensor Interface (J2)

#### PT100 RTD Connection
```
Connector: 3-pin screw terminal (3.81mm pitch)
Pin 1: RTD+ (Red wire, positive RTD terminal)
Pin 2: RTD- (White wire, negative RTD terminal)  
Pin 3: Shield/Screen (Connected to analog ground)

Wire Specification:
- Cable: Shielded twisted pair
- Length: Maximum 100 meters
- Resistance: <1Ω per conductor
- Temperature rating: Matches sensor range
```

#### Signal Conditioning Circuit
```
RTD Current Source: 1mA constant current
Reference Resistor: 1000Ω ± 0.1% precision
Amplifier: INA118P instrumentation amplifier
Gain: 100 (programmed with 499Ω resistor)
Filter: 2nd order Butterworth, fc = 10Hz
Output Range: 0-5V representing -50°C to +200°C

Calibration Points:
- 0°C: 100.00Ω → 0.1V → 1.0V (after amplification)
- 100°C: 138.51Ω → 0.1385V → 1.385V (after amplification)
```

### 3. Current Sensor Interface (J3)

#### CT (Current Transformer) Connection
```
Connector: 2-pin screw terminal (5.08mm pitch)  
Pin 1: CT Secondary + (typically marked S1)
Pin 2: CT Secondary - (typically marked S2)

CT Specifications:
- Ratio: 1000:1 to 2000:1 (primary:secondary)
- Burden Resistor: 22Ω ± 5% (mounted on PCB)
- Protection: Bidirectional TVS diodes
- Maximum Current: 50A primary (25mA secondary)
```

#### Signal Conditioning Circuit
```
Burden Resistor: 22Ω, 5W wire-wound
Protection: SMBJ Series TVS diodes
AC Coupling: 1μF film capacitor
Offset Bias: VCC/2 (2.5V) for unipolar ADC
Amplifier: MCP6002 dual op-amp
Gain: 10x (adjustable with feedback resistors)
Filter: RC low-pass, fc = 1kHz
Output Range: 0-5V representing 0-25A RMS
```

### 4. LCD Display Interface (J4)

#### TopWay LCD Connection
```
Connector: 4-pin header (2.54mm pitch)
Pin 1: +5V Power (Red wire)
Pin 2: Ground (Black wire)
Pin 3: TX Data (Green wire) → PD1 (ATmega128 transmit)
Pin 4: RX Data (Blue wire) → PD0 (ATmega128 receive)

Cable Specification:
- Type: 4-conductor shielded cable
- Length: Maximum 5 meters
- Connector: JST-XH series recommended
```

#### Communication Protocol
```
UART Configuration:
- Baud Rate: 9600 bps
- Data Bits: 8
- Parity: None  
- Stop Bits: 1
- Flow Control: None

LCD Protocol: TopWay HMI protocol
- Command header: 0xAA
- Data format: Address + Data + Checksum
- Response format: Status + Data + Terminator
```

### 5. GSM Module Interface (J5)

#### SIM800L Module Connection
```
Connector: 6-pin header (2.54mm pitch)
Pin 1: +3.7V Power (Red wire, 2A capability)
Pin 2: Ground (Black wire)
Pin 3: TX Data (Green wire) → PE1 (ATmega128 transmit)
Pin 4: RX Data (Blue wire) → PE0 (ATmega128 receive)
Pin 5: RESET (Yellow wire) → PC0 (reset control)
Pin 6: STATUS (Orange wire) → PC1 (status monitoring)

Power Requirements:
- Voltage: 3.7V ± 0.1V
- Current: 2A peak (during transmission)
- Ripple: <50mV peak-to-peak
```

#### Antenna Connection (J6)
```
Connector: SMA female, 50Ω impedance
Antenna Type: GSM/GPRS quad-band (850/900/1800/1900 MHz)
Cable: RG174 coaxial, maximum 3 meters
Gain: 2-5 dBi recommended
```

#### SIM Card Socket (J7)
```
Type: Push-pull micro SIM socket
Contacts: Gold-plated for reliability
Detection: Card detection switch available
Protection: ESD protection on all lines
```

### 6. Programming and Debug Interfaces

#### ISP Header (J8)
```
Connector: 2x3 pin header (2.54mm pitch)
Pin 1: MISO (PB3) | Pin 2: VCC (+5V)
Pin 3: SCK (PB1)  | Pin 4: MOSI (PB2)  
Pin 5: RESET      | Pin 6: GND

Usage: In-System Programming with AVR programmers
Compatible: AVRISP mkII, USBasp, Arduino as ISP
```

#### JTAG Header (J9) - Optional
```
Connector: 2x5 pin header (2.54mm pitch)
Standard ARM/AVR JTAG pinout
Used for: Advanced debugging with JTAG ICE
Required for: Real-time debugging and profiling
```

#### Serial Debug (J10)
```
Connector: 4-pin header (2.54mm pitch)  
Pin 1: +5V | Pin 2: GND
Pin 3: TX  | Pin 4: RX

Usage: Serial debugging and monitoring
Interface: USB-to-Serial converter (FT232RL)
```

## System Integration Procedure

### Phase 1: Power Supply Testing
1. **Visual Inspection**
   - Check all component orientations
   - Verify solder joint quality
   - Confirm no short circuits

2. **Power-On Test**
   ```
   Step 1: Apply 12V to input terminals
   Step 2: Measure +5V digital rail (±5% tolerance)
   Step 3: Measure +3.3V logic rail (±3% tolerance)  
   Step 4: Measure +5V analog rail (±2% tolerance)
   Step 5: Measure +3.7V GSM rail (±3% tolerance)
   Step 6: Check current consumption (<200mA at startup)
   ```

### Phase 2: Microcontroller Programming
1. **ISP Connection**
   ```
   Connect programmer to ISP header (J8)
   Verify connection: avrdude -c usbasp -p m128 -v
   Program bootloader: avrdude -c usbasp -p m128 -U lfuse:w:0xFF:m
   Set high fuse: avrdude -c usbasp -p m128 -U hfuse:w:0xC9:m
   ```

2. **Firmware Upload**
   ```
   Compile project: make clean && make
   Upload firmware: make upload
   Verify operation: Check serial debug output
   ```

### Phase 3: Sensor Calibration

#### Temperature Sensor Calibration
1. **Zero Point Calibration (0°C)**
   ```
   Equipment: Ice bath (0°C ± 0.1°C)
   Procedure: Immerse PT100 in ice bath
   Expected Reading: 1.000V ± 0.01V at ADC input
   Adjustment: Modify offset constant in PT100_config.h
   ```

2. **Span Calibration (100°C)**
   ```
   Equipment: Boiling water bath (100°C ± 0.5°C)  
   Procedure: Immerse PT100 in boiling water
   Expected Reading: 1.385V ± 0.02V at ADC input
   Adjustment: Modify gain constant if required
   ```

#### Current Sensor Calibration
1. **Zero Current Calibration**
   ```
   Procedure: Disconnect CT primary current
   Expected Reading: 2.5V ± 0.05V (ADC midpoint)
   Adjustment: Trim offset bias circuit
   ```

2. **Known Current Calibration**
   ```
   Equipment: Calibrated AC current source (10A)
   Procedure: Apply 10A through CT primary
   Expected Reading: Calculate based on CT ratio and gain
   Adjustment: Modify calibration constant in CTcurrent_config.h
   ```

### Phase 4: Communication Testing

#### LCD Communication Test
1. **Basic Communication**
   ```
   Test Command: Send display test pattern
   Expected Result: LCD shows test message
   Debug: Monitor UART signals with oscilloscope
   ```

2. **Data Exchange**
   ```
   Test: Read/write LCD memory locations
   Verify: Data integrity and command responses
   Performance: Check communication timing
   ```

#### GSM Communication Test
1. **Module Detection**
   ```
   Command: AT (basic attention command)
   Expected Response: OK
   Timeout: 5 seconds maximum
   ```

2. **Network Registration**
   ```
   Command: AT+CREG? (check registration status)
   Expected Response: +CREG: 0,1 (registered)
   Retry: Up to 30 seconds for registration
   ```

3. **SMS Test**
   ```
   Command: Send test SMS to known number
   Verify: Message received successfully
   Timing: Monitor send/receive delays
   ```

### Phase 5: System Integration Testing

#### Functional Test Sequence
1. **Startup Test**
   - Power-on initialization
   - LCD display activation  
   - GSM module registration
   - Sensor baseline readings

2. **Sensor Response Test**
   - Apply known temperature to PT100
   - Apply known current to CT
   - Verify LCD display updates
   - Check alarm thresholds

3. **Communication Test**
   - Send test SMS messages
   - Verify phone list functionality
   - Test emergency calling feature
   - Check data logging capability

4. **Safety System Test**
   - Test over-temperature alarm
   - Test over-current alarm
   - Verify emergency shutdown
   - Check backup communication

## Troubleshooting Guide

### Common Issues and Solutions

#### Power Supply Problems
```
Symptom: No +5V output
Causes: Input voltage too low, regulator failure, thermal shutdown
Solution: Check input voltage, verify heat sink mounting, check for shorts

Symptom: Noisy power supplies  
Causes: Insufficient filtering, ground loops, switching noise
Solution: Add more decoupling, improve ground layout, add ferrite beads
```

#### Sensor Interface Problems
```
Symptom: Temperature reading incorrect
Causes: Wiring error, calibration drift, interference
Solution: Check 3-wire connection, recalibrate, add shielding

Symptom: Current reading unstable
Causes: CT saturation, burden resistor mismatch, EMI
Solution: Check CT rating, verify burden resistance, add filtering
```

#### Communication Problems
```
Symptom: LCD not responding
Causes: Baud rate mismatch, wiring error, power issue
Solution: Verify UART configuration, check connections, measure voltages

Symptom: GSM not connecting
Causes: Antenna problem, SIM card issue, network coverage
Solution: Check antenna connection, verify SIM activation, test signal strength
```

### Diagnostic Tools Required
- Digital multimeter (DMM)
- Oscilloscope (100MHz minimum)
- Function generator (for sensor simulation)
- Temperature calibrator
- Current calibrator  
- GSM network analyzer (optional)

## Safety Precautions

### Electrical Safety
- **Always disconnect power before making connections**
- **Use proper ESD precautions when handling PCB**
- **Verify all connections before applying power**
- **Use isolation transformers for mains-referenced measurements**

### Component Handling
- **Store components in anti-static packaging**
- **Use proper soldering temperatures and techniques**
- **Handle crystal oscillators with care (shock sensitive)**
- **Verify IC orientations before soldering**

### System Testing
- **Start with minimum configuration**
- **Test incrementally, adding one subsystem at a time**
- **Document all calibration values and settings**
- **Maintain test logs for future reference**

## Documentation and Support

### Related Documents
- [PCB Layout Design](PCB_Layout.md)
- [Bill of Materials](BOM.md)
- [Software Configuration](../Software_Guide.md)
- [Safety Guidelines](Safety_Guidelines.md)

### Support Resources
- Hardware design files available in Hardware/ directory
- Software source code with detailed comments
- Test procedures and calibration worksheets
- Troubleshooting flowcharts and checklists