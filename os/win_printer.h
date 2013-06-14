#pragma once


namespace win
{


   class CLASS_DECL_win printer :
      virtual public ::user::printer
   {
   public:


      class CLASS_DECL_win document_properties :
         virtual public ::ca2::object
      {
      public:


         DEVMODE *      m_pdevmode;
         HDC            m_hdc;


         document_properties(sp(::ca2::application) papp);
         virtual ~document_properties();


         virtual bool initialize(::win::printer * pprinter, DEVMODE * pdevmode = NULL);
         virtual bool close();
         virtual ::ca2::graphics * create_graphics();

      };


      HANDLE                  m_hPrinter;
      document_properties     m_documentproperties;


      printer(sp(::ca2::application) papp);
      virtual ~printer();


      virtual bool open(const char * pszDeviceName);
      virtual ::ca2::graphics * create_graphics();
      virtual bool is_opened();
      virtual bool close();



   };
} // namespace user