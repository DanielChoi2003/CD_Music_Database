
musicDB: mcdcli.c mcddb.c mcddb.h
	gcc -g mcdcli.c mcddb.c mcddb.h -o musicDB -L/usr/lib -ldb


clean:
	rm musicDB