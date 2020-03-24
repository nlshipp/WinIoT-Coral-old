/* Copyright (c) Microsoft Corporation. All rights reserved.
   Licensed under the MIT License.

Module Name:

    Codec.c - Sends commands to codec via I2c.

Environment:

    User-mode Driver Framework 2

*/

#include "Driver.h"
#include "Codec.h"
#include <spb.h>
#include "codec.tmh"

typedef struct {
    DWORD MsSleepTime;
    UCHAR CommandBytes[3];
} CODEC_COMMAND, *PCODEC_COMMAND;

#define CODEC_COMMAND(wait, addr, data) {(DWORD)wait, (UCHAR)(addr), (UCHAR)((data & 0xFF00) >> 8), (UCHAR)(data & 0xFF)}

NTSTATUS
CodecSendCommands(
    _In_ PDEVICE_CONTEXT DeviceContext,
    _In_reads_(NumCommands) CODEC_COMMAND CodecCommand[],
    USHORT NumCommands
    );

NTSTATUS
CodecSendCommand(
    _In_ PDEVICE_CONTEXT DeviceContext,
    _In_ PCODEC_COMMAND CodecCommand
    );

NTSTATUS
CodecInitialize(
    _In_ PDEVICE_CONTEXT DeviceContext
    )
{
	// Format of a Codec command
	// Address [23:16] Data [15:0]
	CODEC_COMMAND Commands[] =
	{
		// Reset ALC5645 and base level initialize PR registers
		CODEC_COMMAND(0x0, 0x00, 0x0000),  // MX-00 - all reset

		// selected PR registers
		CODEC_COMMAND(0x0, 0x6a, 0x003d),  // MX-6a - Private register index PR-3d
		CODEC_COMMAND(0x0, 0x6c, 0x3600),  // MX-6c - Private register data
		CODEC_COMMAND(0x0, 0x6a, 0x001c),  // MX-6a - Private register index PR-1c
		CODEC_COMMAND(0x0, 0x6c, 0xfd20),  // MX-6c - Private register data
		CODEC_COMMAND(0x0, 0x6a, 0x0020),  // MX-6a - Private register index PR-20
		CODEC_COMMAND(0x0, 0x6c, 0x611f),  // MX-6c - Private register data
		CODEC_COMMAND(0x0, 0x6a, 0x0021),  // MX-6a - Private register index PR-21
		CODEC_COMMAND(0x0, 0x6c, 0x4040),  // MX-6c - Private register data
		CODEC_COMMAND(0x0, 0x6a, 0x0023),  // MX-6a - Private register index PR-23
		CODEC_COMMAND(0x0, 0x6c, 0x0004),  // MX-6c - Private register data
	};
	NTSTATUS status;

	status = CodecSendCommands(DeviceContext, Commands, ARRAYSIZE(Commands));

	return status;
}

NTSTATUS
CodecInitializeHeadphoneOutJack(
   _In_ PDEVICE_CONTEXT DeviceContext
   )
{
	// Format of a Codec command
	// Address [23:16] Data [15:0]
	CODEC_COMMAND Commands[] =
	{
		// power mgmt
		// I2S1 on, Analog DACL/R 1 on
		CODEC_COMMAND(0x0, 0x61, 0x9800),
		// Stereo1 Filter on
		CODEC_COMMAND(0x0, 0x62, 0x0800),
		// vref1, main bias, bg bias, hp, amp_hp, vref2
		CODEC_COMMAND(0x0, 0x63, 0xe8fa),
		// power (pll on)
		CODEC_COMMAND(0x0, 0x64, 0x0200),
		// HPMIX, ldo2
		CODEC_COMMAND(0x0, 0x65, 0x00c2),
		CODEC_COMMAND(0x0, 0x66, 0x0000),

		// HPOL
		CODEC_COMMAND(0x0, 0x02, 0x4040),
		CODEC_COMMAND(0x0, 0x45, 0x2000),

		// digital input
		CODEC_COMMAND(0x0, 0x19, 0xA1A1),
		CODEC_COMMAND(0x0, 0x79, 0x0123),
		CODEC_COMMAND(0x0, 0x29, 0x8080),

		// i2s1 input
		CODEC_COMMAND(0x0, 0x70, 0x8000),

		// TDM interface ctl
		CODEC_COMMAND(0x0, 0x77, 0x0C00),

		// IN1
		// undoc bit 5 set CBJ_MIC_SEL_L
		CODEC_COMMAND(0x0, 0x0A, 0x0022),
		// undoc bit13 set, IN1 Port Mode manual
		CODEC_COMMAND(0x0, 0x0B, 0x20a7),
		// undoc bit14,13 set
		CODEC_COMMAND(0x0, 0x0C, 0x6000),

		// HP Amp control - depop + hp enable
		// Soft Mute depop,  en_out_hp, Pow_pump_hp, Pow_capless
		CODEC_COMMAND(0x0, 0x8e, 0x8019),
		// Depop mode 2 select , Depop mode 1 enable
		CODEC_COMMAND(0x0, 0x8f, 0x3140),

		// global clock
		// global clock control
		// Sel_sysclk1 = PLL,  PLL source MCLK, PLL prediv /1, stereo dac div /1
		CODEC_COMMAND(0x0, 0x80, 0x4000),
		// pll control
		// pll_n=54, pll_k= 5
		CODEC_COMMAND(0x0, 0x81, 0x1a82),
		// pll_m=15
		CODEC_COMMAND(0x0, 0x82, 0xd000),
		// ASRC
		// Enable I2S1 ASRC, DAC ASRC for Stereo DAC
		CODEC_COMMAND(0x0, 0x83, 0x0c00),
		// ASRC clock sources = i2s1_asrc
		CODEC_COMMAND(0x0, 0x84, 0x1111),
		// ASRC clock div
		CODEC_COMMAND(0x0, 0x8A, 0x0120),

		// General control 1
		// Enable MCLK gating
		CODEC_COMMAND(0x0, 0xFA, 0x2061),
		// undocumented
		CODEC_COMMAND(0x0, 0xFB, 0x4050),
		// En_detect_clk_sys=01'b, EN_IBUF_BST1 (undoc)
		CODEC_COMMAND(0x0, 0xFC, 0x0300),

		// digital mixer
		// Stereo DAC mixer
		// unmute DAC1, mute DAC2
		CODEC_COMMAND(0x0, 0x2A, 0x1616),

		//
		// Updates for Win10

		// pll - double MCLK to 512*fs
		CODEC_COMMAND(0x0, 0x80, 0x4000),
		CODEC_COMMAND(0x0, 0x81, 0x0300),
		CODEC_COMMAND(0x0, 0x82, 0x0000),

		// i2s1 input slave, 24bit, i2s
		CODEC_COMMAND(0x0, 0x70, 0x8008),
		CODEC_COMMAND(0x0, 0x73, 0x10f4),
	};
	NTSTATUS status; 

    status = CodecSendCommands(DeviceContext, Commands, ARRAYSIZE(Commands));

    return status;
}

NTSTATUS
CodecSendCommand(
    _In_ PDEVICE_CONTEXT DeviceContext,
    _In_ PCODEC_COMMAND CodecCommand
    )
{
    NTSTATUS status;
    ULONG_PTR bytesWritten;
    WDF_MEMORY_DESCRIPTOR MemDescriptor;

    WDF_MEMORY_DESCRIPTOR_INIT_BUFFER(&MemDescriptor,&CodecCommand->CommandBytes[0], sizeof(CodecCommand->CommandBytes));

    status = WdfIoTargetSendWriteSynchronously(DeviceContext->I2cTarget, NULL, &MemDescriptor, 0, NULL, &bytesWritten);
    
    if (CodecCommand->MsSleepTime != 0)
    {
        Sleep(CodecCommand->MsSleepTime);
    }

    return status;
}

NTSTATUS
CodecSendCommands(
    _In_ PDEVICE_CONTEXT DeviceContext,
    _In_reads_(NumCommands) CODEC_COMMAND CodecCommand[],
    USHORT NumCommands
    )
{
    ULONG i;
    NTSTATUS status;

    for (i = 0; i < NumCommands; i++)
    {
        status = CodecSendCommand(DeviceContext, &CodecCommand[i]);

        if (!NT_SUCCESS(status))
        {
            return status;
        }
    }

    return STATUS_SUCCESS;
}
