all: salvapantallas

salvapantallas: salvapantallas.c
	gcc $< -o $@ -lX11

install:
	mv salvapantallas /usr/lib/xscreensaver/

clean:
	rm -f salvapantallas
