@echo off
mkdir ..\..\..\time\iexca2\
mkdir ..\..\..\time\iexca2\x64
del ..\..\..\time\iexca2\x64\iexca2.cab
del ..\..\..\time\iexca2\x64\iexca2.dll
del ..\..\..\time\iexca2\x64\aura.dll
del ..\..\..\time\iexca2\x64\aurasqlite.dll
del ..\..\..\time\iexca2\x64\axis.dll
del ..\..\..\time\iexca2\x64\axisbzip2.dll
del ..\..\..\time\iexca2\x64\axisfreeimage.dll
del ..\..\..\time\iexca2\x64\axisfreetype.dll
del ..\..\..\time\iexca2\x64\axisidn.dll
del ..\..\..\time\iexca2\x64\axismysql.dll
del ..\..\..\time\iexca2\x64\axisopenssl.dll
del ..\..\..\time\iexca2\x64\axiszlib.dll
del ..\..\..\time\iexca2\x64\base.dll
del ..\..\..\time\iexca2\x64\msvcp120d.dll
del ..\..\..\time\iexca2\x64\msvcr120d.dll
del ..\..\..\time\iexca2\x64\vcomp120d.dll
del ..\..\..\time\iexca2\x64\draw2d_gdiplus.dll
del ..\..\..\time\iexca2\x64\app.install.exe
copy ..\..\..\stage\x64\iexca2.dll ..\..\..\time\iexca2\x64\iexca2.dll
copy ..\..\..\stage\x64\aura.dll ..\..\..\time\iexca2\x64\aura.dll
copy ..\..\..\stage\x64\aurasqlite.dll ..\..\..\time\iexca2\x64\aurasqlite.dll
copy ..\..\..\stage\x64\axis.dll ..\..\..\time\iexca2\x64\axis.dll
copy ..\..\..\stage\x64\axisbzip2.dll ..\..\..\time\iexca2\x64\axisbzip2.dll
copy ..\..\..\stage\x64\axisfreeimage.dll ..\..\..\time\iexca2\x64\axisfreeimage.dll
copy ..\..\..\stage\x64\axisfreetype.dll ..\..\..\time\iexca2\x64\axisfreetype.dll
copy ..\..\..\stage\x64\axisidn.dll ..\..\..\time\iexca2\x64\axisidn.dll
copy ..\..\..\stage\x64\axismysql.dll ..\..\..\time\iexca2\x64\axismysql.dll
copy ..\..\..\stage\x64\axisopenssl.dll ..\..\..\time\iexca2\x64\axisopenssl.dll
copy ..\..\..\stage\x64\axiszlib.dll ..\..\..\time\iexca2\x64\axiszlib.dll
copy ..\..\..\stage\x64\base.dll ..\..\..\time\iexca2\x64\base.dll
copy ..\..\..\stage\x64\msvcp120d.dll ..\..\..\time\iexca2\x64\msvcp120d.dll
copy ..\..\..\stage\x64\msvcr120d.dll ..\..\..\time\iexca2\x64\msvcr120d.dll
copy ..\..\..\stage\x64\vcomp120d.dll ..\..\..\time\iexca2\x64\vcomp120d.dll
copy ..\..\..\stage\x64\draw2d_gdiplus.dll ..\..\..\time\iexca2\x64\draw2d_gdiplus.dll
copy ..\..\..\stage\x64\app.install.exe ..\..\..\time\iexca2\x64\app.install.exe
..\..\..\nodeapp\thirdparty\binary\signtool sign /f "X:\sensitive\sensitive\certificate\ca2.p12" /p none ..\..\..\time\iexca2\x64\iexca2.dll
..\..\..\nodeapp\thirdparty\binary\signtool sign /f "X:\sensitive\sensitive\certificate\ca2.p12" /p none ..\..\..\time\iexca2\x64\aura.dll
..\..\..\nodeapp\thirdparty\binary\signtool sign /f "X:\sensitive\sensitive\certificate\ca2.p12" /p none ..\..\..\time\iexca2\x64\aurasqlite.dll
..\..\..\nodeapp\thirdparty\binary\signtool sign /f "X:\sensitive\sensitive\certificate\ca2.p12" /p none ..\..\..\time\iexca2\x64\axis.dll
..\..\..\nodeapp\thirdparty\binary\signtool sign /f "X:\sensitive\sensitive\certificate\ca2.p12" /p none ..\..\..\time\iexca2\x64\axisbzip2.dll
..\..\..\nodeapp\thirdparty\binary\signtool sign /f "X:\sensitive\sensitive\certificate\ca2.p12" /p none ..\..\..\time\iexca2\x64\axisfreeimage.dll
..\..\..\nodeapp\thirdparty\binary\signtool sign /f "X:\sensitive\sensitive\certificate\ca2.p12" /p none ..\..\..\time\iexca2\x64\axisfreetype.dll
..\..\..\nodeapp\thirdparty\binary\signtool sign /f "X:\sensitive\sensitive\certificate\ca2.p12" /p none ..\..\..\time\iexca2\x64\axisidn.dll
..\..\..\nodeapp\thirdparty\binary\signtool sign /f "X:\sensitive\sensitive\certificate\ca2.p12" /p none ..\..\..\time\iexca2\x64\axismysql.dll
..\..\..\nodeapp\thirdparty\binary\signtool sign /f "X:\sensitive\sensitive\certificate\ca2.p12" /p none ..\..\..\time\iexca2\x64\axisopenssl.dll
..\..\..\nodeapp\thirdparty\binary\signtool sign /f "X:\sensitive\sensitive\certificate\ca2.p12" /p none ..\..\..\time\iexca2\x64\axiszlib.dll
..\..\..\nodeapp\thirdparty\binary\signtool sign /f "X:\sensitive\sensitive\certificate\ca2.p12" /p none ..\..\..\time\iexca2\x64\base.dll
..\..\..\nodeapp\thirdparty\binary\signtool sign /f "X:\sensitive\sensitive\certificate\ca2.p12" /p none ..\..\..\time\iexca2\x64\msvcp120d.dll
..\..\..\nodeapp\thirdparty\binary\signtool sign /f "X:\sensitive\sensitive\certificate\ca2.p12" /p none ..\..\..\time\iexca2\x64\msvcr120d.dll
..\..\..\nodeapp\thirdparty\binary\signtool sign /f "X:\sensitive\sensitive\certificate\ca2.p12" /p none ..\..\..\time\iexca2\x64\vcomp120d.dll
..\..\..\nodeapp\thirdparty\binary\signtool sign /f "X:\sensitive\sensitive\certificate\ca2.p12" /p none ..\..\..\time\iexca2\x64\draw2d_gdiplus.dll
..\..\..\nodeapp\thirdparty\binary\signtool sign /f "X:\sensitive\sensitive\certificate\ca2.p12" /p none ..\..\..\time\iexca2\x64\app.install.exe
IF NOT EXIST ..\..\..\time\iexca2\x64\iexca2.cab ECHO Deleted iexca2.cab
..\..\..\nodeapp\thirdparty\binary\cabarc N ..\..\..\time\iexca2\x64\iexca2.cab ..\..\..\time\iexca2\x64\iexca2.inf ..\..\..\time\iexca2\x64\draw2d_gdiplus.dll  ..\..\..\time\iexca2\x64\msvcp120d.dll ..\..\..\time\iexca2\x64\msvcr120d.dll ..\..\..\time\iexca2\x64\vcomp120d.dll ..\..\..\time\iexca2\x64\aura.dll ..\..\..\time\iexca2\x64\aurasqlite.dll ..\..\..\time\iexca2\x64\axis.dll ..\..\..\time\iexca2\x64\axisbzip2.dll ..\..\..\time\iexca2\x64\axisfreetype.dll ..\..\..\time\iexca2\x64\axisidn.dll ..\..\..\time\iexca2\x64\axismysql.dll ..\..\..\time\iexca2\x64\axisopenssl.dll ..\..\..\time\iexca2\x64\axiszlib.dll ..\..\..\time\iexca2\x64\axis.dll ..\..\..\time\iexca2\x64\base.dll ..\..\..\time\iexca2\x64\iexca2.dll ..\..\..\time\iexca2\x64\app.install.exe
IF NOT EXIST ..\..\..\time\iexca2\x64\iexca2.cab ECHO ERROR WAS HAPPEND!!! 
IF EXIST ;.\..\..\time\iexca2\x64\iexca2.cab ECHO Copy was successfully
..\..\..\nodeapp\thirdparty\binary\signtool sign /f "X:\sensitive\sensitive\certificate\ca2.p12" /p none ..\..\..\time\iexca2\x64\iexca2.cab