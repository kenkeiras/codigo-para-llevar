COMPRESSOR:=gzip
EXTENSION:=gz

CLIENT_TAG:=tor-ssh-bridge-client
CLIENT_TAR_FILE:=$(CLIENT_TAG).tar.$(EXTENSION)

SERVER_TAG:=tor-ssh-bridge-server
SERVER_TAR_FILE:=$(SERVER_TAG).tar.$(EXTENSION)

DOCKER_RUN_OPTS+=--privileged
SERVICE_NAME:=$(SERVICE_NAME)

.PHONY: all client server run-client run-server export-server

all: server client

client-key:
	ssh-keygen -P '' -f $@

client: client-key
	docker build -t $(CLIENT_TAG) -f client.dockerfile .

run-client: client
	sh -c "if [ -z '$$SERVICE_NAME' ];then 	          \
           echo -n 'Introduce hidden service name: '; \
           read SERVICE_NAME ;                        \
         fi;                                          \
         if [ -z '$$DESTINATION_IP' ];then 	        \
           echo -n 'Introduce destination IP: ';      \
	         read DESTINATiON_IP ;                      \
         fi;                                          \
	       docker run -t -e HIDDEN_SERVICE=$$SERVICE_NAME -e DESTINATION_IP=$$DESTINATION_IP $(DOCKER_RUN_OPTS) $(CLIENT_TAG)"

server: client-key
	docker build -t $(SERVER_TAG) -f server.dockerfile .

run-server: server
	docker run -t $(DOCKER_RUN_OPTS) $(SERVER_TAG)

export-server: server
	docker export `docker create $(SERVER_TAG)` | pv | $(COMPRESSOR) > $(SERVER_TAR_FILE)
