#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/* Cabeceras de X11 */
#include <X11/Xlib.h>
#include "vroot.h"

/* Esto no es necesario para el programa en si,
   per es muy útil en el caso de querer hacer
   comprobaciones y en el caso de que resulten
   mal, detener el programa.
*/
#include <assert.h>


/* Esta es la función que dibuja el salvapantallas. */
void drawScreen(Display* display, Window window,
                XWindowAttributes window_attributes,
                GC gc){

    static int step = 0; // nº de paso
    static int lastg = 0; // Último valor calculado (necesario para el fractal)
    static int x_position = -1; // Posicion actual en la horizontal
    static int y_position = -1; // Posicion actual en la vertical
    static int direction = 0; // 0 = arriba, 1 = derecha, 2 = abajo, 3 = izquierda
    static long color = 0;    // Color actual, 0 = negro, 0xFFFFFF = blanco

    /* Si es la primeira vez o salímos de la pantaia, reiniciamos las coordeadas, la
       dirección y el número de paso y vamos cambiando el color entre blanco y negro.
    */
    if ((x_position < 0) || (x_position >= window_attributes.width) ||
        (y_position < 0) || (y_position >= window_attributes.height)){

        x_position = window_attributes.width / 2;
        y_position = window_attributes.height / 2 ;

        // Dirección, numero de paso y último valor calculado a 0
        direction = step = lastg = 0;

        color ^= 0xFFFFFF; // Si es negro cambia a blanco y al revés
    }

    XSetForeground(display, gc, color);

    // Cálculos del fractal
    int g = step ^ (step >> 1);
    int t = (~g) & lastg;
    lastg = g;

    if (t == 0){ // Se t == 0, giramos 90 graos
        direction = (direction + 1) % 4;
    }
    else{
        // Dado que en C (-1 % 4) == -1,
        // usamos 3 como equivalente % 4
        direction = (direction + 3) % 4;
    }

    // Avanzamos según la dirección
    switch(direction){
    case 0:
        y_position--;
        break;

    case 1:
        x_position++;
        break;

    case 2:
        y_position++;
        break;

    case 3:
        x_position--;
        break;
    }

    XFillRectangle(display, window, gc,
                   x_position, y_position,
                   1, 1);

    // Nos preparamos para el siguiente paso
    step++;
}


/* En la función principal tomaremos la ventana en
   la que escribiremos y la pasamos a la función anterior.
*/
int main (int argc, char **argv){

    /* Tomamos el identificador de pantalla. */
    char *display_id = getenv("DISPLAY");

    /* Suponemos que el display_id no es nulo. */
    assert(display_id != NULL);

    /* Conectamos con la pantalla. */
    Display *display = XOpenDisplay(display_id);

    /* Obtenemos la ventana, es la que mostrará el programa. */
    Window root = DefaultRootWindow(display);

    /* Creamos un contexto gráfico, este es el lienzo
       donde dibujamos.
    */
    GC gc = XCreateGC(display, root, 0, NULL);

    /* Por último leemos las dimensiones de la ventana, 
       que siempre es bueno saber cuales son
    */
    XWindowAttributes window_attributes; 
    XGetWindowAttributes(display, root, &window_attributes);

    /* Y solo queda dar vueltas esperando al usuario */
    while (1){
        /* Dibujamos algo. */
        drawScreen(display, root, window_attributes, gc);

        /* Forzamos los cambios. */
        XFlush(display);

    /* Y le damos un respiro a CPU. */
        usleep(2000);
    }

    /* Esto solo está para que el compilador esté tranquilo ;)  */
    return 0;
}


