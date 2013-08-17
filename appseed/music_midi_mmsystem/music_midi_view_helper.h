#pragma once


namespace music
{

   namespace midi
   {


      class document_helper;


      class CLASS_DECL_VERIWELL_MULTIMEDIA view_helper   :
         virtual public ::ca2::object
      {
      public:

         


         view_helper();
         virtual ~view_helper();


         sp(document_helper) HelperGetMidiDocHelper();

      };


   } // namespace midi


} // namespace music



