# iDMS Hardware Wiring Reference Card

**Purpose**: Consolidated pin-map, connector pinout, ISP programming, and power-rail verification guide.  
**Cross-Reference**: MCAL/DIO/DIO_config.h, Hardware/Hardware_Integration.md, Hardware/PCB_Schematic.md  
**MCU**: ATmega128 (64-pin TQFP)  
**Last Updated**: 2026-02-21 (Feature: 001-foundation-overhaul, US7)

> **Note on UART Assignment** (firmware-actual, differs from some earlier docs):  
> - **UART0** (PD0 RX / PD1 TX) → **GSM module (SIM800L)** via J5  
> - **UART1** (PE0 RX / PE1 TX) → **LCD display (TopWay)** via J4

---

## 1. Complete ATmega128 Pin Map

| Port | Pin | Function | Direction | SW Config Macro | Connector |
|------|-----|----------|-----------|-----------------|-----------|
| A | PA0 | PT100 temperature (ADC0) | IN | `DIO_PIN_A0_DIR` / ADC ch0 | J2 (via conditioning) |
| A | PA1 | CT current sensor (ADC1) | IN | `DIO_PIN_A1_DIR` / ADC ch1 | J3 (via conditioning) |
| A | PA2 | Reserved ADC | IN | `DIO_PIN_A2_DIR` | — |
| A | PA3 | Reserved ADC | IN | `DIO_PIN_A3_DIR` | — |
| A | PA4 | Battery voltage monitor | IN | `DIO_PIN_A4_DIR` / ADC ch4 | — |
| A | PA5 | Supply voltage monitor | IN | `DIO_PIN_A5_DIR` / ADC ch5 | — |
| A | PA6 | Reserved | IN | `DIO_PIN_A6_DIR` | — |
| A | PA7 | Reserved | IN | `DIO_PIN_A7_DIR` | — |
| B | PB0 | External timer input (T0) | IN | `DIO_PIN_B0_DIR` | — |
| B | PB1 | ISP SCK | OUT | `DIO_PIN_B1_DIR` | J8 pin 5 |
| B | PB2 | ISP MOSI | OUT | `DIO_PIN_B2_DIR` | J8 pin 4 |
| B | PB3 | ISP MISO | IN | `DIO_PIN_B3_DIR` | J8 pin 1 |
| B | PB4 | PWM output (cooling fan) | OUT | `DIO_PIN_B4_DIR` | — |
| B | PB5 | Status LED 1 | OUT | `DIO_PIN_B5_DIR` | — |
| B | PB6 | Status LED 2 | OUT | `DIO_PIN_B6_DIR` | — |
| B | PB7 | Alarm buzzer | OUT | `DIO_PIN_B7_DIR` | — |
| C | PC0 | GSM module RESET control | OUT | `DIO_PIN_C0_DIR` | J5 pin 5 |
| C | PC1 | GSM module STATUS input | IN | `DIO_PIN_C1_DIR` | J5 pin 6 |
| C | PC2 | Relay 1 — alarm output | OUT | `DIO_PIN_C2_DIR` | — |
| C | PC3 | Relay 2 — fault output | OUT | `DIO_PIN_C3_DIR` | — |
| C | PC4 | External interrupt input | IN | `DIO_PIN_C4_DIR` | — |
| C | PC5 | System enable/disable | IN | `DIO_PIN_C5_DIR` | — |
| C | PC6 | Reserved | IN | `DIO_PIN_C6_DIR` | — |
| C | PC7 | Emergency stop (active low) | IN | `DIO_PIN_C7_DIR` | — |
| D | PD0 | **UART0 RX** — GSM receive | IN | `DIO_PIN_D0_DIR` / UART0 RXD | J5 pin 4 |
| D | PD1 | **UART0 TX** — GSM transmit | OUT | `DIO_PIN_D1_DIR` / UART0 TXD | J5 pin 3 |
| D | PD2 | INT0 — emergency stop | IN | `DIO_PIN_D2_DIR` | — |
| D | PD3 | INT1 — user input | IN | `DIO_PIN_D3_DIR` | — |
| D | PD4 | ICP1 — input capture | IN | `DIO_PIN_D4_DIR` | — |
| D | PD5 | XCK0 — external clock | IN | `DIO_PIN_D5_DIR` | — |
| D | PD6 | Reserved | IN | `DIO_PIN_D6_DIR` | — |
| D | PD7 | Reserved | IN | `DIO_PIN_D7_DIR` | — |
| E | PE0 | **UART1 RX** — LCD receive | IN | `DIO_PIN_E0_DIR` / UART1 RXD | J4 pin 4 |
| E | PE1 | **UART1 TX** — LCD transmit | OUT | `DIO_PIN_E1_DIR` / UART1 TXD | J4 pin 3 |
| E | PE2 | XCK1 — external clock | IN | `DIO_PIN_E2_DIR` | — |
| E | PE3 | SD card CS (future) | OUT | `DIO_PIN_E3_DIR` | — |
| E | PE4 | SPI chip select 1 | OUT | `DIO_PIN_E4_DIR` | — |
| E | PE5 | SPI chip select 2 | OUT | `DIO_PIN_E5_DIR` | — |
| E | PE6 | Reserved | IN | `DIO_PIN_E6_DIR` | — |
| E | PE7 | Reserved | IN | `DIO_PIN_E7_DIR` | — |
| F | PF0 | Alternate ADC0 | IN | `DIO_PIN_F0_DIR` | — |
| F | PF1 | Alternate ADC1 | IN | `DIO_PIN_F1_DIR` | — |
| F | PF2 | Alternate ADC2 | IN | `DIO_PIN_F2_DIR` | — |
| F | PF3 | Alternate ADC3 | IN | `DIO_PIN_F3_DIR` | — |
| F | PF4 | JTAG TCK | IN | `DIO_PIN_F4_DIR` | J8 (JTAG) |
| F | PF5 | JTAG TMS | IN | `DIO_PIN_F5_DIR` | J8 (JTAG) |
| F | PF6 | JTAG TDO | OUT | `DIO_PIN_F6_DIR` | J8 (JTAG) |
| F | PF7 | JTAG TDI | IN | `DIO_PIN_F7_DIR` | J8 (JTAG) |
| G | PG0 | Crystal driver output | OUT | `DIO_PIN_G0_DIR` | — |
| G | PG1 | Crystal driver input | IN | `DIO_PIN_G1_DIR` | — |
| G | PG2 | Watchdog reset output | OUT | `DIO_PIN_G2_DIR` | — |
| G | PG3 | User button input | IN | `DIO_PIN_G3_DIR` | — |
| G | PG4 | Status indicator output | OUT | `DIO_PIN_G4_DIR` | — |

---

## 2. Connector Pinouts (J1–J8)

### J1 — Power Supply Input
| Pin | Signal | Notes |
|-----|--------|-------|
| 1 | +12V | Input: 10–15V DC |
| 2 | GND | Power ground |

**Connector type**: 2-pin screw terminal (5.08 mm pitch)

---

### J2 — PT100 Temperature Sensor
| Pin | Signal | Notes |
|-----|--------|-------|
| 1 | RTD+ | PT100 positive terminal |
| 2 | RTD– | PT100 negative terminal |
| 3 | Shield | Cable shield / GND |

**Connector type**: 3-pin screw terminal (3.81 mm pitch)  
**MCU pin**: PA0 (ADC channel 0) after signal conditioning

---

### J3 — Current Transformer (CT)
| Pin | Signal | Notes |
|-----|--------|-------|
| 1 | CT S1 | CT secondary winding terminal 1 |
| 2 | CT S2 | CT secondary winding terminal 2 |

**Connector type**: 2-pin screw terminal (5.08 mm pitch)  
**MCU pin**: PA1 (ADC channel 1) after burden resistor + conditioning

---

### J4 — TopWay LCD Display (**UART1**)
| Pin | Signal | MCU Pin | Notes |
|-----|--------|---------|-------|
| 1 | +5V | — | LCD power supply |
| 2 | GND | — | Ground |
| 3 | TX (LCD) | PE1 (UART1 TXD) | Data from MCU to LCD |
| 4 | RX (LCD) | PE0 (UART1 RXD) | Data from LCD to MCU |

**Connector type**: 4-pin header (2.54 mm pitch)  
**Protocol**: TopWay HMI serial protocol, 9600 bps, 8N1

---

### J5 — SIM800L GSM Module (**UART0**)
| Pin | Signal | MCU Pin | Notes |
|-----|--------|---------|-------|
| 1 | +3.7V | — | GSM module power |
| 2 | GND | — | Ground |
| 3 | TX (GSM) | PD1 (UART0 TXD) | AT commands from MCU |
| 4 | RX (GSM) | PD0 (UART0 RXD) | Responses from GSM |
| 5 | RESET | PC0 | Active-high reset control |
| 6 | STATUS | PC1 | Module status monitor (input) |

**Connector type**: 6-pin socket header (2.54 mm pitch)  
**Protocol**: AT commands, 9600 bps, 8N1

---

### J6 — GSM Antenna
| Pin | Signal | Notes |
|-----|--------|-------|
| Centre | RF | Antenna signal |
| Shield | GND | RF ground |

**Connector type**: SMA female, 50 Ω impedance  
**Antenna**: GSM/GPRS quad-band (850/900/1800/1900 MHz), 2–5 dBi gain

---

### J7 — SIM Card Socket
Micro SIM socket wired directly to SIM800L.  
ESD protection diodes and gold-plated contacts.  
Insert SIM with contacts facing down.

---

### J8 — ISP Programmer (2×3 Header)
```
Pin Layout (standard AVR 6-pin ISP):

  ┌───┬───┐
  │ 1 │ 2 │    1: MISO (PB3)   2: VCC (+5V)
  ├───┼───┤
  │ 3 │ 4 │    3: SCK  (PB1)   4: MOSI (PB2)
  ├───┼───┤
  │ 5 │ 6 │    5: RST  (/RESET) 6: GND
  └───┴───┘
```

**Connector type**: 2×3 pin header (2.54 mm pitch)  
**Compatible programmers**: USBasp, AVRISPmkII, AVRISP

---

## 3. ISP Programming Procedure

### Hardware Connection
1. Connect USBasp programmer to J8 per the pinout above.
2. Ensure the board is powered (J1 connected, 12V applied) **or** programmer provides VCC via J8 pin 2.
3. Verify MCU RESET pin (J8 pin 5) is not held low by other circuits.

### Flashing the Firmware
```bash
# Build the firmware
make clean && make all

# Flash to ATmega128
avrdude -c usbasp -p m128 -U flash:w:build/main.hex:i

# Verify flash (optional)
avrdude -c usbasp -p m128 -U flash:v:build/main.hex:i
```

### Fuse Bits (ATmega128, 16 MHz crystal)
```bash
# Set fuses for 16 MHz external crystal, JTAG disabled
avrdude -c usbasp -p m128 -U hfuse:w:0xD9:m -U lfuse:w:0xFF:m
```

**Warning**: Do not set JTAG disable fuse unless JTAG is not needed. Incorrect fuse bits can brick the MCU.

---

## 4. Power Rail Verification

### Power Rails Summary
| Rail | Nominal | Acceptable Range | Regulator | Measurement Point | No-Go Action |
|------|---------|-----------------|-----------|-------------------|--------------|
| +12V input | 12V | 10–15V | External PSU | J1 pin 1 to GND | Check PSU / input cable |
| +5V digital | 5V | 4.75–5.25V | U2 (LM7805) | TP1 to GND | Check U2, input voltage |
| +3.3V logic | 3.3V | 3.2–3.4V | U3 (AMS1117-3.3) | TP2 to GND | Check U3, check +5V |
| +5V analog | 5V | 4.90–5.10V | U4 + L2 (LC filter) | TP3 to GND | Check U4, inductor L2 |
| +3.7V GSM | 3.7V | 3.6–3.8V | U7 (LM2596-ADJ) | TP5 to GND | Check U7, adjust feedback resistor |

### 5. Power-On Go/No-Go Verification Sequence

Perform in order. **Stop at first NO-GO** and resolve before continuing.

| Step | Action | Measurement | GO condition | NO-GO action |
|------|--------|-------------|--------------|--------------|
| 1 | Visual inspect PCB for shorts, solder bridges, polarity markings | — | No shorts visible | Rework as needed |
| 2 | Apply 12V to J1, verify input | Multimeter at J1 pin 1 to GND | 10–15V | Check PSU, cable |
| 3 | Measure +5V digital rail | Multimeter at TP1 to GND | 4.75–5.25V | Stop; check U2 |
| 4 | Measure +3.3V logic rail | Multimeter at TP2 to GND | 3.2–3.4V | Stop; check U3 |
| 5 | Measure +5V analog rail | Multimeter at TP3 to GND | 4.90–5.10V | Stop; check U4/L2 |
| 6 | Measure +3.7V GSM rail | Multimeter at TP5 to GND | 3.6–3.8V | Stop; check U7 |
| 7 | Measure idle current | Clamp meter on +12V supply cable | < 200 mA | Disconnect modules, re-test bare board |

**Only after all 7 steps pass**: connect ISP programmer, sensors (J2, J3), LCD (J4), and GSM module (J5).

---

## 6. Bench-Test Checklist (Full Signal Chain)

Use this checklist after a successful power-on sequence and ISP flash to verify the complete signal chain produces live sensor readings on the LCD.

- [ ] **Power rails**: All 5 rails verified GO per Section 4 above.
- [ ] **ISP flash**: `make all` succeeds; `avrdude` flash command completes without errors.
- [ ] **DIO init**: `MDIO_vInt()` is called in `main()` — verify by checking DDRA=0x00 (Port A all input) and DDRB=0xF6 (outputs for LEDs/buzzer, inputs for ISP/T0).
- [ ] **LCD communication (UART1/J4)**: Power on LCD, observe splash screen within 2 seconds of MCU reset.
- [ ] **PT100 reading (J2)**: Connect known-resistance dummy load (e.g., 110 Ω ≈ 25°C room temp); verify LCD shows temperature in expected range (20–30°C acceptable for room-temperature dummy).
- [ ] **CT reading (J3)**: Connect CT around known AC load; verify LCD shows RMS current within 5% of clamp-meter reading.
- [ ] **GSM registration (UART0/J5)**: Insert SIM, power on GSM module; observe STATUS pin (PC1) go high within 30–60 seconds indicating network registration.
- [ ] **SMS alarm test**: Trigger an alarm condition (overheat / overcurrent threshold); verify SMS received on the enrolled phone number within 60 seconds.
- [ ] **EEPROM persistence**: Run phone list add/delete cycle; power-cycle the board; verify the phone list survives the power cycle.
- [ ] **Watchdog**: Verify watchdog LED / relay deactivates within 5 s if the main loop stops kicking the watchdog (test by temporarily disabling watchdog kick call).
