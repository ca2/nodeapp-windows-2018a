#pragma once


namespace a_spa
{


   class SPALIB_API install_item :
      virtual public object
   {
   public:


      simple_app *            m_papp;
      string                  m_strUrlPrefix;
      string                  m_strPath;
      string                  m_strFile;
      LONG *                  m_plong;
      LONG *                  m_plongOk;
      LONG *                  m_plongBad;
      IDTHREAD                m_dwThreadId;
      string                  m_strMd5;
      string                  m_strPlatform;
      LONG                    m_lTotal;
      e_item_type             m_eitemtype;


      install_item(simple_app * paspa, string strFile, string strPlatform, LONG * plong, LONG lTotal, LONG * plongOk, LONG * plongBad);
      install_item(simple_app * paspa, string strUrlPrefix, string strPath, string strFile, LONG * plong, string strMd5, string strPlatform, LONG lTotal);


      static UINT c_cdecl proc(LPVOID lp);

      void run();

      void progress(double dRate = 1.0);

      void op_spa();

      void op_set();

   };


} // namespace a_spa



