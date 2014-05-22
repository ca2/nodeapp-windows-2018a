#include "framework.h"


CLASS_DECL_BASE int32_t __win_main(sp(::base::system) psystem,::windows::main_init_data * pmaininitdata);


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


   class teste2: public teste
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

   class teste3: public teste2
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

   class teste4: public teste3
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



extern "C" int32_t app_core_main(HINSTANCE hinstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int32_t nCmdShow)
{

   if(!defer_core_init())
      return -1;

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



   UNREFERENCED_PARAMETER(lpCmdLine);

   sp(::plane::system) psystem = canew(::plane::system);

   ASSERT(hPrevInstance == NULL);

   ::windows::main_init_data * pmaininitdata = new ::windows::main_init_data;


   pmaininitdata->m_hInstance = hinstance;
   pmaininitdata->m_hPrevInstance = hPrevInstance;
   pmaininitdata->m_vssCommandLine = ::str::international::unicode_to_utf8(::GetCommandLineW());
   pmaininitdata->m_nCmdShow = nCmdShow;


   int32_t nReturnCode = __win_main(psystem, pmaininitdata);


   try
   {

      delete psystem;

   }
   catch (...)
   {

   }

   psystem = NULL;


   defer_core_term();

   return nReturnCode;

}




