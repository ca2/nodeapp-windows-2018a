type Z:\basis\app\stage\build_folder_pp_comment_se.txt > Z:\basis\nodeapp\stage\release_builder_seal_se.txt
date /T >> Z:\basis\nodeapp\stage\release_builder_seal_se.txt
time /T >> Z:\basis\nodeapp\stage\release_builder_seal_se.txt
type Z:\basis\app\stage\build_machine_pp_comment_se.txt >> Z:\basis\nodeapp\stage\release_builder_seal_se.txt
svn commit --force-log --encoding utf-8 --file=Z:\basis\nodeapp\stage\release_builder_seal_se.txt Z:\basis\nodeapp