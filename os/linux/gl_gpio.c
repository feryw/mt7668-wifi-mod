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
#include <linux/of_gpio.h>
#include <linux/of_irq.h>
#include "gl_gpio.h"

/*******************************************************************************
*                              C O N S T A N T S
********************************************************************************
*/

/*******************************************************************************
*                             D A T A   T Y P E S
********************************************************************************
*/

/*******************************************************************************
*                            P U B L I C   D A T A
********************************************************************************
*/
static struct GPIO_GLUE_INFO gGpioGlueInfo = {
	{
		{
			.pinName = "mt7668-wowlan",
			.pinIndex = WLAN_GPIO_WOW_PIN,
			.debounce = WLAN_GPIO_PIN_WOW_DEBOUNCE_TIME,
			.gpio = 0,
			.irq = 0,
		},
		{
			.pinName = "mt7668-bcn-tsf",
			.pinIndex = WLAN_GPIO_BCN_TSF_PIN,
			.debounce = 0,
			.gpio = 0,
			.irq = 0,
		},
	}
};

/*******************************************************************************
*                           P R I V A T E   D A T A
********************************************************************************
*/

/*******************************************************************************
*                                 M A C R O S
********************************************************************************
*/

/*******************************************************************************
*                  F U N C T I O N   D E C L A R A T I O N S
********************************************************************************
*/

/*******************************************************************************
*                              F U N C T I O N S
********************************************************************************
*/

/**
 * _wlanGpioAlloc - GPIO pin allocated
 * @np: wifi device node in dts file
 * @pGpioCtrl: ctrl information for GPIO pin
 */
static INT_32 _wlanGpioAlloc(struct device_node *np,
	struct GPIO_CTRL_INFO *pGpioCtrl)
{
	INT_32 ret = 0;

	/* Parameter Check */
	if (np == NULL || pGpioCtrl == NULL) {
		DBGLOG(INIT, STATE, "%s parameter check fail !\n", __func__);
		return -EINVAL;
	}

	/* debug info */
	DBGLOG(INIT, STATE, "%s pinName=%s\n", __func__, pGpioCtrl->pinName);
	DBGLOG(INIT, STATE, "%s pinIndex=%d\n", __func__, pGpioCtrl->pinIndex);
	DBGLOG(INIT, STATE, "%s debounce=%d\n", __func__, pGpioCtrl->debounce);

	/* init gpio  */
	pGpioCtrl->gpio = of_get_named_gpio(np, WLAN_GPIO_PIN_NAME,
					pGpioCtrl->pinIndex);

	if (!gpio_is_valid(pGpioCtrl->gpio)) {
		DBGLOG(INIT, STATE, "%s get invalid %s gpio %d\n",
				__func__, pGpioCtrl->pinName, pGpioCtrl->gpio);
		return -EINVAL;
	}

	DBGLOG(INIT, STATE, "%s get %s gpio(%d) suscessfully\n",
		__func__, pGpioCtrl->pinName, pGpioCtrl->gpio);

	ret = gpio_request(pGpioCtrl->gpio, pGpioCtrl->pinName);
	if (ret < 0) {
		DBGLOG(INIT, STATE, "%s request %s gpio(%d) fail\n",
				__func__, pGpioCtrl->pinName, pGpioCtrl->gpio);
		return ret;
	}

	ret = gpio_direction_input(pGpioCtrl->gpio);
	if (ret != 0) {
		DBGLOG(INIT, STATE, "%s set %s gpio(%d) to input mode fail\n",
				__func__, pGpioCtrl->pinName, pGpioCtrl->gpio);
		gpio_free(pGpioCtrl->gpio);
		return ret;
	}

	DBGLOG(INIT, STATE, "%s set %s gpio(%d) to input mode\n",
		__func__, pGpioCtrl->pinName, pGpioCtrl->gpio);

	if (pGpioCtrl->debounce) {
		gpio_set_debounce(pGpioCtrl->gpio, pGpioCtrl->debounce);
		if (ret < 0) {
			DBGLOG(INIT, STATE,
				"%s set %s gpio(%d) debounce fail\n",
				__func__, pGpioCtrl->pinName, pGpioCtrl->gpio);
			gpio_free(pGpioCtrl->gpio);
			return ret;
		}

		DBGLOG(INIT, STATE, "%s set %s gpio(%d) debounce %d\n",
			__func__, pGpioCtrl->pinName, pGpioCtrl->gpio,
			pGpioCtrl->debounce);
	}

	/* init irq */
	pGpioCtrl->irq = of_irq_get(np, pGpioCtrl->pinIndex);

	DBGLOG(INIT, STATE, "%s get %s irq(%d) suscessfully\n",
		__func__, pGpioCtrl->pinName, pGpioCtrl->irq);

	return ret;
}

/**
 * wlanPlatformCfgInit - platform resource initialized for wlan
 */
INT_32 wlanPlatformCfgInit(void)
{
	struct GPIO_GLUE_INFO *prGpioGlueInfo = &gGpioGlueInfo;
	INT_32 ret = 0;

	/* get wlan device node */
	prGpioGlueInfo->np = of_find_compatible_node(NULL,
				NULL, "mediatek,mt7668-wifi");
	if (prGpioGlueInfo->np == NULL) {
		DBGLOG(INIT, STATE, "%s can't find device tree node!\n",
				__func__);
		return -EINVAL;
	}

	/* init WOW PIN */
	ret = _wlanGpioAlloc(prGpioGlueInfo->np,
			&prGpioGlueInfo->pinInfo[WLAN_GPIO_WOW_PIN]);
	if (ret != 0) {
		DBGLOG(INIT, STATE, "%s _wlanGpioAlloc wow fail(%d)!\n",
				__func__, ret);
		return ret;
	}

	/* init BCN TSF PIN */
	ret = _wlanGpioAlloc(prGpioGlueInfo->np,
			&prGpioGlueInfo->pinInfo[WLAN_GPIO_BCN_TSF_PIN]);
	if (ret != 0) {
		DBGLOG(INIT, STATE, "%s _wlanGpioAlloc bcn tsf fail(%d)!\n",
				__func__, ret);
		return ret;
	}

	return ret;
}

/**
 * wlanPlatformCfgDestroy - remove platform resource for wifi
 */
void wlanPlatformCfgDestroy(void)
{
	struct GPIO_GLUE_INFO *prGpioGlueInfo = &gGpioGlueInfo;

	gpio_free(prGpioGlueInfo->pinInfo[WLAN_GPIO_WOW_PIN].gpio);

	gpio_free(prGpioGlueInfo->pinInfo[WLAN_GPIO_BCN_TSF_PIN].gpio);

	kalMemZero(prGpioGlueInfo, sizeof(struct GPIO_CTRL_INFO));
}

struct GPIO_GLUE_INFO *wlanPlatformCfgGet(void)
{
	return &gGpioGlueInfo;
}

#endif /* CFG_USE_LINUX_GPIO_GLUE */


