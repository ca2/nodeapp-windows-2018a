link.exe /ERRORREPORT:PROMPT /OUT:"%TARGET_PATH%.dll" /INCREMENTAL:NO /NOLOGO /LIBPATH:C:\netnode\nodeapp\seed\metaseed\..\..\..\time\x64\basis\ /LIBPATH:C:\netnode\nodeapp\seed\metaseed\../../../nodeapp/thirdparty/library/x64/basis kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib "_ace.lib" "ace.lib" "aura.lib" "sqlite.lib"  "axis.lib" "bzip2.lib" "libcrypto.lib" "axisfreeimage.lib" "mysql.lib" "libssl.lib" "zlib.lib" "base.lib" "core.lib" "data.lib" "pcre.lib" "netnode.lib" "programming.lib" "time.lib" "userdatetime.lib" "app_core_alarm.lib" "aqua.lib" /MANIFEST:NO /DEBUG /PDB:"%HMH_LCTVWILD_PDB_PATH%.pdb" /SUBSYSTEM:WINDOWS /TLBID:1 /DYNAMICBASE /FIXED:NO /NXCOMPAT /MACHINE:X64 /DLL "%CA2_ROOT%time/intermediate/%PLATFORM%/%CONFIGURATION%/netnode_dynamic_source_script/%ITEM_NAME%/%ITEM_TITLE%.obj" "C:/netnode/time/intermediate/x64/basis/netnode_dynamic_source_script/netnode_dynamic_source_script_framework.obj"