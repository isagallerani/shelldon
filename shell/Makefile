all:
	@gcc -Wall -Werror --std=c89 -D_POSIX_C_SOURCE=200112L -o Shelldon parser.c main.c utils.c Item.c listaDinamica.c

clean:
	@rm Shelldon
	rm -f *.o *.d $(lib_static) $(lib_shared) $(bin) $(aux_bin) $(bin:%=%-static) $(aux_bin:%=%-static)
	rm -f $(EXTRA_GARBAGE) 
	rm -rf $(PROJECT) $(PROJECT).tgz


