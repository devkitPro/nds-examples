rem msys can't run without being in the path; so set it here
@set PATH=%DEVKITPRO_WIN%\msys\bin;%PATH%

rem determine the devkitarm 'project name' which is the name of this directory.
for %%* in (.) do set PROJECTNAME=%%~n*

rem BUILD, REBUILD, CLEAN are chosen in the NMake commandline properties in the vcproj
@IF %1==BUILD make && echo touch > build/msvc-lastbuildstate

@IF %1==REBUILD make clean && make && echo touch > build/msvc-lastbuildstate

@IF %1==CLEAN make clean

@IF %1==RUN call run.bat %PROJECTNAME%.nds
