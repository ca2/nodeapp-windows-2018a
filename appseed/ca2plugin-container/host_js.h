/**********************************************************\ 
Original Author: Richard Bateman (taxilian)

Created:    Oct 16, 2009
License:    Dual license model; choose one of two:
New BSD License
http://www.opensource.org/licenses/bsd-license.php
- or -
GNU Lesser General Public License, version 2.1
http://www.gnu.org/licenses/lgpl-2.1.html

Copyright 2009 Richard Bateman, Firebreath development team
\**********************************************************/


#pragma once


namespace npca2
{


   class host_js :
      public NPObject
   {
   public:



      host  *  m_phost;
      bool     m_bAutoRelease;
      bool     m_bValid;


      static NPClass s_npclassHostJs;

      host_js(NPP npp);
      virtual ~host_js();


      bool isValid();



      void Invalidate();
      bool HasMethod(NPIdentifier name);
      bool Invoke(NPIdentifier name, const NPVariant *args, uint32_t argCount, NPVariant *result);
      bool InvokeDefault(const NPVariant *args, uint32_t argCount, NPVariant *result);
      bool HasProperty(NPIdentifier name);
      bool GetProperty(NPIdentifier name, NPVariant *result);
      bool SetProperty(NPIdentifier name, const NPVariant *value);
      bool RemoveProperty(NPIdentifier name);
      bool Enumeration(NPIdentifier **value, uint32_t *count);
      bool Construct(const NPVariant *args, uint32_t argCount, NPVariant *result);


      static bool is_host_js(const NPObject* const npo);

      static host_js * NewObject(host * phost, bool auto_release = false);


      // Static methods referenced in the NPClass
      static NPObject *_Allocate(NPP npp, NPClass *aClass);
      static void _Deallocate(NPObject *npobj);
      static void _Invalidate(NPObject *npobj);
      static bool _HasMethod(NPObject *npobj, NPIdentifier name);
      static bool _Invoke(NPObject *npobj, NPIdentifier name, const NPVariant *args, uint32_t argCount, NPVariant *result);
      static bool _InvokeDefault(NPObject *npobj, const NPVariant *args, uint32_t argCount, NPVariant *result);
      static bool _HasProperty(NPObject *npobj, NPIdentifier name);
      static bool _GetProperty(NPObject *npobj, NPIdentifier name, NPVariant *result);
      static bool _SetProperty(NPObject *npobj, NPIdentifier name, const NPVariant *value);
      static bool _RemoveProperty(NPObject *npobj, NPIdentifier name);
      static bool _Enumeration(NPObject *npobj, NPIdentifier **value, uint32_t *count);
      static bool _Construct(NPObject *npobj, const NPVariant *args, uint32_t argCount, NPVariant *result);

   };

} // namespace npca2



