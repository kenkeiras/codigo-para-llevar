#!/usr/bin/env python
# coding: utf-8

import sqlite3
from threading import Thread


class AsyncSqlite(Thread):
    '''Conseguimos la asincronía usando varios hilos.'''

    def __init__(self, database, queries, callback):
        '''Constructor.'''
        Thread.__init__(self)  # Importante también construir el Thread

        self.database = database
        self.queries = queries
        self.callback = callback

    def run(self):
        '''
        Ya ejecutado en otro hilo, hace las peticiónes y lanza el
        callback con los resultados resultado como parámetro.
        '''

        conn = sqlite3.connect(self.database)
        c = conn.cursor()

        results = []
        for query, params in self.queries:
            c.execute(query, params)
            results.append(c.fetchall())

        conn.commit()
        self.callback(results)
        conn.close()
