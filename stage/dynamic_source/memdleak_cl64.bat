call "%VS_VARS%" 

cl /I"." /I"%VOTAGUS_ROOT%app/seed/metaseed/../../" /I"%VOTAGUS_ROOT%app/seed/metaseed/../../thirdparty/include/x64" /I"%VOTAGUS_ROOT%app/seed/metaseed/../../thirdparty/include" /I"%VOTAGUS_ROOT%app/seed/metaseed/../../thirdparty/appseed" /I"%VOTAGUS_ROOT%app/seed/metaseed/../../appseed" /I"%VOTAGUS_ROOT%app/seed/metaseed/../../appseed/ca2" /I"%VOTAGUS_ROOT%app/seed/metaseed/../../os/windows" /I"%VOTAGUS_ROOT%app/seed/metaseed/../../nodeapp/windows/include" /I"%VOTAGUS_ROOT%app/seed/metaseed/../../appseed" /I"%NETNODE_ROOT%net/netseed/ds/ca2" /I"%NETNODE_ROOT%net/netseed/ds/ca2/include" /I"%VOTAGUS_ROOT%time/dynamic_source/library/include" /Zi /nologo /Ob2 /Od /W3 /WX- /RTC1 /D "_NETNODE_SCRIPT_DLL" /D "_WINDLL" /D "_DEBUG" /D "_MBCS" /D "MEMDLEAK" /GF- /Gm- /EHa /MDd /GS /fp:precise /Zc:wchar_t /Zc:forScope /GR /openmp-   /Yu"framework.h" /Fp"%VOTAGUS_ROOT%time/intermediate/%PLATFORM%/netnodelite_script/%ITEM_NAME%/netnodelite_script.pch" /Fa"%VOTAGUS_ROOT%time/intermediate/%PLATFORM%/netnodelite_script/%ITEM_NAME%/" /Fo"%VOTAGUS_ROOT%time/intermediate/%PLATFORM%/netnodelite_script/%ITEM_NAME%/" /Fd"%VOTAGUS_ROOT%time/intermediate/%PLATFORM%/netnodelite_script/%ITEM_NAME%/vc110.pdb" /Gd  /errorReport:queue -Zm384 /c  "%VOTAGUS_ROOT%time/dynamic_source/%ITEM_NAME%.cpp" > "%VOTAGUS_ROOT%time/dynamic_source/%ITEM_NAME%-compile-log.txt" 

link /OUT:"%TARGET_PATH%.dll" /INCREMENTAL /NOLOGO /LIBPATH:"%VOTAGUS_ROOT%time/library/x64/%CONFIGURATION_NAME%/" /LIBPATH:"%VOTAGUS_ROOT%nodeapp/thirdparty/library/x64/" /LIBPATH:"C:\Program Files (x86)\Microsoft Visual Studio 11.0\VC\lib\amd64" /LIBPATH:"C:\Program Files (x86)\Windows Kits\8.0\Lib\win8\um\x64" /DLL "c.lib" "ca.lib" "app_core_netnodelite.lib" "libeay32.lib" "freeimage.lib" "kernel32.lib" "user32.lib" "gdi32.lib" "winspool.lib" "comdlg32.lib" "advapi32.lib" "shell32.lib" "ole32.lib" "oleaut32.lib" "uuid.lib" "odbc32.lib" "odbccp32.lib" %LIBS_LIBS% /MANIFEST:NO /DEF:"%VOTAGUS_ROOT%app-core/appseed/netnodelite_script/netnodelite_script.def" /DEBUG /PDB:"%TARGET_PATH%.pdb" /SUBSYSTEM:WINDOWS   /TLBID:1 /DYNAMICBASE:NO /NXCOMPAT /MACHINE:X64 /ERRORREPORT:QUEUE  "%VOTAGUS_ROOT%time/intermediate/%PLATFORM%/netnodelite_script/framework.obj" "%VOTAGUS_ROOT%time/intermediate/%PLATFORM%/netnodelite_script/netnodelite_script.obj" "%VOTAGUS_ROOT%time/intermediate/%PLATFORM%/netnodelite_script/%ITEM_NAME%/%ITEM_TITLE%.obj" > "%VOTAGUS_ROOT%time/dynamic_source/%ITEM_NAME%-link-log.txt"
