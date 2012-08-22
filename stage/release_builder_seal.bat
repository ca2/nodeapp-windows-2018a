type V:\stage\app\stage\build_folder_pp_comment.txt > V:\stage\app\stage\release_builder_seal.txt
date /T >> V:\stage\app\stage\release_builder_seal.txt
time /T >> V:\stage\app\stage\release_builder_seal.txt
type V:\stage\app\stage\build_machine_pp_comment.txt >> V:\stage\app\stage\release_builder_seal.txt
type G:\cecyn1.at.hotmail.com\carlos\dedicaverse.txt >> V:\stage\app\stage\release_builder_seal.txt
svn commit --force-log --encoding utf-8 --file=V:\stage\app\stage\release_builder_seal.txt V:\stage\nodeapp