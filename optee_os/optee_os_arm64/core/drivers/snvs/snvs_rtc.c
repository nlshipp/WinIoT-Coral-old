// SPDX-License-Identifier: BSD-2-Clause
/*
 * Copyright 2019 NXP
 * Copyright 2017 NXP
 * Copyright (c) 2014, 2015 Linaro Limited
 *
 * Peng Fan <peng.fan@nxp.com>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include <initcall.h>
#include <io.h>
#include <drivers/imx_snvs.h>
#include <kernel/misc.h>
#include <kernel/tee_time.h>
#include <kernel/time_source.h>
#include <mm/core_memprot.h>
#include <mm/core_mmu.h>
#include <mpa.h>
#include <utee_defines.h>
#include <platform_config.h>
#include <stdint.h>
#include <tee/tee_cryp_utl.h>
#include <types_ext.h>
#include <trace.h>

/* SNVS LP SRTC takes 32K clock as input */
#define SNVS_SRTC_FREQ			0x8000
#define CNT_TO_SECS_SHIFT		15
#define CNT_TO_SECS_MASK		GENMASK_32(14, 0)

static uint64_t snvs_srtc_read_lp_counter(void)
{
	vaddr_t snvs = core_mmu_get_va(SNVS_BASE, MEM_AREA_IO_SEC);
	uint64_t val1, val2;
	uint32_t val;

	do {
		val = read32(snvs + SNVS_LPSRTCMR);
		val1 = val;
		val1 <<= 32;
		val = read32(snvs + SNVS_LPSRTCLR);
		val1 |= val;

		val = read32(snvs + SNVS_LPSRTCMR);
		val2 = val;
		val2 <<= 32;
		val = read32(snvs + SNVS_LPSRTCLR);
		val2 |= val;

	/*
	 * When cpu/bus runs at low freq, we may never get same value
	 * during two consecutive read, so only compare the second value.
	 */
	} while ((val1 >> CNT_TO_SECS_SHIFT) != (val2 >> CNT_TO_SECS_SHIFT));

	return val2;
}

static TEE_Result snvs_srtc_get_sys_time(TEE_Time *time)
{
	uint64_t cnt = snvs_srtc_read_lp_counter();

	time->seconds = cnt >> CNT_TO_SECS_SHIFT;
	time->millis = (cnt & CNT_TO_SECS_MASK) /
		(SNVS_SRTC_FREQ / TEE_TIME_MILLIS_BASE);

	return TEE_SUCCESS;
}

static const struct time_source snvs_srtc_time_source = {
	.name = "snvs srtc",
	.protection_level = 1000,
	.get_sys_time = snvs_srtc_get_sys_time,
};

REGISTER_TIME_SOURCE(snvs_srtc_time_source)

/* Needs to be invoked before service_init */
TEE_Result snvs_srtc_enable(void)
{
	vaddr_t snvs = core_mmu_get_va(SNVS_BASE, MEM_AREA_IO_SEC);
	int timeout = 2000;
	uint32_t val;

	val = read32(snvs + SNVS_LPCR);
	val |= BM_SNVS_LPCR_SRTC_ENV;
	write32(val, snvs + SNVS_LPCR);

	do {
		val = read32(snvs + SNVS_LPCR);
		if (val & BM_SNVS_LPCR_SRTC_ENV)
			break;
	} while (--timeout);

	if (!timeout)
		return TEE_ERROR_GENERIC;

	return TEE_SUCCESS;
}

/* Reused from tee_time_arm_cntpct.c */
void plat_prng_add_jitter_entropy(enum crypto_rng_src sid, unsigned int *pnum)
{
	uint64_t tsc = snvs_srtc_read_lp_counter();
	int bytes = 0, n;
	static uint8_t first, bits;
	static uint16_t acc;

	if (!first) {
		acc = tsc;
		bytes = 2;
		first = 1;
	} else {
		acc = (acc << 2) | ((acc >> 6) & 3);
		for (n = 0; n < 64; n += 2)
			acc ^= (tsc >> n) & 3;
		bits += 2;
		if (bits >= 8) {
			bits = 0;
			bytes = 1;
		}
	}
	if (bytes) {
		FMSG("%s: 0x%02" PRIX16, __func__,
		     acc & GENMASK_32(bytes * 8, 0));
		crypto_rng_add_event(sid, pnum, (uint8_t *)&acc, bytes);
	}
}
