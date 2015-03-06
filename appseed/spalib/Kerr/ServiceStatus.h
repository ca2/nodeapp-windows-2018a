//*****************************************************************************
//
//      Class:          ServiceStatus
//      Author:         Kenny Kerr
//      Date created:   10 April 2004
//      Description:    Provides default initialization for SERVICE_STATUS.
//
//*****************************************************************************

#pragma once

#include <Winsvc.h>

namespace Kerr
{
    class ServiceStatus : public SERVICE_STATUS
    {
    public:

        ServiceStatus()
        {
            //ATLASSERT(sizeof (*this) == sizeof (SERVICE_STATUS));

            ::ZeroMemory(this, sizeof (SERVICE_STATUS));
        }
    };
}
