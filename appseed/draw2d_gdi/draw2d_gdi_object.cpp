#include "framework.h"


namespace draw2d_gdi
{


   object::object()
   {

      m_hgdiobj = NULL;

   }
   
   object::~object()
   { 

      synch_lock ml(&user_mutex());

      for(int i = 0; i < m_ptraGraphics.get_size(); i++)
      {

         try
         {

            m_ptraGraphics[i]->m_ptraObject.remove(this);

         }
         catch(...)
         {

         }

      }

      if(m_hgdiobj != NULL)
      {

         for(int i = 0; i < m_ptraGraphics.get_size(); i++)
         {

            try
            {

               if(::GetCurrentObject(m_ptraGraphics[i]->get_handle1(), ::GetObjectType(m_hgdiobj)) == m_hgdiobj)
               {

                  GDI_GRAPHICS(m_ptraGraphics[i])->set_original_object(::GetObjectType(m_hgdiobj));

               }

            }
            catch(...)
            {

            }

         }

         bool bOk = ::DeleteObject(m_hgdiobj) != FALSE;

      }

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


   ::draw2d_gdi::object * graphics_object_allocator(base_application * papp, HANDLE h)
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

      synch_lock ml(&user_mutex());
      
      for(int i = 0; i < m_ptraGraphics.get_size(); i++)
      {

         try
         {

            ::draw2d_gdi::graphics * pgraphics = m_ptraGraphics[i];

            pgraphics->m_ptraObject.remove(this);

         }
         catch(...)
         {

         }

      }

      if(m_hgdiobj == NULL)
      {
         
         m_ptraGraphics.remove_all();

         return true;

      }

      for(int i = 0; i < m_ptraGraphics.get_size(); i++)
      {

         try
         {

            if(::GetCurrentObject(m_ptraGraphics[i]->get_handle1(), ::GetObjectType(m_hgdiobj)) == m_hgdiobj)
            {

               m_ptraGraphics[i]->set_original_object(::GetObjectType(m_hgdiobj));

            }

         }
         catch(...)
         {

         }

      }

      m_ptraGraphics.remove_all();

      bool bOk = ::DeleteObject(m_hgdiobj) != FALSE;

      m_hgdiobj = NULL;

      return bOk;

   }

#ifdef _DEBUG
   void object::dump(dump_context & dumpcontext) const
   {
      ::object::dump(dumpcontext);

      dumpcontext << "get_handle() = " << get_handle();
      dumpcontext << "\n";
   }

   void object::assert_valid() const
   {
      ::object::assert_valid();
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




