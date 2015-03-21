#include "StdAfx.h"


bool file::exists(const char * path1)
{
   DWORD dwFileAttributes = ::GetFileAttributes(path1);
   if(dwFileAttributes != INVALID_FILE_ATTRIBUTES &&
      (dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0)
      return true;
   else
      return false;
}


std::string file::get_contents(const char * path)
{
   FILE * f = fopen(path, "rb");
   if(!f)
      return "";
   char buf[257];
   size_t read;
   std::string str;
   while((read = fread(buf, 1, sizeof(buf) - 1, f)) > 0)
   {
      buf[read] = '\0';
      str += buf;
   }
   fclose(f);
   return str;
}

void file::put_contents(const char * path, const char * contents)
{
   FILE * f = fopen(path, "wb");
   if(!f)
      return;
   fwrite(contents, 1, strlen(contents), f);
   fclose(f);
}

std::string file::md5(const char * psz)
{
   static unsigned char * s_bufMd5 = NULL;
   FILE * f = fopen(psz, "rb");
   if(!f)
      return "";
   int iBufSize = 1024 * 1024;
   if(s_bufMd5 == NULL)
   {
      s_bufMd5 = new unsigned char[iBufSize];
   }
   unsigned char * buf = s_bufMd5;
   class md5 alg;
   UINT_PTR iRead;
   while((iRead = fread(buf, 1, iBufSize, f)) > 0)
   {
      alg.update(buf, iRead);
   }
   alg.finalize();
   fclose(f);
   std::string str;
   char chbuf[32];
   for(int i = 0; i < 16; i++)
   {
      sprintf(chbuf, "%02x", alg.digest()[i]);
      str += chbuf;
   }
   return str;
}


int file::length(const char * path)
{
   FILE * f = fopen(path, "rb");
   if(!f)
      return -1;
   if(fseek(f, 0, SEEK_END))
   {
      fclose(f);
      return -1;
   }
   int iRet = ftell(f);
   fclose(f);
   return  iRet;
}

std::string file::title(const char * path)
{
   std::string str(path);
   size_t iPos;
   size_t iPos1 = str.rfind('\\');
   size_t iPos2 = str.rfind('/');
   if(iPos1 != -1 && iPos2 != -1)
   {
      if(iPos1 > iPos2)
      {
         iPos = iPos1 + 1;
      }
      else
      {
         iPos = iPos2 + 1;
      }
   }
   else if(iPos1 != -1)
   {
      iPos = iPos1 + 1;
   }
   else if(iPos2 != -1)
   {
      iPos = iPos2 + 1;
   }
   else
   {
      iPos = 0;
   }
   return str.substr(iPos);
}


std::string file::name(const char * path)
{
   std::string str = title(path);
   size_t iPos = str.find('.');
   if(iPos != -1)
   {
      return str.substr(0, iPos);
   }
   else
   {
      return str;
   }
}

std::string file::module_path()
{
   char path[MAX_PATH * 4];
   if(!GetModuleFileName(NULL,
      path,
      sizeof(path)))
      return "";
   else 
   {
      return path;
   }
}



bool read_resource_as_file(
   const char * pszFile,
   HINSTANCE hinst,
   UINT nID, 
   LPCTSTR lpcszType)
{
	HRSRC hrsrc = ::FindResource(
		hinst,
		MAKEINTRESOURCE(nID), 
		lpcszType);
	if(hrsrc == NULL)
		return false;
    HGLOBAL hres = ::LoadResource(hinst, hrsrc);
	if(hres == NULL)
		return false;
    DWORD dwResSize = ::SizeofResource(hinst, hrsrc);

    if(hres != NULL)
    {
       bool bOk = false;
		UINT FAR* lpnRes = (UINT FAR*)::LockResource(hres);
      FILE * f  = fopen(pszFile, "wb");
      if(f != NULL)
      {
         fwrite(lpnRes, 1, dwResSize, f);
         fclose(f);
         bOk = true;
      }
		#ifndef WIN32 //Unlock Resource is obsolete in the Win32 API
			::UnlockResource(hres);
		#endif
            ::FreeResource(hres);
      return bOk;
	}
	return false;

}


std::string get_temp_file_name(const char * pszName, const char * pszExtension)
{
   char lpPathBuffer[MAX_PATH * 16];
 // Get the temp path.
   DWORD dwRetVal = GetTempPath(sizeof(lpPathBuffer),     // length of the buffer
                        lpPathBuffer); // buffer for path 
   if (dwRetVal > sizeof(lpPathBuffer) || (dwRetVal == 0))
   {
      printf ("GetTempPath failed (%d)\n", GetLastError());
      return "";
   }
   std::string str;
   char buf[30];
   size_t iLen= strlen(lpPathBuffer);
   if(!(lpPathBuffer[iLen - 1] == '/'
      || lpPathBuffer[iLen - 1] == '\\'))
   {
      lpPathBuffer[iLen] = '\\';
      lpPathBuffer[iLen+1] = '\0';
   }
   for(int i = 0; i < 1000; i++)
   {
      sprintf(buf, "%d", i);
      str = lpPathBuffer;
      str += pszName;
      str += "-";
      str += buf;
      str += ".";
      str += pszExtension;
      if(file::exists(str.c_str()))
      {
         if(::DeleteFileA(str.c_str()))
            return str;
      }
      else
      {
         return str;
      }
   }
   return "";
}








const int g_iGzUncompressLen = 1024 * 1024;
char * g_pchGzUncompressBuffer = NULL;



int bzuncompress(LPCTSTR lpcszUncompressed, LPCTSTR lpcszGzFileCompressed)
{
   if(g_pchGzUncompressBuffer == NULL)
   {
       g_pchGzUncompressBuffer = new char[g_iGzUncompressLen];
   }
   BZFILE * file = BZ2_bzopen(lpcszGzFileCompressed, "rb");
   if (file == NULL)
   {
      fprintf(stderr, "bzopen error\n");
      return -2;
   }
   std::string strUn(lpcszUncompressed);
//   strUn += ".tmp";
   FILE * fileUn = fopen(strUn.c_str(), "wb+");
   if (fileUn == NULL)
   {
      BZ2_bzclose(file);
      int err;
      _get_errno(&err);
      fprintf(stderr, "fopen error\n %d", err);

      return -1;
   }
   int uncomprLen;
   while((uncomprLen = BZ2_bzread(file, g_pchGzUncompressBuffer, g_iGzUncompressLen)) > 0)
   {
      fwrite(g_pchGzUncompressBuffer, 1, uncomprLen, fileUn);
   }
   fclose(fileUn);
   BZ2_bzclose(file);
//   ::CopyFile(strUn.c_str(), lpcszUncompressed, FALSE);
//   ::DeleteFile(strUn.c_str());
   return 0;
}




/*   void file::dtf(const char * pszFile, const char * pszDir)
   {
      stringa stringa;
      stringa straRelative;
      Application.dir().rls(pszDir, &stringa, NULL, &straRelative);
      ex1::filesp spfile(get_app());
      if(!spfile->Open(pszFile, ::ex1::file::modeCreate | ::ex1::file::modeWrite  | ::ex1::file::typeBinary))
         throw "failed";
      string strVersion;
      strVersion = "fileset v1";
         MD5_CTX ctx;
      write_ex1_string(spfile, NULL, strVersion);
         ex1::filesp file2(get_app());
         int iBufSize = 1024 * 1024;
         int uiRead;
      unsigned char * buf = (unsigned char *)  malloc(iBufSize);
      string strMd5 = "01234567012345670123456701234567";
      int iPos;
      for(int i = 0; i < stringa.get_size(); i++)
      {
         if(Application.dir().is(stringa[i]))
            continue;
         write_n_number(spfile, NULL, 1);
         iPos = spfile->GetPosition();
         write_ex1_string(spfile, NULL, strMd5);
         MD5_Init(&ctx);
         write_ex1_string(spfile, &ctx, straRelative[i]);
         if(!file2->Open(stringa[i], ::ex1::file::modeRead | ::ex1::file::typeBinary))
            throw "failed";
         write_n_number(spfile, &ctx, file2->get_length());
         while((uiRead = file2->Read(buf, iBufSize)) > 0)
         {
            spfile->Write(buf, uiRead);
            MD5_Update(&ctx, buf, uiRead);
         }
         spfile->Seek(iPos, ::ex1::file::SeekBegin);
         MD5_Final(buf,&ctx);
         strMd5.Empty();
         string strFormat;
         for(int i = 0; i < 16; i++)
         {
            strFormat.Format("%02x", buf[i]);
            strMd5 += strFormat;
         }
         write_ex1_string(spfile, NULL, strMd5);
         spfile->SeekToEnd();

      }
      write_n_number(spfile, NULL, 2);
   }*/

   bool file::ftd(const char * pszDir, const char * pszFile)
   {
      FILE * spfile = NULL;
      FILE * file2 = NULL;
      try
      {
         std::string strVersion;
         spfile = fopen(pszFile, "rb");
         if(!spfile)
            throw "failed";
         read_ex1_string(spfile, NULL, strVersion);
         int n;
         std::string strRelative;
         std::string strMd5;
         std::string strMd5New;
         int iBufSize = 1024 * 1024;
         unsigned char * buf = (unsigned char *)  malloc(iBufSize);
         int iLen;
         class md5 ctx;
         UINT_PTR uiRead;
         if(strVersion == "fileset v1")
         {
            while(true)
            {
               read_n_number(spfile, NULL, n);
               if(n == 2)
                  break;
               read_ex1_string(spfile, NULL, strMd5);
               ctx.initialize();
               read_ex1_string(spfile, &ctx, strRelative);
               std::string strPath = dir::path(pszDir, strRelative.c_str());
               dir::mk(dir::name(strPath.c_str()).c_str());
               file2 = fopen(strPath.c_str(), "wb");
               if(!file2)
                  throw "failed";
               read_n_number(spfile, &ctx, iLen);
               while(iLen > 0)
               {
                uiRead = fread(buf, 1, min(iBufSize, iLen ), spfile);
                if(uiRead == 0)
                   break;
                  fwrite(buf, 1, uiRead, file2);
                  ctx.update(buf, uiRead);
                  iLen -= uiRead;
               }
               fclose(file2);
               file2 = NULL;
               ctx.finalize();

               strMd5New.clear();
               std::string strFormat;
               strMd5New = ctx.to_string();
               if(strMd5 != strMd5New)
                  throw "failed";
            }
         }
         if(spfile != NULL)
            fclose(spfile);
         if(file2 != NULL)
            fclose(file2);
         return true;
      }
      catch(...)
      {
         if(spfile != NULL)
            fclose(spfile);
         if(file2 != NULL)
            fclose(file2);
         return false;
      }
   }


/*   void file::write_n_number(FILE * pfile, class md5  * pctx, int iNumber)
   {
      std::string str;
      str.Format("%dn", iNumber);
      pfile->Write((const char *) str, str.get_length());
      if(pctx != NULL)
      {
         MD5_Update(pctx, (const char *) str, str.get_length());
      }
   }
*/
   void file::read_n_number(FILE * pfile, class md5 * pctx, int & iNumber)
   {
      UINT_PTR uiRead;
      std::string str;
      char ch;
      while((uiRead = fread(&ch, 1, 1, pfile)) == 1)
      {
         if(ch >= '0' && ch <= '9')
            str += ch;
         else
            break;
         if(pctx != NULL)
         {
            pctx->update(&ch, 1);
         }
      }
      if(ch != 'n')
         throw "failed";
      if(pctx != NULL)
      {
         pctx->update(&ch, 1);
      }
      iNumber = atoi(str.c_str());
   }

  /* void file::write_ex1_string(ex1::file * pfile, MD5_CTX * pctx, string & str)
   {
      int iLen = str.get_length();
      write_n_number(pfile, pctx, iLen);
      pfile->Write((const char *) str, str.get_length());
      if(pctx != NULL)
      {
         MD5_Update(pctx, (const char *) str, str.get_length());
      }
   }*/

   void file::read_ex1_string(FILE * pfile, class md5 * pctx, std::string & str)
   {
      int iLen;
      read_n_number(pfile, pctx, iLen);
      LPSTR lpsz = (LPSTR) malloc(iLen + 1);
      fread(lpsz, 1, iLen, pfile);
      if(pctx != NULL)
      {
         pctx->update(lpsz, iLen);
      }
      lpsz[iLen] = '\0';
      str = lpsz;
      free(lpsz);
   }





   bool PrintModules(std::string & strImage, DWORD processID, const char * pszDll )
{
    HMODULE hMods[1024];
    HANDLE hProcess;
    DWORD cbNeeded;
    unsigned int i;
    char szImage[MAX_PATH * 8];

    // Print the process identifier.

    printf( "\nProcess ID: %u\n", processID );

    // Get a list of all the modules in this process.

    hProcess = OpenProcess( PROCESS_QUERY_INFORMATION |
                            PROCESS_VM_READ,
                            FALSE, processID );




    if (NULL == hProcess)
        return false;

      GetProcessImageFileName(
  hProcess,
  szImage,
  sizeof(szImage));
      strImage = szImage;

      bool bFound = false;

    if( EnumProcessModules(hProcess, hMods, sizeof(hMods), &cbNeeded))
    {
        for ( i = 0; i < (cbNeeded / sizeof(HMODULE)); i++ )
        {
            TCHAR szModName[MAX_PATH];

            // Get the full path to the module's file.

            if ( GetModuleFileNameEx( hProcess, hMods[i], szModName,
                                      sizeof(szModName) / sizeof(TCHAR)))
            {
              if(!_stricmp(szModName, pszDll))
              {
                 bFound = true;
                 break;
              }
            }
        }
    }

    CloseHandle( hProcess );
    return bFound;
}

void dll_processes(std::vector < DWORD > & dwa, stringa & straProcesses, const char * pszDll)
{
    // Get the list of process identifiers.

    DWORD aProcesses[1024 * 8], cbNeeded, cProcesses;
    unsigned int i;

    if ( !EnumProcesses( aProcesses, sizeof(aProcesses), &cbNeeded ) )
        return;

    // Calculate how many process identifiers were returned.

    cProcesses = cbNeeded / sizeof(DWORD);

    // Print the name of the modules for each process.

    std::string strImage;
    for ( i = 0; i < cProcesses; i++ )
    {
        if(PrintModules(strImage, aProcesses[i], pszDll ))
        {
           straProcesses.push_back(strImage);
           dwa.push_back(aProcesses[i]);
        }
    }
}

