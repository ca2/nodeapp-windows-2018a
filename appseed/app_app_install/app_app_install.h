#pragma once


#include "base/base/base/base.h"


namespace app_app_install
{


   class app;
   class installer;


} // namespace install


#undef Sys
#define Sys(pcaapp) (pcaapp->cast_app < ::app_app_install::app > ())


#include "resource.h"


#include "app_app_install_installer.h"


#include "app_app_install_app.h"