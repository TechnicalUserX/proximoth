![](/images/proximoth_banner_2.png)
# Proximoth
Control Frame Attack Vulnerability Detection Tool


# Usage
```
proximoth [flags] <TARGET>

 <TARGET>                            : MAC address of the target, no need a flag.

FLAGS:

 -h, --help                          : Print this screen.

 -f <FILE>, --file <FILE>            : File name <FILE> to write measurements after the close of Proximoth.

 -b <BSSID>, --bssid <BSSID>         : Custom BSSID to be injected as sender MAC address.
                                       Only first 3 pairs are considered. Last 3 pairs are ignored.

 -i <IFACE>, --interface <IFACE>     : Wireless interface to use packet injection and sniffing.

 -w, --wait-rts                      : Wait until the surrounding is free from all injected RTS frames for 10 seconds.

 -d <FILE>, --dump-file <FILE>       : Write all CTS frames to a pcap file.

 -r <NUM>, --rts-interval <NUM>      : Number of microseconds between every RTS injection, default is 500000.

 --version                           : Prints version number and author information.
```
