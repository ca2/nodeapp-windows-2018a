#pragma once


#include "aura/aura/aura.h"

class app_core
{
public:


   DWORD m_dwStartTime;
   DWORD m_dwAfterApplicationFirstRequest;


};


extern "C" int32_t app_core_main(HINSTANCE hinstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int32_t nCmdShow, app_core & appcore);
