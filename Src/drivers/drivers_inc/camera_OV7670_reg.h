/*
 * camera_OV7670_reg.h
 *
 *  Created on: Apr 3, 2017
 *      Author: Matt Buonadonna
 */

#ifndef __CAMERA_OV7670_REG_H__
#define __CAMERA_OV7670_REG_H__

//Address | Register Name | Default | R/W
//  00        GAIN            00      RW
//Description:
//AGC – Gain control gain setting
//Bit[7:0]: AGC[7:0] (see VREF[7:6] (0x03) for AGC[9:8])
//• Range: [00] to [FF]
//
#define OV7670_REG_GAIN                  0x00

//Address | Register Name | Default | R/W
//  01        BLUE            80
//Description:
//RW AWB – Blue channel gain setting
//  • Range: [00] to [FF]
//
#define OV7670_REG_BLUE

//Address | Register Name | Default | R/W
//  02        RED             80      RW
//Description:
//AWB – Red channel gain setting
//  • Range: [00] to [FF]
//
#define OV7670_REG_RED

//Address | Register Name | Default | R/W
//  03        VREF            03      RW
//Description:
//Vertical Frame Control
//Bit[7:6]: AGC[9:8] (see GAIN[7:0] (0x00) for AGC[7:0])
//Bit[5:4]: Reserved
//Bit[3:2]: VREF end low 2 bits (high 8 bits at VSTOP[7:0]
//Bit[1:0]: VREF start low 2 bits (high 8 bits at VSTRT[7:0]
//
#define OV7670_REG_VREF

//Address | Register Name | Default | R/W
//  04        COM1            00      RW
//Description:
//Common Control 1
//Bit[7]: Reserved
//Bit[6]: CCIR656 format
//        0: Disable
//        1: Enable
//Bit[5:2]: Reserved
//Bit[1:0]: AEC low 2 LSB (see registers AECHH for AEC[15:10] and
//          AECH for AEC[9:2])
//
#define OV7670_REG_COM1

//Address | Register Name | Default | R/W
//  05        BAVE          00        RW
//Description:
//U/B Average Level
//Automatically updated based on chip output format
//
#define OV7670_REG_BAVE

//Address | Register Name | Default | R/W
//  06        GbAVE           00      RW
//Description:
//Y/Gb Average Level
//Automatically updated based on chip output format
//
#define OV7670_REG_GbAVE

//Address | Register Name | Default | R/W
//  07        AECHH           00      RW
//Description:
//Exposure Value - AEC MSB 5 bits
//Bit[7:6]: Reserved
//Bit[5:0]: AEC[15:10] (see registers AECH for AEC[9:2] and COM1
//for AEC[1:0])
//
#define OV7670_REG_AECHH

//Address | Register Name | Default | R/W
//  08        RAVE            00      RW
//Description:
//V/R Average Level
//Automatically updated based on chip output format
//
#define OV7670_REG_RAVE

//Address | Register Name | Default | R/W
//  09        COM2            01      RW
//Description:
//Common Control 2
//Bit[7:5]: Reserved
//Bit[4]: Soft sleep mode
//Bit[3:2]: Reserved
//Bit[1:0]: Output Drive Capability
//          00: 1x
//          01: 2x
//          10: 3x
//          11: 4x
//
#define OV7670_REG_COM2

//Address | Register Name | Default | R/W
//  0A        PID             76      R
//Description:
//Product ID Number MSB (Read only)
//
#define OV7670_REG_PID

//Address | Register Name | Default | R/W
//  0B        VER             70      R
//Description:
//Product ID Number LSB (Read only)
//
#define OV7670_REG_VER

//Address | Register Name | Default | R/W
//  0C        COM3            00      RW
//Description:
//Common Control 3
//Bit[7]: Reserved
//Bit[6]: Output data MSB and LSB swap
//Bit[5]: Tri-state option for output clock at power-down period
//        0: Tri-state at this period
//        1: No tri-state at this period
//Bit[4]: Tri-state option for output data at power-down period
//        0: Tri-state at this period
//        1: No tri-state at this period
//Bit[3]: Scale enable
//        0: Disable
//        1: Enable - if set to a pre-defined format (see COM7[5:3]),
//        then COM14[3] must be set to 1 for manual adjustment.
//Bit[2]: DCW enable
//        0: Disable
//        1: Enable - if set to a pre-defined format (see COM7[5:3]),
//        then COM14[3] must be set to 1 for manual adjustment.
//Bit[1:0]: Reserved
//
#define OV7670_REG_COM3

//Address | Register Name | Default | R/W
//  0D        COM4            40      RW
//Description:
//Common Control 4
//Bit[7:6]: Reserved
//Bit[5:4]: Average option (must be same value as COM17[7:6])
//          00: Full window
//          01: 1/2 window
//          10: 1/4 window
//          11: 1/4 window
//Bit[3:0]: Reserved
//
#define OV7670_REG_COM4

//Address | Register Name | Default | R/W
//  0E        COM5            01      RW
//Description:
//Common Control 5
//Bit[7:0]: Reserved
//
#define OV7670_REG_COM5

//Address | Register Name | Default | R/W
//  0F        COM6            43      RW
//Description:
//Common Control 6
//Bit[7]: Output of optical black line option
//        0: Disable HREF at optical black
//        1: Enable HREF at optical black
//Bit[6]: BLC input selection
//        0: Use electrical black line as BLC signal
//        1: Use optical black line as BLC signal
//Bit[5]: Digital BLC enable
//Bit[4:2]: Reserved
//Bit[1]: Reset all timing when format changes
//Bit[0]: Reserved
//
#define OV7670_REG_COM6

//Address | Register Name | Default | R/W
//  10        AECH            40      RW
//Description:
//Exposure Value
//Bit[7:0]: AEC[9:2] (see registers AECHH for AEC[15:10] and
//          COM1 for AEC[1:0])
//
#define OV7670_REG_AECH

//Address | Register Name | Default | R/W
//  11        CLKRC           80      RW
//Description:
//Internal Clock
//Bit[7]: Digital PLL option
//        0: Disable double clock option, meaning the maximum
//           PCLK can be as high as half input clock
//        1: Enable double clock option, meaning the maximum
//           PCLK can be as high as input clock
//Bit[6]: Use external clock directly (no clock pre-scale available)
//Bit[5:0]: Internal clock pre-scalar
//          F(internal clock) = F(input clock)/(Bit[5:0]+1)
//          • Range: [0 0000] to [1 1111]
//
#define OV7670_REG_CLKRC                 0x11
  //Bit[7]: Digital PLL option
  #define OV7670_REG_CLKRC_DPLL_EN         0b00000000
  #define OV7670_REG_CLKRC_DPLL_DI         0b10000000
  //Bit[6]: Use external clock directly (no clock pre-scale available)
  #define OV7670_REG_CLKRC_USE_INTCLK      0b00000000
  #define OV7670_REG_CLKRC_USE_EXTCLK      0b01000000
  //Bit[5:0]: Internal clock pre-scalar
  #define OV7670_REG_CLKRC_USE_PS_2        0b00000001
  #define OV7670_REG_CLKRC_USE_PS_3        0b00000010
  #define OV7670_REG_CLKRC_USE_PS_4        0b00000011
  #define OV7670_REG_CLKRC_USE_PS_5        0b00000100
  //add more as needed
  #define OV7670_REG_CLKRC_USE_PS_31       0b00011111

//Address | Register Name | Default | R/W
//  12        COM7            00      RW
//Description:
//Common Control 7
//Bit[7]: SCCB Register Reset
//        0: No change
//        1: Resets all registers to default values
//Bit[6]: Reserved
//Bit[5]: Output format - CIF selection
//Bit[4]: Output format - QVGA selection
//Bit[3]: Output format - QCIF selection
//Bit[2]: Output format - RGB selection (see below)
//Bit[1]: Color bar
//        0: Disable
//        1: Enable
//Bit[0]: Output format - Raw RGB (see below)
//                    COM7[2] COM7[0]
//YUV                   0       0
//RGB                   1       0
//Bayer RAW             0       1
//Processed Bayer RAW   1       1
//
#define OV7670_REG_COM7

//Address | Register Name | Default | R/W
//  13        COM8            8F      RW
//Description:
//Common Control 8
//Bit[7]: Enable fast AGC/AEC algorithm
//Bit[6]: AEC - Step size limit
//        0: Step size is limited to vertical blank
//        1: Unlimited step size
//Bit[5]: Banding filter ON/OFF - In order to turn ON the banding
//        filter, BD50ST (0x9D) or BD60ST (0x9E) must be set to a
//        non-zero value.
//        0: ON
//        1: OFF
//Bit[4:3]: Reserved
//Bit[2]: AGC Enable
//Bit[1]: AWB Enable
//Bit[0]: AEC Enable
//
#define OV7670_REG_COM8

//Address | Register Name | Default | R/W
//  14        COM9            4A      RW
//Description:
//Common Control 9
//Bit[7]: Reserved
//Bit[6:4]: Automatic Gain Ceiling - maximum AGC value
//          000: 2x
//          001: 4x
//          010 8x
//          011: 16x
//          100: 32x
//          101 64x
//          110: 128x
//          111: Not allowed
//Bit[3:1]: Reserved
//Bit[0]: Freeze AGC/AEC
//
#define OV7670_REG_COM9

//Address | Register Name | Default | R/W
//  15        COM10           00      RW
//Description:
//Common Control 10
//Bit[7]: Reserved
//Bit[6]: HREF changes to HSYNC
//Bit[5]: PCLK output option
//        0: Free running PCLK
//        1: PCLK does not toggle during horizontal blank
//Bit[4]: PCLK reverse
//Bit[3]: HREF reverse
//Bit[2]: VSYNC option
//        0: VSYNC changes on falling edge of PCLK
//        1: VSYNC changes on rising edge of PCLK
//Bit[1]: VSYNC negative
//Bit[0]: HSYNC negative
//
#define OV7670_REG_COM10

//Address | Register Name | Default | R/W
//  16        RSVD            XX – Reserved
//
#define OV7670_REG_RSVD

//Address | Register Name | Default | R/W
//  17        HSTART          11      RW
//Description:
//Output Format - Horizontal Frame (HREF column) start high 8-bit (low
//3 bits are at HREF[2:0])
//
#define OV7670_REG_HSTART

//Address | Register Name | Default | R/W
//  18        HSTOP           61      RW
//Description:
//Output Format - Horizontal Frame (HREF column) end high 8-bit (low
//3 bits are at HREF[5:3])
//
#define OV7670_REG_HSTOP

//Address | Register Name | Default | R/W
//  19        VSTRT           03      RW
//Description:
//Output Format - Vertical Frame (row) start high 8-bit (low 2 bits are at
//VREF[1:0])
//
#define OV7670_REG_VSTRT

//Address | Register Name | Default | R/W
//  1A        VSTOP           7B      RW
//Description:
//Output Format - Vertical Frame (row) end high 8-bit (low 2 bits are at
//VREF[3:2])
//
#define OV7670_REG_VSTOP

//Address | Register Name | Default | R/W
//  1B        PSHFT           00      RW
//Description:
//Data Format - Pixel Delay Select (delays timing of the D[7:0] data relative
//to HREF in pixel units)
//• Range: [00] (no delay) to [FF] (256 pixel delay which accounts for
//whole array)
//
#define OV7670_REG_PSHFT

//Address | Register Name | Default | R/W
//  1C        MIDH            7F      R
//Description:
//Manufacturer ID Byte – High (Read only = 0x7F)
//
#define OV7670_REG_MIDH

//Address | Register Name | Default | R/W
//  1D        MIDL            A2      R
//Description:
//Manufacturer ID Byte – Low (Read only = 0xA2)
//
#define OV7670_REG_MIDL

//Address | Register Name | Default | R/W
//  1E        MVFP            00      RW
//Description:
//Mirror/VFlip Enable
//Bit[7:6]: Reserved
//Bit[5]: Mirror
//        0: Normal image
//        1: Mirror image
//Bit[4]: VFlip enable
//        0: Normal image
//        1: Vertically flip image
//Bit[3]: Reserved
//Bit[2]: Black sun enable
//Bit[1:0]: Reserved
//
#define OV7670_REG_MVFP

//Address | Register Name | Default | R/W
//  1F        LAEC            00      RW
//Description:
//Reserved
//
#define OV7670_REG_LAEC

//Address | Register Name | Default | R/W
//  20        ADCCTR0         04      RW
//Description:
//ADC Control
//Bit[7:4]: Reserved
//Bit[3]: ADC range adjustment
//        0: 1x range
//        1: 1.5x range
//Bit[2:0]: ADC reference adjustment
//          000: 0.8x
//          100: 1x
//          111: 1.2x
//
#define OV7670_REG_ADCCTR0

//Address | Register Name | Default | R/W
//  21        ADCCTR1         02      RW
//Description:
//Bit[7:0]: Reserved
//
#define OV7670_REG_ADCCTR1

//Address | Register Name | Default | R/W
//  22        ADCCTR2         01      RW
//Description:
//Bit[7:0]: Reserved
//
#define OV7670_REG_ADCCTR2

//Address | Register Name | Default | R/W
//  23        ADCCTR3         80      RW
//Description:
//Bit[7:0]: Reserved
//
#define OV7670_REG_ADCCTR3

//Address | Register Name | Default | R/W
//  24        AEW             75      RW
//Description:
//AGC/AEC - Stable Operating Region (Upper Limit)
//
#define OV7670_REG_AEW

//Address | Register Name | Default | R/W
//  25        AEB             63      RW
//Description:
//AGC/AEC - Stable Operating Region (Lower Limit)
//
#define OV7670_REG_AEB

//Address | Register Name | Default | R/W
//  26        VPT             D4      RW
//Description:
//AGC/AEC Fast Mode Operating Region
//Bit[7:4]: High nibble of upper limit of fast mode control zone
//Bit[3:0]: High nibble of lower limit of fast mode control zone
//
#define OV7670_REG_VPT

//Address | Register Name | Default | R/W
//  27        BBIAS           80      RW
//Description:
//B Channel Signal Output Bias (effective only when COM6[3] = 1)
//Bit[7]: Bias adjustment sign
//        0: Add bias
//        1: Subtract bias
//Bit[6:0]: Bias value of 10-bit range
//
#define OV7670_REG_BBIAS

//Address | Register Name | Default | R/W
//  28        GbBIAS          80      RW
//Description:
//Gb Channel Signal Output Bias (effective only when COM6[3] = 1)
//Bit[7]: Bias adjustment sign
//        0: Add bias
//        1: Subtract bias
//Bit[6:0]: Bias value of 10-bit range
//
#define OV7670_REG_GbBIAS

//Address | Register Name | Default | R/W
//  29        RSVD            XX – Reserved
//
#define OV7670_REG_

//Address | Register Name | Default | R/W
//  2A        EXHCH           00      RW
//Description:
//Dummy Pixel Insert MSB
//Bit[7:4]: 4 MSB for dummy pixel insert in horizontal direction
//Bit[3:2]: HSYNC falling edge delay 2 MSB
//Bit[1:0]: HSYNC rising edge delay 2 MSB
//
#define OV7670_REG_EXHCH

//Address | Register Name | Default | R/W
//  2B        EXHCL           00      RW
//Description:
//Dummy Pixel Insert LSB
//8 LSB for dummy pixel insert in horizontal direction
//
#define OV7670_REG_EXHCL

//Address | Register Name | Default | R/W
//  2C        RBIAS           80      RW
//Description:
//R Channel Signal Output Bias (effective only when COM6[3] = 1)
//Bit[7]: Bias adjustment sign
//        0: Add bias
//        1: Subtract bias
//Bit[6:0]: Bias value of 10-bit range
//
#define OV7670_REG_RBIAS

//Address | Register Name | Default | R/W
//  2D        ADVFL           00      RW
//Description:
//LSB of insert dummy lines in vertical direction (1 bit equals 1 line)
//
#define OV7670_REG_ADVFL

//Address | Register Name | Default | R/W
//  2E        ADVFH           00      RW
//Description:
//MSB of insert dummy lines in vertical direction
//
#define OV7670_REG_ADVFH

//Address | Register Name | Default | R/W
//  2F        YAVE            00      RW
//Description:
//Y/G Channel Average Value
//
#define OV7670_REG_YAVE

//Address | Register Name | Default | R/W
//  30        HSYST           08      RW
//Description:
//HSYNC Rising Edge Delay (low 8 bits)
//
#define OV7670_REG_HSYST

//Address | Register Name | Default | R/W
//  31        HSYEN           30      RW
//Description:
//HSYNC Falling Edge Delay (low 8 bits)
//
#define OV7670_REG_HSYEN

//Address | Register Name | Default | R/W
//  32        HREF            80      RW
//Description:
//HREF Control
//Bit[7:6]: HREF edge offset to data output
//Bit[5:3]: HREF end 3 LSB (high 8 MSB at register HSTOP)
//Bit[2:0]: HREF start 3 LSB (high 8 MSB at register HSTART)
//
#define OV7670_REG_HREF

//Address | Register Name | Default | R/W
//  33        CHLF            08      RW
//Description:
//Array Current Control
//Bit[7:0]: Reserved
//
#define OV7670_REG_CHLF

//Address | Register Name | Default | R/W
//  34        ARBLM           03      RW
//Description: Array Reference Control
//Bit[7:0]: Reserved
//
#define OV7670_REG_ARBLM

//Address | Register Name | Default | R/W
//  35-36     RSVD            XX – Reserved
//
#define OV7670_REG_RSVD

//Address | Register Name | Default | R/W
//  37        ADC             04      RW
//Description:
//ADC Control
//Bit[7:0]: Reserved
//
#define OV7670_REG_ADC

//Address | Register Name | Default | R/W
//  38        ACOM            12      RW
//Description:
// ADC and Analog Common Mode Control
//Bit[7:0]: Reserved
//
#define OV7670_REG_ACOM

//Address | Register Name | Default | R/W
//  39        OFON            00      RW
//Description:
//Bit[7:0]: Reserved
//
#define OV7670_REG_OFON

//Address | Register Name | Default | R/W
//  3A        TSLB            0C      RW
//Description:
//
//
//Line Buffer Test Option
//
//Bit[7:6]: Reserved
//
//Bit[5]: Negative image enable
//
//0: Normal image
//
//1: Negative image
//
//Bit[4]: UV output value
//
//0: Use normal UV output
//
//1: Use fixed UV value set in registers MANU and MANV
//
//as UV output instead of chip output
//
//Bit[3]: Output sequence (use with register COM13[1] (0x3D))
//
//TSLB[3], COM13[1]:
//
//00: Y U Y V
//
//01: Y V Y U
//
//10: U Y V Y
//
//11: V Y U Y
//
//Bit[2:0]: Reserved
//
#define OV7670_REG_TSLB

//Address | Register Name | Default | R/W
//  3B        COM11           00      RW
//Description:
//
//
//Common Control 11
//
//Bit[7]: Night mode
//
//0: Night mode disable
//
//1: Night mode enable - The frame rate is reduced
//
//automatically while the minimum frame rate is limited
//
//by COM11[6:5]. Also, ADVFH and ADVFL will be
//
//automatically updated.
//
//Bit[6:5]: Minimum frame rate of night mode
//
//00: Same as normal mode frame rate
//
//01: 1/2 of normal mode frame rate
//
//10: 1/4 of normal mode frame rate
//
//11: 1/8 of normal mode frame rate
//
//Bit[4]: D56_Auto
//
//0: Disable 50/60 Hz auto detection
//
//1: Enable 50/60 Hz auto detection
//
//Bit[3]: Banding filter value select (effective only when
//
//COM11[4] = 0)
//
//0: Select BD60ST[7:0] (0x9E) as Banding Filter Value
//
//1: Select BD50ST[7:0] (0x9D) as Banding Filter Value
//
//Bit[2]: Reserved
//
//Bit[1]: Exposure timing can be less than limit of banding filter
//
//when light is too strong
//
//Bit[0]: Reserved
//
#define OV7670_REG_COM11

//Address | Register Name | Default | R/W
//  3C        COM12           40      RW
//Description:
//
//
//Common Control 12
//
//Bit[7]: HREF option
//
//0: No HREF when VSYNC is low
//
//1: Always has HREF
//
//Bit[6:0]: Reserved
//
#define OV7670_REG_COM12

//Address | Register Name | Default | R/W
//  3D        COM13           99      RW
//Description:
//
//
//Common Control 13
//
//Bit[7]: Gamma enable
//
//Bit[6]: UV saturation level - UV auto adjustment. Result is saved
//
//in register SATCTR[3:0] (0xC9)
//
//Bit[5:2]: Reserved
//
//Bit[1]: UV swap (use with register TSLB[3] (0x3A))
//
//TSLB[3], COM13[1]:
//
//00: Y U Y V
//
//01: Y V Y U
//
//10: U Y V Y
//
//11: V Y U Y
//
//Bit[0]: Reserved
//
#define OV7670_REG_COM13

//Address | Register Name | Default | R/W
//  3E        COM14           0E      RW
//Description:
//
//
//Common Control 14
//
//Bit[7:5]: Reserved
//
//Bit[4]: DCW and scaling PCLK enable
//
//0: Normal PCLK
//
//1: DCW and scaling PCLK, controlled by register
//
//COM14[2:0] and SCALING_PCLK_DIV[3:0] (0x73))
//
//Bit[3]: Manual scaling enable for pre-defined resolution modes
//
//such as CIF, QCIF, and QVGA
//
//0: Scaling parameter cannot be adjusted manually
//
//1: Scaling parameter can be adjusted manually
//
//Bit[2:0]: PCLK divider (only when COM14[4] = 1)
//
//000: Divided by 1
//
//001: Divided by 2
//
//010 Divided by 4
//
//011: Divided by 8
//
//100: Divided by 16
//
//101~111: Not allowed
//
#define OV7670_REG_COM14

//Address | Register Name | Default | R/W
//  3F        EDGE            88      RW
//Description:
//
//
//Edge Enhancement Adjustment
//
//Bit[7:5]: Reserved
//
//Bit[4:0]: Edge enhancement factor
//
#define OV7670_REG_EDGE

//Address | Register Name | Default | R/W
//  40        COM15           C0      RW
//Description:
//
//
//Common Control 15
//
//Bit[7:6]: Data format - output full range enable
//
//0x: Output range: [10] to [F0]
//
//10: Output range: [01] to [FE]
//
//11: Output range: [00] to [FF]
//
//Bit[5:4]: RGB 555/565 option (must set COM7[2] = 1 and
//
//COM7[0] = 0)
//
//x0: Normal RGB output
//
//01: RGB 565
//
//11: RGB 555
//
//Bit[3:0]: Reserved
//
#define OV7670_REG_COM15

//Address | Register Name | Default | R/W
//  41        COM16           10      RW
//Description:
//
//
//Common Control 16
//
//Bit[7:6]: Reserved
//
//Bit[5]: Enable edge enhancement threshold auto-adjustment for
//
//YUV output (result is saved in register EDGE[4:0] (0x3F)
//
//and range is controlled by registers REG75[4:0] (0x75)
//
//and REG76[4:0] (0x76))
//
//0: Disable
//
//1: Enable
//
//Bit[4]: De-noise threshold auto-adjustment (result is saved in
//
//register DNSTH (0x4C) and range is controlled by register
//
//REG77[7:0] (0x77))
//
//0: Disable
//
//1: Enable
//
//Bit[3]: AWB gain enable
//
//Bit[2]: Reserved
//
//Bit[1]: Color matrix coefficient double option
//
//0: Original matrix
//
//1: Double of original matrix
//
//Bit[0]: Reserved
//
#define OV7670_REG_COM16

//Address | Register Name | Default | R/W
//  42        COM17           08      RW
//Description:
//
//
//Common Control 17
//
//Bit[7:6]: AEC window must be the same value as COM4[5:4])
//
//00: Normal
//
//01: 1/2
//
//10: 1/4
//
//11: 1/4
//
//Bit[5:4]: Reserved
//
//Bit[3]: DSP color bar enable
//
//0: Disable
//
//1: Enable
//
//Bit[2:0]: Reserved
//
#define OV7670_REG_COM17

//Address | Register Name | Default | R/W
//  43        AWBC1           14      RW
//Description:
// Reserved
//
#define OV7670_REG_AWBC1

//Address | Register Name | Default | R/W
//  44        AWBC2           F0      RW Reserved
//
#define OV7670_REG_AWBC2

//Address | Register Name | Default | R/W
//  45        AWBC3           45      RW Reserved
//
#define OV7670_REG_AWBC3

//Address | Register Name | Default | R/W
//  46        AWBC4           61      RW Reserved
//
#define OV7670_REG_AWBC4

//Address | Register Name | Default | R/W
//  47        AWBC5           51      RW Reserved
//
#define OV7670_REG_AWBC5

//Address | Register Name | Default | R/W
//  48        AWBC6           79      RW Reserved
//
#define OV7670_REG_AWBC6

//Address | Register Name | Default | R/W
//  49-4A     RSVD            XX – Reserved
//
#define OV7670_REG_RSVD

//Address | Register Name | Default | R/W
//  4B        REG4B           00      RW
//Description:
//Register 4B
//Bit[7:1]: Reserved
//Bit[0]: UV average enable
//
#define OV7670_REG_REG4B

//Address | Register Name | Default | R/W
//  4C        DNSTH           00      RW
//Description:
// De-noise Threshold
//
#define OV7670_REG_DNSTH

//Address | Register Name | Default | R/W
//  4D-4E RSVD XX – Reserved
//
#define OV7670_REG_

//Address | Register Name | Default | R/W
//  4F MTX1 40 RW
//Description:
// Matrix Coefficient 1
//
#define OV7670_REG_

//Address | Register Name | Default | R/W
//  50 MTX2 34 RW
//Description:
// Matrix Coefficient 2
//
#define OV7670_REG_

//Address | Register Name | Default | R/W
//  51 MTX3 0C RW
//Description:
// Matrix Coefficient 3
//
#define OV7670_REG_

//Address | Register Name | Default | R/W
//  52 MTX4 17 RW
//Description:
// Matrix Coefficient 4
//
#define OV7670_REG_

//Address | Register Name | Default | R/W
//  53 MTX5 29 RW
//Description:
// Matrix Coefficient 5
//
#define OV7670_REG_

//Address | Register Name | Default | R/W
//  54 MTX6 40 RW
//Description:
// Matrix Coefficient 6
//
#define OV7670_REG_

//Address | Register Name | Default | R/W
//  55 BRIGHT 00 RW
//Description:
// Brightness Control
//
#define OV7670_REG_

//Address | Register Name | Default | R/W
//  56 CONTRAS 40 RW
//Description:
// Contrast Control
//
#define OV7670_REG_

//Address | Register Name | Default | R/W
//  57 CONTRASCENTER 80 RW
//Description:
// Contrast Center
//
#define OV7670_REG_

//Address | Register Name | Default | R/W
//  58 MTXS 1E RW
//Description:
//
//
//Matrix Coefficient Sign for coefficient 5 to 0
//
//Bit[7]: Auto contrast center enable
//
//0: Disable, center is set by register
//
//CONTRAS-CENTER (0x57)
//
//1: Enable, register CONTRAS-CENTER is updated
//
//automatically
//
//Bit[6]: Reserved
//
//Bit[5:0]: Matrix coefficient sign
//
//0: Plus
//
//1: Minus
//
#define OV7670_REG_

//Address | Register Name | Default | R/W
//  59-61 RSVD XX – AWB Control
//
#define OV7670_REG_

//Address | Register Name | Default | R/W
//  62 LCC1 00 RW
//Description:
// Lens Correction Option 1
//
#define OV7670_REG_

//Address | Register Name | Default | R/W
//  63 LCC2 00 RW
//Description:
// Lens Correction Option 2
//
#define OV7670_REG_

//Address | Register Name | Default | R/W
//  64 LCC3 10 RW
//Description:
// Lens Correction Option 3
//
#define OV7670_REG_

//Address | Register Name | Default | R/W
//  65 LCC4 80 RW
//Description:
// Lens Correction Option 4
//
#define OV7670_REG_

//Address | Register Name | Default | R/W
//  66 LCC5 00 RW
//Description:
//
//
//Lens Correction Control
//
//Bit[7:3]: Reserved
//
//Bit[2]: Lens correction control select
//
//Bit[1]: Reserved
//
//Bit[0]: Lens correction enable
//
#define OV7670_REG_

//Address | Register Name | Default | R/W
//  67 MANU 80 RW
//Description:
// Manual U Value (effective only when register TSLB[4] is high)
//
#define OV7670_REG_

//Address | Register Name | Default | R/W
//  68 MANV 80 RW
//Description:
// Manual V Value (effective only when register TSLB[4] is high)
//
#define OV7670_REG_

//Address | Register Name | Default | R/W
//  69 GFIX 00 RW
//Description:
//
//
//Fix Gain Control
//
//Bit[7:6]: Fix gain for Gr channel
//
//00: 1x
//
//01: 1.25x
//
//10: 1.5x
//
//11: 1.75x
//
//Bit[5:4]: Fix gain for Gb channel
//
//00: 1x
//
//01: 1.25x
//
//10: 1.5x
//
//11: 1.75x
//
//Bit[3:2]: Fix gain for R channel
//
//00: 1x
//
//01: 1.25x
//
//10: 1.5x
//
//11: 1.75x
//
//Bit[1:0]: Fix gain for B channel
//
//00: 1x
//
//01: 1.25x
//
//10: 1.5x
//
//11: 1.75x
//
#define OV7670_REG_

//Address | Register Name | Default | R/W
//  6A GGAIN 00 RW
//Description:
// G Channel AWB Gain
//
#define OV7670_REG_

//Address | Register Name | Default | R/W
//  6B        DBLV            3A      RW
//Description:
//Bit[7:6]: PLL control
//          00: Bypass PLL
//          01: Input clock x4
//          10: Input clock x8
//          11: Input clock x16
//Bit[5]: Reserved
//Bit[4]: Regulator control
//        0: Enable internal regulator
//        1: Bypass internal regulator
//Bit[3:0]: Clock divider control for DSP scale control (valid only
//          when COM14[3] = 1)
//
#define OV7670_REG_DBLV                  0x6B
  //Bit[7:6]: PLL control
  #define OV7670_REG_DBLV_PLL_BP          0b00000000  //Bypass PLL
  #define OV7670_REG_DBLV_IC_4            0b01000000  //Input clock x4
  #define OV7670_REG_DBLV_IC_8            0b10000000  //Input clock x8
  #define OV7670_REG_DBLV_IC_16           0b11000000  //Input clock x16
  //Bit[4]: Regulator control
  #define OV7670_REG_DBLV_REGULATOR_EN    0b00000000  //Enable internal regulator
  #define OV7670_REG_DBLV_REGULATOR_BP    0b00010000  //Bypass internal regulator
  //Bit[3:0]: Clock divider control for DSP scale control
  #define OV7670_REG_DBLV_CLK_DIV_1       0b00000001

//Address | Register Name | Default | R/W
//  6C AWBCTR3 02 RW
//Description:
// AWB Control 3
//
#define OV7670_REG_AWBCTR3

//Address | Register Name | Default | R/W
//  6D AWBCTR2 55 RW
//Description:
// AWB Control 2
//
#define OV7670_REG_

//Address | Register Name | Default | R/W
//  6E AWBCTR1 00 RW
//Description:
// AWB Control 1
//
#define OV7670_REG_

//Address | Register Name | Default | R/W
//  6F AWBCTR0 9A RW
//Description:
// AWB Control 0
//
#define OV7670_REG_

//Address | Register Name | Default | R/W
//  70 SCALING_ XSC 4A RW
//Description:
//
//
//Bit[7]: Test_pattern[0] - works with test_pattern[1] test_pattern
//
//(SCALING_XSC[7], SCALING_YSC[7]):
//
//00: No test output
//
//01: Shifting "1"
//
//10: 8-bar color bar
//
//11: Fade to gray color bar
//
//Bit[6:0]: Horizontal scale factor
//
#define OV7670_REG_

//Address | Register Name | Default | R/W
//  71 SCALING_ YSC 35 RW
//Description:
//
//
//Bit[7]: Test_pattern[1] - works with test_pattern[0] test_pattern
//
//(SCALING_XSC[7], SCALING_YSC[7])::
//
//00: No test output
//
//01: Shifting "1"
//
//10: 8-bar color bar
//
//11: Fade to gray color bar
//
//Bit[6:0]: Vertical scale factor
//
#define OV7670_REG_

//Address | Register Name | Default | R/W
//  72 SCALING_
//
//DCWCTR 11 RW
//Description:
//
//
//DCW Control
//
//Bit[7:0]: DCW control parameter
//
#define OV7670_REG_

//Address | Register Name | Default | R/W
//  73 SCALING_
//
//PCLK_DIV 00 RW
//Description:
//
//
//Bit[7:4]: Reserved
//
//Bit[3:0]: Clock divider control for DSP scale control (valid only
//
//when COM14[3] = 1). Should change with COM14[2:0].
//
//0000: Divided by 1
//
//0001: Divided by 2
//
//0010: Divided by 4
//
//0011: Divided by 8
//
//0100: Divided by 16
//
//0101~1111: Not allowed
//
#define OV7670_REG_

//Address | Register Name | Default | R/W
//  74 REG74 00 RW
//Description:
//
//
//Bit[7:5]: Reserved
//
//Bit[4]: DG_Manu
//
//0: Digital gain control by VREF[7:6]
//
//1: Digital gain control by REG74[1:0]
//
//Bit[3:2]: Reserved
//
//Bit[1:0]: Digital gain manual control
//
//00: Bypass
//
//01: 1x
//
//10: 2x
//
//11: 4x
//
#define OV7670_REG_

//Address | Register Name | Default | R/W
//  75 REG75 0F RW
//Description:
//
//
//Register 75
//
//Bit[7:5]: Reserved
//
//Bit[4:0]: Edge enhancement lower limit
//
#define OV7670_REG_

//Address | Register Name | Default | R/W
//  76 REG76 01 RW
//Description:
//
//
//Register 76
//
//Bit[7]: Reserved
//
//Bit[6]: White pixel correction enable
//
//0: Disable
//
//1: Enable
//
//Bit[5]: Black pixel correction enable
//
//0: Disable
//
//1: Enable
//
//Bit[4:0]: Edge enhancement higher limit
//
#define OV7670_REG_

//Address | Register Name | Default | R/W
//  77 REG77 10 RW
//Description:
// Register 77
//
//Bit[7:0]: Offset, de-noise range control
//
#define OV7670_REG_

//Address | Register Name | Default | R/W
//  78-79 RSVD XX – Reserved
//
#define OV7670_REG_

//Address | Register Name | Default | R/W
//  7A GAM1 02 RW
//Description:
// Gamma Curve 1st Segment Input End Point 0x010 Output Value
//
#define OV7670_REG_

//Address | Register Name | Default | R/W
//  7B GAM2 07 RW
//Description:
// Gamma Curve 2nd Segment Input End Point 0x020 Output Value
//
#define OV7670_REG_

//Address | Register Name | Default | R/W
//  7C GAM3 1F RW
//Description:
// Gamma Curve 3rd Segment Input End Point 0x040 Output Value
//
#define OV7670_REG_

//Address | Register Name | Default | R/W
//  7D GAM4 49 RW
//Description:
// Gamma Curve 4th Segment Input End Point 0x080 Output Value
//
#define OV7670_REG_

//Address | Register Name | Default | R/W
//  7E GAM5 5A RW
//Description:
// Gamma Curve 5th Segment Input End Point 0x0A0 Output Value
//
#define OV7670_REG_

//Address | Register Name | Default | R/W
//  7F GAM6 6A RW
//Description:
// Gamma Curve 6th Segment Input End Point 0x0C0 Output Value
//
#define OV7670_REG_

//Address | Register Name | Default | R/W
//  80 GAM7 79 RW
//Description:
// Gamma Curve 7th Segment Input End Point 0x0E0 Output Value
//
#define OV7670_REG_

//Address | Register Name | Default | R/W
//  89 GAM8 87 RW
//Description:
// Gamma Curve 8th Segment Input End Point 0x100 Output Value
//
#define OV7670_REG_

//Address | Register Name | Default | R/W
//  89 GAM9 94 RW
//Description:
// Gamma Curve 9th Segment Input End Point 0x120 Output Value
//
#define OV7670_REG_

//Address | Register Name | Default | R/W
//  89 GAM10 9F RW
//Description:
// Gamma Curve 10th Segment Input End Point 0x140 Output Value
//
#define OV7670_REG_

//Address | Register Name | Default | R/W
//  89 GAM11 AF RW
//Description:
// Gamma Curve 11th Segment Input End Point 0x180 Output Value
//
#define OV7670_REG_

//Address | Register Name | Default | R/W
//  86 GAM12 BB RW
//Description:
// Gamma Curve 12th Segment Input End Point 0x1C0 Output Value
//
#define OV7670_REG_

//Address | Register Name | Default | R/W
//  87 GAM13 CF RW
//Description:
// Gamma Curve 13th Segment Input End Point 0x240 Output Value
//
#define OV7670_REG_

//Address | Register Name | Default | R/W
//  88        GAM14           EE      RW
//Description:
//Gamma Curve 14th Segment Input End Point 0x2C0 Output Value
//
#define OV7670_REG_

//Address | Register Name | Default | R/W
//  89        GAM15           EE      RW
//Description:
//Gamma Curve 15th Segment Input End Point 0x340 Output Value
//
#define OV7670_REG_

//Address | Register Name | Default | R/W
//  8A-91     RSVD            XX – Reserved
//
#define OV7670_REG_

//Address | Register Name | Default | R/W
//  92        DM_LNL          00      RW
//Description:
//Dummy Line low 8 bits
//
#define OV7670_REG_

//Address | Register Name | Default | R/W
//  93        DM_LNH          00      RW
//Description:
//Dummy Line high 8 bits
//
#define OV7670_REG_

//Address | Register Name | Default | R/W
//  94        LCC6            50      RW
//Description:
//Lens Correction Option 6 (effective only when LCC5[2] is high)
//
#define OV7670_REG_

//Address | Register Name | Default | R/W
//  95        LCC7            50      RW
//Description:
//Lens Correction Option 7 (effective only when LCC5[2] is high)
//
#define OV7670_REG_

//Address | Register Name | Default | R/W
//  96-9C     RSVD            XX – Reserved
//
#define OV7670_REG_

//Address | Register Name | Default | R/W
//  9D        BD50ST          99      RW
//Description:
//50 Hz Banding Filter Value
//(effective only when COM8[5] is high and COM11[3] is high)
//
#define OV7670_REG_

//Address | Register Name | Default | R/W
//  9E        BD60ST          7F      RW
//Description:
//  60 Hz Banding Filter Value
//(effective only when COM8[5] is high and COM11[3] is low)
//
#define OV7670_REG_

//Address | Register Name | Default | R/W
//  9F-AB                     XX
//Description:
//Histogram-based AEC/AGC Control
//
#define OV7670_REG_

//Address | Register Name | Default | R/W
//  AC        STR-OPT         00      RW
//Description:
//Register AC
//Bit[7]: Strobe enable
//Bit[6]: R / G / B gain controlled by STR_R (0xAD) / STR_G
//        (0xAE) / STR_B (0xAF) for LED output frame
//Bit[5:4]: Xenon mode option
//          00: 1 line
//          01: 2 lines
//          10: 3 lines
//          11: 4 lines
//Bit[3:2]: Reserved
//Bit[1:0]: Mode select
//          00: Xenon
//          01: LED 1&2
//          1x: LED 3
//
#define OV7670_REG_

//Address | Register Name | Default | R/W
//  AD        STR_R           80      RW
//Description:
//R Gain for LED Output Frame
//
#define OV7670_REG_

//Address | Register Name | Default | R/W
//  AE        STR_G           80      RW
//Description:
//G Gain for LED Output Frame
//
#define OV7670_REG_

//Address | Register Name | Default | R/W
//  AF        STR_B           80      RW
////Description:
//B Gain for LED Output Frame
//
#define OV7670_REG_

//Address | Register Name | Default | R/W
//  B0-B2     RSVD            XX – Reserved
//
#define OV7670_REG_

//Address | Register Name | Default | R/W
//  B3        THL_ST          80      RW
//Description:
//Digital BLC Target
//
#define OV7670_REG_

//Address | Register Name | Default | R/W
//  B4        RSVD            XX – Reserved
//
#define OV7670_REG_

//Address | Register Name | Default | R/W
//  B5        THL_DLT         04      RW
//Description:
//Digital BLC Stable Range
//
#define OV7670_REG_

//Address | Register Name | Default | R/W
//  B6-BD     RSVD            XX – Reserved
//
#define OV7670_REG_

//Address | Register Name | Default | R/W
//  BE        AD-CHB          00      RW
//Description:
//Bit[7]: Reserved
//Bit[6]: Sign bit
//Bit[5:0]: ADC offset value
//
#define OV7670_REG_

//Address | Register Name | Default | R/W
//  BF        AD-CHR          00      RW
//Description:
//Bit[7]: Reserved
//Bit[6]: Sign bit
//Bit[5:0]: ADC offset value
//
#define OV7670_REG_

//Address | Register Name | Default | R/W
//  C0        AD-CHGb         00      RW
//Description:
//Bit[7]: Reserved
//Bit[6]: Sign bit
//Bit[5:0]: ADC offset value
//
#define OV7670_REG_

//Address | Register Name | Default | R/W
//  C1        AD-CHGr         00      RW
//Description:
//Bit[7]: Reserved
//Bit[6]: Sign bit
//Bit[5:0]: ADC offset value
//
#define OV7670_REG_

//Address | Register Name | Default | R/W
//  C2-C8     RSVD            XX – Reserved
//
#define OV7670_REG_

//Address | Register Name | Default | R/W
//  C9        SATCTR          C0      RW
//Description:
//Saturation Control
//Bit[7:4]: UV saturation control min
//Bit[3:0]: UV saturation control result

#endif /* __CAMERA_OV7670_REG_H_ */
