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

tun_device=`wait_for_tun`
ifconfig "$tun_device" 10.0.1.1 pointopoint 10.0.1.2

DESTINATION_IP=$1
IP=10.0.1.1
iptables -t nat -A POSTROUTING -j MASQUERADE
iptables -t nat -A PREROUTING -p tcp --dest $IP -j DNAT --to-destination $DESTINATION_IP
iptables -t nat -A PREROUTING -p udp --dest $IP -j DNAT --to-destination $DESTINATION_IP
iptables -t nat -A PREROUTING -p icmp --dest $IP -j DNAT --to-destination $DESTINATION_IP
# iptables -t nat -L

while [ 1 ];do
    sleep 9999999999999h
done
