call "%VS120COMNTOOLS%\..\..\VC\bin\vcvars32.bat"

devenv "Z:\stage\ca2\seed\metaseed\all.sln" /build stage
devenv "Z:\stage\ca2\seed\metaseed\all64.sln" /build stage


