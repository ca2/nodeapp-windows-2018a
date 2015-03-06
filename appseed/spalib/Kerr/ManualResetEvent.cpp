//*****************************************************************************
//
//      Class:          ManualResetEvent
//      Author:         Kenny Kerr
//      Date created:   10 April 2004
//      Description:    Notifies one or more waiting threads that an event has 
//                      occurred.
//
//*****************************************************************************

#include "stdafx.h"
#include "ManualResetEvent.h"

//*****************************************************************************
//
//      Name:           ManualResetEvent
//      Description:    The constructor - initializes a new instance of the 
//                      ManualResetEvent class with an initial state.
//
//*****************************************************************************
Kerr::ManualResetEvent::ManualResetEvent(EventState initialState) :
    m_event(true, SignaledState == initialState)
{
    // Do nothing
}

//*****************************************************************************
//
//      Name:           Signaled
//      Description:    Determines whether the event is currently signaled.
//
//*****************************************************************************
bool Kerr::ManualResetEvent::Signaled() const
{
    return (WAIT_OBJECT_0 == ::WaitForSingleObject(m_event,
                                                   0));
}

//*****************************************************************************
//
//      Name:           Signal
//      Description:    Sets the state of the event to signaled.
//
//*****************************************************************************
void Kerr::ManualResetEvent::Signal()
{
    if (!m_event.Set())
    {
//        AtlThrowLastWin32();
    }
}

//*****************************************************************************
//
//      Name:           Reset
//      Description:    Sets the state of the event to nonsignaled.
//
//*****************************************************************************
void Kerr::ManualResetEvent::Reset()
{
    if (!m_event.Reset())
    {
//        AtlThrowLastWin32();
    }
}

//*****************************************************************************
//
//      Name:           Wait
//      Description:    Waits for the event to become signaled.
//
//*****************************************************************************
bool Kerr::ManualResetEvent::Wait(DWORD timeout)
{
    return (WAIT_OBJECT_0 == ::WaitForSingleObject(m_event,
                                                   timeout));
}
