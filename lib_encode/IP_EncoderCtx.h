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

#include "Sections.h"
#include "lib_encode/lib_encoder.h"
#include "lib_rtos/lib_rtos.h"
#include "lib_bitstream/lib_bitstream.h"
#include "IP_Stream.h"
#include "SourceBufferChecker.h"
#include "lib_common_enc/EncPicInfo.h"
#include "lib_common_enc/EncBuffersInternal.h"
#include "lib_common_enc/PictureInfo.h"
#include "lib_common_enc/EncSliceStatus.h"
#include "lib_common_enc/EncSliceBuffer.h"
#include "lib_common_enc/EncSize.h"
#include "lib_encode/lib_encoder.h"
#include "lib_common/Fifo.h"



typedef struct t_Scheduler TScheduler;

/****************************************************************************/
#define ENC_MAX_REF_DEFAULT 3
#define ENC_MAX_REF_CUSTOM 5
#define ENC_MAX_SRC 2
#define ENC_MAX_VIEW 1
#define MAX_NUM_B 8

/*************************************************************************//*!
   \brief Frame encoding info structure
*****************************************************************************/
typedef struct AL_t_FrameInfo
{
  AL_TEncInfo tEncInfo;
  AL_TBuffer* pQpTable;
}AL_TFrameInfo;


typedef AL_TEncSliceStatus TStreamInfo;

typedef struct AL_t_EncCtx AL_TEncCtx;

typedef struct
{
  void (* configureChannel)(struct AL_t_EncCtx* pCtx, AL_TEncSettings const* pSettings);
  void (* generateSkippedPictureData)(struct AL_t_EncCtx* pCtx);
  void (* generateNals)(AL_TEncCtx* pCtx);
  void (* updateHlsAndWriteSections)(AL_TEncCtx* pCtx, AL_TEncPicStatus* pPicStatus, AL_TBuffer* pStream);
}HighLevelEncoder;

/*************************************************************************//*!
   \brief Encoder Context structure
*****************************************************************************/
typedef struct AL_t_EncCtx
{
  HighLevelEncoder encoder;

  AL_TEncSettings m_Settings;

  AL_HANDLE m_hChannel;

  AL_TSps m_sps;
  AL_TPps m_pps;
  AL_THevcVps m_vps;

  TStreamInfo m_StreamInfo;

  int32_t m_uAltRefRecPOC;
  uint8_t m_uAltRefRecID;
  uint8_t m_uLastRecID;
  int m_iLastIdrId;

  AL_SeiData m_seiData;

  AL_TSrcBufferChecker m_srcBufferChecker;
  AL_TEncRequestInfo m_currentRequestInfo;
  int m_iMaxNumRef;

  AL_TSkippedPicture m_pSkippedPicture;

  int m_iFrameCountDone;
  AL_ERR m_eError;
  int m_iNumLCU;
  int m_iMinQP;
  int m_iMaxQP;

  TBufferEP m_tBufEP1;

  AL_TFrameInfo m_Pool[ENC_MAX_CMD];
  AL_TFifo m_iPoolIds;
  int m_iCurPool;

  AL_TBuffer* m_StreamSent[AL_MAX_STREAM_BUFFER];
  int m_iCurStreamSent;
  int m_iCurStreamRecv;

  AL_TBuffer* m_SourceSent[AL_MAX_SOURCE_BUFFER];


  AL_MUTEX m_Mutex;
  AL_SEMAPHORE m_PendingEncodings; // tracks the count of jobs sent to the scheduler

  TScheduler* m_pScheduler;

  AL_CB_EndEncoding m_callback;

  int iInitialNumB;
  uint16_t uInitialFrameRate;
}AL_TEncCtx;

NalsData AL_ExtractNalsData(AL_TEncCtx* pCtx);

/*@}*/

