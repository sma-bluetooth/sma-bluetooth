ARCH := $(shell getconf LONG_BIT)
C_FLAGS_64 := -L/usr/lib64/mysql
C_FLAGS_32 := -L/usr/lib/mysql


smatool: smatool.o repost.o sma_mysql.o almanac.o sb_commands.o sma_struct.h
	gcc smatool.o repost.o sma_mysql.o almanac.o sb_commands.o -fstack-protector-all -O2 -Wall $(C_FLAGS_$(ARCH)) -lxml2 -lmysqlclient -lbluetooth -lcurl -lm -o smatool 
smatool.o: smatool.c sma_mysql.h
	gcc -O2 -c smatool.c
repost.o: repost.c sma_mysql.h
	gcc -O2 -c repost.c
sma_mysql.o: sma_mysql.c
	gcc -O2 -c sma_mysql.c
almanac.o: almanac.c
	gcc -O2 -c almanac.c
sma_pvoutput.o: sma_pvoutput.c
	gcc -O2 -c sma_pvoutput.c
sb_commands.o: sb_commands.c
	gcc -O2 -c sb_commands.c
clean:
	rm *.o
install:
	install -m 755 smatool /usr/local/bin
	install -m 644 sma.in.new /usr/local/bin
	install -m 644 smatool.conf.new /usr/local/etc
	install -m 644 smatool.xml /usr/local/bin

