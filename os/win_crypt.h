#pragma once


namespace win
{


   class CLASS_DECL_win crypt :
      virtual public ::ca4::crypt
   {
   public:


      crypt(::ca::application * papp);
      virtual ~crypt();


      virtual bool encrypt(primitive::memory & storageEncrypt, const primitive::memory & storageDecrypt, const char * pszSalt);
      virtual bool decrypt(primitive::memory & storageDecrypt, const primitive::memory & storageEncrypt, const char * pszSalt);


   }; 


} // namespace ca4



