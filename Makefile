ARCH := $(shell getconf LONG_BIT)
C_FLAGS_64 := -L/usr/lib64/mysql
C_FLAGS_32 := -L/usr/lib

smatool: smatool.o repost.o sma_mysql.o almanac.o
	gcc smatool.o repost.o sma_mysql.o almanac.o $(C_FLAGS_$(ARCH)) -lmysqlclient -lbluetooth -lcurl -lm -o smatool 
smatool.o: smatool.c sma_mysql.h
	gcc -c smatool.c
repost.o: repost.c sma_mysql.h
	gcc -c repost.c
sma_mysql.o: sma_mysql.c
	gcc -c sma_mysql.c
almanac.o: almanac.c
	gcc -c almanac.c
sma_pvoutput.o: sma_pvoutput.c
	gcc -c sma_pvoutput.c

