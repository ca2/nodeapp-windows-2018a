#pragma once


namespace music
{


   namespace midi
   {


      class CLASS_DECL_VERIWELL_MULTIMEDIA tempo_map_entry
      {
      public:

         imedia::position           tkTempo;
         uint32_t           msBase;
         uint32_t           dwTempo;

         tempo_map_entry()
         {
            tkTempo  = 0;
            msBase   = 0;
            dwTempo  = 0;
         }


      };

      class CLASS_DECL_VERIWELL_MULTIMEDIA tempo_map :
         public  array < tempo_map_entry, tempo_map_entry &>
      {
      public:
      };


   } // namespace midi


} // namespace music




