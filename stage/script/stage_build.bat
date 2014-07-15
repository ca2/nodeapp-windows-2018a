call "%VS120COMNTOOLS%\..\..\VC\bin\vcvars32.bat"

devenv "Z:\stage\cgcl\seed\metaseed\all.sln" /build basis
devenv "Z:\stage\cgcl\seed\metaseed\all64.sln" /build basis


