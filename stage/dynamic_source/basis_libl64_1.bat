call "%VS_VARS%"  %VS_VARS_PLAT2%

link /OUT:"%CA2_ROOT%app\seed\metaseed\..\..\..\stage\%STAGEPLATFORM%\dynamic_source\%ITEM_NAME%.dll" /INCREMENTAL /NOLOGO /LIBPATH:"%CA2_ROOT%time/library/%PLATFORM%/%CONFIGURATION_NAME%/" /LIBPATH:"%CA2_ROOT%nodeapp/thirdparty/library/%PLATFORM%" /LIBPATH:"C:\Program Files (x86)\Microsoft Visual Studio 11.0\VC\lib\amd64" /LIBPATH:"C:\Program Files (x86)\Windows Kits\8.1\Lib\winv6.3\um\%PLATFORM%" /DLL "aura.lib" "axis.lib" "axisfreeimage.lib" "axisfreetype.lib"  "kernel32.lib" "user32.lib" "gdi32.lib" "winspool.lib" "comdlg32.lib" "advapi32.lib" "shell32.lib" "ole32.lib" "oleaut32.lib" "uuid.lib" "odbc32.lib" "odbccp32.lib" "base.lib" "core.lib" "netnode.lib" /DEF:"%CA2_ROOT%app-core/appseed/netnode_dynamic_source_library/netnode_dynamic_source_library.def" /MANIFEST:NO /ALLOW_ISOLATION /DEBUG /PDB:"%TARGET_PATH%.pdb" /SUBSYSTEM:WINDOWS   /TLBID:1 /DYNAMICBASE:NO /NXCOMPAT /IMPLIB:"%CA2_ROOT%app\seed\metaseed\../../../time/library/%PLATFORM%/%ITEM_NAME%.lib" /MACHINE:X64 /ERRORREPORT:QUEUE  "%CA2_ROOT%time\intermediate\%PLATFORM%\netnode_dynamic_source_library\framework.obj" "%CA2_ROOT%time\intermediate\%PLATFORM%\netnode_dynamic_source_library\netnode_dynamic_source_library.obj" %OBJS%  > "%CA2_ROOT%time/dynamic_source/%ITEM_NAME%-link-log.txt"