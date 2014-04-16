#pragma once


#include "nodeapp/appseed/app_core/app_core.h"


namespace base
{

   class teste
   {
   public:
      teste()
      {
         output_debug_string("base::teste constructor\r\n");
      }
      ~teste()
      {
         output_debug_string("base::teste destructor\r\n");
      }

   };


   class teste2 : public teste
   {
   public:
      teste2()
      {
         output_debug_string("base::teste2 constructor\r\n");
      }
      ~teste2()
      {
         output_debug_string("base::teste2 destructor\r\n");
      }
   };

   class teste3 : public teste2
   {
   public:
      teste3()
      {
         output_debug_string("base::teste3 constructor\r\n");
      }
      virtual ~teste3()
      {
         output_debug_string("base::teste3 destructor\r\n");
      }
   };

   class teste4 : public teste3
   {
   public:
      teste4()
      {
         output_debug_string("base::teste4 constructor\r\n");
      }
      virtual ~teste4()
      {
         output_debug_string("base::teste4 destructor\r\n");
      }
   };

}



BEGIN_EXTERN_C

int32_t WINAPI _tWinMain(HINSTANCE hinstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int32_t nCmdShow)
{

   {
      output_debug_string("----1\r\n");
      output_debug_string("instanciando teste\r\n");
      ::base::teste teste;
   }
   {
      output_debug_string("----2\r\n");
      output_debug_string("instanciando teste3\r\n");
      ::base::teste2 teste3;
   }
   {
      output_debug_string("----3\r\n");
      output_debug_string("instanciando teste3\r\n");
      ::base::teste3 teste;
   }
   {
      output_debug_string("----4\r\n");
      output_debug_string("instanciando teste4\r\n");
      ::base::teste4 teste;
   }


   string str = ::str::international::unicode_to_utf8(::GetCommandLineW());

   return app_core_main(hinstance, hPrevInstance, (char *) (const char *) str, nCmdShow);
   
}

END_EXTERN_C