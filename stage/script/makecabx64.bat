@echo off
mkdir ..\..\..\time\iexca2\
mkdir ..\..\..\time\iexca2\x64
del ..\..\..\time\iexca2\x64\iexca2.cab
del ..\..\..\time\iexca2\x64\iexca2.dll
del ..\..\..\time\iexca2\x64\base.dll
del ..\..\..\time\iexca2\x64\os.dll
del ..\..\..\time\iexca2\x64\draw2d_gdiplus.dll
del ..\..\time\iexca2\x64\app-install.exe
copy ..\..\..\stage\x64\iexca2.dll ..\..\..\time\iexca2\x64\iexca2.dll
copy ..\..\..\stage\x64\base.dll ..\..\..\time\iexca2\x64\base.dll
copy ..\..\..\stage\x64\os.dll ..\..\..\time\iexca2\x64\base.dll
copy ..\..\..\stage\x64\draw2d_gdiplus.dll ..\..\..\time\iexca2\x64\draw2d_gdiplus.dll
copy ..\..\..\stage\x64\app-install.exe ..\..\..\time\iexca2\x64\app-install.exe
..\..\..\nodeapp\thirdparty\binary\signtool sign /f "X:\sensitive\sensitive\certificate\ca2.p12" /p none ..\..\..\time\iexca2\x64\iexca2.dll
..\..\..\nodeapp\thirdparty\binary\signtool sign /f "X:\sensitive\sensitive\certificate\ca2.p12" /p none ..\..\..\time\iexca2\x64\base.dll
..\..\..\nodeapp\thirdparty\binary\signtool sign /f "X:\sensitive\sensitive\certificate\ca2.p12" /p none ..\..\..\time\iexca2\x64\os.dll
..\..\..\nodeapp\thirdparty\binary\signtool sign /f "X:\sensitive\sensitive\certificate\ca2.p12" /p none ..\..\..\time\iexca2\x64\draw2d_gdiplus.dll
..\..\..\nodeapp\thirdparty\binary\signtool sign /f "X:\sensitive\sensitive\certificate\ca2.p12" /p none ..\..\..\time\iexca2\x64\app-install.exe
IF NOT EXIST ..\..\..\time\iexca2\x64\iexca2.cab ECHO Deleted iexca2.cab
..\..\..\nodeapp\thirdparty\binary\cabarc N ..\..\..\time\iexca2\x64\iexca2.cab iexca2.inf ..\..\..\time\iexca2\x64\draw2d_gdiplus.dll ..\..\..\time\iexca2\x64\base.dll ..\..\..\time\iexca2\x64\base.dll ..\..\..\time\iexca2\x64\iexca2.dll ..\..\..\time\iexca2\x64\app-install.exe
IF NOT EXIST ..\..\..\time\iexca2\x64\iexca2.cab ECHO ERROR WAS HAPPEND!!! 
IF EXIST ;.\..\..\time\iexca2\x64\iexca2.cab ECHO Copy was successfully
..\..\..\nodeapp\thirdparty\binary\signtool sign /f "X:\sensitive\sensitive\certificate\ca2.p12" /p none ..\..\..\time\iexca2\x64\iexca2.cab