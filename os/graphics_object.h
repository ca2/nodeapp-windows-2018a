#pragma once


namespace win
{


   class CLASS_DECL_win graphics_object :
      virtual public ::ca::graphics_object
   {
   public:

      
      graphics_object();
      virtual ~graphics_object();

      virtual int_ptr get_os_data() const;

      bool delete_object();

   #pragma push_macro("GetObject")
   #undef GetObject
      int ___FUNCNAME(GetObject)(int nCount, LPVOID lpObject) const;
      int GetObject(int nCount, LPVOID lpObject) const;
   #pragma pop_macro("GetObject")
      UINT GetObjectType() const;
      bool CreateStockObject(int nIndex);
      bool UnrealizeObject();
      bool operator==(const ::ca::graphics_object& obj) const;
      bool operator!=(const ::ca::graphics_object& obj) const;

      virtual void dump(dump_context & dumpcontext) const;
      virtual void assert_valid() const;

   };


} // namespace ca