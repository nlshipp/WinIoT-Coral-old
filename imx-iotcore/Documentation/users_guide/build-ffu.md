Building Windows 10 IoT Core for NXP i.MX Processors {#build-bsp}
==============
## Building the BSP
Before you start building the BSP, you need to have an archive with BSP sources downloaded and extracted. After that, you should get the following directory structure:

    ```
    - %WORKSPACE%
       |- cst
       |- edk2
       |- imx-edk2-platforms
       |- imx-iotcore
       |- optee_os
       |- RIoT
       |- u-boots
    ```

### Required Tools
The following tools are required to build the driver packages and IoT Core FFU: Visual Studio 2017, Windows Kits (ADK/SDK/WDK), and the IoT Core OS Packages.

#### Visual Studio 2017
* Make sure that you **install Visual Studio 2017 before the WDK** so that the WDK can install a required plugin.
* Download [Visual Studio 2017](https://docs.microsoft.com/en-us/windows-hardware/drivers/other-wdk-downloads#step-1-install-visual-studio).
* During install select **Desktop development with C++**.
* During install select the following in the Individual components tab. If these options are not available try updating VS2017 to the latest release:
  * **VC++ 2017 version 15.9 v14.16 Libs for Spectre (ARM)**
  * **VC++ 2017 version 15.9 v14.16 Libs for Spectre (ARM64)**
  * **VC++ 2017 version 15.9 v14.16 Libs for Spectre (X86 and x64)**
  * **Visual C++ compilers and libraries for ARM**
  * **Visual C++ compilers and libraries for ARM64**

#### Windows Kits from Windows 10, version 1809
* **IMPORTANT: Make sure that any previous versions of the ADK and WDK have been uninstalled!**
* Install [ADK 1809](https://developer.microsoft.com/en-us/windows/hardware/windows-assessment-deployment-kit)
* Install [WDK 1809](https://developer.microsoft.com/en-us/windows/hardware/windows-driver-kit)
  * Make sure that you allow the Visual Studio Extension to install after the WDK install completes.
* If the WDK installer says it could not find the correct SDK version, install [SDK 1809](https://developer.microsoft.com/en-us/windows/downloads/windows-10-sdk)

#### IoT Core OS Packages
* Visit the [Windows IoT Core Downloads](https://www.microsoft.com/en-us/software-download/windows10IoTCore#!) page and download "Windows 10 IoT Core Packages – Windows 10 IoT Core, version 1809 (LTSC)".
* Open the iso and install ```Windows_10_IoT_Core_ARM_Packages.msi```
* Install ```Windows_10_IoT_Core_ARM64_Packages.msi``` for ARM64 builds.

### One-Time Environment Setup
Test certificates must be installed to generate driver packages on a development machine.
1. Open an Administrator Command Prompt.
2. Navigate to your BSP and into the folder `imx-iotcore\build\tools`.
3. Launch `StartBuildEnv.bat`.
4. Run `SetupCertificate.bat` to install the test certificates.

### FFU Generation

1. Launch Visual Studio 2017 as Administrator.
2. Open the solution imx-iotcore\build\solution\iMXPlatform\iMXPlatform.sln located in the path where you have extracted BSP archive.
3. Change the build type from Debug to Release. Change the build flavor from ARM to ARM64 if building for iMX8.
4. To build press Ctrl-Shift-B or choose Build -> Build Solution from menu. This will compile all driver packages then generate the FFU.
5. Depending on the speed of the build machine FFU generation may take around 10-20 minutes.
   Note: During the process of FFU creation you may error `Error: CreateUsnJournal: Unable to create USN journal, as one already exists on volume`.
   In this case it is needed to set the USN journal registry size to 1 Mb on your development PC by following command and restart PC:
   `reg add HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Control\FileSystem /v NtfsAllowUsnMinSize1Mb  /t REG_DWORD  /d 1`

6. After a successful build the new FFU will be located in `imx-iotcore\build\solution\iMXPlatform\Build\FFU\Sabre_iMX6Q_1GB\` for ARM builds and `imx-iotcore\build\solution\iMXPlatform\Build\FFU\NXPEVK_iMX8M_4GB` for ARM64 builds.
7. The FFU contains firmware components for the NXP IMX8M EVK with i.MX8M Quad Core SOM depending on build flavor. This firmware is automatically applied to the SD Card during the FFU imaging process.

#### Building the FFU for other boards
In order to build an FFU for another board you'll need to modify GenerateFFU.bat in the Build Scripts folder of the Solution Explorer. Comment out the default Sabre_iMX6Q_1GB or NXPEVK_iMX8M_4GB builds with REM and uncomment any other boards you want to build.
```bat
REM cd /d %BATCH_HOME%
REM echo "Building EVK_iMX6ULL_512MB FFU"
REM call BuildImage EVK_iMX6ULL_512MB EVK_iMX6ULL_512MB_TestOEMInput.xml

cd /d %BATCH_HOME%
echo "Building Sabre_iMX6Q_1GB FFU"
call BuildImage Sabre_iMX6Q_1GB Sabre_iMX6Q_1GB_TestOEMInput.xml
```

### Building the FFU with the IoT ADK AddonKit
1. Build the GenerateBSP project to create a BSP folder in the root of the repository.
2. Clone the [IoT ADK AddonKit](https://github.com/ms-iot/iot-adk-addonkit).
3. Follow the [Create a basic image instructions](https://docs.microsoft.com/en-us/windows-hardware/manufacture/iot/create-a-basic-image) from the IoT Core Manufacturing guide with the following changes.
* When importing a BSP use one of the board names from the newly generated BSP folder in the imx-iotcore repo.
    ```
    Import-IoTBSP Sabre_iMX6Q_1GB <Path to imx-iotcore\BSP>
    ```
* When creating a product use the same board name from the BSP import.
    ```
    Add-IoTProduct ProductA Sabre_iMX6Q_1GB
    ```




