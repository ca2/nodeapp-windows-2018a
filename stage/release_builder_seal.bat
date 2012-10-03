type Z:\stage\nodeapp\stage\build_folder_pp_comment.txt > Z:\stage\nodeapp\stage\release_builder_seal.txt
date /T >> Z:\stage\nodeapp\stage\release_builder_seal.txt
time /T >> Z:\stage\nodeapp\stage\release_builder_seal.txt
type Z:\stage\nodepp\stage\build_machine_pp_comment.txt >> Z:\stage\nodeapp\stage\release_builder_seal.txt
svn commit --force-log --encoding utf-8 --file=Z:\stage\nodeapp\stage\release_builder_seal.txt Z:\stage\nodeapp