@echo off
..\..\..\nodeapp\thirdparty\binary\signtool sign /f "X:\sensitive\sensitive\certificate\ca2.p12" /p none ..\..\..\stage\x86\a_spa.exe
..\..\..\nodeapp\thirdparty\binary\signtool sign /f "X:\sensitive\sensitive\certificate\ca2.p12" /p none ..\..\..\stage\x64\a_spa.exe
..\..\..\nodeapp\thirdparty\binary\signtool sign /f "X:\sensitive\sensitive\certificate\ca2.p12" /p none ..\..\..\stage\x86\a_spaadmin.exe
..\..\..\nodeapp\thirdparty\binary\signtool sign /f "X:\sensitive\sensitive\certificate\ca2.p12" /p none ..\..\..\stage\x64\a_spaadmin.exe
copy ..\..\..\stage\x86\a_spa.exe Z:\netnodenet\net\netseed\front\ca2\_std\_std\exe\x86\a_spa.exe
copy ..\..\..\stage\x64\a_spa.exe Z:\netnodenet\net\netseed\front\ca2\_std\_std\exe\x64\a_spa.exe
copy ..\..\..\stage\x86\a_spaadmin.exe Z:\netnodenet\net\netseed\front\ca2\_std\_std\exe\x86\a_spaadmin.exe
copy ..\..\..\stage\x64\a_spaadmin.exe Z:\netnodenet\net\netseed\front\ca2\_std\_std\exe\x64\a_spaadmin.exe
