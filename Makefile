all: main.c
	gcc main.c -o invmat -lm -Wall

doc: 
	cd doc; doxygen; cd ..

clean:
	rm invmat; rm -r "doc/html"
