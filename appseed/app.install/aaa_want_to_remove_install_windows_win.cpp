//#include "framework.h"














#define MAX_LOADSTRING 100

#define ID_START 100
#define ID_FINISH 101
#define ID_STEP_BACKWARD 102

void CommandRegister();
void CommandFinish();

DWORD WINAPI ThreadProcAppListInitialize(LPVOID lpParam);


#ifdef SUPORTA_MATEMATICA_AVANCADA
double g_dPi = asin_dup(1.0) * 4;
#endif















