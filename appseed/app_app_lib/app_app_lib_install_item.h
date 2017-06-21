#pragma once


namespace a_spa
{


   struct install
   {
      LONG     m_lTotal;
      LONG     m_lProcessing;
      LONG     m_lOk;
      LONG     m_lBad;


      install() { ZEROP(this); }

   };



   class SPALIB_API install_item :
      virtual public object
   {
   public:


      simple_app *            m_papp;
      string                  m_strUrlPrefix;
      string                  m_strPath;
      string                  m_strFile;
      install *               m_pinstall;
      IDTHREAD                m_dwThreadId;
      string                  m_strMd5;
      string                  m_strPlatform;
      e_item_type             m_eitemtype;


      install_item(simple_app * paspa, string strFile, string strPlatform, install * pinstall);
      install_item(simple_app * paspa, string strUrlPrefix, string strPath, string strFile, install * pinstall, string strMd5, string strPlatform);


      static UINT c_cdecl proc(LPVOID lp);

      void run();

      void progress(double dRate = 1.0);

      void op_spa();

      void op_set();

   };


} // namespace a_spa



