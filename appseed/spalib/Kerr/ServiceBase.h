//*****************************************************************************
//
//      Class:          ServiceBase
//      Author:         Kenny Kerr
//      Date created:   10 April 2004
//      Description:    Provides a base class for a service that will exist as 
//                      part of a service application. ServiceBase must be 
//                      derived when creating a new service class.
//
//*****************************************************************************

#pragma once

#include "ServiceStatus.h"
#include <string>

namespace Kerr
{
    class ServiceBase
    {
    public:

        virtual ~ServiceBase();

        static void Run(ServiceBase& service);

    protected:

        ServiceBase(DWORD controlsAccepted = SERVICE_ACCEPT_PAUSE_CONTINUE | 
                                             SERVICE_ACCEPT_STOP | 
                                             SERVICE_ACCEPT_SHUTDOWN);

        virtual void Start(DWORD control) = 0;
        virtual void Stop(DWORD control) = 0;

        void UpdateState(DWORD state,
                         HRESULT errorCode = S_OK);

        const std::wstring& ServiceName() const;

    private:

        void SetServiceStatus();

        static void WINAPI ServiceMain(DWORD argumentCount,
                                       PWSTR* arguments);

        static void WINAPI ServiceHandler(DWORD control);

        static ServiceBase* m_service;
        SERVICE_STATUS_HANDLE m_handle;
        ServiceStatus m_status;
        std::wstring m_serviceName;

    };
}
