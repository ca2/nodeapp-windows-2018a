@echo going to production folder
Z:
cd \production\stage\x64\
app.exe : app=nodeapp/production start_deferred=1 base_dir=Z:\stage version=stage build_number=basis