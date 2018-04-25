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
/*
** Id: //Department/DaVinci/BRANCHES/MT6620_WIFI_DRIVER_V2_3/mgmt/assoc.c#5
*/

/*! \file   "assoc.c"
*    \brief  This file includes the association-related functions.
*
*    This file includes the association-related functions.
*/

/*******************************************************************************
*                         C O M P I L E R   F L A G S
********************************************************************************
*/

/*******************************************************************************
*                    E X T E R N A L   R E F E R E N C E S
********************************************************************************
*/
#include "precomp.h"

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
APPEND_VAR_IE_ENTRY_T txAssocReqIETable[] = {
#if CFG_SUPPORT_SPEC_MGMT
	{(ELEM_HDR_LEN + ELEM_MAX_LEN_POWER_CAP), NULL, rlmReqGeneratePowerCapIE}
	,			/* 33 */
	{(ELEM_HDR_LEN + ELEM_MAX_LEN_SUPPORTED_CHANNELS), NULL, rlmReqGenerateSupportedChIE}
	,			/* 36 */
#endif
	{(ELEM_HDR_LEN + ELEM_MAX_LEN_HT_CAP), NULL, rlmReqGenerateHtCapIE}
	,			/* 45 */
#if CFG_SUPPORT_WPS2
	{(ELEM_HDR_LEN + ELEM_MAX_LEN_WSC), NULL, rsnGenerateWSCIE}
	,			/* 221 */
#endif
#if CFG_SUPPORT_WAPI
	{(ELEM_HDR_LEN + ELEM_MAX_LEN_WAPI), NULL, wapiGenerateWAPIIE}
	,			/* 68 */
#endif
#if CFG_SUPPORT_PASSPOINT
	{(ELEM_HDR_LEN + ELEM_MAX_LEN_INTERWORKING), NULL, hs20GenerateInterworkingIE}
	,			/* 107 */
	{(ELEM_HDR_LEN + ELEM_MAX_LEN_ROAMING_CONSORTIUM), NULL, hs20GenerateRoamingConsortiumIE}
	,			/* 111 */
	{(ELEM_HDR_LEN + ELEM_MAX_LEN_HS20_INDICATION), NULL, hs20GenerateHS20IE}
	,			/* 221 */
#endif /* CFG_SUPPORT_PASSPOINT */
	{(ELEM_HDR_LEN + ELEM_MAX_LEN_EXT_CAP), NULL, rlmReqGenerateExtCapIE}
	,			/* 127 */
	{(ELEM_HDR_LEN + ELEM_MAX_LEN_WMM_INFO), NULL, mqmGenerateWmmInfoIE}
	,			/* 221 */
	{(ELEM_HDR_LEN + ELEM_MAX_LEN_RSN), NULL, rsnGenerateRSNIE}
	,			/* 48 */
#if CFG_SUPPORT_802_11AC
	{(ELEM_HDR_LEN + ELEM_MAX_LEN_VHT_CAP), NULL, rlmReqGenerateVhtCapIE}
	,			/*191 */
	{(ELEM_HDR_LEN + ELEM_MAX_LEN_VHT_OP_MODE_NOTIFICATION), NULL, rlmReqGenerateVhtOpNotificationIE}
	,			/*199 */
#endif
#if CFG_SUPPORT_MTK_SYNERGY
	{(ELEM_HDR_LEN + ELEM_MIN_LEN_MTK_OUI), NULL, rlmGenerateMTKOuiIE}
	,			/* 221 */
#endif
	{(ELEM_HDR_LEN + ELEM_MAX_LEN_WPA), NULL, rsnGenerateWPAIE}	/* 221 */
};

#if CFG_SUPPORT_AAA
VERIFY_IE_ENTRY_T rxAssocReqIETable[] = {
	{ELEM_ID_RESERVED, NULL}	/* 255 */
};

APPEND_VAR_IE_ENTRY_T txAssocRespIETable[] = {
	{(ELEM_HDR_LEN + ELEM_MAX_LEN_ERP), NULL, rlmRspGenerateErpIE}
	,			/* 42 */
	{(ELEM_HDR_LEN + ELEM_MAX_LEN_HT_CAP), NULL, rlmRspGenerateHtCapIE}
	,			/* 45 */
	{(ELEM_HDR_LEN + ELEM_MAX_LEN_HT_OP), NULL, rlmRspGenerateHtOpIE}
	,			/* 61 */
#if CFG_ENABLE_WIFI_DIRECT
	{(ELEM_HDR_LEN + ELEM_MAX_LEN_OBSS_SCAN), NULL, rlmRspGenerateObssScanIE}
	,			/* 74 */
	{(0), p2pFuncCalculateP2p_IELenForAssocRsp, p2pFuncGenerateP2p_IEForAssocRsp}
	,			/* 221 */
#if (CFG_SUPPORT_WFD)
	{(0), wfdFuncCalculateWfdIELenForAssocRsp, wfdFuncGenerateWfdIEForAssocRsp}
	,			/* 221 */
#endif
#endif
	{(ELEM_HDR_LEN + ELEM_MAX_LEN_EXT_CAP), NULL, rlmRspGenerateExtCapIE}
	,			/* 127 */
#if CFG_SUPPORT_802_11AC
	{(ELEM_HDR_LEN + ELEM_MAX_LEN_VHT_CAP), NULL, rlmRspGenerateVhtCapIE}
	,			/*191 */
	{(ELEM_HDR_LEN + ELEM_MAX_LEN_VHT_OP), NULL, rlmRspGenerateVhtOpIE}
	,			/*192 */
	{(ELEM_HDR_LEN + ELEM_MAX_LEN_VHT_OP_MODE_NOTIFICATION), NULL, rlmRspGenerateVhtOpNotificationIE}
	,			/*199 */
#endif
	{(ELEM_HDR_LEN + ELEM_MAX_LEN_WMM_PARAM), NULL, mqmGenerateWmmParamIE}
	,			/* 221 */
	{(ELEM_HDR_LEN + ELEM_MAX_LEN_ASSOC_RSP_WSC_IE), NULL, rsnGenerateWSCIEForAssocRsp}
	,			/* 221 */
#if CFG_SUPPORT_MTK_SYNERGY
	{(ELEM_HDR_LEN + ELEM_MIN_LEN_MTK_OUI), NULL, rlmGenerateMTKOuiIE}	/* 221 */
#endif
	,
#if CFG_SUPPORT_802_11W
	{(ELEM_HDR_LEN + ELEM_MAX_LEN_TIMEOUT_IE), NULL, rsnPmfGenerateTimeoutIE}	/* 56 */
#endif

};
#endif /* CFG_SUPPORT_AAA */

/*******************************************************************************
*                           P R I V A T E   D A T A
********************************************************************************
*/

/*******************************************************************************
*                                 M A C R O S
********************************************************************************
*/

/*******************************************************************************
*                   F U N C T I O N   D E C L A R A T I O N S
********************************************************************************
*/

/*******************************************************************************
*                              F U N C T I O N S
********************************************************************************
*/
/*----------------------------------------------------------------------------*/
/*!
* @brief This function is used to compose the Capability Info Field.
*
* @param[in] prStaRec               Pointer to the STA_RECORD_T
*
* @retval Capability Info Field
*/
/*----------------------------------------------------------------------------*/

UINT_16 assocBuildCapabilityInfo(IN P_ADAPTER_T prAdapter, IN P_STA_RECORD_T prStaRec)
{
	UINT_32 u4NonHTPhyType;
	UINT_16 u2CapInfo;
	P_BSS_INFO_T prBssInfo;

	ASSERT(prStaRec);

	prBssInfo = GET_BSS_INFO_BY_INDEX(prAdapter, prStaRec->ucBssIndex);

	/* Set up our requested capabilities. */
	u2CapInfo = CAP_INFO_ESS;
	u2CapInfo |= CAP_CF_STA_NOT_POLLABLE;

	if (prStaRec->u2CapInfo & CAP_INFO_PRIVACY)
		u2CapInfo |= CAP_INFO_PRIVACY;

	/* 7.3.1.4 */
	if (prStaRec->fgHasBasicPhyType) {
		u4NonHTPhyType = prStaRec->ucNonHTBasicPhyType;

		if ((rNonHTPhyAttributes[u4NonHTPhyType].fgIsShortPreambleOptionImplemented) &&
		    /* Short Preamble Option Enable is TRUE */
		    ((prAdapter->rWifiVar.ePreambleType == PREAMBLE_TYPE_SHORT) ||
		     ((prAdapter->rWifiVar.ePreambleType == PREAMBLE_TYPE_AUTO) &&
		      (prStaRec->u2CapInfo & CAP_INFO_SHORT_PREAMBLE)))) {

			/* Case I: Implemented == TRUE and Short Preamble Option Enable == TRUE.
			 * Case II: Implemented == TRUE and Short Preamble == AUTO (depends on
			 *          BSS_DESC_T's capability)
			 */
			u2CapInfo |= CAP_INFO_SHORT_PREAMBLE;
		}

#if CFG_SUPPORT_SPEC_MGMT
		/* 802.11h spectrum management is for 5G band, so
		 * now we only enable spectrum management bit for 5G case.
		 * In TGn 5.2.22, spectrum management bit should set to 1
		 * to pass the UCC's check.
		 */
		if (prBssInfo && prBssInfo->eBand == BAND_5G)
			u2CapInfo |= CAP_INFO_SPEC_MGT;
#endif

		if (rNonHTPhyAttributes[u4NonHTPhyType].fgIsShortSlotTimeOptionImplemented &&
		    prAdapter->rWifiVar.fgIsShortSlotTimeOptionEnable) {
			u2CapInfo |= CAP_INFO_SHORT_SLOT_TIME;
		}
	}

	DBGLOG(SAA, LOUD, "ASSOC REQ: Compose Capability = 0x%04x for Target BSS [" MACSTR "].\n",
	       u2CapInfo, MAC2STR(prStaRec->aucMacAddr));

	return u2CapInfo;

}				/* end of assocBuildCapabilityInfo() */

/*----------------------------------------------------------------------------*/
/*!
* @brief This function is used to compose Common Information Elements for Association
*        Request Frame.
*
* @param[in] prMsduInfo     Pointer to the composed MSDU_INFO_T.
*
* @return (none)
*/
/*----------------------------------------------------------------------------*/
static __KAL_INLINE__ VOID assocBuildReAssocReqFrameCommonIEs(IN P_ADAPTER_T prAdapter, IN P_MSDU_INFO_T prMsduInfo)
{
	P_CONNECTION_SETTINGS_T prConnSettings;
	P_STA_RECORD_T prStaRec;
	PUINT_8 pucBuffer;
	UINT_16 u2SupportedRateSet;
	UINT_8 aucAllSupportedRates[RATE_NUM_SW] = { 0 };
	UINT_8 ucAllSupportedRatesLen;
	UINT_8 ucSupRatesLen;
	UINT_8 ucExtSupRatesLen;

	prConnSettings = &(prAdapter->rWifiVar.rConnSettings);
	ASSERT(prMsduInfo);
	ASSERT(prMsduInfo->eSrc == TX_PACKET_MGMT);

	prStaRec = cnmGetStaRecByIndex(prAdapter, prMsduInfo->ucStaRecIndex);
	ASSERT(prStaRec);

	if (!prStaRec)
		return;

	pucBuffer = (PUINT_8) ((ULONG) prMsduInfo->prPacket + (ULONG) prMsduInfo->u2FrameLength);
	ASSERT(pucBuffer);

	if (IS_STA_IN_AIS(prStaRec)) {

		/* Fill the SSID element. */
		SSID_IE(pucBuffer)->ucId = ELEM_ID_SSID;

		/* NOTE(Kevin): We copy the SSID from CONNECTION_SETTINGS for the case of
		 * Passive Scan and the target BSS didn't broadcast SSID on its Beacon Frame.
		 */

		COPY_SSID(SSID_IE(pucBuffer)->aucSSID,
			  SSID_IE(pucBuffer)->ucLength, prConnSettings->aucSSID, prConnSettings->ucSSIDLen);

		prMsduInfo->u2FrameLength += IE_SIZE(pucBuffer);
		pucBuffer += IE_SIZE(pucBuffer);

	}
#if CFG_ENABLE_WIFI_DIRECT
	else if ((prAdapter->fgIsP2PRegistered) && (IS_STA_IN_P2P(prStaRec)))
		pucBuffer = p2pBuildReAssocReqFrameCommonIEs(prAdapter, prMsduInfo, pucBuffer);

#endif
#if CFG_ENABLE_BT_OVER_WIFI
	else if (IS_STA_BOW_TYPE(prStaRec)) {

		SSID_IE(pucBuffer)->ucId = ELEM_ID_SSID;

		/* NOTE(Kevin): We copy the SSID from CONNECTION_SETTINGS for the case of
		 * Passive Scan and the target BSS didn't broadcast SSID on its Beacon Frame.
		 */

		COPY_SSID(SSID_IE(pucBuffer)->aucSSID,
			  SSID_IE(pucBuffer)->ucLength, prConnSettings->aucSSID, prConnSettings->ucSSIDLen);

		prMsduInfo->u2FrameLength += IE_SIZE(pucBuffer);
		pucBuffer += IE_SIZE(pucBuffer);
	}
#endif

	/* NOTE(Kevin 2008/12/19): 16.3.6.3 MLME-ASSOCIATE.indication -
	 * SupportedRates - The set of data rates that are supported by the STA
	 * that is requesting association.
	 * Original(Portable Driver): Only send the Rates that we'll support.
	 * New: Send the Phy Rates if the result of following & operation == NULL.
	 */
	/* rateGetDataRatesFromRateSet((prBssDesc->u2OperationalRateSet & */
	/* rPhyAttributes[prBssDesc->ePhyType].u2SupportedRateSet), */

	if (prStaRec->fgHasBasicPhyType) {
		UINT_32 u4NonHTPhyType;

		u4NonHTPhyType = prStaRec->ucNonHTBasicPhyType;

		u2SupportedRateSet = (prStaRec->u2OperationalRateSet &
				      rNonHTPhyAttributes[u4NonHTPhyType].u2SupportedRateSet);

		ASSERT(u2SupportedRateSet);

		if (!u2SupportedRateSet)
			u2SupportedRateSet = rNonHTPhyAttributes[u4NonHTPhyType].u2SupportedRateSet;

		/* TODO(Kevin): For P2P, we shouldn't send support rate set which contains 11b rate */

		rateGetDataRatesFromRateSet(u2SupportedRateSet, 0, aucAllSupportedRates, &ucAllSupportedRatesLen);

		ucSupRatesLen = ((ucAllSupportedRatesLen > ELEM_MAX_LEN_SUP_RATES) ?
				 ELEM_MAX_LEN_SUP_RATES : ucAllSupportedRatesLen);

		ucExtSupRatesLen = ucAllSupportedRatesLen - ucSupRatesLen;

		/* Fill the Supported Rates element. */
		if (ucSupRatesLen) {
			SUP_RATES_IE(pucBuffer)->ucId = ELEM_ID_SUP_RATES;
			SUP_RATES_IE(pucBuffer)->ucLength = ucSupRatesLen;
			kalMemCopy(SUP_RATES_IE(pucBuffer)->aucSupportedRates, aucAllSupportedRates, ucSupRatesLen);

			prMsduInfo->u2FrameLength += IE_SIZE(pucBuffer);
			pucBuffer += IE_SIZE(pucBuffer);
		}

		/* Fill the Extended Supported Rates element. */
		if (ucExtSupRatesLen) {

			EXT_SUP_RATES_IE(pucBuffer)->ucId = ELEM_ID_EXTENDED_SUP_RATES;
			EXT_SUP_RATES_IE(pucBuffer)->ucLength = ucExtSupRatesLen;

			kalMemCopy(EXT_SUP_RATES_IE(pucBuffer)->aucExtSupportedRates,
				   &aucAllSupportedRates[ucSupRatesLen], ucExtSupRatesLen);

			prMsduInfo->u2FrameLength += IE_SIZE(pucBuffer);
			pucBuffer += IE_SIZE(pucBuffer);
		}
	}
}				/* end of assocBuildReAssocReqFrameCommonIEs() */

/*----------------------------------------------------------------------------*/
/*!
* @brief This function will compose the (Re)Association Request frame header and
*        its fixed fields
*
* @param[in] prStaRec               Pointer to the STA_RECORD_T
* @param[in] pucBuffer              Pointer to the frame buffer.
* @param[in] aucMACAddress          Given Our MAC Address.
* @param[in out] pu2PayloadLen      Return the length of the composed fixed fields
*
* @return (none)
*/
/*----------------------------------------------------------------------------*/
VOID
assocComposeReAssocReqFrameHeaderAndFF(IN P_ADAPTER_T prAdapter,
				       IN P_STA_RECORD_T prStaRec,
				       IN PUINT_8 pucBuffer, IN UINT_8 aucMACAddress[], IN OUT PUINT_16 pu2PayloadLen)
{
	P_WLAN_ASSOC_REQ_FRAME_T prAssocFrame;
	BOOLEAN fgIsReAssoc;

	UINT_16 u2FrameCtrl;
	UINT_16 u2CapInfo;
	UINT_16 u2ListenInterval;

	ASSERT(prStaRec);
	ASSERT(pucBuffer);
	ASSERT(aucMACAddress);
	ASSERT(pu2PayloadLen);

	prAssocFrame = (P_WLAN_ASSOC_REQ_FRAME_T) pucBuffer;
	fgIsReAssoc = prStaRec->fgIsReAssoc;

	/* 4 <1> Compose the frame header of the (Re)Association Request  frame. */
	/* Fill the Frame Control field. */
	if (fgIsReAssoc)
		u2FrameCtrl = MAC_FRAME_REASSOC_REQ;
	else
		u2FrameCtrl = MAC_FRAME_ASSOC_REQ;

	WLAN_SET_FIELD_16(&prAssocFrame->u2FrameCtrl, u2FrameCtrl);

	/* Fill the DA field with Target BSSID. */
	COPY_MAC_ADDR(prAssocFrame->aucDestAddr, prStaRec->aucMacAddr);

	/* Fill the SA field with our MAC Address. */
	COPY_MAC_ADDR(prAssocFrame->aucSrcAddr, aucMACAddress);

	/* Fill the BSSID field with Target BSSID. */
	COPY_MAC_ADDR(prAssocFrame->aucBSSID, prStaRec->aucMacAddr);

	/* Clear the SEQ/FRAG_NO field(HW won't overide the FRAG_NO, so we need to clear it). */
	prAssocFrame->u2SeqCtrl = 0;

	/* 4 <2> Compose the frame body's common fixed field part of the (Re)Association Request  frame. */
	u2CapInfo = assocBuildCapabilityInfo(prAdapter, prStaRec);

	/* Fill the Capability Information field. */
	WLAN_SET_FIELD_16(&prAssocFrame->u2CapInfo, u2CapInfo);

	/* Calculate the listen interval for the maximum power mode. Currently, we
	*  set it to the value 2 times DTIM period by default.
	*/
	if (prStaRec->ucDTIMPeriod) {
		u2ListenInterval = prStaRec->ucDTIMPeriod * prAdapter->rWifiVar.ucListenDtimInterval;
	} else {
		DBGLOG(SAA, TRACE, "Use default listen interval\n");
		u2ListenInterval = DEFAULT_LISTEN_INTERVAL;
	}
	prStaRec->u2ListenInterval = u2ListenInterval;

	/* Fill the Listen Interval field. */
	WLAN_SET_FIELD_16(&prAssocFrame->u2ListenInterval, u2ListenInterval);

	/* 4 <3> Compose the Current AP Address field for ReAssociation Request  frame. */
	/* Fill the Current AP Address field. */
	if (prStaRec->fgIsReAssoc) {
		if (IS_STA_IN_AIS(prStaRec)) {

			P_BSS_INFO_T prAisBssInfo = prAdapter->prAisBssInfo;
			P_WLAN_REASSOC_REQ_FRAME_T prReAssocFrame = (P_WLAN_REASSOC_REQ_FRAME_T) prAssocFrame;

			COPY_MAC_ADDR(prReAssocFrame->aucCurrentAPAddr, prAisBssInfo->aucBSSID);
		} else {
			ASSERT(0);	/* We don't support ReAssociation for other network */
		}

		*pu2PayloadLen = (CAP_INFO_FIELD_LEN + LISTEN_INTERVAL_FIELD_LEN + CURR_AP_ADDR_FIELD_LEN);
	} else {
		*pu2PayloadLen = (CAP_INFO_FIELD_LEN + LISTEN_INTERVAL_FIELD_LEN);
	}
}				/* end of assocComposeReAssocReqFrame() */

/*----------------------------------------------------------------------------*/
/*!
* @brief This function will send the (Re)Association Request frame
*
* @param[in] prStaRec           Pointer to the STA_RECORD_T
*
* @retval WLAN_STATUS_RESOURCES No available resource for frame composing.
* @retval WLAN_STATUS_SUCCESS   Successfully send frame to TX Module
*/
/*----------------------------------------------------------------------------*/
WLAN_STATUS assocSendReAssocReqFrame(IN P_ADAPTER_T prAdapter, IN P_STA_RECORD_T prStaRec)
{
	P_MSDU_INFO_T prMsduInfo;
	P_BSS_INFO_T prBssInfo;

	UINT_16 u2PayloadLen;
	UINT_16 u2EstimatedFrameLen;
	UINT_16 u2EstimatedExtraIELen;
	BOOLEAN fgIsReAssoc;
	UINT_32 i;

	ASSERT(prStaRec);

	/* 4 <1> Allocate a PKT_INFO_T for Authentication Frame */
	fgIsReAssoc = prStaRec->fgIsReAssoc;

	/* Init with MGMT Header Length + Length of Fixed Fields + Common IE Length */
	if (fgIsReAssoc) {
		u2EstimatedFrameLen = MAC_TX_RESERVED_FIELD +
		    WLAN_MAC_MGMT_HEADER_LEN +
		    CAP_INFO_FIELD_LEN +
		    LISTEN_INTERVAL_FIELD_LEN +
		    CURR_AP_ADDR_FIELD_LEN +
		    (ELEM_HDR_LEN + ELEM_MAX_LEN_SSID) +
		    (ELEM_HDR_LEN + ELEM_MAX_LEN_SUP_RATES) + (ELEM_HDR_LEN + (RATE_NUM_SW - ELEM_MAX_LEN_SUP_RATES));
	} else {
		u2EstimatedFrameLen = MAC_TX_RESERVED_FIELD +
		    WLAN_MAC_MGMT_HEADER_LEN +
		    CAP_INFO_FIELD_LEN +
		    LISTEN_INTERVAL_FIELD_LEN +
		    (ELEM_HDR_LEN + ELEM_MAX_LEN_SSID) +
		    (ELEM_HDR_LEN + ELEM_MAX_LEN_SUP_RATES) + (ELEM_HDR_LEN + (RATE_NUM_SW - ELEM_MAX_LEN_SUP_RATES));
	}

	/* + Extra IE Length */
	u2EstimatedExtraIELen = 0;

#if CFG_ENABLE_WIFI_DIRECT_CFG_80211 && CFG_ENABLE_WIFI_DIRECT
	if (IS_STA_IN_P2P(prStaRec)) {
		if ((prAdapter->fgIsP2PRegistered)) {
			u2EstimatedExtraIELen = p2pCalculate_IEForAssocReq(prAdapter, prStaRec->ucBssIndex, prStaRec);
		} else {
			DBGLOG(P2P, TRACE, "Function Linker Lost.\n");
			ASSERT(FALSE);
		}
	} else {
		for (i = 0; i < sizeof(txAssocReqIETable) / sizeof(APPEND_VAR_IE_ENTRY_T); i++) {
			if (txAssocReqIETable[i].u2EstimatedFixedIELen != 0) {
				u2EstimatedExtraIELen += txAssocReqIETable[i].u2EstimatedFixedIELen;
			} else {
				u2EstimatedExtraIELen +=
				    (UINT_16) txAssocReqIETable[i].pfnCalculateVariableIELen(prAdapter,
											     prStaRec->ucBssIndex,
											     prStaRec);
			}
		}
	}
#else
	for (i = 0; i < sizeof(txAssocReqIETable) / sizeof(APPEND_VAR_IE_ENTRY_T); i++) {
		if (txAssocReqIETable[i].u2EstimatedFixedIELen != 0) {
			u2EstimatedExtraIELen += txAssocReqIETable[i].u2EstimatedFixedIELen;
		} else {
			u2EstimatedExtraIELen +=
			    (UINT_16) txAssocReqIETable[i].pfnCalculateVariableIELen(prAdapter,
										     prStaRec->ucBssIndex, prStaRec);
		}
	}
#endif

	ASSERT(prStaRec->ucBssIndex <= MAX_BSS_INDEX);

	u2EstimatedFrameLen += u2EstimatedExtraIELen;

	/* Allocate a MSDU_INFO_T */
	prMsduInfo = cnmMgtPktAlloc(prAdapter, u2EstimatedFrameLen);
	if (prMsduInfo == NULL) {
		DBGLOG(SAA, WARN, "No PKT_INFO_T for sending (Re)Assoc Request.\n");
		return WLAN_STATUS_RESOURCES;
	}
	/* 4 <2> Compose (Re)Association Request frame header and fixed fields in MSDU_INfO_T. */
	prBssInfo = GET_BSS_INFO_BY_INDEX(prAdapter, prStaRec->ucBssIndex);

	/* Compose Header and Fixed Field */
	assocComposeReAssocReqFrameHeaderAndFF(prAdapter,
					       prStaRec,
					       (PUINT_8) ((ULONG) (prMsduInfo->prPacket) +
							  MAC_TX_RESERVED_FIELD),
					       prBssInfo->aucOwnMacAddr, &u2PayloadLen);

	/* 4 <3> Update information of MSDU_INFO_T */
	TX_SET_MMPDU(prAdapter,
		     prMsduInfo,
		     prStaRec->ucBssIndex,
		     prStaRec->ucIndex,
		     WLAN_MAC_MGMT_HEADER_LEN,
		     WLAN_MAC_MGMT_HEADER_LEN + u2PayloadLen, saaFsmRunEventTxDone, MSDU_RATE_MODE_AUTO);

	/* 4 <4> Compose the frame body's IEs of the (Re)Association Request  frame. */
	assocBuildReAssocReqFrameCommonIEs(prAdapter, prMsduInfo);

	/* 4 <5> Compose IEs in MSDU_INFO_T */
#if CFG_ENABLE_WIFI_DIRECT_CFG_80211 && CFG_ENABLE_WIFI_DIRECT
	if (IS_STA_IN_P2P(prStaRec)) {
		if ((prAdapter->fgIsP2PRegistered)) {
			p2pGenerate_IEForAssocReq(prAdapter, prMsduInfo);
		} else {
			DBGLOG(P2P, TRACE, "Function Linker Lost.\n");
			ASSERT(FALSE);
		}
	} else {
		/* Append IE */
		for (i = 0; i < sizeof(txAssocReqIETable) / sizeof(APPEND_VAR_IE_ENTRY_T); i++) {
			if (txAssocReqIETable[i].pfnAppendIE)
				txAssocReqIETable[i].pfnAppendIE(prAdapter, prMsduInfo);

		}
	}
#else
	/* Append IE */
	for (i = 0; i < sizeof(txAssocReqIETable) / sizeof(APPEND_VAR_IE_ENTRY_T); i++) {
		if (txAssocReqIETable[i].pfnAppendIE)
			txAssocReqIETable[i].pfnAppendIE(prAdapter, prMsduInfo);

	}
#endif

	/* 4 <6> Update the (Re)association request information */
	if (IS_STA_IN_AIS(prStaRec)) {
		P_WLAN_ASSOC_REQ_FRAME_T prAssocFrame;

		prAssocFrame = (P_WLAN_ASSOC_REQ_FRAME_T) ((ULONG) (prMsduInfo->prPacket) + MAC_TX_RESERVED_FIELD);

		kalUpdateReAssocReqInfo(prAdapter->prGlueInfo,
					(PUINT_8) &prAssocFrame->u2CapInfo,
					prMsduInfo->u2FrameLength - offsetof(WLAN_ASSOC_REQ_FRAME_T,
									     u2CapInfo), fgIsReAssoc);
	}
#if CFG_ENABLE_WIFI_DIRECT
	if ((prAdapter->fgIsP2PRegistered) && (IS_STA_IN_P2P(prStaRec))) {
		P_WLAN_ASSOC_REQ_FRAME_T prAssocFrame;

		prAssocFrame = (P_WLAN_ASSOC_REQ_FRAME_T) ((ULONG) (prMsduInfo->prPacket) + MAC_TX_RESERVED_FIELD);

		kalP2PUpdateAssocInfo(prAdapter->prGlueInfo,
				      (PUINT_8) &prAssocFrame->u2CapInfo,
				      prMsduInfo->u2FrameLength - offsetof(WLAN_ASSOC_REQ_FRAME_T,
						u2CapInfo), fgIsReAssoc, prStaRec->ucBssIndex);
	}
#endif

	/* TODO(Kevin): Also release the unused tail room of the composed MMPDU */

	nicTxConfigPktControlFlag(prMsduInfo, MSDU_CONTROL_FLAG_FORCE_TX, TRUE);

	/* 4 <6> Enqueue the frame to send this (Re)Association request frame. */
	nicTxEnqueueMsdu(prAdapter, prMsduInfo);

	return WLAN_STATUS_SUCCESS;
}				/* end of assocSendReAssocReqFrame() */

/*----------------------------------------------------------------------------*/
/*!
* @brief This function will strictly check the TX (Re)Association Request frame for
*        SAA event handling.
*
* @param[in] prMsduInfo         Pointer of MSDU_INFO_T
*
* @retval WLAN_STATUS_FAILURE   This is not the frame we should handle at current state.
* @retval WLAN_STATUS_SUCCESS   This is the frame we should handle.
*/
/*----------------------------------------------------------------------------*/
WLAN_STATUS assocCheckTxReAssocReqFrame(IN P_ADAPTER_T prAdapter, IN P_MSDU_INFO_T prMsduInfo)
{
	P_WLAN_ASSOC_REQ_FRAME_T prAssocReqFrame;
	P_STA_RECORD_T prStaRec;
	UINT_16 u2TxFrameCtrl;

	ASSERT(prMsduInfo);
	ASSERT(prMsduInfo->eSrc == TX_PACKET_MGMT);

	prAssocReqFrame = (P_WLAN_ASSOC_REQ_FRAME_T) (prMsduInfo->prPacket);
	ASSERT(prAssocReqFrame);

	prStaRec = cnmGetStaRecByIndex(prAdapter, prMsduInfo->ucStaRecIndex);
	ASSERT(prStaRec);

	if (!prStaRec)
		return WLAN_STATUS_INVALID_PACKET;

	/* WLAN_GET_FIELD_16(&prAssocReqFrame->u2FrameCtrl, &u2TxFrameCtrl) */
	u2TxFrameCtrl = prAssocReqFrame->u2FrameCtrl;	/* NOTE(Kevin): Optimized for ARM */
	u2TxFrameCtrl &= MASK_FRAME_TYPE;
	if (prStaRec->fgIsReAssoc) {
		if (u2TxFrameCtrl != MAC_FRAME_REASSOC_REQ)
			return WLAN_STATUS_FAILURE;

	} else {
		if (u2TxFrameCtrl != MAC_FRAME_ASSOC_REQ)
			return WLAN_STATUS_FAILURE;

	}

	return WLAN_STATUS_SUCCESS;

}				/* end of assocCheckTxReAssocReqFrame() */

/*----------------------------------------------------------------------------*/
/*!
* @brief This function will strictly check the TX (Re)Association Response frame for
*        AAA event handling.
*
* @param[in] prMsduInfo         Pointer of MSDU_INFO_T
*
* @retval WLAN_STATUS_FAILURE   This is not the frame we should handle at current state.
* @retval WLAN_STATUS_SUCCESS   This is the frame we should handle.
*/
/*----------------------------------------------------------------------------*/
WLAN_STATUS assocCheckTxReAssocRespFrame(IN P_ADAPTER_T prAdapter, IN P_MSDU_INFO_T prMsduInfo)
{
	P_WLAN_ASSOC_RSP_FRAME_T prAssocRspFrame;
	P_STA_RECORD_T prStaRec;
	UINT_16 u2TxFrameCtrl;

	ASSERT(prMsduInfo);
	ASSERT(prMsduInfo->eSrc == TX_PACKET_MGMT);

	prAssocRspFrame = (P_WLAN_ASSOC_RSP_FRAME_T) (prMsduInfo->prPacket);
	ASSERT(prAssocRspFrame);

	prStaRec = cnmGetStaRecByIndex(prAdapter, prMsduInfo->ucStaRecIndex);
	ASSERT(prStaRec);

	if (!prStaRec)
		return WLAN_STATUS_INVALID_PACKET;

	/* WLAN_GET_FIELD_16(&prAssocFrame->u2FrameCtrl, &u2TxFrameCtrl) */
	u2TxFrameCtrl = prAssocRspFrame->u2FrameCtrl;	/* NOTE(Kevin): Optimized for ARM */
	u2TxFrameCtrl &= MASK_FRAME_TYPE;
	if (prStaRec->fgIsReAssoc) {
		if (u2TxFrameCtrl != MAC_FRAME_REASSOC_RSP)
			return WLAN_STATUS_FAILURE;

	} else {
		if (u2TxFrameCtrl != MAC_FRAME_ASSOC_RSP)
			return WLAN_STATUS_FAILURE;

	}

	return WLAN_STATUS_SUCCESS;

}				/* end of assocCheckTxReAssocRespFrame() */

/*----------------------------------------------------------------------------*/
/*!
* @brief This function will validate the incoming (Re)Association Frame and take out
*        the status code.
*
* @param[in] prSwRfb                Pointer to SW RFB data structure.
* @param[out] pu2StatusCode         Pointer to store the Status Code from Authentication.
*
* @retval WLAN_STATUS_FAILURE   This is not the frame we should handle at current state.
* @retval WLAN_STATUS_SUCCESS   This is the frame we should handle.
*/
/*----------------------------------------------------------------------------*/
WLAN_STATUS
assocCheckRxReAssocRspFrameStatus(IN P_ADAPTER_T prAdapter, IN P_SW_RFB_T prSwRfb, OUT PUINT_16 pu2StatusCode)
{
	P_STA_RECORD_T prStaRec;
	P_WLAN_ASSOC_RSP_FRAME_T prAssocRspFrame;
	UINT_16 u2RxFrameCtrl;
	UINT_16 u2RxCapInfo;
	UINT_16 u2RxStatusCode;
	UINT_16 u2RxAssocId;

	if (!prSwRfb || !pu2StatusCode) {
		DBGLOG(SAA, ERROR, "Invalid parameter, ignore!\n");
		return WLAN_STATUS_FAILURE;
	}

	if ((prSwRfb->u2PacketLen - prSwRfb->u2HeaderLen) < (CAP_INFO_FIELD_LEN +
							     STATUS_CODE_FIELD_LEN + AID_FIELD_LEN)) {
		DBGLOG(SAA, WARN,
		  "Invalid AssocRsp packet length. u2PacketLen(%u) u2HeaderLen(%u)\n",
		  prSwRfb->u2PacketLen, prSwRfb->u2HeaderLen);
		return WLAN_STATUS_FAILURE;
	}

	DBGLOG(SAA, LOUD, "prSwRfb->u2PayloadLength = %d\n", prSwRfb->u2PacketLen - prSwRfb->u2HeaderLen);

	prStaRec = cnmGetStaRecByIndex(prAdapter, prSwRfb->ucStaRecIdx);

	if (!prStaRec) {
		DBGLOG(SAA, ERROR, "Invalid prStaRec, ignore!\n");
		return WLAN_STATUS_INVALID_PACKET;
	}

	/* 4 <1> locate the (Re)Association Resp Frame. */
	prAssocRspFrame = (P_WLAN_ASSOC_RSP_FRAME_T) prSwRfb->pvHeader;

	/* if Association Response's BSSID doesn't match our target, ignore */
	if (!EQUAL_MAC_ADDR(prAssocRspFrame->aucBSSID, prStaRec->aucMacAddr)) {
		DBGLOG(SAA, INFO, "Unknown BSSID\n");
		return WLAN_STATUS_FAILURE;
	}

	/* 4 <2> Parse the Header of (Re)Association Resp Frame. */
	/* WLAN_GET_FIELD_16(&prAssocRspFrame->u2FrameCtrl, &u2RxFrameCtrl); */
	u2RxFrameCtrl = prAssocRspFrame->u2FrameCtrl;	/* NOTE(Kevin): Optimized for ARM */
	u2RxFrameCtrl &= MASK_FRAME_TYPE;
	if (prStaRec->fgIsReAssoc) {
		if (u2RxFrameCtrl != MAC_FRAME_REASSOC_RSP)
			return WLAN_STATUS_FAILURE;

	} else {
		if (u2RxFrameCtrl != MAC_FRAME_ASSOC_RSP)
			return WLAN_STATUS_FAILURE;

	}

	/* 4 <3> Parse the Fixed Fields of (Re)Association Resp Frame Body. */
	/* WLAN_GET_FIELD_16(&prAssocRspFrame->u2CapInfo, &u2RxCapInfo); */
	u2RxCapInfo = prAssocRspFrame->u2CapInfo;	/* NOTE(Kevin): Optimized for ARM */

	/* WLAN_GET_FIELD_16(&prAssocRspFrame->u2StatusCode, &u2RxStatusCode); */
	u2RxStatusCode = prAssocRspFrame->u2StatusCode;	/* NOTE(Kevin): Optimized for ARM */

	/* 4 <4> Check CAP_INFO */
	/* NOTE(Kevin): CM suggest to add MGMT workaround for those APs didn't check
	 * the CAP Privacy Bit to overcome a corner case that the Privacy Bit
	 * of our SCAN result didn't consist with AP's Association Resp.
	 */
	if (u2RxStatusCode == STATUS_CODE_SUCCESSFUL) {
#if CFG_SUPPORT_WAPI
		if (prAdapter->rWifiVar.rConnSettings.fgWapiMode) {
			/* WAPI AP allow the customer use WZC to join mode, the privacy bit is 0 */
			/* even at WAI & WAPI_PSK mode, but the assoc respose set the privacy bit set 1 */
			DBGLOG(SEC, TRACE, "Workaround the WAPI AP allow the customer to use WZC to join\n");
		} else
#endif
#if CFG_ENABLE_WIFI_DIRECT
		if (prAdapter->fgIsP2PRegistered && 1) {
			/* Todo:: Fixed this */
		} else
#endif
		{
		}

#if CFG_STRICT_CHECK_CAPINFO_PRIVACY
		if ((prStaRec->u2CapInfo & CAP_INFO_PRIVACY) ^ (u2RxCapInfo & CAP_INFO_PRIVACY))
			u2RxStatusCode = STATUS_CODE_CAP_NOT_SUPPORTED;

#endif
	}

	if (u2RxStatusCode == STATUS_CODE_SUCCESSFUL) {
		/* Update the information in the structure used to query and set
		*  OID_802_11_ASSOCIATION_INFORMATION.
		*/
		kalUpdateReAssocRspInfo(prAdapter->prGlueInfo,
					(PUINT_8) &prAssocRspFrame->u2CapInfo, (UINT_32) (prSwRfb->u2PacketLen));
	}
	/* 4 <5> Update CAP_INFO and ASSOC_ID */
	if (u2RxStatusCode == STATUS_CODE_SUCCESSFUL) {
		prStaRec->u2CapInfo = u2RxCapInfo;

		/* WLAN_GET_FIELD_16(&prAssocRspFrame->u2AssocId, &u2RxAssocId); */
		u2RxAssocId = prAssocRspFrame->u2AssocId;	/* NOTE(Kevin): Optimized for ARM */

		/*  20110715 Workaround for Kingnet 710 AP (Realtek 8186)
		 *   This AP raises the bit 6&7 not bit 14&15 in AID field.
		 *   It cause wrong AID assignment.
		 *   For AID = 2
		 *     Normal case: 0xC002(1100 0000 0000 0010) => 2
		 *     Kingnet 710:  0x00C2(0000 0000 1100 0010) => 194
		 *     workaround: mask bit 6&7 for this AP
		 */
		if ((u2RxAssocId & BIT(6)) && (u2RxAssocId & BIT(7)) && !(u2RxAssocId & BITS(8, 15))) {
			prStaRec->u2AssocId = u2RxAssocId & ~BITS(6, 7);
		} else {
			prStaRec->u2AssocId = u2RxAssocId & ~AID_MSB;
#if CFG_SUPPORT_802_11W
			if (prStaRec->eStaType == STA_TYPE_LEGACY_AP) {
				P_AIS_SPECIFIC_BSS_INFO_T prBssSpecInfo;

				prBssSpecInfo = &prAdapter->rWifiVar.rAisSpecificBssInfo;
				ASSERT(prBssSpecInfo);

				prBssSpecInfo->ucSaQueryTimedOut = 0;
			}
#endif
		}
	}
#if CFG_SUPPORT_802_11W
	if (u2RxStatusCode == STATUS_CODE_AUTH_ALGORITHM_NOT_SUPPORTED) {
		DBGLOG(SAA, INFO, "AP rejected due the authentication algorithm not support\n");
	} else if (u2RxStatusCode == STATUS_CODE_ASSOC_REJECTED_TEMPORARILY) {
		PUINT_8 pucIE, pucTime;
		UINT_16 u2IELength;
		UINT_16 u2Offset = 0;

		u2IELength = prSwRfb->u2PacketLen - prSwRfb->u2HeaderLen;
		pucIE = (PUINT_8) ((ULONG) prSwRfb->pvHeader + prSwRfb->u2HeaderLen);

		IE_FOR_EACH(pucIE, u2IELength, u2Offset) {
			if (IE_ID(pucIE) == ELEM_ID_TIMEOUT_INTERVAL && IE_LEN(pucIE) == 5) {
				pucTime = ((P_IE_HDR_T) pucIE)->aucInfo;
				if (pucTime[0] == ACTION_SA_TIMEOUT_ASSOC_COMEBACK) {
					UINT_32 tu;

					WLAN_GET_FIELD_32(pucTime + 1, &tu);
					DBGLOG(SAA, INFO,
					       "AP rejected association temporarily;comeback duration %u TU (%u ms)\n",
					       tu, TU_TO_MSEC(tu));
					if (tu > TX_ASSOCIATION_RETRY_TIMEOUT_TU) {
						DBGLOG(SAA, INFO, "Update timer based on comeback duration\n");
						/* ieee80211_reschedule_timer(wpa_s, ms); */
					}
				}
				break;
			}
		}		/* end of IE_FOR_EACH */
	}
#endif
	*pu2StatusCode = u2RxStatusCode;

	return WLAN_STATUS_SUCCESS;

}				/* end of assocCheckRxReAssocRspFrameStatus() */

/*----------------------------------------------------------------------------*/
/*!
* \brief This function will compose the Disassociation frame
*
* @param[in] prStaRec               Pointer to the STA_RECORD_T
* @param[in] pucBuffer              Pointer to the frame buffer.
* @param[in] aucMACAddress     Given Our MAC Address.
* @param[in] u2ReasonCode      The reason code of disassociation
*
* \return (none)
*/
/*----------------------------------------------------------------------------*/
static __KAL_INLINE__ VOID
assocComposeDisassocFrame(IN P_STA_RECORD_T prStaRec,
			  IN PUINT_8 pucBuffer, IN UINT_8 aucMACAddress[], IN UINT_16 u2ReasonCode)
{
	P_WLAN_DISASSOC_FRAME_T prDisAssocFrame;
	UINT_16 u2FrameCtrl;

	ASSERT(pucBuffer);
	ASSERT(pucBuffer);
	ASSERT(aucMACAddress);

	prDisAssocFrame = (P_WLAN_DISASSOC_FRAME_T) pucBuffer;

	/* 4 <1> Compose the frame header of the DisAssociation  frame. */
	/* Fill the Frame Control field. */
	u2FrameCtrl = MAC_FRAME_DISASSOC;

	WLAN_SET_FIELD_16(&prDisAssocFrame->u2FrameCtrl, u2FrameCtrl);

	/* Fill the DA field with Target BSSID. */
	COPY_MAC_ADDR(prDisAssocFrame->aucDestAddr, prStaRec->aucMacAddr);

	/* Fill the SA field with our MAC Address. */
	COPY_MAC_ADDR(prDisAssocFrame->aucSrcAddr, aucMACAddress);

	/* Fill the BSSID field with Target BSSID. */
	COPY_MAC_ADDR(prDisAssocFrame->aucBSSID, prStaRec->aucMacAddr);

	/* Clear the SEQ/FRAG_NO field(HW won't overide the FRAG_NO, so we need to clear it). */
	prDisAssocFrame->u2SeqCtrl = 0;

	/* 4 <2> Compose the frame body's fixed field part of the Disassociation frame. */
	/* Fill the Reason Code field. */
	WLAN_SET_FIELD_16(&prDisAssocFrame->u2ReasonCode, u2ReasonCode);
}				/* end of assocComposeDisassocFrame() */

/*----------------------------------------------------------------------------*/
/*!
* @brief This function will send the Disassociation frame
*
* @param[in] prStaRec           Pointer to the STA_RECORD_T
* @param[in] u2ReasonCode  The reason code of disassociation
*
* @retval WLAN_STATUS_RESOURCES No available resource for frame composing.
* @retval WLAN_STATUS_SUCCESS   Successfully send frame to TX Module
*/
/*----------------------------------------------------------------------------*/
WLAN_STATUS assocSendDisAssocFrame(IN P_ADAPTER_T prAdapter, IN P_STA_RECORD_T prStaRec, IN UINT_16 u2ReasonCode)
{
	PUINT_8 pucMacAddress;
	P_MSDU_INFO_T prMsduInfo;
	UINT_16 u2PayloadLen;
	UINT_16 u2EstimatedFrameLen;
	/* UINT_32 u4Status = WLAN_STATUS_SUCCESS; */

	ASSERT(prStaRec);
	ASSERT(prStaRec->ucBssIndex <= MAX_BSS_INDEX);

	DBGLOG(RSN, INFO, "assocSendDisAssocFrame\n");

	/* 4 <1> Allocate a PKT_INFO_T for Disassociation Frame */
	/* Init with MGMT Header Length + Length of Fixed Fields + IE Length */
	u2EstimatedFrameLen = MAC_TX_RESERVED_FIELD + WLAN_MAC_MGMT_HEADER_LEN + REASON_CODE_FIELD_LEN;

	/* Allocate a MSDU_INFO_T */
	prMsduInfo = cnmMgtPktAlloc(prAdapter, u2EstimatedFrameLen);
	if (prMsduInfo == NULL) {
		DBGLOG(SAA, WARN, "No PKT_INFO_T for sending DisAssoc.\n");
		return WLAN_STATUS_RESOURCES;
	}
	/* 4 <2> Compose Disassociation  frame header and fixed fields in MSDU_INfO_T. */
	pucMacAddress = GET_BSS_INFO_BY_INDEX(prAdapter, prStaRec->ucBssIndex)->aucOwnMacAddr;

	/* Compose Header and Fixed Field */
	assocComposeDisassocFrame(prStaRec,
				  (PUINT_8) ((ULONG) (prMsduInfo->prPacket) +
					     MAC_TX_RESERVED_FIELD), pucMacAddress, u2ReasonCode);

#if CFG_SUPPORT_802_11W
	/* AP PMF */
	if (rsnCheckBipKeyInstalled(prAdapter, prStaRec)) {
		/* PMF certification 4.3.3.1, 4.3.3.2 send unprotected deauth reason 6/7 */
		if (prStaRec->rPmfCfg.fgRxDeauthResp != TRUE) {

			P_WLAN_DISASSOC_FRAME_T prDisassocFrame;

			prDisassocFrame =
				(P_WLAN_DISASSOC_FRAME_T) ((ULONG) (prMsduInfo->prPacket) + MAC_TX_RESERVED_FIELD);

			prDisassocFrame->u2FrameCtrl |= MASK_FC_PROTECTED_FRAME;
		}
	}
#endif

	u2PayloadLen = REASON_CODE_FIELD_LEN;

	/* 4 <3> Update information of MSDU_INFO_T */
	TX_SET_MMPDU(prAdapter,
		     prMsduInfo,
		     prStaRec->ucBssIndex,
		     prStaRec->ucIndex,
		     WLAN_MAC_MGMT_HEADER_LEN, WLAN_MAC_MGMT_HEADER_LEN + u2PayloadLen, NULL, MSDU_RATE_MODE_AUTO);

#if CFG_SUPPORT_802_11W
	/* AP PMF */
	/* caution: access prStaRec only if true */
	if (rsnCheckBipKeyInstalled(prAdapter, prStaRec)) {
		/* 4.3.3.1 send unprotected deauth reason 6/7 */
		if (prStaRec->rPmfCfg.fgRxDeauthResp != TRUE) {
			DBGLOG(RSN, INFO, "Disassoc Set MSDU_OPT_PROTECTED_FRAME\n");
			nicTxConfigPktOption(prMsduInfo, MSDU_OPT_PROTECTED_FRAME, TRUE);
		}

		prStaRec->rPmfCfg.fgRxDeauthResp = FALSE;
	}
#endif

	/* 4 <4> Enqueue the frame to send this (Re)Association request frame. */
	nicTxEnqueueMsdu(prAdapter, prMsduInfo);

	return WLAN_STATUS_SUCCESS;
}				/* end of assocSendDisAssocFrame() */

/*----------------------------------------------------------------------------*/
/*!
* @brief This function will parse and process the incoming Disassociation frame
*        if the given BSSID is matched.
*
* @param[in] prSwRfb            Pointer to SW RFB data structure.
* @param[in] aucBSSID           Given BSSID
* @param[out] pu2ReasonCode     Pointer to store the Reason Code from Deauthentication.
*
* @retval WLAN_STATUS_FAILURE   This is not the frame we should handle at current state.
* @retval WLAN_STATUS_SUCCESS   This is the frame we should handle.
*/
/*----------------------------------------------------------------------------*/
WLAN_STATUS
assocProcessRxDisassocFrame(IN P_ADAPTER_T prAdapter,
			    IN P_SW_RFB_T prSwRfb, IN UINT_8 aucBSSID[], OUT PUINT_16 pu2ReasonCode)
{
	P_WLAN_DISASSOC_FRAME_T prDisassocFrame;
	UINT_16 u2RxReasonCode;

	if (!prSwRfb || !aucBSSID || !pu2ReasonCode) {
		DBGLOG(SAA, WARN, "Invalid parameters, ignore this pkt!\n");
		return WLAN_STATUS_FAILURE;
	}

	/* 4 <1> locate the Disassociation Frame. */
	prDisassocFrame = (P_WLAN_DISASSOC_FRAME_T) prSwRfb->pvHeader;

	/* 4 <2> Parse the Header of Disassociation Frame. */
	if ((prSwRfb->u2PacketLen - prSwRfb->u2HeaderLen) < REASON_CODE_FIELD_LEN) {
		DBGLOG(SAA, WARN,
		  "Invalid DisAssoc packet length. u2PacketLen(%u) u2HeaderLen(%u)\n",
		  prSwRfb->u2PacketLen, prSwRfb->u2HeaderLen);
		return WLAN_STATUS_FAILURE;
	}

	/* Check if this Disassoc Frame is coming from Target BSSID */
	if (UNEQUAL_MAC_ADDR(prDisassocFrame->aucBSSID, aucBSSID)) {
		DBGLOG(SAA, LOUD, "Ignore Disassoc Frame from other BSS [" MACSTR "]\n",
		       MAC2STR(prDisassocFrame->aucSrcAddr));
		return WLAN_STATUS_FAILURE;
	}

	/* 4 <3> Parse the Fixed Fields of Deauthentication Frame Body. */
	WLAN_GET_FIELD_16(&prDisassocFrame->u2ReasonCode, &u2RxReasonCode);
	*pu2ReasonCode = u2RxReasonCode;

	return WLAN_STATUS_SUCCESS;

}				/* end of assocProcessRxDisassocFrame() */

#if CFG_SUPPORT_AAA
/*----------------------------------------------------------------------------*/
/*!
* @brief This function will parse and process the incoming Association Req frame
*        and return a Status Code.
*
* @param[in] prAdapter          Pointer to the Adapter structure.
* @param[in] prSwRfb            Pointer to SW RFB data structure.
* @param[out] pu2StatusCode     Pointer to store the Status Code for carried in Association Response.
*
* @retval WLAN_STATUS_FAILURE   This is not the frame we should handle at current state.
* @retval WLAN_STATUS_SUCCESS   This is the frame we should handle.
*/
/*----------------------------------------------------------------------------*/
WLAN_STATUS assocProcessRxAssocReqFrame(IN P_ADAPTER_T prAdapter, IN P_SW_RFB_T prSwRfb, OUT PUINT_16 pu2StatusCode)
{
	P_WLAN_ASSOC_REQ_FRAME_T prAssocReqFrame;
	P_STA_RECORD_T prStaRec;
	P_BSS_INFO_T prBssInfo;
	P_IE_SSID_T prIeSsid = (P_IE_SSID_T) NULL;
	P_RSN_INFO_ELEM_T prIeRsn = (P_RSN_INFO_ELEM_T) NULL;
	P_IE_SUPPORTED_RATE_T prIeSupportedRate = (P_IE_SUPPORTED_RATE_T) NULL;
	P_IE_EXT_SUPPORTED_RATE_T prIeExtSupportedRate = (P_IE_EXT_SUPPORTED_RATE_T) NULL;
	P_WIFI_VAR_T prWifiVar = NULL;
	PUINT_8 pucIE, pucIEStart;
	UINT_16 u2IELength;
	UINT_16 u2Offset = 0;
	UINT_16 u2StatusCode = STATUS_CODE_SUCCESSFUL;
	UINT_16 u2RxFrameCtrl;
	UINT_16 u2BSSBasicRateSet;
	UINT_8 ucFixedFieldLength;
	BOOLEAN fgIsUnknownBssBasicRate;
	BOOLEAN fgIsTKIP = FALSE;
	UINT_32 i;

	if (!prAdapter || !prSwRfb || !pu2StatusCode) {
		DBGLOG(SAA, WARN, "Invalid parameters, ignore this pkt!\n");
		return WLAN_STATUS_FAILURE;
	}

	prWifiVar = &(prAdapter->rWifiVar);

	prStaRec = cnmGetStaRecByIndex(prAdapter, prSwRfb->ucStaRecIdx);

	if (prStaRec == NULL)
		return WLAN_STATUS_FAILURE;

	/* 4 <1> locate the Association Req Frame. */
	prAssocReqFrame = (P_WLAN_ASSOC_REQ_FRAME_T) prSwRfb->pvHeader;

	/* WLAN_GET_FIELD_16(&prAssocReqFrame->u2FrameCtrl, &u2RxFrameCtrl); */
	u2RxFrameCtrl = prAssocReqFrame->u2FrameCtrl;	/* NOTE(Kevin): Optimized for ARM */
	u2RxFrameCtrl &= MASK_FRAME_TYPE;

	/* 4 <2> Parse the Header of Association Req Frame. */
	if (u2RxFrameCtrl == MAC_FRAME_REASSOC_REQ)
		ucFixedFieldLength = CAP_INFO_FIELD_LEN + LISTEN_INTERVAL_FIELD_LEN + CURR_AP_ADDR_FIELD_LEN;
	else
		ucFixedFieldLength = CAP_INFO_FIELD_LEN + LISTEN_INTERVAL_FIELD_LEN;

	if ((prSwRfb->u2PacketLen - prSwRfb->u2HeaderLen) <= ucFixedFieldLength) {
		/* Length of this (re)association req is invalid, ignore it */
		return WLAN_STATUS_FAILURE;
	}

	prBssInfo = GET_BSS_INFO_BY_INDEX(prAdapter, prStaRec->ucBssIndex);

	/* Check if this Disassoc Frame is coming from Target BSSID */
	if (UNEQUAL_MAC_ADDR(prAssocReqFrame->aucBSSID, prBssInfo->aucBSSID))
		return WLAN_STATUS_FAILURE;	/* Just Ignore this MMPDU */

	if (u2RxFrameCtrl == MAC_FRAME_REASSOC_REQ) {
		prStaRec->fgIsReAssoc = TRUE;

		u2IELength = prSwRfb->u2PacketLen -
				    (UINT_16) OFFSET_OF(WLAN_REASSOC_REQ_FRAME_T, aucInfoElem[0]);

		pucIEStart = pucIE = ((P_WLAN_REASSOC_REQ_FRAME_T) (prSwRfb->pvHeader))->aucInfoElem;
	} else {
		prStaRec->fgIsReAssoc = FALSE;

		u2IELength = prSwRfb->u2PacketLen -
				    (UINT_16) OFFSET_OF(WLAN_ASSOC_REQ_FRAME_T, aucInfoElem[0]);

		pucIEStart = pucIE = prAssocReqFrame->aucInfoElem;
	}

	/* 4 <3> Parse the Fixed Fields of Assoc Req Frame Body. */
	prStaRec->u2CapInfo = prAssocReqFrame->u2CapInfo;

#if 0				/* CFG_ENABLE_WIFI_DIRECT && CFG_ENABLE_HOTSPOT_PRIVACY_CHECK */
	if (prAdapter->fgIsP2PRegistered && IS_STA_P2P_TYPE(prStaRec)) {
		if (((prStaRec->u2CapInfo & CAP_INFO_PRIVACY) && !kalP2PGetCipher(prAdapter->prGlueInfo))) {
			u2StatusCode = STATUS_CODE_CAP_NOT_SUPPORTED;
			DBGLOG(RSN, TRACE, "STA Assoc req privacy bit check fail\n");
			return WLAN_STATUS_SUCCESS;
		}
	}
#endif

	prStaRec->u2ListenInterval = prAssocReqFrame->u2ListenInterval;
	prStaRec->ucPhyTypeSet = 0;

	/* Might be legacy client or p2p gc. */
	prStaRec->eStaType = STA_TYPE_LEGACY_CLIENT;

	/* 4 <4> Parse the IE of Assoc Req Frame Body. */
	IE_FOR_EACH(pucIE, u2IELength, u2Offset) {
		switch (IE_ID(pucIE)) {
		case ELEM_ID_SSID:
			if ((!prIeSsid) &&	/* NOTE(Kevin): Get SSID once */
			    (IE_LEN(pucIE) <= ELEM_MAX_LEN_SSID)) {
				prIeSsid = (P_IE_SSID_T) pucIE;
			}
			break;

		case ELEM_ID_SUP_RATES:
			if ((!prIeSupportedRate) && (IE_LEN(pucIE) <= RATE_NUM_SW))
				prIeSupportedRate = SUP_RATES_IE(pucIE);

			break;

		case ELEM_ID_EXTENDED_SUP_RATES:
			if (!prIeExtSupportedRate)
				prIeExtSupportedRate = EXT_SUP_RATES_IE(pucIE);
			break;
		case ELEM_ID_HT_CAP:
			prStaRec->ucPhyTypeSet |= PHY_TYPE_BIT_HT;
			break;
		case ELEM_ID_VHT_CAP:
			prStaRec->ucPhyTypeSet |= PHY_TYPE_BIT_VHT;
			break;
		case ELEM_ID_RSN:
#if CFG_ENABLE_WIFI_DIRECT && CFG_ENABLE_HOTSPOT_PRIVACY_CHECK
			if (prAdapter->fgIsP2PRegistered && IS_STA_IN_P2P(prStaRec)) {
				prIeRsn = RSN_IE(pucIE);
				rsnParserCheckForRSNCCMPPSK(prAdapter, prIeRsn, prStaRec, &u2StatusCode);
				if (u2StatusCode != STATUS_CODE_SUCCESSFUL) {
					*pu2StatusCode = u2StatusCode;
					return WLAN_STATUS_SUCCESS;
				}
			}
#endif
			break;
		case ELEM_ID_VENDOR:
			if (p2pFuncParseCheckForTKIPInfoElem(pucIE))
				fgIsTKIP = TRUE;

#if CFG_ENABLE_WIFI_DIRECT
			{
				if ((prAdapter->fgIsP2PRegistered)) {
					UINT_8 ucOuiType = 0;

					p2pFuncParseCheckForP2PInfoElem(prAdapter, pucIE, &ucOuiType);

					if (ucOuiType == VENDOR_OUI_TYPE_P2P) {
						DBGLOG(P2P, TRACE, "Target Client is a P2P group client\n");
						prStaRec->eStaType = STA_TYPE_P2P_GC;
					}
				}
			}
#endif
			break;
		case ELEM_ID_IBSS_PARAM_SET:
			/* Check IBSS parameter set length to avoid abnormal content */
			if (IE_LEN(pucIE) != ELEM_MAX_LEN_IBSS_PARAMETER_SET) {
				*pu2StatusCode = STATUS_CODE_UNSPECIFIED_FAILURE;
				DBGLOG(SAA, WARN, "Invalid IBSS Parameter IE length!\n");
				return WLAN_STATUS_FAILURE;
			}
			break;
		default:
			for (i = 0; i < (sizeof(rxAssocReqIETable) / sizeof(VERIFY_IE_ENTRY_T)); i++) {

				if (((IE_ID(pucIE)) == rxAssocReqIETable[i].ucElemID) &&
				    (rxAssocReqIETable[i].pfnVarifyIE != NULL)) {
					rxAssocReqIETable[i].pfnVarifyIE(prAdapter, prSwRfb,
									 (P_IE_HDR_T) pucIE, &u2StatusCode);

					if (u2StatusCode != STATUS_CODE_SUCCESSFUL) {
						*pu2StatusCode = u2StatusCode;
						return WLAN_STATUS_SUCCESS;
					}
				}
			}

			break;
		}
	}			/* end of IE_FOR_EACH */

	/*
	 * According to TGn & TGac 4.2.44, AP should not bring HT/VHT Cap IE in
	 * the IE of Assoc resp, if the STA request to use TKIP cipher
	 */
	if (fgIsTKIP && !prWifiVar->ucApAllowHtVhtTkip)
		prStaRec->ucPhyTypeSet &= ~(PHY_TYPE_BIT_VHT | PHY_TYPE_BIT_HT);

	/* parsing for WMM related information (2010/12/21) */
	mqmProcessAssocReq(prAdapter, prSwRfb, pucIEStart, u2IELength);

	do {
		if (prIeSsid) {
			if (UNEQUAL_SSID(prBssInfo->aucSSID, prBssInfo->ucSSIDLen,
				prIeSsid->aucSSID, prIeSsid->ucLength)) {

				u2StatusCode = STATUS_CODE_UNSPECIFIED_FAILURE;
				break;
			}
		} else {
			u2StatusCode = STATUS_CODE_UNSPECIFIED_FAILURE;
			break;
		}

		prStaRec->u2OperationalRateSet = 0;
		prStaRec->u2BSSBasicRateSet = 0;

		if (!prIeSupportedRate) {
			DBGLOG(SAA, WARN, "Supported Rate IE not present!\n");
			u2StatusCode = STATUS_CODE_ASSOC_DENIED_RATE_NOT_SUPPORTED;
			break;
		}
		/* Ignore any Basic Bit */
		rateGetRateSetFromIEs(prIeSupportedRate, prIeExtSupportedRate,
				      &prStaRec->u2OperationalRateSet, &u2BSSBasicRateSet,
				      &fgIsUnknownBssBasicRate);

		if ((prBssInfo->u2BSSBasicRateSet & prStaRec->u2OperationalRateSet) !=
		    prBssInfo->u2BSSBasicRateSet) {
			u2StatusCode = STATUS_CODE_ASSOC_DENIED_RATE_NOT_SUPPORTED;
			DBGLOG(SAA, WARN, "Basic rate not supported!\n");
			break;
		}

		/* Accpet the Sta, update BSSBasicRateSet from Bss */
		prStaRec->u2BSSBasicRateSet = prBssInfo->u2BSSBasicRateSet;
		prStaRec->u2DesiredNonHTRateSet = (prStaRec->u2OperationalRateSet & RATE_SET_ALL_ABG);

		if (HAL_RX_STATUS_GET_RF_BAND(prSwRfb->prRxStatus) == BAND_2G4) {
			if (prStaRec->u2OperationalRateSet & RATE_SET_OFDM)
				prStaRec->ucPhyTypeSet |= PHY_TYPE_BIT_ERP;
			if (prStaRec->u2OperationalRateSet & RATE_SET_HR_DSSS)
				prStaRec->ucPhyTypeSet |= PHY_TYPE_BIT_HR_DSSS;
		} else {	/* (BAND_5G == prBssDesc->eBande) */
			if (prStaRec->u2OperationalRateSet & RATE_SET_OFDM)
				prStaRec->ucPhyTypeSet |= PHY_TYPE_BIT_OFDM;
		}
		/* Update default Tx rate */
		nicTxUpdateStaRecDefaultRate(prStaRec);

#if CFG_ENABLE_WIFI_DIRECT && CFG_ENABLE_HOTSPOT_PRIVACY_CHECK
		if (prAdapter->fgIsP2PRegistered && IS_STA_IN_P2P(prStaRec)) {
			if (prIeRsn) {
				if (!kalP2PGetCipher(prAdapter->prGlueInfo, (UINT_8) prBssInfo->u4PrivateData)) {
					u2StatusCode = STATUS_CODE_CIPHER_SUITE_REJECTED;
					break;
				}
			} else {
				/* prStaRec->rSecInfo.fgAllowOnly1x = FALSE; */
				/* if (kalP2PGetCipher(prAdapter->prGlueInfo)) { */
				/* Only Allow 1x */
				/* prStaRec->rSecInfo.fgAllowOnly1x = TRUE; */
				/* break; */
				/* } */
			}
		}
#endif

	} while (FALSE);

#if CFG_ENABLE_WIFI_DIRECT
	if (prAdapter->fgIsP2PRegistered && IS_STA_IN_P2P(prStaRec)) {
#if 1				/* ICS */
		{
			PUINT_8 cp = (PUINT_8) &prAssocReqFrame->u2CapInfo;

			if (prStaRec->fgIsReAssoc)
				cp += 10;
			else
				cp += 4;
			if (prStaRec->pucAssocReqIe) {
				kalMemFree(prStaRec->pucAssocReqIe, VIR_MEM_TYPE, prStaRec->u2AssocReqIeLen);
				prStaRec->pucAssocReqIe = NULL;
			}
			prStaRec->u2AssocReqIeLen = u2IELength;
			if (u2IELength) {
				prStaRec->pucAssocReqIe = kalMemAlloc(u2IELength, VIR_MEM_TYPE);

				if (prStaRec->pucAssocReqIe)
					kalMemCopy(prStaRec->pucAssocReqIe, cp, u2IELength);
				else
					DBGLOG(SAA, LOUD,
						"allocate memory for prStaRec->pucAssocReqIe failed!\n");
			}
		}
#endif
		kalP2PUpdateAssocInfo(prAdapter->prGlueInfo, (PUINT_8) &prAssocReqFrame->u2CapInfo,
				      u2IELength + (prStaRec->fgIsReAssoc ? 10 : 4), prStaRec->fgIsReAssoc,
				      prStaRec->ucBssIndex);
	}
#endif

	*pu2StatusCode = u2StatusCode;

	return WLAN_STATUS_SUCCESS;

}				/* end of assocProcessRxAssocReqFrame() */

/*----------------------------------------------------------------------------*/
/*!
* @brief This function is used to compose Common Information Elements for Association
*        Response Frame.
*
* @param[in] prMsduInfo     Pointer to the composed MSDU_INFO_T.
* @param[in] prBssInfo      Pointer to the BSS_INFO_T.
*
* @return (none)
*/
/*----------------------------------------------------------------------------*/
static __KAL_INLINE__ VOID
assocBuildReAssocRespFrameCommonIEs(IN P_ADAPTER_T prAdapter, IN P_MSDU_INFO_T prMsduInfo, IN P_BSS_INFO_T prBssInfo)
{
	PUINT_8 pucBuffer;
	P_STA_RECORD_T prStaRec;
	UINT_8 ucSupRatesLen;
	UINT_8 ucExtSupRatesLen;

	ASSERT(prMsduInfo);
	ASSERT(prMsduInfo->eSrc == TX_PACKET_MGMT);

	prStaRec = cnmGetStaRecByIndex(prAdapter, prMsduInfo->ucStaRecIndex);
	ASSERT(prStaRec);

	pucBuffer = (PUINT_8) ((ULONG) prMsduInfo->prPacket + (ULONG) prMsduInfo->u2FrameLength);
	ASSERT(pucBuffer);

	if (prBssInfo->ucAllSupportedRatesLen > ELEM_MAX_LEN_SUP_RATES) {

		ucSupRatesLen = ELEM_MAX_LEN_SUP_RATES;
		ucExtSupRatesLen = prBssInfo->ucAllSupportedRatesLen - ELEM_MAX_LEN_SUP_RATES;
	} else {
		ucSupRatesLen = prBssInfo->ucAllSupportedRatesLen;
		ucExtSupRatesLen = 0;
	}

	/* Fill the Supported Rates element. */
	if (ucSupRatesLen) {
		SUP_RATES_IE(pucBuffer)->ucId = ELEM_ID_SUP_RATES;
		SUP_RATES_IE(pucBuffer)->ucLength = ucSupRatesLen;
		kalMemCopy(SUP_RATES_IE(pucBuffer)->aucSupportedRates, prBssInfo->aucAllSupportedRates, ucSupRatesLen);

		prMsduInfo->u2FrameLength += IE_SIZE(pucBuffer);
		pucBuffer += IE_SIZE(pucBuffer);
	}

	/* Fill the Extended Supported Rates element. */
	if (ucExtSupRatesLen) {

		EXT_SUP_RATES_IE(pucBuffer)->ucId = ELEM_ID_EXTENDED_SUP_RATES;
		EXT_SUP_RATES_IE(pucBuffer)->ucLength = ucExtSupRatesLen;

		kalMemCopy(EXT_SUP_RATES_IE(pucBuffer)->aucExtSupportedRates,
			   &prBssInfo->aucAllSupportedRates[ucSupRatesLen], ucExtSupRatesLen);

		prMsduInfo->u2FrameLength += IE_SIZE(pucBuffer);
	}
}				/* end of assocBuildReAssocRespFrameCommonIEs() */

/*----------------------------------------------------------------------------*/
/*!
* @brief This function will compose the (Re)Association Response frame
*
* @param[in] prStaRec               Pointer to the STA_RECORD_T
* @param[in] pucBuffer              Pointer to the frame buffer.
* @param[in] aucBssid               Given BSSID.
* @param[in] u2CapInfo              Capability Field of current BSS.
* @param[in out] pu2PayloadLen      Return the length of the composed fixed fields
*
* @return (none)
*/
/*----------------------------------------------------------------------------*/
static __KAL_INLINE__ VOID
assocComposeReAssocRespFrameHeaderAndFF(IN P_STA_RECORD_T prStaRec,
					IN PUINT_8 pucBuffer,
					IN UINT_8 aucBSSID[], IN UINT_16 u2CapInfo, IN OUT PUINT_16 pu2PayloadLen)
{
	P_WLAN_ASSOC_RSP_FRAME_T prAssocRspFrame;
	BOOLEAN fgIsReAssoc;

	UINT_16 u2FrameCtrl;

	ASSERT(prStaRec);
	ASSERT(pucBuffer);
	ASSERT(aucBSSID);
	ASSERT(pu2PayloadLen);

	prAssocRspFrame = (P_WLAN_ASSOC_RSP_FRAME_T) pucBuffer;
	fgIsReAssoc = prStaRec->fgIsReAssoc;

	/* 4 <1> Compose the frame header of the (Re)Association Request  frame. */
	/* Fill the Frame Control field. */
	if (fgIsReAssoc)
		u2FrameCtrl = MAC_FRAME_REASSOC_RSP;
	else
		u2FrameCtrl = MAC_FRAME_ASSOC_RSP;

	/* WLAN_SET_FIELD_16(&prAssocFrame->u2FrameCtrl, u2FrameCtrl); */
	prAssocRspFrame->u2FrameCtrl = u2FrameCtrl;	/* NOTE(Kevin): Optimized for ARM */

	/* Fill the DA field with Target MAC Address. */
	COPY_MAC_ADDR(prAssocRspFrame->aucDestAddr, prStaRec->aucMacAddr);

	/* Fill the SA field with current BSSID. */
	COPY_MAC_ADDR(prAssocRspFrame->aucSrcAddr, aucBSSID);

	/* Fill the BSSID field with current BSSID. */
	COPY_MAC_ADDR(prAssocRspFrame->aucBSSID, aucBSSID);

	/* Clear the SEQ/FRAG_NO field(HW won't overide the FRAG_NO, so we need to clear it). */
	prAssocRspFrame->u2SeqCtrl = 0;

	/* 4 <2> Compose the frame body's common fixed field part of the (Re)Association Request  frame. */
	/* Fill the Capability Information field. */
	/* WLAN_SET_FIELD_16(&prAssocFrame->u2CapInfo, u2CapInfo); */
	prAssocRspFrame->u2CapInfo = u2CapInfo;	/* NOTE(Kevin): Optimized for ARM */

	/* WLAN_SET_FIELD_16(&prAssocFrame->u2StatusCode, prStaRec->u2StatusCode); */
	prAssocRspFrame->u2StatusCode = prStaRec->u2StatusCode;	/* NOTE(Kevin): Optimized for ARM */

	/* WLAN_SET_FIELD_16(&prAssocFrame->u2AssocId, ((prStaRec->u2AssocId & AID_MASK) | AID_MSB)); */
	/* NOTE(Kevin): Optimized for ARM */
	prAssocRspFrame->u2AssocId = ((prStaRec->u2AssocId & AID_MASK) | AID_MSB);

	*pu2PayloadLen = (CAP_INFO_FIELD_LEN + STATUS_CODE_FIELD_LEN + AID_FIELD_LEN);
}				/* end of assocComposeReAssocRespFrameHeaderAndFF() */

/*----------------------------------------------------------------------------*/
/*!
* @brief This function will send the (Re)Association Resp frame
*
* @param[in] prStaRec           Pointer to the STA_RECORD_T
*
* @retval WLAN_STATUS_RESOURCES No available resource for frame composing.
* @retval WLAN_STATUS_SUCCESS   Successfully send frame to TX Module
*/
/*----------------------------------------------------------------------------*/
WLAN_STATUS assocSendReAssocRespFrame(IN P_ADAPTER_T prAdapter, IN P_STA_RECORD_T prStaRec)
{
	P_BSS_INFO_T prBssInfo;
	P_MSDU_INFO_T prMsduInfo;

	UINT_16 u2PayloadLen;
	UINT_16 u2EstimatedFrameLen;
	UINT_16 u2EstimatedExtraIELen;
	BOOLEAN fgIsReAssoc;
	UINT_32 i;

	ASSERT(prStaRec);
	ASSERT(prStaRec->ucBssIndex <= MAX_BSS_INDEX);

	/* 4 <1> Allocate a PKT_INFO_T for Authentication Frame */
	fgIsReAssoc = prStaRec->fgIsReAssoc;

	/* Init with MGMT Header Length + Length of Fixed Fields + Common IE Length */
	u2EstimatedFrameLen = MAC_TX_RESERVED_FIELD +
	    WLAN_MAC_MGMT_HEADER_LEN +
	    CAP_INFO_FIELD_LEN +
	    STATUS_CODE_FIELD_LEN +
	    AID_FIELD_LEN +
	    (ELEM_HDR_LEN + ELEM_MAX_LEN_SUP_RATES) + (ELEM_HDR_LEN + (RATE_NUM_SW - ELEM_MAX_LEN_SUP_RATES));

	/* + Extra IE Length */
	u2EstimatedExtraIELen = 0;

	for (i = 0; i < sizeof(txAssocRespIETable) / sizeof(APPEND_VAR_IE_ENTRY_T); i++) {
		if (txAssocRespIETable[i].u2EstimatedFixedIELen != 0) {
			u2EstimatedExtraIELen += txAssocRespIETable[i].u2EstimatedFixedIELen;
		} else if (txAssocRespIETable[i].pfnCalculateVariableIELen != NULL) {
			u2EstimatedExtraIELen +=
			    (UINT_16) txAssocRespIETable[i].pfnCalculateVariableIELen(prAdapter,
										      prStaRec->ucBssIndex, prStaRec);
		}

	}

	u2EstimatedFrameLen += u2EstimatedExtraIELen;

	/* Allocate a MSDU_INFO_T */
	prMsduInfo = cnmMgtPktAlloc(prAdapter, u2EstimatedFrameLen);
	if (prMsduInfo == NULL) {
		DBGLOG(AAA, WARN, "No PKT_INFO_T for sending (Re)Assoc Response.\n");
		return WLAN_STATUS_RESOURCES;
	}
	/* 4 <2> Compose (Re)Association Request frame header and fixed fields in MSDU_INfO_T. */
	ASSERT(prStaRec->ucBssIndex != prAdapter->prAisBssInfo->ucBssIndex);
	prBssInfo = GET_BSS_INFO_BY_INDEX(prAdapter, prStaRec->ucBssIndex);

	/* Compose Header and Fixed Field */
	assocComposeReAssocRespFrameHeaderAndFF(prStaRec,
						(PUINT_8) ((ULONG) (prMsduInfo->prPacket) +
							   MAC_TX_RESERVED_FIELD),
						prBssInfo->aucBSSID, prBssInfo->u2CapInfo, &u2PayloadLen);

	/* 4 <3> Update information of MSDU_INFO_T */
	TX_SET_MMPDU(prAdapter,
		     prMsduInfo,
		     prStaRec->ucBssIndex,
		     prStaRec->ucIndex,
		     WLAN_MAC_MGMT_HEADER_LEN,
		     WLAN_MAC_MGMT_HEADER_LEN + u2PayloadLen, aaaFsmRunEventTxDone, MSDU_RATE_MODE_AUTO);

	/* 4 <4> Compose the frame body's IEs of the (Re)Association Request  frame. */
	assocBuildReAssocRespFrameCommonIEs(prAdapter, prMsduInfo, prBssInfo);

	/* 4 <5> Compose IEs in MSDU_INFO_T */

	/* Append IE */
	for (i = 0; i < sizeof(txAssocRespIETable) / sizeof(APPEND_VAR_IE_ENTRY_T); i++) {
		if (txAssocRespIETable[i].pfnAppendIE)
			txAssocRespIETable[i].pfnAppendIE(prAdapter, prMsduInfo);

	}

#if CFG_SUPPORT_WFD
	/* TODO put WFD IE in assoc resp if driver will send assoc resp */

#endif

	/* TODO(Kevin): Also release the unused tail room of the composed MMPDU */

	nicTxConfigPktControlFlag(prMsduInfo, MSDU_CONTROL_FLAG_FORCE_TX, TRUE);

	/* 4 <6> Enqueue the frame to send this (Re)Association request frame. */
	nicTxEnqueueMsdu(prAdapter, prMsduInfo);

	return WLAN_STATUS_SUCCESS;

}				/* end of assocSendReAssocRespFrame() */

#endif /* CFG_SUPPORT_AAA */
