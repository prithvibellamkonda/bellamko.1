CC = gcc 

bt : new_main.o
	$(CC) -o $@ $^

new_main.o: new_main.c
	$(CC) -c -o $@ $< 


clean:
	rm -f bt *.o
