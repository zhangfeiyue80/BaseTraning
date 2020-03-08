EXEC= create_res
#EXEC= test

all : $(EXEC)
create_res : create_res.c
	gcc -g -o $@ $<

test: test.c
	gcc -g -o $@ $<

clean:
	rm -f $(EXEC)
