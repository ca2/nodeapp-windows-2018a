#include "framework.h"

namespace app_app
{


   trace_file::trace_file(::aura::application * papp, const string & strLabel) :
      ::object(papp),
      m_mutex(papp, false, "Global\\ca2-app-app-install-" + strLabel),
      m_sl(&m_mutex),
      m_strLabel(strLabel)
   {

      dir::mk(dir::element().c_str());

      ::file::path path = dir::element() / ("install-" + strLabel + ".log");

      m_pfile = Application.file().get_file(path, ::file::type_binary | ::file::mode_write | ::file::mode_create);

      m_pfile->seek_to_end();

   }

   trace_file::~trace_file()
   {

   }


   void trace_file::print(const string & str)
   {

      if (m_pfile.is_set() && str.length() > 0)
      {

         m_pfile->seek_to_end();

         m_pfile->write(str.c_str(), str.length());

         m_pfile->flush();

      }

   }


   void trace_file::print(double dRate)
   {
      if (dRate < 0.0)
         dRate = 0.0;
      if (dRate > 1.0)
         dRate = 1.0;
      dRate = dRate * 1000.0 * 1000.0 * 1000.0;
      //int32_t i = ftol(dRate);
      int i = (int)dRate;
      string str;
      str = "|||";
      char sz[20];
      str += ::_itoa(i, sz, 10);
      str += "\r\n";
      print(str);
   }


} // namespace app_app

