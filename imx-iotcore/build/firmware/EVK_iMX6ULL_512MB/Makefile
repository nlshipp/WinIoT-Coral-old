
UBOOT_CONFIG=imx6ull_14x14_evk_nt_defconfig
UBOOT_ROOT=../../../../u-boots/u-boot-imx_arm

EDK2_DSC=EVK_iMX6ULL_512MB
EDK2_PLATFORM=NXP/EVK_iMX6ULL_512MB
EDK2_DEBUG_RELEASE=RELEASE

FTPM_CRYPTO_PROVIDER= CFG_FTPM_USE_WOLF=n 
AUTHVARS_CRYPTO_PROVIDER= CFG_AUTHVARS_USE_WOLF=n

UBOOT_OPTEE_ITS=uboot_optee_unsigned.its
UEFI_ITS=uefi_im6ull_unsigned.its

all: firmware_fit.merged firmwareversions.log

include ../Common.mk

.PHONY: $(OPTEE)
$(OPTEE):
	$(MAKE) -C $(OPTEE_ROOT) O=$(OPTEE_OUT) PLATFORM=imx-mx6ullevk \
	  $(OPTEE_FLAGS_IMX6ULL)
