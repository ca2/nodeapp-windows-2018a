chcp 65001
call "%VS_VARS%" %VS_VARS_PLAT2%

cl /I"." /I"%CA2_ROOT%app/seed/metaseed/../../" /I"%CA2_ROOT%app/seed/metaseed/../../thirdparty/include/x86" /I"%CA2_ROOT%app/seed/metaseed/../../thirdparty/include" /I"%CA2_ROOT%app/seed/metaseed/../../thirdparty/appseed" /I"%CA2_ROOT%app/seed/metaseed/../../appseed" /I"%CA2_ROOT%app/seed/metaseed/../../appseed/ca2" /I"%CA2_ROOT%" /I"%CA2_ROOT%app/seed/metaseed/../../os/windows" /I"%CA2_ROOT%app/seed/metaseed/../../nodeapp/windows/include" /I"%CA2_ROOT%app/seed/metaseed/../../appseed" /I"%NETNODE_ROOT%net/netseed" /I"%NETNODE_ROOT%net/netseed/include" /I"%CA2_ROOT%time/dynamic_source/library/include" /Zi /nologo /Ob2 /Od /Ob2 /W3 /WX- /RTC1 /D "_DYNAMIC_SOURCE_SCRIPT_LIBRARY" /D "_WINDLL" /D "_DEBUG" /D "_MBCS" /GF- /Gm- /EHa /MDd /GS /sdl /fp:fast /fp:except- /D_NO_PREFETCHW /Zc:wchar_t /Zc:forScope /GR /openmp-   /Yu"framework.h" /Fp"%CA2_ROOT%time/intermediate/%PLATFORM%/%CONFIGURATION%/netnode_dynamic_source_script/%ITEM_NAME%/netnode_dynamic_source_script.pch" /Fa"%CA2_ROOT%time/intermediate/%PLATFORM%/%CONFIGURATION%/netnode_dynamic_source_script/%ITEM_NAME%/" /Fo"%CA2_ROOT%time/intermediate/%PLATFORM%/%CONFIGURATION%/netnode_dynamic_source_script/%ITEM_NAME%/" /Fd"%CA2_ROOT%time/intermediate/%PLATFORM%/%CONFIGURATION%/netnode_dynamic_source_script/%ITEM_NAME%/%SDK1%.pdb" /Gd /Oy- /errorReport:queue -Zm384 /c  "%CA2_ROOT%time/dynamic_source/%ITEM_NAME%.cpp" > "%CA2_ROOT%time/dynamic_source/%ITEM_NAME%-compile-log.txt" 

link /OUT:"%TARGET_PATH%.dll" /INCREMENTAL /NOLOGO /LIBPATH:"%CA2_ROOT%time/library/%STAGEPLATFORM%/%CONFIGURATION_NAME%" /LIBPATH:"%CA2_ROOT%nodeapp/thirdparty/library/%STAGEPLATFORM%" /LIBPATH:"C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\lib" /LIBPATH:"C:\Program Files (x86)\Windows Kits\8.1\Lib\winv6.3\um\x86" /DLL "aura.lib" "axis.lib" "axisfreeimage.lib" "axisfreetype.lib" "base.lib" "core.lib" "netnode.lib" "kernel32.lib" "user32.lib" "gdi32.lib" "winspool.lib" "comdlg32.lib" "advapi32.lib" "shell32.lib" "ole32.lib" "oleaut32.lib" "uuid.lib" "odbc32.lib" "odbccp32.lib" "%LIBS_LIBS%" /MANIFEST:NO /DEF:"%CA2_ROOT%app-core/appseed/netnode_dynamic_source_script/netnode_dynamic_source_script.def" /DEBUG /PDB:"%TARGET_PATH%.pdb" /SUBSYSTEM:WINDOWS   /TLBID:1 /DYNAMICBASE /NXCOMPAT /MACHINE:X86 /ERRORREPORT:QUEUE  "%CA2_ROOT%time/intermediate/%PLATFORM%/%CONFIGURATION%/netnode_dynamic_source_script/framework.obj" "%CA2_ROOT%time/intermediate/%PLATFORM%/%CONFIGURATION%/netnode_dynamic_source_script/netnode_dynamic_source_script.obj" "%CA2_ROOT%time/intermediate/%PLATFORM%/%CONFIGURATION%/netnode_dynamic_source_script/%ITEM_NAME%/%ITEM_TITLE%.obj" > "%CA2_ROOT%time/dynamic_source/%ITEM_NAME%-link-log.txt"