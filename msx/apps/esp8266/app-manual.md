### ESP8266

A utility to allow access to some of the key functions of the RC2014 ESP8266 based Wi-Fi Module.

\small
\begin{importantblock}
\begin{raggedright}
You will need to connect your RC2014 Wi-Fi Module to your RC2014 serial interface (USB FTDI or SIO/2 Port B).  And the RC2014 Wi-Fi Module's ESP8266 must be flashed as per the GitHub repository at https://github.com/vipoo/yellow-msx-esp8266-wifi-module
\end{raggedright}
\end{importantblock}
\normalsize

Supported sub commands:

|Command|Description
|--|---|
|ESP8266 SET-WIFI <ssid> <password>|Issue `nat+cwjap=<ssid>,<password>` to set the ESP8266's persisted Wi-Fi credentials|
|ESP8266 SET-TIMEZONE <zone>|Issue `at+locale=<zone>` to set the ESP8266's time zone to be used|
|ESP8266 TIME-SYNC|Issue `at+time?` and assigned the retrieved internet date & time to your system|
|ESP8266 WGET <url> <localfile>|Issue `at+wget<url>` to retrieve the specified internet file|

#### Set Wi-Fi

`ESP8266 SET-WIFI <ssid> <password>`

Use this command to assign a your Wi-Fi's access credentials.

example:

```
ESP8266 SET-WIFI myssid mypassword
```

#### Set Time Zone

`ESP8266 SET-TIMEZONE <zone>`

Use this command to set the time zone your ESP8266 should assume when converting the time from UTC.

The `zone` parameter must be a valid TZ string.  For a list of valid zone, see the Wikipedia page
[https://en.wikipedia.org/wiki/List_of_tz_database_time_zones](https://en.wikipedia.org/wiki/List_of_tz_database_time_zones)

example:

```
ESP8266 SET-TIMEZONE Australia/Melbourne
```

#### Time Sync

`ESP8266 TIME-SYNC`

Use this command to set your computer's date and time to the current date/time as per your internet connection.

#### WGET

`ESP8266 WGET <url> <localfile>`

Use this command to retrieve a file using the ESP8266's internet connection.

> Please note, that this command can only accept `http://` connection - TLS connections using `https://` is not supported.

To Retrieve a simple webpage into a file called `home.htm`.

```
ESP8266 WGET http://info.cern.ch home.htm
```

You can also retrieve files from your PC if you create a simple static web server.

One way to create a simple server is using the node package `http-server` (https://www.npmjs.com/package/http-server).

If you have node installed on your main PC, you can publish files by running the command `npx http-server` in a directory of files you want published.

For example:

```BASH
me@PC:~$ mkdir ~/myfiles
me@PC:~$ echo "simple file" > ~/myfiles/test.txt
me@PC:~$ cd myfiles/
me@PC:~/myfiles$ npx http-server
Starting up http-server, serving ./

http-server version: 14.1.1

http-server settings:
CORS: disabled
Cache: 3600 seconds
Connection Timeout: 120 seconds
Directory Listings: visible
AutoIndex: visible
Serve GZIP Files: false
Serve Brotli Files: false
Default File Extension: none

Available on:
  http://127.0.0.1:8080
  http://192.168.14.23:8080
Hit CTRL-C to stop the server
```

Given the above server is running, then on your MSX computer you can issue the following command:

`ESP8266 WGET http://192.168.14.23:8080/test.txt test.txt`

> As `http-server` is a node application, it will work on a Windows, Mac or Linux computer.

