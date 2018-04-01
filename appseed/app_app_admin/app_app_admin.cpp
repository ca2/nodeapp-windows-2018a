#include "aura/aura/app/acid.inl"


::aura::application * get_acid_app(::aura::application * papp)
{

   ::install::application * pinstallapp = new ::install::application();

   pinstallapp->m_bAdmin = true;

   return pinstallapp;

}