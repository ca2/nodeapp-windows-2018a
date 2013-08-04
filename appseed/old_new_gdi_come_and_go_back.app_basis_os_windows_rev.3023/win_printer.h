#pragma once

namespace win
{
   
   class CLASS_DECL_VMSWIN printer :
      virtual public ::user::printer
   {
   public:


      class CLASS_DECL_VMSWIN document_properties :
         virtual public ::radix::object
      {
      public:


         DEVMODE *      m_pdevmode;
         HDC            m_hdc;


         document_properties(::ca::application * papp);
         virtual ~document_properties();


         virtual bool initialize(::win::printer * pprinter, DEVMODE * pdevmode = NULL);
         virtual bool close();
         virtual ::ca::graphics * create_graphics();

      };


      HANDLE                  m_hPrinter;
      document_properties     m_documentproperties;


      printer(::ca::application * papp);
      virtual ~printer();


      virtual bool open(const char * pszDeviceName);
      virtual ::ca::graphics * create_graphics();
      virtual bool is_opened();
      virtual bool close();



   };
} // namespace user