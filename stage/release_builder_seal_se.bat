type C:\basis\app\stage\build_folder_pp_comment_se.txt > C:\basis\nodeapp\stage\release_builder_seal_se.txt
date /T >> C:\basis\nodeapp\stage\release_builder_seal_se.txt
time /T >> C:\basis\nodeapp\stage\release_builder_seal_se.txt
type C:\basis\app\stage\build_machine_pp_comment_se.txt >> C:\basis\nodeapp\stage\release_builder_seal_se.txt
svn commit --force-log --encoding utf-8 --file=C:\basis\nodeapp\stage\release_builder_seal_se.txt C:\basis\nodeapp