link /OUT:"%TARGET_PATH%.dll" /EXPORT:get_new_library /MANIFEST:NO /NXCOMPAT /PDB:"%HMH_LCTVWILD_PDB_PATH%.pdb" /DYNAMICBASE "_ace.lib" "ace.lib" "aura.lib" "sqlite.lib"  "axis.lib" "bzip2.lib" "crypto.lib" "axisfreeimage.lib" "openssl.lib" "zlib.lib" "base.lib" "core.lib" "programming.lib" "time.lib" "userdatetime.lib" "aqua.lib" /DEBUG /FIXED:NO /DLL /MACHINE:X64 /INCREMENTAL:NO /SUBSYSTEM:WINDOWS  /ERRORREPORT:PROMPT /NOLOGO /LIBPATH:"%CA2_ROOT%time/library/x64/basis" /LIBPATH:"C:/Program Files (x86)/Microsoft Visual Studio 14.0/VC/lib/amd64" /LIBPATH:"C:/Program Files (x86)/Microsoft Visual Studio 14.0/VC/atlmfc/lib/amd64" /LIBPATH:"C:/Program Files (x86)/Windows Kits/10/lib/10.0.10150.0/ucrt/x64" /LIBPATH:"C:/Program Files (x86)/Windows Kits/8.1/lib/winv6.3/um/x64" /LIBPATH:"C:/Program Files (x86)/Windows Kits/NETFXSDK/4.6/Lib/um/x64" /LIBPATH:"%CA2_ROOT%time/x64/basis" /LIBPATH:"%CA2_ROOT%nodeapp/thirdparty/library/x64/basis" /TLBID:1    "%CA2_ROOT%time/intermediate/%PLATFORM%/%CONFIGURATION%/xmpp_resident/%ITEM_NAME%/%ITEM_TITLE%.obj"