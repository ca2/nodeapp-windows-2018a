//*****************************************************************************
//
//      Class:          ThreadPool
//      Author:         Kenny Kerr
//      Date created:   10 April 2004
//      Description:    Provides the ability to queue simple member functions
//                      to the Windows thread pool.
//
//*****************************************************************************

#pragma once

#include <memory>

namespace Kerr
{
    class ThreadPool
    {
    public:

        template <typename T>
        static void QueueUserWorkItem(void (T::*function)(),
                                      T* object,
                                      ULONG flags = WT_EXECUTELONGFUNCTION)
        {
            typedef std::pair<void (T::*)(), T*> CallbackType;
            std::auto_ptr<CallbackType> p(new CallbackType(function, object));

            if (::QueueUserWorkItem(ThreadProc<T>,
                                    p.get(),
                                    flags))
            {
                // The ThreadProc now has the responsibility of deleting the pair.
                p.release();
            }
            else
            {
//                AtlThrowLastWin32();
            }                
        }

    private:

        template <typename T>
        static DWORD WINAPI ThreadProc(PVOID context)
        {
            typedef std::pair<void (T::*)(), T*> CallbackType;

            std::auto_ptr<CallbackType> p(static_cast<CallbackType*>(context));

            (p->second->*p->first)();
            return 0;
        }

        ThreadPool();
    };
}
