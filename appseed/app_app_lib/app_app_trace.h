#pragma once


namespace app_app
{


   class SPALIB_API trace_file :
      virtual public ::object
   {
   public:


      string            m_strLabel;
      ::mutex           m_mutex;
      synch_lock        m_sl;

      ::file::file_sp   m_pfile;


      trace_file(::aura::application * papp, const string & strLabel);
      ~trace_file();


      void print(const string & str);
      void print(double dRate);


   };


} // namespace app_app




