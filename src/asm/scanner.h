#ifndef _SCANNER_H_
#define _SCANNER_H_

int open_file(const char *fname);
char *get_file_name(void);
int get_line_number(void);
int get_col_number(void);
const char *get_tok_str(void);

#define fatal_error(fmt, ...) do { \
            fprintf(stderr, "Fatal Error: "); \
            fprintf(stderr, fmt, ## __VA_ARGS__ ); \
            fprintf(stderr, "\n"); \
            exit(1); \
        } while(0)


#endif