/******************************************************************************
 *
 * This file is provided under a dual license.  When you use or
 * distribute this software, you may choose to be licensed under
 * version 2 of the GNU General Public License ("GPLv2 License")
 * or BSD License.
 *
 * GPLv2 License
 *
 * Copyright(C) 2016 MediaTek Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of version 2 of the GNU General Public License as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See http://www.gnu.org/licenses/gpl-2.0.html for more details.
 *
 * BSD LICENSE
 *
 * Copyright(C) 2016 MediaTek Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *  * Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *****************************************************************************/

/*******************************************************************************
*                         C O M P I L E R   F L A G S
********************************************************************************
*/

/*******************************************************************************
*                    E X T E R N A L   R E F E R E N C E S
********************************************************************************
*/

#include "precomp.h"

#if (CFG_SUPPORT_SINGLE_SKU == 1)
#if (CFG_SUPPORT_SINGLE_SKU_LOCAL_DB == 1)
#include "rlm_domain.h"

#if 0
/**
 * enum ieee80211_channel_flags - channel flags
 *
 * Channel flags set by the regulatory control code.
 *
 * @IEEE80211_CHAN_DISABLED: This channel is disabled.
 * @IEEE80211_CHAN_NO_IR: do not initiate radiation, this includes
 *      sending probe requests or beaconing.
 * @IEEE80211_CHAN_RADAR: Radar detection is required on this channel.
 * @IEEE80211_CHAN_NO_HT40PLUS: extension channel above this channel
 *      is not permitted.
 * @IEEE80211_CHAN_NO_HT40MINUS: extension channel below this channel
 *      is not permitted.
 * @IEEE80211_CHAN_NO_OFDM: OFDM is not allowed on this channel.
 * @IEEE80211_CHAN_NO_80MHZ: If the driver supports 80 MHz on the band,
 *      this flag indicates that an 80 MHz channel cannot use this
 *      channel as the control or any of the secondary channels.
 *      This may be due to the driver or due to regulatory bandwidth
 *      restrictions.
 * @IEEE80211_CHAN_NO_160MHZ: If the driver supports 160 MHz on the band,
 *      this flag indicates that an 160 MHz channel cannot use this
 *      channel as the control or any of the secondary channels.
 *      This may be due to the driver or due to regulatory bandwidth
 *      restrictions.
 * @IEEE80211_CHAN_INDOOR_ONLY: see %NL80211_FREQUENCY_ATTR_INDOOR_ONLY
 * @IEEE80211_CHAN_GO_CONCURRENT: see %NL80211_FREQUENCY_ATTR_GO_CONCURRENT
 * @IEEE80211_CHAN_NO_20MHZ: 20 MHz bandwidth is not permitted
 *      on this channel.
 * @IEEE80211_CHAN_NO_10MHZ: 10 MHz bandwidth is not permitted
 *      on this channel.
 *
 */
enum ieee80211_channel_flags {
	IEEE80211_CHAN_DISABLED         = 1<<0,
	IEEE80211_CHAN_NO_IR            = 1<<1,
	/* hole at 1<<2 */
	IEEE80211_CHAN_RADAR            = 1<<3,
	IEEE80211_CHAN_NO_HT40PLUS      = 1<<4,
	IEEE80211_CHAN_NO_HT40MINUS     = 1<<5,
	IEEE80211_CHAN_NO_OFDM          = 1<<6,
	IEEE80211_CHAN_NO_80MHZ         = 1<<7,
	IEEE80211_CHAN_NO_160MHZ        = 1<<8,
	IEEE80211_CHAN_INDOOR_ONLY      = 1<<9,
	IEEE80211_CHAN_GO_CONCURRENT    = 1<<10,
	IEEE80211_CHAN_NO_20MHZ         = 1<<11,
	IEEE80211_CHAN_NO_10MHZ         = 1<<12,
};

#define IEEE80211_CHAN_NO_HT40 \
	(IEEE80211_CHAN_NO_HT40PLUS | IEEE80211_CHAN_NO_HT40MINUS)


struct ieee80211_freq_range {
	u32 start_freq_khz;
	u32 end_freq_khz;
	u32 max_bandwidth_khz;
};

struct ieee80211_power_rule {
	u32 max_antenna_gain;
	u32 max_eirp;
};

/**
* enum reg_flags (the same as nl80211_reg_rule_flags) - regulatory rule flags
*
* @NL80211_RRF_NO_OFDM: OFDM modulation not allowed
* @NL80211_RRF_NO_CCK: CCK modulation not allowed
* @NL80211_RRF_NO_INDOOR: indoor operation not allowed
* @NL80211_RRF_NO_OUTDOOR: outdoor operation not allowed
* @NL80211_RRF_DFS: DFS support is required to be used
* @NL80211_RRF_PTP_ONLY: this is only for Point To Point links
* @NL80211_RRF_PTMP_ONLY: this is only for Point To Multi Point links
* @NL80211_RRF_NO_IR: no mechanisms that initiate radiation are allowed,
*      this includes probe requests or modes of operation that require
*      beaconing.
* @NL80211_RRF_AUTO_BW: maximum available bandwidth should be calculated
*      base on contiguous rules and wider channels will be allowed to cross
*      multiple contiguous/overlapping frequency ranges.
* @NL80211_RRF_IR_CONCURRENT: See &NL80211_FREQUENCY_ATTR_IR_CONCURRENT
* @NL80211_RRF_NO_HT40MINUS: channels can't be used in HT40- operation
* @NL80211_RRF_NO_HT40PLUS: channels can't be used in HT40+ operation
* @NL80211_RRF_NO_80MHZ: 80MHz operation not allowed
* @NL80211_RRF_NO_160MHZ: 160MHz operation not allowed
*/
enum reg_flags {
	NL80211_RRF_NO_OFDM             = 1<<0,
	NL80211_RRF_NO_CCK              = 1<<1,
	NL80211_RRF_NO_INDOOR           = 1<<2,
	NL80211_RRF_NO_OUTDOOR          = 1<<3,
	NL80211_RRF_DFS                 = 1<<4,
	NL80211_RRF_PTP_ONLY            = 1<<5,
	NL80211_RRF_PTMP_ONLY           = 1<<6,
	NL80211_RRF_NO_IR               = 1<<7,
	NL80211_RRF_NO_IBSS           = 1<<8,
	NL80211_RRF_AUTO_BW             = 1<<11,
	NL80211_RRF_IR_CONCURRENT       = 1<<12,
	NL80211_RRF_NO_HT40MINUS        = 1<<13,
	NL80211_RRF_NO_HT40PLUS         = 1<<14,
	NL80211_RRF_NO_80MHZ            = 1<<15,
	NL80211_RRF_NO_160MHZ           = 1<<16,
};

#define NL80211_RRF_PASSIVE_SCAN        NL80211_RRF_NO_IR
#define NL80211_RRF_NO_IBSS             NL80211_RRF_NO_IR
#define NL80211_RRF_NO_IR               NL80211_RRF_NO_IR
#define NL80211_RRF_NO_HT40             (NL80211_RRF_NO_HT40MINUS |\
										NL80211_RRF_NO_HT40PLUS)
#define NL80211_RRF_GO_CONCURRENT       NL80211_RRF_IR_CONCURRENT

/* For backport compatibility with older userspace */
#define NL80211_RRF_NO_IR_ALL           (NL80211_RRF_NO_IR | __NL80211_RRF_NO_IBSS)

struct ieee80211_reg_rule {
	struct ieee80211_freq_range freq_range;
	struct ieee80211_power_rule power_rule;
	u32 reg_flags; /*enum reg_flags*/
	u32 dfs_cac_ms;
};

struct ieee80211_regdomain {
#define MAX_NUMER_REG_RULES	6
	char alpha2[3];
	u32 n_reg_rules;
	enum nl80211_dfs_regions dfs_region;
	struct ieee80211_reg_rule reg_rules[MAX_NUMER_REG_RULES];
};


#define MHZ_TO_KHZ(freq) ((freq) * 1000)
#define KHZ_TO_MHZ(freq) ((freq) / 1000)
#define DBI_TO_MBI(gain) ((gain) * 100)
#define MBI_TO_DBI(gain) ((gain) / 100)
#define DBM_TO_MBM(gain) ((gain) * 100)
#define MBM_TO_DBM(gain) ((gain) / 100)

#define REG_RULE_EXT(start, end, bw, gain, eirp, dfs_cac, reg_flags)    \
{                                                                       \
	.freq_range.start_freq_khz = MHZ_TO_KHZ(start),                 \
	.freq_range.end_freq_khz = MHZ_TO_KHZ(end),                     \
	.freq_range.max_bandwidth_khz = MHZ_TO_KHZ(bw),                 \
	.power_rule.max_eirp = DBM_TO_MBM(eirp),                        \
	.flags = reg_flags,                                             \
	.dfs_cac_ms = dfs_cac,                                          \
}

#define REG_RULE(start, end, bw, gain, eirp, reg_flags) \
{                                                       \
	.freq_range.start_freq_khz = MHZ_TO_KHZ(start), \
	.freq_range.end_freq_khz = MHZ_TO_KHZ(end),     \
	.freq_range.max_bandwidth_khz = MHZ_TO_KHZ(bw), \
	.power_rule.max_antenna_gain = DBI_TO_MBI(gain),\
	.power_rule.max_eirp = DBM_TO_MBM(eirp),        \
	.flags = reg_flags,                             \
}
#endif

/***************************************************
 * Hello ^++++^
 * Here to describe the regulatory rules of yours.
 **************************************************/

/*
 * Step1. Decclare struct ieee80211_regdomain
 */

const struct ieee80211_regdomain regdom_us01 = {
	.n_reg_rules = 6,
	.reg_rules = {
	/* channels 1..11 */
	REG_RULE_LIGHT(2412-10, 2462+10, 40, 0),
	/* channels 36..48 */
	REG_RULE_LIGHT(5180-10, 5240+10, 40, 0),
	/* channels 56..64 */
	REG_RULE_LIGHT(5260-10, 5320+10, 40, KAL_RRF_DFS),
	/* channels 100..118 */
	REG_RULE_LIGHT(5500-10, 5590+10, 40, KAL_RRF_DFS),
	/* channels 132..140 */
	REG_RULE_LIGHT(5660-10, 5700+10, 40, KAL_RRF_DFS),
	/* channels 149..165 */
	REG_RULE_LIGHT(5745-10, 5825+10, 40, 0) }
};

const struct ieee80211_regdomain regdom_tw = {
	.n_reg_rules = 5,
	.dfs_region = NL80211_DFS_FCC,
	.reg_rules = {
	/* channels 1..11 */
	REG_RULE_LIGHT(2412-10, 2474+10, 40, 0),
	/* channels 36..48 */
	REG_RULE_LIGHT(5160-10, 5240+10, 80, KAL_RRF_AUTO_BW),
	/* channels 52..64 */
	REG_RULE_LIGHT(5260-10, 5340+10, 80, KAL_RRF_DFS | KAL_RRF_AUTO_BW),
	/* channels 100..140*/
	REG_RULE_LIGHT(5480-10, 5715+10, 160, KAL_RRF_DFS),
	/* channels 149..165 */
	REG_RULE_LIGHT(5735-10, 5840+10, 80, 0) }
};

const struct ieee80211_regdomain regdom_us = {
	.n_reg_rules = 5,
	.dfs_region = NL80211_DFS_FCC,
	.reg_rules = {
	/* channels 1..11 */
	REG_RULE_LIGHT(2412-10, 2462+10, 40, 0),
	/* channels 36..48 */
	REG_RULE_LIGHT(5180-10, 5240+10, 80, KAL_RRF_AUTO_BW),
	/* channels 52..64 */
	REG_RULE_LIGHT(5260-10, 5320+10, 80, KAL_RRF_DFS | KAL_RRF_AUTO_BW),
	/* channels 100..144 */
	REG_RULE_LIGHT(5500-10, 5720+10, 160, KAL_RRF_DFS),
	/* channels 149..165 */
	REG_RULE_LIGHT(5745-10, 5825+10, 80, 0) }
};

const struct ieee80211_regdomain regdom_ca = {
	.n_reg_rules = 6,
	.dfs_region = NL80211_DFS_FCC,
	.reg_rules = {
	/* channels 1..11 */
	REG_RULE_LIGHT(2412-10, 2462+10, 40, 0),
	/* channels 36..48 */
	REG_RULE_LIGHT(5160-10, 5240+10, 80, KAL_RRF_AUTO_BW),
	/* channels 52..64 */
	REG_RULE_LIGHT(5260-10, 5340+10, 80, KAL_RRF_DFS | KAL_RRF_AUTO_BW),
	REG_RULE_LIGHT(5460+10, 5590+10, 80, KAL_RRF_DFS),
	/* channels 100..144 */
	REG_RULE_LIGHT(5660-10, 5720+10, 80, KAL_RRF_DFS),
	/* channels 149..165 */
	REG_RULE_LIGHT(5745-10, 5825+10, 80, 0) }
};

const struct ieee80211_regdomain regdom_cn = {
	.n_reg_rules = 4,
	.dfs_region = NL80211_DFS_FCC,
	.reg_rules = {
	/* channels 1..13 */
	REG_RULE_LIGHT(2412-10, 2472+10, 40, 0),
	/* channels 36..48 */
	REG_RULE_LIGHT(5180-10, 5240+10, 80, KAL_RRF_AUTO_BW),
	/* channels 52..64 */
	REG_RULE_LIGHT(5260-10, 5320+10, 80, KAL_RRF_DFS | KAL_RRF_AUTO_BW),
	/* channels 149..165 */
	REG_RULE_LIGHT(5745-10, 5825+10, 80, 0) }
};

const struct ieee80211_regdomain regdom_pa = {
	.n_reg_rules = 5,
	.dfs_region = NL80211_DFS_FCC,
	.reg_rules = {
	/* channels 1..13 */
	REG_RULE_LIGHT(2412-10, 2472+10, 40, 0),
	/* channels 36..48 */
	REG_RULE_LIGHT(5160-10, 5240+10, 80, KAL_RRF_AUTO_BW),
	/* channels 52..64 */
	REG_RULE_LIGHT(5260-10, 5340+10, 80, KAL_RRF_AUTO_BW),
	/* channels 52..64 */
	REG_RULE_LIGHT(5480-10, 5715+10, 160, 0),
	/* channels 149..165 */
	REG_RULE_LIGHT(5735-10, 5840+10, 80, 0) }
};

const struct ieee80211_regdomain regdom_cz_nl = {
	.n_reg_rules = 5,
	.dfs_region = NL80211_DFS_ETSI,
	.reg_rules = {
	/* channels 1..13 */
	REG_RULE_LIGHT(2412-10, 2472+10, 40, 0),
	/* channels 36..48 */
	REG_RULE_LIGHT(5160-10, 5240+10, 80, KAL_RRF_AUTO_BW),
	/* channels 52..64 */
	REG_RULE_LIGHT(5260-10, 5340+10, 80, KAL_RRF_DFS | KAL_RRF_AUTO_BW),
	/* channels 100..140 */
	REG_RULE_LIGHT(5480-10, 5715+10, 160, KAL_RRF_DFS),
	/* channels 100..140 */
	REG_RULE_LIGHT(5735-10, 5865+10, 80, 0) }
};

const struct ieee80211_regdomain regdom_cz_no = {
	.n_reg_rules = 5,
	.dfs_region = NL80211_DFS_ETSI,
	.reg_rules = {
	/* channels 1..13 */
	REG_RULE_LIGHT(2412-10, 2472+10, 40, 0),
	/* channels 36..48 */
	REG_RULE_LIGHT(5160-10, 5240+10, 80, KAL_RRF_AUTO_BW),
	/* channels 52..64 */
	REG_RULE_LIGHT(5260-10, 5340+10, 80, KAL_RRF_DFS | KAL_RRF_AUTO_BW),
	/* channels 100..140 */
	REG_RULE_LIGHT(5480-10, 5785+10, 160, KAL_RRF_DFS),
	/* channels 100..140 */
	REG_RULE_LIGHT(5825-10, 5840+10, 35, KAL_RRF_DFS) }
};

const struct ieee80211_regdomain regdom_hr = {
	.n_reg_rules = 4,
	.dfs_region = NL80211_DFS_ETSI,
	.reg_rules = {
	/* channels 1..13 */
	REG_RULE_LIGHT(2412-10, 2472+10, 40, 0),
	/* channels 36..48 */
	REG_RULE_LIGHT(5160-10, 5240+10, 80, KAL_RRF_AUTO_BW),
	/* channels 52..64 */
	REG_RULE_LIGHT(5260-10, 5340+10, 80, KAL_RRF_DFS | KAL_RRF_AUTO_BW),
	/* channels 100..140 */
	REG_RULE_LIGHT(5480-10, 5715+10, 160, KAL_RRF_DFS) }
};

const struct ieee80211_regdomain regdom_at = {
	.n_reg_rules = 4,
	.dfs_region = NL80211_DFS_ETSI,
	.reg_rules = {
	/* channels 1..13 */
	REG_RULE_LIGHT(2412-10, 2472+10, 40, 0),
	/* channels 36..48 */
	REG_RULE_LIGHT(5180-10, 5240+10, 80, KAL_RRF_AUTO_BW),
	/* channels 52..64 */
	REG_RULE_LIGHT(5260-10, 5320+10, 80, KAL_RRF_DFS | KAL_RRF_AUTO_BW),
	/* channels 100..140 */
	REG_RULE_LIGHT(5500-10, 5700+10, 160, KAL_RRF_DFS) }
};

const struct ieee80211_regdomain regdom_nl = {
	.n_reg_rules = 5,
	.dfs_region = NL80211_DFS_ETSI,
	.reg_rules = {
	/* channels 1..13 */
	REG_RULE_LIGHT(2412-10, 2472+10, 40, 0),
	/* channels 36..48 */
	REG_RULE_LIGHT(5180-10, 5240+10, 80, KAL_RRF_AUTO_BW),
	/* channels 52..64 */
	REG_RULE_LIGHT(5260-10, 5320+10, 80, KAL_RRF_DFS | KAL_RRF_AUTO_BW),
	/* channels 100..140 */
	REG_RULE_LIGHT(5500-10, 5700+10, 160, KAL_RRF_DFS),
	/* channels 100..140 */
	REG_RULE_LIGHT(5735-10, 5865+10, 80, 0) }
};

const struct ieee80211_regdomain regdom_jp = {
	.n_reg_rules = 7,
	.dfs_region = NL80211_DFS_JP,
	.reg_rules = {
	/* channels 1..13 */
	REG_RULE_LIGHT(2412-10, 2472+10, 40, 0),
	/* channels 14 */
	REG_RULE_LIGHT(2484-10, 2484+10, 20, KAL_RRF_NO_OFDM),
	/* channels 184..196 */
	REG_RULE_LIGHT(4920-10, 4980+10, 40, 0),
	/* channels 8..16 */
	REG_RULE_LIGHT(5040-10, 5080+10, 40, 0),
	/* channels 36..48 */
	REG_RULE_LIGHT(5180-10, 5240+10, 80, KAL_RRF_AUTO_BW),
	/* channels 52..64 */
	REG_RULE_LIGHT(5260-10, 5320+10, 80, KAL_RRF_DFS | KAL_RRF_AUTO_BW),
	/* channels 100..140 */
	REG_RULE_LIGHT(5500-10, 5700+10, 160, KAL_RRF_DFS) }
};

const struct ieee80211_regdomain regdom_tr = {
	.n_reg_rules = 4,
	.dfs_region = NL80211_DFS_ETSI,
	.reg_rules = {
	/* channels 1..13 */
	REG_RULE_LIGHT(2412-10, 2472+10, 40, 0),
	/* channels 36..48 */
	REG_RULE_LIGHT(5180-10, 5240+10, 80, KAL_RRF_AUTO_BW),
	/* channels 52..64 */
	REG_RULE_LIGHT(5260-10, 5320+10, 80, KAL_RRF_DFS | KAL_RRF_AUTO_BW),
	/* channels 100..140 */
	REG_RULE_LIGHT(5480-10, 5715+10, 160, KAL_RRF_DFS) }
};

const struct ieee80211_regdomain regdom_mx = {
	.n_reg_rules = 5,
	.dfs_region = NL80211_DFS_FCC,
	.reg_rules = {
	/* channels 1..13 */
	REG_RULE_LIGHT(2412-10, 2472+10, 40, 0),
	/* channels 36..48 */
	REG_RULE_LIGHT(5180-10, 5240+10, 80, KAL_RRF_AUTO_BW),
	/* channels 52..64 */
	REG_RULE_LIGHT(5260-10, 5320+10, 80, KAL_RRF_AUTO_BW),
	/* channels 100..140 */
	REG_RULE_LIGHT(5500-10, 5720+10, 160, KAL_RRF_DFS),
	/* channels 149..165 */
	REG_RULE_LIGHT(5745-10, 5825+10, 80, 0) }
};

const struct ieee80211_regdomain regdom_au = {
	.n_reg_rules = 6,
	.dfs_region = NL80211_DFS_ETSI,
	.reg_rules = {
	/* channels 1..13 */
	REG_RULE_LIGHT(2412-10, 2472+10, 40, 0),
	/* channels 36..48 */
	REG_RULE_LIGHT(5160-10, 5240+10, 80, KAL_RRF_AUTO_BW),
	/* channels 52..64 */
	REG_RULE_LIGHT(5260-10, 5340+10, 80, KAL_RRF_DFS | KAL_RRF_AUTO_BW),
	REG_RULE_LIGHT(5480-10, 5590+10, 80, KAL_RRF_DFS),
	/* channels 100..140 */
	REG_RULE_LIGHT(5660-10, 5720+10, 80, KAL_RRF_DFS),
	/* channels 149..165 */
	REG_RULE_LIGHT(5740-10, 5840+10, 80, 0) }
};

const struct ieee80211_regdomain regdom_ru = {
	.n_reg_rules = 5,
	.dfs_region = NL80211_DFS_ETSI,
	.reg_rules = {
	/* channels 1..13 */
	REG_RULE_LIGHT(2412-10, 2472+10, 40, 0),
	/* channels 36..48 */
	REG_RULE_LIGHT(5180-10, 5240+10, 80, KAL_RRF_AUTO_BW),
	/* channels 52..64 */
	REG_RULE_LIGHT(5260-10, 5320+10, 80, KAL_RRF_DFS | KAL_RRF_AUTO_BW),
	REG_RULE_LIGHT(5660-10, 5720+10, 80, KAL_RRF_DFS),
	/* channels 149..165 */
	REG_RULE_LIGHT(5745-10, 5825+10, 80, 0) }
};

const struct ieee80211_regdomain regdom_ng = {
	.n_reg_rules = 3,
	.dfs_region = NL80211_DFS_FCC,
	.reg_rules = {
	/* channels 1..13 */
	REG_RULE_LIGHT(2412-10, 2472+10, 40, 0),
	/* channels 52..64 */
	REG_RULE_LIGHT(5260-10, 5320+10, 80, KAL_RRF_DFS),
	/* channels 149..165 */
	REG_RULE_LIGHT(5745-10, 5825+10, 80, 0) }
};

const struct ieee80211_regdomain regdom_ae = {
	.n_reg_rules = 5,
	.dfs_region = NL80211_DFS_FCC,
	.reg_rules = {
	/* channels 1..13 */
	REG_RULE_LIGHT(2412-10, 2472+10, 40, 0),
	/* channels 36..48 */
	REG_RULE_LIGHT(5180-10, 5240+10, 80, KAL_RRF_AUTO_BW),
	/* channels 52..64 */
	REG_RULE_LIGHT(5260-10, 5320+10, 80, KAL_RRF_DFS | KAL_RRF_AUTO_BW),
	/* channels 100..144 */
	REG_RULE_LIGHT(5500-10, 5720+10, 160, KAL_RRF_DFS),
	/* channels 149..161 */
	REG_RULE_LIGHT(5745-10, 5825+10, 80, 0) }
};

const struct ieee80211_regdomain regdom_gt = {
	.n_reg_rules = 4,
	.dfs_region = NL80211_DFS_FCC,
	.reg_rules = {
	/* channels 1..13 */
	REG_RULE_LIGHT(2412-10, 2472+10, 40, 0),
	/* channels 36..48 */
	REG_RULE_LIGHT(5180-10, 5240+10, 80, KAL_RRF_AUTO_BW),
	/* channels 52..64 */
	REG_RULE_LIGHT(5260-10, 5320+10, 80, KAL_RRF_DFS | KAL_RRF_AUTO_BW),
	/* channels 149..161 */
	REG_RULE_LIGHT(5745-10, 5825+10, 80, 0) }
};

const struct ieee80211_regdomain regdom_sg = {
	.n_reg_rules = 4,
	.dfs_region = NL80211_DFS_FCC,
	.reg_rules = {
	/* channels 1..13 */
	REG_RULE_LIGHT(2412-10, 2472+10, 40, 0),
	/* channels 36..48 */
	REG_RULE_LIGHT(5160-10, 5240+10, 80, KAL_RRF_AUTO_BW),
	/* channels 52..64 */
	REG_RULE_LIGHT(5260-10, 5340+10, 80, KAL_RRF_DFS | KAL_RRF_AUTO_BW),
	/* channels 149..161 */
	REG_RULE_LIGHT(5735-10, 5840+10, 80, 0) }
};

const struct ieee80211_regdomain regdom_my = {
	.n_reg_rules = 5,
	.dfs_region = NL80211_DFS_FCC,
	.reg_rules = {
	/* channels 1..13 */
	REG_RULE_LIGHT(2412-10, 2472+10, 40, 0),
	/* channels 36..48 */
	REG_RULE_LIGHT(5180-10, 5240+10, 80, KAL_RRF_AUTO_BW),
	/* channels 52..64 */
	REG_RULE_LIGHT(5260-10, 5320+10, 80, KAL_RRF_DFS | KAL_RRF_AUTO_BW),
	REG_RULE_LIGHT(5500-10, 5640+10, 160, KAL_RRF_DFS),
	/* channels 149..165 */
	REG_RULE_LIGHT(5745-10, 5825+10, 80, 0) }
};

const struct ieee80211_regdomain regdom_hn = {
	.n_reg_rules = 5,
	.dfs_region = NL80211_DFS_FCC,
	.reg_rules = {
	/* channels 1..13 */
	REG_RULE_LIGHT(2412-10, 2472+10, 40, 0),
	/* channels 36..48 */
	REG_RULE_LIGHT(5180-10, 5240+10, 80, KAL_RRF_AUTO_BW),
	/* channels 52..64 */
	REG_RULE_LIGHT(5260-10, 5320+10, 80, KAL_RRF_DFS | KAL_RRF_AUTO_BW),
	REG_RULE_LIGHT(5500-10, 5720+10, 160, KAL_RRF_DFS),
	/* channels 149..165 */
	REG_RULE_LIGHT(5745-10, 5825+10, 80, 0) }
};

const struct ieee80211_regdomain regdom_ua = {
	.n_reg_rules = 5,
	.dfs_region = NL80211_DFS_FCC,
	.reg_rules = {
	/* channels 1..13 */
	REG_RULE_LIGHT(2412-10, 2472+10, 40, 0),
	/* channels 36..48 */
	REG_RULE_LIGHT(5160-10, 5240+10, 80, KAL_RRF_AUTO_BW),
	/* channels 52..64 */
	REG_RULE_LIGHT(5260-10, 5340+10, 80, KAL_RRF_DFS | KAL_RRF_AUTO_BW),
	REG_RULE_LIGHT(5500-10, 5660+10, 160, KAL_RRF_DFS),
	/* channels 149..165 */
	REG_RULE_LIGHT(5745-10, 5825+10, 80, 0) }
};

const struct ieee80211_regdomain regdom_in = {
	.n_reg_rules = 5,
	.reg_rules = {
	/* channels 1..13 */
	REG_RULE_LIGHT(2412-10, 2472+10, 40, 0),
	/* channels 36..48 */
	REG_RULE_LIGHT(5160-10, 5240+10, 80, 0),
	/* channels 52..64 */
	REG_RULE_LIGHT(5260-10, 5340+10, 80, 0),
	REG_RULE_LIGHT(5480-10, 5715+10, 160, 0),
	/* channels 149..165 */
	REG_RULE_LIGHT(5735-10, 5865+10, 80, 0) }
};

const struct ieee80211_regdomain regdom_id = {
	.n_reg_rules = 2,
	.dfs_region = NL80211_DFS_JP,
	.reg_rules = {
	/* channels 1..13 */
	REG_RULE_LIGHT(2412-10, 2472+10, 20, 0),
	/* channels 149..161 */
	REG_RULE_LIGHT(5745-10, 5805+10, 20, 0) }
};

const struct ieee80211_regdomain regdom_ao = {
	.n_reg_rules = 1,
	.dfs_region = NL80211_DFS_ETSI,
	.reg_rules = {
	/* channels 1..13 */
	REG_RULE_LIGHT(2412-10, 2472+10, 40, 0) }
};

const struct ieee80211_regdomain regdom_qa = {
	.n_reg_rules = 5,
	.dfs_region = NL80211_DFS_ETSI,
	.reg_rules = {
	/* channels 1..13 */
	REG_RULE_LIGHT(2412-10, 2472+10, 40, 0),
	REG_RULE_LIGHT(5160-10, 5240+10, 80, KAL_RRF_AUTO_BW),
	REG_RULE_LIGHT(5260-10, 5340+10, 80, KAL_RRF_DFS | KAL_RRF_AUTO_BW),
	REG_RULE_LIGHT(5480-10, 5715+10, 160, KAL_RRF_AUTO_BW),
	/* channels 149..165 */
	REG_RULE_LIGHT(5735-10, 5865+10, 80, 0) }
};

const struct ieee80211_regdomain regdom_kr = {
	.n_reg_rules = 5,
	.dfs_region = NL80211_DFS_JP,
	.reg_rules = {
	/* channels 1..13 */
	REG_RULE_LIGHT(2412-10, 2472+10, 40, 0),
	/* channels 36..48 */
	REG_RULE_LIGHT(5180-10, 5240+10, 80, KAL_RRF_AUTO_BW),
	/* channels 52..64 */
	REG_RULE_LIGHT(5260-10, 5320+10, 80, KAL_RRF_DFS | KAL_RRF_AUTO_BW),
	/* channels 100..128 */
	REG_RULE_LIGHT(5500-10, 5700+10, 160, KAL_RRF_DFS),
	/* channels 149..161 */
	REG_RULE_LIGHT(5745-10, 5825+10, 80, 0) }
};

const struct ieee80211_regdomain regdom_ke = {
	.n_reg_rules = 4,
	.dfs_region = NL80211_DFS_JP,
	.reg_rules = {
	/* channels 1..13 */
	REG_RULE_LIGHT(2412-10, 2472+10, 40, 0),
	/* channels 36..48 */
	REG_RULE_LIGHT(5180-10, 5240+10, 80, 0),
	/* channels 100..128 */
	REG_RULE_LIGHT(5500-10, 5560+10, 80, KAL_RRF_DFS),
	/* channels 149..161 */
	REG_RULE_LIGHT(5745-10, 5765+10, 40, 0) }
};

const struct ieee80211_regdomain regdom_jo = {
	.n_reg_rules = 3,
	.dfs_region = NL80211_DFS_JP,
	.reg_rules = {
	/* channels 1..13 */
	REG_RULE_LIGHT(2412-10, 2472+10, 40, 0),
	/* channels 36..48 */
	REG_RULE_LIGHT(5180-10, 5240+10, 80, 0),
	/* channels 149..165 */
	REG_RULE_LIGHT(5745-10, 5825+10, 80, 0) }
};

const struct ieee80211_regdomain regdom_ir = {
	.n_reg_rules = 2,
	.dfs_region = NL80211_DFS_JP,
	.reg_rules = {
	/* channels 1..13 */
	REG_RULE_LIGHT(2412-10, 2472+10, 40, 0),
	/* channels 149..165 */
	REG_RULE_LIGHT(5745-10, 5825+10, 80, 0) }
};

const struct ieee80211_regdomain regdom_sy = {
	.n_reg_rules = 1,
	.reg_rules = {
	/* channels 1..13 */
	REG_RULE_LIGHT(2412-10, 2472+10, 40, 0) }
};

const struct ieee80211_regdomain regdom_il = {
	.n_reg_rules = 3,
	.dfs_region = NL80211_DFS_ETSI,
	.reg_rules = {
	/* channels 1..13 */
	REG_RULE_LIGHT(2412-10, 2472+10, 40, 0),
	/* channels 36..48 */
	REG_RULE_LIGHT(5160-10, 5240+10, 80, KAL_RRF_AUTO_BW),
	/* channels 52..64 */
	REG_RULE_LIGHT(5260-10, 5340+10, 80, KAL_RRF_DFS | KAL_RRF_AUTO_BW) }
};

const struct ieee80211_regdomain regdom_eg = {
	.n_reg_rules = 3,
	.dfs_region = NL80211_DFS_ETSI,
	.reg_rules = {
	/* channels 1..13 */
	REG_RULE_LIGHT(2412-10, 2472+10, 40, 0),
	/* channels 36..48 */
	REG_RULE_LIGHT(5180-10, 5240+10, 40, 0),
	/* channels 52..64 */
	REG_RULE_LIGHT(5260-10, 5320+10, 40, KAL_RRF_DFS) }
};

const struct ieee80211_regdomain regdom_sa = {
	.n_reg_rules = 4,
	.dfs_region = NL80211_DFS_ETSI,
	.reg_rules = {
	/* channels 1..13 */
	REG_RULE_LIGHT(2412-10, 2472+10, 40, 0),
	/* channels 36..48 */
	REG_RULE_LIGHT(5180-10, 5240+10, 80, KAL_RRF_AUTO_BW),
	/* channels 52..64 */
	REG_RULE_LIGHT(5260-10, 5320+10, 80, KAL_RRF_DFS | KAL_RRF_AUTO_BW),
	REG_RULE_LIGHT(5500-10, 5700+10, 160, KAL_RRF_DFS) }
};

const struct ieee80211_regdomain regdom_bh = {
	.n_reg_rules = 4,
	.dfs_region = NL80211_DFS_JP,
	.reg_rules = {
	/* channels 1..13 */
	REG_RULE_LIGHT(2412-10, 2472+10, 40, 0),
	/* channels 36..48 */
	REG_RULE_LIGHT(5180-10, 5240+10, 20, 0),
	/* channels 36..48 */
	REG_RULE_LIGHT(5260-10, 5320+10, 20, KAL_RRF_DFS),
	/* channels 52..64 */
	REG_RULE_LIGHT(5745-10, 5825+10, 80, 0) }
};

const struct ieee80211_regdomain regdom_dz = {
	.n_reg_rules = 4,
	.dfs_region = NL80211_DFS_JP,
	.reg_rules = {
	/* channels 1..13 */
	REG_RULE_LIGHT(2412-10, 2472+10, 40, 0),
	/* channels 36..48 */
	REG_RULE_LIGHT(5180-10, 5240+10, 80, KAL_RRF_AUTO_BW),
	/* channels 36..48 */
	REG_RULE_LIGHT(5260-10, 5320+10, 80, KAL_RRF_DFS | KAL_RRF_AUTO_BW),
	/* channels 52..64 */
	REG_RULE_LIGHT(5500-10, 5660+10, 160, KAL_RRF_DFS) }
};

/*
 * Step2. Decclare struct mtk_regdomain
 */

const struct mtk_regdomain my_regdom_us01 = {
	.country_code = "US01",
	.prRegdRules = &regdom_us01
};

const struct mtk_regdomain my_regdom_tw = {
	.country_code = "TW",
	.prRegdRules = &regdom_tw
};

const struct mtk_regdomain my_regdom_ao = {
	.country_code = "AO",
	.prRegdRules = &regdom_ao
};

const struct mtk_regdomain my_regdom_us = {
	.country_code = "US",
	.prRegdRules = &regdom_us
};

const struct mtk_regdomain my_regdom_ca = {
	.country_code = "CA",
	.prRegdRules = &regdom_ca
};

const struct mtk_regdomain my_regdom_ng = {
	.country_code = "NG",
	.prRegdRules = &regdom_ng
};

const struct mtk_regdomain my_regdom_qa = {
	.country_code = "QA",
	.prRegdRules = &regdom_qa
};

const struct mtk_regdomain my_regdom_pk = {
	.country_code = "PK",
	.prRegdRules = &regdom_ir
};

const struct mtk_regdomain my_regdom_hn = {
	.country_code = "HN",
	.prRegdRules = &regdom_hn
};

const struct mtk_regdomain my_regdom_at = {
	.country_code = "AT",
	.prRegdRules = &regdom_at
};

const struct mtk_regdomain my_regdom_de = {
	.country_code = "DE",
	.prRegdRules = &regdom_cz_nl
};

const struct mtk_regdomain my_regdom_es = {
	.country_code = "ES",
	.prRegdRules = &regdom_cz_nl
};

const struct mtk_regdomain my_regdom_fr = {
	.country_code = "FR",
	.prRegdRules = &regdom_cz_nl
};

const struct mtk_regdomain my_regdom_hr = {
	.country_code = "HR",
	.prRegdRules = &regdom_hr
};

const struct mtk_regdomain my_regdom_by = {
	.country_code = "BY",
	.prRegdRules = &regdom_at
};

const struct mtk_regdomain my_regdom_iq = {
	.country_code = "IQ",
	.prRegdRules = &regdom_cz_nl
};

const struct mtk_regdomain my_regdom_it = {
	.country_code = "IT",
	.prRegdRules = &regdom_at
};

const struct mtk_regdomain my_regdom_gb = {
	.country_code = "GB",
	.prRegdRules = &regdom_at
};

const struct mtk_regdomain my_regdom_se = {
	.country_code = "SE",
	.prRegdRules = &regdom_cz_nl
};

const struct mtk_regdomain my_regdom_cn = {
	.country_code = "CN",
	.prRegdRules = &regdom_cn
};

const struct mtk_regdomain my_regdom_bh = {
	.country_code = "BH",
	.prRegdRules = &regdom_bh
};

const struct mtk_regdomain my_regdom_eg = {
	.country_code = "EG",
	.prRegdRules = &regdom_eg
};

const struct mtk_regdomain my_regdom_zm = {
	.country_code = "ZM",
	.prRegdRules = &regdom_cn
};

const struct mtk_regdomain my_regdom_nl = {
	.country_code = "NL",
	.prRegdRules = &regdom_nl
};

const struct mtk_regdomain my_regdom_cz = {
	.country_code = "CZ",
	.prRegdRules = &regdom_hr
};

const struct mtk_regdomain my_regdom_jp = {
	.country_code = "JP",
	.prRegdRules = &regdom_jp
};

const struct mtk_regdomain my_regdom_mx = {
	.country_code = "MX",
	.prRegdRules = &regdom_mx
};

const struct mtk_regdomain my_regdom_au = {
	.country_code = "AU",
	.prRegdRules = &regdom_au
};

const struct mtk_regdomain my_regdom_ar = {
	.country_code = "AR",
	.prRegdRules = &regdom_ae
};

const struct mtk_regdomain my_regdom_lb = {
	.country_code = "LB",
	.prRegdRules = &regdom_ae
};

const struct mtk_regdomain my_regdom_hk = {
	.country_code = "HK",
	.prRegdRules = &regdom_ae
};

const struct mtk_regdomain my_regdom_nz = {
	.country_code = "NZ",
	.prRegdRules = &regdom_ae
};

const struct mtk_regdomain my_regdom_gt = {
	.country_code = "GT",
	.prRegdRules = &regdom_gt
};

const struct mtk_regdomain my_regdom_cr = {
	.country_code = "CR",
	.prRegdRules = &regdom_ae
};

const struct mtk_regdomain my_regdom_ec = {
	.country_code = "EC",
	.prRegdRules = &regdom_ae
};

const struct mtk_regdomain my_regdom_sg = {
	.country_code = "SG",
	.prRegdRules = &regdom_sg
};

const struct mtk_regdomain my_regdom_id = {
	.country_code = "ID",
	.prRegdRules = &regdom_id
};

const struct mtk_regdomain my_regdom_vn = {
	.country_code = "VN",
	.prRegdRules = &regdom_ae
};

const struct mtk_regdomain my_regdom_my = {
	.country_code = "MY",
	.prRegdRules = &regdom_my
};

const struct mtk_regdomain my_regdom_sv = {
	.country_code = "SV",
	.prRegdRules = &regdom_gt
};

const struct mtk_regdomain my_regdom_ve = {
	.country_code = "VE",
	.prRegdRules = &regdom_gt
};

const struct mtk_regdomain my_regdom_ph = {
	.country_code = "PH",
	.prRegdRules = &regdom_hn
};

const struct mtk_regdomain my_regdom_in = {
	.country_code = "IN",
	.prRegdRules = &regdom_in
};

const struct mtk_regdomain my_regdom_kr = {
	.country_code = "KR",
	.prRegdRules = &regdom_kr
};

const struct mtk_regdomain my_regdom_ru = {
	.country_code = "RU",
	.prRegdRules = &regdom_ru
};

const struct mtk_regdomain my_regdom_gr = {
	.country_code = "GR",
	.prRegdRules = &regdom_at
};

const struct mtk_regdomain my_regdom_pl = {
	.country_code = "PL",
	.prRegdRules = &regdom_at
};

const struct mtk_regdomain my_regdom_hu = {
	.country_code = "HU",
	.prRegdRules = &regdom_cz_nl
};

const struct mtk_regdomain my_regdom_pa = {
	.country_code = "PA",
	.prRegdRules = &regdom_pa
};

const struct mtk_regdomain my_regdom_co = {
	.country_code = "CO",
	.prRegdRules = &regdom_mx
};

const struct mtk_regdomain my_regdom_cl = {
	.country_code = "CL",
	.prRegdRules = &regdom_bh
};

const struct mtk_regdomain my_regdom_pe = {
	.country_code = "PE",
	.prRegdRules = &regdom_hn
};

const struct mtk_regdomain my_regdom_th = {
	.country_code = "TH",
	.prRegdRules = &regdom_hn
};

const struct mtk_regdomain my_regdom_ae = {
	.country_code = "AE",
	.prRegdRules = &regdom_hn
};

const struct mtk_regdomain my_regdom_tr = {
	.country_code = "TR",
	.prRegdRules = &regdom_hr
};

const struct mtk_regdomain my_regdom_be = {
	.country_code = "BE",
	.prRegdRules = &regdom_at
};

const struct mtk_regdomain my_regdom_bg = {
	.country_code = "BG",
	.prRegdRules = &regdom_nl
};

const struct mtk_regdomain my_regdom_cy = {
	.country_code = "CY",
	.prRegdRules = &regdom_at
};

const struct mtk_regdomain my_regdom_dk = {
	.country_code = "DK",
	.prRegdRules = &regdom_hr
};

const struct mtk_regdomain my_regdom_ee = {
	.country_code = "EE",
	.prRegdRules = &regdom_at
};

const struct mtk_regdomain my_regdom_no = {
	.country_code = "NO",
	.prRegdRules = &regdom_cz_no
};

const struct mtk_regdomain my_regdom_fi = {
	.country_code = "FI",
	.prRegdRules = &regdom_cz_nl
};

const struct mtk_regdomain my_regdom_ie = {
	.country_code = "IE",
	.prRegdRules = &regdom_cz_nl
};

const struct mtk_regdomain my_regdom_lv = {
	.country_code = "LV",
	.prRegdRules = &regdom_at
};

const struct mtk_regdomain my_regdom_lt = {
	.country_code = "LT",
	.prRegdRules = &regdom_at
};

const struct mtk_regdomain my_regdom_lu = {
	.country_code = "LU",
	.prRegdRules = &regdom_at
};

const struct mtk_regdomain my_regdom_mt = {
	.country_code = "MT",
	.prRegdRules = &regdom_at
};

const struct mtk_regdomain my_regdom_pt = {
	.country_code = "PT",
	.prRegdRules = &regdom_nl
};

const struct mtk_regdomain my_regdom_ke = {
	.country_code = "KE",
	.prRegdRules = &regdom_ke
};

const struct mtk_regdomain my_regdom_om = {
	.country_code = "OM",
	.prRegdRules = &regdom_at
};

const struct mtk_regdomain my_regdom_ro = {
	.country_code = "RO",
	.prRegdRules = &regdom_at
};

const struct mtk_regdomain my_regdom_dz = {
	.country_code = "DZ",
	.prRegdRules = &regdom_dz
};

const struct mtk_regdomain my_regdom_sk = {
	.country_code = "SK",
	.prRegdRules = &regdom_at
};

const struct mtk_regdomain my_regdom_si = {
	.country_code = "SI",
	.prRegdRules = &regdom_at
};

const struct mtk_regdomain my_regdom_sa = {
	.country_code = "SA",
	.prRegdRules = &regdom_sa
};

const struct mtk_regdomain my_regdom_sn = {
	.country_code = "SN",
	.prRegdRules = &regdom_us
};

const struct mtk_regdomain my_regdom_jo = {
	.country_code = "JO",
	.prRegdRules = &regdom_jo
};

const struct mtk_regdomain my_regdom_za = {
	.country_code = "ZA",
	.prRegdRules = &regdom_at
};

const struct mtk_regdomain my_regdom_ir = {
	.country_code = "IR",
	.prRegdRules = &regdom_ir
};

const struct mtk_regdomain my_regdom_ps = {
	.country_code = "PS",
	.prRegdRules = &regdom_ir
};

const struct mtk_regdomain my_regdom_sd = {
	.country_code = "SD",
	.prRegdRules = &regdom_ir
};

const struct mtk_regdomain my_regdom_sy = {
	.country_code = "SY",
	.prRegdRules = &regdom_sy
};

const struct mtk_regdomain my_regdom_il = {
	.country_code = "IL",
	.prRegdRules = &regdom_il
};

const struct mtk_regdomain my_regdom_am = {
	.country_code = "AM",
	.prRegdRules = &regdom_eg
};

const struct mtk_regdomain my_regdom_az = {
	.country_code = "AZ",
	.prRegdRules = &regdom_eg
};

const struct mtk_regdomain my_regdom_ma = {
	.country_code = "MA",
	.prRegdRules = &regdom_eg
};

const struct mtk_regdomain my_regdom_kw = {
	.country_code = "KW",
	.prRegdRules = &regdom_eg
};

const struct mtk_regdomain my_regdom_br = {
	.country_code = "BR",
	.prRegdRules = &regdom_mx
};

const struct mtk_regdomain my_regdom_ua = {
	.country_code = "UA",
	.prRegdRules = &regdom_ua
};

const struct mtk_regdomain my_regdom_kz = {
	.country_code = "KZ",
	.prRegdRules = &regdom_hr
};

const struct mtk_regdomain my_regdom_dm = {
	.country_code = "DM",
	.prRegdRules = &regdom_gt
};

/*
 * Step3. Register to table
 */

const struct mtk_regdomain *g_prRegRuleTable[] = {
	&my_regdom_us01,
	&my_regdom_us,
	&my_regdom_cn,
	&my_regdom_nl,
	&my_regdom_cz,
	&my_regdom_jp,
	&my_regdom_ca,
	&my_regdom_at,
	&my_regdom_de,
	&my_regdom_es,
	&my_regdom_fr,
	&my_regdom_it,
	&my_regdom_gb,
	&my_regdom_se,
	&my_regdom_mx,
	&my_regdom_au,
	&my_regdom_sg,
	&my_regdom_id,
	&my_regdom_vn,
	&my_regdom_my,
	&my_regdom_ph,
	&my_regdom_in,
	&my_regdom_kr,
	&my_regdom_ru,
	&my_regdom_gr,
	&my_regdom_pl,
	&my_regdom_hu,
	&my_regdom_pa,
	&my_regdom_co,
	&my_regdom_cl,
	&my_regdom_pe,
	&my_regdom_th,
	&my_regdom_ae,
	&my_regdom_tr,
	&my_regdom_sn,
	&my_regdom_jo,
	&my_regdom_za,
	&my_regdom_ir,
	&my_regdom_il,
	&my_regdom_br,
	&my_regdom_ar,
	&my_regdom_dm,
	&my_regdom_ec,
	&my_regdom_ve,
	&my_regdom_gt,
	&my_regdom_sv,
	&my_regdom_nz,
	&my_regdom_ua,
	&my_regdom_kz,
	&my_regdom_be,
	&my_regdom_bg,
	&my_regdom_cy,
	&my_regdom_dk,
	&my_regdom_ee,
	&my_regdom_no,
	&my_regdom_ie,
	&my_regdom_lv,
	&my_regdom_lt,
	&my_regdom_lu,
	&my_regdom_mt,
	&my_regdom_pt,
	&my_regdom_ro,
	&my_regdom_sk,
	&my_regdom_si,
	&my_regdom_kw,
	&my_regdom_om,
	&my_regdom_sa,
	&my_regdom_bh,
	&my_regdom_lb,
	&my_regdom_ke,
	&my_regdom_zm,
	&my_regdom_sd,
	&my_regdom_dz,
	&my_regdom_sy,
	&my_regdom_co,
	&my_regdom_eg,
	&my_regdom_ma,
	&my_regdom_qa,
	&my_regdom_pk,
	&my_regdom_hn,
	&my_regdom_ng,
	&my_regdom_tw,
	&my_regdom_ao,
	&my_regdom_iq,
	&my_regdom_ps,
	&my_regdom_am,
	&my_regdom_az,
	&my_regdom_hr,
	&my_regdom_by,
	&my_regdom_hk,
	&my_regdom_cr,
	&my_regdom_fi,
	NULL /* this NULL SHOULD be at the end of the array */
};

#endif
#endif
