chcp 65001
call "%VS_VARS%" %VS_VARS_PLAT2%



cl /MP /GS /Qpar- /W3 /Gy- /Zc:wchar_t /I"%CA2_ROOT%include" /I"%CA2_ROOT%nodeapp/x64/include" /I"%CA2_ROOT%nodeapp/include" /I"%CA2_ROOT%app/include" /I"." /I"%CA2_ROOT%app-core/appseed/netnode_dynamic_source_script/framework" /I"%CA2_ROOT%" /I"%CA2_ROOT%app" /I"%CA2_ROOT%app/base/include" /I"%CA2_ROOT%app/base/appseed" /I"%CA2_ROOT%app/appseed" /I"%CA2_ROOT%app/appseed/core" /I"%CA2_ROOT%app/appseed/base" /I"%CA2_ROOT%app/appseed/axis" /I"%CA2_ROOT%app/appseed/aura" /I"%CA2_ROOT%app/axis" /I"%CA2_ROOT%nodeapp/thirdparty/appseed" /I"%CA2_ROOT%nodeapp/seed/metaseed/../../" /I"%CA2_ROOT%app-core/appseed/netnode_dynamic_source_script/../../appseed" /I"%CA2_ROOT%app-core/appseed/netnode_dynamic_source_script/../../" /I"%CA2_ROOT%app-core/appseed/netnode_dynamic_source_script/../" /I"%CA2_ROOT%app-core/appseed/netnode_dynamic_source_script/" /I"%CA2_ROOT%app-core/appseed/netnode_dynamic_source_script/" /I"%NETNODE_ROOT%net/netseed" /I"%NETNODE_ROOT%net/netseed/include" /I"%CA2_ROOT%time/dynamic_source/library/include" /Zi /Gm- /Od /Ob0 /sdl /Fd"%CA2_ROOT%time/intermediate/%PLATFORM%/%CONFIGURATION%/netnode_dynamic_source_script/%ITEM_NAME%/%SDK1%.pdb" /Zc:inline /fp:fast /D "_DYNAMIC_SOURCE_SCRIPT_LIBRARY" /D "_WINDLL" /D "_MBCS" /fp:except- /errorReport:prompt /GF- /WX- /Zc:forScope /RTC1 /GR /Gd /Oy- /MDd /openmp- /Fa"%CA2_ROOT%time/intermediate/%PLATFORM%/%CONFIGURATION%/netnode_dynamic_source_script/%ITEM_NAME%/" /EHa /nologo /Fo"%CA2_ROOT%time/intermediate/%PLATFORM%/%CONFIGURATION%/netnode_dynamic_source_script/%ITEM_NAME%/" /Zm384 /bigobj /c "%CA2_ROOT%time/dynamic_source/%ITEM_NAME%.cpp" > "%CA2_ROOT%time/dynamic_source/%ITEM_NAME%-compile-log.txt" 




