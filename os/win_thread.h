#pragma once


bool __internal_pre_translate_message(MSG* pMsg);


namespace ca
{

   struct  thread_startup
   {
      ::ca::thread *          m_pthread;    // thread for new thread
      HANDLE hEvent;          // event triggered after success/non-success
      HANDLE hEvent2;         // event triggered after thread is resumed

      thread_startup();
      ~thread_startup();
   };


} // namespace ca


namespace win
{


   class CLASS_DECL_win thread :
      virtual public ::ca::thread,
      virtual public ::ca::message_window_simple_callback
   {
   public:

      // only valid while running
      HANDLE m_hThread;       // this thread's HANDLE
      uint32_t  m_nThreadID;      // this thread's ID


      static comparable_array < HANDLE > s_haThread;
      static comparable_array < ::ca::thread * > s_threadptra;
      static mutex s_mutex;


      // list of frame_window objects for thread
      simple_list < sp(::user::frame_window) >   m_frameList;

      // temporary/permanent map state
      DWORD m_nTempMapLock;           // if not 0, temp maps locked



      LPVOID                              m_pThreadParams; // generic parameters passed to starting function
      __THREADPROC                      m_pfnThreadProc;

      event                              m_evFinish;
      UINT                                m_nDisablePumpCount;
      mutex                               m_mutexUiPtra;
      
      ::ca::thread *                      m_pAppThread;

      UINT                                m_dwFinishTimeout;


      thread(sp(::ca::application) papp);
      virtual ~thread();


      virtual void construct(__THREADPROC pfnThreadProc, LPVOID pParam);



      virtual void * get_os_data() const;
      virtual int_ptr get_os_int() const;


      operator HANDLE() const;


      void set_os_data(void * pvoidOsData);
      void set_os_int(int_ptr iData);

      virtual void set_p(::ca::thread * p);

      virtual bool begin(::ca::e_thread_priority epriority = get_thread_priority_normal(), uint_ptr nStackSize = 0, uint32_t dwCreateFlags = 0, LPSECURITY_ATTRIBUTES lpSecurityAttrs = ::null());

      bool create_thread(::ca::e_thread_priority epriority = get_thread_priority_normal(), uint32_t dwCreateFlags = 0, uint_ptr nStackSize = 0, LPSECURITY_ATTRIBUTES lpSecurityAttrs = ::null()) override;


      virtual sp(::user::interaction) SetMainWnd(sp(::user::interaction) pui);

      virtual int32_t thread_entry(::ca::thread_startup * pstartup);
      virtual int32_t main();
      virtual int32_t thread_term(int32_t nResult);


      virtual void add(sp(::user::interaction) pui);
      virtual void remove(::user::interaction * pui);
      virtual ::count get_ui_count();
      virtual sp(::user::interaction) get_ui(::index iIndex);
      virtual void set_timer(sp(::user::interaction) pui, uint_ptr nIDEvent, UINT nEllapse);
      virtual void unset_timer(sp(::user::interaction) pui, uint_ptr nIDEvent);
      virtual void set_auto_delete(bool bAutoDelete = true);
      virtual void set_run(bool bRun = true);
      virtual event & get_finish_event();
      virtual bool get_run();
      virtual ::ca::thread * get_app_thread();
      virtual sp(::user::interaction) get_active_ui();
      virtual sp(::user::interaction) set_active_ui(sp(::user::interaction) pui);
      virtual void step_timer();


/**
* \file		src/lib/pal/linux/thread_linux.hpp
* \brief	Platform independent threads and synchronization objects (linux version)
*/

      virtual void start();

      ::ca::e_thread_priority get_thread_priority();
      bool set_thread_priority(::ca::e_thread_priority epriority);

   // Operations
      //DWORD SuspendThread();
      uint32_t ResumeThread();
      bool post_thread_message(UINT message, WPARAM wParam, LPARAM lParam);
      bool post_message(sp(::user::interaction) pguie, UINT message, WPARAM wParam, LPARAM lParam);

      virtual bool PreInitInstance();

      // called when occurs an standard_exception exception in run
      // return true to call run again
      virtual bool on_run_exception(::ca::exception & e);

   // Overridables
      // thread initialization
      virtual bool initialize_instance();

      virtual ::ca::message::e_prototype thread::GetMessagePrototype(UINT uiMessage, UINT uiCode); 

      // running and idle processing
      virtual int32_t run();
      virtual void pre_translate_message(::ca::signal_object * pobj);
      virtual bool pump_message();     // low level message pump
      virtual bool on_idle(LONG lCount); // return TRUE if more idle processing
      virtual bool is_idle_message(::ca::signal_object * pobj);  // checks for special messages
      virtual bool is_idle_message(LPMSG lpmsg);  // checks for special messages
      virtual void message_handler(::ca::signal_object * pobj);

      // thread termination
      virtual int32_t exit_instance(); // default will 'delete this'

      // Advanced: exception handling
      virtual void ProcessWndProcException(base_exception * e, ::ca::signal_object * pMsg);

      // Advanced: handling messages sent to message filter hook
      virtual void ProcessMessageFilter(int32_t code, ::ca::signal_object * pobj);

      // Advanced: virtual access to GetMainWnd()
      virtual sp(::user::interaction) GetMainWnd();

      virtual void assert_valid() const;
      virtual void dump(dump_context & dumpcontext) const;
      void CommonConstruct();
      virtual void Delete();
         // 'delete this' only if m_bAutoDelete == TRUE



      virtual void DispatchThreadMessageEx(::ca::signal_object * pobj);  // helper
      virtual void message_window_message_handler(::ca::signal_object * pobj);

      virtual void delete_temp();


      virtual void LockTempMaps();
      virtual bool UnlockTempMaps(bool bDeleteTemp);


		///  \brief		waits for signaling the thread forever
		virtual void wait();

		///  \brief		waits for signaling the thread for a specified time
		///  \param		duration time period to wait for thread
		///  \return	result of waiting action as defined in wait_result
		virtual wait_result wait(const duration & duration);

		///  \brief		sets thread priority
		///  \param		new priority
		void set_priority(int32_t priority);

		///  \brief		gets thread priority
		///  \param		priority
		int32_t priority();


      virtual bool has_message();


      virtual HANDLE item() const;


   };


   CLASS_DECL_win ::ca::thread * get_thread();
   CLASS_DECL_win ::ca::thread_state * get_thread_state();


} // namespace win




