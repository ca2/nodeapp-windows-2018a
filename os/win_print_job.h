#pragma once


namespace win
{


	class CLASS_DECL_win print_job :
	   virtual public ::user::print_job
	{
	public:


	   print_job(sp(base_application) papp);
	   virtual ~print_job();


	   virtual int32_t run() = 0;

	};


} // namespace win




