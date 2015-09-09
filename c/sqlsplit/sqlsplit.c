/*
 * Copyright © 2014 kenkeiras <me@codigoparallevar.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See http://www.wtfpl.net/ for more details.
 *
 */

#include <stdio.h>
#include <string.h>
#include <assert.h>

#define BUFFER_SIZE 4096

enum parsing_state {
    parsing_string,
    parsing_string_escaped,
    parsing_commented_line,
    parsing_bound_comment,
    parsing_bound_comment_found_asterisk,
    parsing_dash,
    parsing_slash,
    parsing_other
};


int sqlsplit(FILE *input, FILE *output){

    // Useful macros
#define WRITE_FULL_BUFFER(buffer, buffer_pos, buffer_len, file) \
    while(buffer_pos < buffer_len){                             \
        buffer_pos += fwrite(&buffer[buffer_pos], sizeof(char), \
                             buffer_len - buffer_pos, file);    \
    }

#define TO_OUT_BUFFER(c)                        \
    outputted = 1;                              \
    if ((out_buffer_len + 10) >= BUFFER_SIZE){  \
        out_buffer_pos = 0;                     \
        WRITE_FULL_BUFFER(out_buffer,           \
                          out_buffer_pos,       \
                          out_buffer_len,       \
                          output);              \
        out_buffer_len = 0;                     \
    }                                           \
    out_buffer[out_buffer_len++] = c;



    enum parsing_state state = parsing_other;

    char in_buffer[BUFFER_SIZE * 2];
    char out_buffer[BUFFER_SIZE];

    int in_buffer_pos = 0,
        in_buffer_len = 0,
        out_buffer_len = 0,
        out_buffer_pos = 0;

    int parens_level = 0;

    char string_delimiter = '\'';

    while ((!feof(input)) || (in_buffer_pos < in_buffer_len)){
        if (in_buffer_pos >= in_buffer_len){
            in_buffer_pos = 0;
            in_buffer_len = fread(in_buffer, sizeof(char), BUFFER_SIZE * 2, input);
        }

        if (in_buffer_pos < in_buffer_len){
            char c = in_buffer[in_buffer_pos++];
            int outputted = 0;

            if (c != '\r'){

                outputted = 0;

                switch(state){
                case parsing_string:
                    TO_OUT_BUFFER(c);
                    if (c == string_delimiter){
                        state = parsing_other;
                    }
                    else if(c == '\\'){
                        state = parsing_string_escaped;
                    }
                    break;

                case parsing_string_escaped:
                    TO_OUT_BUFFER(c);
                    state = parsing_string;
                    break;

                case parsing_commented_line:
                    TO_OUT_BUFFER(c);
                    if (c == '\n'){
                        state = parsing_other;
                    }
                    break;

                case parsing_bound_comment:
                    TO_OUT_BUFFER(c);
                    if (c == '*'){
                        state = parsing_bound_comment_found_asterisk;
                    }
                    break;

                case parsing_bound_comment_found_asterisk:
                    TO_OUT_BUFFER(c);
                    if (c == '/'){
                        state = parsing_other;
                    }
                    else{
                        state = parsing_bound_comment;
                    }
                    break;

                case parsing_dash:
                    if (c == '-'){
                        state = parsing_commented_line;
                        TO_OUT_BUFFER(c);
                    }
                    else{
                        state = parsing_other;
                    }
                    break;

                case parsing_slash:
                    if (c == '*'){
                        state = parsing_bound_comment;
                        TO_OUT_BUFFER(c);
                    }
                    else{
                        state = parsing_other;
                    }
                    break;

                case parsing_other: break;
                }

                if (!outputted){
                    assert(state == parsing_other);
                    switch (c) {
                    case '\'': case '"': case '`':
                        TO_OUT_BUFFER(c);
                        state = parsing_string;
                        string_delimiter = c;
                        break;

                    case '(':
                        if (parens_level == 0){
                            TO_OUT_BUFFER('\n');

                            // Indentation
                            TO_OUT_BUFFER(' ');
                            TO_OUT_BUFFER(' ');
                            TO_OUT_BUFFER(' ');
                            TO_OUT_BUFFER(' ');
                        }
                        TO_OUT_BUFFER(c);
                        parens_level++;
                        break;

                    case ')':
                        TO_OUT_BUFFER(c);
                        parens_level--;
                        assert(parens_level >= 0);
                        break;

                    case '-':
                        TO_OUT_BUFFER(c);
                        state = parsing_dash;
                        break;

                    case '/':
                        TO_OUT_BUFFER(c);
                        state = parsing_slash;
                        break;

                    default:
                        TO_OUT_BUFFER(c);
                    }
                    assert(outputted);
                }
            }
        }
    }

    out_buffer_pos = 0;
    WRITE_FULL_BUFFER(out_buffer, out_buffer_pos, out_buffer_len, output);

    return 0;

}
#undef WRITE_FULL_BUFFER
#undef TO_OUT_BUFFER



int main(int argc, char** argv){
    if (argc != 3){
        printf("%s <input> <output>\n", argv[0] == NULL? "sqlsplit": argv[0]);
        return 0;
    }

    FILE *input = fopen(argv[1], "rb");
    if (input == NULL){
        perror(argv[1]);
        return 1;
    }

    FILE *output = fopen(argv[2], "wb");
    if (output == NULL){
        perror(argv[2]);
        return 2;
    }

    return sqlsplit(input, output);
}
