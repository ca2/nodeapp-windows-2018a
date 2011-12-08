#pragma once


namespace win
{


   class CLASS_DECL_VMSWIN graphics_object :
      virtual public ::ca::graphics_object
   {
   public:

      
      graphics_object();
      virtual ~graphics_object();

      virtual void * get_os_data() const;

      BOOL delete_object();

   #pragma push_macro("GetObject")
   #undef GetObject
      int _AFX_FUNCNAME(GetObject)(int nCount, LPVOID lpObject) const;
      int GetObject(int nCount, LPVOID lpObject) const;
   #pragma pop_macro("GetObject")
      UINT GetObjectType() const;
      BOOL CreateStockObject(int nIndex);
      BOOL UnrealizeObject();
      BOOL operator==(const ::ca::graphics_object& obj) const;
      BOOL operator!=(const ::ca::graphics_object& obj) const;

   #ifdef _DEBUG
      virtual void dump(dump_context & dumpcontext) const;
      virtual void assert_valid() const;
   #endif

   };


} // namespace ca