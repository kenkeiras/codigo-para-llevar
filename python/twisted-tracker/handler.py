#!/usr/bin/env python
# coding: utf-8
import sqlite3
DB_NAME = 'tracker.sqlite3'
MAX_PEERS = 200

# Creación de la tabla en caso de no existir
conn = sqlite3.connect(DB_NAME)
c = conn.cursor()
c.execute('''CREATE TABLE IF NOT EXISTS peers
             (info_hash BLOB(20), peer_id BLOB(20), ip VARCHAR(15),
              port INT, complete BOOLEAN)''')
conn.commit()
conn.close()


def callback(data):
    '''Mostramos los datos que Bittorrent necesita.'''
    seeders = data[2][0][0]
    leechers = data[3][0][0]
    peers = data[4]

    # Formato compacto
    if compact:
        peer_list = []
        for peer in peers:  # El reduce convierte la IP a binario
            ip = reduce(lambda x, y:
                        (int(x) << 8) + int(y), peer[1].split("."))
            peer_list.append(struct.pack("IH", ip, peer[2]))

        peer_structure = ''.join(peer_list)

    else:
        peer_structure = []
        for peer in peers:
            new_peer = {'ip': peer[1],
                        'port': peer[2]}

            if (peer[0] is not None) and (not no_peer_id):
                new_peer['peer_id'] = peer[0]

            peer_structure.append(new_peer)

    request.setHeader('Content-Type', 'text/plain')
    # Segundos a esperar antes de la próxima petición
    request.write(bencode({'interval': 5 * 60,
                           'complete': seeders,
                           'incomplete': leechers,
                           'peers': peer_structure}))

    request.finish()


def handle_request(request, info_hash, peer_id, port, uploaded,
                   downloaded, left, compact, no_peer_id,
                   event, ip, numwant, key, trackerid):
    '''Prepara la respuesta a una petición al tracker.'''

    binfo_hash = sqlite3.Binary(info_hash)
    AsyncSqlite(DB_NAME, (('''DELETE FROM peers
                                WHERE info_hash = ? AND peer_id = ?''',
                           (binfo_hash, peer_id)),
                          ('''INSERT INTO peers (info_hash, peer_id, ip, port, complete)
                                VALUES (?, ?, ?, ?)''',
                           (binfo_hash, peer_id, ip, port,
                            1 if left == 0 else 0)),
                          ('''SELECT count(1) as seeders FROM peers
                                WHERE complete = 1''', ()),
                          ('''SELECT count(1) as leechers FROM peers
                                WHERE complete = 0''', ()),
                          ('''SELECT peer_id, ip, port FROM peers
                                WHERE info_hash = ? AND peer_id != ?
                                ORDER BY RANDOM() LIMIT ?''',
                           (binfo_hash, peer_id, min(numwant, MAX_PEERS)))),
                callback).start()
