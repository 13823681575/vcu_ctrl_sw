/******************************************************************************
*
* Copyright (C) 2017 Allegro DVT2.  All rights reserved.
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* Use of the Software is limited solely to applications:
* (a) running on a Xilinx device, or
* (b) that interact with a Xilinx device through a bus or interconnect.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
* XILINX OR ALLEGRO DVT2 BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
* WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF
* OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*
* Except as contained in this notice, the name of  Xilinx shall not be used
* in advertising or otherwise to promote the sale, use or other dealings in
* this Software without prior written authorization from Xilinx.
*
*
* Except as contained in this notice, the name of Allegro DVT2 shall not be used
* in advertising or otherwise to promote the sale, use or other dealings in
* this Software without prior written authorization from Allegro DVT2.
*
******************************************************************************/

/****************************************************************************
   -----------------------------------------------------------------------------
 **************************************************************************//*!
   \addtogroup lib_base
   @{
   \file
 *****************************************************************************/
#include "L2CacheParam.h"
#include "lib_common/Utils.h"


/****************************************************************************/
void AL_L2C_GetL2CacheMaxRange(AL_TEncChanParam const* pChParam, AL_ESliceType eSliceType, uint8_t uNumCore, uint16_t* pHorzRange, uint16_t* pVertRange)
{
  int iCacheSize = pChParam->uL2CacheMemSize;
  int iCTBSize = 1 << pChParam->uMaxCuSize;
  int iLcuWidth = (pChParam->uWidth + (1 << pChParam->uMaxCuSize) - 1) >> pChParam->uMaxCuSize;
  int iPictWidth = iLcuWidth << pChParam->uMaxCuSize;
  int iVRange, iHRange;

  if(eSliceType == SLICE_B)
    iCacheSize >>= 1;

  iCacheSize = iCacheSize * 8 / HW_IP_BIT_DEPTH;

  if(AL_GET_CHROMA_MODE(pChParam->ePicFormat) == CHROMA_4_2_0)
    iCacheSize = (iCacheSize * 2) / 3;
  else if(AL_GET_CHROMA_MODE(pChParam->ePicFormat) == CHROMA_4_2_2)
    iCacheSize = iCacheSize / 2;

  iCacheSize -= L2C_REFILL_MARGIN * L2C_W * L2C_H * uNumCore;

  iCacheSize -= L2C_W * iCTBSize * uNumCore; // Recycling margin : LCU step are smaller than the L2C tile width, so the tiles can not be recycled at each step.

  if(pChParam->eOptions & AL_OPT_WPP)
  {
    int iCacheWidth = ALIGN_UP(iPictWidth, L2C_W);
    int iCoreGap = (uNumCore > 1) ? L2C_WPP_CORE_GAP : 0;

    int iTmp1 = 2 * iCacheWidth;
    int iTmp2 = 2 * uNumCore * iCTBSize;

    iCacheSize -= 2 * uNumCore * iCTBSize * iCTBSize * (iCoreGap + 1);
    iCacheSize += 2 * iCoreGap * uNumCore * iCTBSize;

    iVRange = 64;

    for(;;)
    {
      iHRange = ALIGN_DOWN((iCacheSize - iVRange * iTmp1) / iTmp2, iCTBSize);

      if(iHRange < iVRange || iHRange < L2C_W)
        iVRange -= AL_IS_AVC(pChParam->eProfile) ? ME_HMIN_AVC : ME_HMIN_HEVC;
      else
        break;
    }
  }
  else
  {
    int iTileWidth = ALIGN_UP(iPictWidth / uNumCore, L2C_W);

    iVRange = 64;

    for(;;)
    {
      int iTmp1, iTmp2;

      iTmp1 = uNumCore * (2 * iVRange * iTileWidth + 2 * iCTBSize * iCTBSize);
      iTmp2 = 2 * (2 * iVRange + iCTBSize) * (uNumCore - 1) + 2 * uNumCore * iCTBSize;

      iHRange = ALIGN_DOWN((iCacheSize - iTmp1) / iTmp2, L2C_H);

      if(iHRange < iVRange || iHRange < L2C_W)
        iVRange -= L2C_H;
      else
        break;
    }
  }

  if(pHorzRange)
    *pHorzRange = (uint16_t)Min(iHRange, 3968); // 4032 - 64

  if(pVertRange)
    *pVertRange = (uint16_t)iVRange;
}

/****************************************************************************/
static uint32_t AL_sL2Cache_GetL2CacheSize(AL_TEncChanParam const* pChParam, uint8_t uNumCore, int iHRange, int iVRange)
{
  uint32_t uCacheMinSize = 0;

  int iLcuSize;

  if(AL_IS_AVC(pChParam->eProfile))
    iLcuSize = 16;
  else
  iLcuSize = 32;

  iHRange = ALIGN_UP(iHRange, L2C_W);
  iVRange = ALIGN_UP(iVRange, L2C_H);

  if(pChParam->eOptions & AL_OPT_WPP)
  {
    int iCacheWidth = ALIGN_UP(pChParam->uWidth, L2C_W);
    int iCoreGap = (uNumCore > 1) ? L2C_WPP_CORE_GAP : 0;
    uCacheMinSize += uNumCore * iLcuSize * ALIGN_UP(iLcuSize * 2 * (iCoreGap + 1) + 2 * iHRange, L2C_W);
    uCacheMinSize += 2 * (iVRange * iCacheWidth - ALIGN_UP(iCoreGap * uNumCore * iLcuSize, L2C_W));
  }
  else
  {
    int iTileWidth = ALIGN_UP(pChParam->uWidth / uNumCore, L2C_W);

    uCacheMinSize += uNumCore * (2 * iVRange * iTileWidth + iLcuSize * 2 * (iHRange + iLcuSize));
    uCacheMinSize += (uNumCore - 1) * (2 * iVRange + iLcuSize) * 2 * iHRange;
  }
  uCacheMinSize += L2C_W * iLcuSize * uNumCore; // Recycling margin : LCU step are smaller than the L2C tile width, so the tiles can not be recycled at each step.

  uCacheMinSize += L2C_REFILL_MARGIN * L2C_W * L2C_H * uNumCore;

  if(AL_GET_CHROMA_MODE(pChParam->ePicFormat) == CHROMA_4_2_0)
    uCacheMinSize += (uCacheMinSize >> 1);
  else if(AL_GET_CHROMA_MODE(pChParam->ePicFormat) == CHROMA_4_2_2)
    uCacheMinSize += uCacheMinSize;

  uCacheMinSize = uCacheMinSize * HW_IP_BIT_DEPTH / 8;

  if(pChParam->tGopParam.uNumB || (pChParam->tGopParam.eMode == AL_GOP_MODE_LOW_DELAY_B && pChParam->tGopParam.uGopLength))
    uCacheMinSize <<= 1;

  return uCacheMinSize;
}

/****************************************************************************/
uint32_t AL_L2C_GetL2CacheMinSize(AL_TEncChanParam const* pChParam, uint8_t uNumCore)
{
  int iHRange = L2C_W;
  int iVRange = AL_IS_AVC(pChParam->eProfile) ? ME_HMIN_AVC : ME_HMIN_HEVC;

  return AL_sL2Cache_GetL2CacheSize(pChParam, uNumCore, iHRange, iVRange);
}

/****************************************************************************/
uint32_t AL_L2C_GetL2CacheMaxSize(AL_TEncChanParam const* pChParam, uint8_t uNumCore)
{
  uint32_t uResMax;
  uint32_t uMin = AL_L2C_GetL2CacheMinSize(pChParam, uNumCore);
  int iLcuSize = 1 << pChParam->uMaxCuSize;
  int iHRange = ALIGN_UP(pChParam->uWidth, L2C_W);
  int iVRange = ALIGN_UP(pChParam->uHeight, L2C_H);
  uint32_t uMax1, uMax2, uMax3;

  // Max Size from Max Range
  uMax1 = AL_sL2Cache_GetL2CacheSize(pChParam, uNumCore, iHRange, iVRange);

  // Max size from picture Width
  uMax2 = (iVRange + iLcuSize + iVRange) * ALIGN_UP(pChParam->uWidth, L2C_W);

  if(AL_GET_CHROMA_MODE(pChParam->ePicFormat) == CHROMA_4_2_0)
    uMax2 += (uMax2 >> 1);
  else if(AL_GET_CHROMA_MODE(pChParam->ePicFormat) == CHROMA_4_2_2)
    uMax2 += uMax2;
  uMax2 = uMax2 * HW_IP_BIT_DEPTH / 8;

  // Max Size shall not exceed the maximum number of L2C block addr in the L2C controller
  uMax3 = ((pChParam->eOptions & AL_OPT_WPP) && pChParam->uNumCore > 1) ? L2C_MAX_BLOCK * 2 : L2C_MAX_BLOCK;

  if(pChParam->tGopParam.uNumB > 0 || pChParam->tGopParam.eMode == AL_GOP_MODE_LOW_DELAY_B)
    uMax3 /= 2; // half/half split  for 2 references
  uMax3 -= (ALIGN_UP(pChParam->uWidth, L2C_W) / L2C_W) * ((1 << pChParam->uMaxCuSize) / L2C_H); // 1 LCU Row margin for loop-back
  uMax3 *= L2C_W * L2C_H; // size in number of block to size in bytes (luma)

  if(AL_GET_CHROMA_MODE(pChParam->ePicFormat) == CHROMA_4_2_0)
    uMax3 += (uMax3 >> 1);
  else if(AL_GET_CHROMA_MODE(pChParam->ePicFormat) == CHROMA_4_2_2)
    uMax3 += uMax3;

  if(pChParam->tGopParam.uNumB > 0 || pChParam->tGopParam.eMode == AL_GOP_MODE_LOW_DELAY_B)
    uMax3 *= 2; // Total size with the 2 ref.

  uResMax = uMax1;

  if(uResMax > uMax2)
    uResMax = uMax2;

  if(uResMax > uMax3)
    uResMax = uMax3;

  return uResMax < uMin ? uMin : uResMax;
}

/*@}*/
