#include "framework.h"

namespace win
{

   graphics_object::graphics_object()
   {
      //set_handle(::null()); 
   }
   
   graphics_object::~graphics_object()
   { 
      //delete_object(); 
   }
   
   int32_t graphics_object::get_object(int32_t nCount, LPVOID lpObject) const
   { 
      //return ::GetObject(get_handle(), nCount, lpObject); 
      return 0;
   }

   bool graphics_object::CreateStockObject(int32_t nIndex)
   {
      //return (set_handle(::GetStockObject(nIndex))) != ::null(); 
      return 0;
   }
   bool graphics_object::UnrealizeObject()
   { 
      //ASSERT(get_handle() != ::null()); return ::UnrealizeObject(get_handle()); 
      return 0;
   }
   UINT graphics_object::GetObjectType() const
   {
      //return (UINT)::GetObjectType(get_handle()); 
      return 0;
   }
   bool graphics_object::operator==(const ::ca::graphics_object& obj) const
   { 
      //return ((HGDIOBJ) obj.get_os_data()) == get_handle(); 
      return FALSE;
   }
   bool graphics_object::operator!=(const ::ca::graphics_object& obj) const
   { 
      //return ((HGDIOBJ) obj.get_os_data()) != get_handle(); 
      return TRUE;
   }


   graphics_object * graphics_object_allocator(sp(::ca::application) papp, HANDLE h)
   {
      /*switch(::GetObjectType(h))
      {
      case OBJ_BITMAP:
         return dynamic_cast < graphics_object * > (new bitmap(papp));
      case OBJ_REGION:
         return dynamic_cast < graphics_object * > (new region(papp));
      case OBJ_PEN:
         return dynamic_cast < graphics_object * > (new pen(allocer()));
      case OBJ_BRUSH:
         return dynamic_cast < graphics_object * > (new brush(papp));
      case OBJ_PAL:
         return dynamic_cast < graphics_object * > (new palette(papp));
      case OBJ_FONT:
         return dynamic_cast < graphics_object * > (new font(allocer()));
      }
      return new graphics_object();*/
      return ::null();
   }

   /*graphics_object* graphics_object::from_handle(sp(::ca::application) papp, HGDIOBJ h)
   {
      hgdiobj_map* pMap = afxMapHGDIOBJ(TRUE); //create map if not exist
      ASSERT(pMap != ::null());
      graphics_object* pObject = (::win::graphics_object*)pMap->from_handle(h, &graphics_object_allocator, papp);
      ASSERT(pObject == ::null() || pObject->get_os_data() == h);
      return pObject;
   }

   bool graphics_object::attach(HGDIOBJ hObject)
   {
      if (hObject == ::null())
      {
         return FALSE;
      }
      if(get_os_data() != ::null())
      {
         delete_object();
      }
      // remember early to avoid leak
      set_handle(hObject);
      hgdiobj_map* pMap = afxMapHGDIOBJ(TRUE); // create map if not exist
      ASSERT(pMap != ::null());
      pMap->set_permanent(get_os_data(), this);
      return TRUE;
   }

   HGDIOBJ graphics_object::detach()
   {
      HGDIOBJ hObject = get_os_data();
      if (hObject != ::null())
      {
         hgdiobj_map* pMap = afxMapHGDIOBJ(); // don't create if not exist
         if (pMap != ::null())
            pMap->remove_handle(get_os_data());
      }

      set_handle(::null());
      return hObject;
   }*/

   bool graphics_object::delete_object()
   {
      //if (get_os_data() == ::null())
        // return FALSE;
      //return ::DeleteObject(detach());
      return FALSE;
   }

   /////////////////////////////////////////////////////////////////////////////
   // ::ca::graphics_object


   void graphics_object::dump(dump_context & dumpcontext) const
   {
      ::ca::object::dump(dumpcontext);

//      dumpcontext << "get_handle() = " << get_handle();
  //    dumpcontext << "\n";
   }

   void graphics_object::assert_valid() const
   {
      ::ca::object::assert_valid();
      //ASSERT(get_handle() == ::null() ||
        // (afxData.bWin95 || ::GetObjectType(get_handle()) != 0));
   }


   /*void * graphics_object::detach_os_data()
   {
      return detach();
   }*/

   /*void * graphics_object::get_os_data() const
   {
   return get_os_data();
   }*/


   void * graphics_object::get_os_data() const
   {

      return ::null();

   }


} // namespace win


/*hgdiobj_map * afxMapHGDIOBJ(bool bCreate)
{
   UNREFERENCED_PARAMETER(bCreate);
   try
   {
      __MODULE_STATE* pState = __get_module_state();
      if(pState == ::null())
         return ::null();
      return pState->m_pmapHGDIOBJ;
   }
   catch(...)
   {
      return ::null();
   }

}
*/