copy ccvotagus\Release\spa.exe ca2sp\spa.exe /y
copy ccvotagus\Release\spaadmin.exe ca2sp\spaadmin.exe /y
svn commit --force-log --file ..\ccvotagus\votagus_dedication\000-000-000-001-002.txt ca2sp\spa.exe ca2sp\spaadmin.exe
