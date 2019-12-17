/*
* Description: iMX8M Quad SPI Controllers
*
*  Copyright (c) Microsoft Corporation. All rights reserved.
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

Device (SPI1)
{
  Name (_HID, "NXP0105")
  Name (_HRV, 0x1)  // REV_0001
  Name (_UID, 0x1)
  Method (_STA)
  {
    Return(0xf)
  }
  Name (_CRS, ResourceTemplate () {
    MEMORY32FIXED(ReadWrite, 0x30820000, 0x4000, )
    Interrupt(ResourceConsumer, Level, ActiveHigh, Exclusive) { 63 }

    // SS0 (PAD_ECSPI1_SS0) - GPIO5_IO09 (137) - J15 pin 24
    GpioIO (Shared, PullUp, 0, 0, IoRestrictionNone, "\\_SB.GPIO", 0, ResourceConsumer, , ) { 137 }

    // SS1 (PAD_NAND_CE1_B) - GPIO3_IO02 (66) - J15 pin 26
    GpioIO (Shared, PullUp, 0, 0, IoRestrictionNone, "\\_SB.GPIO", 0, ResourceConsumer, , ) { 66 }

    // MISO (PAD_ECSPI1_MISO) - GPIO5_IO08 (136) - J15 pin 21
    // MOSI (PAD_ECSPI1_MOSI) - GPIO5_IO07 (135) - J15 pin 19
    // SCLK (PAD_ECSPI1_SCLK) - GPIO5_IO06 (134) - J15 pin 23
    // MsftFunctionConfig (Exclusive, PullNone, IMX_ALT0, "\\_SB.GPIO", 0,
    //                     ResourceConsumer, ) { 134, 135, 136 }
    //
    // MsftFunctionConfig (Arg0, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6) { Pin List }
    VendorLong () {
      MSFT_UUID,            // Vendor UUID (MSFT UUID)
      MSFT_FUNCTION_CONFIG, // Resource Identifier (MSFT Function Config)
      0x1f,0x00,            // Length (0xF + sizeof(PinList) + sizeof(ResourceName))
      0x01,                 // Revision (0x1)
      RESOURCECONSUMER_EXCLUSIVE, // Flags (Arg5 | Arg0: ResourceConsumer | Exclusive)
      PULL_NONE,            // Pin configuration (Arg1: PullNone)
      IMX_ALT0,0x00,        // Function Number (Arg2: IMX_ALT0)
      PIN_TABLE_OFFSET,     // Pin Table Offset (0x12)
      0x00,                 // Resource Source Index (Arg4: 0)
      0x18,0x00,            // Resource Source Name Offset (0x12 + sizeof(PinList))
      0x22,0x00,            // Vendor Data Offset (0x12 + sizeof(PinList) + sizeof(ResourceName))
      0x00,0x00,            // Vendor Data Length (sizeof(Arg6) = 0)
      0x86,0x00,0x87,0x00,0x88,0x00,  // Pin List (134, 135, 136)
      SB_GPIO               // Resource Name (Arg3: \_SB.GPIO in ASCII)
    }
  })
}

Device (SPI2)
{
  Name (_HID, "NXP0105")
  Name (_HRV, 0x1)  // REV_0001
  Name (_UID, 0x2)
  Method (_STA)
  {
    Return(0x0)
  }
  Name (_CRS, ResourceTemplate () {
    MEMORY32FIXED(ReadWrite, 0x30830000, 0x4000, )
    Interrupt(ResourceConsumer, Level, ActiveHigh, Exclusive) { 64 }

    // SS0 (PAD_ECSPI2_SS0) - GPIO5_IO13 (141) - J15 pin 36
    // MISO (PAD_ECSPI2_MISO) - GPIO5_IO12 (140) - J15 pin 22
    // MOSI (PAD_ECSPI2_MOSI) - GPIO5_IO11 (139) - not exposed
    // SCLK (PAD_ECSPI2_SCLK) - GPIO5_IO10 (138) - J15 pin 18
  })
}

Device (SPI3)
{
  Name (_HID, "NXP0105")
  Name (_HRV, 0x1)  // REV_0001
  Name (_UID, 0x3)
  Method (_STA)
  {
    Return(0x0)
  }
  Name (_CRS, ResourceTemplate () {
      MEMORY32FIXED(ReadWrite, 0x30840000, 0x4000, )
      Interrupt(ResourceConsumer, Level, ActiveHigh, Exclusive) { 65 }
  })
}

