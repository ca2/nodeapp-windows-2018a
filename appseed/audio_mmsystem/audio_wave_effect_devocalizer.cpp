#include "framework.h"
#include <math.h>


namespace audio
{


   wave_effect_devocalizer::wave_effect_devocalizer(sp(::ca2::application) papp, bool bEnable, int32_t iSampleCount, int32_t iSamplesPerSecond, int32_t iDivisionCount, int32_t iGroupCount) :
      ca2(papp),
      m_groupa(papp)
   {


      m_iChannelCount = 2;

      m_bEnable = bEnable;

      m_iSamplesPerSecond = iSamplesPerSecond;
      m_iDivisionCount = iDivisionCount;
      m_iGroupCount = iGroupCount;
      m_iSampleCount = iSampleCount;
      m_spfftd.release();
      m_spifftd.release();
      m_spfftd = new kissfft::fftd(m_iSampleCount);
      m_spifftd = new kissfft::ifftd(m_iSampleCount);

      double fMinFreq = 110.0; // Hz
      double fMaxFreq = 1000.0; // Hz

      m_iMin = GetFrequencyIndex(fMinFreq);
      m_iMax = GetFrequencyIndex(fMaxFreq);
      m_iCancelNoteCount = 1;
      m_wDiv = 30;

      m_iInputDivisionCount = 0;
      m_iFirstInputDivision = 0;
      m_iFirstChunk = 0;
      m_iChunkCount = 0;
      m_iFirstInputGroup = 0;
      m_iInputGroupCount = 0;
      m_iOutChunk = 0;
      m_iOutDivision = 0;
      m_iOutputCount = 0;
      m_iOutputGroup = 0;
      m_iFirstOutput = 0;

      m_f2aMax.set_size(m_iChannelCount);
      m_i2aMax.set_size(m_iChannelCount);

      int32_t i;
      for(i = 0; i < m_iChannelCount; i++)
      {
         m_f2aMax[i].set_size(m_iMax - m_iMin + 1);
         //m_f2aMax[i].AddCoupledSwapInterface(&m_i2aMax[i]);
         m_i2aMax[i].set_size(m_iMax - m_iMin + 1);
      }


      m_chunka.set_size(iGroupCount / iDivisionCount + 2);
      m_chunkaOut.set_size(m_chunka.get_size());
      m_groupa.set_size(m_chunka.get_size() * m_iDivisionCount);

      for(i = 0; i < m_groupa.get_size(); i++)
      {
         m_groupa[i].Initialize(this);
      }

      for(i = 0; i < m_chunka.get_size(); i++)
      {
         m_chunka[i].Initialize(this);
      }

      for(i = 0; i < m_chunkaOut.get_size(); i++)
      {
         m_chunkaOut[i].Initialize(this);
      }

      m_chunk.Initialize(this);

      m_iDivisionSampleCount = m_iSampleCount / m_iDivisionCount;

   }

   wave_effect_devocalizer::~wave_effect_devocalizer()
   {
   }

   void wave_effect_devocalizer::Chunk::Initialize(wave_effect_devocalizer * pparent)
   {
      m_pparent = pparent;
      int32_t iSampleCount = pparent->m_iSampleCount;
      int32_t iChannelCount = m_pparent->m_iChannelCount;
      m_sha.set_size(iSampleCount * iChannelCount);
      memset(m_sha.get_data(), 0, sizeof(int16_t) * iSampleCount * iChannelCount);
   }


   bool wave_effect_devocalizer::Process(int16_t * pshIn, int16_t * pshOut)
   {
      if(!m_bEnable)
      {
         memcpy(pshOut, pshIn, m_iSampleCount * 2 * 2);
         return true;
      }

      if(m_iChunkCount <= 0)
      {
         m_iFirstChunk = 0;
         m_iChunkCount = 1;
      }
      else
      {
         m_iChunkCount++;
      }

      m_iInputDivisionCount += m_iDivisionCount;

      int32_t iChunk = (m_iFirstChunk + m_iChunkCount - 1) % m_chunka.get_size();


      m_chunka[iChunk].Input(pshIn);

      ChunkToGroup();

      GroupToOutput();

      return Output(pshOut);

   }


   int32_t wave_effect_devocalizer::Process16bits(int16_t * pshStream, int32_t iCount)
   {
      UNREFERENCED_PARAMETER(pshStream);
      /* bool bDiff     = false;
      bool bFilter   = true;

      double * pfL = m_pfL1;
      double * pfR = m_pfR1;
      int16_t * psh = pshStream;
      double * pfL2;
      double * pfR2;

      int32_t iDivisor;

      for(int32_t i = 0; i < 16 * 1024; i++)
      {
      *pfL++ = *psh++;
      *pfR++ = *psh++;
      }

      pfL = m_pfL1;
      pfR = m_pfR1;
      double * pfL3 = m_pfL3;
      double * pfR3 = m_pfR3;

      iDivisor = 0;

      if(bDiff)
      {
      iDivisor += 3;
      for(i = 0; i < 16 * 1024; i++)
      {
      *pfL3 = (*pfL++ - *pfR++) * 3;
      *pfR3 = *pfL3;
      *pfL3++;
      *pfR3++;
      }
      }
      else
      {
      iDivisor += 1;
      for(i = 0; i < 16 * 1024; i++)
      {
      *pfL3++ = 0.0f;
      *pfR3++ = 0.0f;
      }
      }

      if(bFilter)
      {
      iDivisor += 1;

      double fMod;
      const int32_t iMaxCount = 3;
      CBaseSortArray < double, double > m_faMax;
      Carray < int32_t, int32_t > m_iaMax;

      m_faMax.AddCoupledSwapInterface(&m_iaMax);
      //double pfMax[iMaxCount];
      //int32_t piMax[iMaxCount];

      m_pfftd->Fft(m_pfL1, m_pcpxL);
      double fMean;
      double fArea = 0.0;
      for(i = 0; i < 8 * 1024 + 1; i++)
      {
      m_pcpxL[i].r /= 16384 / 2;
      m_pcpxL[i].i /= 16384 / 2;
      m_pfMod[i] = sqrt(m_pcpxL[i].r * m_pcpxL[i].r + m_pcpxL[i].i * m_pcpxL[i].i);
      fArea += m_pfMod[i];
      }
      fMean = fArea / (8 * 1024 + 1);
      // 82 -> 220 Hz
      // 163 -> 440 Hz
      int32_t j;
      int32_t iBin;
      for(i = 82; i <= 326; i++)
      {
      fMod = 0.0f;
      for(j = 1;;j++)
      {
      iBin = i * j;
      if(iBin > (2 * 1024 + 1))
      break;
      fMod += m_pfMod[iBin];
      }
      /*         for(j = 2;;j++)
      {
      iBin = i / j;
      if(iBin < 82)
      break;
      fMod += m_pfMod[iBin];
      }*/

      /*     m_faMax.add(fMod);
      m_iaMax.add(i);
      }
      int32_t wDiv = 10;
      //goto breakFilter1;
      m_faMax.quick_sort();

      double wHalf;

      //if(false)
      int32_t k;
      int32_t iMax;
      double fMax;
      double fFirstMax = m_faMax[m_iaMax.get_size() - 1];
      double fRate = fMean * 5.0f / fFirstMax;
      double fDiv;
      double fTonic;
      for(k = 0; k < 5; k++)
      {
      fMax = m_faMax[m_iaMax.get_size() - k - 1];
      if(fMax < 100)
      break;
      iMax = m_iaMax[m_iaMax.get_size() - k - 1];
      double dFreq = iMax * (44100.0 / 2)/ (16 * 1024 / 2);
      for(i = 1;;i++)
      {
      iBin = iMax * i;
      if(iBin > 1536)
      break;
      fTonic = max(11.0f - i, 1.0) * 10.0;
      wHalf = iBin / wDiv;
      for(j = - wHalf + 1; j < wHalf; j++)
      {
      fDiv = fTonic * (wHalf - abs(j));
      m_pcpxL[iBin + j].r /= fDiv;
      m_pcpxL[iBin + j].i /= fDiv;
      }
      /*            m_pcpxL[iBin - 1].r = 0;
      m_pcpxL[iBin - 1].i = 0;
      m_pcpxL[iBin].r = 0;
      m_pcpxL[iBin].i = 0;
      m_pcpxL[iBin + 1].r = 0;
      m_pcpxL[iBin + 1].i = 0;
      m_pcpxL[iBin + 2].r = 0;
      m_pcpxL[iBin + 2].i = 0;*/
      //   }
      /*         for(i = 2;;i++)
      {
      iBin = iMax / i;
      if(iBin < 82)
      break;
      wHalf = iBin / wDiv;
      for(j = -wHalf; j <= wHalf; j++)
      {
      m_pcpxL[iBin + j].r /= (wHalf - abs(j));
      m_pcpxL[iBin + j].i /= (wHalf - abs(j));
      }
      }*/
      /*      }
      //m_pcpxL[i].r /= 16384;
      //m_pcpxL[i].i /= 16384;
      breakFilter1:
      m_pffti->Fft(m_pcpxL, m_pfL2);



      m_pfftd->Fft(m_pfR1, m_pcpxR);
      for(i = 0; i < 8 * 1024 + 1; i++)
      {
      //m_pcpxR[i].r /= 16384;
      //m_pcpxR[i].i /= 16384;
      m_pcpxR[i].r = 0;
      m_pcpxR[i].i = 0;
      }
      m_pffti->Fft(m_pcpxR, m_pfR2);
      }
      else
      {
      pfL  = m_pfL1;
      pfR  = m_pfR1;
      pfL2 = m_pfL2;
      pfR2 = m_pfR2;
      iDivisor += 1;
      for(i = 0; i < 16 * 1024; i++)
      {
      *pfL2++ = 0;
      *pfR2++ = 0;
      }
      }


      pfL  = m_pfL1;
      pfR  = m_pfR1;
      pfL2 = m_pfL2;
      pfR2 = m_pfR2;
      pfL3 = m_pfL3;
      pfR3 = m_pfR3;

      for(i = 0; i < 16 * 1024; i++)
      {
      *pfL++ = *pfL2++ + *pfL3++;
      *pfR++ = *pfR2++ + *pfR3++;
      }

      pfL = m_pfL1;
      pfR = m_pfR1;
      psh = pshStream;

      iDivisor *= 2;

      for(i = 0; i < 16 * 1024; i++)
      {
      *psh++ = *pfL++ / iDivisor;
      *psh++ = *pfR++ / iDivisor;
      }
      return iCount;
      */
      return iCount;
   }

   wave_effect_devocalizer::Group::Group(sp(::ca2::application) papp) :
      ca2(papp),
      m_channela(papp)
   {
   }

   void wave_effect_devocalizer::Group::Initialize(wave_effect_devocalizer * pparent)
   {
      m_pparent = pparent;
      int32_t iSampleCount = pparent->m_iSampleCount;
      m_channela.set_size(pparent->m_iChannelCount);

      for(int32_t i = 0; i < m_channela.get_size(); i++)
      {
         m_channela[i].Initialize(this);
      }
      m_daMod.set_size(iSampleCount);
   }

   void wave_effect_devocalizer::Group::Channel::Initialize(Group * pgroup)
   {
      m_pgroup = pgroup;
      int32_t iSampleCount = m_pgroup->m_pparent->m_iSampleCount;
      m_da1.set_size(iSampleCount);
      m_da2.set_size(iSampleCount);
      m_da3.set_size(iSampleCount);
      m_cpxa.set_size(iSampleCount / 2 + 1);
      m_faMax.set_size(m_pgroup->m_pparent->m_iMax - m_pgroup->m_pparent->m_iMin + 1);
      m_iaMax.set_size(m_pgroup->m_pparent->m_iMax - m_pgroup->m_pparent->m_iMin + 1);
   }


   bool wave_effect_devocalizer::GroupToOutput()
   {

      int32_t i;
      int32_t iGroup;
      int32_t j;
      int32_t k;

      while(m_iInputGroupCount >= m_iGroupCount)
      {

         for(k = 0; k < m_f2aMax.get_size(); k++)
         {
            for(j = m_iMin; j <= m_iMax; j++)
            {
               m_f2aMax[k][j - m_iMin] = 0.0;
               m_i2aMax[k][j - m_iMin] = j;
               for(i = 0; i < m_iGroupCount; i++)
               {
                  iGroup = i + m_iOutputGroup;
                  iGroup %= m_groupa.get_size();
                  m_f2aMax[k][j - m_iMin] += m_groupa[i].m_channela[k].m_faMax[j - m_iMin];
               }
            }
            m_f2aMax[k].quick_sort();
         }



         int32_t n = m_iDivisionSampleCount * m_iChannelCount * m_iOutDivision;

         m_groupa[m_iOutputGroup].Output1(&m_chunkaOut[m_iOutChunk].m_sha[n]);

         int32_t iOutDiv2 = m_iOutDivision;
         int32_t iOutChk2 = m_iOutChunk;

         iOutDiv2++;
         if(iOutDiv2 >= m_iDivisionCount)
         {
            iOutDiv2 = 0;
            iOutChk2++;
            if(iOutChk2 >= m_chunkaOut.get_size())
               iOutChk2 = 0;
         }
         n = m_iDivisionSampleCount * m_iChannelCount * iOutDiv2;
         if(m_iGroupCount > 1)
         {
            m_groupa[m_iOutputGroup].Output2(&m_chunkaOut[iOutChk2].m_sha[n]);
         }

         m_iOutputGroup = (m_iOutputGroup + 1) % m_groupa.get_size();

         m_iOutDivision++;
         if(m_iOutDivision >= m_iDivisionCount)
         {
            m_iOutDivision = 0;
            m_iOutChunk++;
            if(m_iOutChunk >= m_chunkaOut.get_size())
               m_iOutChunk = 0;
         }

         m_iOutputCount++;

         m_iInputGroupCount--;

         /*   m_iFirstOutputGroup++;
         if(m_iFirstOutputGroup >= m_groupa.get_size())
         m_iFirstOutputGroup = 0;
         m_iOutputGroupCount--;
         m_iFirstInputDivision++;
         if(m_iFirstInputDivision >= m_chunka.get_size() * m_iDivisionCount)
         m_iFirstInputDivision = 0;
         m_iInputDivisionCount--;*/
      }

      return true;
   }


   bool wave_effect_devocalizer::Output(int16_t * psh)
   {
      if(m_iOutputCount < m_iDivisionCount)
      {
         return false;
      }

      memcpy(psh, m_chunkaOut[m_iFirstOutput].m_sha.get_data(), m_iSampleCount * sizeof(int16_t) * m_iChannelCount);

      m_iFirstOutput++;
      if(m_iFirstOutput >= m_chunkaOut.get_size())
         m_iFirstOutput = 0;

      m_iOutputCount -= m_iDivisionCount;

      return true;
   }

   bool wave_effect_devocalizer::Chunk::Input(int16_t * pshStream)
   {
      memcpy(m_sha.get_data(), pshStream, m_pparent->m_iSampleCount * 2 * m_pparent->m_iChannelCount);
      return true;
   }


   wave_effect_devocalizer::Group::Channel::Channel(sp(::ca2::application) papp) :
      ca2(papp)
   {
   }


   void wave_effect_devocalizer::Group::Channel::Input(bool bFilter)
   {
      int32_t i, k;
      int32_t iSampleCount = m_pgroup->m_pparent->m_iSampleCount;
      //   int32_t iBinCount = iSampleCount / 2 + 1;
      double wDiv = m_pgroup->m_pparent->m_wDiv;

      if(false)
      {
         m_pgroup->m_dDivisor += 1,9;

         double fMod;
         //      const int32_t iMaxCount = 3;

         m_pgroup->m_pparent->m_spfftd->fft(m_da1.get_data(), m_cpxa.get_data());
         double fMean;
         double fArea = 0.0;
         double fDiv;
         double fTonic;
         double wHalf;
         int32_t iBinCount = iSampleCount / 2 + 1;
         for(i = 0; i < iBinCount; i++)
         {
            m_cpxa[i].r /= 16384.0 / 2.0;
            m_cpxa[i].i /= 16384.0 / 2.0;
            m_pgroup->m_daMod[i] = (double) ::sqrt(m_cpxa[i].r * m_cpxa[i].r + m_cpxa[i].i * m_cpxa[i].i);
            fArea += m_pgroup->m_daMod[i];
         }
         fMean = fArea / iBinCount;
         int32_t j;
         int32_t iBin;
         for(i = m_pgroup->m_pparent->m_iMin; i <= m_pgroup->m_pparent->m_iMax; i++)
         {
            fMod = 0.0f;
            for(j = 1; j < 5;j++)
            {
               iBin = i * j;
               if(iBin > iBinCount)
                  break;

               fTonic = (double) (max(11.0f - i, 1.0) * 45.0);
               wHalf = iBin / wDiv;
               for(k = (int32_t) -wHalf; k <= (int32_t) wHalf; k++)
               {
                  fDiv = (double) fabs(fTonic * (wHalf - abs(k) + 1));
                  if((iBin + k)> iBinCount)
                     break;
                  fMod += m_pgroup->m_daMod[iBin + k] / fDiv;
               }
               //fMod += m_pgroup->m_pfMod[iBin];
            }
            /*         for(j = 2;;j++)
            {
            iBin = i / j;
            if(iBin < 82)
            break;
            fMod += m_pfMod[iBin];
            }*/

            m_faMax[i - m_pgroup->m_pparent->m_iMin] = fMod;
            m_iaMax[i - m_pgroup->m_pparent->m_iMin] = i;
            if(i == 91)
            {
               //TRACE("Input iaMax = %d fMod = %f\n", i, fMod);
            }
         }
         //      int32_t wDiv = 10;




      }
      if(bFilter)
      {
         m_pgroup->m_dDivisor += 1.5 ;

         double fDivisor = iSampleCount / 4.0f;
         if(m_pgroup->m_pparent->m_iGroupCount > 1)
         {
            fDivisor = iSampleCount * 2.0f;
         }

         //      const int32_t iMaxCount = 3;

         m_pgroup->m_pparent->m_spfftd->fft(m_da1.get_data(), m_cpxa.get_data());
         //      double fArea = 0.0;
         int32_t iBinCount = iSampleCount / 2 + 1;
         int32_t iLBin = m_pgroup->m_pparent->GetFrequencyIndex(220.0);
         int32_t iUBin = m_pgroup->m_pparent->GetFrequencyIndex(4000.0);

         for(i = 0; i < iLBin; i++)
         {
            m_cpxa[i].r /= fDivisor;
            m_cpxa[i].i /= fDivisor;
         }
         for(; i < iUBin; i++)
         {
            m_cpxa[i].r = 0.0f;
            m_cpxa[i].i = 0.0f;
         }
         for(; i < iBinCount; i++)
         {
            m_cpxa[i].r /= fDivisor;
            m_cpxa[i].i /= fDivisor;
         }




      }
      else
      {
         double * pf2 = m_da2.get_data();
         m_pgroup->m_dDivisor += 1;
         for(i = 0; i < iSampleCount; i++)
         {
            *pf2++ = 0;
         }
      }
   }

   bool wave_effect_devocalizer::Group::Input(int16_t * pshStream)
   {
      bool bDiff     = true;
      bool bFilter   = true;

      double * pfL = m_channela[0].m_da1.get_data();
      double * pfR = m_channela[1].m_da1.get_data();
      int16_t * psh = pshStream;
      double * pfL2;
      double * pfR2;
      double * pfL3_;
      double * pfR3_;

      int32_t iSampleCount = m_pparent->m_iSampleCount;

      int32_t i;
      for(i = 0; i < iSampleCount; i++)
      {
         *pfL++ = *psh++;
         *pfR++ = *psh++;
      }


      pfL   = m_channela[0].m_da1.get_data();
      pfR   = m_channela[1].m_da1.get_data();
      pfL3_ = m_channela[0].m_da3.get_data();
      pfR3_ = m_channela[1].m_da3.get_data();

      m_dDivisor = 0;

      if(bDiff)
      {
         m_dDivisor += 1.0;
         for(i = 0; i < iSampleCount; i++)
         {
            *pfL3_ = (double) ((*pfL - *pfR) / 1.0);
            *pfR3_ = *pfL3_;
            pfL3_++;
            pfR3_++;
            pfL++;
            pfR++;
         }
      }
      else
      {
         m_dDivisor += 1.0;
         for(i = 0; i < iSampleCount; i++)
         {
            *pfL3_++ = 0.0f;
            *pfR3_++ = 0.0f;
         }
      }

      m_channela[0].Input(bFilter);
      m_channela[1].Input(bFilter);


      if(!bDiff && !bFilter)
      {
         pfL   = m_channela[0].m_da1.get_data();
         pfR   = m_channela[1].m_da1.get_data();
         pfL2  = m_channela[0].m_da2.get_data();
         pfR2  = m_channela[1].m_da2.get_data();
         m_dDivisor += 1.0;
         for(i = 0; i < iSampleCount; i++)
         {
            *pfL2++ = *pfL++;
            *pfR2++ = *pfR++;
         }
      }

      return true;

   }

   void wave_effect_devocalizer::Group::Channel::Output(
      sort_array < double, double > & faMax,
      int_array & iaMax)
   {

      //   double * pfL = m_pf1;
      double wHalf;

      int32_t iSampleCount = m_pgroup->m_pparent->m_iSampleCount;
      int32_t iBinCount = iSampleCount / 2 + 1;

      int32_t iMaxVoiceBin = m_pgroup->m_pparent->GetFrequencyIndex(8000.0);
      int32_t i;
      int32_t iBin;
      double wDiv = m_pgroup->m_pparent->m_wDiv;
      int32_t j;
      //if(false)
      int32_t k;
      int32_t iMax;
      double fMax;
      //   double fFirstMax = faMax[m_iaMax.get_size() - 1];
      //      double fRate = fMean * 5.0f / fFirstMax;
      double fDiv;
      double fTonic;
      int32_t iCount = m_pgroup->m_pparent->m_iSampleCount / 2 + 1;
      int_array ia;
      ia.set_size(iCount);
      if(false)
      {
         for(k = 0; k < m_pgroup->m_pparent->m_iCancelNoteCount; k++)
         {
            fMax = faMax[m_iaMax.get_size() - k - 1];
            iMax = iaMax[m_iaMax.get_size() - k - 1];
            TRACE("Output iMax = %d fMax = %f\n", iMax, fMax);

            //         double dFreq = iMax * (m_pgroup->m_pparent->m_iSamplesPerSecond)/ (iBinCount);
            for(i = 1;;i++)
            {
               iBin = iMax * i;
               if(iBin > iMaxVoiceBin)
                  break;
               if(iBin > iBinCount)
                  break;
               fTonic = (double) (max(6.0f - i, 1.0) * 90.0);
               wHalf = iBin / (wDiv + i);
               for(j = (int32_t) (-wHalf + 1.0f); j < (int32_t) wHalf; j++)
               {
                  fDiv = fTonic * (wHalf - abs(j));
                  if(iBin + j> iMaxVoiceBin)
                     break;
                  m_cpxa[iBin + j].r /= fDiv;
                  m_cpxa[iBin + j].i /= fDiv;
                  ia[iBin + j] = 1;
               }
            }
         }
      }
      /*
      for(j = 0; j < iCount; j++)
      {
      if(ia[j] <= 0)
      {
      m_pcpx[j].r = 0.0;
      m_pcpx[j].i = 0.0;
      }
      }
      */
      m_pgroup->m_pparent->m_spifftd->ifft(m_cpxa.get_data(), m_da2.get_data());
   }

   bool wave_effect_devocalizer::Group::Output1(int16_t * pshStream)
   {
      int32_t i;
      for(i = 0; i < m_pparent->m_iChannelCount; i++)
      {
         m_channela[i].Output(m_pparent->m_f2aMax[i], m_pparent->m_i2aMax[i]);
      }

      double * pfL = m_channela[0].m_da1.get_data();
      double * pfR = m_channela[1].m_da1.get_data();
      int16_t * psh = pshStream;
      double * pfL2;
      double * pfR2;
      double * pfL3_;
      double * pfR3_;


      pfL  = m_channela[0].m_da1.get_data();
      pfR  = m_channela[1].m_da1.get_data();
      pfL2 = m_channela[0].m_da2.get_data();
      pfR2 = m_channela[1].m_da2.get_data();
      pfL3_ = m_channela[0].m_da3.get_data();
      pfR3_ = m_channela[1].m_da3.get_data();

      int32_t iCount = m_pparent->m_iDivisionSampleCount * 2;

      for(i = 0; i < iCount; i++)
      {
         *pfL++ = *pfL2++ + *pfL3_++;
         *pfR++ = *pfR2++ + *pfR3_++;
      }

      pfL  = m_channela[0].m_da1.get_data();
      pfR  = m_channela[1].m_da1.get_data();
      psh = pshStream;

      m_dDivisor *= 1.2f;

      if(m_pparent->m_iGroupCount > 1)
      {
         m_dDivisor /= 3.2f;
      }

      //iDivisor = 1;

      iCount /= 2;

      double f;

      if(m_pparent->m_iGroupCount > 1)
      {
         for(i = 0; i < iCount; i++)
         {
            f = ((double)*psh + *pfL++) / m_dDivisor;

            if(f >= 32767.0f)
               *psh = 32767;
            else if(f <= -32768.0f)
               *psh = -32768;
            else
               *psh = (int16_t) f;
            psh++;

            f = ((double)*psh + *pfR++) / m_dDivisor;
            if(f >= 32767.0f)
               *psh = 32767;
            else if(f <= -32768.0f)
               *psh = -32768;
            else
               *psh = (int16_t) f;
            psh++;
         }
      }
      else
      {
         for(i = 0; i < iCount; i++)
         {
            f = *pfL++ / m_dDivisor;
            if(f >= 32767.0f)
               *psh = 32767;
            else if(f <= -32768.0f)
               *psh = -32768;
            else
               *psh = (int16_t) f;
            psh++;

            f = *pfR++ / m_dDivisor;
            if(f >= 32767.0f)
               *psh = 32767;
            else if(f <= -32768.0f)
               *psh = -32768;
            else
               *psh = (int16_t) f;
            psh++;
         }
      }

      return true;
   }

   bool wave_effect_devocalizer::Group::Output2(int16_t * pshStream)
   {

      int32_t iCount = m_pparent->m_iDivisionSampleCount;
      double * pfL = &m_channela[0].m_da1[iCount];
      double * pfR = &m_channela[1].m_da1[iCount];

      int16_t * psh = pshStream;



      for(int32_t i = 0; i < iCount; i++)
      {
         *psh++ = (int16_t) *pfL++;
         *psh++ = (int16_t) *pfR++;
      }

      return true;
   }


   bool wave_effect_devocalizer::IsEnabled()
   {
      return m_bEnable;
   }

   void wave_effect_devocalizer::Enable(bool bSet)
   {
      m_bEnable = bSet;
   }

   void wave_effect_devocalizer::PrepareChunk(int32_t iDivision)
   {
      int32_t i;
      int32_t n;
      int32_t s;
      int32_t iLChunk = iDivision / m_iDivisionCount;
      int32_t iLDivision = iDivision % m_iDivisionCount;
      int32_t iUChunk = ((iDivision + m_iDivisionCount - 1) / m_iDivisionCount) % m_chunka.get_size();
      int32_t iUDivision = (iDivision + m_iDivisionCount - 1) % m_iDivisionCount;

      s = m_iSampleCount * m_iChannelCount;

      i = s * iLDivision / m_iDivisionCount;
      n = s - i;
      memcpy(m_chunk.m_sha.get_data(), &m_chunka[iLChunk].m_sha[i], n * 2);

      int32_t j = n;

      if(iUChunk != iLChunk)
      {
         n = s * (iUDivision + 1) / m_iDivisionCount;
         memcpy(&m_chunk.m_sha[j], m_chunka[iUChunk].m_sha.get_data(), n * 2);
      }
   }

   void wave_effect_devocalizer::InputChunk()
   {
      m_iInputGroupCount++;
      m_iFirstInputGroup = (m_iFirstInputGroup + 1) % m_groupa.get_size();

      m_groupa[m_iFirstInputGroup].Input(m_chunk.m_sha.get_data());


   }

   void wave_effect_devocalizer::ChunkToGroup()
   {
      while(m_iInputDivisionCount >= m_iDivisionCount)
      {
         PrepareChunk(m_iFirstInputDivision);
         InputChunk();
         m_iFirstInputDivision++;
         if(m_iFirstInputDivision >= m_groupa.get_size())
            m_iFirstInputDivision = 0;
         m_iInputDivisionCount--;
      }

   }

   int32_t wave_effect_devocalizer::GetFrequencyIndex(double dFrequency)
   {
      return (int32_t) (dFrequency * (m_iSampleCount / 2) / (m_iSamplesPerSecond / 2));
   }



} // namespace audio


