#pragma once


#include "np.h"



namespace npca2
{

   class host_js;


   class host : 
      public nsPluginInstanceBase,
      public ::hotplugin::host
   {
   public:

   
      HWND                          m_hwndMessage;
      NPP                           m_instance;
      HWND                          m_hwnd;
      bool                          m_bStream;
      NPObject *                    m_pobjectWindow;
      NPVariant                     m_varDocument;

      vsstring                      m_vssPluginName;
      vsstring                      m_vssPluginDescription;

      host_js *                     m_phostjs;





      


      host(NPP aInstance);
      virtual ~host();

      NPError SetWindow(NPWindow* pNPWindow) ;

   

      uint16 HandleEvent(void * pvoid);


      // npca2::host_interaction::plugin
      virtual HWND get_host_window();
      virtual void redraw();
      virtual bool is_ok();
      virtual void post_message(UINT uiMessage, WPARAM wparam, LPARAM lparam);
      

      NPBool init(NPWindow* aWindow);
      void shut();
      NPBool isInitialized();
      virtual NPError NewStream(NPMIMEType type, NPStream* stream, NPBool seekable, uint16* stype);
      int32  Write(NPStream *stream, int32 offset, int32 len, void *buffer);
      virtual NPError DestroyStream(NPStream *stream, NPError reason);

     // locals
      const char * getVersion();


      // host
      virtual bool      open_url(const char * pszUrl);
      virtual bool      reload_plugin();


      virtual vsstring  calc_location_url();


      void on_paint(HDC hdcWindow, LPCRECT lprect);

      bool finalize();

      virtual NPError GetValue(NPPVariable variable, void *value);
      ////////////////////////////////////////////////////////////////////////////////////////////////////
      /// @fn NPObject * FB::Npapi::NpapiPlugin::getScriptableObject()
      ///
      /// @brief  Returns the wrapped object
      ///
      /// @return NPObject * for wrapped object
      ////////////////////////////////////////////////////////////////////////////////////////////////////
      NPObject * getScriptableObject();

      vsstring StringFromIdentifier(NPIdentifier identifier);


#ifdef WINDOWS
      uint_ptr message_handler(uint_ptr uiMessage, WPARAM wparam, LPARAM lparam);
#else
      int message_handler(XEvent * pevent);
#endif

      virtual void * get_system();
      virtual void set_system(void * pvoidPluginSystem);

   };

} // namespace npca2

