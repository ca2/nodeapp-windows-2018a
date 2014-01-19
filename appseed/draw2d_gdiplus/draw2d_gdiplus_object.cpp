#include "framework.h"

namespace draw2d_gdiplus
{

   object::object()
   {
      //set_handle(NULL); 
   }
   
   object::~object()
   { 
      //delete_object(); 
   }
   
   int32_t object::get_object(int32_t nCount, LPVOID lpObject) const
   { 
      //return ::GetObject(get_handle(), nCount, lpObject); 
      return 0;
   }

   bool object::CreateStockObject(int32_t nIndex)
   {
      //return (set_handle(::GetStockObject(nIndex))) != NULL; 
      return 0;
   }
   bool object::UnrealizeObject()
   { 
      //ASSERT(get_handle() != NULL); return ::UnrealizeObject(get_handle()); 
      return 0;
   }
   UINT object::GetObjectType() const
   {
      //return (UINT)::GetObjectType(get_handle()); 
      return 0;
   }
   bool object::operator==(const ::draw2d::object& obj) const
   { 
      //return ((HGDIOBJ) obj.get_os_data()) == get_handle(); 
      return FALSE;
   }
   bool object::operator!=(const ::draw2d::object& obj) const
   { 
      //return ((HGDIOBJ) obj.get_os_data()) != get_handle(); 
      return TRUE;
   }


   object * graphics_object_allocator(sp(base_application) papp, HANDLE h)
   {
      /*switch(::GetObjectType(h))
      {
      case OBJ_BITMAP:
         return dynamic_cast < object * > (new bitmap(papp));
      case OBJ_REGION:
         return dynamic_cast < object * > (new region(papp));
      case OBJ_PEN:
         return dynamic_cast < object * > (new pen(allocer()));
      case OBJ_BRUSH:
         return dynamic_cast < object * > (new brush(papp));
      case OBJ_PAL:
         return dynamic_cast < object * > (new palette(papp));
      case OBJ_FONT:
         return dynamic_cast < object * > (new font(allocer()));
      }
      return new object();*/
      return NULL;
   }

   /*object* object::from_handle(sp(base_application) papp, HGDIOBJ h)
   {
      hgdiobj_map* pMap = afxMapHGDIOBJ(TRUE); //create map if not exist
      ASSERT(pMap != NULL);
      object* pObject = (::draw2d_gdiplus::object*)pMap->from_handle(h, &graphics_object_allocator, papp);
      ASSERT(pObject == NULL || pObject->get_os_data() == h);
      return pObject;
   }

   bool object::attach(HGDIOBJ hObject)
   {
      if (hObject == NULL)
      {
         return FALSE;
      }
      if(get_os_data() != NULL)
      {
         delete_object();
      }
      // remember early to avoid leak
      set_handle(hObject);
      hgdiobj_map* pMap = afxMapHGDIOBJ(TRUE); // create map if not exist
      ASSERT(pMap != NULL);
      pMap->set_permanent(get_os_data(), this);
      return TRUE;
   }

   HGDIOBJ object::detach()
   {
      HGDIOBJ hObject = get_os_data();
      if (hObject != NULL)
      {
         hgdiobj_map* pMap = afxMapHGDIOBJ(); // don't create if not exist
         if (pMap != NULL)
            pMap->remove_handle(get_os_data());
      }

      set_handle(NULL);
      return hObject;
   }*/

   bool object::delete_object()
   {
      //if (get_os_data() == NULL)
        // return FALSE;
      //return ::DeleteObject(detach());
      return FALSE;
   }

   /////////////////////////////////////////////////////////////////////////////
   // ::draw2d::object


   void object::dump(dump_context & dumpcontext) const
   {
      ::object::dump(dumpcontext);

//      dumpcontext << "get_handle() = " << get_handle();
  //    dumpcontext << "\n";
   }

   void object::assert_valid() const
   {
      ::object::assert_valid();
      //ASSERT(get_handle() == NULL ||
        // (afxData.bWin95 || ::GetObjectType(get_handle()) != 0));
   }


   /*void * object::detach_os_data()
   {
      return detach();
   }*/

   /*void * object::get_os_data() const
   {
   return get_os_data();
   }*/


   void * object::get_os_data() const
   {

      return NULL;

   }


} // namespace draw2d_gdiplus


/*hgdiobj_map * afxMapHGDIOBJ(bool bCreate)
{
   UNREFERENCED_PARAMETER(bCreate);
   try
   {
      __MODULE_STATE* pState = __get_module_state();
      if(pState == NULL)
         return NULL;
      return pState->m_pmapHGDIOBJ;
   }
   catch(...)
   {
      return NULL;
   }

}
*/