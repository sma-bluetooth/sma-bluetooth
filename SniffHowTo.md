# Introduction #

Sniffing smabluetooth from Windows.


# Details #

These details are very brief but should help anyone trying to get the data.

1. Get a USB sniffing tool for Windows. I have used UsbSnoop http://sourceforge.net/projects/usbsnoop/ and SnoopyPro http://sourceforge.net/projects/usbsnoop/files/SnoopyPro/

2. Install the usb sniffing program of choice - if you have important data on your computer it is recommended to back it up.

3. run sunnyexplorer from the command line for the previous day such as
> sunnyexplorer.exe "C:\Users\user\Documents\SMA\Sunny Explorer\mysolar.sxp" -userlevel user -password 0000 -exportdir c:\downloads\sma-export -exportrange 20110101-20111231 -export energy5min

The directory locations, mysolar.sxp, and password should be set appropriately. Only use a date range for the day before not a year as in this example.

4. Check if getting output that makes sense in the output file.

5. Start your sniffer, configure it (not that hard) and run the command above with sunny explorer.

6. Save all the output from the sniffer - it will probably make no sense to you but all of it is needed.

7. Post the results to the mailing list and your inverter can be added to the supported ones if all goes well

This is a very basic guide, anyone willing to enhance it is welcome.