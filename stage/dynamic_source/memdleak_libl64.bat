call "%VS_VARS%"

link /OUT:"%VOTAGUS_ROOT%app\seed\metaseed\..\..\..\stage\%PLATFORM%\dynamic_source\%ITEM_NAME%.dll" /INCREMENTAL /NOLOGO /LIBPATH:"%VOTAGUS_ROOT%time/library/x64/%CONFIGURATION_NAME%/" /LIBPATH:"%VOTAGUS_ROOT%nodeapp/thirdparty/library/x64" /LIBPATH:"C:\Program Files (x86)\Microsoft Visual Studio 11.0\VC\lib\amd64" /LIBPATH:"C:\Program Files (x86)\Windows Kits\8.0\Lib\win8\um\x64" /DLL "freeimage.lib" "kernel32.lib" "user32.lib" "gdi32.lib" "winspool.lib" "comdlg32.lib" "advapi32.lib" "shell32.lib" "ole32.lib" "oleaut32.lib" "uuid.lib" "odbc32.lib" "odbccp32.lib" "%VOTAGUS_ROOT%time/library/x64/%CONFIGURATION_NAME%/c.lib" "%VOTAGUS_ROOT%time/library/x64/%CONFIGURATION_NAME%/ca.lib" "%VOTAGUS_ROOT%time/library/x64/%CONFIGURATION_NAME%/app_core_netnodelite.lib"  "%VOTAGUS_ROOT%time/library/x64/%CONFIGURATION_NAME%/os.lib" /DEF:"%VOTAGUS_ROOT%app-core/appseed/netnodelite_library/netnodelite_library.def" /MANIFEST:NO /ALLOW_ISOLATION /DEBUG /PDB:"%TARGET_PATH%.pdb"  /SUBSYSTEM:WINDOWS   /TLBID:1 /DYNAMICBASE:NO /NXCOMPAT /IMPLIB:"%VOTAGUS_ROOT%app\seed\metaseed\../../../time/library/%PLATFORM%/%ITEM_NAME%.lib" /MACHINE:X64 /ERRORREPORT:QUEUE  "%VOTAGUS_ROOT%time\intermediate\%PLATFORM%\netnodelite_library\framework.obj" "%VOTAGUS_ROOT%time\intermediate\%PLATFORM%\netnodelite_library\netnodelite_library.obj" %OBJS%  > "%VOTAGUS_ROOT%time/dynamic_source/%ITEM_NAME%-link-log.txt"
