call "%VS_VARS%" 

cl /I"." /I"%CA2_ROOT%app" /I"%CA2_ROOT%app/thirdparty/include/x64" /I"%CA2_ROOT%app/thirdparty/include" /I"%CA2_ROOT%app/thirdparty/appseed" /I"%CA2_ROOT%app/appseedcore" /I"%CA2_ROOT%app/appseedcore/ca2" /I"%CA2_ROOT%app/nodeapp/windows" /I"%CA2_ROOT%app/nodeapp/windows/include" /I"%CA2_ROOT%app/appseed" /I"%CA2_ROOT%app/appseed/cube" /I"%NETNODE_ROOT%net/netseed" /I"%NETNODE_ROOT%net/netseed/include" /I"%CA2_ROOT%time/dynamic_source/library/include" /Zi /nologo /O2 /Ob2 /Oi /Ot /Oy /W3 /WX- /D "_NETNODE_LIBRARY_LIBRARY" /D "_WINDLL" /D "_DEBUG" /D "_MBCS" /GF- /Gm- /EHa /MD /GS /fp:fast /fp:except- /Zc:wchar_t /Zc:forScope /GR /openmp-   /Yu"framework.h" /Fp"%CA2_ROOT%time/intermediate/%PLATFORM%/netnode_dynamic_source_library/netnode_dynamic_source_library.pch" /Fa"%CA2_ROOT%time/intermediate/%PLATFORM%/netnode_dynamic_source_library/%ITEM_DIR%" /Fo"%CA2_ROOT%time/intermediate/%PLATFORM%/netnode_dynamic_source_library/%ITEM_DIR%" /Fd"%CA2_ROOT%time/intermediate/%PLATFORM%/netnode_dynamic_source_library/vc110.pdb" /Gd /analyze-  /errorReport:queue   /Zm384 /bigobj /c "%CA2_ROOT%time/dynamic_source/%ITEM_NAME%.cpp" > "%CA2_ROOT%time/dynamic_source/%ITEM_NAME%-compile-log.txt" 
