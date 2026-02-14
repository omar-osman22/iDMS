# iDMS Assembly Guide

## Overview
This guide provides step-by-step instructions for assembling the Industrial Monitoring System (iDMS) PCB. Follow all safety procedures and quality control checkpoints to ensure reliable operation.

## Pre-Assembly Requirements

### Required Tools
- **Soldering Equipment**
  - Temperature-controlled soldering station (300-370°C)
  - Fine-tip soldering iron (0.5-1.0mm tip)
  - Hot air rework station (for SMD components)
  - Flux pen or liquid flux
  - Lead-free solder (SAC305: 96.5% Sn, 3% Ag, 0.5% Cu)
  - Solder wick for cleanup

- **Assembly Tools**
  - Anti-static mat and wrist strap
  - Fine-tip tweezers (non-magnetic)
  - 10x magnifying glass or stereo microscope
  - Precision screwdrivers (Phillips and flathead)
  - Wire strippers (24-18 AWG)
  - Heat gun (for heat shrink tubing)

- **Test Equipment**
  - Digital multimeter (DMM)
  - Oscilloscope (minimum 100MHz)
  - Function generator
  - DC power supply (0-15V, 3A)
  - AVR programmer (USBasp or similar)

- **Safety Equipment**
  - Safety glasses
  - Ventilation fan or fume extractor
  - First aid kit
  - Fire extinguisher (Class C electrical)

### Component Preparation
1. **ESD Protection**: Ensure all components are stored in anti-static packaging
2. **Component Verification**: Check all components against BOM
3. **PCB Inspection**: Verify PCB quality (no cracks, proper finish, correct dimensions)
4. **Workspace Setup**: Clean, well-lit area with proper ventilation

## Assembly Sequence

### Phase 1: SMT Component Assembly (Bottom Side)

#### Step 1.1: Solder Paste Application
```
Equipment: Solder paste (SAC305), stencil, squeegee
Procedure:
1. Align stencil with PCB bottom side
2. Apply solder paste with squeegee (45° angle)
3. Remove stencil vertically
4. Inspect paste deposits (no bridges, complete coverage)
```

#### Step 1.2: Component Placement - Passives
**Order: Resistors → Capacitors → Inductors → Diodes**

```
Resistors (0805 package):
- Use pick-and-place or fine tweezers
- Verify orientation (non-polarized)
- Check component values with markings
- Components: R1-R15 as per BOM

Capacitors (0805/1206 packages):
- Non-polarized: Place without orientation concern
- Verify capacitance markings
- Components: C1-C31 as per BOM

Inductors (1210 package):
- L1: 100μH for main switching regulator
- L2: 47μH for GSM regulator
- Verify inductance value and current rating

Diodes (SMD packages):
- D1, D2: Schottky diodes (1N5819 equivalent)
- D3, D4: TVS protection diodes
- Verify polarity markings (cathode band)
```

#### Step 1.3: Reflow Soldering - Bottom Side
```
Reflow Profile (Lead-free SAC305):
- Preheat: 150°C for 60-90 seconds
- Ramp: 2-3°C/second to 217°C
- Peak: 245-250°C for 10-30 seconds
- Cool down: <6°C/second to room temperature

Post-reflow Inspection:
- Check for cold joints (dull appearance)
- Verify no solder bridges
- Confirm all components properly seated
- Use magnification for detailed inspection
```

### Phase 2: SMT Component Assembly (Top Side)

#### Step 2.1: Flip PCB and Secure
```
Procedure:
1. Carefully flip PCB (bottom components now down)
2. Support PCB on anti-static foam or fixture
3. Clean top side with isopropyl alcohol
4. Apply solder paste to top side pads
```

#### Step 2.2: IC Component Placement
**Order: Power ICs → Microcontroller → Op-amps → Support ICs**

```
U2 (LM7805): TO-220 Package
- Verify pin 1 orientation (Input)
- Ensure proper thermal pad contact
- Place heat sink provisionally

U3 (AMS1117-3.3): SOT-223 Package  
- Pin 1 is Output (check datasheet)
- Thermal pad faces PCB
- Verify proper alignment

U4, U7 (LM2596): TO-263 Package
- Heat sinks required for proper operation
- Thermal interface material on exposed pad
- Verify input/output pin assignment

U1 (ATmega128): TQFP-64 Package
- CRITICAL: Verify pin 1 orientation (dot marking)
- Use flux for proper wetting
- Check for bent pins before placement
- Align all sides before reflow

U5 (INA118P): DIP-8 Package (if SMD version)
- Pin 1 notch orientation
- Proper amplifier power supply sequence

U6 (MCP6002): DIP-8 Package (if SMD version)
- Dual op-amp configuration
- Verify supply voltage compatibility
```

#### Step 2.3: Crystal and Timing Components
```
Y1 (16MHz Crystal): HC-49/S Package
- Handle with care (shock sensitive)
- Minimize trace length to MCU
- Place load capacitors C1, C2 nearby
- Verify crystal frequency marking
```

#### Step 2.4: Reflow Soldering - Top Side
```
Same reflow profile as bottom side
Special attention to:
- Large thermal mass components (regulators)
- Fine-pitch IC pins (ATmega128)
- Crystal placement and orientation
```

### Phase 3: Through-Hole Component Assembly

#### Step 3.1: Connectors and Headers
```
J1 (Power Input): 2-pin Screw Terminal
- Verify 5.08mm pitch
- Secure mounting to PCB
- Test connection security

J2 (PT100 Sensor): 3-pin Screw Terminal
- 3.81mm pitch
- Wire gauge acceptance: 24-12 AWG
- Verify pin 1 marking

J3 (CT Sensor): 2-pin Screw Terminal
- 5.08mm pitch for high current
- Secure clamping mechanism
- Verify current rating

J4 (LCD Interface): 4-pin Header
- 2.54mm pitch standard
- Right-angle or straight as required
- Proper mating connector supplied

J5 (GSM Module): 6-pin Socket Header
- Accepts standard pin headers
- Verify pin assignment
- Secure mounting

J6 (GSM Antenna): SMA Female Connector
- 50Ω impedance critical
- Edge-mount or panel-mount
- Verify connector type

J7 (SIM Card): Micro SIM Socket
- Push-pull mechanism
- Card detection switch connection
- ESD protection verification

J8 (ISP Programming): 2x3 Pin Header
- Standard AVR ISP pinout
- Shrouded connector recommended
- Proper pin 1 marking

J9 (JTAG Debug): 2x5 Pin Header
- ARM/AVR compatible pinout
- Optional for production units
- Shrouded connector type

J10 (Serial Debug): 4-pin Header
- USB-to-serial compatible
- Standard pinout (VCC, GND, TX, RX)
```

#### Step 3.2: Large Passive Components
```
R6 (Burden Resistor): 22Ω 5W
- Wire-wound type for stability
- Proper heat dissipation clearance
- Verify resistance value

C3 (Input Filter): 1000μF 25V
- Polarized electrolytic capacitor
- Verify polarity markings
- Secure mounting (vibration resistance)

C4, C5 (Output Filters): 470μF 16V
- Regulator output filtering
- ESR specification critical
- Temperature rating verification

L1, L2 (Power Inductors): Through-hole mount
- Current rating verification
- Core saturation limits
- Proper orientation (if directional)
```

#### Step 3.3: User Interface Components
```
LED1-LED4 (Status Indicators): 3mm LEDs
- Color coding as per specification
- Proper polarity (anode/cathode)
- Current limiting resistors verified

SW1, SW2 (Push Buttons): 6x6mm tactile
- Normally open configuration
- Proper actuation force
- Panel mount alignment

SW3 (Mode Select): SPDT Toggle
- Center-off or maintained position
- Panel mount provision
- Contact rating verification

BZ1 (Alarm Buzzer): 12mm piezo
- Voltage rating (5V operation)
- Sound pressure level (85dB minimum)
- Polarity if applicable
```

### Phase 4: Final Assembly and Testing

#### Step 4.1: Heat Sink Installation
```
Voltage Regulators (U2, U4, U7):
1. Clean IC thermal pad with isopropyl alcohol
2. Apply thin layer of thermal interface material
3. Mount heat sink with appropriate hardware
4. Verify thermal resistance meets requirements
5. Torque screws to specification (avoid over-tightening)

Heat Sink Specifications:
- Thermal resistance: <10°C/W for TO-220
- Mounting pressure: Even distribution
- Clearance: No interference with other components
```

#### Step 4.2: Quality Control Inspection
```
Visual Inspection Checklist:
□ All components properly oriented
□ No cold solder joints (shiny, complete coverage)
□ No solder bridges between pins
□ Proper component seating
□ Clean flux residue removed
□ No damaged components or traces
□ Proper connector orientation
□ Heat sinks securely mounted
□ No loose or missing components

Electrical Continuity:
□ Power supply rail continuity
□ Ground plane continuity
□ Critical signal path continuity
□ No short circuits between rails
□ Proper isolation where required
```

#### Step 4.3: Power-On Test
```
Pre-Power Test:
1. Visual inspection complete
2. Multimeter continuity tests passed
3. No obvious assembly defects
4. Power supply connected but not energized

Power-On Sequence:
1. Set input supply to 12V, current limit 500mA
2. Apply power and monitor current consumption
3. Verify output voltages:
   - +5V digital: 4.75-5.25V
   - +3.3V logic: 3.135-3.465V
   - +5V analog: 4.95-5.05V (tighter tolerance)
   - +3.7V GSM: 3.515-3.885V

Expected Current Consumption (unloaded):
- Total: <200mA
- +5V digital: <100mA
- +3.3V logic: <50mA
- +5V analog: <20mA
- +3.7V GSM: <10mA (standby)
```

## Programming and Initial Configuration

### Step 5.1: Bootloader Programming
```
ISP Connection (J8):
Pin 1: MISO ← Programmer
Pin 2: VCC (5V)
Pin 3: SCK ← Programmer  
Pin 4: MOSI ← Programmer
Pin 5: RESET ← Programmer
Pin 6: GND

Programming Commands:
avrdude -c usbasp -p m128 -v
avrdude -c usbasp -p m128 -U lfuse:w:0xFF:m
avrdude -c usbasp -p m128 -U hfuse:w:0xC9:m
avrdude -c usbasp -p m128 -U efuse:w:0xFF:m

Fuse Settings:
- Low Fuse: 0xFF (External crystal, fast startup)
- High Fuse: 0xC9 (SPI enabled, Brown-out 2.7V)
- Extended Fuse: 0xFF (Default settings)
```

### Step 5.2: Firmware Upload
```
Compilation:
cd /path/to/iDMS
make clean
make debug

Upload:
make upload

Verification:
- Serial debug output at 9600 baud
- Status LED blinking pattern
- LCD initialization sequence
- GSM module detection
```

### Step 5.3: Initial System Test
```
Basic Functionality Test:
1. Power supply stability under load
2. Crystal oscillator operation (16MHz)
3. UART communication (LCD and GSM)
4. ADC baseline readings
5. Digital I/O operation
6. Status indicators functioning

Test Equipment Setup:
- Oscilloscope: Monitor crystal and UART signals
- Multimeter: Verify all supply voltages
- Function generator: Simulate sensor inputs
- Serial terminal: Monitor debug output
```

## Quality Control and Testing

### Electrical Testing
```
Power Supply Tests:
□ Input voltage range: 10-15V
□ Output regulation: ±5% or better
□ Load regulation: 1% max
□ Ripple voltage: <50mV pk-pk
□ Current limiting: Functional
□ Thermal shutdown: Tested

Signal Integrity Tests:
□ Crystal frequency: 16MHz ±0.01%
□ UART signal levels: 0-5V, clean edges
□ ADC linearity: <1 LSB error
□ Power supply rejection: >40dB
□ EMI compliance: Meets class B limits

Functional Tests:
□ Temperature sensor: Accurate to ±1°C
□ Current sensor: Accurate to ±2%
□ LCD communication: Commands acknowledged
□ GSM registration: Network connection
□ Phone list operations: Add/remove entries
□ Alarm functions: Triggered correctly
□ Safety shutdown: Emergency stop works
```

### Environmental Testing
```
Temperature Testing:
- Operating range: -20°C to +70°C
- Storage range: -40°C to +85°C
- Thermal cycling: 10 cycles minimum

Humidity Testing:
- Operating humidity: 10-90% RH non-condensing
- Storage humidity: 5-95% RH non-condensing

Vibration Testing:
- Frequency range: 10-200Hz
- Acceleration: 2G maximum
- Duration: 30 minutes per axis

Shock Testing:
- Peak acceleration: 50G
- Pulse duration: 11ms
- Half-sine wave
```

## Troubleshooting Guide

### Common Assembly Issues

#### Power Supply Problems
```
Symptom: No output voltage
Possible Causes:
- Input fuse blown
- Regulator damaged during assembly
- Solder bridge on input
- Component placement error

Solution Steps:
1. Check input fuse continuity
2. Verify regulator orientation
3. Check for solder bridges with magnification
4. Measure regulator input voltage
5. Replace regulator if damaged
```

#### Microcontroller Issues
```
Symptom: MCU not responding to programming
Possible Causes:
- Incorrect crystal frequency
- Power supply issues
- Reset circuit malfunction
- ISP connection problems

Solution Steps:
1. Verify 5V power at MCU VCC pins
2. Check crystal oscillation with scope
3. Verify reset signal (should be high)
4. Check ISP connector wiring
5. Try different programmer
```

#### Communication Problems
```
Symptom: LCD/GSM not responding
Possible Causes:
- UART configuration mismatch
- Wiring errors
- Power supply to peripheral
- Baud rate settings

Solution Steps:
1. Verify UART signal levels with scope
2. Check power supply to peripheral
3. Verify connector pinout
4. Test with different baud rates
5. Check software configuration
```

### Component-Specific Issues

#### Temperature Sensor Interface
```
Common Issues:
- Incorrect gain setting (check R3 value)
- Noise in measurements (improve filtering)
- Offset errors (calibration required)
- PT100 wiring problems (3-wire vs 4-wire)

Debug Procedure:
1. Verify reference resistor (R2) value
2. Check amplifier power supplies
3. Measure offset voltage at amplifier output
4. Test with precision current source
5. Verify ADC reference voltage
```

#### Current Sensor Interface
```
Common Issues:
- CT ratio mismatch (check specifications)
- Burden resistor value (verify R6)
- Saturation at high currents
- AC coupling problems

Debug Procedure:
1. Verify burden resistor value and power rating
2. Check TVS diode operation
3. Test amplifier offset and gain
4. Verify AC coupling capacitor
5. Check for proper grounding
```

## Safety Considerations

### Electrical Safety
- **Power disconnection**: Always disconnect power before assembly work
- **ESD protection**: Use anti-static measures throughout assembly
- **High voltage**: Be aware of switching regulator voltages
- **Current paths**: Verify proper grounding and return paths

### Component Safety
- **Heat sensitive**: Handle crystal and semiconductors with care
- **Polarized components**: Double-check orientation before soldering
- **Static sensitive**: Maintain ESD precautions for all ICs
- **Thermal management**: Proper heat sink installation critical

### Assembly Safety
- **Soldering safety**: Use proper ventilation and temperature control
- **Chemical safety**: Handle flux and cleaning solvents safely
- **Sharp tools**: Exercise care with cutting tools and wire strippers
- **Fire safety**: Keep fire extinguisher accessible during assembly

## Documentation and Certification

### Assembly Records
- **Build log**: Record all assembly steps and any deviations
- **Test results**: Document all electrical and functional tests
- **Calibration data**: Record sensor calibration values
- **Serial numbers**: Assign unique identifier to each unit

### Certification Requirements
- **RoHS compliance**: All components lead-free and compliant
- **CE marking**: EMC and safety directive compliance
- **FCC certification**: Radio frequency emission compliance (GSM)
- **UL listing**: Safety standard compliance for industrial use

## Maintenance and Service

### Routine Maintenance
- **Calibration check**: Annual sensor calibration verification
- **Connection inspection**: Check all screw terminals for tightness
- **Cleaning**: Remove dust and debris from heat sinks
- **Software updates**: Install firmware updates as available

### Service Procedures
- **Component replacement**: Use identical or approved substitutes
- **Recalibration**: Required after sensor interface repairs
- **Testing**: Full functional test after any service work
- **Documentation**: Update service records and calibration certificates

## Related Documentation
- [PCB Layout Design](PCB_Layout.md)
- [Hardware Integration Guide](Hardware_Integration.md) 
- [Bill of Materials](BOM.md)
- [Test Procedures](Test_Procedures.md)
- [Safety Guidelines](Safety_Guidelines.md)

This assembly guide ensures reliable construction of the iDMS monitoring system with proper quality control and safety procedures.