all: main.c
	gcc main.c -o invmat

doc: 
	cd doc; doxygen; cd ..

clean:
	rm invmat
