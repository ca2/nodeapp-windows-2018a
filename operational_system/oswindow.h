#pragma once




typedef HWND oswindow;


/*

class oswindow_data;

class oswindow_dataptra;

class CLASS_DECL_c oswindow
{
public:


   oswindow_data *   m_pdata;


   static oswindow_dataptra * s_pdataptra;
   static int32_t find(Windows::UI::Core::CoreWindow ^ window);
   static oswindow_data * get(Windows::UI::Core::CoreWindow ^ window);



public:


   oswindow();
   oswindow(const ::core::null & null);
   oswindow(Windows::UI::Core::CoreWindow ^ window);
   oswindow(const oswindow & oswindow);
   oswindow(const void * p);
   oswindow(const LPARAM & lparam);
   oswindow(const WPARAM & wparam);



   void * ptr()
   {
      return m_pdata;
   }

   void * ptr() const
   {
      return m_pdata;
   }

   operator DWORD_PTR ()
   {
      return (DWORD_PTR) m_pdata;
   }

   operator DWORD_PTR () const
   {
      return (DWORD_PTR) m_pdata;
   }

   static bool remove(Windows::UI::Core::CoreWindow ^ window);


   oswindow & operator = (const oswindow & window);

   bool operator == (const void * p) const
   {
      return m_pdata == p;
   }
   bool operator != (const void * p) const
   {
      return m_pdata != p;
   }
   bool operator == (const oswindow & w) const
   {
      return m_pdata == w.m_pdata;
   }
   bool operator != (const oswindow & w) const
   {
      return m_pdata != w.m_pdata;
   }

   Windows::UI::Core::CoreWindow ^ window();
   Windows::UI::Core::CoreWindow ^ window() const;


};

*/





