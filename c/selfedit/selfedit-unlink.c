#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include <assert.h>
#include <sys/stat.h> /* chmod() */

/* Modification point type */
struct modpoint {
    char m1[16];
    uint32_t v;     
};
  
/* Modification point data */
volatile volatile struct modpoint modp = {
    .m1 = ">>>>>>>>>>>>>>>>",
    .v  = 0,
};

/**
 * File loading routine.
 * Loads everything to memory, don't bother with efficiency, it's just a test.
 * Save filesize to a given pointer.
 * 
 */
uint8_t *load_file(char *fname, int *filesize){
    FILE *f = fopen(fname, "rb");
    assert(f != NULL);

    fseek(f, 0, SEEK_END);
    *filesize = ftell(f);

    uint8_t *contents = malloc(sizeof(uint8_t) * (*filesize));
    assert(contents != NULL);

    rewind(f);
    int to_read = *filesize;
    uint8_t *cptr = contents;
    while (to_read > 0){
        int read_step = fread(cptr, sizeof(uint8_t), to_read, f);
        cptr += read_step;  
        to_read -= read_step;
    }
    fclose(f);
   
    return contents;
}   
 
   
/** 
 * Locate the modification point position inside the file contents. 
 * 
 */ 
char *locate_value(char *contents, int filesize){
    char *needle = memmem(contents, filesize, modp.m1, 16);
    return needle == NULL? NULL : needle + sizeof(char) * 16;
}  


/**
 * Show the value stored in the file.
 * 
 */
void show_value(char *fname){
    int filesize;
    uint8_t *contents = load_file(fname, &filesize);
    uint32_t *value = (uint32_t*) locate_value((char*)contents, filesize);
    assert(value != NULL);

    printf("Stored: %i\n", *((uint32_t*) value));

    free(contents);
}


/**
 * Change the value stored in the file.
 * 
 */
void set_value(char *fname, uint32_t new_value){
    int filesize;
    uint8_t *contents = load_file(fname, &filesize);
    uint32_t *fvalue = (uint32_t*) locate_value((char*)contents, filesize);
    assert(fvalue != NULL);
  
    *fvalue = new_value;
  
    /* Remove old file */
    remove(fname);
   
    /* Generate new file */
    FILE *out = fopen(fname, "wb");
    assert(out != NULL);
    uint8_t *cptr = contents;
    int to_write = filesize;
    while (to_write > 0){
        int write_step = fwrite(cptr, sizeof(uint8_t), to_write, out);
        cptr += write_step;
        to_write -= write_step;
    } 
    fclose(out);

    /* Set new file as readable and executable */
    assert(chmod(fname, S_IXUSR | S_IRUSR) == 0);
}
 

int main(int argc, char **argv){
    show_value(argv[0]);
    if (argc == 2){
        uint32_t num;
        char *endptr;
        num = strtol(argv[1], &endptr, 10);
        if (endptr == argv[1]){ 
            fprintf(stderr, "%s [<number to set>]\n", argv[0]);
        }
        else {
            set_value(argv[0], num);
        }
    }
    return 0;
}
