// pane_view_update_hint.cpp: implementation of the pane_view_update_hint class.
//
//////////////////////////////////////////////////////////////////////

#include "framework.h"
#include "pane_view_update_hint.h"

namespace production
{

// IMPLEMENT_DYNAMIC(pane_view_update_hint, ::radix::object)

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

pane_view_update_hint::pane_view_update_hint()
{

}

pane_view_update_hint::~pane_view_update_hint()
{

}

bool pane_view_update_hint::is_type_of(e_type e_type)
{
   return m_etype == e_type;
}

void pane_view_update_hint::set_type(e_type e_type)
{
   m_etype = e_type;
}

} // namespace production