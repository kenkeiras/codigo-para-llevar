FROM debian:stretch

# Prepare the base
RUN apt update
RUN apt install -y ssh tor
RUN apt install -y iptables
RUN apt install -y net-tools
RUN apt install -y python2.7-minimal

# Setup the SSH service
RUN mkdir /root/.ssh/
ADD client-key /root/.ssh/id_rsa
ADD client-key.pub /root/.ssh/id_rsa.pub

ADD client-entrypoint.sh /entrypoint.sh
RUN chmod +x /entrypoint.sh

ENTRYPOINT /entrypoint.sh
