#include "framework.h"


namespace audio
{


   wave_pre_buffer::wave_pre_buffer(sp(::ca2::application) papp) :
   ca2(papp),
      m_thread(papp)
   {
      m_thread.SetPreBuffer(this);
      m_iChunkCount        = 0;
      m_iChunkSampleCount  = 0;
      m_pstreameffectOut   = NULL;
      m_iOutCount          = 0;
      m_iMinFreeBuffer     = 0;
      m_pdecoder           = NULL;
      m_position           = 0;
   }

   wave_pre_buffer::~wave_pre_buffer()
   {

   }


   bool wave_pre_buffer::open(::audio::wave_pre_buffer_callback * pcallback, int32_t iChannelCount, int32_t iChunkCount, int32_t iChunkSampleCount)
   {
      if(!_UpdateBuffer(iChannelCount, iChunkCount, iChunkSampleCount, m_iMinFreeBuffer))
      {
         return false;
      }

      m_iChannelCount = iChannelCount;
      m_iChunkCount = iChunkCount;
      m_iChunkSampleCount = iChunkSampleCount;
      m_pcallback = pcallback;

      // no m_speffect by default, removed devocalizer during memory usage optimization 16:35 2011-04-05
      //m_spdevocalizer.destroy();
      //m_spdevocalizer(new ::audio::wave_effect_devocalizer(get_app(), false, iChunkSampleCount, 44100, 2, 2));
      return true;
   }

   bool wave_pre_buffer::SetMinL1BufferCount(int32_t iMinL1BufferCount)
   {
      if(!_UpdateBuffer(m_iChannelCount, m_iChunkCount, m_iChunkSampleCount, iMinL1BufferCount))
      {
         return false;
      }

      m_iMinFreeBuffer = iMinL1BufferCount;
      return true;
   }

   void wave_pre_buffer::SetDecoder(audio_decode::decoder * pinterface)
   {
      spa(audio_decode::decoder)::add(pinterface);
      m_pdecoder = pinterface;
   }


   int16_t * wave_pre_buffer::GetInBuffer(int32_t iChunk)
   {
      return m_chunkaL1[iChunk].m_psh;
   }

   int16_t * wave_pre_buffer::GetOutBufferId(int32_t iId)
   {
      return &m_shaL2[_IdToChunk(iId) * m_iChunkSampleCount * m_iChannelCount];
   }

   int32_t wave_pre_buffer::GetChunkByteCount()
   {
      return m_iChunkSampleCount * 2 * m_iChannelCount;
   }

   bool wave_pre_buffer::IdFree(int32_t iId)
   {
      return _FreeId(iId);
   }


   bool wave_pre_buffer::_FreeId(int32_t iId)
   {
      single_lock sl(&m_cs, TRUE);

      m_iOutCount--;

      if(m_iOutCount < 0)
         m_iOutCount = 0;

      Chunk * pchunk1 = m_chunkaL1.GetChunkById(iId);
      Chunk * pchunk2 = m_chunkaL2.GetChunkById(iId);

      if(m_pdecoder->DecoderEOF())
         return false;

      ::primitive::memory_size iBytesRecorded = m_pdecoder->DecoderFillBuffer(pchunk1->m_psh, GetChunkByteCount());

      int64_t iRemain = GetChunkByteCount() - iBytesRecorded;
      if(iRemain > 0)
      {
         LPBYTE lpb = (LPBYTE) pchunk1->m_psh;
         memset(&lpb[iBytesRecorded], 0, (size_t) iRemain);
      }

      memcpy(pchunk2->m_psh, pchunk1->m_psh, GetChunkByteCount());
      /*if(m_speffect != NULL && !m_speffect->Process16bits(pchunk1->m_psh, pchunk2->m_psh))
      {
      return false;
      }*/

      if(!_PreOutputId(pchunk2))
      {
         return false;
      }

      m_iOutCount++;

      m_pcallback->OnPreBufferDone(iId);
      return true;
   }

   bool wave_pre_buffer::_DeferKick()
   {
      return false;
      /*
      single_lock sl(&m_cs, TRUE);
      if(m_iaFree.get_size() == 0)
      return false;
      int32_t iId = m_iaFree[0];
      if(IsEOF())
      return false;
      if(m_iLastId >= 0)
      return false;
      Chunk * pchunk1 = m_chunkaL1.GetChunkById(iId);
      Chunk * pchunk2 = m_chunkaL2.GetChunkById(iId);
      if(!m_pdecoder->DecoderEOF())
      {
      int32_t iBytesRecorded = m_pdecoder->DecoderFillBuffer(
      pchunk1->m_psh,
      GetChunkByteCount());

      if(m_pdecoder->DecoderEOF())
      {
      m_iLastId = iId;
      }

      int32_t iRemain = GetChunkByteCount() - iBytesRecorded;
      if(iRemain > 0)
      {
      LPBYTE lpb = (LPBYTE) pchunk1->m_psh;
      memset(&lpb[iBytesRecorded], 0, iRemain);
      }

      if(!m_pdevocalizer->Process(pchunk1->m_psh, pchunk2->m_psh))
      {
      return false;
      }

      if(!_PreOutputId(pchunk2))
      {
      return false;
      }

      m_iOutCount++;
      m_iaFree.remove_first(iId);

      m_pcallback->OnPreBufferDone(iId);
      return true;
      }
      else
      {
      return false;
      }*/
   }

   bool wave_pre_buffer::_PreOutputId(Chunk * pchunk)
   {
      if(m_pstreameffectOut != NULL)
      {
         m_pstreameffectOut->Process16bits(pchunk->m_psh, m_iChunkSampleCount);
         if(m_pstreameffectOut->IsEndOfEffect())
         {
            if(!m_pstreameffectOut->m_bEnd)
            {
               delete m_pstreameffectOut;
               m_pstreameffectOut = NULL;
            }
         }
      }
      return true;
   }


   void wave_pre_buffer::_Kick()
   {
      single_lock sl(&m_cs, TRUE);

      while(_DeferKick() && m_bPlay);
   }

   bool wave_pre_buffer::IsDevocalized()
   {
      return m_speffect != NULL; // && m_speffect->IsEnabled();
   }

   void wave_pre_buffer::Devocalize(bool bSet)
   {
      UNREFERENCED_PARAMETER(bSet);
      if(m_speffect == NULL)
         return;
      //m_speffect->Enable(bSet);
   }

   bool wave_pre_buffer::IsEOF()
   {
      return m_pdecoder->DecoderEOF() ||
         (m_pstreameffectOut != NULL && m_pstreameffectOut->IsEndOfEffect());
   }

   void wave_pre_buffer::ClearBuffer()
   {
      //   single_lock sl(&m_cs, TRUE);

      /*
      for(int32_t iId = m_iNextId; iId < iOldNextId; iId++)
      {
      int32_t iChunk = _IdToChunk(iId);
      m_chunkaL2[iChunk].m_iId  = -1;
      }*/
      if(m_iOutCount < 0)
         m_iOutCount = 0;
      m_thread.Step();
   }





   void wave_pre_buffer::Reset()
   {
      single_lock sl(&m_cs, TRUE);
      m_iOutCount = 0;
      m_chunkaL1.Reset();
      m_chunkaL2.Reset();
   }

   wave_pre_buffer::Chunk::Chunk()
   {
      m_iId = -1;
   }


   bool wave_pre_buffer::_UpdateBuffer(int32_t iChannelCount, int32_t iChunkCount, int32_t iChunkSampleCount, int32_t iMinL1BufferCount)
   {
      int32_t iChunk;

      int32_t iTotalChunkCount = (iChunkCount + iMinL1BufferCount);

      m_shaL1.set_size(iTotalChunkCount * iChannelCount * iChunkSampleCount);
      m_shaL2.set_size(iTotalChunkCount * iChannelCount * iChunkSampleCount);

      m_chunkaL1.set_size(iTotalChunkCount);
      for(iChunk = 0; iChunk < iTotalChunkCount; iChunk++)
      {
         m_chunkaL1[iChunk].m_psh = &m_shaL1[iChunk * iChannelCount * iChunkSampleCount];
      }

      m_chunkaL2.set_size(iTotalChunkCount);
      for(iChunk = 0; iChunk < iTotalChunkCount; iChunk++)
      {
         m_chunkaL2[iChunk].m_psh = &m_shaL2[iChunk * iChannelCount * iChunkSampleCount];
      }

      return true;
   }

   int32_t wave_pre_buffer::GetTotalChunkCount()
   {
      return m_iChunkCount + m_iMinFreeBuffer;

   }

   int32_t wave_pre_buffer::_IdToChunk(int32_t iId)
   {
      return iId % GetTotalChunkCount();
   }

   void wave_pre_buffer::ChunkArray::Reset()
   {
      for(int32_t i = 0; i < this->get_size(); i++)
      {
         this->element_at(i).Reset();
      }
   }

   void wave_pre_buffer::Chunk::Reset()
   {
      m_iId = -1;
   }

   wave_pre_buffer::Chunk * wave_pre_buffer::ChunkArray::GetChunkById(int32_t iId)
   {
      return &this->element_at(iId % this->get_size());
   }

   wave_pre_buffer::Thread::Thread(sp(::ca2::application) papp) :
   ca2(papp),
      thread(papp),
      simple_thread(papp),
      m_evStep(papp)
   {
      m_estate = state_initial;
   }

   wave_pre_buffer::Thread::~Thread()
   {
      if(GetState() != state_initial)
      {
         Stop();
      }
   }


   int32_t wave_pre_buffer::Thread::run()
   {
      wave_pre_buffer * pprebuffer = m_pprebuffer;
      m_evStep.wait();
      m_estate = StateRunning;
      while(GetState() == StateRunning && get_run())
      {
         pprebuffer->_Kick();
         m_evStep.ResetEvent();
         m_evStep.wait(millis(100));
      }
      m_estate = state_initial;
      return 0;
   }

   void wave_pre_buffer::Thread::SetPreBuffer(wave_pre_buffer * pprebuffer)
   {
      m_pprebuffer = pprebuffer;
   }

   void wave_pre_buffer::Thread::Step()
   {
      m_evStep.SetEvent();
   }

   void wave_pre_buffer::Thread::Start()
   {
      m_estate = StateStarting;
      begin(::ca2::scheduling_priority_highest);
   }

   void wave_pre_buffer::Thread::Stop()
   {
      m_estate = StateStopping;
      m_evStep.SetEvent();
   }

   void wave_pre_buffer::Start(const imedia::position & position)
   {
      m_bPlay        = true;
      m_position     = position;
      if(m_position > 0)
      {
         primitive::memory mem;
         mem.allocate(1024 * 128);
         imedia::position positionSkip = position;
         ::primitive::memory_size iBytesRecorded;
         while(positionSkip > 0)
         {
            if(natural(positionSkip) > mem.get_size())
            {
               iBytesRecorded = m_pdecoder->DecoderFillBuffer(mem.get_data(), mem.get_size());
            }
            else
            {
               iBytesRecorded = m_pdecoder->DecoderFillBuffer(mem.get_data(), (::primitive::memory_size) positionSkip);
            }
            positionSkip -= iBytesRecorded;
         }
      }
   }

   void wave_pre_buffer::Stop()
   {
      m_bPlay = false;
      //m_thread.Stop();
   }






   int64_t wave_pre_buffer::GetMillisLength()
   {
      if(m_pdecoder != NULL)
         return m_pdecoder->DecoderGetMillisLength();
      else
         return 0;
   }


} // namespace audio





