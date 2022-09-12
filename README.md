![](/images/proximoth_banner.png)
# Proximoth
### Control Frame Attack Vulnerability Detection Tool
Proximoth performs `Control Frame Attack` to detect the vulnerability of Wi-Fi devices.

Control Frame Attack lets the attacker find out whether the target device is in proximity with the limitation of Wi-Fi antennas' scan range.

# More info about Control Frame Attack:
* https://www.researchgate.net/publication/314361145_A_Study_of_MAC_Address_Randomization_in_Mobile_Devices_and_When_it_Fails
* https://technicaluserx.wordpress.com/2022/07/22/proximoth/

-------------------------------------------


![](/images/i1.png) 
![](/images/i2.png) 



## Installation By Building
### Required Libraries
* libpcap-dev
* libc6 
``` bash
$ sudo apt-get install libc6 libpcap-dev
```

### Installation
```bash
$ git clone https://github.com/TechnicalUserX/proximoth.git
$ cd proximoth
$ make
```

## Put The Interface In Monitor Mode (Required By Proximoth)
> `wlan0` name may vary depending on your operating system.
> Sometimes, it may look like wlp2s0 or wlx6a4cdd56f9b1.
```bash
#Put the interface into monitor mode
$ sudo ip link set wlan0 down
$ sudo iw wlan0 set type monitor
$ sudo ip link set wlan0 up

#To put the interface back to managed mode
$ sudo ip link set wlan0 down
$ sudo iw wlan0 set type managed
$ sudo ip link set wlan0 up
```


## To Put The Interface Into A Specific Channel
> CHAN is arbitrary, like 2 or 11. Available channels depend on your Wi-Fi antenna.

```bash
$ sudo iw wlan0 set channel <CHAN>
```
> Channel numbers are generally between 1 and 11.


# Usage
## Example Usages
```bash
$ sudo proximoth -i wlan0 10:DA:41:E8:FA:DC

$ sudo proximoth -i wlan0 10:DA:41:E8:FA:DC --dump-file dump --file record

$ sudo proximoth -i wlan0 10:DA:41:E8:FA:DC --wait-rts

$ sudo proximoth -i wlan0 10:DA:41:E8:FA:DC -b 1C:CC:D6:00:00:00
```
## General Usage Information
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
