//*****************************************************************************
//
//      Class:          ManualResetEvent
//      Author:         Kenny Kerr
//      Date created:   10 April 2004
//      Description:    Notifies one or more waiting threads that an event has 
//                      occurred.
//
//*****************************************************************************

#pragma once

class CHandle
{
public:
	CHandle() throw();
	CHandle( CHandle& h ) throw();
	explicit CHandle( HANDLE h ) throw();
	~CHandle() throw();

	CHandle& operator=( CHandle& h ) throw();

	operator HANDLE() const throw();

	// Attach to an existing handle (takes ownership).
	void Attach( HANDLE h ) throw();
	// Detach the handle from the object (releases ownership).
	HANDLE Detach() throw();

	// Close the handle.
	void Close() throw();

public:
	HANDLE m_h;
};

inline CHandle::CHandle() throw() :
	m_h( NULL )
{
}

inline CHandle::CHandle( CHandle& h ) throw() :
	m_h( NULL )
{
	Attach( h.Detach() );
}

inline CHandle::CHandle( HANDLE h ) throw() :
	m_h( h )
{
}

inline CHandle::~CHandle() throw()
{
	if( m_h != NULL )
	{
		Close();
	}
}

inline CHandle& CHandle::operator=( CHandle& h ) throw()
{
	if( this != &h )
	{
		if( m_h != NULL )
		{
			Close();
		}
		Attach( h.Detach() );
	}

	return( *this );
}

inline CHandle::operator HANDLE() const throw()
{
	return( m_h );
}

inline void CHandle::Attach( HANDLE h ) throw()
{
//	ATLASSUME( m_h == NULL );
	m_h = h;  // Take ownership
}

inline HANDLE CHandle::Detach() throw()
{
	HANDLE h;

	h = m_h;  // Release ownership
	m_h = NULL;

	return( h );
}

inline void CHandle::Close() throw()
{
	if( m_h != NULL )
	{
		::CloseHandle( m_h );
		m_h = NULL;
	}
}


class CEvent :
	public CHandle
{
public:
	CEvent() throw();
	CEvent( CEvent& h ) throw();
	CEvent( BOOL bManualReset, BOOL bInitialState );
	CEvent( LPSECURITY_ATTRIBUTES pSecurity, BOOL bManualReset, BOOL bInitialState, LPCTSTR pszName );
	explicit CEvent( HANDLE h ) throw();

	// Create a new event
	BOOL Create( LPSECURITY_ATTRIBUTES pSecurity, BOOL bManualReset, BOOL bInitialState, LPCTSTR pszName ) throw();
	// Open an existing named event
	BOOL Open( DWORD dwAccess, BOOL bInheritHandle, LPCTSTR pszName ) throw();
	// Pulse the event (signals waiting objects, then resets)
	BOOL Pulse() throw();
	// Set the event to the non-signaled state
	BOOL Reset() throw();
	// Set the event to the signaled state
	BOOL Set() throw();
};

inline CEvent::CEvent() throw()
{
}

inline CEvent::CEvent( CEvent& hEvent ) throw() :
	CHandle( hEvent )
{
}

inline CEvent::CEvent( BOOL bManualReset, BOOL bInitialState )
{
	BOOL bSuccess;

	bSuccess = Create( NULL, bManualReset, bInitialState, NULL );
	if( !bSuccess )
	{
//		AtlThrowLastWin32();
	}
}

inline CEvent::CEvent( LPSECURITY_ATTRIBUTES pAttributes, BOOL bManualReset, BOOL bInitialState, LPCTSTR pszName )
{
	BOOL bSuccess;

	bSuccess = Create( pAttributes, bManualReset, bInitialState, pszName );
	if( !bSuccess )
	{
//		AtlThrowLastWin32();
	}
}


inline CEvent::CEvent( HANDLE h ) throw() :
	CHandle( h )
{
}

inline BOOL CEvent::Create( LPSECURITY_ATTRIBUTES pSecurity, BOOL bManualReset, BOOL bInitialState, LPCTSTR pszName ) throw()
{
//	ATLASSUME( m_h == NULL );

	m_h = ::CreateEvent( pSecurity, bManualReset, bInitialState, pszName );

	return( m_h != NULL );
}

inline BOOL CEvent::Open( DWORD dwAccess, BOOL bInheritHandle, LPCTSTR pszName ) throw()
{
//	ATLASSUME( m_h == NULL );

	m_h = ::OpenEvent( dwAccess, bInheritHandle, pszName );
	return( m_h != NULL );
}

inline BOOL CEvent::Pulse() throw()
{
//	ATLASSUME( m_h != NULL );

	return( ::PulseEvent( m_h ) );
}

inline BOOL CEvent::Reset() throw()
{
//	ATLASSUME( m_h != NULL );

	return( ::ResetEvent( m_h ) );
}

inline BOOL CEvent::Set() throw()
{
//	ATLASSUME( m_h != NULL );

	return( ::SetEvent( m_h ) );
}


namespace Kerr
{
    class ManualResetEvent
    {
    public:

        enum EventState
        {
            NonsignaledState,
            SignaledState
        };

        explicit ManualResetEvent(EventState initialState = NonsignaledState);

        bool Signaled() const;

        void Signal();
        void Reset();
        bool Wait(DWORD timeout = INFINITE);

    private:

       CEvent m_event;
    };
}
