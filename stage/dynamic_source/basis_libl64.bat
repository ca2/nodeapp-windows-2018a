chcp 65001
call "%VS_VARS%" %VS_VARS_PLAT2%





link /OUT:"%TARGET_PATH%.dll" /MANIFEST:NO /NXCOMPAT /PDB:"%TARGET_PATH%.pdb" /DYNAMICBASE "aura.lib" "auracharguess.lib"  "aurasqlite.lib"  "axis.lib" "axisbzip2.lib" "axiscrypto.lib" "axisfreeimage.lib" "axisidn.lib" "axismysql.lib" "axisopenssl.lib" "axiszlib.lib" "base.lib" "core.lib" "data.lib" "html.lib" "html_lite.lib" "math.lib" "netnode.lib" "programming.lib" "time.lib" "userdatetime.lib" "app_core_alarm.lib" "aqua.lib" /DEBUG /FIXED:NO /DLL /MACHINE:X64 /INCREMENTAL:NO /SUBSYSTEM:WINDOWS  /ERRORREPORT:PROMPT /NOLOGO /LIBPATH:"%CA2_ROOT%time/library/x64/basis" /LIBPATH:"C:/Program Files (x86)/Microsoft Visual Studio 14.0/VC/lib/amd64" /LIBPATH:"C:/Program Files (x86)/Microsoft Visual Studio 14.0/VC/atlmfc/lib/amd64" /LIBPATH:"C:/Program Files (x86)/Windows Kits/10/lib/10.0.10150.0/ucrt/x64" /LIBPATH:"C:/Program Files (x86)/Windows Kits/8.1/lib/winv6.3/um/x64" /LIBPATH:"C:/Program Files (x86)/Windows Kits/NETFXSDK/4.6/Lib/um/x64" /LIBPATH:"%CA2_ROOT%time/x64/basis" /LIBPATH:"%CA2_ROOT%nodeapp/thirdparty/library/x64/basis" /TLBID:1    %OBJECTS% > "%CA2_ROOT%time/dynamic_source/%ITEM_NAME%-link-log.txt"



