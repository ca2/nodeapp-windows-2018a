#pragma once


namespace a_spa
{


   class SPALIB_API trace_file
   {
   public:


      simple_app  *  m_papp;
      mutex          m_mutex;
      synch_lock     m_sl;

      HANDLE         m_hfile;


      trace_file(simple_app * papp);
      ~trace_file();


      void print(const string & str);
      void print(double dRate);


   };


} // namespace a_spa




