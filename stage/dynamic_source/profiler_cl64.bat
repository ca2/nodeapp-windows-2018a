
chcp 65001
call "%VS_VARS%" 
chcp 65001
cl /I"." /I"%CA2_ROOT%app/seed/metaseed/../../" /I"%CA2_ROOT%app/seed/metaseed/../../thirdparty/include/x64" /I"%CA2_ROOT%app/seed/metaseed/../../thirdparty/include" /I"%CA2_ROOT%app/seed/metaseed/../../thirdparty/appseed" /I"%CA2_ROOT%app/seed/metaseed/../../appseed" /I"%CA2_ROOT%app/seed/metaseed/../../appseed/ca2" /I"%CA2_ROOT%" /I"%CA2_ROOT%app/seed/metaseed/../../os/windows" /I"%CA2_ROOT%app/seed/metaseed/../../nodeapp/windows/include" /I"%CA2_ROOT%app/seed/metaseed/../../appseed" /I"%NETNODE_ROOT%net/netseed" /I"%NETNODE_ROOT%net/netseed/include" /I"%CA2_ROOT%time/dynamic_source/library/include" /Zi /nologo /O2 /Ob2 /Oi /Ot /Oy /W3 /WX- /D "_DYNAMIC_SOURCE_SCRIPT_LIBRARY" /D "_DEBUG" /D "_WINDLL" /D "_MBCS" /GF- /Gm- /EHa /MD /GS- /fp:fast /fp:except- /D__AVX__ /D_NO_PREFETCHW /Zc:wchar_t /Zc:forScope /GR /openmp-   /Yu"framework.h" /Fp"%CA2_ROOT%time/intermediate/%PLATFORM%/netnode_dynamic_source_script/%ITEM_NAME%/netnode_dynamic_source_script.pch" /Fa"%CA2_ROOT%time/intermediate/%PLATFORM%/netnode_dynamic_source_script/%ITEM_NAME%/" /Fo"%CA2_ROOT%time/intermediate/%PLATFORM%/netnode_dynamic_source_script/%ITEM_NAME%/" /Fd"%CA2_ROOT%time/intermediate/%PLATFORM%/netnode_dynamic_source_script/%ITEM_NAME%/vc110.pdb" /Gd  /errorReport:queue -Zm384 /c  "%CA2_ROOT%time/dynamic_source/%ITEM_NAME%.cpp" > "%CA2_ROOT%time/dynamic_source/%ITEM_NAME%-compile-log.txt" 
chcp 65001
link /OUT:"%TARGET_PATH%.dll" /INCREMENTAL /NOLOGO /LIBPATH:"%CA2_ROOT%time/library/x64/%CONFIGURATION_NAME%/" /LIBPATH:"%CA2_ROOT%nodeapp/thirdparty/library/x64/" /LIBPATH:"C:\Program Files (x86)\Microsoft Visual Studio 11.0\VC\lib\amd64" /LIBPATH:"C:\Program Files (x86)\Windows Kits\8.0\Lib\win8\um\x64" /DLL  "basezlib.lib" "basefreeimage.lib" "baseopenssl.lib" "basefreetype.lib" "base.lib" "boot.lib" "core.lib" "netnode.lib"  "os.lib" "kernel32.lib" "user32.lib" "gdi32.lib" "winspool.lib" "comdlg32.lib" "advapi32.lib" "shell32.lib" "ole32.lib" "oleaut32.lib" "uuid.lib" "odbc32.lib" "odbccp32.lib" %LIBS_LIBS% /MANIFEST:NO /DEF:"%CA2_ROOT%app-core/appseed/netnode_dynamic_source_script/netnode_dynamic_source_script.def" /DEBUG /PDB:"%TARGET_PATH%.pdb" /SUBSYSTEM:WINDOWS   /TLBID:1 /DYNAMICBASE:NO /NXCOMPAT /MACHINE:X64 /ERRORREPORT:QUEUE  "%CA2_ROOT%time/intermediate/%PLATFORM%/netnode_dynamic_source_script/framework.obj" "%CA2_ROOT%time/intermediate/%PLATFORM%/netnode_dynamic_source_script/netnode_dynamic_source_script.obj" "%CA2_ROOT%time/intermediate/%PLATFORM%/netnode_dynamic_source_script/%ITEM_NAME%/%ITEM_TITLE%.obj" > "%CA2_ROOT%time/dynamic_source/%ITEM_NAME%-link-log.txt"
