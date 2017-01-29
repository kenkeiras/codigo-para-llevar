#!/usr/bin/env bash

get_tun() {
    ifconfig -a |grep ^tun|cut -d: -f1
}

wait_for_tun() {
    while [ 1 ];do
        tun=`get_tun`
        if [ ! -z "$tun" ];then
            echo "$tun"
            return
        fi
    done
}

service tor start

echo 1 > /proc/sys/net/ipv4/ip_forward

echo "Waiting 15s"
sleep 15s
python2.7 -c $'f = open("/var/log/tor/log")\nfor line in f:\n print(line.rstrip())\n if "Bootstrapped 100%: Done" in line:\n  break' &

echo "Connecting..."

torify ssh -o StrictHostKeyChecking=no -p 2222 -w any root@$HIDDEN_SERVICE /client_in_server_run_code.sh $DESTINATION_IP &

tun_device=`wait_for_tun`
IP=`hostname -i`

ifconfig $tun_device 10.0.1.2 pointopoint 10.0.1.1

iptables -t nat -A POSTROUTING -j MASQUERADE
iptables -t nat -A PREROUTING -p tcp --dest $IP -j DNAT --to-destination 10.0.1.1
iptables -t nat -A PREROUTING -p udp --dest $IP -j DNAT --to-destination 10.0.1.1
iptables -t nat -A PREROUTING -p icmp --dest $IP -j DNAT --to-destination 10.0.1.1
iptables -t nat -L

watch -d -c -n 0.2 "echo -n '\033[1mClient side: \033[0m'; hostname -i ; ifconfig -a"
