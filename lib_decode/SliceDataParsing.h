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
   \addtogroup lib_decode_hls
   @{
   \file
 *****************************************************************************/
#pragma once

#include "lib_common/ScalingList.h"
#include "lib_common/SPS.h"
#include "lib_common/PPS.h"
#include "lib_common/SliceHeader.h"

#include "lib_parsing/I_PictMngr.h"

typedef struct t_Dec_Ctx TDecCtx;

/*************************************************************************//*!
   \brief The GetBlk2Buffers retrieves the buffer needed for the second block decoding
   \param[in]  pCtx       Pointer to a decoder context object
   \param[in]  pSP        Pointer to the current slice parameters
*****************************************************************************/
void GetBlk2Buffers(AL_TDecCtx* pCtx, AL_TDecSliceParam* pSP);

/*************************************************************************//*!
   \brief The AL_LaunchDecoding function launch a frame decoding request to the Hardware IP
   \param[in]  pCtx              Pointer to a decoder context object
*****************************************************************************/
void AL_LaunchFrameDecoding(AL_TDecCtx* pCtx);

/*************************************************************************//*!
   \brief The AL_LaunchSliceDecoding function launch a slice decoding request to the Hardware IP
   \param[in]  pCtx              Pointer to a decoder context object
   \param[in]  bIsLastAUNal      Specify if it's the last AU's slice data
*****************************************************************************/
void AL_LaunchSliceDecoding(AL_TDecCtx* pCtx, bool bIsLastAUNal);

/*************************************************************************//*!
   \brief The AL_InitFrameBuffers function intializes the frame buffers needed to process the current frame decoding
   \param[in]  pCtx              Pointer to a decoder context object
   \param[in]  iWidth            Picture width in pixels unit
   \param[in]  iHeight           Picture height in pixels unit
   \param[in]  pPP               Pointer to the current picture parameters
*****************************************************************************/
void AL_InitFrameBuffers(AL_TDecCtx* pCtx, int iWidth, int iHeight, AL_TDecPicParam* pPP);

/*************************************************************************//*!
   \brief The AL_InitIntermediateBuffers function intializes the intermediate buffers needed to process the current frame decoding
   \param[in]  pCtx              Pointer to a decoder context object
   \param[in]  pBufs             Pointer to the current picture buffers
*****************************************************************************/
void AL_InitIntermediateBuffers(AL_TDecCtx* pCtx, AL_TDecPicBuffers* pBufs);

/*************************************************************************//*!
   \brief The AL_TerminatePreviousCommand flush one decoding command by computing parameters relative to next slice
   \param[in]  pCtx              Pointer to a decoder context object
   \param[in]  pPP               Pointer to the current picture parameters
   \param[in]  pSP               Pointer to the current slice parameters
   \param[in]  bIsLastVclNalInAU Specifies if this is the last NAL of the current access unit
   \param[in]  bNextIsDependent  Specifies if the next slice segment is a dependent or non-dependent slice
*****************************************************************************/
void AL_TerminatePreviousCommand(AL_TDecCtx* pCtx, AL_TDecPicParam* pPP, AL_TDecSliceParam* pSP, bool bIsLastVclNalInAU, bool bNextIsDependent);

/*************************************************************************//*!
   \brief The AL_AVC_PrepareCommand function prepares the buffers for the hardware decoding process
   \param[in]  pCtx              Pointer to a decoder context object
   \param[in]  pSCL              Pointer to a scaling list object
   \param[in]  pPP               Pointer to the current picture parameters
   \param[in]  pBufs             Pointer to the current picture buffers
   \param[in]  pSP               Pointer to the current slice parameters
   \param[in]  pSlice            Pointer to the current slice header
   \param[in]  bIsLastVclNalInAU Specifies if this is the last NAL of the current access unit
   \param[in]  bIsValid          Specifies if the current NAL has been correctly decoded
*****************************************************************************/
void AL_AVC_PrepareCommand(AL_TDecCtx* pCtx, AL_TScl* pSCL, AL_TDecPicParam* pPP, AL_TDecPicBuffers* pBufs, AL_TDecSliceParam* pSP, AL_TAvcSliceHdr* pSlice, bool bIsLastVclNalInAU, bool bIsValid);

/*************************************************************************//*!
   \brief The AL_HEVC_PrepareCommand function prepares the buffers for the hardware decoding process
   \param[in]  pCtx              Pointer to a decoder context object
   \param[in]  pSCL              Pointer to a scaling list object
   \param[in]  pPP               Pointer to the current picture parameters
   \param[in]  pBufs             Pointer to the current picture buffers
   \param[in]  pSP               Pointer to the current slice parameters
   \param[in]  pSlice            Pointer to the current slice header
   \param[in]  bIsLastVclNalInAU Specifies if this is the last NAL of the current access unit
   \param[in]  bIsValid          Specifies if the current NAL has been correctly decoded
*****************************************************************************/
void AL_HEVC_PrepareCommand(AL_TDecCtx* pCtx, AL_TScl* pSCL, AL_TDecPicParam* pPP, AL_TDecPicBuffers* pBufs, AL_TDecSliceParam* pSP, AL_THevcSliceHdr* pSlice, bool bIsLastVclNalInAU, bool bIsValid);

/*@}*/
