#!/usr/bin/env python
# coding: utf-8
from twisted.web import server, resource  # Objetos referentes a HTTP
from twisted.internet import reactor      # "Motor" asíncrono de Twisted

from handler import handle_request


def bencode_dict(d):
    '''Convierte a Bencode un diccionario.
       Ojo, las claves tienen que estar ordenadas _lexicográficamente_.'''
    bencoded = []
    for key in sorted(d.keys(), key=str.upper):
        bencoded.append(bencode(key) + bencode(d[key]))

    return 'd' + (''.join(bencoded)) + 'e'


def bencode_list(l):
    '''Convierte a Bencode una lista.'''
    bencoded = []
    for element in l:
        bencoded.append(bencode(element))

    return 'l' + (''.join(bencoded)) + 'e'


def bencode_int(n):
    '''Convierte a Bencode un número.'''
    return 'i%ie' % n


def bencode_str(s):
    '''Convierte a Bencode una cadena.'''
    return '%i:%s' % (len(s), s)


def bencode(o):
    '''Convierte a Bencode un objeto.'''
    if type(o) in (str, unicode):
        return bencode_str(o).encode('ascii')
    elif type(o) == int:
        return bencode_int(o).encode('ascii')
    elif type(o) == list:
        return bencode_list(o).encode('ascii')
    elif type(o) == dict:
        return bencode_dict(o).encode('ascii')
    raise Exception('Invalid type ' + str(type(o)))


class Tracker(resource.Resource):
    '''Cada objeto de esta clase representará un tracker.'''
    isLeaf = True  # Declaramos que no pretende ser una superclase.

    def render_GET(self, request):
        '''Una petición GET al servidor.'''

        # Lectura de las opciones de la petición
        args = request.args

        # Identificador del torrent
        info_hash = args.get('info_hash', [None])[0]

        # Identificador del cliente (generado con cierta aleatoriedad)
        peer_id = args.get('peer_id', [None])[0]

        # Puerto del cliente torrent
        port = args.get('port', [None])[0]

        # Número total de bytes subidos (en ASCII base 10), por defecto 0
        uploaded = int(args.get('uploaded', [0])[0])

        # Número total de bytes descargados (en ASCII base 10), por defecto 0
        downloaded = int(args.get('downloaded', [0])[0])

        # Número de bytes restantes para completar todos los
        # archivos del torrent (en ASCII base 10).
        left = args.get('left', [None])[0]

        # Respuesta compacta
        compact = bool(int(args.get('compact', [False])[0]))

        # Se permite la omisión del peer_id de los clientes en la respuesta.
        no_peer_id = bool(args.get('no_peer_id', [False])[0])

        # `Started', `stopped' o `completed' (opcional).
        event = args.get('event', [None])[0]

        # IP de la máquina (opcional).
        ip = args.get('ip', [request.getClientIP()])[0]

        # Número de pares en la respuesta (opcional), por defecto 50.
        numwant = int(args.get('numwant', [50])[0])

        # Clave del cliente (por motivos de seguridad).
        key = args.get('key', [None])[0]

        # (Opcional) ID que fué enviada al cliente en una petición anterior.
        trackerid = args.get('trackerid', [None])[0]

        handle_request(request, info_hash, peer_id, port, uploaded, downloaded,
                       left, compact, no_peer_id, event, ip, numwant, key,
                       trackerid)

        # La respuesta no volverá en este hilo
        return server.NOT_DONE_YET


if __name__ == "__main__":
    # Inicialización de un tracker como un servidor web
    tracker = server.Site(Tracker())
    # Inicialización del motor de Twisted
    reactor.listenTCP(8080, tracker)
    # Se pone a funcionar el servidor
    reactor.run()
