@echo off
echo 打包 QAmbitionClient

rem 删除目录
rmdir /s /Q QAmbitionClient

rem 创建目录
md QAmbitionClient

rem 打包UpperComputer
xcopy /y /s /f Release\QAmbitionClient.exe 					QAmbitionClient\
xcopy /y /s /f ..\EssentialFile\config\ClientConfig.ini 	QAmbitionClient\config\
rem xcopy /y /s /f ..\3rdParty\libcurl\bin\* 						QAmbitionClient\
rem xcopy /y /s /f qt\* 												QAmbitionClient\
rem D:\Qt\Qt5.6.3\5.6.3\msvc2013\bin\windeployqt.exe 	QAmbitionClient\QAmbitionClient.exe

rem 拷贝PDB文件
xcopy /y /s /f Release\*.pdb 			QAmbitionClient\PDB\

pause 