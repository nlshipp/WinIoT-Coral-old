Building and Updating ARM64 Firmware {#build-arm64-firmware}
==============

This chapter describes the process of setting up a build-environment to build the latest firmware, update the firmware on the SD Card for testing and development, and include the new firmware in the FFU builds.

Note: The UEFI build environment has changed for 1903 and any existing build environment must be updated.

## Setting up your build environment

1) Set up a Linux environment.
    * Dedicated Linux system
    * Linux Virtual Machine
    * Windows Subsystem for Linux ([WSL setup instructions](https://docs.microsoft.com/en-us/windows/wsl/install-win10))
    * Note: We validate with both Ubuntu in WSL and standalone Ubuntu machines.

2) Update and install build tools

    ```bash
    sudo apt-get update
    sudo apt-get upgrade
    sudo apt-get install attr build-essential python python-dev python-crypto python-wand device-tree-compiler bison flex swig iasl uuid-dev wget git bc libssl-dev zlib1g-dev python3-pip
    *** new for 1903 UEFI
    sudo apt-get install gcc g++ make python3 mono-devel
    ***
    pushd ~
    wget https://releases.linaro.org/components/toolchain/binaries/7.2-2017.11/aarch64-linux-gnu/gcc-linaro-7.2.1-2017.11-x86_64_aarch64-linux-gnu.tar.xz
    tar xf gcc-linaro-7.2.1-2017.11-x86_64_aarch64-linux-gnu.tar.xz
    rm gcc-linaro-7.2.1-2017.11-x86_64_aarch64-linux-gnu.tar.xz
    popd
    ```

3) At this point your directory structure should look like the following

    ```
    - %WORKSPACE%
       |- cst
       |- edk2
       |- firmware-imx-8.1
       |- imx-atf
       |- imx-edk2-platforms
       |- imx-iotcore
       |- imx-mkimage
       |- mu_platform_nxp
       |- optee_os
       |- RIoT
       |- u-boots
      ```

4) Build firmware to test the setup. Adding "-j 20" to make will parallelize the build and speed it up significantly on WSL, but since the firmwares build in parallel it will be more difficult to diagnose any build failures. You can customize the number to work best with your system.

    ```bash
   # U-Boot
   export CROSS_COMPILE=~/gcc-linaro-7.2.1-2017.11-x86_64_aarch64-linux-gnu/bin/aarch64-linux-gnu-
   export ARCH=arm64

   pushd u-boots/u-boot-imx_arm64

   make imx8mq_evk_nt_defconfig
   ---or---
   make imx8mm_evk_nt_defconfig

   make
   popd


   # Arm Trusted Firmware

   export CROSS_COMPILE=~/gcc-linaro-7.2.1-2017.11-x86_64_aarch64-linux-gnu/bin/aarch64-linux-gnu-
   export ARCH=arm64

   pushd imx-atf
   make PLAT=imx8mq SPD=opteed bl31
   ---or---
   make PLAT=imx8mm SPD=opteed bl31
   popd

   # OP-TEE OS

   export -n CROSS_COMPILE
   export -n ARCH
   export CROSS_COMPILE64=~/gcc-linaro-7.2.1-2017.11-x86_64_aarch64-linux-gnu/bin/aarch64-linux-gnu-
   pushd optee_os/optee_os_arm64

   make PLATFORM=imx PLATFORM_FLAVOR=mx8mqevk \
     CFG_TEE_CORE_DEBUG=n CFG_TEE_CORE_LOG_LEVEL=2 \
     CFG_RPMB_FS=y CFG_RPMB_TESTKEY=y CFG_RPMB_WRITE_KEY=y CFG_REE_FS=n  \
     CFG_IMXCRYPT=y CFG_CORE_HEAP_SIZE=98304
   ---or---
   make PLATFORM=imx PLATFORM_FLAVOR=mx8mmevk \
     CFG_TEE_CORE_DEBUG=n CFG_TEE_CORE_LOG_LEVEL=2 \
     CFG_RPMB_FS=y CFG_RPMB_TESTKEY=y CFG_RPMB_WRITE_KEY=y CFG_REE_FS=n  \
     CFG_IMXCRYPT=y CFG_CORE_HEAP_SIZE=98304

   # debug
   # make PLATFORM=imx PLATFORM_FLAVOR=mx8mqevk \
   #   CFG_TEE_CORE_DEBUG=y CFG_TEE_CORE_LOG_LEVEL=3 \
   #   CFG_RPMB_FS=y CFG_RPMB_TESTKEY=y CFG_RPMB_WRITE_KEY=y CFG_REE_FS=n \
   #   CFG_TA_DEBUG=y CFG_TEE_CORE_TA_TRACE=1 CFG_TEE_TA_LOG_LEVEL=2 \
   #   CFG_IMXCRYPT=y CFG_CORE_HEAP_SIZE=98304
   # ---or---
   # make PLATFORM=imx PLATFORM_FLAVOR=mx8mmevk \
   #   CFG_TEE_CORE_DEBUG=y CFG_TEE_CORE_LOG_LEVEL=3 \
   #   CFG_RPMB_FS=y CFG_RPMB_TESTKEY=y CFG_RPMB_WRITE_KEY=y CFG_REE_FS=n \
   #   CFG_TA_DEBUG=y CFG_TEE_CORE_TA_TRACE=1 CFG_TEE_TA_LOG_LEVEL=2 \
   #   CFG_IMXCRYPT=y CFG_CORE_HEAP_SIZE=98304

   ${CROSS_COMPILE64}objcopy -O binary ./out/arm-plat-imx/core/tee.elf ./out/arm-plat-imx/tee.bin
   popd

   # OP-TEE Trusted Applications

   export TA_DEV_KIT_DIR=../../../../optee_os/out/arm-plat-imx/export-ta_arm64
   export TA_CROSS_COMPILE=~/gcc-linaro-7.2.1-2017.11-x86_64_aarch64-linux-gnu/bin/aarch64-linux-gnu-
   export TA_CPU=cortex-a53

   # imx-mkimage

   export CROSS_COMPILE=~/gcc-linaro-7.2.1-2017.11-x86_64_aarch64-linux-gnu/bin/aarch64-linux-gnu-
   export ARCH=arm64

   pushd imx-mkimage/iMX8M

   cp ../../firmware-imx-8.1/firmware/ddr/synopsys/lpddr4_pmu_train_*.bin .
   cp ../../firmware-imx-8.1/firmware/hdmi/cadence/signed_hdmi_imx8m.bin .
   cp ../../optee_os/optee_os_arm64/out/arm-plat-imx/tee.bin .

   cp ../../imx-atf/build/imx8mq/release/bl31.bin .
   ---or---
   cp ../../imx-atf/build/imx8mm/release/bl31.bin .

   cp ../../u-boots/u-boot-imx_arm64/u-boot-nodtb.bin  .
   cp ../../u-boots/u-boot-imx_arm64/spl/u-boot-spl.bin .

   cp ../../u-boots/u-boot-imx_arm64/arch/arm/dts/fsl-imx8mq-evk.dtb .
   ---or---
   cp ../../u-boots/u-boot-imx_arm64/arch/arm/dts/fsl-imx8mm-evk.dtb .

   cp ../../u-boots/u-boot-imx_arm64/tools/mkimage .

   mv mkimage mkimage_uboot

   cd ..

   make SOC=iMX8M flash_hdmi_spl_uboot
   ---or---
   make SOC=iMX8MM flash_hdmi_spl_uboot

   popd

   # UEFI
   # note: On Windows Ubuntu, ignore Python errors during build specifically like 
   # "ERROR - Please upgrade Python! Current version is 3.6.7. Recommended minimum is 3.7."

   # setup
   pushd mu_platform_nxp
   export GCC5_AARCH64_PREFIX=~/gcc-linaro-7.2.1-2017.11-x86_64_aarch64-linux-gnu/bin/aarch64-linux-gnu-
   pip3 install -r requirements.txt --upgrade

   python3 NXP/MCIMX8M_EVK_4GB/PlatformBuild.py --setup
   # if error here about NugetDependency.global_cache_path, then make sure mono-devel package is installed
   # using apt-get as listed in "Update and install build tools" above.

   cd MU_BASECORE
   make -C BaseTools
   cd ..

   popd

   # clean
   pushd mu_platform_nxp
   rm -r Build
   rm -r Config
   popd

   # build
   pushd mu_platform_nxp


   export GCC5_AARCH64_PREFIX=~/gcc-linaro-7.2.1-2017.11-x86_64_aarch64-linux-gnu/bin/aarch64-linux-gnu-

   python3 NXP/MCIMX8M_EVK_4GB/PlatformBuild.py -V TARGET=RELEASE \
     PROFILE=DEV MAX_CONCURRENT_THREAD_NUMBER=20
   ---or---
   python3 NXP/MCIMX8M_MINI_EVK_2GB/PlatformBuild.py -V TARGET=RELEASE \
     PROFILE=DEV MAX_CONCURRENT_THREAD_NUMBER=20

   # debug
   # python3 NXP/MCIMX8M_EVK_4GB/PlatformBuild.py -V TARGET=DEBUG \
   #   PROFILE=DEV MAX_CONCURRENT_THREAD_NUMBER=20
   # ---or---
   # python3 NXP/MCIMX8M_MINI_EVK_2GB/PlatformBuild.py -V TARGET=DEBUG \
   #   PROFILE=DEV MAX_CONCURRENT_THREAD_NUMBER=20

   cd Build/MCIMX8M_EVK_4GB/RELEASE_GCC5/FV
   ---or---
   cd Build/MCIMX8M_MINI_EVK_2GB/RELEASE_GCC5/FV

   cp ../../../../../imx-iotcore/build/firmware/its/uefi_imx8_unsigned.its .
   ../../../../../u-boots/u-boot-imx_arm64/tools/mkimage -f uefi_imx8_unsigned.its -r uefi.fit

   popd
    ```
    
5) After a successful build you should have several output files:
    ```bash
    imx-mkimage/iMX8M/flash.bin - Contains SPL, ATF, OP-TEE, and U-Boot proper

    mu_platform_nxp/Build/MCIMX8M_EVK_4GB/RELEASE_GCC5/FV/uefi.fit - Contains the UEFI firmware
    ---or---
    mu_platform_nxp/Build/MCIMX8M_MINI_EVK_2GB/RELEASE_GCC5/FV/uefi.fit - Contains the UEFI firmware
    ```

## Adding updated firmware to your ARM64 FFU
1) To make the updated firmware a part of your FFU build, you must copy the firmwares to your board's Package folder in imx-iotcore.
 * Copy uefi.fit into /board/boardname/Package/BootFirmware
 * Copy flash.bin into /board/boardname/Package/BootLoader

    ```bash
    cp imx-mkimage/iMX8M/flash.bin imx-iotcore/build/board/NXPEVK_iMX8M_4GB/Package/BootLoader/flash.bin
    cp mu_platform_nxp/Build/MCIMX8M_EVK_4GB/RELEASE_GCC5/FV/uefi.fit imx-iotcore/build/board/NXPEVK_iMX8M_4GB/Package/BootFirmware/uefi.fit
    ---or---
    cp imx-mkimage/iMX8M/flash.bin imx-iotcore/build/board/NXPEVK_iMX8M_Mini_2GB/Package/BootLoader/flash.bin
    cp mu_platform_nxp/Build/MCIMX8M_MINI_EVK_2GB/RELEASE_GCC5/FV/uefi.fit imx-iotcore/build/board/NXPEVK_iMX8M_Mini_2GB/Package/BootFirmware/uefi.fit
    ```

## Deploying firmware to an SD card manually

### Deploying U-Boot, ATF, OP-TEE (flash.bin) and UEFI (uefi.fit) for development
  On Windows you can use [DD for Windows](http://www.chrysocome.net/dd) from an administrator command prompt to deploy flash.bin and uefi.fit.
  Be careful that you use the correct `of` and `seek`, DD will not ask for confirmation.

  ```bash
  powershell Get-WmiObject Win32_DiskDrive
  dd if=flash.bin of=\\.\PhysicalDrive<X> bs=512 seek=66

  dd if=uefi.fit of=\\.\PhysicalDrive<X> bs=1024 seek=2176

  ```
  Where `PhysicalDrive<X>` is the DeviceID of your SD card as shown by `Get-WmiObject`.

You might get the output: `Error reading file: 87 The parameter is incorrect`. This error can be ignored.

If you are working on a dedicated Linux machine (not WSL or VM) use:
```
dd if=flash.bin of=/dev/sdX bs=512 seek=66

dd if=uefi.fit of=/dev/sdX bs=1024 seek=2176
```
