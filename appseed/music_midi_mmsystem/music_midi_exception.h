#pragma once

#define MIDIPLAYERSTOPEXCEPTION 10000
#define MIDIPLAYERCLOSEEXCEPTION 10010
#define MIDIPLAYERSAVEEXCEPTION 10020

#define MIDIPLAYERPRERROLLSTREAMOPENEXCEPTION 20020
#define MIDIPLAYERPRERROLLSTREAMPROPERTYEXCEPTION 20030
#define MIDIPLAYERPRERROLLNOMEMORY1EXCEPTION 20040
#define MIDIPLAYERPRERROLLNOMEMORY2EXCEPTION 20050
#define MIDIPLAYERPRERROLLPREPAREHEADEREXCEPTION 20060
#define MIDIPLAYERPRERROLLSTREAMOUTEXCEPTION 20070
#define MIDIPLAYERPRERROLLREADEVENTSEXCEPTION 20080
#define MIDIPLAYERPRERROLLPREPAREHEADER2EXCEPTION 20090
#define MIDIPLAYERPRERROLLSTREAMOUT2EXCEPTION 20100

#pragma once


namespace music
{


   namespace midi
   {


      class CLASS_DECL_VERIWELL_MULTIMEDIA exception : public base_exception  
      {
      public:

         enum e_type 
         {
            TypeUndefined,
            TypeMultimediaSystem,
            TypeMidiFile,
         };


         ::multimedia::result                        m_mmr;
         int32_t                           m_iAppError;
         ::music::midi::e_file_result    m_mfr;
         string                        m_strUser;
         e_type                        m_etype;


         exception(sp(::ca2::application) papp);
         exception(sp(::ca2::application) papp, ::multimedia::result mmr, int32_t iAppError);
         exception(sp(::ca2::application) papp, ::music::midi::e_file_result mfr);
         virtual ~exception();

         string GetUserText();
         void SetUserText(const char * lpsz);
         string GetDetailsText();
         void SetMMResult(::multimedia::result mmr);


      };


   } // namespace midi


} // namespace music




