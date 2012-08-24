@echo off
mkdir ..\..\..\time\iexca2\
mkdir ..\..\..\time\iexca2\x64
del ..\..\..\time\iexca2\x64\iexca2.cab
IF NOT EXIST ..\..\..\time\iexca2\x64\iexca2.cab ECHO Deleted iexca2.cab
..\..\..\nodeapp\thirdparty\binary\cabarc N ..\..\..\time\iexca2\x64\iexca2.cab iexca2.inf ..\..\..\stage\x64\iexca2.dll
IF NOT EXIST ..\..\..\time\iexca2\x64\iexca2.cab ECHO ERROR WAS HAPPEND!!! 
IF EXIST ..\..\..\time\iexca2\x64\iexca2.cab ECHO Copy was successfully