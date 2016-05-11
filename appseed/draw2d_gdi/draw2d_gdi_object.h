#pragma once


namespace draw2d_gdi
{

   
   class graphics;


   class CLASS_DECL_DRAW2D_GDI object :
      virtual public ::draw2d::object
   {
   public:


      HGDIOBJ                                            m_hgdiobj;
      ref_array < ::draw2d_gdi::graphics >      m_ptraGraphics;


      object();
      virtual ~object();


      operator HGDIOBJ() const;
      virtual void * get_os_data() const;
      virtual HGDIOBJ get_handle() const;


      bool attach(void * posdata);

      bool Attach(HGDIOBJ hObject);
      HGDIOBJ Detach();

      virtual bool destroy();

      int get_object(int nCount, LPVOID lpObject) const;

      UINT GetObjectType() const;

      bool CreateStockObject(int nIndex);

      bool UnrealizeObject();

      virtual void dump(dump_context & dumpcontext) const;
      virtual void assert_valid() const;

      void * detach_os_data();


      virtual void update();


   };




} // namespace draw2d_gdi




