#pragma once


namespace _template
{

      /////////////////////////////////////////////////////////////////////////////
   // string resource helpers

   
#pragma warning(push)
   #pragma warning(disable: 4200)
      struct ATLSTRINGRESOURCEIMAGE
      {
         WORD nLength;
         WCHAR achString[];
      };
   #pragma warning(pop)   // C4200

   inline const ATLSTRINGRESOURCEIMAGE* _AtlGetStringResourceImage( HINSTANCE hInstance, HRSRC hResource, UINT id ) throw()
   {
      const ATLSTRINGRESOURCEIMAGE* pImage;
      const ATLSTRINGRESOURCEIMAGE* pImageEnd;
      ULONG nResourceSize;
      HGLOBAL hGlobal;
      UINT iIndex;

      hGlobal = ::LoadResource( hInstance, hResource );
      if( hGlobal == NULL )
      {
         return( NULL );
      }

      pImage = (const ATLSTRINGRESOURCEIMAGE*)::LockResource( hGlobal );
      if( pImage == NULL )
      {
         return( NULL );
      }

      nResourceSize = ::SizeofResource( hInstance, hResource );
      pImageEnd = (const ATLSTRINGRESOURCEIMAGE*)(LPBYTE( pImage )+nResourceSize);
      iIndex = id&0x000f;

      while( (iIndex > 0) && (pImage < pImageEnd) )
      {
         pImage = (const ATLSTRINGRESOURCEIMAGE*)(LPBYTE( pImage )+(sizeof( ATLSTRINGRESOURCEIMAGE )+(pImage->nLength*sizeof( WCHAR ))));
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

   inline const ATLSTRINGRESOURCEIMAGE* AtlGetStringResourceImage( HINSTANCE hInstance, UINT id ) throw()
   {
      HRSRC hResource;

      hResource = ::FindResource( hInstance, MAKEINTRESOURCE( ((id>>4)+1) ), RT_STRING );
      if( hResource == NULL )
      {
         return( NULL );
      }

      return _AtlGetStringResourceImage( hInstance, hResource, id );
   }

   inline const ATLSTRINGRESOURCEIMAGE* AtlGetStringResourceImage( HINSTANCE hInstance, UINT id, WORD wLanguage ) throw()
   {
      HRSRC hResource;

      hResource = ::FindResourceEx( hInstance, RT_STRING, MAKEINTRESOURCE( ((id>>4)+1) ), wLanguage );
      if( hResource == NULL )
      {
         return( NULL );
      }

      return _AtlGetStringResourceImage( hInstance, hResource, id );
   }

   /*
   Needed by both atlcomcli and atlsafe, so needs to be in here
   */
   inline HRESULT AtlSafeArrayGetActualVartype
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


} // namespace _template


