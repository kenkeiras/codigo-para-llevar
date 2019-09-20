.PHONY: all

all: graph.svg graph.png

graph.svg: graph.gv
	dot $< -Tsvg -o $@

graph.png: graph.gv
	dot $< -Tsvg -o $@
