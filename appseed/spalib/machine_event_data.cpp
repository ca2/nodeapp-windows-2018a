#include "StdAfx.h"




blob::blob()
{ 
   m_sizet     = 0; 
   m_pchData   = NULL; 
}

blob::~blob()
{
   if(m_pchData != NULL)
   {
      free(m_pchData); 
      m_pchData = NULL;
   }
}


void blob::read(FILE * f)
{
   if(m_pchData != NULL)
      free(m_pchData);
   fread(&m_sizet, 1, sizeof(m_sizet), f);
   if(m_sizet == 0)
   {
      m_pchData = NULL;
   }
   else
   {
      m_pchData = (char *) malloc(m_sizet);
      fread(m_pchData, 1, m_sizet, f);
   }
}

void blob::write(FILE * f)
{
   fwrite(&m_sizet, 1, sizeof(m_sizet), f);
   if(m_sizet >= 0)
   {
      fwrite(m_pchData, 1, m_sizet, f);
   }
}

blob & blob::operator = (const char * psz)
{
   m_sizet = strlen(psz) + 1;
   m_pchData = _strdup(psz);
   return *this;
}


machine_event_data::fixed::fixed()
{
   m_bRequestCloseApplication    = false;
   m_bSpaUpgrade                 = false;
}

void machine_event_data::read(FILE * f)
{
   fread(&m_fixed, 1, sizeof(m_fixed), f);
   m_blobCommand.read(f);
}

void machine_event_data::write(FILE * f)
{
   fwrite(&m_fixed, 1, sizeof(m_fixed), f);
   m_blobCommand.write(f);
}
