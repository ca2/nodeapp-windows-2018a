#include "framework.h"
#include <mmddk.h>

// http://blogs.msdn.com/b/matthew_van_eerde/archive/2012/09/21/enumerating-midi-devices.aspx
// Maurits [MSFT]
//21 Sep 2012 9:31 AM

//0
// MSDN Blogs > Matthew van Eerde's web log > Enumerating MIDI devices 

#define LOG(format, ...) TRACE(format, __VA_ARGS__)




namespace music
{


   namespace midi
   {


      namespace mmsystem
      {


         midi::midi(::aura::application * papp) :
            ::object(papp),
            ::aura::department(papp),
            ::music::midi::object(papp),
            ::music::midi::midi(papp)
         {

               m_uiMidiOutDevice = 0;

         }


         midi::~midi()
         {

         }


         ::multimedia::e_result midi::enumerate_midi_devices()
         {
            
            UINT_PTR devs = midiInGetNumDevs();
            
            LOG("midiIn devices: %u", devs);

            for (UINT_PTR dev = 0; dev < devs; dev++) 
            {
            
               MIDIINCAPSW caps = {};

               MMRESULT mmr = midiInGetDevCapsW(dev, &caps, sizeof(caps));

               if (MMSYSERR_NOERROR != mmr) 
               {
                  
                  return translate_os_result(mmr, "enumerate_midi_devices", "midiInGetDevCapsW");

               }

               m_uiaIn.add(dev);

               mmsystem_LogMidiInCaps(dev, caps);

               mmsystem_GetMidiInDeviceInterface(dev);

            }

            devs = midiOutGetNumDevs();
            
            LOG("midiOut devices: %u", devs);
            
            for (UINT dev = 0; dev < devs; dev++)
            {
               
               MIDIOUTCAPSW caps = {};

               MMRESULT mmr = midiOutGetDevCapsW(dev, &caps, sizeof(caps));

               if (MMSYSERR_NOERROR != mmr)
               {
                  
                  return translate_os_result(mmr, "enumerate_midi_devices", "midiOutGetDevCaps");

               }

               mmsystem_LogMidiOutCaps(dev, caps);

               mmsystem_GetMidiOutDeviceInterface(dev);

            }

            return ::multimedia::result_success;

         }




         void midi::mmsystem_GetMidiInDeviceInterface(UINT_PTR i)
         {

            // query the size of the device interface string
            HMIDIIN h = reinterpret_cast<HMIDIIN>(i);
            ULONG size = 0;
            MMRESULT mmr = midiInMessage(
               h,
               DRV_QUERYDEVICEINTERFACESIZE,
               reinterpret_cast<DWORD_PTR>(&size),
               0
               );
            if (MMSYSERR_NOERROR != mmr) {
               LOG("midiInMessage(DRV_QUERYDEVICEINTERFACESIZE) failed: mmr = 0x%08x", mmr);
               return;
            }

            if (0 == size) {
               LOG("No device interface");
               return;
            }
            if (size % sizeof(WCHAR)) {
               LOG("Device interface length in bytes (%u) should be a multiple of the size of a WCHAR!", size);
               return;
            }

            wstring wstr;
            wstr.alloc(size);




            mmr = midiInMessage(
               h,
               DRV_QUERYDEVICEINTERFACE,
               reinterpret_cast<DWORD_PTR>((wchar_t *) wstr),
               size
               );
            if (MMSYSERR_NOERROR != mmr) {
               LOG("midiInMessage(DRV_QUERYDEVICEINTERFACE) failed: mmr = 0x%08x", mmr);
               return;
            }

            string str(wstr);

            LOG("    Device interface: \"%S\"", str);
         }

         void midi::mmsystem_GetMidiOutDeviceInterface(UINT_PTR i)
         {

            // query the size of the device interface string
            HMIDIOUT h = reinterpret_cast<HMIDIOUT>(i);
            ULONG size = 0;
            MMRESULT mmr = midiOutMessage(
               h,
               DRV_QUERYDEVICEINTERFACESIZE,
               reinterpret_cast<DWORD_PTR>(&size),
               0
               );
            if (MMSYSERR_NOERROR != mmr) {
               LOG("midiOutMessage(DRV_QUERYDEVICEINTERFACESIZE) failed: mmr = 0x%08x", mmr);
               return;
            }

            if (0 == size) {
               LOG("No device interface");
               return;
            }
            if (size % sizeof(WCHAR)) {
               LOG("Device interface length in bytes (%u) should be a multiple of the size of a WCHAR!", size);
               return;
            }

            wstring wstr;
            wstr.alloc(size);

            mmr = midiOutMessage(
               h,
               DRV_QUERYDEVICEINTERFACE,
               reinterpret_cast<DWORD_PTR>((wchar_t *) wstr),
               size
               );
            if (MMSYSERR_NOERROR != mmr) {
               LOG("midiOutMessage(DRV_QUERYDEVICEINTERFACE) failed: mmr = 0x%08x", mmr);
               return;
            }

            string str(wstr);

            LOG("    Device interface: \"%S\"", str);
         }

         void midi::mmsystem_LogMidiInCaps(UINT_PTR i, MIDIINCAPSW caps) {
            LOG(
               "-- %u: %S --\n"
               "    Device ID: %u\n"
               "    Manufacturer identifier: %u\n"
               "    Product identifier: %u\n"
               "    Driver version: %u.%u\n"
               "    Product name: %S\n"
               "    Support: 0x%x"
               ,
               i,
               caps.szPname ? caps.szPname : L"(no name)",
               i,
               caps.wMid,
               caps.wPid,
               caps.vDriverVersion / 256, caps.vDriverVersion % 256,
               caps.szPname ? caps.szPname : L"(no name)",
               caps.dwSupport
               );
            string strName;
            if (caps.szPname != NULL)
            {
               strName.Format("%s (%d)", caps.szPname, i);
            }
            else
            {
               strName.Format("(no name midi In device) (%d)", i);
            }
            m_straIn.add(strName);
         }

         void midi::mmsystem_LogMidiOutCaps(UINT_PTR i, MIDIOUTCAPSW caps) {
            LOG(
               "-- %u: %S --\n"
               "    Device ID: %u\n"
               "    Manufacturer identifier: %u\n"
               "    Product identifier: %u\n"
               "    Driver version: %u.%u\n"
               "    Product name: %S\n"
               "    Technology: %u (%S)\n"
               "    Voices: %u\n"
               "    Notes: %u\n"
               "    Channel mask: 0x%x\n"
               "    Support: 0x%x"
               "%S%S%S%S"
               ,
               i,
               caps.szPname ? caps.szPname : L"(no name)",
               i,
               caps.wMid,
               caps.wPid,
               caps.vDriverVersion / 256, caps.vDriverVersion % 256,
               caps.szPname ? caps.szPname : L"(no name)",
               caps.wTechnology,
               caps.wTechnology == MOD_MIDIPORT ? L"MOD_MIDIPORT" :
               caps.wTechnology == MOD_SYNTH ? L"MOD_SYNTH" :
               caps.wTechnology == MOD_SQSYNTH ? L"MOD_SQSYNTH" :
               caps.wTechnology == MOD_FMSYNTH ? L"MOD_FMSYNTH" :
               caps.wTechnology == MOD_MAPPER ? L"MOD_MAPPER" :
               caps.wTechnology == MOD_WAVETABLE ? L"MOD_WAVETABLE" :
               caps.wTechnology == MOD_SWSYNTH ? L"MOD_SWSYNTH" :
               L"Unrecognized",
               caps.wVoices,
               caps.wNotes,
               caps.wChannelMask,
               caps.dwSupport,
               (caps.dwSupport & MIDICAPS_CACHE) ? L"\n        MIDICAPS_CACHE" : L"",
               (caps.dwSupport & MIDICAPS_LRVOLUME) ? L"\n        MIDICAPS_LRVOLUME" : L"",
               (caps.dwSupport & MIDICAPS_STREAM) ? L"\n        MIDICAPS_STREAM" : L"",
               (caps.dwSupport & MIDICAPS_VOLUME) ? L"\n        MIDICAPS_VOLUME" : L""
               );
            string strName;
            if (caps.szPname != NULL)
            {
               strName.Format("%S (%d)", caps.szPname, i);
            }
            else
            {
               strName.Format("(no name midi Out device) (%d)", i);
            }
            m_straOut.add(strName);
         }


         ::multimedia::e_result midi::translate_os_result(string & strMessage, string & strOsMessage, ::music::midi::object * pobject, int64_t iOsResult, const string & strContext, const string & strText)
         {

            ::multimedia::e_result mmrc = translate_mmr((MMRESULT) iOsResult, strMessage, strOsMessage);

            if (mmrc == ::multimedia::result_success)
               return ::multimedia::result_success;

            return mmrc;

         }

         static mutex * s_pmutex = NULL;

         mutex & get_midi_mutex()
         {

            if(s_pmutex == NULL)
            {
               s_pmutex = new mutex(::get_thread_app());

               // TODO :
               // register s_pmutex in Sys(::get_thread_app()).register_static_system_object
               // for example, for deletion before alloc system is deleted.

            }

            return *s_pmutex;

         }

      } // namespace mmsystem


   } // namespace midi


} // namespace music








