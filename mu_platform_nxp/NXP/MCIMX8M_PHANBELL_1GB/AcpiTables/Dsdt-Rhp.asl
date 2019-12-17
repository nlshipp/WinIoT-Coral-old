/** @file
* Description: NXP iMX8MQ EVK Resource Hub Proxy
*
*  Copyright (c) 2018, Microsoft Corporation. All rights reserved.
*
*  This program and the accompanying materials
*  are licensed and made available under the terms and conditions of the BSD License
*  which accompanies this distribution.  The full text of the license may be found at
*  http://opensource.org/licenses/bsd-license.php
*
*  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
*  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
*
*/

Device(RHPX)
{
  Name(_HID, "MSFT8000")
  Name(_CID, "MSFT8000")
  Name(_UID, 1)

  Name(_CRS, ResourceTemplate()
  {
    // Index 0
    I2CSerialBus(0xFFFF,, 0,, "\\_SB.I2C1",,,,)

    // Index 1
    I2CSerialBus(0xFFFF,, 0,, "\\_SB.I2C2",,,,)

    // Index 2
    I2CSerialBus(0xFFFF,, 0,, "\\_SB.I2C3",,,,)

    // Index 3
    I2CSerialBus(0xFFFF,, 0,, "\\_SB.I2C4",,,,)

    // Index 2
//    SPISerialBus (          // SCLK
//                            // MOSI
//                            // MISO
//                            // SS0
//      0,                    // Device selection (SS0)
//      PolarityLow,          // Device selection polarity
//      FourWireMode,         // wiremode
//      8,                    // databit len
//      ControllerInitiated,  // slave mode
//      4000000,              // connection speed
//      ClockPolarityLow,     // clock polarity
//      ClockPhaseFirst,      // clock phase
//      "\\_SB.SPI1",         // ResourceSource: SPI bus controller name
//      0,                    // ResourceSourceIndex
//                            // Resource usage
//                            // DescriptorName: creates name for offset of resource descriptor
//    )                       // Vendor Data
//
//    // Index 3
//    SPISerialBus (          // SCLK
//                            // MOSI
//                            // MISO
//                            // SS1
//      1,                    // Device selection (SS1)
//      PolarityLow,          // Device selection polarity
//      FourWireMode,         // wiremode
//      8,                    // databit len
//      ControllerInitiated,  // slave mode
//      4000000,              // connection speed
//      ClockPolarityLow,     // clock polarity
//      ClockPhaseFirst,      // clock phase
//      "\\_SB.SPI1",         // ResourceSource: SPI bus controller name
//      0,                    // ResourceSourceIndex
//                            // Resource usage
//                            // DescriptorName: creates name for offset of resource descriptor
//    )                       // Vendor Data

//    // GPIO1_IO01 PAD_GPIO1_IO01 (PWM1) - J15 pin 32
//    GpioIO(Shared, PullNone, 0, 0, IoRestrictionNone, "\\_SB.GPIO", 0, ResourceConsumer, , ) { 1 } // 0 * 32 + 1
//    GpioInt(Edge, ActiveBoth, Shared, PullNone, 0, "\\_SB.GPIO",) { 1 }
//
//    // GPIO1_IO06 PAD_GPIO_06 - J15 pin 13
//    GpioIO(Shared, PullNone, 0, 0, IoRestrictionNone, "\\_SB.GPIO", 0, ResourceConsumer, , ) { 6 } // 0 * 32 + 6
//    GpioInt(Edge, ActiveBoth, Shared, PullNone, 0, "\\_SB.GPIO",) { 6 }
//
//    // GPIO1_IO07 PAD_GPIO1_IO07 - J15 pin 29
//    GpioIO(Shared, PullNone, 0, 0, IoRestrictionNone, "\\_SB.GPIO", 0, ResourceConsumer, , ) { 7 } // 0 * 32 + 7
//    GpioInt(Edge, ActiveBoth, Shared, PullNone, 0, "\\_SB.GPIO",) { 7 }
//
//    // GPIO1_IO08 PAD_GPIO1_IO08 - J15 pin 31
//    GpioIO(Shared, PullNone, 0, 0, IoRestrictionNone, "\\_SB.GPIO", 0, ResourceConsumer, , ) { 8 } // 0 * 32 + 8
//    GpioInt(Edge, ActiveBoth, Shared, PullNone, 0, "\\_SB.GPIO",) { 8 }
//
//    // GPIO1_IO12 PAD_GPIO1_IO12 - P1 pin23 (MIPI cam1-resetb)
//    GpioIO(Shared, PullNone, 0, 0, IoRestrictionNone, "\\_SB.GPIO", 0, ResourceConsumer, , ) { 12 } // 0 * 32 + 12
//    GpioInt(Edge, ActiveBoth, Shared, PullNone, 0, "\\_SB.GPIO",) { 12 }
//
//    // GPIO1_IO13 PAD_GPIO1_IO13 (PWM2) - J15 pin 33
//    GpioIO(Shared, PullNone, 0, 0, IoRestrictionNone, "\\_SB.GPIO", 0, ResourceConsumer, , ) { 13 } // 0 * 32 + 13
//    GpioInt(Edge, ActiveBoth, Shared, PullNone, 0, "\\_SB.GPIO",) { 13 }
//
//    // GPIO1_IO14 PAD_GPIO1_IO14 (PWM3) - J15 pin 15
//    GpioIO(Shared, PullNone, 0, 0, IoRestrictionNone, "\\_SB.GPIO", 0, ResourceConsumer, , ) { 14 } // 0 * 32 + 14
//    GpioInt(Edge, ActiveBoth, Shared, PullNone, 0, "\\_SB.GPIO",) { 14 }
//
//    // GPIO3_IO02 PAD_NAND_CE1_B (ECSPI1_SS1) - J15 pin 26
//    GpioIO(Shared, PullNone, 0, 0, IoRestrictionNone, "\\_SB.GPIO", 0, ResourceConsumer, , ) { 66 } // 2 * 32 + 2
//    GpioInt(Edge, ActiveBoth, Shared, PullNone, 0, "\\_SB.GPIO",) { 66 }
//
    // GPIO3_IO05 PAD_NAND_CLE - J6 pin1 (fan control)
    GpioIO(Shared, PullNone, 0, 0, IoRestrictionNone, "\\_SB.GPIO", 0, ResourceConsumer, , ) { 69 } // 2 * 32 + 5
    GpioInt(Edge, ActiveBoth, Shared, PullNone, 0, "\\_SB.GPIO",) { 69 }
//
//    // GPIO3_IO08 PAD_NAND_DATA2 - P1 pin17 (MIPI cam1-pwrdn)
//    GpioIO(Shared, PullNone, 0, 0, IoRestrictionNone, "\\_SB.GPIO", 0, ResourceConsumer, , ) { 72 } // 2 * 32 + 8
//    GpioInt(Edge, ActiveBoth, Shared, PullNone, 0, "\\_SB.GPIO",) { 72 }
//
//    // GPIO3_IO09 PAD_NAND_DATA03 - J15 pin 16
//    GpioIO(Shared, PullNone, 0, 0, IoRestrictionNone, "\\_SB.GPIO", 0, ResourceConsumer, , ) { 73 } // 2 * 32 + 9
//    GpioInt(Edge, ActiveBoth, Shared, PullNone, 0, "\\_SB.GPIO",) { 73 }
//
//    // GPIO3_IO13 PAD_NAND_DATA07 - J15 pin 37
//    GpioIO(Shared, PullNone, 0, 0, IoRestrictionNone, "\\_SB.GPIO", 0, ResourceConsumer, , ) { 77 } // 2 * 32 + 13
//    GpioInt(Edge, ActiveBoth, Shared, PullNone, 0, "\\_SB.GPIO",) { 77 }
//
//    // GPIO4_IO02 PAD_SAI1_RXD0 - J15 pin 38
//    GpioIO(Shared, PullNone, 0, 0, IoRestrictionNone, "\\_SB.GPIO", 0, ResourceConsumer, , ) { 98 } // 3 * 32 + 2
//    GpioInt(Edge, ActiveBoth, Shared, PullNone, 0, "\\_SB.GPIO",) { 98 }
//
//    // GPIO4_IO10 PAD_SAI1_TXFS - J15 pin 35
//    GpioIO(Shared, PullNone, 0, 0, IoRestrictionNone, "\\_SB.GPIO", 0, ResourceConsumer, , ) { 106 } // 3 * 32 + 10
//    GpioInt(Edge, ActiveBoth, Shared, PullNone, 0, "\\_SB.GPIO",) { 106 }
//
//    // GPIO4_IO11 PAD_SAI1_TXC - J15 pin 12
//    GpioIO(Shared, PullNone, 0, 0, IoRestrictionNone, "\\_SB.GPIO", 0, ResourceConsumer, , ) { 107 } // 3 * 32 + 11
//    GpioInt(Edge, ActiveBoth, Shared, PullNone, 0, "\\_SB.GPIO",) { 107 }
//
//    // GPIO4_IO12 PAD_SAI1_TXD0 - J15 pin 40
//    GpioIO(Shared, PullNone, 0, 0, IoRestrictionNone, "\\_SB.GPIO", 0, ResourceConsumer, , ) { 108 } // 3 * 32 + 12
//    GpioInt(Edge, ActiveBoth, Shared, PullNone, 0, "\\_SB.GPIO",) { 108 }

//    // GPIO5_IO06 PAD_ECSPI1_SCLK - J15 pin 23
//    GpioIO(Shared, PullNone, 0, 0, IoRestrictionNone, "\\_SB.GPIO", 0, ResourceConsumer, , ) { 134 } // 4 * 32 + 6
//    GpioInt(Edge, ActiveBoth, Shared, PullNone, 0, "\\_SB.GPIO",) { 134 }
//
//    // GPIO5_IO07 PAD_ECSPI1_MOSI - J15 pin 19
//    GpioIO(Shared, PullNone, 0, 0, IoRestrictionNone, "\\_SB.GPIO", 0, ResourceConsumer, , ) { 135 } // 4 * 32 + 7
//    GpioInt(Edge, ActiveBoth, Shared, PullNone, 0, "\\_SB.GPIO",) { 135 }
//
//    // GPIO5_IO08 PAD_ECSPI1_MISO - J15 pin 21
//    GpioIO(Shared, PullNone, 0, 0, IoRestrictionNone, "\\_SB.GPIO", 0, ResourceConsumer, , ) { 136 } // 4 * 32 + 8
//    GpioInt(Edge, ActiveBoth, Shared, PullNone, 0, "\\_SB.GPIO",) { 136 }
//
//    // GPIO5_IO09 PAD_ECSPI1_SS0 - J15 pin 24
//    GpioIO(Shared, PullNone, 0, 0, IoRestrictionNone, "\\_SB.GPIO", 0, ResourceConsumer, , ) { 137 } // 4 * 32 + 9
//    GpioInt(Edge, ActiveBoth, Shared, PullNone, 0, "\\_SB.GPIO",) { 137 }

//    // GPIO5_IO10 PAD_ECSPI2_SCLK - J15 pin 18
//    GpioIO(Shared, PullNone, 0, 0, IoRestrictionNone, "\\_SB.GPIO", 0, ResourceConsumer, , ) { 138 } // 4 * 32 + 10
//    GpioInt(Edge, ActiveBoth, Shared, PullNone, 0, "\\_SB.GPIO",) { 138 }
//
//    // GPIO5_IO12 PAD_ECSPI2_MISO - J15 pin 22
//    GpioIO(Shared, PullNone, 0, 0, IoRestrictionNone, "\\_SB.GPIO", 0, ResourceConsumer, , ) { 140 } // 4 * 32 + 12
//    GpioInt(Edge, ActiveBoth, Shared, PullNone, 0, "\\_SB.GPIO",) { 140 }
//
//    // GPIO5_IO13 PAD_ECSPI2_SS0 - J15 pin 36
//    GpioIO(Shared, PullNone, 0, 0, IoRestrictionNone, "\\_SB.GPIO", 0, ResourceConsumer, , ) { 141 } // 4 * 32 + 13
//    GpioInt(Edge, ActiveBoth, Shared, PullNone, 0, "\\_SB.GPIO",) { 141 }
//
//    // GPIO5_IO16 PAD_I2C2_SCL - J15 pin 5
//    GpioIO(Shared, PullNone, 0, 0, IoRestrictionNone, "\\_SB.GPIO", 0, ResourceConsumer, , ) { 144 } // 4 * 32 + 16
//    GpioInt(Edge, ActiveBoth, Shared, PullNone, 0, "\\_SB.GPIO",) { 144 }
//
//    // GPIO5_IO17 PAD_I2C2_SDA - J15 pin 3
//    GpioIO(Shared, PullNone, 0, 0, IoRestrictionNone, "\\_SB.GPIO", 0, ResourceConsumer, , ) { 145 } // 4 * 32 + 17
//    GpioInt(Edge, ActiveBoth, Shared, PullNone, 0, "\\_SB.GPIO",) { 145 }
//
//    // GPIO5_IO18 PAD_I2C3_SCL - J15 pin 28
//    GpioIO(Shared, PullNone, 0, 0, IoRestrictionNone, "\\_SB.GPIO", 0, ResourceConsumer, , ) { 146 } // 4 * 32 + 18
//    GpioInt(Edge, ActiveBoth, Shared, PullNone, 0, "\\_SB.GPIO",) { 146 }
//
//    // GPIO5_IO19 PAD_I2C3_SDA - J15 pin 27
//    GpioIO(Shared, PullNone, 0, 0, IoRestrictionNone, "\\_SB.GPIO", 0, ResourceConsumer, , ) { 147 } // 4 * 32 + 19
//    GpioInt(Edge, ActiveBoth, Shared, PullNone, 0, "\\_SB.GPIO",) { 147 }
//
//    // GPIO5_IO22 PAD_UART_RXD - J15 pin 10
//    GpioIO(Shared, PullNone, 0, 0, IoRestrictionNone, "\\_SB.GPIO", 0, ResourceConsumer, , ) { 150 } // 4 * 32 + 22
//    GpioInt(Edge, ActiveBoth, Shared, PullNone, 0, "\\_SB.GPIO",) { 150 }
//
//    // GPIO5_IO23 PAD_UART1_TXD - J15 pin 8
//    GpioIO(Shared, PullNone, 0, 0, IoRestrictionNone, "\\_SB.GPIO", 0, ResourceConsumer, , ) { 151 } // 4 * 32 + 23
//    GpioInt(Edge, ActiveBoth, Shared, PullNone, 0, "\\_SB.GPIO",) { 151 }
//
//    // GPIO5_IO26 PAD_UART3_RXD - J15 pin 11
//    GpioIO(Shared, PullNone, 0, 0, IoRestrictionNone, "\\_SB.GPIO", 0, ResourceConsumer, , ) { 154 } // 4 * 32 + 26
//    GpioInt(Edge, ActiveBoth, Shared, PullNone, 0, "\\_SB.GPIO",) { 154 }
//
//    // GPIO5_IO27 PAD_UART3_TXD - J15 pin 7
//    GpioIO(Shared, PullNone, 0, 0, IoRestrictionNone, "\\_SB.GPIO", 0, ResourceConsumer, , ) { 155 } // 4 * 32 + 27
//    GpioInt(Edge, ActiveBoth, Shared, PullNone, 0, "\\_SB.GPIO",) { 155 }
//
  })

  Name(_DSD, Package()
  {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {
      // I2C busses 1-4
      Package(2) { "bus-I2C-I2C1", Package() { 0 }},    // index 0
      Package(2) { "bus-I2C-I2C2", Package() { 1 }},    // index 1
      Package(2) { "bus-I2C-I2C3", Package() { 2 }},    // index 2
      Package(2) { "bus-I2C-I2C4", Package() { 3 }},    // index 3

      // SPI 1
      Package(2) { "bus-SPI-SPI1", Package() { 4, 5 }},                    // Indices 4 and 5
      Package(2) { "SPI1-MinClockInHz", 46 },                              // 46 Hz
      Package(2) { "SPI1-MaxClockInHz", 12000000 },                        // 12 MHz
      Package(2) { "SPI1-SupportedDataBitLengths", Package() { 8,16,32 }}, // Data bit length
      
      // GPIO Pin Count and supported drive modes
      Package (2) { "GPIO-PinCount", 157 },
      Package (2) { "GPIO-UseDescriptorPinNumbers", 1 },

      // InputHighImpedance, InputPullUp, OutputCmos
      Package (2) { "GPIO-SupportedDriveModes", 0x0b },
    }
  })
}
