call "%VS_VARS%" 

cl /I"." /I"%CA2_ROOT%app" /I"%CA2_ROOT%app/thirdparty/include/x64" /I"%CA2_ROOT%app/thirdparty/include" /I"%CA2_ROOT%app/thirdparty/appseed" /I"%CA2_ROOT%app/appseedcore" /I"%CA2_ROOT%app/appseedcore/ca2" /I"%CA2_ROOT%app/nodeapp/windows" /I"%CA2_ROOT%app/nodeapp/windows/include" /I"%CA2_ROOT%app/appseed" /I"%CA2_ROOT%app/appseed/cube" /I"%NETNODE_ROOT%net/netseed/ds/ca2" /I"%NETNODE_ROOT%net/netseed/ds/ca2/include" /I"%CA2_ROOT%time/dynamic_source/library/include" /Zi /nologo /O2 /Ob2 /Oi /Ot /Oy /W3 /WX- /D "_DYNAMIC_SOURCE_LIBRARY_DLL" /D "_WINDLL" /D "_MBCS" /GF- /Gm- /EHa /MD /GS /fp:precise /Zc:wchar_t /Zc:forScope /GR /openmp-   /Yu"StdAfx.h" /Fp"%CA2_ROOT%time/intermediate/%PLATFORM%/dynamic_source_library/dynamic_source_library.pch" /Fa"%CA2_ROOT%time/intermediate/%PLATFORM%/dynamic_source_library/%ITEM_DIR%" /Fo"%CA2_ROOT%time/intermediate/%PLATFORM%/dynamic_source_library/%ITEM_DIR%" /Fd"%CA2_ROOT%time/intermediate/%PLATFORM%/dynamic_source_library/vc110.pdb" /Gd /analyze-  /errorReport:queue   /Zm384 /bigobj /c "%CA2_ROOT%time/dynamic_source/%ITEM_NAME%.cpp" > "%CA2_ROOT%time/dynamic_source/%ITEM_NAME%-compile-log.txt" 
