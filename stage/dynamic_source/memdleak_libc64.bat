call "%VS_VARS%" 

cl /I"." /I"%CA2_ROOT%app" /I"%CA2_ROOT%app/thirdparty/include/x64" /I"%CA2_ROOT%app/thirdparty/include" /I"%CA2_ROOT%app/thirdparty/appseed" /I"%CA2_ROOT%app/appseedcore" /I"%CA2_ROOT%app/appseedcore/ca2" /I"%CA2_ROOT%app/nodeapp/windows" /I"%CA2_ROOT%app/nodeapp/windows/include" /I"%CA2_ROOT%app/appseed" /I"%CA2_ROOT%app/appseed/cube" /I"%NETNODE_ROOT%net/netseed/ds/ca2" /I"%NETNODE_ROOT%net/netseed/ds/ca2/include" /I"%CA2_ROOT%time/dynamic_source/library/include" /Zi /nologo /Ob2 /Od /W3 /WX- /RTC1 /D "_NETNODE_LIBRARY_LIBRARY" /D "_WINDLL" /D "_DEBUG" /D "_MBCS" /D "MEMDLEAK" /GF- /Gm- /EHa /MDd /GS /fp:precise /Zc:wchar_t /Zc:forScope /GR /openmp-   /Yu"framework.h" /Fp"%CA2_ROOT%time/intermediate/%PLATFORM%/netnode_dynamic_source_library/netnode_dynamic_source_library.pch" /Fa"%CA2_ROOT%time/intermediate/%PLATFORM%/netnode_dynamic_source_library/%ITEM_DIR%" /Fo"%CA2_ROOT%time/intermediate/%PLATFORM%/netnode_dynamic_source_library/%ITEM_DIR%" /Fd"%CA2_ROOT%time/intermediate/%PLATFORM%/netnode_dynamic_source_library/vc110.pdb" /Gd /analyze-  /errorReport:queue   /Zm384 /bigobj /c "%CA2_ROOT%time/dynamic_source/%ITEM_NAME%.cpp" > "%CA2_ROOT%time/dynamic_source/%ITEM_NAME%-compile-log.txt" 
