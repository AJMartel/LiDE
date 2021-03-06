# LiDE "Li*nux* D*isk* E*xtractor*"

As of version 0.0.3.0 it has the same functionality as running a string of commands:

* dd to capture a file (more like cat though)
* pv "pipe viewer" to view the progress of the file transfer
* netcat to send the file over the network
* md5sum to report the unique file hash, in order to confirm a 100% good file transfer
* ifconfig to report the local IP address

All packaged into a small executable less than 19Kb in size.

The following command could accomplish the same results:

*ifconfig | grep -A1 eth0 | grep inet | cut -f2 -d ":" | cut -f1 -d " " && md5sum File_to_Transfer && cat File_to_Transfer | pv -b | nc -l -p 31337*

##Summary ChangeLog

###v0.1
Equivalent to: *cat File_to_Transfer | nc -l -p 9999*
>Original source code: https://github.com/joshualockhart/Hermes

>By Josh Lockhart, joshualockhart@gmail.com, 30 Oct 2010

###v0.2
* Fixed hang, on end of file transfer.
* Submited Pull request to joshualockhart/Hermes

###v0.0.1.1
* Added ChangeLog
* Renamed project to LiME
* Renamed files.
* Fixed version.
* Patched Makefile

###v0.0.1.2
* Removed option for verbose, always verbose.
* Changed default port.
* Removed new line for progress status (verbose).

###v0.0.1.3
* Progress status removed cursor (looks cleaner).

###v0.0.1.4
* Re-enable cursor after program termination.

###v0.0.1.5
* Fixed Help to reflect program as it is now layed out.
* Changed displayed wording.
* Added colors to output.
* Cleaned up Makefile

###v0.0.2.0
Equivalent to: *md5sum File_to_Transfer && cat File_to_Transfer | pv -b | nc -l -p 31337*
* Added MD5 function for source file
>MD5 Function:

>http://stackoverflow.com/questions/1220046/how-to-get-the-md5-hash-of-a-file-in-c/1220177#1220177

>D'Nabre, 2 Aug 2009

###v0.0.2.1
-Fixed MD5 function error displayed if no file available.

###v0.0.3.0
Equivalent to: *ifconfig | grep -A1 eth0 | grep inet | cut -f2 -d ":" | cut -f1 -d " " && md5sum File_to_Transfer && cat File_to_Transfer | pv -b | nc -l -p 31337*
* Removed references for "be_verbose"
* Added get local IP Address of eth0 (hardcoded)
* Added display remote command to acquire file transfer
* Clean up comments  
>Get local IP Address Function:

>http://www.geekpage.jp/en/programming/linux-network/get-ipaddr.php

>Akimichi Ogawa

###v0.0.3.1
* Added option to choose interface, now can display remote command to acquire file transfer on interface other than eth0 by using "-i [iface]"
>Example "*lide -i wlan0 File_to_Transfer*" 

###v0.0.3.2
* Changed display of executed program, easier to read.

###v0.0.3.3
* Moved program version info to LiDE.h
* Added mktarball.sh for easier Source Archiving

###v0.0.3.4
* Added "-i" option to program help
* Edited LiDE help
* Added to mktarball.sh for creating Patches


##Referenced Sources

**Original Source**

>https://github.com/joshualockhart/hermes

>Thanks to Joshua Lockhart for getting my project off the ground. 


**MD5 Function**

>http://stackoverflow.com/questions/1220046/how-to-get-the-md5-hash-of-a-file-in-c/1220177#1220177

>D'Nabre, 2 Aug 2009


**Get local IP Address Function**

>http://www.geekpage.jp/en/programming/linux-network/get-ipaddr.php

>Akimichi Ogawa
