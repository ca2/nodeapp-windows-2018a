#include "StdAfx.h"



void str_trim_left(string & str)
{
   while(str.length() > 0 &&
      (str[0] == '\n' || str[0] == '\t'
      || str[0] == '\r' || str[0] == ' '))
   {
      str = str.substr(1);
   }
}

void str_trim_right(string & str)
{
   while(str.length() > 0 &&
      (str[str.length() - 1] == '\n' || str[str.length() - 1] == '\t'
      || str[str.length() - 1] == '\r' || str[str.length() - 1] == ' '))
   {
      str = str.substr(0, str.length() - 1);
   }
}

void str_trim(string & str)
{
   str_trim_left(str);
   str_trim_right(str);
}



void __cdecl wparse_cmdline (
    WCHAR *cmdstart,
    WCHAR **argv,
    WCHAR *args,
    int *numargs,
    int *numchars
    )
{
        WCHAR *p;
        WCHAR c;
        int inquote;                    /* 1 = inside quotes */
        int copychar;                   /* 1 = copy char to *args */
        unsigned numslash;              /* num of backslashes seen */

        *numchars = 0;
        *numargs = 1;                   /* the program name at least */

        /* first scan the program name, copy it, and count the bytes */
        p = cmdstart;
        if (argv)
            *argv++ = args;

#ifdef WILDCARD
        /* To handle later wild card expansion, we prefix each entry by
        it's first character before quote handling.  This is done
        so _[w]cwild() knows whether to expand an entry or not. */
        if (args)
            *args++ = *p;
        ++*numchars;

#endif  /* WILDCARD */

        /* A quoted program name is handled here. The handling is much
           simpler than for other arguments. Basically, whatever lies
           between the leading double-quote and next one, or a terminal null
           character is simply accepted. Fancier handling is not required
           because the program name must be a legal NTFS/HPFS file name.
           Note that the double-quote characters are not copied, nor do they
           contribute to numchars. */
        if ( *p == DQUOTEWCHAR ) {
            /* scan from just past the first double-quote through the next
               double-quote, or up to a null, whichever comes first */
            while ( (*(++p) != DQUOTEWCHAR) && (*p != NULWCHAR) ) {

/*#ifdef _MBCS
                if (_ismbblead(*p)) {
                    ++*numchars;
                    if ( args )
                        *args++ = *p++;
                }
#endif  /* _MBCS */
                ++*numchars;
                if ( args )
                    *args++ = *p;
            }
            /* append the terminating null */
            ++*numchars;
            if ( args )
                *args++ = NULWCHAR;

            /* if we stopped on a double-quote (usual case), skip over it */
            if ( *p == DQUOTEWCHAR )
                p++;
        }
        else {
            /* Not a quoted program name */
            do {
                ++*numchars;
                if (args)
                    *args++ = *p;

                c = (WCHAR) *p++;
/*#ifdef _MBCS
                if (_ismbblead(c)) {
                    ++*numchars;
                    if (args)
                        *args++ = *p;   /* copy 2nd byte too */
    //                p++;  /* skip over trail byte */
  //              }
//#endif  /* _MBCS */

            } while ( c != SPACEWCHAR && c != NULWCHAR && c != TABWCHAR );

            if ( c == NULWCHAR ) {
                p--;
            } else {
                if (args)
                    *(args-1) = NULWCHAR;
            }
        }

        inquote = 0;

        /* loop on each argument */
        for(;;) {

            if ( *p ) {
                while (*p == SPACEWCHAR || *p == TABWCHAR)
                    ++p;
            }

            if (*p == NULWCHAR)
                break;              /* end of args */

            /* scan an argument */
            if (argv)
                *argv++ = args;     /* store ptr to arg */
            ++*numargs;

#ifdef WILDCARD
        /* To handle later wild card expansion, we prefix each entry by
        it's first character before quote handling.  This is done
        so _[w]cwild() knows whether to expand an entry or not. */
        if (args)
            *args++ = *p;
        ++*numchars;

#endif  /* WILDCARD */

        /* loop through scanning one argument */
        for (;;) {
            copychar = 1;
            /* Rules: 2N backslashes + " ==> N backslashes and begin/end quote
               2N+1 backslashes + " ==> N backslashes + literal "
               N backslashes ==> N backslashes */
            numslash = 0;
            while (*p == SLASHWCHAR) {
                /* count number of backslashes for use below */
                ++p;
                ++numslash;
            }
            if (*p == DQUOTEWCHAR) {
                /* if 2N backslashes before, start/end quote, otherwise
                    copy literally */
                if (numslash % 2 == 0) {
                    if (inquote) {
                        if (p[1] == DQUOTEWCHAR)
                            p++;    /* Double quote inside quoted string */
                        else        /* skip first quote char and copy second */
                            copychar = 0;
                    } else
                        copychar = 0;       /* don't copy quote */

                    inquote = !inquote;
                }
                numslash /= 2;          /* divide numslash by two */
            }

            /* copy slashes */
            while (numslash--) {
                if (args)
                    *args++ = SLASHWCHAR;
                ++*numchars;
            }

            /* if at end of arg, break loop */
            if (*p == NULWCHAR || (!inquote && (*p == SPACEWCHAR || *p == TABWCHAR)))
                break;

            /* copy character into argument */
/*#ifdef _MBCS
            if (copychar) {
                if (args) {
                    if (_ismbblead(*p)) {
                        *args++ = *p++;
                        ++*numchars;
                    }
                    *args++ = *p;
                } else {
                    if (_ismbblead(*p)) {
                        ++p;
                        ++*numchars;
                    }
                }
                ++*numchars;
            }
            ++p;
#else  /* _MBCS */
            if (copychar) {
                if (args)
                    *args++ = *p;
                ++*numchars;
            }
            ++p;
//#endif  /* _MBCS */
            }

            /* null-terminate the argument */

            if (args)
                *args++ = NULWCHAR;          /* terminate string */
            ++*numchars;
        }

        /* We put one last argument in -- a null ptr */
        if (argv)
            *argv++ = NULL;
        ++*numargs;
}



wstring u16(const char * psz)
{
   int iCount = MultiByteToWideChar(
		CP_UTF8,
		0,
		psz,
		-1,
		NULL,
		0);
   WCHAR * pwz = new WCHAR[iCount + 1];
   MultiByteToWideChar(
		CP_UTF8,
		0,
		psz,
		-1,
		pwz,
		iCount);
   wstring wstr(pwz);
   delete pwz;
   return wstr;
}


string u8(const wchar_t * pwz)
{
   int iCount = WideCharToMultiByte(
		CP_UTF8,
		0,
		pwz,
		-1,
		NULL,
		0,
      NULL,
      NULL);
   CHAR * psz = new CHAR[iCount + 1];
   WideCharToMultiByte(
		CP_UTF8,
		0,
		pwz,
		-1,
		psz,
		iCount,
      NULL,
      NULL);
   string str(psz);
   delete psz;
   return str;
}


bool str_ends(const char * psz, const char * pszSuffix)
{
   string str(psz == NULL ? "" : psz);
   string strSuffix(pszSuffix == NULL ? "" : pszSuffix);
   if(str.length() < strSuffix.length())
      return false;
   if(str.substr(str.length() - strSuffix.length(), strSuffix.length()) != strSuffix)
      return false;
   return true;
}

bool str_ends_ci(const char * psz, const char * pszSuffix)
{
   string str(psz == NULL ? "" : psz);
   string strSuffix(pszSuffix == NULL ? "" : pszSuffix);
   if(str.length() < strSuffix.length())
      return false;
   if(_stricmp(str.substr(str.length() - strSuffix.length(), strSuffix.length()).c_str(), strSuffix.c_str()) != 0)
      return false;
   return true;
}

bool str_begins_ci(const char * psz, const char * pszPrefix)
{
   string str(psz == NULL ? "" : psz);
   string strPrefix(pszPrefix == NULL ? "" : pszPrefix);
   if(str.length() < strPrefix.length())
      return false;
   if(_stricmp(str.substr(0, strPrefix.length()).c_str(), strPrefix.c_str()) != 0)
      return false;
   return true;
}


string read_resource_as_string(
   HINSTANCE hinst,
   UINT nID, 
   LPCTSTR lpcszType)
{
   string str;
	HRSRC hrsrc = ::FindResource(
		hinst,
		MAKEINTRESOURCE(nID), 
		lpcszType);
	if(hrsrc == NULL)
		return str;
    HGLOBAL hres = ::LoadResource(hinst, hrsrc);
	if(hres == NULL)
		return str;
    DWORD dwResSize = ::SizeofResource(hinst, hrsrc);

	if(hres != NULL)
    {
		UINT FAR* lpnRes = (UINT FAR*)::LockResource(hres);
      string strRes((char *) lpnRes, dwResSize);
      str = strRes;
		#ifndef WIN32 //Unlock Resource is obsolete in the Win32 API
			::UnlockResource(hres);
		#endif
            ::FreeResource(hres);
	}
	return str;

}


BSTR BSTRFromCStr(UINT codePage, LPCSTR s)
{
    int wideLen = MultiByteToWideChar(codePage, 0, s, -1, NULL, 0);
    if( wideLen > 0 )
    {
        WCHAR* wideStr = (WCHAR*)CoTaskMemAlloc(wideLen*sizeof(WCHAR));
        if( NULL != wideStr )
        {
            BSTR bstr;

            ZeroMemory(wideStr, wideLen*sizeof(WCHAR));
            MultiByteToWideChar(codePage, 0, s, -1, wideStr, wideLen);
            bstr = SysAllocStringLen(wideStr, wideLen-1);
            CoTaskMemFree(wideStr);

            return bstr;
        }
    }
    return NULL;
};

