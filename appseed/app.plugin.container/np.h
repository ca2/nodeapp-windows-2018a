#pragma once

#include "npapi.h"
#include "npfunctions.h"
#include "pluginbase.h"


extern NPIdentifier sFoo_id;
extern NPIdentifier sBar_id;
extern NPIdentifier sDocument_id;
extern NPIdentifier sLocation_id;
extern NPIdentifier sBody_id;
extern NPIdentifier sCreateElement_id;
extern NPIdentifier sCreateTextNode_id;
extern NPIdentifier sAppendChild_id;
extern NPIdentifier sPluginType_id;
extern NPObject *sWindowObj;


__declspec(dllimport) NPError fillPluginFunctionTable(NPPluginFuncs* aNPPFuncs);
__declspec(dllimport) NPError fillNetscapeFunctionTable(NPNetscapeFuncs* aNPPFuncs);
