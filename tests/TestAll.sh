#!/bin/bash

mkdir ./tmp
cd tmp
gcc -o LineTest ../LineTest.c -I../../ui ../../ui/*.c -lGL -lGLU -lglut -lm -lSOIL
gcc -o MenuBarTest ../MenuBarTest.c -I../../ui ../../ui/*.c -lGL -lGLU -lglut -lm -lSOIL
gcc -o MenuListTest ../MenuListTest.c -I../../ui ../../ui/*.c -lGL -lGLU -lglut -lm -lSOIL
gcc -o MenuItemTest ../MenuItemTest.c -I../../ui ../../ui/*.c -lGL -lGLU -lglut -lm -lSOIL
gcc -o WindowTest ../WindowTest.c -I../../ui ../../ui/*.c -lGL -lGLU -lglut -lm -lSOIL
gcc -o WindowManagerTest ../WindowManagerTest.c -I../../ui ../../ui/*.c -lGL -lGLU -lglut -lm -lSOIL
gcc -o WindowPaneTest ../WindowPaneTest.c -I../../ui ../../ui/*.c -lGL -lGLU -lglut -lm -lSOIL

./LineTest
./MenuBarTest
./MenuListTest
./MenuItemTest
./WindowTest
./WindowManagerTest
./WindowPaneTest

cd ../
rm -rf ./tmp
