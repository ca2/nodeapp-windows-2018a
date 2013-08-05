#include "framework.h"


namespace draw2d_gdi
{


   object::object()
   {

      m_hgdiobj = NULL;

   }
   
   object::~object()
   { 

      destroy(); 

   }
   

   int object::get_object(int nCount, LPVOID lpObject) const
   {

      return ::GetObjectW(m_hgdiobj, nCount, lpObject); 
   
   }

   bool object::CreateStockObject(int nIndex)
   {
   
      return (m_hgdiobj = ::GetStockObject(nIndex)) != NULL; 
   
   }

   bool object::UnrealizeObject()
   { 
   
      ASSERT(get_handle() != NULL); 
      
      return ::UnrealizeObject(get_handle()) != FALSE; 
   
   }

   UINT object::GetObjectType() const
   { 
   
      return (UINT)::GetObjectType(get_handle()); 
   
   }


   ::draw2d_gdi::object * graphics_object_allocator(::ca2::application * papp, HANDLE h)
   {

      switch(::GetObjectType(h))
      {
      case OBJ_BITMAP:
         return dynamic_cast < ::draw2d_gdi::object * > (canew(bitmap(papp)));
      case OBJ_REGION:
         return dynamic_cast < ::draw2d_gdi::object * > (canew(region(papp)));
      case OBJ_PEN:
         return dynamic_cast < ::draw2d_gdi::object * > (canew(pen(papp)));
      case OBJ_BRUSH:
         return dynamic_cast < ::draw2d_gdi::object * > (canew(brush(papp)));
      case OBJ_PAL:
         return dynamic_cast < ::draw2d_gdi::object * > (canew(palette(papp)));
      case OBJ_FONT:
         return dynamic_cast < ::draw2d_gdi::object * > (canew(font(papp)));
      }
      
      return NULL;

   }

   ::draw2d::object * object::from_handle(::ca2::application * papp, HGDIOBJ h)
   {
      
      ::draw2d_gdi::object * pobject = graphics_object_allocator(papp, h);

      if(pobject == NULL)
         return NULL;

      pobject->Attach(h);

      return pobject;

   }

   bool object::Attach(HGDIOBJ hObject)
   {
      
      if (hObject == NULL)
      {

         return FALSE;

      }

      if(m_hgdiobj != NULL)
      {

         destroy();

      }

      m_hgdiobj = hObject;

      return TRUE;

   }


   HGDIOBJ object::Detach()
   {
      
      HGDIOBJ hgdiobj = m_hgdiobj;

      m_hgdiobj = NULL;

      return hgdiobj;

   }


   bool object::destroy()
   {
      
      if(m_hgdiobj == NULL)
         return true;

      return ::DeleteObject(Detach()) != FALSE;

   }

#ifdef _DEBUG
   void object::dump(dump_context & dumpcontext) const
   {
      ::ca2::object::dump(dumpcontext);

      dumpcontext << "get_handle() = " << get_handle();
      dumpcontext << "\n";
   }

   void object::assert_valid() const
   {
      ::ca2::object::assert_valid();
/*      ASSERT(get_handle() == NULL ||
         (afxData.bWin95 || ::GetObjectType(get_handle()) != 0));*/
   }
#endif

   void * object::detach_os_data()
   {
      return Detach();
   }

   void * object::get_handle() const
   {

      return get_os_data();

   }

   object::operator HGDIOBJ() const
   { 

      return this == NULL ? NULL : (HGDIOBJ) get_os_data(); 

   }
   

   HGDIOBJ object::get_os_data() const
   {

      if(this == NULL)
         return NULL;
      
      defer_update();

      return m_hgdiobj; 

   }

   void object::update()
   {

      if(m_hgdiobj != NULL)
      {
         
         destroy();

         m_hgdiobj = NULL;

      }
      
   }


}  // namespace draw2d_gdi

