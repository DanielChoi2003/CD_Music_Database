
musicDB: mcdcli.c mcddb.c mcddb.h
	gcc -g mcdcli.c mcddb.c mcddb.h mcdutil.c mcdutil.h mcdscr.h -o musicDB -L/usr/lib -ldb -lncurses


clean:
	rm musicDB