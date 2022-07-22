![](/images/proximoth_banner.png)
# Proximoth
Control Frame Attack Vulnerability Detection Tool

![](/images/i1.png) 
![](/images/i2.png) 

## Installation By Building

```bash
$ git clone https://github.com/TechnicalUserX/proximoth.git
$ cd proximoth
$ make
```
## Put the interface in monitor mode
> `wlan0` name may vary depending on your operating system
> Sometimes, it may look like wlp2s0 or wlx6a4cdd56f9b1
```bash
$ sudo ip link set wlan0 down
$ sudo iw wlan0 set type monitor
$ sudo ip link set wlan0 up

```
## To put the interface into a specific channel
> CHAN is arbitrary, like 2 or 11.
```bash
$ sudo iw wlan0 set channel <CHAN>
```


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
