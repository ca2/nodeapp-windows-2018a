#pragma once


namespace music
{


   namespace midi_mmsystem
   {


      class sequence;


      class CLASS_DECL_VERIWELL_MULTIMEDIA_MUSIC_MIDI_MMSYSTEM document_helper :
         public virtual ::ca2::object
      {
      public:

         sequence         m_sequenceHelper;
         bool                          m_bSavingHelper;
         ::music::karaoke_util                   m_karaokeutilHelper;
         sp(::music::microke1::section)               m_pmicroke1;
         bool                          m_bIsSoftKaraokeFile;
         e_file_type      m_efiletype;
         ::music::xf::info_headers                m_xfInfoHeaders;


         document_helper(sp(::ca2::application) papp);
         virtual ~document_helper();

         bool HelperOpenFile(primitive::memory & storage);

         virtual sequence & get_midi_sequence();
         virtual void SetLyricDelay(int32_t iDelay);
         virtual void SetTempoShift(int32_t iShift);
         ::music::xf::info_headers * HelperGetXFInfoHeaderSet();
         sp(::user::document) HelperGetDocument();
         ::music::xf::info_header * HelperGetXFInfoHeader();
         bool SuperHelperOnOpenDocument(const char * lpcszPath);
         void HelperDeleteContents();
         bool HelperOnOpenDocument(var varFile);
         void HelperSetModifiedFlag(bool bModified = TRUE );
         bool HelperIsSaving();

         virtual ::ca2::thread * HelperGetMidiPlayerCallbackThread() = 0;

         inline sequence & HelperGetMidiSequence();


      };


   } // namespace midi_mmsystem


} // namespace music





