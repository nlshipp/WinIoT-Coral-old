Booting WinPE and Flashing eMMC
==============

This chapter describes the process of booting Windows from eMMC. To boot a device from eMMC, you first need to flash a Windows image to eMMC. Since eMMC is soldered to the board, the only way to write to it is to boot a manufacturing OS on the device, then write the image from this manufacturing OS. The manufacturing OS is booted from a removable storage such as USB or SD. The tools and techniques can be adapted to other hardware designs.

For the manufacturing OS, we will use [Windows PE (WinPE)](https://docs.microsoft.com/en-us/windows-hardware/manufacture/desktop/winpe-intro), which stands for Windows Preinstallation Environment. Windows PE is a small Windows image that can boot without persistent storage, and contains tools to help install Windows such as `diskpart` and `dism`.

The high-level process we will follow is:
1. Specify the location of the bootloader.
1. Prepare an FFU to be flashed to eMMC.
1. Prepare a WinPE image, which will flash the FFU to eMMC.

## Identifying boot loader location

First, specify the location of the bootloader. i.MX6/7/8 can boot from a number of sources including eMMC/SD, NOR flash, SPI, I2C, and USB. For more information about i.MX6/7/8 early boot, see [Firmware Boot Documentation](#boot-sequence) and the "System Boot" chapter of the processor reference manual. In this chapter we will assume the initial boot device is SD.

To avoid bricking your device, we recommend putting the first stage bootloader on media that can be reprogrammed via external means if necessary, such as an SD card, SPI flash with external programing circuitry, or I2C flash with external programming circuitry. By external programming circuitry, we mean that you can write new contents to the storage device without booting the i.MX processor.

Another strategy is to place the bootloader on eMMC and have a second, read-only eMMC part containing a recovery boot image which can be selected via hardware muxing. This way, if the primary eMMC part becomes corrupted, you can press a button or connect a jumper and boot the device from the backup eMMC part, which then allows you to recover the main eMMC part.

## Preparing an FFU to be flashed to eMMC

The FFU that gets flashed to MMC does not have any special requirements. If the firmware is going to be located on a different storage device, it does not need to contain the firmware packages. Use the same FFU that gets flashed to the SD card.

## Creating the WinPE Image

Create an image that can boot from removable media, does not require persistent storage, and contains dism.exe, which is the tool that writes an FFU to storage. WinPE is designed for this purpose. To create a bootable WinPE image, we need to:
1. Inject i.MX drivers into the image.
1. Copy the WinPE image to an SD card.
1. Copy firmware to the SD card.

The script `build/tools/make-winpe.cmd` (or `build/tools/make-winpe-i.MX8.cmd` for ARM64 platfrom ) does all of the above, and can be used to set the WinPE image to flash an FFU to a storage device at boot.

Install the following software:

1. [ADK for Windows 10](https://docs.microsoft.com/en-us/windows-hardware/manufacture/desktop/download-winpe--windows-pe)
1. [Windows PE add-on for the ADK](https://docs.microsoft.com/en-us/windows-hardware/manufacture/desktop/download-winpe--windows-pe)
1. [dd for windows](http://www.chrysocome.net/dd), which must be placed on your PATH or in the current directory

First, create a WinPE image on our machine. In this example, we specify the ''/ffu'' option because we want to deploy an FFU to eMMC. You must have already built the FFU. The script must be run as administrator, and it will create files in the directory in which it runs.

```cmd
mkdir winpe
cd winpe
:: For ARM platfrom run:
make-winpe.cmd /builddir ..\imx-iotcore\build\solution\iMXPlatform\Build\FFU\bspcabs\ARM\Debug\ /firmware path\to\firmware_fit.merged /uefi path\to\uefi.fit /ffu path\to\bsp.ffu
:: For ARM64 platform run:
make-winpe-i.MX8.cmd /builddir ..\imx-iotcore\build\solution\iMXPlatform\Build\FFU\bspcabs\ARM64\Debug\ /firmware path\to\flash.bin /uefi /uefi path\to\uefi.fit /ffu path\to\bsp.ffu
```

If /ffu switch command is omitted the board will boot just into WinPE.

Then, apply the image to an SD card. Insert an SD card into your machine, then determine the physical disk number by running:

```cmd
diskpart
> list disk
> exit
```

The output will look something like this:

```
DISKPART> list disk

  Disk ###  Status         Size     Free     Dyn  Gpt
  --------  -------------  -------  -------  ---  ---
  Disk 0    Online          931 GB      0 B        *
  Disk 1    Online          931 GB      0 B
  Disk 2    Online          953 GB      0 B        *
  Disk 3    No Media           0 B      0 B
  Disk 4    No Media           0 B      0 B
  Disk 5    No Media           0 B      0 B
  Disk 6    No Media           0 B      0 B
* Disk 7    Online           14 GB      0 B
```

In this example, the physical disk number is 7.

Apply the WinPE image to the SD card:

```cmd
:: For ARM platfrom run:
make-winpe.cmd /apply 7
:: For ARM64 platform run:
make-winpe-i.MX8.cmd /apply 7
```

It will format the SD card, copy the WinPE image contents, and write the firmware to the reserved sector at the beginning of the card.

You can now insert the SD card in your board and boot. It will boot into WinPE, then flash the FFU to eMMC, then reboot. Before rebooting, it renames the `EFI` folder at the root of the SD card to `_efi`, which causes UEFI to skip the SD card when it's looking for a filesystem to boot from. It will find the `EFI` directory on eMMC instead, and boot from there. If you wish to boot into WinPE again, you can rename `_efi` back to `EFI`.

If you wish to boot directly from eMMC configure the board switched accordingly and restart the board.
