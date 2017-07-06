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

#pragma once
#include "lib_common_enc/EncBuffers.h"
#include "EncEPBuffer.h"

#define ENC_MAX_CMD (AL_MAX_NUM_B_PICT + 2)

/*************************************************************************//*!
   \brief Retrieves the maximum size of one NAL unit
   \param[in] uWidth Frame Width in pixel
   \param[in] uHeight Frame Height in pixel
   \param[in] uMaxCuSize  Maximum Size of a Coding Unit
   \return maximum size of one NAL unit
*****************************************************************************/
uint32_t GetMaxLCU(uint16_t uWidth, uint16_t uHeight, uint8_t uMaxCuSize);

// Encoder Parameter Buf 1 Flag,  Size, Offset
static const TBufInfo EP1_BUF_LAMBDAS =
{
  1, 256, 0
}; // only 208 bytes used
static const TBufInfo EP1_BUF_PROBAS_I =
{
  2, 2048, 256
}; // only for VP9
static const TBufInfo EP1_BUF_PROBAS_P =
{
  4, 2048, 2304
}; // only for VP9
static const TBufInfo EP1_BUF_COUNTER =
{
  8, 8192, 4352
}; // only for VP9
static const TBufInfo EP1_BUF_SCL_LST =
{
  16, 25344, 256
};

static const TBufInfo EP3_BUF_RC_TABLE1 =
{
  1, 512, 0
};
static const TBufInfo EP3_BUF_RC_TABLE2 =
{
  2, 512, 512
};
static const TBufInfo EP3_BUF_RC_CTX =
{
  4, 4096, 1024
}; // no fixed size with max = 4096
static const TBufInfo EP3_BUF_RC_LVL =
{
  8, 32, 5120
};

/*************************************************************************//*!
   \brief  Retrieves the size of a QP parameters buffer 1 (Lda + SclMtx)
   \return maximum size (in bytes) needed to store
*****************************************************************************/
uint32_t GetAllocSizeEP1();

/*************************************************************************//*!
   \brief  Retrieves the size of a Encoder parameters buffer 3 (HW RateCtrl)
   \return maximum size (in bytes) needed to store
*****************************************************************************/
uint32_t GetAllocSizeEP3();

static const size_t MVBUFF_PL_OFFSET[2] =
{
  0, 64
}; // POC List
static const size_t MVBUFF_LONG_TERM_OFFSET = 128; // Long term flag List
static const size_t MVBUFF_USED_POC = 132; // Used POCs
static const size_t MVBUFF_MV_OFFSET = 256; // Motion Vectors

/****************************************************************************/

typedef TBufferYuv TBufferSrc;

/*************************************************************************//*!
   \brief Retrieves the size of a Reference YUV frame buffer
   \param[in] iWidth Frame width in pixel unit
   \param[in] iHeight Frame height in pixel unit
   \param[in] uBitDepth YUV bit-depth
   \param[in] eChromaMode Chroma Mode
   \param[in] bFbc Reference compression flag
   \return maximum size (in bytes) needed for the YUV frame buffer
*****************************************************************************/
uint32_t GetAllocSize_Ref(int iWidth, int iHeight, uint8_t uBitDepth, AL_EChromaMode eChromaMode, bool bFbc);

/*************************************************************************//*!
   \brief Retrieves the size of a YUV frame buffer
   \param[in] iWidth Frame width in pixel unit
   \param[in] iHeight Frame height in pixel unit
   \param[in] uBitDepth YUV bit-depth
   \param[in] eChromaMode Chroma Mode
   \param[in] iPitch Internal memory pitch
   \param[in] bFbc Reference compression flag
   \return maximum size (in bytes) needed for the YUV frame buffer
*****************************************************************************/
uint32_t GetAllocSize_RefPitch(int iWidth, int iHeight, uint8_t uBitDepth, AL_EChromaMode eChromaMode, int iPitch, bool bFbc);

/*************************************************************************//*!
   \brief Retrieves the size of a compressed buffer(LCU header + MVDs + Residuals)
   \param[in] iWidth  Frame width in pixels
   \param[in] iHeight Frame height in pixels
   \return maximum size (in bytes) needed for the compressed buffer
*****************************************************************************/
uint32_t GetAllocSize_CompData(int iWidth, int iHeight);

/*************************************************************************//*!
   \brief Retrieves the offset of the current LCU Hdr_MVDs words
   \param[in] iWidth  Frame width in pixels
   \param[in] iHeight Frame height in pixels
   \return maximum size (in bytes) needed for the LCU Info buffer
*****************************************************************************/
uint32_t GetAllocSize_CompMap(int iWidth, int iHeight);

/*************************************************************************//*!
   \brief Retrieves the size of a colocated frame buffer
   \param[in] iWidth Frame Width in pixel
   \param[in] iHeight  Frame Height in pixel
   \param[in] uLCUSize Max Size of a Coding Unit
   \param[in] Codec Flag which specifies the codec used
   \return the size (in bytes) needed for the colocated frame buffer
*****************************************************************************/
uint32_t GetAllocSize_MV(int iWidth, int iHeight, uint8_t uLCUSize, AL_ECodec Codec);

/*************************************************************************//*!
   \brief Retrieves the size of a entry_points size buffer
   \param[in] iLCUHeight Frame Height in pixel
   \param[in] iNumSlices Number of slices within the frame
   \param[in] uNumCore Number of used core
   \return the size (in bytes) needed for the entry_points size buffer
*****************************************************************************/
uint32_t GetAllocSize_WPP(int iLCUHeight, int iNumSlices, uint8_t uNumCore);

uint32_t GetAllocSize_SliceSize(uint32_t uWidth, uint32_t uHeight, uint32_t uNumSlices, uint32_t uMaxCuSize);

/*************************************************************************//*!
   \brief Retrieves the size of a stream part size buffer
   \param[in] iLCUHeight Frame Height in pixel
   \param[in] iNumSlices Number of slices within the frame
   \param[in] iSliceSize Number of byte per slice or zero if not used
   \return the size (in bytes) needed for the entry_points size buffer
*****************************************************************************/
uint32_t GetAllocSize_StreamPart(int iLCUHeight, int iNumSlices, int iSliceSize);
