smatool: smatool.o
	gcc smatool.o -L/usr/lib -lmysqlclient -lbluetooth -lcurl -lm -o smatool 
smatool.o: smatool.c
	gcc -c smatool.c
