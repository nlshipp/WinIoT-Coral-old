/** @file
*
*  Copyright (c) 2018 Microsoft Corporation. All rights reserved.
*  Copyright 2019 NXP
*
*  This program and the accompanying materials
*  are licensed and made available under the terms and conditions of the BSD License
*  which accompanies this distribution.  The full text of the license may be found at
*  http://opensource.org/licenses/bsd-license.php
*
*  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
*  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
*
**/

Device (PWM1)
{
  Name (_HID, "NXP010E")
  Name (_UID, 0x1)

  Method (_STA) {
    Return (0xf)
  }

  Name (_CRS, ResourceTemplate () {
    MEMORY32FIXED (ReadWrite, 0x30660000, 0x10000, )
    Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive) { 113 }
  })

  Name (_DSD, Package () {
    ToUUID ("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
      Package () {
        Package (2) {"Pwm-SchematicName", "PWM1"}
      }
  })
}

Device (PWM2)
{
  Name (_HID, "NXP010E")
  Name (_UID, 0x2)

  Method (_STA) {
    Return (0xf)
  }

  Name (_CRS, ResourceTemplate () {
    MEMORY32FIXED (ReadWrite, 0x30670000, 0x10000, )
    Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive) { 114 }
  })

  Name (_DSD, Package () {
    ToUUID ("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
      Package () {
        Package (2) {"Pwm-SchematicName", "PWM2"}
      }
  })
}

Device (PWM3)
{
  Name (_HID, "NXP010E")
  Name (_UID, 0x3)

  Method (_STA) {
    Return (0xf)
  }

  Name (_CRS, ResourceTemplate () {
    MEMORY32FIXED (ReadWrite, 0x30680000, 0x10000, )
    Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive) { 115 }
  })

  Name (_DSD, Package () {
    ToUUID ("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
      Package () {
        Package (2) {"Pwm-SchematicName", "PWM3"}
      }
  })
}

Device (PWM4)
{
  Name (_HID, "NXP010E")
  Name (_UID, 0x4)

  Method (_STA) {
    Return (0xf)
  }

  Name (_CRS, ResourceTemplate () {
    MEMORY32FIXED (ReadWrite, 0x30690000, 0x10000, )
    Interrupt (ResourceConsumer, Level, ActiveHigh, Exclusive) { 116 }
  })

  Name (_DSD, Package () {
    ToUUID ("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
      Package () {
        Package (2) {"Pwm-SchematicName", "PWM4"}
      }
  })
}
