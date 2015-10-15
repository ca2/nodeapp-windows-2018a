@echo off
mkdir ..\..\..\time\iexca2\
mkdir ..\..\..\time\iexca2\x86
del ..\..\..\time\iexca2\x86\iexca2.cab
del ..\..\..\time\iexca2\x86\iexca2.dll
del ..\..\..\time\iexca2\x86\aqua.dll
del ..\..\..\time\iexca2\x86\aura.dll
del ..\..\..\time\iexca2\x86\aurasqlite.dll
del ..\..\..\time\iexca2\x86\axis.dll
del ..\..\..\time\iexca2\x86\axisbzip2.dll
del ..\..\..\time\iexca2\x86\axisidn.dll
del ..\..\..\time\iexca2\x86\axisopenssl.dll
del ..\..\..\time\iexca2\x86\axiszlib.dll
del ..\..\..\time\iexca2\x86\base.dll
del ..\..\..\time\iexca2\x86\msvcp120.dll
del ..\..\..\time\iexca2\x86\msvcr120.dll
del ..\..\..\time\iexca2\x86\vcomp120.dll
del ..\..\..\time\iexca2\x86\draw2d_gdiplus.dll
del ..\..\..\time\iexca2\x86\app.install.exe
copy ..\..\..\time\Win32\stage\iexca2.dll ..\..\..\time\iexca2\x86\iexca2.dll
copy ..\..\..\time\Win32\stage\aqua.dll ..\..\..\time\iexca2\x86\aqua.dll
copy ..\..\..\time\Win32\stage\aura.dll ..\..\..\time\iexca2\x86\aura.dll
copy ..\..\..\time\Win32\stage\aurasqlite.dll ..\..\..\time\iexca2\x86\aurasqlite.dll
copy ..\..\..\time\Win32\stage\axis.dll ..\..\..\time\iexca2\x86\axis.dll
copy ..\..\..\time\Win32\stage\axisbzip2.dll ..\..\..\time\iexca2\x86\axisbzip2.dll
copy ..\..\..\time\Win32\stage\axisidn.dll ..\..\..\time\iexca2\x86\axisidn.dll
copy ..\..\..\time\Win32\stage\axisopenssl.dll ..\..\..\time\iexca2\x86\axisopenssl.dll
copy ..\..\..\time\Win32\stage\axiszlib.dll ..\..\..\time\iexca2\x86\axiszlib.dll
copy ..\..\..\time\Win32\stage\base.dll ..\..\..\time\iexca2\x86\base.dll
copy ..\..\..\time\Win32\stage\vcruntime140.dll ..\..\..\time\iexca2\x86\vcruntime140.dll
copy ..\..\..\time\Win32\stage\vcomp140.dll ..\..\..\time\iexca2\x86\vcomp140.dll
copy ..\..\..\time\Win32\stage\draw2d_gdiplus.dll ..\..\..\time\iexca2\x86\draw2d_gdiplus.dll
copy ..\..\..\time\Win32\stage\app.install.exe ..\..\..\time\iexca2\x86\app.install.exe
..\..\..\nodeapp\thirdparty\binary\signtool sign /f "X:\sensitive\sensitive\certificate\ca2.p12" /p none ..\..\..\time\iexca2\x86\iexca2.dll
..\..\..\nodeapp\thirdparty\binary\signtool sign /f "X:\sensitive\sensitive\certificate\ca2.p12" /p none ..\..\..\time\iexca2\x86\aqua.dll
..\..\..\nodeapp\thirdparty\binary\signtool sign /f "X:\sensitive\sensitive\certificate\ca2.p12" /p none ..\..\..\time\iexca2\x86\aura.dll
..\..\..\nodeapp\thirdparty\binary\signtool sign /f "X:\sensitive\sensitive\certificate\ca2.p12" /p none ..\..\..\time\iexca2\x86\aurasqlite.dll
..\..\..\nodeapp\thirdparty\binary\signtool sign /f "X:\sensitive\sensitive\certificate\ca2.p12" /p none ..\..\..\time\iexca2\x86\axis.dll
..\..\..\nodeapp\thirdparty\binary\signtool sign /f "X:\sensitive\sensitive\certificate\ca2.p12" /p none ..\..\..\time\iexca2\x86\axisbzip2.dll
..\..\..\nodeapp\thirdparty\binary\signtool sign /f "X:\sensitive\sensitive\certificate\ca2.p12" /p none ..\..\..\time\iexca2\x86\axisidn.dll
..\..\..\nodeapp\thirdparty\binary\signtool sign /f "X:\sensitive\sensitive\certificate\ca2.p12" /p none ..\..\..\time\iexca2\x86\axisopenssl.dll
..\..\..\nodeapp\thirdparty\binary\signtool sign /f "X:\sensitive\sensitive\certificate\ca2.p12" /p none ..\..\..\time\iexca2\x86\axiszlib.dll
..\..\..\nodeapp\thirdparty\binary\signtool sign /f "X:\sensitive\sensitive\certificate\ca2.p12" /p none ..\..\..\time\iexca2\x86\base.dll
..\..\..\nodeapp\thirdparty\binary\signtool sign /f "X:\sensitive\sensitive\certificate\ca2.p12" /p none ..\..\..\time\iexca2\x86\msvcp120.dll
..\..\..\nodeapp\thirdparty\binary\signtool sign /f "X:\sensitive\sensitive\certificate\ca2.p12" /p none ..\..\..\time\iexca2\x86\msvcr120.dll
..\..\..\nodeapp\thirdparty\binary\signtool sign /f "X:\sensitive\sensitive\certificate\ca2.p12" /p none ..\..\..\time\iexca2\x86\vcomp120.dll
..\..\..\nodeapp\thirdparty\binary\signtool sign /f "X:\sensitive\sensitive\certificate\ca2.p12" /p none ..\..\..\time\iexca2\x86\draw2d_gdiplus.dll
..\..\..\nodeapp\thirdparty\binary\signtool sign /f "X:\sensitive\sensitive\certificate\ca2.p12" /p none ..\..\..\time\iexca2\x86\app.install.exe
IF NOT EXIST ..\..\..\time\iexca2\x86\iexca2.cab ECHO Deleted iexca2.cab
..\..\..\nodeapp\thirdparty\binary\cabarc N ..\..\..\time\iexca2\x86\iexca2.cab ..\..\..\time\iexca2\x86\iexca2.inf ..\..\..\time\iexca2\x86\vcruntime140.dll ..\..\..\time\iexca2\x86\vcomp140.dll ..\..\..\time\iexca2\x86\draw2d_gdiplus.dll ..\..\..\time\iexca2\x86\aqua.dll ..\..\..\time\iexca2\x86\aura.dll ..\..\..\time\iexca2\x86\aurasqlite.dll ..\..\..\time\iexca2\x86\axis.dll ..\..\..\time\iexca2\x86\axisbzip2.dll ..\..\..\time\iexca2\x86\axisidn.dll ..\..\..\time\iexca2\x86\axisopenssl.dll ..\..\..\time\iexca2\x86\axiszlib.dll ..\..\..\time\iexca2\x86\base.dll ..\..\..\time\iexca2\x86\iexca2.dll ..\..\..\time\iexca2\x86\app.install.exe
IF NOT EXIST ..\..\..\time\iexca2\x86\iexca2.cab ECHO ERROR WAS HAPPEND!!! 
IF EXIST ;.\..\..\time\iexca2\x86\iexca2.cab ECHO Copy was successfully
..\..\..\nodeapp\thirdparty\binary\signtool sign /f "X:\sensitive\sensitive\certificate\ca2.p12" /p none ..\..\..\time\iexca2\x86\iexca2.cab