@echo off
echo 打包 WPOSystem

rem 删除目录
rmdir /s /Q WPOSystem

rem 创建目录
md WPOSystem

rem 打包UpperComputer
xcopy /y /s /f config\UpperComputerConfig.ini 	WPOSystem\WPOUpperComputer\config\
xcopy /y /s /f Release\WPOUpperComputer.exe 	WPOSystem\WPOUpperComputer\
xcopy /y /s /f ..\3rdParty\libcurl\bin\* 					WPOSystem\WPOUpperComputer\
xcopy /y /s /f ..\3rdParty\Qwt\bin\qwt.dll 				WPOSystem\WPOUpperComputer\
xcopy /y /s /f qt\* 												WPOSystem\WPOUpperComputer\
rem 拷贝汉王手写板控件
xcopy /y /s /f hw\*.ocx 			WPOSystem\WPOUpperComputer\
xcopy /y /s /f Release\WPOzHW.exe 	WPOSystem\WPOUpperComputer\
rem D:\Qt\Qt5.6.3\5.6.3\msvc2013\bin\windeployqt.exe 	WPOSystem\WPOUpperComputer\WPOUpperComputer.exe

rem 打包Coordinator
xcopy /y /s /f config\CoordinatorConfig.ini 		WPOSystem\WPOCoordinator\config\
xcopy /y /s /f Release\WPOCoordinator.exe 		WPOSystem\WPOCoordinator\
xcopy /y /s /f qt\* 											WPOSystem\WPOCoordinator\

rem 拷贝PDB文件
xcopy /y /s /f Release\*.pdb 			WPOSystem\PDB\



pause 