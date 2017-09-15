all: main.c
	gcc main.c -o invmat

doc: # ainda nao entendi como fazer essa opcao


clean:
	rm *.o; rm invmat
