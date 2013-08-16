#include "framework.h"
#include <math.h>


namespace audio
{


   wave_effect_crescendo::wave_effect_crescendo()
   {
      m_bSustain     = true;
      m_bTriggerEnd  = true;
      m_bEnd         = true;
   }

   wave_effect_crescendo::~wave_effect_crescendo()
   {

   }

   void wave_effect_crescendo::SetLengthMillis(uint32_t dwMillis, uint32_t dwSamplesPerSec)
   {
      m_iLength = dwSamplesPerSec * dwMillis / 1000;
   }

   void wave_effect_crescendo::SetFinalScale(int16_t shMul, int16_t shDiv)
   {
      m_shFinalScaleDiv = shMul;
      m_shFinalScaleMul = shDiv;
   }

   int32_t wave_effect_crescendo::Process16bits(int16_t * pshStream, int32_t iCountParam)
   {
      ASSERT(iCountParam % 2 == 0);


      ::count iCount = (count) min(iCountParam, m_iLength - m_iStep);

      int16_t * psh = pshStream;
      int32_t i;
      for(i = 0; i < iCount; i++)
      {
         m_dCurrentRate *= m_dDifferentialRate;
         *psh++ = (int16_t) (*psh * m_dCurrentRate);
         *psh++ = (int16_t) (*psh * m_dCurrentRate);
         m_iStep++;
      }
      if(m_bSustain)
      {
         for(; i < iCountParam; i++)
         {
            *psh++ = (int16_t) (*psh * m_dCurrentRate);
            *psh++ = (int16_t) (*psh * m_dCurrentRate);
            m_iStep++;
         }
      }
      return (int32_t) iCount;
   }

   void wave_effect_crescendo::Initialize()
   {
      m_iStep = 0;
      m_dDifferentialRate = 1.0 - (7.0 / (double) m_iLength);
      m_dCurrentRate = 1.0;
   }

   bool wave_effect_crescendo::IsEndOfEffect()
   {
      if(m_bEnd)
      {
         if(m_iStep >= (int32_t) (m_iLength - 1))
            return true;
      }
      return false;
   }



} // namespace audio


