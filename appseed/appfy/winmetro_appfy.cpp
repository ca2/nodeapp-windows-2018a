#include "nodeapp/os/metrowin.h"



[Platform::MTAThread]
int main(Platform::Array<Platform::String^>^)
{

   auto source = ::metrowin::new_directx_application_source("m_app.exe : app=app-core/hellomultiverse");

   ::Windows::ApplicationModel::Core::CoreApplication::Run(source);
   
   return 0;

}



/*[Platform::MTAThread]
int main(Platform::Array<Platform::String^>^)
{
   
   // No Depends
   
   return 0;

}
*/