

int bzuncompress(LPCTSTR lpcszUncompressed,LPCTSTR lpcszGzFileCompressed)
{
   const int iGzUncompressLen = 1024 * 1024;
   char * pchGzUncompressBuffer = NULL;
   if(pchGzUncompressBuffer == NULL)
   {
      pchGzUncompressBuffer = new char[iGzUncompressLen];
   }
   BZFILE * file = BZ2_bzopen(lpcszGzFileCompressed,"rb");
   if(file == NULL)
   {
      fprintf(stderr,"bzopen error\n");
      return -2;
   }
   string strUn(lpcszUncompressed);
   //   strUn += ".tmp";
   FILE * fileUn = fopen(strUn.c_str(),"wb+");
   if(fileUn == NULL)
   {
      BZ2_bzclose(file);
      int err;
      _get_errno(&err);
      fprintf(stderr,"fopen error\n %d",err);

      return -1;
   }
   int uncomprLen;
   while((uncomprLen = BZ2_bzread(file,pchGzUncompressBuffer,iGzUncompressLen)) > 0)
   {
      fwrite(pchGzUncompressBuffer,1,uncomprLen,fileUn);
   }
   fclose(fileUn);
   BZ2_bzclose(file);
   //   ::CopyFile(strUn.c_str(), lpcszUncompressed, FALSE);
   //   ::DeleteFile(strUn.c_str());
   return 0;
}
