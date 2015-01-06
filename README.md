Tcpdumpservice and Tcpdumpclient
==============
TCPDUMP as Application FWK

*  An app with UI get packets form binder service by JNI invoking of servicemanager
*  A native service using binder and get packets by libpcap api

Build and flash to rom
------------------------
1.   Download AOSP and put `tcpdumpclient` `tcpdumpservice` to `external` folder of AOSP.
2.   Build and run your AOSP

Build and just test without build entire AOSP
------------------------
1.   Download AOSP and put `tcpdumpclient` `tcpdumpservice` to `external` folder of AOSP.
2.   Use the script in the folders to build only the app or native app
3.   Push native app to /data/local/tmp and run it
4.   Make a project in eclipse ADT and copy app code to ADT 
5.   Make a `lib` folder in eclipse ADT project and copy `libtcpdumpclient.so` to it   

Implement apis by libpcap
-------------------------
Wrap up TCPDUMP program to provide necessary APIs 
`Bool  getDataPacketFromIP (char *ip, char *packet)`
`Bool  getDataPacketToIP (char *ip, char *packet)`
`Bool  getDataPacketFromIPAndPort (char *ip, char *port, char *packet)`
`Bool  getDataPacketFromPort (char *port, char *packet)`
`Bool  getDataPacketToPort (char *port, char *packet)`
`Bool  getDataPacketFromIPToIP (char *srcip, char *dstip  char *packet)`
`Bool  getDataPacketFromMac (char* mac, char* packet)`
`Bool  getDataPacketToMac (char* mac, char* packet)`
`Bool  getDataPacketFromMacToMac (char *srcmac, char *desmac, char *packet)`
`Bool  getDataPacketWithProtocol (protocolType type, char* packet)`

Start Tcpdumpservice when system boots up
-----------------
1. Open `/system/core/rootdir/init.rc`
2. Add this
```
service tcpdumpservice /system/bin/tcpdumpservice
class main
user root
group root
```

Reference
------------------------
*  [Native binder service](https://github.com/DaleKocian/Android-Binder-Service-Example)
*  [JNI guide](https://hackpad.com/NDK-Basic-9rhWFyljkMs)
*  [Programming with pcap](http://www.tcpdump.org/pcap.html)
*  [libpcap sniffex](http://www.tcpdump.org/sniffex.c)
*  [Android.mk with Appcompact support](https://android.googlesource.com/platform/packages/apps/UnifiedEmail/+/master/Android.mk)
*  [Textview update without block UI](http://www.ipcas.com/blog/2011/06/threads-in-android-part-2-updating-the-ui/)
*  [Spinner example](http://www.mkyong.com/android/android-spinner-drop-down-list-example/)
*  [Spinner onItemSelected called erroneously (without user action)](http://stackoverflow.com/questions/5124835/spinner-onitemselected-called-erroneously-without-user-action)