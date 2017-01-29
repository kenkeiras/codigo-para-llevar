FROM debian:stretch

# Prepare the base
RUN apt update
RUN apt install -y ssh tor
RUN apt install -y net-tools
RUN apt install -y iptables

# Setup the SSH service
RUN mkdir /root/.ssh/
ADD client-key.pub /root/.ssh/authorized_keys
RUN echo 'PermitTunnel yes' >> /etc/ssh/sshd_config

# Prepare the hidden service
RUN echo 'HiddenServiceDir /var/lib/tor/hidden_service/' >> /etc/tor/torrc
RUN echo 'HiddenServicePort 2222 127.0.0.1:22' >> /etc/tor/torrc

ADD client_in_server_run_code.sh /client_in_server_run_code.sh
RUN chmod +x /client_in_server_run_code.sh

ADD server-entrypoint.sh /entrypoint.sh
RUN chmod +x /entrypoint.sh

ENTRYPOINT /entrypoint.sh
