#pragma once


class CLASS_DECL_MACHINE_EVENT machine_event
{
public:
   HANDLE   m_hmutex;
   bool     m_bInitialized;

   bool read(machine_event_data * pdata);
   bool write(machine_event_data * pdata);

   machine_event();
   virtual ~machine_event();
   bool initialize();
};
