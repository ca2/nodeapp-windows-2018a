@echo off
..\..\..\nodeapp\thirdparty\binary\signtool sign /f "C:\sensitive\sensitive\certificate\ca2.p12" /p none ..\..\..\time\Win32\basis\a_spa.exe
..\..\..\nodeapp\thirdparty\binary\signtool sign /f "C:\sensitive\sensitive\certificate\ca2.p12" /p none ..\..\..\time\x64\basis\a_spa.exe
..\..\..\nodeapp\thirdparty\binary\signtool sign /f "C:\sensitive\sensitive\certificate\ca2.p12" /p none ..\..\..\time\Win32\basis\a_spaadmin.exe
..\..\..\nodeapp\thirdparty\binary\signtool sign /f "C:\sensitive\sensitive\certificate\ca2.p12" /p none ..\..\..\time\x64\basis\a_spaadmin.exe
copy ..\..\..\time\Win32\basis\a_spa.exe C:\netnodenet\net\netseed\front\ca2\_std\_std\exe\x86\a_spa.exe
copy ..\..\..\time\x64\basis\a_spa.exe C:\netnodenet\net\netseed\front\ca2\_std\_std\exe\x64\a_spa.exe
copy ..\..\..\time\Win32\basis\a_spaadmin.exe C:\netnodenet\net\netseed\front\ca2\_std\_std\exe\x86\a_spaadmin.exe
copy ..\..\..\time\x64\basis\a_spaadmin.exe C:\netnodenet\net\netseed\front\ca2\_std\_std\exe\x64\a_spaadmin.exe
svn commit -m "new spa" C:\netnodenet\net
