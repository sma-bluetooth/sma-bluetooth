Installation of sma-bluetooth

# Introduction #

A brief and changing installation instruction.
Please add comments and added advice.

# Details #

1. Download latest source

2. uncompress it with something like **tar -xz -f sma-bluetooth.tgz** it will install into a directory sma-bluetooth relative to your current directory.

3. **cd sma-bluetooth**

4. see if you can see your inverter using **hcitool scan** it should show your inverter bluetooth address - note it you will need it later. this needs to work before anything else is done. If you don't have hcitool then bluez tools are probably not installed

5. ensure the bluetooth service is active - the method will depend on distribution.

6. Compile program  **gcc -lbluetooth -lcurl -lmysqlclient -g -o smatool smatool.c**

7. Edit **smatool.conf** I hope it is self explanatory. Don't set mysql or pvoutput data until later, this is just a test

8. run it **smatool -v -from "2011-01-06 00:00:00" -to "2011-01-06 23:55:00"** It will give lots of screen info if it works. It should show 5 minute data for the day - you can change the dates to a more appropriate period.

9. If it works well done. Fill in the rest of **samtool.conf**

10. more to come