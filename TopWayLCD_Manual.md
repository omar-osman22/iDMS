URL: [http://www.topwaydisplay.com](http://www.topwaydisplay.com)
Document Name: HKT070DTA-1C-Manual-Rev0.2.doc

# HKT070DTA-1C

## LCD Module User Manual

#### Prepared by:

#### HeHongLiang

#### Checked by: Approved by:

#### Date: 2017-10-25 Date: Date:

#### Rev. Descriptions Release Date

0.1 - Preliminary Draft release 2017-02-
0.2 - Assemble Precaution 2017-10-


## TOPWAY LCD Module User Manual HKT070DTA-1C

URL: [http://www.topwaydisplay.com](http://www.topwaydisplay.com)
Document Name: HKT070DTA-1C-Manual-Rev0.2.doc


## TOPWAY LCD Module User Manual HKT070DTA-1C

URL: [http://www.topwaydisplay.com](http://www.topwaydisplay.com)
Document Name: HKT070DTA-1C-Manual-Rev0.2.doc

### 1 Basic Specification

#### TOPWAY HKT070DTA-1C is a Smart TFT Module with 32bit MCU on board. Its graphics engine

#### provides numbers of outstanding features. It supports TOPWAY TML 3.0 for preload and pre-design

#### display interface that simplify the host operation and development time. Suitable for industry control,

#### instrumentation, medical electronics, power electric equipment applications.

### 1.1 General Specification

```
Note:
*1. For saturated color display content (eg. pure-red, pure-green, pure-blue, or pure-colors-combinations).
*2. For “color scales” display content.
*3. Color tone may slightly change by Temperature and Driving Condition.
```
### 1.2 Block Diagram

#### Screen Size(Diagonal) : 7”

#### Resolution : 800(RGB) x 480

#### Color Depth : 65k color (16bit)

#### Pixel Configuration : RGB Stripe

#### Display Mode : Transmissive / Normal White

#### Viewing Direction :

#### 6H(*1)(gray-scale inverse)

#### 12H(*2)

#### Outline Dimension : 200 x 125.0 x 30 (max)(mm)

#### (see attached drawing for details)

#### Active Area : 154.08 x 85.92(mm)

#### Backlight : LED

#### Command I/F: RS-232C

#### Project Download: by PC or by U-Drive (with OTG cable)

#### Operating Temperature : -20 ~ +70°C

#### Storage Temperature : -30 ~ +80°C

```
VDD, VSS
```
```
RTS(BUSY)
TX, RX
```
```
ID, D-, D+
```
```
K
```
```
K
```
```
VUSB, VSS
```
#### 7" TFT

#### 800 x 480 pixels

#### Interface Circuit

#### &

#### Power Circuit

#### Display Function Controller

#### with RTC

#### Touch Panel

#### Flash

#### Memory

#### BUZZER

#### RAM


## TOPWAY LCD Module User Manual HKT070DTA-1C

URL: [http://www.topwaydisplay.com](http://www.topwaydisplay.com)
Document Name: HKT070DTA-1C-Manual-Rev0.2.doc

### 1.3 Terminal Function

#### RS232 Interface Terminal (K1)

#### Pin No. Pin Name I/O Descriptions

```
1 VSS P Ground, (0V)
2 NC -- No connection, leave open
3 RTS(BUSY) O Request To Send (could function as busy BUSY signal)
(eg. to PC’s RS232C pin8 <9pin D-connector> )
4 TX O Data output
(eg. to PC’s RS232C pin2 <9pin D-connector>)
5 RX I Data Input
(eg. to PC’s RS232C pin3 <9pin D-connector>)
6 VDD P Power supply (11.0 ~ 26.0)
```
#### Note.

#### *1. User data and commands transfer through this terminal.

#### *2. HOST using command hand shake during communication is suggested.

#### USB Interface Terminal (K2)

#### Pin No. Pin Name I/O Descriptions

```
1 VUSB P Power supply (5V)
2 D- I/O USB DATA negative signal
3 D+ I/O USB DATA positive signal
4 ID I USB_ID, 1:Client , 0:HOST
5 VSS P Ground, (0V)
```
#### Note.

#### *1. TML files and image files preload through this terminal

#### *2. Standard “USB-drive” functions provided

#### *3. During the files transfer, all others display functions will be suspended

### 2 Absolute Maximum Ratings

#### Items Symbol Min. Max. Unit Condition

#### Power Supply voltage Vdd -0.3 26.0 V

#### Operating Temperature TOP -20 70 C No Condensation

#### Storage Temperature TST -30 80 C No Condensation

Note:
*1. This rating applies to all parts of the module and should not be exceeded.
*2. The operating temperature only guarantees operation of the circuit. The contrast, response speed,
and the other specification related to electro-optical display quality is determined at the room temperature, TOP=25°C
*3. Ambient temperature when the backlight is lit (reference value)
*4. Any Stresses exceeding the Absolute Maximum Ratings may cause substantial damage to the device. Functional
operation of this device at other conditions beyond those listed in the specification is not implied and prolonged
exposure to extreme conditions may affect device reliability.


## TOPWAY LCD Module User Manual HKT070DTA-1C

URL: [http://www.topwaydisplay.com](http://www.topwaydisplay.com)
Document Name: HKT070DTA-1C-Manual-Rev0.2.doc

### 3 Electrical Characteristics

### 3.1 DC Characteristics

#### VSS=0V, VDD=12V, TOP =25C

#### Items Symbol MIN. TYP. MAX. Unit Applicable Pin

#### Operating Voltage VDD 11.0 12.0 26.0 V VDD

#### Rx Input MARK(1) VRxM -3.0 - -15.0 V Rx

#### Rx Input SPACE(0) VRXS +3.0 - +15.0 V Rx

#### Tx Output MARK(1) VTXM -3.0 - -15.0 V Tx

#### Tx Output SPACE(0) VTXS +3.0 - +15.0 V Tx

#### RTS Output High VTXH -3.0 - -15.0 V RTS(BUSY)

#### RTS Output Low VTXL +3.0 - +15.0 V RTS(BUSY)

#### Operating Current IDD - 315 - mA VDD (*1)

#### Operating Current (USB) IVUSB - 200 - mA VUSB

#### Battery Supply current IBAT - 6 - uA (*2)

#### Note.

#### *1. Normal display condition.

#### *2. For Battery driving RTC application, RTC configuration should be enabled in the project global setting.

#### And after install/re-install the battery, it should be power-on once for correct RTC configurations.

### 3.2 AC Characteristics

#### Items JP1,JP7=close,

```
JP2,JP8=open
```
#### (factory default)

```
JP1,JP8=close,
JP2,JP7=open
```
```
JP1,JP8= open,
JP2,JP7= close
```
```
JP1,JP7= open,
JP2,JP8= close
```
```
Start bit 1 1 1 1
Data bit 8 8 8 8
Parity bit None None Even Odd
Stop bit 1 1 1 1
Baud Rate(*1) 115200 bps 9600 bps 115200 bps 115200 bps
```
#### Items JP3=close, JP4=open

#### (factory default)

```
JP3= open, JP4=close
```
```
Serial Data
Flow Control
```
```
busy Xon/Xoff
```
#### Note.

#### *1.Baud Rate (1200bps～115200bps) could be adjusted by software.


## TOPWAY LCD Module User Manual HKT070DTA-1C

URL: [http://www.topwaydisplay.com](http://www.topwaydisplay.com)
Document Name: HKT070DTA-1C-Manual-Rev0.2.doc

### 4 Function Specifications

### 4.1 Basic Operation Function Descriptions

#### - TML files, Picture files, ICON files are stored inside FLASH memory area.

#### They are preloaded to HKT070DTA-1C for stand alone interface use.

#### - Those files are preloaded via USB interface as an USB drive.

#### - All the interface flow and the touch response are based on the preloaded TML files

#### - VP variables memory is inside RAM area,

#### it provides real time access via UART by the HOST or display onto the TFT by TML file.

#### - Custom Memories are inside FLASH memory area

#### It can be accessed via UART interface by the HOST.

#### - Control and Draw Engine executes HOST commands and response respectively

#### - It also reports the real time Touch Key number to the HOST

```
HOST
PC
```
```
TFT Display
```
```
Custom
Memories
```
```
TML files
Picture files
ICONS files
```
```
Control and Draw Engine
```
```
USB
interface
```
```
Touch Panel
```
```
UART interface
```
```
HKT070DTA-1C
```
```
VP variables
```

## TOPWAY LCD Module User Manual HKT070DTA-1C

URL: [http://www.topwaydisplay.com](http://www.topwaydisplay.com)
Document Name: HKT070DTA-1C-Manual-Rev0.2.doc

### 4.2 Quick Start Guide

#### 1.

#### Install TOPWAY

Graphics Editor

#### 2.

#### Import pictures

design UI flow

#### 3.

#### Download to

Smart LCD

#### 4.

#### power on &

#### display

#### 5.

#### Connect to

#### host Show

#### real time

#### data

### 4.3 Command Descriptions

#### Please refer to “SMART LCD Command Manual”.


## TOPWAY LCD Module User Manual HKT070DTA-1C

URL: [http://www.topwaydisplay.com](http://www.topwaydisplay.com)
Document Name: HKT070DTA-1C-Manual-Rev0.2.doc

```
Item Symbol Condition Min Typ Max Unit Remark
```
```
θT 70 80 -
```
```
θB 50 60 -
```
```
θL 70 80 -
```
```
View Angles
```
```
θR
```
##### CR≧ 10

##### 70 80 -

```
Degree Note2,
```
```
Contrast Ratio CR θ=0°^700 900 -^ Note 3
```
```
TON
Response Time
TOFF
```
```
25 °C - 20 30 ms Note 4
```
```
x 0.255 0.305 0.
White
y 0.277 0.327 0.
```
```
Note 1,
```
```
x 0.534 0.584 0.
Red
y 0.300 0.350 0.
```
```
Note 1,
```
```
x 0.290 0.340 0.
Green
y 0.543 0.593 0.
```
```
Note 1,
```
```
x 0.102 0.152 0.
```
```
Chromaticity
```
```
Blue
y
```
```
Backlight is
```
```
on
```
##### 0.040 0.090 0.

```
Note 1,
```
```
Uniformity U 75 80 - % Note 6
```
```
NTSC 45 50 - % Note 5
```
```
Luminance L - 500 - cd/ ㎡ Note 7
```
### 5 Optical Characteristics

#### 1.IF= 40 mA, and the ambient temperature is 25 .°C

#### 2. The test systems refer to Note 1 and Note 2.


## TOPWAY LCD Module User Manual HKT070DTA-1C

URL: [http://www.topwaydisplay.com](http://www.topwaydisplay.com)
Document Name: HKT070DTA-1C-Manual-Rev0.2.doc

Note 1:
The data are measured after LEDs are turned on for 5 minutes.
LCM displays full white. The brightness is the average value of 9
measured spots. Measurement equipment SR-3A (1°)
Measuring condition:

- Measuring surroundings: Dark room
- Measuring temperature: Ta=25°C.
- Adjust operating voltage to get optimum contrast at
    the center of the display.

```
Note 2:
The definition of viewing angle:
Refer to the graph below marked by θ and Ф
```
Note 3:
The definition of contrast ratio (Test LCM using SR-3A (1°)):
Contrast Luminance When LCD is at “White” state
Ratio(CR) = Luminance When LCD is at “Black” state
(Contrast Ratio is measured in optimum common electrode

#### voltage)

```
Note 4:
Definition of Response time. (Test LCD using BM-7A(2°)):
The output signals of photo detector are measured
when the input signals are changed from
“black” to “white”(falling time)
and from “white” to “black”(rising time), respectively.
The response time is defined as
the time interval between the 10% and 90% of amplitudes.Refer to
figure as below.
```
Note 5:
Definition of Color of CIE1931 Coordinate and NTSC Ratio.

Color gamut:

```
S= Area of RGB triangle
Area of NTSC triangle X100%
```
```
Note 6:
The luminance uniformity is calculated by using following formula.
△Bp = Bp (Min.) / Bp (Max.)×100 (%)
Bp (Max.) = Maximum brightness in 9 measured spots
```
#### Bp (Min.) = Minimum brightness in 9 measured spots.

```
Note 7:
```
#### Measured the luminance of white state at center point


## TOPWAY LCD Module User Manual HKT070DTA-1C

URL: [http://www.topwaydisplay.com](http://www.topwaydisplay.com)
Document Name: HKT070DTA-1C-Manual-Rev0.2.doc

### 6 Precautions of using LCD Modules

Mounting

- Mounting must use holes arranged in four corners or four sides.
- The mounting structure so provide even force on to LCD module. Uneven force (ex. Twisted stress)
    should not applied to the module. And the case on which a module is mounted should have sufficient
    strength so that external force is not transmitted directly to the module.
- It is suggested to attach a transparent protective plate to the surface in order to protect the polarizer. It
    should have sufficient strength in order to the resist external force.
- The housing should adopt radiation structure to satisfy the temperature specification.
- Acetic acid type and chlorine type materials for the cover case are not desirable because the former
    generates corrosive gas of attacking the polarizer at high temperature and the latter causes circuit
    break by electro-chemical reaction.
- Do not touch, push or rub the exposed polarizers with glass, tweezers or anything harder than HB
    pencil lead. Never rub with dust clothes with chemical treatment. Do not touch the surface of polarizer
    for bare hand or greasy cloth.(Some cosmetics deteriorate the polarizer.)
- When the surface becomes dusty, please wipe gently with absorbent cotton or other soft materials like
    chamois soaks with petroleum benzine. Normal-hexane is recommended for cleaning the adhesives
    used to attach front / rear polarizers. Do not use acetone, toluene and alcohol because they cause
    chemical damage to the polarizer.
- Wipe off saliva or water drops as soon as possible. Their long time contact with polarizer
Operating
- The spike noise causes the mis-operation of circuits. It should be within the ±200mV level (Over and
under shoot voltage)
- Response time depends on the temperature.(In lower temperature, it becomes longer.)
- Brightness depends on the temperature. (In lower temperature, it becomes lower.) And in lower
temperature, response time(required time that brightness is stable after turned on) becomes longer.
- Be careful for condensation at sudden temperature change. Condensation makes damage to polarizer
or electrical contacted parts. And after fading condensation, smear or spot will occur.
- When fixed patterns are displayed for a long time, remnant image is likely to occur.
- Module has high frequency circuits. Sufficient suppression to the electromagnetic interference shall be
done by system manufacturers. Grounding and shielding methods may be important to minimized the
interference
Electrostatic Discharge Control
Since a module is composed of electronic circuits, it is not strong to electrostatic discharge. Make certain
that treatment persons are connected to ground through wrist band etc. And don’t touch interface pin
directly.
Strong Light Exposure
Strong light exposure causes degradation of polarizer and color filter.
Storage
When storing modules as spares for a long time, the following precautions are necessary.
- Store them in a dark place. Do not expose the module to sunlight or fluorescent light. Keep the
temperature between 5°C and 35°C at normal humidity.
- The polarizer surface should not come in contact with any other object. It is recommended that they
be stored in the container in which they were shipped.
Protection Film
- When the protection film is peeled off, static electricity is generated between the film and polarizer.
This should be peeled off slowly and carefully by people who are electrically grounded and with well
ion-blown equipment or in such a condition, etc.
- The protection film is attached to the polarizer with a small amount of glue. If some stress is applied to
rub the protection film against the polarizer during the time you peel off the film, the glue is apt tore
main on the polarizer. Please carefully peel off the protection film without rubbing it against the
polarizer.
- When the module with protection film attached is stored for a long time, sometimes there remains a
very small amount of glue still on the polarizer after the protection film is peeled off.
- You can remove the glue easily. When the glue remains on the polarizer surface or its vestige is
recognized, please wipe them off with absorbent cotton waste or other soft material like chamois
soaked with normal-hexane.
Transportation
The LCD modules should be no falling and violent shocking during transportation, and also should avoid
excessive press, water, damp and sunshine.


## TOPWAY LCD Module User Manual HKT070DTA-1C

URL: [http://www.topwaydisplay.com](http://www.topwaydisplay.com)
Document Name: HKT070DTA-1C-Manual-Rev0.2.doc

### 7 Assemble Precaution

##### 安装注意事项

#### 1. Customer front panel opening and thickness for TOPWAY

#### display module should be fit for its assembling and sealing.

#### - The suggested assemble gap(A) should be about

#### 0.3~0.5mm on each side.

#### - The suggested front panel thickness(B) should be about

#### 1.5~4.0mm.

#### 客户面板开窗及厚度应适合 TOPWAY显示模块的安装及密封.

#### · 建议每边安装间隙 约(A) 为0.3 ~0.5mm.

#### · 建议面板厚度(B) 约为1.5~4.0mm.

#### 2. A silicon sealing ring ships with TOPWAY display module. It

#### should be in place before assembling to the front panel.

#### TOPWAY显示模块上的硅胶密封圈在安装时确保嵌入到位.

#### 3. It should fix the TOPWAY display module into the front panel

#### with two steps.

#### - Pre-fixing: Slightly tighten the screws on beam clamp in

#### sequence as picture on the right side.

#### - Final-fixing: Tighten the fixing screws on beam clamp in

#### sequence as well with twist torque about 6~8kg.cm (*1).

#### and put the beam clamp straight.

```
Note:
*1. Over tightening might damage the shell and cause bad sealing result.
```
#### 应分两步将TOPWAY显示模块固定在面板上.

#### · 预紧: 将卡扣螺钉按右图所示顺序稍加预紧.

· 紧定再次按顺序用: 6~8kg.cm扭力拧紧卡扣螺钉(*1),并注

#### 意卡扣置正无歪斜.

```
注:
*1. 过度拧紧可能会损坏外壳和影响密封效果.
```
#### 4. It is strongly suggested to check the seal balancing of the

#### four-side of the TOPWAY display module.

#### - The suggested after assemble sealed gap(C) should be

#### about 1.0~1.5mm.

#### 需注意检查TOPWAY显示模块四周在安装后保证平衡密封.

#### · 建议组装后的密封间隙(C)约为1.0 ~1.5 mm.

#### 5. Others:

#### - Never hot plug the device! Power off the device before connect or disconnect the display

#### module.

#### - Don't forget to remove the cover protective film for normal operation.

#### 其它:

#### · 视频线禁止带电插拔！ 在连接或断开显示模块之前先关闭设备电源.

#### · 使用前请揭去保护膜.


URL: [http://www.topwaydisplay.com](http://www.topwaydisplay.com)
Document Name:SMART LCD Command v5.04 Manual Rev0.4.doc

# SMART LCD

## Command V5.

## Manual

#### Prepared by:

#### liu

#### Checked by: Approved by:

#### Date: 2017-07-24 Date: Date:

#### Rev. Descriptions Release Date

0.1 - Preliminary Draft release 2016-11-
0.2 - Add a description of the 0xE8 command 2017-01-

0.3 - 4.1, 0xE2, 0xE3 command name correction 2017-05-
0.4 - Add a description of the 0xE7 command 2017-07-


## URL: http://www.topwaydisplay.com

Document Name:SMART LCD Command v5.04 Manual Rev0.4.doc

- 1  Basic Specification..................................................................................................................................... Table of Content
   - 1.1 General Specification............................................................................................................................
   - 1.2 Block Diagram ......................................................................................................................................
   - 1.3 Terminal Function .................................................................................................................................
- 2  Absolute Maximum Ratings ......................................................................................................................
- 3  Electrical Characteristics...........................................................................................................................
   - 3.1 DC Characteristics ................................................................................................................................
   - 3.2 AC Characteristics ................................................................................................................................
- 4  Function Specifications .............................................................................................................................
   - 4.1 Basic Operation Function Descriptions ................................................................................................
   - 4.2 Quick Start Guide .................................................................................................................................
   - 4.3 Command Descriptions ........................................................................................................................
- 5  Optical Characteristics...............................................................................................................................
- 6  Precautions of using LCD Modules ........................................................................................................
- 7 Assemble Precaution .................................................................................................................................
- TOPWAY SMART LCD Command Manual Protocol V5.
- 1  Basic Specifications Table of Content
   - 1.1 Hardware connection
- 2  Command Structure
   - 2.1 Communication Packet Structure
   - 2.2 Packet Acknowledgment
- 3  Data arrangement
   - 3.1 Color Data Value Configuration
   - 3.2 Data / Address / Page_ID / Location Values Configuration
- 4  Command Descriptions
   - 4.1 Command table
   - 4.2 Config/ Status Function Commands Details
      - 4.2.1 hand_shake ( 0x30 )
      - 4.2.2 read_version ( 0x31 )
      - 4.2.3 read_pg_id ( 0x32 )
      - 4.2.4 touch_response ( 0x72/ 0x73/ 0x77/ 0x78/ 0x79 )
      - 4.2.5 set_sys_config ( 0xE0 )
      - 4.2.6 sel_project ( 0xE1 )
      - 4.2.7 touch_calib ( 0xE4 )
      - 4.2.8 screen_saver (0x5E)
      - 4.2.9 backlight_ctrl ( 0x5F )
      - 4.2.10 buzzer_touch_sound ( 0x79 )
      - 4.2.11 buzzer_ ctrl ( 0x7A )
      - 4.2.12 Flash_write ( 0x90 )
      - 4.2.13 Flash_read ( 0x91 )
      - 4.2.14 RTC_read ( 0x9B )
      - 4.2.15 RTC_set ( 0x9C )
      - 4.2.16 USR_bin_read ( 0x93 )
      - 4.2.17 U_drv_format ( 0xE2 )
      - 4.2.18 U_drv_unlock ( 0xE3 )
      - 4.3.1 disp_page ( 0x70 ) 4.3 Display Control Function Commands Details 12 
      - 4.3.2 set_element_fg ( 0x7E )
      - 4.3.3 set_element_bg ( 0x7F )
      - 4.3.4 Set_codepage (0xE7)
      - 4.3.5 suspend_vp_refresh (0xE8)
   - 4.4 VP Function Commands Details
      - 4.4.1 Successive_write ( 0x82 )
      - 4.4.2 Successive_read ( 0x83 )
      - 4.4.3 BP1_write ( 0x4B )
      - 4.4.4 BP1_write_compress ( 0x4C )
      - 4.4.5 G16_write ( 0x4D )
      - 4.4.6 G16_write_rotate ( 0x4E )
      - 4.4.7 System Register Write ( 0x3B )
      - 4.4.8 System Register Read ( 0x3C )
      - 4.4.9 STR_write ( 0x42 )
      - 4.4.10 STR_read ( 0x43 )
      - 4.4.11 STR_fill ( 0x46 )
      - 4.4.12 N16_write ( 0x3D )
      - 4.4.13 N16_read ( 0x3E )
      - 4.4.14 N16_fill ( 0x3F )
      - 4.4.15 N32_write ( 0x44 )
      - 4.4.16 N32_read ( 0x45 )
      - 4.4.17 N32_fill ( 0x47 )
      - 4.4.18 N64_write ( 0x48 )
      - 4.4.19 N64_read ( 0x49 )
      - 4.4.20 N64_fill ( 0x4A )


## TOPWAY SMART LCD Command Manual Protocol V5.

URL: [http://www.topwaydisplay.com](http://www.topwaydisplay.com)
Document Name:SMART LCD Command v5.04 Manual Rev0.4.doc

### 1 Basic Specifications

#### TOPWAY Smart LCD serial command is for real-time control and access. Host machine get the data

#### which input through the Smart LCD interface or provide the data for display.

### 1.1 Hardware connection

#### Smart LCD serial UART interface are mainly base on RS232-C standard, by default, config as 8N

#### 115200bps.

### 2 Command Structure

### 2.1 Communication Packet Structure

#### Commands and Response Packet should be format as follow (hostmodule):

```
Seq Code Code type Description
1 0xAA Packet header 1byte
2 Cmd-code Command
code
```
```
1byte
```
```
3 Par-data Parameter or
Data
```
##### (*1)

##### : : - -

##### : : - -

##### : : - -

N-3 th 0xCC Packet tail 4byte
N-2 th 0x
N-1 th 0xC
N th 0x3C
Note.
*1. Unless otherwise specified,
all the multi-byte values, data, address’ byte sequence are MSB first, LSB last.

### 2.2 Packet Acknowledgment

#### Packet Acknowledgment is two byte in ASCII (module  host):

```
Response code Description
Command (in packet) executed and
wait for next Command
```
```
":>" In ASCII
(0x3a, 0x3e)
Command (in packet) error and
wait for next Command
```
```
"!>" In ASCII
```
#### 15 B 15 B 15 B 15 B (0x21,0x3e)

Note.
*1. Packet Acknowledgement response to a valid packet only.


## TOPWAY SMART LCD Command Manual Protocol V5.

URL: [http://www.topwaydisplay.com](http://www.topwaydisplay.com)
Document Name:SMART LCD Command v5.04 Manual Rev0.4.doc

### 3 Data arrangement

### 3.1 Color Data Value Configuration

#### 16 bit Color value

```
16 bit color value
R4 R3 R2 R1 R0 G5 G4 G3 G2 G1 G0 B4 B3 B2 B1 B
High byte (MSB) Low byte (LSB)
D7 D6 D5 D4 D3 D2 D1 D0 D7 D6 D5 D4 D3 D2 D1 D
```
### 3.2 Data / Address / Page_ID / Location Values Configuration

#### 64bit value

```
64 bit number value
D63...D56 D55...D48 D47...D40 D39..D32 D31...D24 D23...D16 D15...D8 D7...D
Byte
(MSB)
```
```
Byte
(LSB)
D7...D0 D7...D0 D7...D0 D7...D0 D7...D0 D7...D0 D7...D0 D7...D
```
#### 32bit value

```
32 bit number value
D31...D24 D23...D16 D15...D8 D7...D
Byte3 (MSB) Byte0 (LSB)
D7...D0 D7...D0 D7...D0 D7...D
```
#### 16bit value

```
16 bit number value
D15...D8 D7...D
High Byte (MSB) Low Byte (LSB)
D7...D0 D7...D
```

## TOPWAY SMART LCD Command Manual Protocol V5.

URL: [http://www.topwaydisplay.com](http://www.topwaydisplay.com)
Document Name:SMART LCD Command v5.04 Manual Rev0.4.doc

### 4 Command Descriptions

### 4.1 Command table

Functions Name Code Description
Config/
Status
Functions

```
hand_shake 0x30 Read a Hand Shake
read_version 0x31 Read firmware version
read_pg_id 0x32 Read Current page ID
touch_response 0x72/0x73/
0x77/0x78/
0x
```
```
see also set_sys_config
```
set_sys_config 0xE0 System parameter configuration and Baud Rate
sel_project 0xE1 Specify operating project folder
touch_calib 0xE4 Touch panel calibration
screen_saver 0x5E Screen saver (backlight dim down time out)
backlight_ctrl 0x5F backlight brightness control (64 levels)
buzzer_touch_sound 0x79 buzzer enable time length (in 10ms step)
buzzer_ctrl 0x7A Buzzer control
Flash_write 0x90 Write data to the flash
Flash_read 0x91 Read data from the flash
RTC_read 0x9B Read the RTC values
RTC_set 0x9C Set the RTC
USR_bin_read 0x93 Read data from the USR_bin
U_drv_format 0xE2 Format the U_drv
U_drv_unlock 0xE3 Unlock the U_drv with pre-stored password
Display
Control
Functions

disp_page 0x70 Display a pre-stored TML file (page)
set_element_fg 0x7E Set the foreground color of STR, N16, N32 or N
set_element_bg 0x7F Set the background color of STR, N16, N32 or N
set_codepage 0xE7 Sets country character set and code-page character set
suspend_vp_fresh 0xE8 Set the screen to pause the refresh and deactivate the
touchkey or release the pause to refresh and enable
the touchkey
VP
Functions

```
Successive_write 0x82 Write successive value to VP_N16, VP_N32, VP_N
Successive_read 0x83 Read successive value from VP_N16, VP_N32, VP_N
BP1_write 0x4B Write bit-map (1bpp) data to VP_BP
BP1_write_comp 0x4C Write compressed bit-map (1bpp) data to VP_BP
G16_write 0x4D Write 16bit (signed integer) graphic array to VP_G
G16_write_rotate 0x4E Rotate the VP_G16 array data inside the module and
write a 16bit (signed integer) value into end-of-array
System Register Write 0x3B Write System Register
System Register Read 0x3C Read System Register
STR_write 0x42 Write string to VP_STR
STR_read 0x43 Read string form VP_STR
STR_fill 0x46 Fill strings to the VP_STR
N16_write 0x3d Write 16bit (signed integer) value to VP_N
N16_read 0x3e Read 16bit (signed integer) value from VP_N
N16_fill 0x3f Fill numbers to the VP_N
N32_write 0x44 Write 32bit (signed integer) value to VP_N
N32_read 0x45 Read 32bit (signed integer) value from VP_N
N32_fill 0x47 Fill numbers to the VP_N
N64_write 0x48 Write 64bit (signed integer) value to VP_N
N64_read 0x49 Read 64bit (signed integer) value from VP_N
N64_fill 0x4A Fill numbers to the VP_N
```

## TOPWAY SMART LCD Command Manual Protocol V5.

URL: [http://www.topwaydisplay.com](http://www.topwaydisplay.com)
Document Name:SMART LCD Command v5.04 Manual Rev0.4.doc

### 4.2 Config/ Status Function Commands Details

#### 4.2.1 hand_shake ( 0x30 )

seq Cmd-code / Par-data Descriptions
1 0x30 Read a Hand Shake
Note.

#### *1. Command should be transferred in communication packet structure (see Communication Packet Structure Section for details)

#### Response code:

```
Seq. Content Byte in Hex Descriptions
1 st Header 0xAA Communication packet header
2 nd Command 0x30 Command executed
3 rd “T” 0x54 “Topway HMT Ready\0” in ASCII
```
```
‘\0’(0x00): string end mark
```
4 th “o” 0x6f
5 th “P” 0x
6 th “w” 0x
7 th “a” 0x
8 th “y” 0x
9 th “ “ 0x
10 th “H” 0x
11 th “M” 0x4d
12 th “T” 0x
13 th “ “ 0x
14 th “R” 0x
15 th “e” 0x
16 th “a” 0x
17 th “d” 0x
18 th “y” 0x
19 th \0 0x
20 th Tail 0xCC Communication packet tail
21 st 0x
22 nd 0xC
23 rd 0x3C
Note.
*1. The Response code with communication packet format (see Communication Packet Structure Section for details)

#### 4.2.2 read_version ( 0x31 )

Seq Cmd-code / Par-data Descriptions
1 0x31 Read firmware version
Note.
*1. Command should be transferred in communication packet structure (see Communication Packet Structure Section for details)

#### Response code:

```
Seq. Content Byte in Hex Descriptions
1 st Header 0xAA Communication packet header
2 nd Command 0x31 Command executed
3 rd “1” 0x31 “1.06\0” in ASCII
Where firmware version is V1.06(example)
```
```
‘\0’(0x00): string end mark
```
4 th “.” 0x2e
5 th “0” 0x
6 th “6” 0x
7 th \0 0x
8 th Tail 0xCC Communication packet tail
9 th 0x
10 th 0xC
11 th 0x3C
Note.
*1. The Response code with communication packet format (see Communication Packet Structure Section for details)


## TOPWAY SMART LCD Command Manual Protocol V5.

URL: [http://www.topwaydisplay.com](http://www.topwaydisplay.com)
Document Name:SMART LCD Command v5.04 Manual Rev0.4.doc

#### 4.2.3 read_pg_id ( 0x32 )

Seq Cmd-code / Par-data Descriptions
1 0x32 Read Current page ID
Note.
*1. Command should be transferred in communication packet structure (see Communication Packet Structure Section for details)

#### Response code:

Seq. Content Byte in Hex Descriptions
1 st Header 0xAA Communication packet header
2 nd Command 0x32 Command executed
3 rd Page ID Page_IDh Current Page ID in 16bit binary value
4 th Page_IDl
5 th Tail 0xCC Communication packet tail
6 th 0x
7 th 0xC
8 th 0x3C
Note.
*1. The Response code with communication packet format (see Communication Packet Structure Section for details)

#### 4.2.4 touch_response ( 0x72/ 0x73/ 0x77/ 0x78/ 0x79 )

```
seq Cmd-code / Par-data Descriptions
1 -- Use set_sys_config to config the functions
```
#### Touch Release Coordinate Response code (0x72):

```
Seq. Content Byte in Hex Descriptions
1 st Header 0xAA Communication packet header
2 nd Command 0x72 Touched release Coordinate
3 rd X coordinate Xh Coordinate in 16bit binary value
X = horizontal coordinate
Y = vertical coordinate
```
4 th Xl
5 th Y coordinate Yh
6 th Yl
7 th Tail 0xCC Communication packet tail
8 th 0x
9 th 0xC
10 th 0x3C
Note.
*1. The Response code with communication packet format (see Communication Packet Structure Section for details)

#### Touch Down Coordinate Response code ( 0x73 ):

```
Seq. Content Byte in Hex Descriptions
1 st Header 0xAA Communication packet header
2 nd Command 0x73 Touched down Coordinate
3 rd X coordinate Xh Coordinate in 16bit binary value
X = horizontal coordinate
Y = vertical coordinate
```
4 th Xl
5 th Y coordinate Yh
6 th Yl
7 th Tail 0xCC Communication packet tail
8 th 0x
9 th 0xC
10 th 0x3C
Note.
*1. The Response code with communication packet format (see Communication Packet Structure Section for details)

#### Touch Key ID Response code ( 0x78 ):

Seq. Content Byte in Hex Descriptions
1 st Header 0xAA Communication packet header
2 nd Command 0x78 Touched release Key_ID defined by TOPWAY TML Graphic
Editor will be response to host
3 rd Page_ID Page_IDh Page_ID = the touch key in page(16bit binary value)
4 th Page_IDl
5 th Y coordinate Key_ID Key_ID (8bit binary value)
6 th Tail 0xCC Communication packet tail
7 th 0x
8 th 0xC
9 th 0x3C
Note.
*1. The Response code with communication packet format (see Communication Packet Structure Section for details)


## TOPWAY SMART LCD Command Manual Protocol V5.

URL: [http://www.topwaydisplay.com](http://www.topwaydisplay.com)
Document Name:SMART LCD Command v5.04 Manual Rev0.4.doc

#### Touch Key ID Response code ( 0x79 ):

Seq. Content Byte in Hex Descriptions
1 st Header 0xAA Communication packet header
2 nd Command 0x79 Touched down Key_ID defined by TOPWAY TML Graphic
Editor will be response to host
3 rd Page_ID Page_IDh Page_ID = the touch key in page(16bit binary value)
4 th Page_IDl
5 th Y coordinate Key_ID Key_ID (8bit binary value)
6 th Tail 0xCC Communication packet tail
7 th 0x
8 th 0xC
9 th 0x3C
Note.
*1. The Response code with communication packet format (see Communication Packet Structure Section for details)

#### Touch Key VP_ADD+VP_Value Response code ( 0x77 ):

```
Seq. Content Byte in Hex Descriptions
1 st Header 0xAA Communication packet header
2 nd Command 0x77 Touch Key VP_ADD+VP_Value Response code
3 rd VP_ADD Addr3（MSB） VP Address
0x080000 ~ 0x08FFFF:VP_N16 Address
0x020000 ~ 0x02FFFF:VP_N32 Address
0x030000 ~ 0x03FFFF:VP_N64 Address
0x000000 ~ 0x01FFFF:VP_STR Address
```
```
4 th Addr
5 th Addr
6 th Addr0(LSB)
```
```
7 th Data : No.of byte
VP_N16: 2byte
VP_N32: 4byte
VP_N64: 8byte
VP_STR: string (with end mark (‘‘\0’(0x00)))
```
: (^) :
: (^) :
: (^) :
: Tail 0xCC Communication packet tail
: 0x
: 0xC
: 0x3C
Note.
*1. The Response code with communication packet format (see Communication Packet Structure Section for details)

#### 4.2.5 set_sys_config ( 0xE0 )

seq Cmd-code / Par-data Descriptions
1 0xE0 Baud Rate and system parameter configuration
2 0x
3 0xAA
4 0x5A
5 0xA
6 Baud_Set Baudrate Set:
0x00 = 1200bps
0x01 = 2400bps
0x02 = 4800bps
0x03 = 9600bps
0x04 = 19200bps
0x05 = 38400bps
0x06 = 57600bps
0x07 = 115200bps
7 sys_par1 Bit7 = 0: Touch panel function disable
Bit7 = 1: Touch panel functions enable (*3)(default)
Bit[1..0]: Touch actions configuration (*2, *3)
8 0x00 Reserved
Note.
*1. Command should be transferred in communication packet structure (see Communication Packet Structure Section for details)
*2. Touch panel configuration:
Sys_par
Bit

```
Sys_par
Bit
```
```
Sys_par
Bit
```
Response
To host Descriptions
0 0 0 Null Not touch panel functions
1 0 1 Coordinates Touch down Coordinate will be response to host
1 1 0 Coordinates Touch release Coordinate will be response to host
1 1 1 Key ID Touch Key_ID defined byTOPWAY TML Graphic Editorwill be
response to host
*3. see set_touch section for response code


## TOPWAY SMART LCD Command Manual Protocol V5.

URL: [http://www.topwaydisplay.com](http://www.topwaydisplay.com)
Document Name:SMART LCD Command v5.04 Manual Rev0.4.doc

#### 4.2.6 sel_project ( 0xE1 )

seq Cmd-code / Par-data Descriptions
1 0xE1 Select project folder
2 Prj_ID 0~9, project ID
0: System execute the default project “THMT”
1~9: System execute the project “THMT01”~“THMT09”
Note.
*1. Command should be transferred in communication packet structure (see Communication Packet Structure Section for details)

#### 4.2.7 touch_calib ( 0xE4 )

seq Cmd-code / Par-data Descriptions
1 0xE4 Touch panel calibration
2 0x
3 0xAA
4 0x5A
5 0xA
Note.
*1. Command should be transferred in communication packet structure (see Communication Packet Structure Section for details)
*2. Keep pressing the touch panel during power on, could also trigger the touch_calib function

#### 4.2.8 screen_saver (0x5E)

seq Cmd-code / Par-data Descriptions
1 0x5E Screen saver
2 Time1h time out time in seconds, range: 0x0000 ~ 0xffff
3 Time1l (0x0000: disable screen saver function) (*2)
4 PWM_LE PWM_LE = 0 ~ 0x3F (default 0x19 in dim down),
the backlight dimmed level in screen saving mode (*2)
Screensavers brightness can not be greater than the backlight
brightness.
Note.
*1. Command should be transferred in communication packet structure (see Communication Packet Structure Section for details)
*2. default value defined by TML graphic editor configuration

#### 4.2.9 backlight_ctrl ( 0x5F )

seq Cmd-code / Par-data Descriptions
1 0x5F backlight brightness control
2 PWM_LE PWM_LE=0x00 ~ 0x3F (*2)
Note.
*1. Command should be transferred in communication packet structure (see Communication Packet Structure Section for details)
*2. default value defined by TML graphic editor configuration

#### 4.2.10 buzzer_touch_sound ( 0x79 )

seq Cmd-code / Par-data Descriptions
1 0x79 buzzer touch sound control
2 Time Sounding time length (in 10ms), range 0x00~0x3F
0x00= disable (*2)
Note.
*1. Command should be transferred in communication packet structure (see Communication Packet Structure Section for details)
*2. default value defined by TML graphic editor configuration

#### 4.2.11 buzzer_ ctrl ( 0x7A )

seq Cmd-code / Par-data Descriptions
1 0x7A Buzzer control
2 Loop count Loop count, Range: 0x01 ~ 0xFF.
0xFF = buzzer infinite loop
3 T1 Buzzer play time 1
Range: 0x00 ~ 0xFF (0~25.5s)(unit 100ms)
4 T2 Buzzer play time 2
Range: 0x00 ~ 0xFF (0~25.5s)(unit 100ms)
5 Freq1 T1 time Buzzer frequency,Unit 100 Hz
Ranges: 0x05 ~ 0x32 (500Hz ~ 5KHz)
0x00 = T1 time period buzzer turn off
6 Freq2 T2 time Buzzer frequency,Unit 100 Hz
Ranges: 0x05 ~ 0x32 (500Hz ~ 5KHz)
0x00 = T1 time period buzzer turn off
Note:

1. The buzzer sound time is (T1 + T2)*100ms


## TOPWAY SMART LCD Command Manual Protocol V5.04

URL: [http://www.topwaydisplay.com](http://www.topwaydisplay.com)
Document Name:SMART LCD Command v5.04 Manual Rev0.4.doc

#### 4.2.12 Flash_write ( 0x90 )

seq Cmd-code / Par-data Descriptions
1 0x90 Write data to the flash at specified address
2 Address3(MSB) the specified start address to write
3 Address2 Address range =0x00000 ~ 0x03FFFF
4 Address1
5 Address0(LSB)
6 Data_Lengthh The no. of data byte to write.
7 Data_Lengthl Length =0x0001 ~ 0x0400
8 Data data to write.
: :^
: :
: :
Note.
*1. Command should be transferred in communication packet structure (see Communication Packet Structure Section for details)

#### 4.2.13 Flash_read ( 0x91 )

seq Cmd-code / Par-data Descriptions
1 0x91 Read data from the flash at specified address
2 Address3(MSB) the specified start address to write
3 Address2 Address range =0x00000 ~ 0x03FFFF
4 Address1
5 Address0(LSB)
6 Data_Lengthh The no. of data byte to read
7 Data_Lengthl Length =0x0001 ~ 0x0400
Note.
*1. Command should be transferred in communication packet structure (see Communication Packet Structure Section for details)

#### Response code:

Seq. Content Byte in Hex Descriptions
1 st Header 0xAA Communication packet header
2 nd Command 0x91 Command executed
3 rd Data data Read back data
: :
: :
: Tail 0xCC Communication packet tail
: 0x33
: 0xC3
: 0x3C
Note.
*1. The Response code with communication packet format (see Communication Packet Structure Section for details)

#### 4.2.14 RTC_read ( 0x9B )

```
seq Cmd-code / Par-data Descriptions
1 0x9B Read the current RTC value
```
#### Response code:

```
Seq. Content Byte in Hex Descriptions
1 st Header 0xAA Communication packet header
2 nd Command 0x9B Command executed
3 rd Date Year Year: 00~99 (00=year 2000) (8bit binary value)
Month: 01~12 (8bit binary value)
Day: 01~31 (8bit binary value)
```
```
4 th Month
5 th Day
6 th Time Hour Hour 00~23 (24hr format)(8bit binary value)
Minutes 00~59 (8bit binary value)
Second 00~59 (8bit binary value)
```
7 th Minute
8 th Second
9 th Tail 0xCC Communication packet tail
10 th 0x33
11 th 0xC3
12 th 0x3C
Note.
*1. The Response code with communication packet format (see Communication Packet Structure Section for details)


## TOPWAY SMART LCD Command Manual Protocol V5.04

URL: [http://www.topwaydisplay.com](http://www.topwaydisplay.com)
Document Name:SMART LCD Command v5.04 Manual Rev0.4.doc

#### 4.2.15 RTC_set ( 0x9C )

```
seq Cmd-code / Par-data Descriptions
1 0x9C Set the RTC
2 Year Year = 00~99(2000 ~ 2099)
Month = 00~12
Date = 00~31
Hour = 00~23
Minute = 00~59
Second = 00~59
```
3 Month
4 Date
5 Hour
6 Minute
7 Second
Note.
*1. Command should be transferred in communication packet structure (see Communication Packet Structure Section for details)

#### 4.2.16 USR_bin_read ( 0x93 )

seq Cmd-code / Par-data Descriptions
1 0x93 Read USR_bin data from the flash at specified address
2 Address3(MSB) the specified start address to write
3 Address2 Address range = 0x00000 ~ 0x03FFFF
4 Address1
5 Address0(LSB)
6 Data_Lengthh The no. of data byte to read
7 Data_Lengthl Length = 0x0001 ~ 0x0400
Note.
*1. Command should be transferred in communication packet structure (see Communication Packet Structure Section for details)

#### Response code:

Seq. Content Byte in Hex Descriptions
1 st Header 0xAA Communication packet header
2 nd Command 0x93 Command executed
3 rd Data data Read back data
: :
: :
: Tail 0xCC Communication packet tail
: 0x33
: 0xC3
: 0x3C
Note.
*1. The Response code with communication packet format (see Communication Packet Structure Section for details)

#### 4.2.17 U_drv_format ( 0xE2 )

seq Cmd-code / Par-data Descriptions
1 0xE2 Format the USB drive.
2 0x55 All the files (include the security lock file) will be erased.
3 0xAA
4 0x5A
5 0xA5
Note.
*1. Command should be transferred in communication packet structure (see Communication Packet Structure Section for details)

#### 4.2.18 U_drv_unlock ( 0xE3 )

```
seq Cmd-code / Par-data Descriptions
1 0xE3 Unlock the USB drive of file read/write with pre-stored password.
2 PW PW: password in ASCII
Length = 127max.
```
```
‘\0’(0x00): string end mark
```
##### : :

##### : :

##### : ‘\0’

Note.
*1. Command should be transferred in communication packet structure (see Communication Packet Structure Section for details)


## TOPWAY SMART LCD Command Manual Protocol V5.04

URL: [http://www.topwaydisplay.com](http://www.topwaydisplay.com)
Document Name:SMART LCD Command v5.04 Manual Rev0.4.doc

### 4.3 Display Control Function Commands Details

#### 4.3.1 disp_page ( 0x70 )

seq Cmd-code / Par-data Descriptions
1 0x70 Display a pre-stored TML file(page)
2 Page_IDh Page_ID = 0~999
3 Page_IDl
Note.
*1. Command should be transferred in communication packet structure (see Communication Packet Structure Section for details)

#### 4.3.2 set_element_fg ( 0x7E )

Seq Cmd-code / Par-data Descriptions
1 0x7E Set foreground colors of the STR, N16, N32 or N64
2 Element 0x00 = STR; 0x01 = N16, N32, N64
3 Page_IDh Page_ID = 0~999
4 Page_IDl
5 Element_ID VP_STR = 0~127; N16, N32, N64 = 0~119
6 0x00 Reserve
7 FGh Foreground color(0~0xffff)
8 FGl
Note.
*1. Command should be transferred in communication packet structure (see Communication Packet Structure Section for details)

#### 4.3.3 set_element_bg ( 0x7F )

Seq Cmd-code / Par-data Descriptions
1 0x7F Set background color of the STR, N16, N32 or N64
2 Element 0x00 = STR; 0x01 = N16, N32, N64
3 Page_IDh Page_ID = 0~999
4 Page_IDl
5 Element_ID VP_STR = 0~127, N16, N32, N64 = 0~119
6 Mode 0x00:non transparent; 0x01 : transparent
7 BGh Background color(0 ~ 0xffff)
8 BGl
Note.
*1. Command should be transferred in communication packet structure (see Communication Packet Structure Section for details)

#### 4.3.4 Set_codepage (0xE7)

Seq Cmd-code / Par-data Descriptions
1 0xE7 Sets country character set and code-page character set
2 Country 1 ~ 11, country character set
3 Codepage 1 ~ 22, code-page character set
Note.
*1. Country and CodePage table
Country Code Descriptions Code Page Descriptions
1 USA 1 437 (OEM –United States)
2 France 2 737 (OEM –Greek 437G)
3 Germany 3 852 (OEM –Latin II)
4 United Kingdom 4 860 (OEM –Portuguese)
5 Demark I 5 863 (OEM –Canadian French)
6 Demark II 6 865 (OEM –Nordic)
7 Sweden 7 866 (OEM –Russian)
8 Italy 8 874 (ANSI/OEM – Thai)
9 Spain 9 932 (ANSI/OEM – Japanese Shift-JIS)
10 Japan 10 1250 (ANSI - Central Europe)
11 Norway 11 1251 (ANSI –Cyrillic)
-- -- 12 1252 (ANSI –Latin I)
-- -- 13 1253 (ANSI –Greek)
-- -- 14 1254 (ANSI –Turkish)
-- -- 15 1255 (ANSI –Hebrew)
-- -- 16 1256 (ANSI –Arabic)
-- -- 17 1257 (ANSI –Baltic)
-- -- 18 1258 (ANSI –Viet Nam)
-- -- 19 GB2312
-- -- 20 GBK
-- -- 21 EUC_KR
-- -- 22 Big 5


## TOPWAY SMART LCD Command Manual Protocol V5.04

URL: [http://www.topwaydisplay.com](http://www.topwaydisplay.com)
Document Name:SMART LCD Command v5.04 Manual Rev0.4.doc

#### 4.3.5 suspend_vp_refresh (0xE8)

```
Seq Cmd-code / Par-data Descriptions
1 0xE8 Set the screen to pause the refresh and deactivate the touchkey or
2 55 release the pause to refresh and enable the touchkey
3 AA
4 5A
5 A 5
```
#### 6 Mode 0x00:release the pause to refresh and enable the touchkey

0x01: pause the refresh and deactivate the touchkey
Note.
*1. Command should be transferred in communication packet structure (see Communication Packet Structure Section for details)

### 4.4 VP Function Commands Details

#### 4.4.1 Successive_write ( 0x82 )

```
Seq Cmd-code / Par-data Descriptions
1 0x82 Write successive value to VP_N16, VP_N32, VP_N64
2 Addr3(MSB) VP_N16Address = 0x080000 ~ 0x08FFFF (should be aligned every 2
byte)
VP_N32 Address = 0x020000 ~ 0x02FFFF (should be aligned every 4
byte)
VP_N64 Address = 0x030000 ~ 0x03FFFF (should be aligned every 8
byte)
```
```
3 Addr2
4 Addr1
5 Addr0(LSB)
```
```
6 Length The number of data to write (Length = 1~255)
7 Data 1(MSB) the value to write
No. of byte of Data:
VP_N16 = Length *2,
VP_N32 = Length *4,
VP_N64 = Length *8,
```
```
8 Data 2
9 Data 3
:
:
```
##### :

```
Data n(LSB)
```
Note.
*1. Command should be transferred in communication packet structure (see Communication Packet Structure Section for details)

#### 4.4.2 Successive_read ( 0x83 )

```
Seq Cmd-code / Par-data Descriptions
1 0x83 Read successive value to VP_N16, VP_N32, VP_N64
2 Addr3(MSB) VP_N16Address = 0x080000 ~ 0x08FFFF (should be aligned every 2
byte)
VP_N32 Address = 0x020000 ~ 0x02FFFF (should be aligned every 4
byte)
VP_N64 Address = 0x030000 ~ 0x03FFFF (should be aligned every 8
byte)
```
```
3 Addr2
4 Addr1
5 Addr0(LSB)
```
6 Length The number of data to write (Length = 1 ~ 255)
Note.
*1. Command should be transferred in communication packet structure (see Communication Packet Structure Section for details)

#### Response code:

```
Seq. Content Byte in Hex Descriptions
1 st Header 0xAA Communication packet header
2 nd Command 0x83 Command executed
3 rd VP_ADD Addr3(MSB） VP Address
0x080000 ~ 0x08FFFF:VP_N16 Address
0x020000 ~ 0x02FFFF:VP_N32 Address
0x030000 ~ 0x03FFFF:VP_N64 Address
```
```
4 th Addr2
5 th Addr1
6 th Addr0(LSB)
7 rd Length NO. No. of data
8 rd Data Data n(MSB) No. of byte of Data:
VP_N16 = Length *2,
VP_N32 = Length *4,
VP_N64 = Length *8,
```
```
:
:
```
:
:
(n-1)th Data1
nth Data0(LSB)
(n+1)th Tail 0xCC Communication packet tail
(n+2)th 0x33
(n+3)th 0xC3
(n+4)th 0x3C
Note.
*1. The Response code with communication packet format (see Communication Packet Structure Section for details)


## TOPWAY SMART LCD Command Manual Protocol V5.04

URL: [http://www.topwaydisplay.com](http://www.topwaydisplay.com)
Document Name:SMART LCD Command v5.04 Manual Rev0.4.doc

#### 4.4.3 BP1_write ( 0x4B )

Seq Cmd-code / Par-data Descriptions
1 0x4B Write raw bit-map data to the VP_BP1
2 Addr3(MSB) VP_BP1 Address = 0x040000 ~ 0x05FFFF
3 Addr2
4 Addr1
5 Addr0(LSB)
6 Length3(MSB) the number of data
Length = 1 ~ 98304
7 Length2
8 Length1
9 Length0(LSB)
Note.
*1. Command should be transferred in communication packet structure (see Communication Packet Structure Section for details)
*2. After the above command issued, it follow with the raw data byte with out communication packet structure.

#### *3. over all command flow

```
HOST Flow module
BP1_write Command
(in communication packet structure)
```
```
 Instruct to wait for data....
```
```
Raw 1bpp image data
(without communication packet structure)
```
```
 Store the data into VP_BP1
```
```
 Response code “:>” in ASCII
(without communication packet structure)
```
#### 4.4.4 BP1_write_compress ( 0x4C )

Seq Cmd-code / Par-data Descriptions
1 0x4C Write compressed bit-map data to the VP_BP1
2 Addr3(MSB) VP_BP1 Address = 0x040000 ~ 0x05FFFF
3 Addr2
4 Addr1
5 Addr0(LSB)
6 Length3(MSB) the number of data
7 Length2 Length = 1 ~ 98304
8 Length1
9 Length0(LSB)
Note.
*1. Command should be transferred in communication packet structure (see Communication Packet Structure Section for details)
*2. After the above command issued, it follow with the compressed data byte with out communication packet structure.

#### *3. over all command flow

```
HOST Flow module
BP1_write Command
(in communication packet structure)
```
```
 Instruct to wait for data....
```
```
compressed 1bpp image data
(without communication packet structure)
```
```
 Store the data into VP_BP1
```
```
 Response code “:>” in ASCII
(without communication packet structure)
```

## TOPWAY SMART LCD Command Manual Protocol V5.04

URL: [http://www.topwaydisplay.com](http://www.topwaydisplay.com)
Document Name:SMART LCD Command v5.04 Manual Rev0.4.doc

#### 4.4.5 G16_write ( 0x4D )

Seq Cmd-code / Par-data Descriptions
1 0x4D Write graph values to the VP_G16 array
2 Addr1_H VP_G16 Address = 0x060000 ~ 0x07FFFF
3 Addr1l
4 Addr2h
5 Addr2l
6 Sizeh Array-size = 1 ~ 1024
7 Sizel (*2, *3)
8 Data(MSB) 16 bit data array
9 Data(LSB) (no. of byte = 2x array-size)
10 Data(MSB)
11 Data(LSB)
: :
: :
: :
: :
: :
Note.
*1. Command should be transferred in communication packet structure (see Communication Packet Structure Section for details)
*2. Array-size = no. of 16bit values
*3. Array-size suggest to be same at the size value defined in TML editor

#### 4.4.6 G16_write_rotate ( 0x4E )

Seq Cmd-code / Par-data Descriptions
1 0x4E Write graph values to the last position of VP_G16 array
with rotation effect
2 Addr1_H VP_G16 Address = 0x060000 ~ 0x07FFFF
3 Addr1_L
4 Addr2_H
5 Addr2_L
6 Size_H Array-size to be rotate = 1 ~ 65535
7 Size_L (*2. *3)
8 Data(MSB) 16 bit data value to be add to the end-of-array
9 Data(LSB)
Note.
*1. Command should be transferred in communication packet structure (see Communication Packet Structure Section for details)
*2. Array-size = no. of 16bit values
*3. Array-size suggest to be same at the value defined in TML editor

#### 4.4.7 System Register Write ( 0x3B )

```
seq Cmd-code / Par-data Descriptions
1 0x3B System Register Write Command
2 Addr3(MSB)
Timer_Ctrl0 Address = 0xFFFF00
：
Timer_Ctrl7 Address = 0xFFFF07
```
3 Addr2
4 Addr1
5 Addr0(LSB)
6 Data the value to write
Note.
*1. Command should be transferred in communication packet structure (see Communication Packet Structure Section for details)


## TOPWAY SMART LCD Command Manual Protocol V5.04

URL: [http://www.topwaydisplay.com](http://www.topwaydisplay.com)
Document Name:SMART LCD Command v5.04 Manual Rev0.4.doc

#### 4.4.8 System Register Read ( 0x3C )

```
seq Cmd-code / Par-data Descriptions
1 0x3C System Register Read Command
2 Addr3(MSB)
Address = 0xFFFF00
：
Address = 0xFFFFFF
```
3 Addr2
4 Addr1
5 Addr0(LSB)
Note.
*1. Command should be transferred in communication packet structure (see Communication Packet Structure Section for details)

#### Response code:

```
Seq. Content Byte in Hex Descriptions
1 st Header 0xAA Communication packet header
2 nd Command 0x3C Command executed
3 rd
Address
```
```
Addr3(MSB) Address = 0xFFFF00
：
Address = 0xFFFFFF
```
```
4 th Addr2
5 th Addr1
6 th Addr0(LSB)
7 th Data Data(1Byte) the value of the register
8 th
Tail
```
```
0xCC
```
(^9) Communication packet tail
th 0x33
10 th 0xC3
11 th 0x3C
Note.
*1. The Response code with communication packet format (see Communication Packet Structure Section for details)
*2. When Timer reach the 0x00000000 or 0x7FFFFFFF, a notification will be provided a 0x77 response code with the corresponding
Timer Address and Value.(See touch_response(0x77)for details)

#### 4.4.9 STR_write ( 0x42 )

```
Seq Cmd-code / Par-data Descriptions
1 0x42 Write string to VP_STR
2 Addr3(MSB) the VP_STR Address = 0x00000 ~ 0x01FFFF
(each VP_STR = 128 bytes)
(address value must be divisible by 128)
```
3 Addr2
4 Addr1
5 Addr0(LSB)
6 data String to write
: : Total no. of byte in string ≤^128
: :
: ‘\0’ ‘\0’(0x00):string end mark
Note.
*1. Command should be transferred in communication packet structure (see Communication Packet Structure Section for details)

#### 4.4.10 STR_read ( 0x43 )

```
Seq Cmd-code / Par-data Descriptions
1 0x43 Read string from VP_STR
2 Addr3(MSB) the VP_STR Address = 0x00000 ~ 0x01FFFF
(each VP_STR = 128 bytes)
(address value must be divisible by 128)
```
3 Addr2
4 Addr1
5 Addr0(LSB)
Note.
*1. Command should be transferred in communication packet structure (see Communication Packet Structure Section for details)

#### Response code:

Seq. Content Byte in Hex Descriptions
1 st Header 0xAA Communication packet header
2 nd Command 0x43 Command executed
3 rd String data data String code
: :
: :
: \0 0x00 ‘‘\0’(0x00): string end mark
: Tail 0xCC Communication packet tail
: 0x33
: 0xC3
: 0x3C
Note.
*1. The Response code with communication packet format (see Communication Packet Structure Section for details)


## TOPWAY SMART LCD Command Manual Protocol V5.04

URL: [http://www.topwaydisplay.com](http://www.topwaydisplay.com)
Document Name:SMART LCD Command v5.04 Manual Rev0.4.doc

#### 4.4.11 STR_fill ( 0x46 )

```
Seq Cmd-code / Par-data Descriptions
1 0x46 Write string to VP_STR
2 Addr3(MSB) he VP_STR Address = 0x00000 ~ 0x01FFFF
(each VP_STR = 128 bytes)
(address value must be divisible by 128)
```
3 Addr2
4 Addr1
5 Addr0(LSB)
6 Lengthh the number of VP_STR (including the start address) to be filled
7 Lengthl Length = 1 ~ 1024
8 data String to write
: : Total no. of byte in string ≤^128
: :
: ‘\0’ ‘\0’(0x00):string end mark
Note.
*1. Command should be transferred in communication packet structure (see Communication Packet Structure Section for details)

#### 4.4.12 N16_write ( 0x3D )

```
Seq Cmd-code / Par-data Descriptions
1 0x3D Write 16bit number to VP_N16
2 Addr3(MSB) VP_N16 Address = 0x080000 ~ 0x08FFFF
(each VP_N16 = 2 byte)
(address value must be divisible by 2)
```
3 Addr2
4 Addr1
5 Addr0(LSB)
6 High Byte The 16 bit value to write
7 Low Byte
Note.
*1. Command should be transferred in communication packet structure (see Communication Packet Structure Section for details)

#### 4.4.13 N16_read ( 0x3E )

```
Seq Cmd-code / Par-data Descriptions
1 0x3E Read 16bit number from VP_N16
2 Addr3(MSB) VP_N16 Address = 0x080000 ~ 0x08FFFF
(each VP_N16 = 2 byte)
(address value must be divisible by 2)
```
3 Addr2
4 Addr1
5 Addr0(LSB)
Note.
*1. Command should be transferred in communication packet structure (see Communication Packet Structure Section for details

#### Response code:

Seq. Content Byte in Hex Descriptions
1 st Header 0xAA Communication packet header
2 nd Command 0x3E Command executed
3 rd N16 value Data1(MSB) 16 bit value
4 th Data0(LSB)
5 th Tail 0xCC Communication packet tail
6 th 0x33
7 th 0xC3
8 th 0x3C
Note.
*1. The Response code with communication packet format (see Communication Packet Structure Section for details)

#### 4.4.14 N16_fill ( 0x3F )

```
Seq Cmd-code / Par-data Descriptions
1 0x3F Fill 16bit number to the VP_N16
2 Addr3(MSB) VP_N16 Address = 0x080000 ~ 0x08FFFF
(each VP_N16 = 2 byte)
(address value must be divisible by 2)
```
3 Addr2
4 Addr1
5 Addr0(LSB)
6 Lengthh the number of VP_N16 (including the start address) to be filled
7 Lengthl Length = 1 ~ 32768
8 High Byte the 16 bit value to fill
9 Low Byte
Note.

#### *1. Command should be transferred in communication packet structure (see Communication Packet Structure Section for details)


## TOPWAY SMART LCD Command Manual Protocol V5.04

URL: [http://www.topwaydisplay.com](http://www.topwaydisplay.com)
Document Name:SMART LCD Command v5.04 Manual Rev0.4.doc

#### 4.4.15 N32_write ( 0x44 )

```
Seq Cmd-code / Par-data Descriptions
1 0x44 Write 32bit number to VP_N32
2 Addr3(MSB) VP_N32 Address = 0x020000 ~ 0x02FFFF
(each VP_N32 = 4 byte)
(address value must be divisible by 4)
```
3 Addr2
4 Addr1
5 Addr0(LSB)
6 Data3(MSB) the 32 bit no. value write.
7 Data2
8 Data1
9 Data0(LSB)
Note.
*1. Command should be transferred in communication packet structure (see Communication Packet Structure Section for details)

#### 4.4.16 N32_read ( 0x45 )

```
Seq Cmd-code / Par-data Descriptions
1 0x45 Read 32bit number from VP_N32
2 Addr3(MSB) VP_N32 Address = 0x020000 ~ 0x02FFFF
(each VP_N32 = 4 byte)
(address value must be divisible by 4)
```
3 Addr2
4 Addr1
5 Addr0(LSB)
Note.
*1. Command should be transferred in communication packet structure (see Communication Packet Structure Section for details

#### Response code:

Seq. Content Byte in Hex Descriptions
1 st Header 0xAA Communication packet header
2 nd Command 0x45 Command executed
3 rd N32 value Data3(MSB) 32 bit value
4 th Data2
5 th Data1
6 th Data0(LSB)
7 th Tail 0xCC Communication packet tail
8 th 0x33
9 th 0xC3
10 th 0x3C
Note.
*1. The Response code with communication packet format (see Communication Packet Structure Section for details)

#### 4.4.17 N32_fill ( 0x47 )

```
Seq Cmd-code / Par-data Descriptions
1 0x47 Fill 32bit number to the VP_N32
2 Addr3(MSB) VP_N32 Address = 0x020000 ~ 0x02FFFF
(each VP_N32 = 4 byte)
(address value must be divisible by 4)
```
3 Addr2
4 Addr1
5 Addr0(LSB)
6 Lengthh the number of VP_N32 (including the start address) to be filled
7 Lengthl Length = 1 ~ 16384
8 Data3(MSB) the 32 bit no. value to fill
9 Data2
10 Data1
11 Data0(LSB)
Note.
*1. Command should be transferred in communication packet structure (see Communication Packet Structure Section for details)


## TOPWAY SMART LCD Command Manual Protocol V5.04

URL: [http://www.topwaydisplay.com](http://www.topwaydisplay.com)
Document Name:SMART LCD Command v5.04 Manual Rev0.4.doc

#### 4.4.18 N64_write ( 0x48 )

```
Seq Cmd-code / Par-data Descriptions
1 0x48 Write 64bit number to VP_N64
2 Addr3(MSB) VP_N64 Address= 0x030000 ~ 0x03FFFF
(each VP_N64 = 8 byte)
(address value must be divisible by 8)
```
```
3 Addr2
4 Addr1
5 Addr0(LSB)
6 Data7(MSB) the 64bit no. value write.
7 Data6
:
:
```
##### :

##### :

12 Data1
13 Data0(LSB)
Note.
*1. Command should be transferred in communication packet structure (see Communication Packet Structure Section for details)

#### 4.4.19 N64_read ( 0x49 )

```
Seq Cmd-code / Par-data Descriptions
1 0x49 Read 64bit number from VP_N64
2 Addr3(MSB) VP_N64 Address= 0x030000 ~ 0x03FFFF
(each VP_N64 = 8 byte)
(address value must be divisible by 8)
```
3 Addr2
4 Addr1
5 Addr0(LSB)
Note.
*1. Command should be transferred in communication packet structure (see Communication Packet Structure Section for details

#### Response code:

```
Seq. Content Byte in Hex Descriptions
1 st Header 0xAA Communication packet header
2 nd Command 0x49 Command executed
3 rd N64 value Data7(MSB) 64 bit value
4 th Data6
:
:
```
:
:
9 th Data1
10 th Data0(LSB)
11 th Tail 0xCC Communication packet tail
12 th 0x33
13 th 0xC3
14 th 0x3C
Note.
*1. The Response code with communication packet format (see Communication Packet Structure Section for details)

#### 4.4.20 N64_fill ( 0x4A )

```
Seq Cmd-code / Par-data Descriptions
1 0x4A Fill 64bit number to the VP_N64
2 Addr3(MSB) VP_N64 Address= 0x030000 ~ 0x03FFFF
(each VP_N64 = 8 byte)
(address value must be divisible by 8)
```
```
3 Addr2
4 Addr1
5 Addr0(LSB)
6 Lengthh the number of VP_N64 (including the start address) to be filled
7 Lengthl Length = 1 ~ 8192
8 Data7(MSB) the 64 bit no. value to fill
9 Data6
:
:
```
##### :

##### :

14 Data1
15 Data0(LSB)
Note.

#### *1. Command should be transferred in communication packet structure (see Communication Packet Structure Section for details)



