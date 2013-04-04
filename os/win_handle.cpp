#include "framework.h"


namespace win
{


   void window::DeleteTempMap()
   {
      
      oswindow_map* pMap = __get_module_state()->m_pmapHWND;

      if(::ca::is_null(pMap))
         return;

      single_lock sl(&pMap->m_mutex, TRUE);

      array < oswindow > oswindowaRemove;
      array < ::win::window * > wndptraRemove;

      POSITION pos = pMap->m_temporaryMap.get_start_position();
      while (pos != NULL)
      {
         HANDLE h; // just used for asserts
         ::win::window * pTemp;
         pMap->m_temporaryMap.get_next_assoc(pos, h, pTemp);

         if(!::IsWindow((oswindow) h))
         {
            oswindowaRemove.add((oswindow) h);
            wndptraRemove.add(pTemp);
         }

      }

      for(int32_t i = 0; i < wndptraRemove.get_count(); i++)
      {
         wndptraRemove[i]->m_handlea[0] = NULL;
         wndptraRemove[i]->m_handlea[1] = NULL;

         (*pMap->m_pfnDestructObject)(wndptraRemove[i]);   // destruct the object
      }

      for(int32_t i = 0; i < oswindowaRemove.get_count(); i++)
      {
         pMap->m_temporaryMap.remove_key(oswindowaRemove[i]);
      }

   }


} // namespace win


