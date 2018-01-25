all clean:
	@for dir in [0-9][0-9]_*;\
	 do test -f $$dir/Makefile\
		&& (cd $$dir && make $@)\
		|| true;\
	 done
