#include "framework.h"


int_bool file_exists_raw(const char * path1)
{

   uint32_t dwFileAttributes = GetFileAttributesA(path1);

   if (dwFileAttributes == INVALID_FILE_ATTRIBUTES || (dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0)
      return FALSE;

   return TRUE;

}

void file_put_contents_raw(const char * path, const char * psz)
{

   FILE * f = fopen(path, "wb");

   if (f == NULL)
      return;

   ::count iSize = strlen(psz);

   ::count iRead = fwrite(psz, 1, iSize, f);

   fclose(f);

}

void file_add_contents_raw(const char * path, const char * psz)
{

   FILE * f = fopen(path, "ab");

   if (f == NULL)
      return;

   ::count iSize = strlen(psz);

   ::count iRead = fwrite(psz, 1, iSize, f);

   fclose(f);

}

void file_beg_contents_raw(const char * path, const char * psz)
{

   FILE * f = fopen(path, "rb+");

   ::count iLen = strlen(psz);

   fseek(f, iLen, SEEK_END);

   long  iEnd = ftell(f);

   int iSize = 1024 * 1024;
   char * buf = (char *)malloc(iSize);
   int iRemain = iEnd - iLen;
   while (iRemain > 0)
   {
      fseek(f, iEnd - iRemain - iLen, SEEK_SET);
      fread(buf, 1, MIN(iRemain, iSize), f);
      fseek(f, iEnd - iRemain, SEEK_SET);
      fwrite(buf, 1, MIN(iRemain, iSize), f);
      iRemain -= MIN(iRemain, iSize);
   }
   free(buf);
   fseek(f, 0, SEEK_SET);

   ::count iRead = fwrite(psz, 1, iLen, f);

   fclose(f);

}

uint64_t file_length_raw(const char * path)
{

   struct _stat st;
   ::_stat(path, &st);
   return st.st_size;

}
