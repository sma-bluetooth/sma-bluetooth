Quick quide to what additional modules are needed for ubuntu based systems to get this to compile.

# Introduction #

These are the packages needed (all from std ubuntu repo), to get it all to compile.

# Details #

nice to have:
a DB, this is the db i used: (these currently link to mysql 5.1)
mysql-server
mysql-client
mysql-query-browser

needed:
bluez
libbluetooth-dev
curl
libcurl3
libcurl4-nss-dev


or the easy way via terminal: ( Ctl + Alt + T)
sudo apt-get install mysql-server mysql-client mysql-query-browser
sudo apt-get install bluez libbluetooth-dev curl libcurl3 libcurl4-nss-dev


now compile the software: (go to the path you saved smatool.c)
gcc -lbluetooth -lcurl -lmysqlclient -g -o smatool smatool.c

find your bluetooth address for the inverter.
hcitool scan

enter all the details into the the config file: (you may need to copy use the .new as a basis).
leave out the pvoutput details if you dont want to upload
gedit smatool.conf

run the command in debug verbose to see it going.
./smatool -d -v