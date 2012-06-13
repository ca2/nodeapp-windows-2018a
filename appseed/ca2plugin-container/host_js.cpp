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

#include "framework.h"


namespace npca2
{


   host_js *host_js::NewObject(host * phost, bool auto_release/* = false*/)
   {

      host_js *pobj = static_cast<host_js *>(NPN_CreateObject(phost->m_instance, &s_npclassHostJs));

      if (pobj)
      {
         // There are certain cases where this may have failed;
         // If obj is null, return null; it's probably during shutdown
         pobj->m_phost = phost;
         pobj->m_bAutoRelease = auto_release;
         pobj->m_bValid = true;
      }

      return pobj;

   }

   bool host_js::is_host_js(const NPObject* const npo)
   {
      return npo->_class == &s_npclassHostJs;
   }

   host_js::host_js(NPP npp)
      : m_bAutoRelease(false), m_bValid(false)
   {
   }

   host_js::~host_js()
   {
      /*    if (m_autoRelease && !m_browser.expired() && !m_api.expired()) {
      // If the JSAPI object is still around and we're set to autorelease, tell the BrowserHost
      // that we're done with it.  Otherwise it's either gone or we don't control its lifecycle
      getHost()->releaseJSAPIPtr(m_api.lock());
      }*/
   }

   bool host_js::isValid()
   {
      return m_bValid;
   }

   void host_js::Invalidate()
   {
      m_bValid = false;
   }

   bool host_js::HasMethod(NPIdentifier name)
   {

      if (!isValid())
         return false;

      vsstring mName = m_phost->StringFromIdentifier(name);

      if(mName == "is_installing")
         return true;

      if(mName == "is_installed")
         return true;

      if(mName == "should_reload")
         return true;

      return false;

   }

   bool host_js::Invoke(NPIdentifier name, const NPVariant *args, uint32_t argCount, NPVariant *result)
   {

      VOID_TO_NPVARIANT(*result);

      if(!isValid())
         return false;

      vsstring mName;

      if (name != NULL)
      {
         mName = m_phost->StringFromIdentifier(name);
      }

      if(mName.m_psz != NULL && mName == "is_installing")
      {
         result->type = NPVariantType_Int32;
         result->value.intValue = is_installation_lock_file_locked() ? 1 : 0;
         return true;
      }

      if(mName.m_psz != NULL && mName == "is_installed")
      {
         result->type = NPVariantType_Int32;
         result->value.intValue = ::is_ca2_installed() ? 1 : 0;
         return true;
      }

      if(mName.m_psz != NULL && mName == "should_reload")
      {
         result->type = NPVariantType_Int32;
         result->value.intValue = 0;
         //result->value.intValue = m_phost->m_bReload ? 1 : 0;
         return true;
      }

      return false;

   }

   bool host_js::InvokeDefault(const NPVariant *args, uint32_t argCount, NPVariant *result)
   {
      return Invoke(NULL, args, argCount, result);
   }

   bool host_js::HasProperty(NPIdentifier name)
   {
      if (!isValid()) return false;
      return false;
      /*    try {
      NpapiBrowserHostPtr browser(getHost());
      // Handle numeric identifiers
      if(!browser->IdentifierIsString(name)) {
      int32_t sIdx = browser->IntFromIdentifier(name);
      return getAPI()->HasProperty(sIdx);
      }

      vsstring sName(browser->StringFromIdentifier(name));
      // We check for events of that name as well in order to allow setting of an event handler in the
      // old javascript style, i.e. plugin.onload = function() .....;

      if (sName == "addEventListener" || sName == "removeEventListener") {
      return true;
      } else if (sName != "toString" && getAPI()->HasMethodObject(sName))
      return true;
      else
      return !HasMethod(name) && getAPI()->HasProperty(sName);
      } catch (const std::bad_cast&) {
      return false; // invalid object
      } catch (const script_error& e) {
      if (!m_browser.expired()) 
      getHost()->SetException(this, e.what());
      return false;
      }*/
   }

   bool host_js::GetProperty(NPIdentifier name, NPVariant *result)
   {
      if (!isValid()) return false;
      return false;
      /*    try {
      NpapiBrowserHostPtr browser(getHost());
      FB::variant res;
      if (browser->IdentifierIsString(name)) {
      vsstring sName(browser->StringFromIdentifier(name));
      if (sName == "addEventListener") {
      res = m_addEventFunc;
      } else if (sName == "removeEventListener") {
      res = m_removeEventFunc;
      } else if (getAPI()->HasMethodObject(sName)) {
      res = getAPI()->GetMethodObject(sName);
      } else {
      res = getAPI()->GetProperty(sName);
      }
      } else {
      res = getAPI()->GetProperty(browser->IntFromIdentifier(name));
      }

      browser->getNPVariant(result, res);
      return true;
      } catch (const std::bad_cast&) {
      return false; // invalid object
      } catch (const script_error& e) {
      if (!m_browser.expired())
      getHost()->SetException(this, e.what());
      return false;
      }*/
   }

   bool host_js::SetProperty(NPIdentifier name, const NPVariant *value)
   {
      if (!isValid()) return false;
      /*try {
      NpapiBrowserHostPtr browser(getHost());
      FB::variant arg = browser->getVariant(value);
      if (browser->IdentifierIsString(name)) {
      vsstring sName(browser->StringFromIdentifier(name));
      if (getAPI()->HasMethodObject(sName)) {
      throw FB::script_error("This property cannot be changed");
      } else {
      getAPI()->SetProperty(sName, arg);
      }
      } else {
      getAPI()->SetProperty(browser->IntFromIdentifier(name), arg);
      }
      return true;
      } catch (const std::bad_cast&) {
      return false; // invalid object
      } catch(const script_error& e) {
      if (!m_browser.expired())
      getHost()->SetException(this, e.what());
      return false;
      }*/

      return false;

   }

   bool host_js::RemoveProperty(NPIdentifier name)
   {
      if (!isValid()) return false;
      /*try {
      NpapiBrowserHostPtr browser(getHost());
      if (browser->IdentifierIsString(name)) {
      vsstring sName(browser->StringFromIdentifier(name));
      getAPI()->RemoveProperty(sName);
      } else {
      getAPI()->RemoveProperty(browser->IntFromIdentifier(name));
      }
      return true;
      } catch (const std::bad_cast&) {
      return false; // invalid object
      } catch(const script_error& e) {
      if (!m_browser.expired())
      getHost()->SetException(this, e.what());
      return false;
      }*/

      return false;

   }

   bool host_js::Enumeration(NPIdentifier **value, uint32_t *count)
   {

      if (!isValid())
         return false;

      stra_dup stra;

      stra.add("is_installing");
      stra.add("is_installed");
      stra.add("should_reload");

      *count = (uint32_t) stra.get_count();
      NPIdentifier *outList(NULL);

      outList = (NPIdentifier*)NPN_MemAlloc((uint32_t)(sizeof(NPIdentifier) * *count));

      for (uint32_t i = 0; i < *count; i++)
      {
         outList[i] = NPN_GetStringIdentifier(stra[i]);
      }

      *value = outList;

      return true;

      /*try {
      typedef std::vector<vsstring> StringArray;
      StringArray memberList;
      getAPI()->getMemberNames(memberList);
      *count = memberList.size() + 2;
      NPIdentifier *outList(NULL);

      NpapiBrowserHostPtr browser(getHost());
      outList = (NPIdentifier*)browser->MemAlloc((uint32_t)(sizeof(NPIdentifier) * *count));

      for (uint32_t i = 0; i < memberList.size(); i++) {
      outList[i] = browser->GetStringIdentifier(memberList.at(i).c_str());
      }
      outList[memberList.size()] = browser->GetStringIdentifier("addEventListener");
      outList[memberList.size() + 1] = browser->GetStringIdentifier("removeEventListener");
      *value = outList;
      return true;
      } catch (const std::bad_cast&) {
      *count = 0;
      return false; // invalid object
      } catch (const script_error& e) {
      *count = 0;
      if (!m_browser.expired())
      getHost()->SetException(this, e.what());
      return false;
      }*/
      return false;

   }

   bool host_js::Construct(const NPVariant *args, uint32_t argCount, NPVariant *result)
   {
      //VOID_TO_NPVARIANT(*result);
      if (!isValid()) return false;
      return false;

      /*    try {
      NpapiBrowserHostPtr browser(getHost());

      std::vector<FB::variant> vArgs;
      for (unsigned int i = 0; i < argCount; i++) {
      vArgs.push_back(browser->getVariant(&args[i]));
      }
      // Default method call
      FB::variant ret = getAPI()->Construct(vArgs);
      browser->getNPVariant(result, ret);
      return true;
      } catch (const std::bad_cast&) {
      return false; // invalid object
      } catch (const script_error& e) {
      if (!m_browser.expired())
      getHost()->SetException(this, e.what());
      return false;
      }*/
   }


   /****************************************************************************\
   These are the static functions given to the browser in the NPClass struct.
   You might look at these as the "entry points" for the host_js
   \****************************************************************************/

   NPObject *host_js::_Allocate(NPP npp, NPClass *aClass)
   {
      return (NPObject *)new host_js(npp);
   }

   void host_js::_Deallocate(NPObject *npobj)
   {
      host_js * phostjs = static_cast<host_js *>(npobj);
      if(phostjs->m_phost != NULL &&
         phostjs->m_phost->m_phostjs == phostjs)
      {
         phostjs->m_phost->m_phostjs = NULL;
      }
      delete phostjs;
   }

   void host_js::_Invalidate(NPObject *npobj)
   {
      host_js *obj = static_cast<host_js *>(npobj);
      obj->Invalidate();
   }

   bool host_js::_HasMethod(NPObject *npobj, NPIdentifier name)
   {
      host_js *obj = static_cast<host_js *>(npobj);
      return obj->HasMethod(name);
   }

   bool host_js::_Invoke(NPObject *npobj, NPIdentifier name, const NPVariant *args, uint32_t argCount, NPVariant *result)
   {
      host_js *obj = static_cast<host_js *>(npobj);
      return obj->Invoke(name, args, argCount, result);
   }

   bool host_js::_InvokeDefault(NPObject *npobj, const NPVariant *args, uint32_t argCount, NPVariant *result)
   {
      host_js *obj = static_cast<host_js *>(npobj);
      return obj->InvokeDefault(args, argCount, result);
   }

   bool host_js::_HasProperty(NPObject *npobj, NPIdentifier name)
   {
      host_js *obj = static_cast<host_js *>(npobj);
      return obj->HasProperty(name);
   }

   bool host_js::_GetProperty(NPObject *npobj, NPIdentifier name, NPVariant *result)
   {
      host_js *obj = static_cast<host_js *>(npobj);
      return obj->GetProperty(name, result);
   }

   bool host_js::_SetProperty(NPObject *npobj, NPIdentifier name, const NPVariant *value)
   {
      host_js *obj = static_cast<host_js *>(npobj);
      return obj->SetProperty(name, value);
   }

   bool host_js::_RemoveProperty(NPObject *npobj, NPIdentifier name)
   {
      host_js *obj = static_cast<host_js *>(npobj);
      return obj->RemoveProperty(name);
   }

   bool host_js::_Enumeration(NPObject *npobj, NPIdentifier **value, uint32_t *count)
   {
      host_js *obj = static_cast<host_js *>(npobj);
      return obj->Enumeration(value, count);
   }

   bool host_js::_Construct(NPObject *npobj, const NPVariant *args, uint32_t argCount, NPVariant *result)
   {
      host_js *obj = static_cast<host_js *>(npobj);
      return obj->Construct(args, argCount, result);
   }

   // This defines the "entry points"; it's how the browser knows how to create the object
   // when you call NPN_CreateObject, and how it knows how to call functions on it
   NPClass host_js::s_npclassHostJs =
   {
      NP_CLASS_STRUCT_VERSION_CTOR,
      host_js::_Allocate,
      host_js::_Deallocate,
      host_js::_Invalidate,
      host_js::_HasMethod,
      host_js::_Invoke,
      host_js::_InvokeDefault,
      host_js::_HasProperty,
      host_js::_GetProperty,
      host_js::_SetProperty,
      host_js::_RemoveProperty,
      host_js::_Enumeration,
      host_js::_Construct
   };


} // namespace npca2


