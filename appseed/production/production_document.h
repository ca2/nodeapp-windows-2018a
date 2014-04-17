#pragma once


namespace production
{


   class CLASS_DECL_CA2_PRODUCTION document :
      public ::user::object
   {
   public:


      document(sp(base_application) papp);

      DECL_GEN_VSIGNAL(data_on_after_change);

	   virtual bool on_new_document();

   public:
	   virtual ~document();
   #ifdef DEBUG
	   virtual void assert_valid() const;
	   virtual void dump(dump_context & dumpcontext) const;
   #endif

      bool on_open_document(var varFile);


   };


} // namespace production



