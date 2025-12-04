@echo off
set "COMPILER=g++"
set "SOURCE_FILE1=src/main/main.cpp"
set "SOURCE_FILE2=src/main/audiovisualizer.cpp"
set "BINARY_FILE1=main.o"
set "BINARY_FILE2=audiovisualizer.o"
set "OUTPUT_FILE=app.exe"
set "INCLUDE_DIRS=-I"C:\Users\dafri\OneDrive\Documents\Visual Code\Coding Projects\SFML Projects\AudioVisualizer\include""
set "LIBRARY_DIRS=-L"C:\SFML-2.6.1\lib""
set "LIBRARIES=-lsfml-graphics -lsfml-window -lsfml-system -lopengl32 -lwinmm -lgdi32 -lcomdlg32 -lsfml-audio"

%COMPILER% -c %SOURCE_FILE1% %INCLUDE_DIRS%
%COMPILER% -c %SOURCE_FILE2% %INCLUDE_DIRS%
%COMPILER% %BINARY_FILE1% %BINARY_FILE2% -o %OUTPUT_FILE%  %LIBRARY_DIRS% %LIBRARIES%

start %OUTPUT_FILE%