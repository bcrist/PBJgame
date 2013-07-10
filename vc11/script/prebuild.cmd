@echo off
pushd %1lib
%1script\md5sum.exe --status -c %1script\md5.txt
if errorlevel 1 (
echo Asset Import library out of date!
del assimpd.pdb assimps.lib assimpsd.lib
%1script\wget.exe --no-check-certificate -nv -O assimpd.pdb https://www.dropbox.com/s/dcx7q76ob6nb7qh/assimpd.pdb?dl=1
%1script\wget.exe --no-check-certificate -nv -O assimps.lib https://www.dropbox.com/s/yc0ccee31g44lol/assimps.lib?dl=1
%1script\wget.exe --no-check-certificate -nv -O assimpsd.lib https://www.dropbox.com/s/x8ybpdxodi2ktq1/assimpsd.lib?dl=1
)
popd