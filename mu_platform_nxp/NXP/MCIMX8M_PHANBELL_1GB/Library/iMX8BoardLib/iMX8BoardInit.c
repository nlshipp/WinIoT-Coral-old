/** @file
*
*  Copyright (c) 2018, Microsoft Corporation. All rights reserved.
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

#include <Library/ArmPlatformLib.h>
#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/PrintLib.h>
#include <Library/PcdLib.h>
#include <Library/SerialPortLib.h>
#include <Library/TimerLib.h>
#include <Library/ArmSmcLib.h>
#include <Ppi/ArmMpCoreInfo.h>


#include "iMX8.h"
#include "iMX8ClkPwr.h"
#include "iMX8MIoMux.h"

ARM_CORE_INFO iMX8Ppi[] =
{
  {
    // Cluster 0, Core 0
    0x0, 0x0,
    // MP Core MailBox Set/Get/Clear Addresses and Clear Value.
    // Not used with i.MX8, set to 0
    (EFI_PHYSICAL_ADDRESS)0x00000000,
    (EFI_PHYSICAL_ADDRESS)0x00000000,
    (EFI_PHYSICAL_ADDRESS)0x00000000,
    (UINT64)0
  },
#if FixedPcdGet32(PcdCoreCount) > 1
  {
    // Cluster 0, Core 1
    0x0, 0x1,
    // MP Core MailBox Set/Get/Clear Addresses and Clear Value
    // Not used with i.MX8, set to 0
    (EFI_PHYSICAL_ADDRESS)0x00000000,
    (EFI_PHYSICAL_ADDRESS)0x00000000,
    (EFI_PHYSICAL_ADDRESS)0x00000000,
    (UINT64)0
  },
#endif // FixedPcdGet32(PcdCoreCount) > 1
#if FixedPcdGet32(PcdCoreCount) > 2
  {
    // Cluster 0, Core 2
    0x0, 0x2,
    // MP Core MailBox Set/Get/Clear Addresses and Clear Value
    // Not used with i.MX8, set to 0
    (EFI_PHYSICAL_ADDRESS)0x00000000,
    (EFI_PHYSICAL_ADDRESS)0x00000000,
    (EFI_PHYSICAL_ADDRESS)0x00000000,
    (UINT64)0
  },
  {
    // Cluster 0, Core 3
    0x0, 0x3,
    // MP Core MailBox Set/Get/Clear Addresses and Clear Value
    // Not used with i.MX8, set to 0
    (EFI_PHYSICAL_ADDRESS)0x00000000,
    (EFI_PHYSICAL_ADDRESS)0x00000000,
    (EFI_PHYSICAL_ADDRESS)0x00000000,
    (UINT64)0
  }
#endif // FixedPcdGet32(PcdCoreCount) > 2
};

EFI_STATUS PrePeiCoreGetMpCoreInfo(OUT UINTN *CoreCount, OUT ARM_CORE_INFO **ArmCoreTable)
{
  // Only support one cluster
  *CoreCount = sizeof(iMX8Ppi) / sizeof(ARM_CORE_INFO);
  ASSERT (*CoreCount == FixedPcdGet32 (PcdCoreCount));
  *ArmCoreTable = iMX8Ppi;
  return EFI_SUCCESS;
}

ARM_MP_CORE_INFO_PPI mMpCoreInfoPpi = { PrePeiCoreGetMpCoreInfo };

EFI_PEI_PPI_DESCRIPTOR  gPlatformPpiTable[] = {
  {
    EFI_PEI_PPI_DESCRIPTOR_PPI,
    &gArmMpCoreInfoPpiGuid,
    &mMpCoreInfoPpi
  }
};

VOID ArmPlatformGetPlatformPpiList(OUT UINTN *PpiListSize, OUT EFI_PEI_PPI_DESCRIPTOR **PpiList)
{
  *PpiListSize = sizeof(gPlatformPpiTable);
  *PpiList = gPlatformPpiTable;
}

/**
  Initalize the Audio system
**/
#define SAI_PAD_CFG_OUT (IOMUXC_PAD_PUE_ENABLE | IOMUXC_PAD_DSE_R0_DIV_3 | IOMUXC_PAD_SRE_FAST)
VOID AudioInit(VOID)
{
  EFI_STATUS status;
  // Mux the SAI2 pins to wm8524 codec
  IOMUXC_SW_MUX_CTL_PAD_SAI2_TXFS = IOMUXC_MUX_ALT0;
  IOMUXC_SW_MUX_CTL_PAD_SAI2_TXC  = IOMUXC_MUX_ALT0;
  IOMUXC_SW_MUX_CTL_PAD_SAI2_TXD0 = IOMUXC_MUX_ALT0;
  IOMUXC_SW_MUX_CTL_PAD_SAI2_MCLK = IOMUXC_MUX_ALT0;

  IOMUXC_SW_PAD_CTL_PAD_SAI2_TXFS = SAI_PAD_CFG_OUT;
  IOMUXC_SW_PAD_CTL_PAD_SAI2_TXC  = SAI_PAD_CFG_OUT;
  IOMUXC_SW_PAD_CTL_PAD_SAI2_TXD0 = SAI_PAD_CFG_OUT;
  IOMUXC_SW_PAD_CTL_PAD_SAI2_MCLK = SAI_PAD_CFG_OUT;
  // unmute audio
  IOMUXC_SW_MUX_CTL_PAD_GPIO1_IO08 = IOMUXC_MUX_ALT0;
  GPIO1_DR   |= GPIO_DR_DR(1 << 8);
  GPIO1_GDIR |= GPIO_DR_DR(1 << 8);
  // enable the AUDIO_PLL - 44,100 Hz * 256
  status = ImxSetSAI2ClockRate (11289600);
  if (EFI_ERROR (status)) {
    DebugPrint (DEBUG_ERROR, "AudioInit - ImxSetAudioMclkClockRate failed");
  }
}

/**
  Initialize I2C modules on the SOC and perform required pin-muxing
**/
#define I2C_PAD_CTRL (IOMUXC_PAD_DSE_R0_DIV_3 | IOMUXC_PAD_SRE_SLOW | IOMUXC_PAD_ODE_ENABLED | \
                      IOMUXC_PAD_HYS_ENABLED | IOMUXC_PAD_PUE_ENABLE)
VOID I2cInit()
{
  IOMUXC_SW_MUX_CTL_PAD_I2C1_SCL = IOMUXC_MUX_ALT0 | IOMUXC_MUX_SION_ENABLED;
  IOMUXC_SW_MUX_CTL_PAD_I2C1_SDA = IOMUXC_MUX_ALT0 | IOMUXC_MUX_SION_ENABLED;
  IOMUXC_SW_MUX_CTL_PAD_I2C2_SCL = IOMUXC_MUX_ALT0 | IOMUXC_MUX_SION_ENABLED;
  IOMUXC_SW_MUX_CTL_PAD_I2C2_SDA = IOMUXC_MUX_ALT0 | IOMUXC_MUX_SION_ENABLED;
  IOMUXC_SW_MUX_CTL_PAD_I2C3_SCL = IOMUXC_MUX_ALT0 | IOMUXC_MUX_SION_ENABLED;
  IOMUXC_SW_MUX_CTL_PAD_I2C3_SDA = IOMUXC_MUX_ALT0 | IOMUXC_MUX_SION_ENABLED;

  IOMUXC_SW_PAD_CTL_PAD_I2C1_SCL = I2C_PAD_CTRL;
  IOMUXC_SW_PAD_CTL_PAD_I2C1_SDA = I2C_PAD_CTRL;
  IOMUXC_SW_PAD_CTL_PAD_I2C2_SCL = I2C_PAD_CTRL;
  IOMUXC_SW_PAD_CTL_PAD_I2C2_SDA = I2C_PAD_CTRL;
  IOMUXC_SW_PAD_CTL_PAD_I2C3_SCL = I2C_PAD_CTRL;
  IOMUXC_SW_PAD_CTL_PAD_I2C3_SDA = I2C_PAD_CTRL;
}

/**
  Initialize ENETs modules on the SOC and perform required pin-muxing.
**/
#define PIN_ENET_RST_B (1<<9)
#define ENET_MDC_PAD_CTRL     (IOMUXC_PAD_DSE_R0_DIV_3 | IOMUXC_PAD_SRE_SLOW)
#define ENET_MDIO_PAD_CTRL    (IOMUXC_PAD_DSE_R0_DIV_3 | IOMUXC_PAD_SRE_SLOW | IOMUXC_PAD_ODE_ENABLED)
#define ENET_TX_PAD_CTRL      (IOMUXC_PAD_DSE_R0_DIV_7 | IOMUXC_PAD_SRE_MAX)
#define ENET_RX_PAD_CTRL      (IOMUXC_PAD_DSE_R0_DIV_1 | IOMUXC_PAD_SRE_FAST | IOMUXC_PAD_HYS_ENABLED)
VOID EnetInit(VOID)
{
    // ENET1/2 MDIO bus (both ENETs share one MDIO bus conected to the ENET1 controller)
    IOMUXC_SW_MUX_CTL_PAD_ENET_MDC    = IOMUXC_MUX_ALT0;    // ENET1_MDC  -> PAD_GPIO1_IO11
    IOMUXC_SW_MUX_CTL_PAD_ENET_MDIO   = IOMUXC_MUX_ALT0;    // ENET1_MDIO -> PAD_PAD_ENET_MDIO
    IOMUXC_ENET1_MDIO_SELECT_INPUT    = IOMUXC_MUX_ALT1;    // ENET1_MDIO <- PAD_PAD_ENET_MDIO
    IOMUXC_SW_PAD_CTL_PAD_ENET_MDC    = ENET_MDC_PAD_CTRL;  // ENET1_MDC  electrical settings
    IOMUXC_SW_PAD_CTL_PAD_ENET_MDIO   = ENET_MDIO_PAD_CTRL; // ENET1_MDIO electrical settings
    // ENET1 RGMMI pins routing
    IOMUXC_SW_MUX_CTL_PAD_ENET_TD3    = IOMUXC_MUX_ALT0;    // ENET1_RGMII_TD3    -> PAD_ENET1_TD3
    IOMUXC_SW_MUX_CTL_PAD_ENET_TD2    = IOMUXC_MUX_ALT0;    // ENET1_RGMII_TD2    -> PAD_ENET1_TD2
    IOMUXC_SW_MUX_CTL_PAD_ENET_TD1    = IOMUXC_MUX_ALT0;    // ENET1_RGMII_TD1    -> PAD_ENET1_TD1
    IOMUXC_SW_MUX_CTL_PAD_ENET_TD0    = IOMUXC_MUX_ALT0;    // ENET1_RGMII_TD0    -> PAD_ENET1_TD0
    IOMUXC_SW_MUX_CTL_PAD_ENET_TX_CTL = IOMUXC_MUX_ALT0;    // ENET1_RGMII_TX_CTL -> PAD_ENET1_TX_CTL
    IOMUXC_SW_MUX_CTL_PAD_ENET_TXC    = IOMUXC_MUX_ALT0;    // ENET1_RGMII_TXC    -> PAD_ENET1_TXC
    IOMUXC_SW_MUX_CTL_PAD_ENET_RD3    = IOMUXC_MUX_ALT0;    // ENET1_RGMII_RD3    <- PAD_ENET1_RD3
    IOMUXC_SW_MUX_CTL_PAD_ENET_RD2    = IOMUXC_MUX_ALT0;    // ENET1_RGMII_RD2    <- PAD_ENET1_RD2
    IOMUXC_SW_MUX_CTL_PAD_ENET_RD1    = IOMUXC_MUX_ALT0;    // ENET1_RGMII_RD1    <- PAD_ENET1_RD1
    IOMUXC_SW_MUX_CTL_PAD_ENET_RD0    = IOMUXC_MUX_ALT0;    // ENET1_RGMII_RD0    <- PAD_ENET1_RD0
    IOMUXC_SW_MUX_CTL_PAD_ENET_RX_CTL = IOMUXC_MUX_ALT0;    // ENET1_RGMII_RX_CTL <- PAD_ENET1_RX_CTL
    IOMUXC_SW_MUX_CTL_PAD_ENET_RXC    = IOMUXC_MUX_ALT0;    // ENET1_RGMII_RXC    <- PAD_ENET1_RXC
    // ENET1 RGMMI pins electrical settings
    IOMUXC_SW_PAD_CTL_PAD_ENET_RD3    = ENET_RX_PAD_CTRL;  // ENET1_RGMII_RD3    electrical settings
    IOMUXC_SW_PAD_CTL_PAD_ENET_RD2    = ENET_RX_PAD_CTRL;  // ENET1_RGMII_RD2    electrical settings
    IOMUXC_SW_PAD_CTL_PAD_ENET_RD1    = ENET_RX_PAD_CTRL;  // ENET1_RGMII_RD1    electrical settings
    IOMUXC_SW_PAD_CTL_PAD_ENET_RD0    = ENET_RX_PAD_CTRL;  // ENET1_RGMII_RD0    electrical settings
    IOMUXC_SW_PAD_CTL_PAD_ENET_RX_CTL = ENET_RX_PAD_CTRL;  // ENET1_RGMII_RX_CTL electrical settings
    IOMUXC_SW_PAD_CTL_PAD_ENET_RXC    = ENET_RX_PAD_CTRL;   // ENET1_RGMII_RXC    electrical settings
    IOMUXC_SW_PAD_CTL_PAD_ENET_TD3    = ENET_TX_PAD_CTRL;  // ENET1_RGMII_TD3    electrical settings
    IOMUXC_SW_PAD_CTL_PAD_ENET_TD2    = ENET_TX_PAD_CTRL;  // ENET1_RGMII_TD2    electrical settings
    IOMUXC_SW_PAD_CTL_PAD_ENET_TD1    = ENET_TX_PAD_CTRL;  // ENET1_RGMII_TD1    electrical settings
    IOMUXC_SW_PAD_CTL_PAD_ENET_TD0    = ENET_TX_PAD_CTRL;  // ENET1_RGMII_TD0    electrical settings
    IOMUXC_SW_PAD_CTL_PAD_ENET_TX_CTL = ENET_TX_PAD_CTRL;  // ENET1_RGMII_TX_CTL electrical settings
    IOMUXC_SW_PAD_CTL_PAD_ENET_TXC    = ENET_TX_PAD_CTRL;   // ENET1_RGMII_TXC    electrical settings
    // Configure ENET_nRST signal
    IOMUXC_SW_MUX_CTL_PAD_GPIO1_IO09 = IOMUXC_MUX_ALT0;
    IOMUXC_SW_PAD_CTL_PAD_GPIO1_IO09 = IOMUXC_PAD_DSE_R0_DIV_1 | IOMUXC_PAD_SRE_SLOW;
    GPIO1_DR &= ~PIN_ENET_RST_B;                                   // Set ENET_nRST = 0
    GPIO1_GDIR |= PIN_ENET_RST_B;                                  // Set direction to output
    MicroSecondDelay(500);
    GPIO1_DR |= PIN_ENET_RST_B;                                    // Set ENET_nRST = 1
    CCM_CCGR_ENET1 = 0x00000003;                                   // ENET1 clock gate
}

#if (FixedPcdGet32(PcdPcie1Enable) || FixedPcdGet32(PcdPcie2Enable))
/**
  Initialize PCI Express module on the SOC and perform required pin-muxing
**/
VOID PcieInit ()
{
  ARM_SMC_ARGS smc_args;

  /* Enable PCIe1 power domain */
  smc_args.Arg0 = 0xC2000000;   // FSL_SIP_GPC
  smc_args.Arg1 = 0x03;         // FSL_SIP_CONFIG_GPC_PM_DOMAIN
  smc_args.Arg2 = 0x01;         // Domain ID = PCIe1
  smc_args.Arg3 = 0x01;         // Enable domain
  smc_args.Arg4 = 0x00;
  ArmCallSmc(&smc_args);
  /* Enable PCIe2 power domain */
  smc_args.Arg0 = 0xC2000000;   // FSL_SIP_GPC
  smc_args.Arg1 = 0x03;         // FSL_SIP_CONFIG_GPC_PM_DOMAIN
  smc_args.Arg2 = 0x0A;         // Domain ID = PCIe2
  smc_args.Arg3 = 0x01;         // Enable domain
  smc_args.Arg4 = 0x00;
  ArmCallSmc(&smc_args);

#if FixedPcdGet32(PcdPcie1Enable)
  /* Disable PCIE_CTRL clock root */
  CCM_CCGR_CLR(37) = 0x03;
  /* Set PCIe controller input clock to 250MHz (SYSTEM_PLL2_DIV4) */
  CCM_TARGET_ROOT(70) = CCM_TARGET_ROOT_MUX(0x01);
  CCM_TARGET_ROOT_SET(70) = CCM_TARGET_ROOT_ENABLE_MASK;
  /* Set PCIe PHy input clock to 100MHz (SYSTEM_PLL2_DIV10) */
  CCM_TARGET_ROOT(71) = CCM_TARGET_ROOT_MUX(0x01);
  CCM_TARGET_ROOT_SET(71) = CCM_TARGET_ROOT_ENABLE_MASK;
  /* Set PCIe AUX input clock to 10MHz (SYSTEM_PLL2_DIV20 / 5) */
  CCM_TARGET_ROOT(72) = CCM_TARGET_ROOT_MUX(0x02) | CCM_TARGET_ROOT_POST_PODF(4);
  CCM_TARGET_ROOT_SET(72) = CCM_TARGET_ROOT_ENABLE_MASK;
  /* Enable PCIE_CTRL clock root */
  CCM_CCGR_SET(37) = 0x03;

  /* Configure NAND_DQS as GPIO to control PCIe WL_nWAKE PAD */
  IOMUXC_SW_MUX_CTL_PAD_NAND_DQS = IOMUXC_MUX_ALT5;
  GPIO3_GDIR &= ~(0x01 << 14);                   /* Set input direction */

  /* Configure UART4_RXD as GPIO to control PCIe WL_nPERST PAD */
  IOMUXC_SW_MUX_CTL_PAD_UART4_RXD = IOMUXC_MUX_ALT5;
  GPIO5_DR &= ~(0x01 << 28);                    /* Set the pad to the low level */
  GPIO5_GDIR |= (0x01 << 28);                   /* Set output direction */

  /* Configure UART4_TXD to control PCIe WL_REG_ON PAD */
  IOMUXC_SW_MUX_CTL_PAD_UART4_TXD = IOMUXC_MUX_ALT5;
  GPIO5_DR |= (0x01 << 29);                     /* Set the pad to the high level */
  GPIO5_GDIR |= (0x01 << 29);                   /* Set output direction */
#endif
#if FixedPcdGet32(PcdPcie2Enable)
  /* Disable PCIE_CTRL clock root */
  CCM_CCGR_CLR(100) = 0x03;
  /* Set PCIe controller input clock to 250MHz (SYSTEM_PLL2_DIV4) */
  CCM_TARGET_ROOT(128) = CCM_TARGET_ROOT_MUX(0x01);
  CCM_TARGET_ROOT_SET(128) = CCM_TARGET_ROOT_ENABLE_MASK;
  /* Set PCIe PHy input clock to 100MHz (SYSTEM_PLL2_DIV10) */
  CCM_TARGET_ROOT(129) = CCM_TARGET_ROOT_MUX(0x01);
  CCM_TARGET_ROOT_SET(129) = CCM_TARGET_ROOT_ENABLE_MASK;
  /* Set PCIe AUX input clock to 10MHz (SYSTEM_PLL2_DIV20 / 5) */
  CCM_TARGET_ROOT(130) = CCM_TARGET_ROOT_MUX(0x02) | CCM_TARGET_ROOT_POST_PODF(4);
  CCM_TARGET_ROOT_SET(130) = CCM_TARGET_ROOT_ENABLE_MASK;
  /* Enable PCIE_CTRL clock root */
  CCM_CCGR_SET(100) = 0x03;

  /* Configure ECSPI2_MOSI as GPIO to control PCIe nWAKE PAD */
  IOMUXC_SW_MUX_CTL_PAD_ECSPI2_MOSI = IOMUXC_MUX_ALT5;
  GPIO5_GDIR &= ~(0x01 << 7);                   /* Set input direction */

  /* Configure ECSPI2_MISO as GPIO to control PCIe nPERST PAD */
  IOMUXC_SW_MUX_CTL_PAD_ECSPI2_MISO = IOMUXC_MUX_ALT5;
  GPIO5_DR &= ~(0x01 << 12);                    /* Set the pad to the low level */
  GPIO5_GDIR |= (0x01 << 12);                   /* Set output direction */

  /* Configure ECSPI2_SCLK to control PCIe nDISABLE PAD */
  IOMUXC_SW_MUX_CTL_PAD_ECSPI2_SCLK = IOMUXC_MUX_ALT5;
  GPIO5_DR |= (0x01 << 10);                     /* Set the pad to the high level */
  GPIO5_GDIR |= (0x01 << 10);                   /* Set output direction */
#endif
}
#endif

/**
  Initialize PWM block and perform required pin-muxing.
**/
VOID PwmInit()
{
    int pwm_ccgr_offset = 40;
    int pwm_clk_root_offset = 103;

    /* Initialize PWM1-PWM4 clocks */
    for(int i = 0; i < 4; i++)
    {
      CCM_CCGR_CLR(pwm_ccgr_offset + i) = 0x03;
      CCM_TARGET_ROOT(pwm_clk_root_offset + i) = (CCM_TARGET_ROOT_MUX(0x00) | CCM_TARGET_ROOT_ENABLE_MASK);       /* Set 25M_REF_CLK as PWM input clock, no PRE nor POST divider, Enable PWM clock */
      CCM_CCGR_SET(pwm_ccgr_offset + i) = 0x03;
    }

    /* Configure GPIO1_IO01 as PWM1_OUT */
    IOMUXC_SW_MUX_CTL_PAD_GPIO1_IO01 = IOMUXC_MUX_ALT1;
    IOMUXC_SW_PAD_CTL_PAD_GPIO1_IO01 = (IOMUXC_SW_PAD_CTL_PAD_DSE(4) | /* Drive strength 85 Ohm @3.3V, 80 Ohm @2.5V, 75 Ohm @1.8V, 90 Ohm @1.2V */
                                        IOMUXC_SW_PAD_CTL_PAD_SRE(0)); /* Slow Frequency Slew Rate (50Mhz) */
}

/**
  Initialize controllers that must setup at the early stage
**/
RETURN_STATUS ArmPlatformInitialize(IN UINTN MpId)
{
  if (!ArmPlatformIsPrimaryCore(MpId)) {
    return RETURN_SUCCESS;
  }
  // Initialize debug serial port
  SerialPortInitialize();
  SerialPortWrite((UINT8 *)SERIAL_DEBUG_PORT_INIT_MSG, (UINTN)sizeof(SERIAL_DEBUG_PORT_INIT_MSG));
  // Initialize peripherals
  ImxUngateActiveClock();
//  AudioInit();
  I2cInit();
  EnetInit();
#if (FixedPcdGet32(PcdPcie1Enable) || FixedPcdGet32(PcdPcie2Enable))
  PcieInit ();
#endif
  PwmInit();
  return RETURN_SUCCESS;
}

/**
  Return the current Boot Mode
  This function returns the boot reason on the platform
**/
EFI_BOOT_MODE ArmPlatformGetBootMode (VOID)
{
  return BOOT_WITH_FULL_CONFIGURATION;
}
