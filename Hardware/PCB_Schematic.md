# iDMS PCB Schematic Diagram

## System Overview Schematic

```
                              iDMS Industrial Monitoring System
                                     PCB Schematic Layout
                              ═══════════════════════════════════

┌─ Power Input ─────────────────────────────────────────────────────────────┐
│  J1                                                                       │
│ ┌───┐    F1     L1    ┌─────────┐    C4     ┌─────────┐                   │
│ │+12V├─── 3A ───100μH─┤ LM7805  ├───470μF───┤  +5V    │ ──→ Digital       │
│ │GND │         ┌──────┤         │           │ Rails   │                   │
│ └───┘         │      └─────────┘           └─────────┘                   │
│               │                                                          │
│               │      ┌─────────┐    C5     ┌─────────┐                   │
│               └──────┤AMS1117  ├───470μF───┤ +3.3V   │ ──→ Logic         │
│                      │  -3.3   │           │ Rails   │                   │
│                      └─────────┘           └─────────┘                   │
│                                                                          │
│               L2     ┌─────────┐    C12    ┌─────────┐                   │
│              47μH────┤ LM2596  ├───22μF────┤ +5V_A   │ ──→ Analog        │
│                      │ (Analog)│           │ Rails   │                   │
│                      └─────────┘           └─────────┘                   │
│                                                                          │
│                      ┌─────────┐    C13    ┌─────────┐                   │
│                      ┤ LM2596  ├───22μF────┤ +3.7V   │ ──→ GSM           │
│                      │ (GSM)   │           │ Rails   │                   │
│                      └─────────┘           └─────────┘                   │
└──────────────────────────────────────────────────────────────────────────┘

┌─ Microcontroller Core ────────────────────────────────────────────────────┐
│                                                                           │
│    Y1                              U1 ATmega128                           │
│  ┌──────┐     C1      C2         ┌─────────────────────┐                  │
│  │16MHz │────22pF────22pF────────┤XTAL1           PA0  ├─→ Temp ADC       │
│  │      │                        │                PA1  ├─→ Current ADC    │
│  └──────┘                        │XTAL2           PA2  ├─→ (Reserved)     │
│                                  │                PA3  ├─→ (Reserved)     │
│  R1                              │RESET           PA4  ├─→ Battery Mon    │
│  10kΩ ──┐                        │                PA5  ├─→ Supply Mon     │
│          ├─ RESET                │VCC      GND    PA6  ├─→ (Reserved)     │
│         ┌┴┐                       │                PA7  ├─→ (Reserved)     │
│         │ │ SW1                   │                     │                  │
│         └┬┘                       │PB0             PD0  ├─→ UART0_RX (GSM) │
│          └─ GND                   │PB1 (SCK)       PD1  ├─→ UART0_TX (GSM) │
│                                   │PB2 (MOSI)      PD2  ├─→ INT0 (EmrgStop)│
│                                   │PB3 (MISO)      PD3  ├─→ INT1 (User)    │
│                                   │PB4             PD4  ├─→ (Reserved)     │
│                                   │PB5             PD5  ├─→ (Reserved)     │
│                                   │PB6             PD6  ├─→ (Reserved)     │
│                                   │PB7             PD7  ├─→ (Reserved)     │
│                                   │                     │                  │
│                                   │PC0             PE0  ├─→ UART1_RX (LCD) │
│                                   │PC1             PE1  ├─→ UART1_TX (LCD) │
│                                   │PC2             PE2  ├─→ (Reserved)     │
│                                   │PC3             PE3  ├─→ SD_CS          │
│                                   │PC4             PE4  ├─→ SPI_CS1        │
│                                   │PC5             PE5  ├─→ SPI_CS2        │
│                                   │PC6             PE6  ├─→ (Reserved)     │
│                                   │PC7             PE7  ├─→ (Reserved)     │
│                                   └─────────────────────┘                  │
│                                                                           │
│  ISP Programming (J8)             Status LEDs                             │
│  ┌─────────────┐                  LED1 LED2 LED3 LED4                     │
│  │1-MISO  2-VCC│                   │    │    │    │                       │
│  │3-SCK   4-MOSI│←────────────── PB3  PB5  PB6  PB7                      │
│  │5-RST   6-GND │                  ↓    ↓    ↓    ↓                       │
│  └─────────────┘                 330Ω 330Ω 330Ω 330Ω                    │
│                                   ↓    ↓    ↓    ↓                       │
│                                  GND  GND  GND  GND                      │
└───────────────────────────────────────────────────────────────────────────┘

┌─ Temperature Sensor Interface ────────────────────────────────────────────┐
│                                                                           │
│  J2 PT100 Connector                      U5 INA118P                      │
│  ┌─────────────┐                        ┌─────────────┐                   │
│  │1-RTD+       │ ──→ R2 (1kΩ ref) ──→   │+IN      OUT │ ──→ Filter ──→ PA0│
│  │2-RTD-       │ ──→ Current Source ──→  │-IN      RG  │                   │
│  │3-Shield/GND │ ──→ AGND                │VCC      GND │                   │
│  └─────────────┘                        └─────────────┘                   │
│                                                ↑                          │
│                                               R3                          │
│                                              499Ω                         │
│                                          (Gain = 100)                     │
│                                                                           │
│  2nd Order Sallen-Key Filter (fc = 10Hz)                                 │
│  ┌──────────────────────────────────────────────────────────────┐        │
│  │     R4        C26         R5        C27                      │        │
│  │ IN ──1.6kΩ────┬────100nF──1.6kΩ────┬────100nF──── OUT       │        │
│  │               │                     │        │               │        │
│  │               └─── C28 (10nF) ──────┘        └─── U5A ───────┘        │
│  │                                                   (Buffer)            │
│  └──────────────────────────────────────────────────────────────┘        │
│                                                                           │
│  Signal Range: 0-5V representing -50°C to +200°C                         │
│  Resolution: ~0.05°C (with 10-bit ADC)                                   │
└───────────────────────────────────────────────────────────────────────────┘

┌─ Current Sensor Interface ────────────────────────────────────────────────┐
│                                                                           │
│  J3 CT Connector                         Protection & Conditioning        │
│  ┌─────────────┐                        ┌─────────────────────────────────┐│
│  │1-CT_SEC+    │ ──→ D3 (TVS) ──→       │         U6 MCP6002              ││
│  │2-CT_SEC-    │ ──→ D4 (TVS) ──→       │    ┌─────────────────────────┐  ││
│  └─────────────┘         │              │    │ Op-Amp 1 (AC Coupling) │  ││
│                           ↓              │    │                         │  ││
│                    R6 (22Ω, 5W)          │C29 │+IN              OUT     │  ││
│                    Burden Resistor       │1μF │-IN (VCC/2 bias) │      │  ││
│                           │              │    │VCC              GND     │  ││
│                           ↓              │    └─────────────────────────┘  ││
│                    AC Coupling                       │                    ││
│                      (C29, 1μF)                      ↓                    ││
│                           │              │    ┌─────────────────────────┐  ││
│                           ↓              │    │ Op-Amp 2 (Amplifier)   │  ││
│                    Bias to VCC/2         │    │                         │  ││
│                    (R7, R8 = 10kΩ)       │    │+IN              OUT ────┼──┼→ PA1
│                           │              │    │-IN (Feedback)   │       │  ││
│                           ↓              │    │VCC              GND     │  ││
│                    Amplifier             │    └─────────────────────────┘  ││
│                    (Gain = 10)           │                 ↑               ││
│                           │              │           Feedback Network      ││
│                           ↓              │           R9, R10 (10kΩ)        ││
│                    Low-pass Filter       │           R11 (1kΩ gain)        ││
│                    (C30, C31 = 1nF)      │                                ││
│                           │              └─────────────────────────────────┘│
│                           ↓                                                │
│                    Output to ADC                                          │
│                                                                           │
│  Signal Range: 0-5V representing 0-25A RMS                               │
│  Resolution: ~0.025A (with 10-bit ADC)                                   │
│  Protection: Bidirectional TVS diodes for transient protection           │
└───────────────────────────────────────────────────────────────────────────┘

┌─ Communication Interfaces ────────────────────────────────────────────────┐
│                                                                           │
│  LCD Interface (J4)                     GSM Module Interface (J5)        │
│  ┌─────────────┐                        ┌─────────────────────────────────┐│
│  │1-+5V        │ ←── +5V Rail           │1-+3.7V      ←── +3.7V Rail     ││
│  │2-GND        │ ←── GND                │2-GND        ←── GND             ││
│  │3-TX_LCD     │ ←── PE1 (UART1_TX)     │3-TX_GSM     ←── PD1 (UART0_TX) ││
│  │4-RX_LCD     │ ──→ PE0 (UART1_RX)     │4-RX_GSM     ──→ PD0 (UART0_RX) ││
│  └─────────────┘                        │5-RESET      ←── PC0 (Control)  ││
│                                         │6-STATUS     ──→ PC1 (Monitor)  ││
│  UART1 Configuration:                   └─────────────────────────────────┘│
│  - Baud: 9600 bps                                                        │
│  - 8N1 (8 data, no parity, 1 stop)                                      │
│  - TopWay HMI Protocol                   UART0 Configuration:             │
│                                         - Baud: 9600 bps                 │
│                                         - 8N1 (8 data, no parity, 1 stop)│
│                                         - AT Command Protocol             │
│                                                                           │
│  GSM Antenna (J6)                       SIM Card Socket (J7)             │
│  ┌─────────────┐                        ┌─────────────────────────────────┐│
│  │  SMA Female │ ←── Antenna            │  Micro SIM Socket               ││
│  │  50Ω        │     (2-5dBi gain)      │  - Card detect switch           ││
│  │  Impedance  │                        │  - ESD protection               ││
│  └─────────────┘                        │  - Gold-plated contacts         ││
│                                         └─────────────────────────────────┘│
└───────────────────────────────────────────────────────────────────────────┘

┌─ Digital I/O and Control ─────────────────────────────────────────────────┐
│                                                                           │
│  User Interface                          Alarm and Control Outputs        │
│  ┌─────────────────────────────────────┐ ┌─────────────────────────────────┐│
│  │ SW2 (User Button)                   │ │ BZ1 (Alarm Buzzer)              ││
│  │ ┌─────┐                             │ │ ┌─────────┐                     ││
│  │ │     ├─── Pull-up ─── +5V          │ │ │ 85dB    │ ←── PB7             ││
│  │ │ SW2 ├─── PD3 (INT1)               │ │ │ 5V      │                     ││
│  │ │     ├─── GND (when pressed)       │ │ │ 12mm    │                     ││
│  │ └─────┘                             │ │ └─────────┘                     ││
│  │                                     │ │                                 ││
│  │ SW3 (Mode Select)                   │ │ Relay Outputs                   ││
│  │ ┌─────┐                             │ │ ┌─────────────────────────────┐ ││
│  │ │SPDT ├─── Common ─── +5V           │ │ │ K1 (Alarm Relay)            │ ││
│  │ │     ├─── NC ─── PC5               │ │ │ ┌─────┐                     │ ││
│  │ │     ├─── NO ─── PC6               │ │ │ │Coil ├── PC2               │ ││
│  │ └─────┘                             │ │ │ │12V  │                     │ ││
│  └─────────────────────────────────────┘ │ │ └─────┘                     │ ││
│                                          │ │ ┌─────┐  (Flyback Diode)    │ ││
│  Emergency Stop Circuit                  │ │ │Cont.├── Output Terminals   │ ││
│  ┌─────────────────────────────────────┐ │ │ │SPDT │                     │ ││
│  │ Emergency Stop Button               │ │ │ └─────┘                     │ ││
│  │ ┌─────┐                             │ │ └─────────────────────────────┘ ││
│  │ │ E-  ├─── NC (Normally Closed)     │ │                                 ││
│  │ │STOP ├─── PC7 (INT2)               │ │ K2 (Fault Relay) - Similar      ││
│  │ │     ├─── GND (when activated)     │ │ configuration on PC3            ││
│  │ └─────┘                             │ │                                 ││
│  │                                     │ │                                 ││
│  │ Safety Circuit:                     │ │                                 ││
│  │ - Hardware interrupt on activation  │ │                                 ││
│  │ - Immediate system shutdown         │ │                                 ││
│  │ - Alarm notification                │ │                                 ││
│  └─────────────────────────────────────┘ └─────────────────────────────────┘│
└───────────────────────────────────────────────────────────────────────────┘

┌─ Ground and Reference System ─────────────────────────────────────────────┐
│                                                                           │
│  Ground Plane Distribution                                                │
│  ┌─────────────────────────────────────────────────────────────────────┐  │
│  │                        Single Point Ground                          │  │
│  │                              │                                      │  │
│  │  ┌──── Digital Ground (DGND) ┼──── Analog Ground (AGND)              │  │
│  │  │           │               │              │                       │  │
│  │  │     ┌─────┴─────┐         │        ┌─────┴─────┐                 │  │
│  │  │     │ MCU GND   │         │        │Sensor GND│                 │  │
│  │  │     │ Logic GND │         │        │Op-amp GND│                 │  │
│  │  │     │ Comm GND  │         │        │ ADC GND   │                 │  │
│  │  │     └───────────┘         │        └───────────┘                 │  │
│  │  │                           │                                      │  │
│  │  └─── FB1 (Ferrite Bead) ────┴──── Power Supply Common              │  │
│  │       (Noise Isolation)                                             │  │
│  │                                                                     │  │
│  │  Voltage Reference (AREF)                                           │  │
│  │  ┌─────────────────────────────────────────────────────────────────┐ │  │
│  │  │  +3.3V ──── 100Ω ──── C_filter ──── AREF (ATmega128)          │ │  │
│  │  │                         │                                       │ │  │
│  │  │                        GND                                      │ │  │
│  │  │                                                                 │ │  │
│  │  │  Features:                                                      │ │  │
│  │  │  - Filtered 3.3V reference for stable ADC operation            │ │  │
│  │  │  - Decoupled from digital switching noise                      │ │  │
│  │  │  - Precision voltage reference for accurate measurements       │ │  │
│  │  └─────────────────────────────────────────────────────────────────┘ │  │
│  └─────────────────────────────────────────────────────────────────────┘  │
└───────────────────────────────────────────────────────────────────────────┘

┌─ Test Points and Debug Interfaces ───────────────────────────────────────┐
│                                                                           │
│  Power Supply Test Points                Debug and Programming             │
│  ┌─────────────────────────────────────┐ ┌─────────────────────────────────┐│
│  │ TP1: +12V Input                     │ │ JTAG Interface (J9)             ││
│  │ TP2: +5V Digital                    │ │ ┌─────────────────────────────┐ ││
│  │ TP3: +3.3V Logic                    │ │ │1-VCC    2-NC    3-nTRST     │ ││
│  │ TP4: +5V Analog                     │ │ │4-GND    5-TDI   6-NC        │ ││
│  │ TP5: +3.7V GSM                      │ │ │7-TMS    8-TDO   9-TCK       │ ││
│  │ TP6: GND Reference                  │ │ │10-GND                       │ ││
│  └─────────────────────────────────────┘ │ └─────────────────────────────┘ ││
│                                          │                                 ││
│  Signal Test Points                      │ Serial Debug (J10)              ││
│  ┌─────────────────────────────────────┐ │ ┌─────────────────────────────┐ ││
│  │ TP7: Temperature ADC Input          │ │ │1-+5V     3-TX (Debug out)   │ ││
│  │ TP8: Current ADC Input              │ │ │2-GND     4-RX (Debug in)    │ ││
│  │ TP9: UART0 TX (GSM)                 │ │ └─────────────────────────────┘ ││
│  │ TP10: UART1 TX (LCD)                │ │                                 ││
│  │ TP11: Crystal Oscillator            │ │ Debug Features:                 ││
│  │ TP12: Reset Signal                  │ │ - Real-time data monitoring     ││
│  └─────────────────────────────────────┘ │ - Configuration interface       ││
│                                          │ - Error logging and diagnostics ││
│                                          │ - Calibration assistance        ││
│                                          └─────────────────────────────────┘│
└───────────────────────────────────────────────────────────────────────────┘
```

## Component Layout Map

```
                              Physical Component Layout
                                   (Top View, 100mm x 80mm)

    0    10   20   30   40   50   60   70   80   90   100mm
    ├────┼────┼────┼────┼────┼────┼────┼────┼────┼────┤
 0  │    │    │ J1 │    │    │    │    │    │    │    │  0
    │    │ F1 │PWR │    │ TP1-TP6   │    │    │    │    │
    │    │ U2 │    │    │(Test Points)   │    │    │    │
 10 ├────┼ U3 ┼ U4 ┼────┼────┼────┼────┼ J2 ┼────┼────┤ 10
    │    │(Regulators) │    │    │    │PT100│    │    │
    │    │ L1 │ L2 │    │    │    │    │    │    │    │
    │ J8 │ C3 │ C4 │    │    │    │    │ U5 │    │ J6 │
 20 ├ISP─┼ C5 ┼────┼────┼────┼ Y1 ┼────┼INA ┼────┼ANT─┤ 20
    │    │    │    │    │    │16MHz│    │118 │    │    │
    │ J9 │    │    │    │  C1│ C2 │    │    │    │    │
    │JTAG│    │    │    │    │    │    │    │    │    │
 30 ├────┼────┼────┼────┼ U1 ┼────┼────┼────┼ J3 ┼────┤ 30
    │    │    │    │    │ATmega128  │    │    │ CT │    │
    │ J10│    │    │    │    │    │    │    │Sensor   │
    │DBG │    │    │    │TQFP-64    │    │ U6 │    │    │
 40 ├────┼────┼────┼────┼────┼────┼────┼MCP ┼────┼────┤ 40
    │    │    │    │    │    │    │    │6002│    │    │
    │    │    │    │    │    │    │    │    │    │    │
    │LED1│LED2│LED3│LED4│    │    │    │ R6 │    │    │
 50 ├────┼────┼────┼────┼────┼────┼────┼22Ω ┼────┼────┤ 50
    │    │    │    │    │    │    │    │5W  │    │    │
    │    │    │    │    │    │    │    │    │    │    │
    │ SW1│ SW2│ SW3│    │    │    │    │    │    │    │
 60 ├────┼────┼────┼────┼────┼────┼ J4 ┼────┼ J5 ┼ J7 ┤ 60
    │    │    │    │    │    │    │LCD │    │GSM │SIM │
    │    │    │    │    │    │    │UART│    │    │    │
    │    │    │    │    │    │    │    │    │ U7 │    │
 70 ├────┼────┼────┼────┼────┼────┼────┼────┼GSM ┼────┤ 70
    │    │    │    │    │    │    │    │    │REG │    │
    │ BZ1│    │    │    │    │    │    │    │    │    │
    │    │    │    │    │    │    │    │    │    │    │
 80 └────┴────┴────┴────┴────┴────┴────┴────┴────┴────┘ 80mm

Legend:
═══════
U1-U7: Integrated Circuits          C1-C31: Capacitors
J1-J10: Connectors                  L1-L2: Inductors  
R1-R15: Resistors                   D1-D4: Diodes
TP1-TP12: Test Points               SW1-SW3: Switches
LED1-LED4: Status LEDs              BZ1: Buzzer
F1: Fuse                            Y1: Crystal
FB1: Ferrite Bead (not shown)       
```

## Layer Stack-up Details

```
Layer 1 (Top): Component Side
┌─────────────────────────────────────────────────────────────────┐
│ • Component placement and local routing                         │
│ • High-speed digital signals                                   │
│ • Crystal oscillator traces (guarded)                          │
│ • Short connections to minimize loop area                      │
│ • Via connections to inner layers                              │
└─────────────────────────────────────────────────────────────────┘

Layer 2 (Ground): Solid Ground Plane  
┌─────────────────────────────────────────────────────────────────┐
│ • Continuous ground plane (minimal splits)                     │
│ • Analog ground section (isolated via ferrite bead)            │
│ • Thermal vias under high-power components                     │
│ • Return path for all signals                                  │
│ • EMI shielding and noise suppression                          │
└─────────────────────────────────────────────────────────────────┘

Layer 3 (Power): Split Power Planes
┌─────────────────────────────────────────────────────────────────┐
│ • +5V Digital (40% of layer)                                   │
│ • +3.3V Logic (20% of layer)                                   │
│ • +5V Analog (20% of layer)                                    │
│ • +3.7V GSM (20% of layer)                                     │
│ • Proper plane separation and clearances                       │
└─────────────────────────────────────────────────────────────────┘

Layer 4 (Bottom): Routing and Components
┌─────────────────────────────────────────────────────────────────┐
│ • Secondary component placement                                 │
│ • Power supply routing                                          │
│ • Low-speed control signals                                    │
│ • Connector footprints                                         │
│ • Return paths for layer 1 signals                            │
└─────────────────────────────────────────────────────────────────┘
```

## Critical Design Rules

### Signal Integrity
- **Impedance Control**: 50Ω ±10% single-ended, 100Ω ±10% differential
- **Via Stitching**: Ground vias every 5mm along plane boundaries  
- **Guard Traces**: Crystal and ADC inputs surrounded by grounded guards
- **Length Matching**: Critical clock signals matched within 0.1mm

### Power Integrity  
- **Decoupling**: 0.1μF within 5mm of each IC power pin
- **Bulk Capacitors**: 470μF near each regulator output
- **Power Plane Clearance**: 0.2mm minimum around via antipads
- **Current Capacity**: 1oz copper = 1A per mm trace width

### Thermal Management
- **Thermal Vias**: 9x 0.3mm vias under each regulator
- **Component Spacing**: 5mm minimum between high-power parts
- **Airflow**: Orientation allows natural convection cooling
- **Heat Spreading**: Large copper areas under power components

## Manufacturing Notes

### Assembly Sequence
1. **Bottom side SMT**: Passive components first
2. **Bottom side reflow**: Lead-free profile 
3. **Flip board**: Top side component placement
4. **Top side SMT**: All remaining SMT components
5. **Top side reflow**: Second reflow cycle
6. **Through-hole**: Hand solder connectors and large parts
7. **Test and QC**: Electrical test and visual inspection

### Special Requirements
- **Lead-free**: ROHS compliant solder and components
- **Conformal coating**: Optional for harsh environments  
- **Test fixtures**: Bed-of-nails for production testing
- **Programming**: In-circuit programming during assembly

This completes the comprehensive PCB design documentation for the iDMS system.