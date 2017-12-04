#pragma once


namespace draw2d_gdiplus
{


   class CLASS_DECL_DRAW2D_GDIPLUS object :
      virtual public ::draw2d::object
   {
      public:


         object();
         virtual ~object();


         virtual void * get_os_data() const;


         bool delete_object();


         int32_t get_object(int32_t nCount, LPVOID lpObject) const;
         UINT GetObjectType() const;
         bool CreateStockObject(int32_t nIndex);
         bool UnrealizeObject();
         bool operator==(const ::draw2d::object& obj) const;
         bool operator!=(const ::draw2d::object& obj) const;

#ifdef DEBUG
         virtual void assert_valid() const override;
         virtual void dump(dump_context & dumpcontext) const override;
#endif

   };


} // namespace core



