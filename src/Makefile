GCC = g++ -std=c++17 -g -Wall -Werror -Wextra
TEST = tests/test.cc
MODEL = 3D_Viewer/models/sources/*_model.cc 3D_Viewer/models/sources/s21_matrix_oop.cc
CONTRL = 3D_Viewer/controllers/sources/*.cc
GCOV =--coverage
LIBA = *.a
LIBO = *.o
OS = $(shell uname)
FILECASE_H = `find . -name "*.h"`
FILECASE_CC = `find . -name "*.cc"`

ifeq ($(OS), Darwin)
LIBFLAGS = -lm -lgtest -lstdc++ -lgtest_main
else
LIBFLAGS=-lstdc++ `pkg-config --cflags --libs gtest  --libs gtest_main`
endif

all: clean tests

tests: clean
	$(GCC) $(TEST) $(MODEL) $(CONTRL)  $(LIBFLAGS)  -o test
	./test

clean:
	rm -rf *.o *.a  *.out test *.dSYM  report build coverage.info *.gcda *.gcno *.txt *.tgz .clang-format *.user


dvi:
	open ./Readme_rus.html

dist:
	rm -rf Archive_3dviewer_v2.0/
	mkdir Archive_3dviewer_v2.0/
	mkdir Archive_3dviewer_v2.0/src
	cp -rf Makefile 3D_Viewer Archive_3dviewer_v2.0/src/
	tar cvzf Archive_3dviewer_v2.0.tgz Archive_3dviewer_v2.0/
	rm -rf Archive_3dviewer_v2.0/

s21_3dviewer.a: clean
	$(GCC) -c $(MODEL) $(TEST) $(CONTRL)
	ar rcs $(LIBA) $(LIBO)
	ranlib $(LIBA)

gcov_report: s21_3dviewer.a
	$(GCC) $(GCOV) $(MODEL) $(CONTRL) $(LIBA) -L. $(LIBA)  $(LIBFLAGS) -o test
	./test
	lcov -t "test" -c -d  ./ --no-external --output-file ./coverage.info
	genhtml ./coverage.info --output-directory ./report/
	open ./report/index.html

install:
	make clean
	mkdir build
	cd build && qmake ../3D_Viewer/3dviewer.pro && make --no-print-directory
	rm -rf build/*.o build/*.cpp build/*.h build/Makefile

uninstall: 
	rm -rf build*

check:
	cp ../materials/linters/.clang-format ./
	cppcheck --enable=all --suppress=missingIncludeSystem --inconclusive --check-config $(FILECASE_H) $(FILECASE_CC)
	clang-format -style=Google -n $(FILECASE_H) $(FILECASE_CC)
	clang-format -style=Google -i $(FILECASE_H) $(FILECASE_CC)
	make tests

ifeq ($(OS), Darwin)
	leaks --atExit -- test
else
	CK_FORK=no valgrind --vgdb=no --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose --log-file=RESULT_VALGRIND.txt ./test
endif

open_app:
ifeq ($(OS), Darwin)
	./build/3dviewer.app/Contents/MacOS/3dviewer
else
	./build/3dviewer
endif