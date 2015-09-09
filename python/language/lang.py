#!/usr/bin/env python
# coding: utf-8

import re
import os
import sys

# Define un color para mostrar los resultados
color = True
if color:
    colorfile = "\x1b[0;92m"  # Verde
    colorend = "\x1b[0m"      # Fin del color
else:
    colorfile = colorend = ""

# Diccionarios a comprobar
dictionaries = ["american-english", "british-english", "galician", "spanish"]
path = "/usr/share/dict"


class LangReader:
    words = re.compile(r'\w+')

    def __init__(self, dicts):
        '''Carga los diccionarios con las palabras,
           una por línea, en minúsculas.
        '''
        self.dicts = {}
        for d in dicts:
            self.dicts[d] = set(map(lambda x: x.strip().lower(),
                                    open(d, "rt").read().strip().split("\n")))

    def get_file_props(self, f):
        return self.get_props(f.read())

    def get_percents(self, data):
        '''Devuelve el porcentaje de palabras de cada idioma.'''
        props, total = self.get_props(data)
        percents = {}
        for lang in props:
            percents[lang] = round((float(props[lang]) / total) * 100, 2)
        return percents

    def get_props(self, data):
        '''Devuelve el número de palabras que se corresponde
           con el diccionario de cada idioma.'''
        counters = {}
        total = 0
        words = map(lambda w: w.lower(), self.words.findall(data))

        for lang in self.dicts:
            ldict = self.dicts[lang]
            counter = 0
            for word in words:
                if word in ldict:
                    counter += 1

            counters[lang] = counter

        return counters, len(words)


if __name__ == "__main__":
    if len(sys.argv) == 1:
        print >>sys.stderr, "%s <file>" % sys.argv[0]
        exit(0)

    files = map(lambda x: open(x, "rt"), sys.argv[1:])
    os.chdir(path)

    ld = LangReader(dictionaries)
    first = True
    for f in files:

        if not first:
            print "\n"

        props, total = ld.get_file_props(f)
        if total > 0:
            print "%s%s%s [%i]\n" % (colorfile, f.name, colorend, total)
            for i in sorted(props,
                            lambda x, y: props[x].__cmp__(props[y]),
                            reverse=True):

                print "%5.2f%%  %s" % ((props[i] / float(total)) * 100, i)

        else:
            print "%s%s%s nothing found" % (colorfile, f.name, colorend)
        first = False
