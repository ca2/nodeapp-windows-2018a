#include "axis/axis/axis/axis.h"
#include "aura/aura/os/windows/windows_app.inl"
#include <stdio.h>
#include <psapi.h>
#include <tlhelp32.h>




#define IDI_CCVOTAGUS_CA2_SPA			107

string path_dir_name_for_relative(const char * pszPath);
//string solve_relative_compressions(const char * pszAbsolute);
//string path_defer_solve_relative_name(const char * pszRelative, const char * pszAbsolute);


// These next two structs represent how the icon information is stored
// in an ICO file.
struct ICON_ITEM
{
   BYTE	bWidth;               // Width of the image
   BYTE	bHeight;              // Height of the image (times 2)
   BYTE	bColorCount;          // Number of colors in image (0 if >=8bpp)
   BYTE	bReserved;            // Reserved
   WORD	wPlanes;              // Color Planes
   WORD	wBitCount;            // Bits per pixel
   DWORD	dwBytesInRes;         // how many bytes in this resource?
   DWORD	dwImageOffset;        // where in the file is this image
};

struct ICON_HEADER
{
   WORD			idReserved;   // Reserved
   WORD			idType;       // resource type (1 for icons)
   WORD			idCount;      // how many images?
};


void dprint(const char * psz)
{



   //string str;

   //str.Format("app._ : %s : %s\n",__argv[2],psz);

   //printf("%s", str);



}


struct MEM_ICON_ITEM
{
   BYTE	bWidth;               // Width of the image
   BYTE	bHeight;              // Height of the image (times 2)
   BYTE	bColorCount;          // Number of colors in image (0 if >=8bpp)
   BYTE	bReserved;            // Reserved
   WORD	wPlanes;              // Color Planes
   WORD	wBitCount;            // Bits per pixel
   DWORD	dwBytesInRes;         // how many bytes in this resource?
   WORD	nID;                  // the ID

};

struct MEM_ICON_HEADER
{
   WORD			idReserved;   // Reserved
   WORD			idType;       // resource type (1 for icons)
   WORD			idCount;      // how many images?
};


void copy(MEM_ICON_ITEM * dst, ICON_ITEM * pitem)
{
   dst->bWidth = pitem->bWidth;
   dst->bHeight = pitem->bHeight;
   dst->bColorCount = pitem->bColorCount;
   dst->bReserved = pitem->bReserved;
   dst->wPlanes = pitem->wPlanes;
   dst->wBitCount = pitem->wBitCount;
   dst->dwBytesInRes = pitem->dwBytesInRes;
}




class appfy :
   public ::axis::app
{
public:


   enum e_message
   {
      message_none,
      message_ok,
      message_failed,
      message_unknown,
   };


   e_message                  m_emessage;
   HANDLE                     m_hmutexSpabootInstall;
   ::aura::ipc::rx       m_rxchannel;

   char *                     m_modpath;
   char *                     m_pszDllEnds;
   uint32_t *                    m_dwaProcess;
   int32_t                        m_iSizeProcess;
   HMODULE *                  m_hmodulea;
   int32_t                        m_iSizeModule;
   bool                       m_bInstallerInstalling;

   appfy();
   virtual ~appfy();


   virtual int32_t run();

   virtual bool end();

};

//CLASS_DECL_AURA ::exception::engine * g_ee;
//void init_ee(::exception::engine * ee);
// if MSVC CRT is used
//extern "C" int32_t WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int32_t nCmdShow)
//{
//
//   if (!defer_aura_init())
//   {
//
//      return -1;
//
//   }
//
//
//   appfy * psystem = new appfy;
//
//   int iReturnCode = ::app_main(psystem, hInstance, hPrevInstance, lpCmdLine, nCmdShow);
//
//   defer_aura_term();
//
//   return iReturnCode;
//
//
//
//}

// if MSVC CRT is stripped
/*extern "C" int32_t WinMainCRTStartup() \
{

ExitProcess(app::s_main < appfy > ());

}*/


appfy::appfy() :
   ::object(this),
   m_rxchannel(this)
{

   {

      stringa stra;

      stra.add("C:\\a\\b\\c\\..\\..\\..\\d\\");
      stra.add("C:\\");
      stra.add("C:\\.");
      stra.add("C:\\.\\");
      stra.add("C:\\..");
      stra.add("C:\\..\\");
      stra.add("C:\\a\\");
      stra.add("C:\\a\\.");
      stra.add("C:\\a\\.\\");
      stra.add("C:\\a\\..\\");
      stra.add("C:\\a\\.\\b");
      stra.add("C:\\a\\.\\b\\");
      stra.add("C:\\a\\..\\b");
      stra.add("C:\\a\\..\\b\\");

      for (auto & str : stra)
      {

         output_debug_string("Converting: \"" + str + "\"\n");

         ::file::path path = solve_relative_compressions(str);

         output_debug_string("into: -> \"" + string(path.c_str()) + "\"\n");

      }

   }
   ////////////////////////////////////////////////////////////
   // configuration encryption system : with C:\\" hardware :-)
   // short login               short password  access configuration
   // |                         |               |
   // -----------------------   --       --------
   //                       |    |       |
   if (file_exists_dup(::dir::system() / "config\\appfy\\appfy_beg_debug_box.txt"))
   {
      debug_box("app_app_admin", "app", 0);
   }

   m_hinstance = ::GetModuleHandleA(NULL);
   m_hmutexSpabootInstall = NULL;
   e_message m_emessage = message_none;
   m_modpath = NULL;
   m_pszDllEnds = NULL;
   m_dwaProcess = NULL;
   m_iSizeProcess = 0;
   m_hmodulea = NULL;
   m_iSizeModule = 0;
   m_bInstallerInstalling = false;

   construct(NULL);

   m_pauraapp = this;

   m_bLicense = false;


}

appfy::~appfy()
{
}


int32_t appfy::run()
{

   string strSrc;
   {

      //new string();

      //memory_alloc(100);

      //if (!pre_run())
      // return -1;

      //MessageBox(NULL,"xrun1","y",MB_OK);

      m_modpath = (char *)memory_alloc(MAX_PATH * 8);
      m_pszDllEnds = (char *)memory_alloc(MAX_PATH * 8);
      m_iSizeProcess = 1024;
      m_dwaProcess = (uint32_t *)memory_alloc(m_iSizeProcess);
      m_iSizeModule = 1024;
      m_hmodulea = (HMODULE *)memory_alloc(m_iSizeModule);

      //System.oprop("do_not_initialize_user_presence") = true;


      if (file_exists_dup(::dir::system() / "config\\plugin\\appfy_beg_debug_box.txt"))
      {

         debug_box("appfy run", "appfy run", MB_OK);

      }


      if (__argc < 4)
      {

         ::simple_message_box(NULL, "Incorrect Number of Arguments passed to appfy. Expected 3 or 4; passed " + ::str::from(__argc - 1), "", 0);

         return -2;

      }

      dprint("Starting!!");

      strSrc = solve_relative_compressions(__argv[1]);

      string strApp = __argv[2];

      string strDst = solve_relative_compressions(__argv[3]);

      string strBuild = __argv[2];

      if (strBuild.is_empty())
      {

         //      strBuild = ca2_get_build("stage");

      }


      string strRoot;

      string strDomain;

      index iFind = strApp.find("/");

      if (iFind >= 0)
      {
         strRoot = strApp.Left(iFind);
         strDomain = strApp.Mid(iFind + 1);
      }
      else
      {
         string strParse(strApp);
         if (::str::begins_eat_ci(strParse, "app_core_"))
         {
            strRoot = "app-core";
            strDomain = strParse;
         }
         else if (::str::begins_eat_ci(strParse, "app_veriwell_"))
         {
            strRoot = "app-veriwell";
            strDomain = strParse;
         }
         else if (::str::begins_eat_ci(strParse, "app_cidadedecuritiba_"))
         {
            strRoot = "app-cidadedecuritiba";
            strDomain = strParse;
         }
         else if (::str::begins_eat_ci(strParse, "app_gtech_"))
         {
            strRoot = "app-gtech";
            strDomain = strParse;
         }
         else if (::str::begins_eat_ci(strParse, "app_frontpage_"))
         {
            strRoot = "app-frontpage";
            strDomain = strParse;
         }
         else if (::str::begins_eat_ci(strParse, "app_sysutils_"))
         {
            strRoot = "app-sysutils";
            strDomain = strParse;
         }
         else if (::str::begins_eat_ci(strParse, "design_"))
         {
            strRoot = "design";
            strDomain = strParse;
         }
         else if (::str::begins_eat_ci(strParse, "game_"))
         {
            strRoot = "game";
            strDomain = strParse;
         }
         else if (::str::begins_eat_ci(strParse, "rootkiller_"))
         {
            strRoot = "rootkiller";
            strDomain = strParse;
         }
         else if (::str::begins_eat_ci(strParse, "app_"))
         {
            int iFind = strParse.find("_");
            if (iFind < 0)
            {
               strRoot = strApp;
               strDomain = "main";
            }
            else
            {
               strRoot = "app-" + strParse.Left(iFind);
               strDomain = strParse.Mid(iFind + 1);
            }
         }
         else if (::str::begins_eat_ci(strParse, "nodeapp_"))
         {
            strRoot = "nodeapp";
            strDomain = strParse;
         }
         else
         {
            strRoot = strApp;
            strDomain = "";
         }
      }

      dprint("a bit of parsing!!");

      Application.file().copy(strDst, strSrc, false);

      dprint("main copy should be done!!");

      string strIcon;

      string strCandidate = Application.dir().matter("main/icon.ico", false, strRoot, strDomain);

      if (Application.file().exists(strCandidate))
         strIcon = strCandidate;

      dprint("does main frame icon exist 1!!");

      if (strIcon.is_empty())
      {

         strCandidate = Application.dir().matter("main/icon.ico", false);

         if (Application.file().exists(strCandidate))
            strIcon = strCandidate;

         dprint("does main frame icon exist 2!!");

      }


      HANDLE hupdate = BeginUpdateResource(strDst, FALSE);

      dprint("BeginUpdateResource");

      dprint(strDst);

      if (hupdate != NULL)
      {

         dprint("hupdate ok");

      }
      else
      {

         dprint("hupdate false");

         return -3;

      }

      if (Application.file().exists(strIcon))
      {



         memory memory;
         ::memory memoryItem;
         ::memory_file fileHd(get_app());

         Application.file().as_memory(strIcon, memory);

         dprint("icon as memory");

         ICON_HEADER * phd = (ICON_HEADER *)memory.get_data();
         int iTotalHd = sizeof(ICON_HEADER) + (sizeof(ICON_ITEM)* phd->idCount);


         ICON_ITEM * itema = (ICON_ITEM *)(memory.get_data() + 6);

         MEM_ICON_HEADER hd;

         hd.idReserved = phd->idReserved;
         hd.idType = phd->idType;
         hd.idCount = phd->idCount;

         fileHd.write(&hd, sizeof(hd));

         int iSize = iTotalHd;
         for (int i = 0; i < phd->idCount; i++)
         {
            ICON_ITEM * pitem = &itema[i];
            MEM_ICON_ITEM item;
            ::copy(&item, pitem);
            item.nID = IDI_CCVOTAGUS_CA2_SPA + i + 1;
            UpdateResource(hupdate, RT_ICON, MAKEINTRESOURCE(IDI_CCVOTAGUS_CA2_SPA + i + 1), MAKELANGID(LANG_NEUTRAL, SUBLANG_NEUTRAL), memory.get_data() + pitem->dwImageOffset, pitem->dwBytesInRes);
            fileHd.write(&item, 14);
         }

         UpdateResource(hupdate, RT_GROUP_ICON, "MAINICON", MAKELANGID(LANG_NEUTRAL, SUBLANG_NEUTRAL), fileHd.get_data(), fileHd.get_size());

         dprint("icon should be updated");

      }

      string strCoreApp;

      if (strRoot.has_char())
      {
         if (strDomain.has_char())
         {
            strCoreApp = strRoot + "/" + strDomain;
         }
         else
         {
            strCoreApp = strRoot;
         }
      }
      else
      {
         if (strDomain.has_char())
         {
            strCoreApp = strDomain;
         }
      }
      if (strCoreApp.has_char())
      {
         UpdateResource(hupdate, "APPID", MAKEINTRESOURCE(1), MAKELANGID(LANG_NEUTRAL, SUBLANG_NEUTRAL), (LPVOID)(LPCTSTR)strCoreApp, strCoreApp.get_length());
         dprint("APPID should be updated");
      }

      EndUpdateResource(hupdate, FALSE);

      printf("%s", ("Program \"" + strDst + "\" successfully generated!!").c_str());

   }

   do_events();

   return 0;

}



bool appfy::end()
{

   app::end();

   memory_free(m_hmodulea);
   memory_free(m_dwaProcess);
   memory_free(m_pszDllEnds);
   memory_free(m_modpath);

   return true;

}









#define PATH_SEP1 "\\"
#define PATH_SEP2 "/"


string path_url_dir_name_for_relative(const char * pszPath)
{
   string strDir(pszPath);

   if (str::ends(strDir, PATH_SEP1))
      return strDir;

   if (str::ends(strDir, PATH_SEP2))
      return strDir;

   strsize iFind1 = strDir.reverse_find(PATH_SEP1);
   strsize iFind2 = strDir.reverse_find(PATH_SEP2);
   strsize iFind = MAX(iFind1, iFind2);

   if (iFind < 0)
      return PATH_SEP1;

   return strDir.substr(0, iFind + 1);

}



#undef new
void init_exception_engine(class ::exception::engine * pexceptionengine)
{
   
   new(pexceptionengine) class ::exception::engine(NULL);

}







::aura::application * get_acid_app(::aura::application * papp)
{

   return new ::appfy();

}