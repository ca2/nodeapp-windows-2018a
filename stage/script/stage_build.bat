call "%VS100COMNTOOLS%\..\..\VC\bin\vcvars32.bat"

VCExpress "T:\stage\app\seed\metaseed\seed_all.sln" /build basis
VCExpress "T:\stage\app\seed\metaseed\seed_all64.sln" /build basis


