#pragma once


namespace win
{


   class CLASS_DECL_win crypt :
      virtual public ::crypto::crypto
   {
   public:


      crypt(sp(base_application) papp);
      virtual ~crypt();


      virtual bool encrypt(primitive::memory & storageEncrypt, const primitive::memory & storageDecrypt, const char * pszSalt);
      virtual bool decrypt(primitive::memory & storageDecrypt, const primitive::memory & storageEncrypt, const char * pszSalt);


   }; 


} // namespace core



