@echo off
C:\bergedge\hi5\program\bin\signtool sign /f "C:\sensitive\sensitive\certificate\ca2.p12" /p none ..\..\..\time\Win32\stage\app_app.exe
C:\bergedge\hi5\program\bin\signtool sign /f "C:\sensitive\sensitive\certificate\ca2.p12" /p none ..\..\..\time\x64\stage\app_app.exe
C:\bergedge\hi5\program\bin\signtool sign /f "C:\sensitive\sensitive\certificate\ca2.p12" /p none ..\..\..\time\Win32\stage\app_app_admin.exe
C:\bergedge\hi5\program\bin\signtool sign /f "C:\sensitive\sensitive\certificate\ca2.p12" /p none ..\..\..\time\x64\stage\app_app_admin.exe
C:\bergedge\hi5\program\bin\signtool sign /f "C:\sensitive\sensitive\certificate\ca2.p12" /p none ..\..\..\time\Win32\basis\app_app.exe
C:\bergedge\hi5\program\bin\signtool sign /f "C:\sensitive\sensitive\certificate\ca2.p12" /p none ..\..\..\time\x64\basis\app_app.exe
C:\bergedge\hi5\program\bin\signtool sign /f "C:\sensitive\sensitive\certificate\ca2.p12" /p none ..\..\..\time\Win32\basis\app_app_admin.exe
C:\bergedge\hi5\program\bin\signtool sign /f "C:\sensitive\sensitive\certificate\ca2.p12" /p none ..\..\..\time\x64\basis\app_app_admin.exe
copy ..\..\..\time\Win32\stage\app_app.exe C:\netnodenet\net\netseed\front\ca2\_std\_std\exe\x86\stage\app_app.exe
copy ..\..\..\time\x64\stage\app_app.exe C:\netnodenet\net\netseed\front\ca2\_std\_std\exe\x64\stage\app_app.exe
copy ..\..\..\time\Win32\stage\app_app_admin.exe C:\netnodenet\net\netseed\front\ca2\_std\_std\exe\x86\stage\app_app_admin.exe
copy ..\..\..\time\x64\stage\app_app_admin.exe C:\netnodenet\net\netseed\front\ca2\_std\_std\exe\x64\stage\app_app_admin.exe
copy ..\..\..\time\Win32\basis\app_app.exe C:\netnodenet\net\netseed\front\ca2\_std\_std\exe\x86\basis\app_app.exe
copy ..\..\..\time\x64\basis\app_app.exe C:\netnodenet\net\netseed\front\ca2\_std\_std\exe\x64\basis\app_app.exe
copy ..\..\..\time\Win32\basis\app_app_admin.exe C:\netnodenet\net\netseed\front\ca2\_std\_std\exe\x86\basis\app_app_admin.exe
copy ..\..\..\time\x64\basis\app_app_admin.exe C:\netnodenet\net\netseed\front\ca2\_std\_std\exe\x64\basis\app_app_admin.exe
svn commit -m "new spa" C:\netnodenet\net
