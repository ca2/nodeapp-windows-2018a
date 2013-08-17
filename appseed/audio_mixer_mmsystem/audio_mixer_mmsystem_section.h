#pragma once


namespace audio_mixer
{


   class CLASS_DECL_CA2_MULTIMEDIA section :
      virtual public ::ca2::section,
      virtual public ::user::interaction
   {
   public:



   enum EIds
   {

      IdsMuteAll,
      IdsMute

   };

      
      section(sp(::ca2::application) papp);
      virtual ~section();


      virtual bool initialize1();



      bool initialize();
      

      bool finalize();





   };


} // namespace mixguserbase




