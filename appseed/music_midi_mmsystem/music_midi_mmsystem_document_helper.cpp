#include "framework.h"


namespace music
{


   namespace midi
   {


      document_helper::document_helper(sp(::ca2::application) papp) :
         m_sequenceHelper(papp),
         m_karaokeutilHelper(papp),
         ca2(papp)
      {
         
         m_bSavingHelper = false;

         m_pmicroke1 = new ::music::microke1::section(papp);

      }


      document_helper::~document_helper()
      {

         if(m_pmicroke1 != NULL)
         {
            delete m_pmicroke1;
            m_pmicroke1 = NULL;
         }

      }

      bool document_helper::HelperIsSaving()
      {
         return m_bSavingHelper;
      }

      void document_helper::HelperSetModifiedFlag(bool bModified)
      {

         HelperGetDocument()->set_modified_flag(bModified);
      }

      sp(::user::document) document_helper::HelperGetDocument()
      {
         sp(::user::document) pdoc = (this);
         ASSERT(pdoc);
         return pdoc;
      }


      void document_helper::HelperDeleteContents()
      {
         file & file = HelperGetMidiSequence().GetFile();
         file.delete_contents();
      }

      bool document_helper::SuperHelperOnOpenDocument(const char * lpcszPath)
      {
         UNREFERENCED_PARAMETER(lpcszPath);
         ASSERT(FALSE);
         return FALSE;
      }

      ::music::xf::info_header * document_helper::HelperGetXFInfoHeader()
      {
         return &m_xfInfoHeaders.m_xfInfoHeader;

      }

      ::music::xf::info_headers * document_helper::HelperGetXFInfoHeaderSet()
      {
         return &m_xfInfoHeaders;
      }

      void document_helper::SetTempoShift(int32_t iShift)
      {
         UNREFERENCED_PARAMETER(iShift);
      }

      void document_helper::SetLyricDelay(int32_t iDelay)
      {
         UNREFERENCED_PARAMETER(iDelay);
      }





      sequence & document_helper::GetMidiSequence()
      {
         return m_sequenceHelper;
      }

      bool document_helper::HelperOpenFile(primitive::memory & storage)
      {
         if(storage.get_data() == NULL || storage.get_size() <= 0)
            return false;
         if(m_pmicroke1->IsMicroke1File(storage))
         {
            primitive::memory storageExtract;
            if(!m_pmicroke1->ExtractFile(storage, storageExtract))
               return false;
            return HelperOpenFile(storageExtract);
         }
         else if(m_karaokeutilHelper.IsStar350File(storage))
         {
            ::music::star350::file starFile(get_app());
            starFile.OpenFile(&storage, 0, ::music::storage_attach);
            m_karaokeutilHelper.ConvertStar350ToXF(&starFile, &HelperGetMidiSequence(), OpenForPlaying);
            HelperGetMidiSequence().GetFile().ToWorkStorage();
         }
         else
         {
            ::multimedia::result mmr = HelperGetMidiSequence().OpenFile(&storage, OpenForPlaying, ::music::storage_own);
            if(mmr != MMSYSERR_NOERROR)
               return false;
            util midiutil(get_app());
            midiutil.ConvertToXF(HelperGetMidiSequence(), OpenForPlaying);
         }
         HelperGetMidiSequence().GetFile().GetTracks().GetXFInfoHeaders(&m_xfInfoHeaders);
         return TRUE;
      }

      bool document_helper::HelperOnOpenDocument(var varFile)
      {


         primitive::memory storage;

         try
         {
            Application.file().as_memory(varFile, storage);
         }
         catch(not_licensed & e)
         {
            string strLink = e.m_strUrl;
            if(System.get_host_location_url().has_char())
            {
               ::ca2::property_set setQuery(get_app());
               setQuery.parse_url_query(strLink);
               if(setQuery.has_property("karfile_url"))
               {
                  System.url().string_set(strLink, "karfile_url", (const char *) System.get_host_location_url());
               }
            }
            System.open_link(strLink, "ca2-licensing");
            return FALSE;
         }
         catch(...)
         {
            return FALSE;
         }


         return HelperOpenFile(storage) ? TRUE : FALSE;


      }

      sequence & document_helper::HelperGetMidiSequence()
      {
         return m_sequenceHelper;
      }


   } // namespace midi


} // namespace music









