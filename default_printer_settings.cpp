#include "winspool.h"

int SetDefaultPrinterOrientation(short dmOrientation)
{
   HANDLE hPrinter = NULL;
   DWORD dwNeeded = 0;
   PRINTER_INFO_2 *pi2 = NULL;
   DEVMODE *pDevMode = NULL;
   PRINTER_DEFAULTS pd;
   BOOL bFlag;
   LONG lFlag;
   LPTSTR pPrinterName = NULL;
   DWORD size;
   GetDefaultPrinter(NULL,&size);
   TCHAR* buffer = new TCHAR[size];
   if(GetDefaultPrinter(buffer,&size))
      pPrinterName = buffer;
   else
   {
      if(buffer != NULL)
         delete buffer;
      return 0;
   }
   // Open printer handle (on Windows NT, you need full-access because you
   // will eventually use SetPrinter)...
   ZeroMemory(&pd,sizeof(pd));
   pd.DesiredAccess = PRINTER_ALL_ACCESS;
   bFlag = OpenPrinter(pPrinterName,&hPrinter,&pd);
   if(!bFlag || (hPrinter == NULL))
   {
      if(buffer != NULL)
         delete buffer;
      return 0;
   }
   // The first GetPrinter tells you how big the buffer should be in 
   // order to hold all of PRINTER_INFO_2. Note that this should fail with 
   // ERROR_INSUFFICIENT_BUFFER. If GetPrinter fails for any other reason 
   // or dwNeeded isn't set for some reason, then there is a problem...
   SetLastError(0);
   bFlag = GetPrinter(hPrinter,2,0,0,&dwNeeded);
   if((!bFlag) && (GetLastError() !=
      ERROR_INSUFFICIENT_BUFFER) || (dwNeeded == 0))
   {
      ClosePrinter(hPrinter);
      if(buffer != NULL)
         delete buffer;
      return 0;
   }
   // Allocate enough space for PRINTER_INFO_2...
   pi2 = (PRINTER_INFO_2 *)GlobalAlloc(GPTR,dwNeeded);
   if(pi2 == NULL)
   {
      ClosePrinter(hPrinter);
      if(buffer != NULL)
         delete buffer;
      return 0;
   }
   // The second GetPrinter fills in all the current settings, so all you
   // need to do is modify what you're interested in...
   bFlag = GetPrinter(hPrinter,2,(LPBYTE)pi2,dwNeeded,&dwNeeded);
   if(!bFlag)
   {
      GlobalFree(pi2);
      ClosePrinter(hPrinter);
      if(buffer != NULL)
         delete buffer;
      return 0;
   }
   // If GetPrinter didn't fill in the DEVMODE,
   // try to get it by calling DocumentProperties...
   if(pi2->pDevMode == NULL)
   {
      dwNeeded = DocumentProperties(NULL,hPrinter,
         pPrinterName,NULL,NULL,0);
      if(dwNeeded <= 0)
      {
         GlobalFree(pi2);
         ClosePrinter(hPrinter);
         if(buffer != NULL)
            delete buffer;
         return 0;
      }
      pDevMode = (DEVMODE *)GlobalAlloc(GPTR,dwNeeded);
      if(pDevMode == NULL)
      {
         GlobalFree(pi2);
         ClosePrinter(hPrinter);
         if(buffer != NULL)
            delete buffer;
         return 0;
      }
      lFlag = DocumentProperties(NULL,hPrinter,
         pPrinterName,pDevMode,NULL,DM_OUT_BUFFER);
      if(lFlag != IDOK || pDevMode == NULL)
      {
         GlobalFree(pDevMode);
         GlobalFree(pi2);
         ClosePrinter(hPrinter);
         if(buffer != NULL)
            delete buffer;
         return 0;
      }
      pi2->pDevMode = pDevMode;
   }
   // Driver is reporting that it doesn't support this change...
   if(!(pi2->pDevMode->dmFields & DM_ORIENTATION))
   {
      GlobalFree(pi2);
      ClosePrinter(hPrinter);
      if(pDevMode)
         GlobalFree(pDevMode);
      if(buffer != NULL)
         delete buffer;
      return 0;
   }
   // Specify exactly what we are attempting to change...
   pi2->pDevMode->dmFields = DM_ORIENTATION;
   // Make note of the current Orientation setting
   // If the functions works, return it to the calling program
   // So that, the application can set it back to whatever
   // it used to be before the call
   int OriginalOrientation = pi2->pDevMode->dmOrientation;
   // Now, change it to whatever was requested by the calling application
   pi2->pDevMode->dmOrientation = dmOrientation;
   // Do not attempt to set security descriptor...
   pi2->pSecurityDescriptor = NULL;
   // Make sure the driver-dependent part of devmode is updated...
   lFlag = DocumentProperties(NULL,hPrinter,pPrinterName,
      pi2->pDevMode,pi2->pDevMode,
      DM_IN_BUFFER | DM_OUT_BUFFER);
   if(lFlag != IDOK)
   {
      GlobalFree(pi2);
      ClosePrinter(hPrinter);
      if(pDevMode)
         GlobalFree(pDevMode);
      if(buffer != NULL)
         delete buffer;
      return 0;
   }
   // Update printer information...
   bFlag = SetPrinter(hPrinter,2,(LPBYTE)pi2,0);
   if(!bFlag)
      // The driver doesn't support, or it is unable to make the change...
   {
      GlobalFree(pi2);
      ClosePrinter(hPrinter);
      if(pDevMode)
         GlobalFree(pDevMode);
      if(buffer != NULL)
         delete buffer;
      return 0;
   }
   // Tell other apps that there was a change...
   SendMessageTimeout(HWND_BROADCAST,WM_DEVMODECHANGE,0L,
      (LPARAM)(LPCSTR)pPrinterName,SMTO_NORMAL,1000,NULL);
   // Clean up...
   if(pi2)
      GlobalFree(pi2);
   if(hPrinter)
      ClosePrinter(hPrinter);
   if(pDevMode)
      GlobalFree(pDevMode);
   if(buffer != NULL)
      delete buffer;
   return OriginalOrientation;
}

// It seems that you can’t change more than one attribute
// at a time, hence I needed to
// create two separate functions, on to change orientation,
// and the other to select /
// change the paper type for my specific task
BOOL SetPaperType(short dmPaperSize)
{
   HANDLE hPrinter = NULL;
   DWORD dwNeeded = 0;
   PRINTER_INFO_2 *pi2 = NULL;
   DEVMODE *pDevMode = NULL;
   PRINTER_DEFAULTS pd;
   BOOL bFlag;
   LONG lFlag;
   LPTSTR pPrinterName = NULL;
   DWORD size;
   GetDefaultPrinter(NULL,&size);
   TCHAR* buffer = new TCHAR[size];
   if(GetDefaultPrinter(buffer,&size))
      pPrinterName = buffer;
   else
   {
      if(buffer != NULL)
         delete buffer;
      return FALSE;
   }
   // Open printer handle (on Windows NT,
   // you need full-access because you
   // will eventually use SetPrinter)...
   ZeroMemory(&pd,sizeof(pd));
   pd.DesiredAccess = PRINTER_ALL_ACCESS;
   bFlag = OpenPrinter(pPrinterName,&hPrinter,&pd);
   if(!bFlag || (hPrinter == NULL))
   {
      if(buffer != NULL)
         delete buffer;
      return FALSE;
   }
   // The first GetPrinter tells you how big the buffer should be in 
   // order to hold all of PRINTER_INFO_2. Note that this should fail with 
   // ERROR_INSUFFICIENT_BUFFER. If GetPrinter fails for any other reason 
   // or dwNeeded isn't set for some reason, then there is a problem...
   SetLastError(0);
   bFlag = GetPrinter(hPrinter,2,0,0,&dwNeeded);
   if((!bFlag) && (GetLastError() !=
      ERROR_INSUFFICIENT_BUFFER) || (dwNeeded == 0))
   {
      ClosePrinter(hPrinter);
      if(buffer != NULL)
         delete buffer;
      return FALSE;
   }
   // Allocate enough space for PRINTER_INFO_2...
   pi2 = (PRINTER_INFO_2 *)GlobalAlloc(GPTR,dwNeeded);
   if(pi2 == NULL)
   {
      ClosePrinter(hPrinter);
      if(buffer != NULL)
         delete buffer;
      return FALSE;
   }
   // The second GetPrinter fills in all the current settings, so all you
   // need to do is modify what you're interested in...
   bFlag = GetPrinter(hPrinter,2,(LPBYTE)pi2,dwNeeded,&dwNeeded);
   if(!bFlag)
   {
      GlobalFree(pi2);
      ClosePrinter(hPrinter);
      if(buffer != NULL)
         delete buffer;
      return FALSE;
   }
   // If GetPrinter didn't fill in the DEVMODE,
   // try to get it by calling DocumentProperties...
   if(pi2->pDevMode == NULL)
   {
      dwNeeded = DocumentProperties(NULL,hPrinter,
         pPrinterName,NULL,NULL,0);
      if(dwNeeded <= 0)
      {
         GlobalFree(pi2);
         ClosePrinter(hPrinter);
         if(buffer != NULL)
            delete buffer;
         return FALSE;
      }
      pDevMode = (DEVMODE *)GlobalAlloc(GPTR,dwNeeded);
      if(pDevMode == NULL)
      {
         GlobalFree(pi2);
         ClosePrinter(hPrinter);
         if(buffer != NULL)
            delete buffer;
         return FALSE;
      }
      lFlag = DocumentProperties(NULL,hPrinter,
         pPrinterName,pDevMode,NULL,DM_OUT_BUFFER);
      if(lFlag != IDOK || pDevMode == NULL)
      {
         GlobalFree(pDevMode);
         GlobalFree(pi2);
         ClosePrinter(hPrinter);
         if(buffer != NULL)
            delete buffer;
         return FALSE;
      }
      pi2->pDevMode = pDevMode;
   }
   // Specify exactly what we are attempting to change...
   pi2->pDevMode->dmFields = DM_PAPERSIZE;
   // Now, change it to whatever was requested by the calling application
   pi2->pDevMode->dmPaperSize = dmPaperSize;
   // Do not attempt to set security descriptor...
   pi2->pSecurityDescriptor = NULL;
   // Make sure the driver-dependent part of devmode is updated...
   lFlag = DocumentProperties(NULL,hPrinter,pPrinterName,
      pi2->pDevMode,pi2->pDevMode,
      DM_IN_BUFFER | DM_OUT_BUFFER);
   if(lFlag != IDOK)
   {
      GlobalFree(pi2);
      ClosePrinter(hPrinter);
      if(pDevMode)
         GlobalFree(pDevMode);
      if(buffer != NULL)
         delete buffer;
      return FALSE;
   }
   // Update printer information...
   bFlag = SetPrinter(hPrinter,2,(LPBYTE)pi2,0);
   if(!bFlag)
      // The driver doesn't support, or it is unable to make the change...
   {
      GlobalFree(pi2);
      ClosePrinter(hPrinter);
      if(pDevMode)
         GlobalFree(pDevMode);
      if(buffer != NULL)
         delete buffer;
      return FALSE;
   }
   // Tell other apps that there was a change...
   SendMessageTimeout(HWND_BROADCAST,WM_DEVMODECHANGE,0L,
      (LPARAM)(LPCSTR)pPrinterName,SMTO_NORMAL,1000,NULL);
   // Clean up...
   if(pi2)
      GlobalFree(pi2);
   if(hPrinter)
      ClosePrinter(hPrinter);
   if(pDevMode)
      GlobalFree(pDevMode);
   if(buffer != NULL)
      delete buffer;
   return TRUE;
}
