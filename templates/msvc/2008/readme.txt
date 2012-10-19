Instructions:

1. make sure you have DEVKITPRO_WIN environment variable set, for example, to c:\devkitPro

2. do not use a directory name with spaces in it for your project, or else you will be sorry.
   in fact, you should never use paths with spaces in them.

3. You may wish to drop desmume.exe into a directory named desmume. then you can use ctrl+f5 to run it in emulator.

That is all.

Notes
* Leave your source in the source folder. The template makefiles provided rely upon it.
* Build with F7.
* We don't use Visual Studio's output file vcproj property for dependency tracking.. we let make handle it. So make is always run.

Addendum A. sln and vcproj names
The template filenames are named arm9. This is a reasonable name, for reasons you may not understand yet.
However, you may wish to name the .sln and .vcproj and the msvc project to match your project and directoryname. 
It shouldn't be necessary with this template; it would be purely for aesthetic purposes. We can appreciate this.
For instance:
  c:\test\myproject
  c:\test\myproject\myproject.sln
  c:\test\myproject\myproject.vcproj
  c:\test\myproject\myproject.nds
If you do rename these, then you must text-edit the .sln to reference the new vcproj filename:
. Rename the sln and vcproj files
. Textedit the sln. At about line 4, you will see:
    > "arm9", "arm9.vcproj",
  "arm9" is the cosmetic name for the project. change it to "myproject"
  "arm9.vcproj" is the link to the projectfile. change it to "myproject.vcproj" -- whatever matches the vcproj filename you just renamed to.

