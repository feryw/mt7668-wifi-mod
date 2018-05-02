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

#ifndef _GL_GPIO_H
#define _GL_GPIO_H

#include "gl_os.h"

#ifdef CFG_USE_LINUX_GPIO_GLUE

/*******************************************************************************
*                         C O M P I L E R   F L A G S
********************************************************************************
*/

/*******************************************************************************
*                    E X T E R N A L   R E F E R E N C E S
********************************************************************************
*/

#include <linux/of.h>

/*******************************************************************************
*                              C O N S T A N T S
********************************************************************************
*/

#define WLAN_GPIO_PIN_NAME "mt7668-wifi-gpio"
#define WLAN_GPIO_PIN_NAME_LEN 32

#define  WLAN_GPIO_PIN_WOW_DEBOUNCE_TIME 100 /* ms */


enum {
	WLAN_GPIO_WOW_PIN,
	WLAN_GPIO_BCN_TSF_PIN,
	WLAN_GPIO_PIN_ID_MAX
};

/*******************************************************************************
*                             D A T A   T Y P E S
********************************************************************************
*/

struct GPIO_CTRL_INFO {
	CHAR pinName[WLAN_GPIO_PIN_NAME_LEN];
	INT_32 pinIndex;
	INT_32 gpio;
	INT_32 irq;
	INT_32 debounce;
};

struct GPIO_GLUE_INFO {
	struct GPIO_CTRL_INFO pinInfo[WLAN_GPIO_PIN_ID_MAX];
	struct device_node *np;
};

/*******************************************************************************
*                  F U N C T I O N   D E C L A R A T I O N S
********************************************************************************
*/
INT_32 wlanPlatformCfgInit(void);
void wlanPlatformCfgDestroy(void);
struct GPIO_GLUE_INFO *wlanPlatformCfgGet(void);

#endif /* CFG_USE_LINUX_GPIO_GLUE */

#endif /* _GL_GPIO_H */

