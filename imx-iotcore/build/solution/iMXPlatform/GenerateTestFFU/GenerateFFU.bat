:: Copyright (c) Microsoft Corporation. All rights reserved.
:: Licensed under the MIT License.

set BATCH_HOME=%~dp0
call ..\..\..\tools\SetupBuildEnv.bat


if "%PLATFORM%"=="ARM" (

    REM i.MX6 Platforms

    cd /d %BATCH_HOME%
    echo "Building EVK_iMX6UL_512MB FFU"
    call BuildImage EVK_iMX6UL_512MB EVK_iMX6UL_512MB_TestOEMInput.xml

    REM cd /d %BATCH_HOME%
    REM echo "Building EVK_iMX6ULL_512MB FFU"
    REM call BuildImage EVK_iMX6ULL_512MB EVK_iMX6ULL_512MB_TestOEMInput.xml

    REM cd /d %BATCH_HOM%
    REM echo "Building Sabre_iMX6DL_1GB FFU"
    REM call BuildImage Sabre_iMX6DL_1GB Sabre_iMX6DL_1GB_TestOEMInput.xml

    REM cd /d %BATCH_HOME%
    REM echo "Building Sabre_iMX6Q_1GB FFU"
    REM call BuildImage Sabre_iMX6Q_1GB Sabre_iMX6Q_1GB_TestOEMInput.xml

    REM cd /d %BATCH_HOME%
    REM echo "Building Sabre_iMX6QP_1GB FFU"
    REM call BuildImage Sabre_iMX6QP_1GB Sabre_iMX6QP_1GB_TestOEMInput.xml

    REM cd /d %BATCH_HOME%
    REM echo "Building Sabre_iMX6SX_1GB FFU"
    REM call BuildImage Sabre_iMX6SX_1GB Sabre_iMX6SX_1GB_TestOEMInput.xml

    REM i.MX7 Platforms

    REM cd /d %BATCH_HOME%
    REM echo "Building Sabre_iMX7D_1GB FFU"
    REM call BuildImage Sabre_iMX7D_1GB Sabre_iMX7D_1GB_TestOEMInput.xml
)

if "%PLATFORM%"=="ARM64" (

    REM i.MX8M Platforms

    cd /d %BATCH_HOME%
    echo "Building NXPEVK_iMX8M_4GB FFU"
    call BuildImage NXPEVK_iMX8M_4GB NXPEVK_iMX8M_4GB_TestOEMInput.xml

    REM cd /d %BATCH_HOME%
    REM echo "Building NXPEVK_iMX8M_Mini_2GB FFU"
    REM call BuildImage NXPEVK_iMX8M_Mini_2GB NXPEVK_iMX8M_Mini_2GB_TestOEMInput.xml
)
