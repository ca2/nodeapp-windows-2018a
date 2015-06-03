#pragma once



struct CLASS_DECL_MACHINE_EVENT blob
{


   size_t   m_sizet;
   char *   m_pchData;


   blob();
   ~blob();
   void read(FILE * f);
   void write(FILE * f);
   blob & operator = (const char * psz);
};

class CLASS_DECL_MACHINE_EVENT machine_event_data
{
public:
   struct CLASS_DECL_MACHINE_EVENT fixed
   {
      fixed();
      bool  m_bRequestCloseApplication;
      bool  m_bSpaUpgrade;
   };
   fixed m_fixed;
   blob m_blobCommand;

   void read(FILE * f);
   void write(FILE * f);
};
