call "%VS_VARS%"

link /OUT:"%CA2_ROOT%app\seed\metaseed\..\..\..\stage\%PLATFORM%\dynamic_source\%ITEM_NAME%.dll" /INCREMENTAL /NOLOGO /LIBPATH:"%CA2_ROOT%time/library/x64/%CONFIGURATION_NAME%/" /LIBPATH:"%CA2_ROOT%nodeapp/thirdparty/library/x64" /LIBPATH:"C:\Program Files (x86)\Microsoft Visual Studio 11.0\VC\lib\amd64" /LIBPATH:"C:\Program Files (x86)\Windows Kits\8.0\Lib\win8\um\x64" /DLL "_ca2zlib.lib" "_ca2freeimage.lib" "_ca2openssl.lib" "_ca2freetype.lib"  "kernel32.lib" "user32.lib" "gdi32.lib" "winspool.lib" "comdlg32.lib" "advapi32.lib" "shell32.lib" "ole32.lib" "oleaut32.lib" "uuid.lib" "odbc32.lib" "odbccp32.lib" "_c.lib" "ca.lib" "ca2.lib" "%CA2_ROOT%time/library/x64/%CONFIGURATION_NAME%/app_core_netnodelite.lib"  "%CA2_ROOT%time/library/x64/%CONFIGURATION_NAME%/os.lib" /DEF:"%CA2_ROOT%app-core/appseed/netnodelite_library/netnodelite_library.def" /MANIFEST:NO /DEBUG /PDB:"%TARGET_PATH%.pdb" /SUBSYSTEM:WINDOWS   /TLBID:1 /DYNAMICBASE:NO /NXCOMPAT /IMPLIB:"%CA2_ROOT%app\seed\metaseed\../../../time/library/%PLATFORM%/%ITEM_NAME%.lib" /MACHINE:X64 /ERRORREPORT:QUEUE  "%CA2_ROOT%time\intermediate\%PLATFORM%\netnodelite_library\framework.obj" "%CA2_ROOT%time\intermediate\%PLATFORM%\netnodelite_library\netnodelite_library.obj" %OBJS%  > "%CA2_ROOT%time/dynamic_source/%ITEM_NAME%-link-log.txt"
