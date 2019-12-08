Known Issues/Limitations
====

Read through all hardware-related reference material and ensure that you have made all the necessary hardware modifications before using the software.

The imx-iotcore reference BSP has the following limitations:
 - SD/eMMC is the only supported boot media


Table: Common known issues and workarounds for i.MX Family SoC

| SoC            | Module  | Source      | Description                   | Workaround                 |
|:---------------|:--------|:------------|:------------------------------|:---------------------------|
| All SoC                                | USB      | Software    | Only USB HOST mode is supported.                                                                                                                | No wokaround                                 |
| i.MX 6UL, 6ULL, 6SX, 6DL               | PEP      | Software    | PEP driver is not implemented, the power management may not be fully functional.                                                                | No wokaround                                 |
| i.MX 6SX, 6UL, 6ULL                    | IMXDOD  | Software    | Display backlight is not turned off after switch to PowerDeviceD3 mode (parallel LCD display should be "freezed", LCD signals stop to generate) | No wokaround                                 |
| i.MX 7D                                | IMXDOD  | Software    | Display backlight is not turned off after switch to PowerDeviceD3 mode (parallel LCD display should be "freezed", LCD signals stop to generate) | No wokaround                                 |
| i.MX 8MM                               | Display  | Software    | Display driver supports 1920 x 1080 60 Hz resolution only.                                                                                                  | No wokaround                                 |
| i.MX 8MQ                               | Display  | Software    | Display driver supports 1280 x 720 60 Hz resolution only.                                                                                                   | No wokaround                                 |
| i.MX8                                  | eMMC Boot | Software   | eMMC boot on ARM64. flash.bin is not deployed from WinPE onto eMMC device thus execution of boolotader from SD card is still needed when booting from eMMC. | No wokaround                                 |
| i.MX8                                  | USB | Software | USB Type-C port driver is not implemented yet. Port properties are detected/configured only in UBOOT and are not enumerated when OS is running. | USB cable must be plugged before UBOOT is running. |
