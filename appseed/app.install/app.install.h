#pragma once


#include "base/base/base/base.h"


class install_app;

namespace install
{

   class installer;

} // namespace install


#undef App
#define App(pcaapp) (pcaapp->cast_app < ::base::application > ())


#include "resource.h"

#include "install_installer.h"


#include "install_app.h"