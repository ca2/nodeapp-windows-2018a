#pragma once


namespace gen
{

      /////////////////////////////////////////////////////////////////////////////
   // string resource helpers

   
#pragma warning(push)
   #pragma warning(disable: 4200)
      struct STRINGRESOURCEIMAGE
      {
         WORD nLength;
         WCHAR achString[];
      };
   #pragma warning(pop)   // C4200

   inline const STRINGRESOURCEIMAGE* _gen_GetStringResourceImage( HINSTANCE hInstance, HRSRC hResource, UINT id ) throw()
   {
      const STRINGRESOURCEIMAGE* pImage;
      const STRINGRESOURCEIMAGE* pImageEnd;
      ULONG nResourceSize;
      HGLOBAL hGlobal;
      UINT iIndex;

      hGlobal = ::LoadResource( hInstance, hResource );
      if( hGlobal == NULL )
      {
         return( NULL );
      }

      pImage = (const STRINGRESOURCEIMAGE*)::LockResource( hGlobal );
      if( pImage == NULL )
      {
         return( NULL );
      }

      nResourceSize = ::SizeofResource( hInstance, hResource );
      pImageEnd = (const STRINGRESOURCEIMAGE*)(LPBYTE( pImage )+nResourceSize);
      iIndex = id&0x000f;

      while( (iIndex > 0) && (pImage < pImageEnd) )
      {
         pImage = (const STRINGRESOURCEIMAGE*)(LPBYTE( pImage )+(sizeof( STRINGRESOURCEIMAGE )+(pImage->nLength*sizeof( WCHAR ))));
         iIndex--;
      }
      if( pImage >= pImageEnd )
      {
         return( NULL );
      }
      if( pImage->nLength == 0 )
      {
         return( NULL );
      }

      return( pImage );
   }

   inline const STRINGRESOURCEIMAGE* gen_GetStringResourceImage( HINSTANCE hInstance, UINT id ) throw()
   {
      HRSRC hResource;

      hResource = ::FindResource( hInstance, MAKEINTRESOURCE( ((id>>4)+1) ), RT_STRING );
      if( hResource == NULL )
      {
         return( NULL );
      }

      return _gen_GetStringResourceImage( hInstance, hResource, id );
   }

   inline const STRINGRESOURCEIMAGE* gen_GetStringResourceImage( HINSTANCE hInstance, UINT id, WORD wLanguage ) throw()
   {
      HRSRC hResource;

      hResource = ::FindResourceEx( hInstance, RT_STRING, MAKEINTRESOURCE( ((id>>4)+1) ), wLanguage );
      if( hResource == NULL )
      {
         return( NULL );
      }

      return _gen_GetStringResourceImage( hInstance, hResource, id );
   }

   /*
   Needed by both atlcomcli and atlsafe, so needs to be in here
   */
   inline HRESULT gen_SafeArrayGetActualVartype
   (
       SAFEARRAY *psaArray,
       VARTYPE *pvtType
   )
   {
       HRESULT hrSystem=::SafeArrayGetVartype(psaArray, pvtType);

       if(FAILED(hrSystem))
       {
           return hrSystem;
       }

       /*
       When Windows has a SAFEARRAY of type VT_DISPATCH with FADF_HAVEIID,
       it returns VT_UNKNOWN instead of VT_DISPATCH. We patch the value to be correct
       */
       if(pvtType && *pvtType==VT_UNKNOWN)
       {
           if(psaArray && ((psaArray->fFeatures & FADF_HAVEIID)!=0))
           {
               if(psaArray->fFeatures & FADF_DISPATCH)
               {
                   *pvtType=VT_DISPATCH;
               }
           }
       }

       return hrSystem;
   }


} // namespace gen


