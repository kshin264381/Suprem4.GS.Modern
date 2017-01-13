PYTHON=python3
PYCC=cython
PYCCFLAGS=-3
PYINCLUDE=/usr/include/python3.4m
PYLNK=-L/usr/lib -lpython3.4m
PYINSTALLER=/home/oxygen/KSHIN/local/Python3/bin/pyinstaller

CC=/usr/bin/gcc
CFLAGS=-g -gdwarf-3 -I$(PYINCLUDE) $(PYLNK)

all: freeze

strread.c: strread.py
	$(PYCC) $(PYCCFLAGS) $< -o $@

process_str.c: process_str.py
	$(PYCC) $(PYCCFLAGS) $< -o $@ --embed

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

compile: strread.o process_str.o
	$(CC) -o strread $<

freeze: strread.py process_str.py plot_str.py
	$(PYINSTALLER) -F ./strread.py

clean:
	rm -rfv *.o *.c ./__pycache__ *.pyc *.xls *.xlsx
	rm -rfv ./build ./dist ./*.spec
