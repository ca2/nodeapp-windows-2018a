#pragma once


namespace music
{


   namespace midi
   {


      class CLASS_DECL_VERIWELL_MULTIMEDIA util : 
         virtual public ::ca2::object
      {
      protected:


         tracks            m_tracks;
         string2a          m_kar2DTokens;
         string2a          m_tune10002DTokens;
         stringa           m_straTokens;


      public:


         util(sp(::ca2::application) papp);


         bool ExtractLyrics(string & str, sequence &seq);

         ::multimedia::result ConvertKarToXF(stringa *      pKar1DTokens, imedia::position_array *        pKar1DTicks, track *      pXFTrk, const char * psz);
         ::multimedia::result WorkConvertKarToXF(stringa *      pKar1DTokens, imedia::position_array *        pKar1DTicks, track *      pXFTrk, const char * psz);
         ::multimedia::result ConvertToXF(sequence &seq, int32_t iOpenMode);
         e_file_type GetFileType(sequence & seq, stringa * pstra);
         ::multimedia::result ConvertTune1000ToXF(sequence & seq1000, sequence &seqXf, int32_t iOpenMode);
         ::multimedia::result ConvertTune1000ToXF(sequence & seq, int32_t iOpenMode);
         ::multimedia::result ConvertKarToXF(sequence &seqKar, sequence &seqXf, int32_t iOpenMode, stringa & stra);
         ::multimedia::result ConvertKarToXF(sequence &seq, int32_t iOpenMode, stringa & stra);
         void PrepareNoteOnOffEvents(events_v1 * pNoteOnEvents, events_v1 * pNoteOffEvents, int32_t iTrack, int32_t iFormat, events_v1 * pNoteOnOffEvents, imedia::position_array & positiona);
         void PrepareLevel2Events(events_v1 * peventsLevel2Beg, events_v1 * peventsLevel2End, int32_t iTrack, int32_t iFormat, events_v1 * peventsLevel2, imedia::position_array & positiona);


      };


   } // namespace midi


} // namespace music






