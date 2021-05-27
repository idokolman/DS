@echo off

set opts=-FC -GR- -EHa -nologo -Zi -W4 -Wno-unused-parameter
set code=%cd%
pushd build
echo -- building ex2.exe --
clang-cl %opts% %code%\library2.cpp %code%\DynamicArray.cpp %code%\RankedTree.cpp %code%\HashTable.cpp %code%\main2.cpp -Feex2 -D_CRT_SECURE_NO_WARNINGS
REM clang-cl %opts% %code%\CourseManager.cpp -Feex2 -D_CRT_SECURE_NO_WARNINGS
popd
