call "%VS_VARS%" 

cl /c /I. /I%CA2_ROOT%nodeapp\seed\metaseed\../../../ /I%CA2_ROOT%nodeapp\seed\metaseed\../../../app /I%CA2_ROOT%nodeapp\seed\metaseed\../../../app/thirdparty/include /I%CA2_ROOT%nodeapp\seed\metaseed\../../../app/thirdparty/appseed /I%CA2_ROOT%nodeapp\seed\metaseed\../../../app/appseed /I%CA2_ROOT%nodeapp\seed\metaseed\../../../app/appseed/ca2 /I%CA2_ROOT%nodeapp\seed\metaseed\../../../nodeapp/thirdparty/appseed /I%CA2_ROOT%nodeapp\seed\metaseed\../../ /I"%CA2_ROOT%app-core\appseed\netnode_dynamic_source_library\../../appseed" /I"%CA2_ROOT%app-core\appseed\netnode_dynamic_source_library\../../" /I"%CA2_ROOT%app-core\appseed\netnode_dynamic_source_library\../" /I"%CA2_ROOT%app-core\appseed\netnode_dynamic_source_library\\" /Zi /nologo /W3 /WX- /Od /D MEMDLEAK /D _NETNODE_LIBRARY_LIBRARY /D _WINDLL /D _MBCS /GF- /Gm- /EHa /RTC1 /MDd /GS /fp:precise /Zc:wchar_t /Zc:forScope /GR /openmp- /Yu"framework.h" /Fp"%CA2_ROOT%NODEAPP\SEED\METASEED\..\..\..\TIME\INTERMEDIATE\X64\NETNODE_DYNAMIC_SOURCE_LIBRARY\NETNODE_DYNAMIC_SOURCE_LIBRARY.PCH" /Fo"%CA2_ROOT%NODEAPP\SEED\METASEED\..\..\..\TIME\INTERMEDIATE\X64\NETNODE_DYNAMIC_SOURCE_LIBRARY\%ITEM_DIR%" /Fd"%CA2_ROOT%NODEAPP\SEED\METASEED\..\..\..\TIME\INTERMEDIATE\X64\NETNODE_DYNAMIC_SOURCE_LIBRARY\VC120.PDB" /Gd /TP /FC -Zm384 /bigobj %CA2_ROOT%time/dynamic_source/%ITEM_NAME%.cpp" > "%CA2_ROOT%time/dynamic_source/%ITEM_NAME%-compile-log.txt" 
