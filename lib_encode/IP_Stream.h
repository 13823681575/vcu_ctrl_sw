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
   \addtogroup lib_encode
   @{
   \file
 *****************************************************************************/
#pragma once

#include "lib_rtos/types.h"

/****************************************************************************/
/* Basic Bytes Stream */
/****************************************************************************/

/*************************************************************************//*!
   \brief TODO
*****************************************************************************/
typedef struct t_Stream
{
  uint8_t* m_pData;
  uint8_t* m_pCur;
  uint32_t m_uSize;
}TStream;

/****************************************************************************/
void StreamInitBuffer(TStream* pStream, uint8_t* pBuf, uint32_t uSize);
void StreamReset(TStream* pStream);

void StreamWriteByte(TStream* pStream, uint8_t uByte);
void StreamCopyBytes(TStream* pStream, uint8_t* pBuf, int iNumber);
uint32_t StreamGetNumBytes(TStream* pStream);

/****************************************************************************/
void AntiEmul(TStream* pStream, uint8_t const* pData, int iNumBytes);

/****************************************************************************/
void FlushNAL(TStream* pStream, uint8_t uNUT, uint8_t uTempID, uint8_t uLayerID, uint8_t* pDataInNAL, int iBitsInNAL, bool bCheckEmul, uint8_t uNalIdc, bool bAvc);
/*****************************************************************************/

/*@}*/
