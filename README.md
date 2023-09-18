![](/images/proximoth_banner.png)
# Proximoth
### Control Frame Attack Vulnerability Detection Tool
Proximoth performs `Control Frame Attack` to detect the proximal privacy violation vulnerability.

Control Frame Attack lets the attacker find out whether the target device is in range, in other words, presence detection.

# More Info About Control Frame Attack:
* https://www.researchgate.net/publication/314361145_A_Study_of_MAC_Address_Randomization_in_Mobile_Devices_and_When_it_Fails
* https://technicaluserx.wordpress.com/2022/07/22/proximoth/

---

![](/images/i1.png) 
![](/images/i2.png) 



## Installation By Building
### Required Packages To Compile
* libpcap-dev
* gcc
* make
* gzip

```bash
$ sudo apt-get install libpcap-dev gcc make gzip
```

### Installation
```bash
$ git clone https://github.com/TechnicalUserX/proximoth.git
$ cd proximoth
$ make
```

## Put The Interface In Monitor Mode (Obligatory)
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

## Set Interface Channel (Optional)
> `<channel>` is arbitrary, like 2 or 11. Available channels depend on your Wi-Fi antenna.
> You might want to change the channel of your interface in scenarios which the default channel is in heavy disturbance by other Wi-Fi devices.

```bash
$ sudo iw wlan0 set channel <channel>
```
> Channel numbers are generally between 1 and 11.


# Usage
## Example Usages
```bash
$ sudo proximoth -i wlan0 10:DA:41:E8:FA:DC

$ sudo proximoth -i wlan0 10:DA:41:E8:FA:DC --dump-file dump --out-file record

$ sudo proximoth -i wlan0 10:DA:41:E8:FA:DC --rts-interval 100000

$ sudo proximoth -i wlan0 10:DA:41:E8:FA:DC -b 1C:CC:D6:00:00:00
```
## General Usage Information
```
Usage: proximoth [options] <target>

 <target>                            : MAC address of the target.

options:

 -h, --help                          : Prints this screen.

 -o <file>, --out-file <file>        : File to write statistics after shutdown.

 -b <bssid>, --bssid <bssid>         : Custom BSSID to be injected as sender MAC address.
                                       Address is fixed automatically to be global and unicast.

 -a, --no-mac-autofix                : Disables unicast/global auto fix for BSSID MAC addresses.

 -i <iface>, --interface <iface>     : Wireless interface to use packet injection and sniffing.
                                       Obligatory option.

 -d <file>, --dump-file <file>       : Write all CTS captures to a PCAP file.

 -r <us>, --rts-interval <us>        : Microseconds as threshold to wait between RTS injections.
                                       Setting it to a low value might cause malfunction.
                                       Default: 500000

 -t, --text-mode                     : Enables text only mode.

 --version                           : Prints version number and author information.

```

# Docker Containers

## Building And Running

```bash
$ git clone https://github.com/TechnicalUserX/proximoth.git
$ cd proximoth
$ sudo docker build -t proximoth .
$ sudo docker run --tty --rm --network=host proximoth -i wlan0 XX:XX:XX:XX:XX:XX
```
## Pulling From Docker Hub
```bash
$ sudo docker pull technicaluserx/proximoth:latest
$ sudo docker run --tty --rm --network=host technicaluserx/proximoth -i wlan0 XX:XX:XX:XX:XX:XX
```

---

# Disclaimer

This software is distributed in the hope that it will be useful, but without a warranty.
No responsibility will be taken for any illegal activity, use it at your own risk.