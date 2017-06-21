#pragma once


namespace app_app
{


   class SPALIB_API trace_file
   {
   public:


      app  *  m_papp;
      mutex          m_mutex;
      synch_lock     m_sl;

      HANDLE         m_hfile;


      trace_file(app * papp);
      ~trace_file();


      void print(const string & str);
      void print(double dRate);


   };


} // namespace app_app




