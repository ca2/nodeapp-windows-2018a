#pragma once



#include <Natupnp.h>
#include <UPnP.h>


namespace win
{


   class port_forward :
      virtual public ::core::port_forward
   {
   public:
	
	   // forward declarations
	
	   interface IDerivedNATExternalIPAddressCallback;
	   interface IDerivedNATNumberOfEntriesCallback;
	
	
	   // protected interfaces, which were forward-declared above, and which are used for event notifications from COM
	   // most of the code is here in this .h file, except for the QueryInterface method which is in the .cpp file

	   interface IDerivedNATExternalIPAddressCallback : public INATExternalIPAddressCallback
	   {
		   IDerivedNATExternalIPAddressCallback( ::core::port_forward_change_callbacks* p ) : m_pointer( p ), m_dwRef( 0 ) { };
		
		   HRESULT STDMETHODCALLTYPE NewExternalIPAddress( BSTR bstrNewExternalIPAddress )
		   {
			   ASSERT( m_pointer != NULL );			
            string strNewExternalIPAddress(bstrNewExternalIPAddress);
			   return m_pointer->OnNewExternalIPAddress(strNewExternalIPAddress );
		   }
		
		   HRESULT STDMETHODCALLTYPE QueryInterface(REFIID iid, void ** ppvObject);
		
		   ULONG STDMETHODCALLTYPE AddRef()  {	return ++m_dwRef; }
		
		   ULONG STDMETHODCALLTYPE Release()  
		   {
			   if ( --m_dwRef == 0 )
				   delete this;
			
			   return m_dwRef;
		   }
		
		   DWORD		m_dwRef;
		   ::core::port_forward_change_callbacks*	m_pointer;
	   };
	
	   interface IDerivedNATNumberOfEntriesCallback : public INATNumberOfEntriesCallback
	   {
		   IDerivedNATNumberOfEntriesCallback( ::core::port_forward_change_callbacks* p ) : m_pointer( p ), m_dwRef( 0 ) { };
		
		   HRESULT STDMETHODCALLTYPE NewNumberOfEntries( long lNewNumberOfEntries )
		   {
			   ASSERT( m_pointer != NULL );			
			   return m_pointer->OnNewNumberOfEntries( lNewNumberOfEntries );
		   }
		
		   HRESULT STDMETHODCALLTYPE QueryInterface(REFIID iid, void ** ppvObject); 
		
		   ULONG STDMETHODCALLTYPE AddRef()  { return ++m_dwRef; }
		
		   ULONG STDMETHODCALLTYPE Release()
		   {
			   if ( --m_dwRef == 0 )
				   delete this;
			
			   return m_dwRef;
		   }
		
		   DWORD		m_dwRef;
		   ::core::port_forward_change_callbacks*	m_pointer;
	   };
	
	   // public functions -- there are only a few
	
	   port_forward(sp(base_application) papp);
	   virtual ~port_forward();
	
	   virtual HRESULT ListenForUpnpChanges(::core::port_forward_change_callbacks *pCallbacks = NULL);  // NULL==default object; if you provide your own pointer to a port_forward_change_callbacks-derived object it is deleted for you automatically
	   virtual HRESULT StopListeningForUpnpChanges( );  // Stops listenting for UPnP change events on the router and deletes any port_forward_change_callbacks-derived objects
	
	   virtual bool GetDeviceInformationUsingThread( oswindow oswindow );  // starts a thread that will get IGD (router) device information; the thread posts a UWM_PORT_FORWARD_ENGINE_THREAD_NOTIFICATION message to oswindow when it's done
	   virtual bool GetMappingsUsingThread( oswindow oswindow );  // starts a thread that will get all mappings; the thread posts a UWM_PORT_FORWARD_ENGINE_THREAD_NOTIFICATION message to oswindow when it's done
	   virtual bool EditMappingUsingThread( port_map& oldMapping, port_map& newMapping, oswindow oswindow );  // starts a thread that will edit one specific mapping; the thread posts a UWM_PORT_FORWARD_ENGINE_THREAD_NOTIFICATION message to oswindow when it's done
	   virtual bool AddMappingUsingThread( port_map& newMapping, oswindow oswindow );  // starts a thread that will add one new mapping; the thread posts a UWM_PORT_FORWARD_ENGINE_THREAD_NOTIFICATION message to oswindow when it's done
	   virtual bool DeleteMappingUsingThread( port_map& oldMapping, oswindow oswindow );  // starts a thread that will delete one specific mapping; the thread posts a UWM_PORT_FORWARD_ENGINE_THREAD_NOTIFICATION message to oswindow when it's done
	
	   virtual smart_pointer_array < port_map > get_port_map() const;  // gets a copy of currently-known port mappings
	   virtual smart_pointer_array < device >  get_igd() const;  // gets a copy of currently-know device information
	
	   virtual bool IsAnyThreadRunning() const;  // returns TRUE if there is any thread currently running
	
	
   protected:
	
	   // protected functions used internally by PortMappingEngine
	
	   void InitializeMembersToNull();
	   void DeinitializeCom();
	   HRESULT PopulateDeviceInfoContainer( IUPnPDevice* piDevice, device & deviceInfo, oswindow oswindow=NULL );
	   HRESULT GetNextMapping( IEnumVARIANT* piEnumerator, port_map & mappingContainer );
	   HRESULT SetChangeEventCallbackPointer(::core::port_forward_change_callbacks *pCallbacks);
	
	   static UINT ThreadForPortRetrieval( LPVOID pVoid );
	   static UINT ThreadForDeviceInformationRetrieval( LPVOID pVoid );
	   static UINT ThreadToEditMapping( LPVOID pVoid );
	   static UINT ThreadToAddMapping( LPVOID pVoid );
	   static UINT ThreadToDeleteMapping( LPVOID pVoid );
	
	
	
   protected:
		
	   // protected members
	
	   IUPnPNAT*								m_piNAT;					
	   IDerivedNATExternalIPAddressCallback*	m_piExternalIPAddressCallback;
	   IDerivedNATNumberOfEntriesCallback*		m_piNumberOfEntriesCallback;
	
	   INATEventManager*						m_piEventManager;
	   ::core::port_forward_change_callbacks*			m_pChangeCallbackFunctions;
	
	   bool m_bListeningForUpnpChanges;
	
	   ::core::thread_sp m_pPortMappingThread;
	   ::core::thread_sp m_pDeviceInfoThread;
	   ::core::thread_sp m_pAddMappingThread;
	   ::core::thread_sp m_pEditMappingThread;
	   ::core::thread_sp m_pDeleteMappingThread;
	
	   smart_pointer_array<port_map> m_MappingContainer;
	
	   CRITICAL_SECTION m_cs;
	
	
	   // some more protected members, most of which could not be declared until after full-declaration
	   // of the device_information_container and port_map nested classes
	
	   smart_pointer_array<device> m_DeviceInfo;
	
	   // good-enough method for inter-thread transfer of information, since only
	   // one thread of each type is ever permitted at one time
	
	   port_map m_scratchpadOldMapping;
	   port_map m_scratchpadNewMapping;
	   port_map m_scratchpadAddedMapping;
	   port_map m_scratchpadDeletedMapping;
	
	   oswindow m_oswindow_ForPortMappingThread;
	   oswindow m_oswindow_ForDeviceInfoThread;
	   oswindow m_oswindow_ForAddMappingThread;
	   oswindow m_oswindow_ForEditMappingThread;
	   oswindow m_oswindow_ForDeleteMappingThread;
	
	
   };


} // namespace core

