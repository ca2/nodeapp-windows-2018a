chcp 65001
call "%VS_VARS%" %VS_VARS_PLAT2%


link /OUT:"%CA2_ROOT%app\seed\metaseed\..\..\..\%CONFIGURATION%\%PLATFORM%\dynamic_source\%ITEM_NAME%.dll" /MANIFEST:NO /PROFILE /LTCG /NXCOMPAT /PDB:"%TARGET_PATH%..pdb" /DYNAMICBASE "kernel32.lib" "user32.lib" "gdi32.lib" "winspool.lib" "comdlg32.lib" "advapi32.lib" "shell32.lib" "ole32.lib" "oleaut32.lib" "uuid.lib" "odbc32.lib" "odbccp32.lib" "aura.lib" "axis.lib" "data.lib" "programming.lib" "netnode.lib" /DEF:"%CA2_ROOT%app-core/appseed/netnode_dynamic_source_library/netnode_dynamic_source_library.def" /FIXED:NO /IMPLIB:"%CA2_ROOT%app\seed\metaseed\../../../time/library/%PLATFORM%/%ITEM_NAME%.lib" /DEBUG /DLL /MACHINE:X64 /INCREMENTAL:NO  /SUBSYSTEM:WINDOWS /MANIFEST:NO /ERRORREPORT:PROMPT /NOLOGO /LIBPATH:"C:\netnode\nodeapp\seed\metaseed\../../../time/library/x64/profiler" /LIBPATH:"%CA2_ROOT%time/library/x64/%CONFIGURATION_NAME%/" /TLBID:1   "%CA2_ROOT%time\intermediate\%PLATFORM%\%CONFIGURATION%\netnode_dynamic_source_library\framework.obj" "%CA2_ROOT%time\intermediate\%PLATFORM%\%CONFIGURATION%\netnode_dynamic_source_library\netnode_dynamic_source_library.obj" %OBJS%  > "%CA2_ROOT%time/dynamic_source/%ITEM_NAME%-link-log.txt"


