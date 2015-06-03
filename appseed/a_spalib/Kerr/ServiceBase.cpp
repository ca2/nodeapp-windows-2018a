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

#include "stdafx.h"
#include "ServiceBase.h"

//
// The single, static service instance pointer.
//
Kerr::ServiceBase* Kerr::ServiceBase::m_service = 0;

//*****************************************************************************
//
//      Name:           ServiceBase
//      Description:    The constructor - initializes a new instance of the 
//                      ServiceBase class.
//
//*****************************************************************************
Kerr::ServiceBase::ServiceBase(DWORD controlsAccepted) :
    m_handle(0)
{
    m_status.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
    m_status.dwCurrentState = SERVICE_START_PENDING;
    m_status.dwControlsAccepted = controlsAccepted;
}

//*****************************************************************************
//
//      Name:           ServiceBase
//      Description:    The virtual destructor.
//
//*****************************************************************************
Kerr::ServiceBase::~ServiceBase()
{
    // Do nothing
}

//*****************************************************************************
//
//      Name:           Run
//      Description:    Provides the main entry point for an executable that 
//                      contains a single service. Loads the service into 
//                      memory so it can be started. This method blocks until
//                      the service has stopped.
//
//*****************************************************************************
void Kerr::ServiceBase::Run(ServiceBase& service)
{
//	ATLASSERT(0 == m_service);
	m_service = &service;

	SERVICE_TABLE_ENTRYW serviceTable[] = 
	{
		// Even though the service name is ignored for own process services,
		// it must be a valid string and cannot be 0.
		{ L"", ServiceMain },

		// Designates the end of table.
		{ 0, 0 } 
	};

	if (!::StartServiceCtrlDispatcherW(serviceTable))
	{
//		AtlThrowLastWin32();
	}
}

DWORD Win32FromHResult(HRESULT value)
{
//    ATLASSERT(FACILITY_WIN32 == HRESULT_FACILITY(value));
    return value & ~0x80070000;
}


//*****************************************************************************
//
//      Name:           UpdateState
//      Description:    Updates the current state and exit code of the service 
//                      and notifies the service control manager of the change.
//
//*****************************************************************************
void Kerr::ServiceBase::UpdateState(DWORD state,
                                    HRESULT errorCode)
{
    m_status.dwCurrentState = state;

//    ATLASSERT(0 == m_status.dwWin32ExitCode);
//    ATLASSERT(0 == m_status.dwServiceSpecificExitCode);

    if (FAILED(errorCode))
    {
        if (FACILITY_WIN32 == HRESULT_FACILITY(errorCode))
        {
            m_status.dwWin32ExitCode = Win32FromHResult(errorCode);
        }
        else
        {
            m_status.dwWin32ExitCode = ERROR_SERVICE_SPECIFIC_ERROR; 
            m_status.dwServiceSpecificExitCode = errorCode;
        }
    }

    SetServiceStatus();
}

//*****************************************************************************
//
//      Name:           SetServiceStatus
//      Description:    Wrapper for the SetServiceStatus SCM function.
//
//*****************************************************************************
void Kerr::ServiceBase::SetServiceStatus()
{
    // If m_service is zero it means we're not running as a service but
    // rather from the console. This is possible in debug mode.

#ifdef DEBUG
    if (0 != m_service)
    {
#endif
//        ATLASSERT(0 != m_service);

        if (!::SetServiceStatus(m_handle,
                                &m_status))
        {
//            AtlThrowLastWin32();
        }
#ifdef DEBUG
    }
#endif
}

//*****************************************************************************
//
//      Name:           ServiceName
//      Description:    Gets the service name of the service (as apposed to the
//                      display name). This is the name reported by the service
//                      control manager.
//
//*****************************************************************************
const std::wstring& Kerr::ServiceBase::ServiceName() const
{
    return m_serviceName;
}

//*****************************************************************************
//
//      Name:           ServiceMain
//      Description:    The starting point for the service.
//
//*****************************************************************************
void WINAPI Kerr::ServiceBase::ServiceMain(DWORD argumentCount,
                                           PWSTR* arguments)
{
    //
    // Since there's no way to inform the SCM of failure before a successful
    // call to RegisterServiceCtrlHandler, if an error occurs before we have 
    // a service status handle we don't catch any exceptions and let the 
    // process terminate. The SCM will diligently log this event.
    //

//    ATLASSERT(0 != m_service);

    if (1 != argumentCount || 0 == arguments || 0 == arguments[0])
    {
//        AtlThrow(E_INVALIDARG);
    }

    m_service->m_serviceName = arguments[0];

    m_service->m_handle = ::RegisterServiceCtrlHandler("",
                                                       ServiceHandler);

    if (0 == m_service->m_handle)
    {
//        AtlThrowLastWin32();
    }

    m_service->SetServiceStatus();

    try
    {
        m_service->Start(0);
        m_service->UpdateState(SERVICE_RUNNING);
    }
    catch (const char * psz)
    {
        //
        // If the service can't start it should throw an exception from the 
        // Start method. If this happens, we catch it here and notify the 
        // SCM so that it can log the error code.
        //

        m_service->UpdateState(SERVICE_STOPPED, atoi(psz));
    }
}

//*****************************************************************************
//
//      Name:           Handler
//      Description:    The handler function called by the control dispatcher 
//                      when an event occurs.
//
//*****************************************************************************
void WINAPI Kerr::ServiceBase::ServiceHandler(DWORD control)
{
    try
    {
        switch (control)
        {
            case SERVICE_CONTROL_CONTINUE :
            {
                m_service->UpdateState(SERVICE_CONTINUE_PENDING);
                m_service->Start(control);
                m_service->UpdateState(SERVICE_RUNNING);
                break;
            }
            case SERVICE_CONTROL_PAUSE :
            {
                m_service->UpdateState(SERVICE_PAUSE_PENDING);
                m_service->Stop(control);
                m_service->UpdateState(SERVICE_PAUSED);
                break;
            }
            case SERVICE_CONTROL_SHUTDOWN :
            case SERVICE_CONTROL_STOP :
            {
                m_service->UpdateState(SERVICE_STOP_PENDING);
                m_service->Stop(control);
                m_service->UpdateState(SERVICE_STOPPED);
                break;
            }
        }
    }
    catch (const char * psz)
    {
        m_service->UpdateState(SERVICE_STOPPED, atoi(psz));
    }
}
