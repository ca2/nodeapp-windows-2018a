chcp 65001
call "%VS_VARS%" %VS_VARS_PLAT2%



cl /MP /GS /Qpar- /W3 /Gy- /Zc:wchar_t /I"%CA2_ROOT%include" /I"%CA2_ROOT%app/include" /I"." /I"%CA2_ROOT%app-core/appseed/netnode_dynamic_source_script/framework" /I"%CA2_ROOT%" /I"%CA2_ROOT%app" /I"%CA2_ROOT%app/base/include" /I"%CA2_ROOT%app/base/appseed" /I"%CA2_ROOT%app/appseed" /I"%CA2_ROOT%app/appseed/core" /I"%CA2_ROOT%app/appseed/base" /I"%CA2_ROOT%app/appseed/axis" /I"%CA2_ROOT%app/appseed/aura" /I"%CA2_ROOT%app/axis" /I"%CA2_ROOT%nodeapp/thirdparty/appseed" /I"%CA2_ROOT%nodeapp/seed/metaseed/../../" /I"%CA2_ROOT%app-core/appseed/netnode_dynamic_source_script/../../appseed" /I"%CA2_ROOT%app-core/appseed/netnode_dynamic_source_script/../../" /I"%CA2_ROOT%app-core/appseed/netnode_dynamic_source_script/../" /I"%CA2_ROOT%app-core/appseed/netnode_dynamic_source_script/" /I"%CA2_ROOT%app-core/appseed/netnode_dynamic_source_script/" /I"%NETNODE_ROOT%net/netseed" /I"%NETNODE_ROOT%net/netseed/include" /I"%CA2_ROOT%time/dynamic_source/library/include" /Zi /Gm- /Od /Ob0 /sdl /Fd"%CA2_ROOT%time/intermediate/%PLATFORM%/%CONFIGURATION%/netnode_dynamic_source_script/%ITEM_NAME%/%SDK1%.pdb" /Zc:inline /fp:fast /D "_DYNAMIC_SOURCE_SCRIPT_LIBRARY" /D "_WINDLL" /D "_MBCS" /fp:except- /errorReport:prompt /GF- /WX- /Zc:forScope /RTC1 /GR /Gd /Oy- /MDd /openmp- /Fa"%CA2_ROOT%time/intermediate/%PLATFORM%/%CONFIGURATION%/netnode_dynamic_source_script/%ITEM_NAME%/" /EHa /nologo /Fo"%CA2_ROOT%time/intermediate/%PLATFORM%/%CONFIGURATION%/netnode_dynamic_source_script/%ITEM_NAME%/" /Zm384 /bigobj /c "%CA2_ROOT%time/dynamic_source/%ITEM_NAME%.cpp" > "%CA2_ROOT%time/dynamic_source/%ITEM_NAME%-compile-log.txt" 


link /OUT:"%TARGET_PATH%.dll" /MANIFEST:NO /NXCOMPAT /PDB:"%TARGET_PATH%.pdb" /DYNAMICBASE "aura.lib" "auracharguess.lib"  "aurasqlite.lib"  "axis.lib" "axisbzip2.lib" "axiscrypto.lib" "axisfreeimage.lib" "axisidn.lib" "axismysql.lib" "axisopenssl.lib" "axiszlib.lib" "base.lib" "core.lib" "data.lib" "html.lib" "html_lite.lib" "math.lib" "netnode.lib" "programming.lib" "time.lib" "userdatetime.lib" "app_core_alarm.lib" "aqua.lib" /DEF:"%CA2_ROOT%app-core/appseed/netnode_dynamic_source_script/netnode_dynamic_source_script.def" /DEBUG /FIXED:NO /DLL /MACHINE:X64 /INCREMENTAL:NO /SUBSYSTEM:WINDOWS  /ERRORREPORT:PROMPT /NOLOGO /LIBPATH:"%CA2_ROOT%time/library/x64/basis" /LIBPATH:"C:/Program Files (x86)/Microsoft Visual Studio 14.0/VC/lib/amd64" /LIBPATH:"C:/Program Files (x86)/Microsoft Visual Studio 14.0/VC/atlmfc/lib/amd64" /LIBPATH:"C:/Program Files (x86)/Windows Kits/10/lib/10.0.10150.0/ucrt/x64" /LIBPATH:"C:/Program Files (x86)/Windows Kits/8.1/lib/winv6.3/um/x64" /LIBPATH:"C:/Program Files (x86)/Windows Kits/NETFXSDK/4.6/Lib/um/x64" /LIBPATH:"%CA2_ROOT%time/x64/basis" /LIBPATH:"%CA2_ROOT%nodeapp/thirdparty/library/x64/basis" /TLBID:1    "%CA2_ROOT%time/intermediate/%PLATFORM%/%CONFIGURATION%/netnode_dynamic_source_script/%ITEM_NAME%/%ITEM_TITLE%.obj" > "%CA2_ROOT%time/dynamic_source/%ITEM_NAME%-link-log.txt"



