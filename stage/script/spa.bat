@echo off
..\..\..\nodeapp\thirdparty\binary\signtool sign /f "X:\sensitive\sensitive\certificate\ca2.p12" /p none ..\..\..\stage\x86\spa.exe
..\..\..\nodeapp\thirdparty\binary\signtool sign /f "X:\sensitive\sensitive\certificate\ca2.p12" /p none ..\..\..\stage\x86\spaadmin.exe
copy ..\..\..\stage\x86\spa.exe Z:\netnodenet\net\netseed\front\cc\ca2\_std\_std\exe\spa.exe
copy ..\..\..\stage\x86\spaadmin.exe Z:\netnodenet\net\netseed\front\cc\ca2\_std\_std\exe\spaadmin.exe
