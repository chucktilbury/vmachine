/*
 * Generic command line that sort of works like a simplified Python command
 * line parser.
 *  1.  create_cmd_line()
 *  2.  add_xxx_param() for each parameter to be parsed.
 *  3.  parse_cmd_line() to read the command line and parse it into the
 *      parameters.
 *  4.  get_xxx_param() to read the values that were parsed from the command
 *      line.
 *
 * If an invalid parameter or some other command line error happens then text
 * is produced on stderr to describe the command parameters that have been
 * specified.
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

#include "cmd_line.h"
#include "memory.h"

// Define this to 1 or 0 to turn the excess file functionality off or on
#ifndef USE_EXCESS
    #define USE_EXCESS 1
#endif
// Define this to 1 or 0 to require excess file names
#ifndef EXCESS_REQUIRED
    #define EXCESS_REQUIRED 1
#endif

typedef struct _cpt_ {
    const char* name;
    const char* parm;
    const char* help;
    union {
        int ival;
        bool bval;
        const char* sval;
    } value;
    cmd_flags_t flags;
    cmd_type_t type;
} cmd_parameter_t;

typedef struct {
    const char* docs;
    const char* prog;
    cmd_parameter_t** clist;
    size_t cap;
    size_t len;
#if USE_EXCESS != 0
    char** excess;
    size_t ecap;
    size_t elen;
    size_t eidx;
#endif
} command_line_t;

static void destroy_parm(cmd_parameter_t* parm)
{

    if(parm != NULL) {
        if(parm->name != NULL) {
            _free((void*)parm->name);
        }

        if(parm->parm != NULL) {
            _free((void*)parm->parm);
        }

        if(parm->help != NULL) {
            _free((void*)parm->help);
        }

        if(parm->type == CT_STR) {
            _free((void*)parm->value.sval);
        }

        _free(parm);
    }
}

static void add_node(command_line_t* cmd, cmd_parameter_t* parm)
{

    assert(cmd != NULL);
    assert(parm != NULL);

    if(cmd->len + 1 > cmd->cap) {
        cmd->cap <<= 1;
        cmd->clist = (cmd_parameter_t**)_realloc(cmd->clist, sizeof(cmd_parameter_t*) * cmd->cap);
    }

    cmd->clist[cmd->len] = parm;
    cmd->len++;
}

static cmd_parameter_t* find_node(command_line_t* cmd, const char* key)
{

    assert(cmd != NULL);
    assert(key != NULL);

    for(size_t i = 0; i < cmd->len; i++) {
        if(strcmp(key, cmd->clist[i]->name) == 0) {
            return cmd->clist[i];
        }
    }

    return NULL;
}

/*
 * Create the main data structure for the command line and provide some
 * documentation about what the program is supposed to do in the case of
 * an error or if the "-h" parameter is found in the command line.
 */
cmd_line create_cmd_line(const char* docs)
{

    command_line_t* line = _malloc(sizeof(command_line_t));
    if(docs != NULL) {
        line->docs = _strdup(docs);
    }
    else {
        docs = NULL;
    }
    line->cap = 0x01 << 3;
    line->len = 0;
    line->clist = (cmd_parameter_t**)_malloc(sizeof(cmd_parameter_t*) * line->cap);
#if USE_EXCESS != 0
    line->ecap = 0x01 << 3;
    line->elen = 0;
    line->excess = (char**)_malloc(sizeof(char*) * line->ecap);
    line->eidx = 0;
#endif
    return line;
}

/*
 * Destroy the command line parser to free all memory.
 */
void destroy_cmd_line(cmd_line cmd)
{

    if(cmd != NULL) {
        command_line_t* line = (command_line_t*)cmd;

        if(line->docs != NULL) {
            _free((void*)line->docs);
        }

        if(line->clist != NULL) {
            for(size_t i = 0; i < line->len; i++) {
                destroy_parm(line->clist[i]);
            }
        }
#if USE_EXCESS != 0
        for(size_t idx = 0; idx < line->elen; idx++) {
            _free((void*)line->excess[idx]);
        }
        _free(line->excess);
#endif
        _free(line->clist);
        _free(line);
    }
}

/*
 * Add a string command line parameter to the parser. Any normal command line
 * string is accepted. If the string has spaces, then it must be enclosed in
 * double quotes.
 */
void add_str_param(cmd_line cptr, const char* name, const char* parm, const char* help, const char* val,
                   cmd_flags_t flags)
{

    assert(cptr != NULL);
    assert(parm != NULL);
    assert(strlen(parm) > 0);

    command_line_t* cmd = (command_line_t*)cptr;
    cmd_parameter_t* p = _malloc(sizeof(cmd_parameter_t));
    p->parm = _strdup(parm);
    p->name = _strdup(name);
    p->help = _strdup(help);
    p->flags = flags;
    p->type = CT_STR;
    p->value.sval = _strdup(val);

    add_node(cmd, p);
}

/*
 * Add a boolean command line parameter to the parser. Acceptible values are
 * 1, 0, true, and false. The parameters are upcased before they are checked
 * for validity.
 */
void add_bool_param(cmd_line cptr, const char* name, const char* parm, const char* help, bool val, cmd_flags_t flags)
{

    assert(cptr != NULL);
    assert(parm != NULL);
    assert(strlen(parm) > 0);

    command_line_t* cmd = (command_line_t*)cptr;
    cmd_parameter_t* p = _malloc(sizeof(cmd_parameter_t));
    p->parm = _strdup(parm);
    p->help = _strdup(help);
    p->name = _strdup(name);
    p->flags = flags;
    p->type = CT_BOOL;
    p->value.bval = val;

    add_node(cmd, p);
}

/*
 * Add a numeric command line parameter to the parser. All numbers must be
 * numbers that are less than MAX_INT and greater than MIN_INT. strtol() is
 * used to convert the string into a number.
 */
void add_num_param(cmd_line cptr, const char* name, const char* parm, const char* help, int val, cmd_flags_t flags)
{

    assert(cptr != NULL);
    assert(parm != NULL);
    assert(strlen(parm) > 0);

    command_line_t* cmd = (command_line_t*)cptr;
    cmd_parameter_t* p = _malloc(sizeof(cmd_parameter_t));
    p->parm = _strdup(parm);
    p->help = _strdup(help);
    p->name = _strdup(name);
    p->flags = flags;
    p->type = CT_NUM;
    p->value.ival = val;

    add_node(cmd, p);
}

/*
 * Add a boolean command line parameter to the parser. If this parameter is
 * present, then the value is set to true. Otherwise, it is false.
 */
void add_none_param(cmd_line cptr, const char* name, const char* parm, const char* help, cmd_flags_t flags)
{

    assert(cptr != NULL);
    assert(parm != NULL);
    assert(strlen(parm) > 0);

    command_line_t* cmd = (command_line_t*)cptr;
    cmd_parameter_t* p = _malloc(sizeof(cmd_parameter_t));
    p->parm = _strdup(parm);
    p->help = _strdup(help);
    p->name = _strdup(name);
    p->flags = flags;
    p->type = CT_NONE;
    p->value.bval = false;

    add_node(cmd, p);
}

/*
 * Get the value associated with a string command line parameter. Could
 * return NULL if the parameter is not required, it's was not provided
 * and there is no default.
 */
const char* get_str_param(cmd_line cptr, const char* name)
{

    command_line_t* cmd = (command_line_t*)cptr;
    cmd_parameter_t* p = find_node(cmd, name);
    assert(p != NULL);

    return p->value.sval;
}

/*
 * Return a boolean command parameter.
 */
bool get_bool_param(cmd_line cptr, const char* name)
{

    command_line_t* cmd = (command_line_t*)cptr;
    cmd_parameter_t* p = find_node(cmd, name);
    assert(p != NULL);

    return p->value.bval;
}

/*
 * Return a numerical command parameter as an int, which could be a signed
 * value from the command line.
 */
int get_num_param(cmd_line cptr, const char* name)
{

    command_line_t* cmd = (command_line_t*)cptr;
    cmd_parameter_t* p = find_node(cmd, name);
    assert(p != NULL);

    return p->value.ival;
}

/*
 * Return a numerical command parameter as an int, which could be a signed
 * value from the command line.
 */
bool get_none_param(cmd_line cptr, const char* name)
{

    command_line_t* cmd = (command_line_t*)cptr;
    cmd_parameter_t* p = find_node(cmd, name);
    assert(p != NULL);

    return p->value.bval;
}

/*
 * Actually read the command line and fill out the data structures. do this
 * after all of the parameters have been added, but before any attempt to
 * read the value of a command parameter.
 */
void parse_cmd_line(cmd_line cptr, int argc, char** argv)
{

    command_line_t* cmd = (command_line_t*)cptr;

    cmd->prog = _strdup(argv[0]);

    for(int args_idx = 1; args_idx < argc; args_idx++) {
        size_t parm_idx;
        for(parm_idx = 0; parm_idx < cmd->len; parm_idx++) {
            if(0 == strcmp(cmd->clist[parm_idx]->parm, argv[args_idx])) {
                break;    // found it.
            }
        }

        if(parm_idx >= cmd->len) {
            if(0 != strcmp(argv[args_idx], "-h") &&
                    0 != strcmp(argv[args_idx], "-help") &&
                    0 != strcmp(argv[args_idx], "--h") &&
                    0 != strcmp(argv[args_idx], "--help") &&
                    0 != strcmp(argv[args_idx], "-?")) {
                // unknown parm
#if USE_EXCESS != 0
                if(argv[args_idx][0] != '-') {
                    // add it to the excess list
                    if(cmd->elen + 1 > cmd->ecap) {
                        cmd->ecap <<= 1;
                        cmd->excess = _realloc(cmd->excess, sizeof(char*)*cmd->ecap);
                    }
                    cmd->excess[cmd->elen] = (char*)_strdup(argv[args_idx]);
                    cmd->elen++;
                    continue; // outer for() loop
                }
                else {
                    fprintf(stderr, "cmd_err: unknown command parameter: \"%s\"\n", argv[args_idx]);
                    cmd_use(cmd); // does not return
                }
#else
                fprintf(stderr, "cmd_err: unknown command parameter: \"%s\"\n", argv[args_idx]);
                cmd_use(cmd); // does not return
#endif
            }
            else {
                cmd_use(cmd);    // does not return
            }

        }

        switch(cmd->clist[parm_idx]->type) {
            case CT_BOOL: { // has a bool parameter, can be 0, 1, true or false
                    if(args_idx + 1 >= argc) {
                        fprintf(stderr, "cmd_err: missing arg for \"%s\"\n", argv[args_idx]);
                        cmd_use(cmd);
                    }

                    int ch = argv[args_idx + 1][0];
                    switch(ch) {
                        case '0':
                        case 'f':
                        case 'F':
                            if(0 != strcmp("0", argv[args_idx + 1]) &&
                                    0 != strcmp("false", argv[args_idx + 1]) &&
                                    0 != strcmp("False", argv[args_idx + 1]) &&
                                    0 != strcmp("FALSE", argv[args_idx + 1])) {
                                fprintf(stderr, "cmd_err: invalid boolean: \"%s\": must be '0', '1', 'true', or 'false'\n", argv[args_idx + 1]);
                                cmd_use(cmd);
                            }
                            else {
                                cmd->clist[parm_idx]->value.bval = false;
                                cmd->clist[parm_idx]->flags |= CF_PRESENT;
                                args_idx++;
                            }
                            break;
                        case '1':
                        case 't':
                        case 'T':
                            if(0 != strcmp("1", argv[args_idx + 1]) &&
                                    0 != strcmp("true", argv[args_idx + 1]) &&
                                    0 != strcmp("True", argv[args_idx + 1]) &&
                                    0 != strcmp("TRUE", argv[args_idx + 1])) {
                                fprintf(stderr, "cmd_err: invalid boolean: \"%s\": must be '0', '1', 'true', or 'false'\n", argv[args_idx + 1]);
                                cmd_use(cmd);
                            }
                            else {
                                cmd->clist[parm_idx]->value.bval = true;
                                cmd->clist[parm_idx]->flags |= CF_PRESENT;
                                args_idx++;
                            }
                            break;
                        default:
                            fprintf(stderr, "cmd_err: invalid boolean: \"%s\": must be '0', '1', 'true', or 'false'\n", argv[args_idx + 1]);
                            cmd_use(cmd);
                    }
                }
                break;

            case CT_NUM:  { // has a signed integer parameter
                    if(args_idx + 1 >= argc) {
                        fprintf(stderr, "cmd_err: missing arg for \"%s\"\n", argv[args_idx]);
                        cmd_use(cmd);
                    }

                    int result = 0;
                    bool neg = false;
                    const char* str = argv[args_idx + 1];
                    if(str[0] == '-') {
                        neg = true;
                    }
                    for(int i = 0; str[i] != '\0'; i++) {
                        if(isdigit(str[i])) {
                            result *= 10;
                            result += str[i] - '0';
                        }
                        else {
                            fprintf(stderr, "cmd_err: invalid number: \"%s\"\n", argv[args_idx + 1]);
                            cmd_use(cmd);
                        }
                    }
                    if(neg) {
                        result = -result;
                    }

                    cmd->clist[parm_idx]->value.ival = result;
                    cmd->clist[parm_idx]->flags |= CF_PRESENT;
                    args_idx++;
                }
                break;

            case CT_STR:  { // has a string parameter
                    if(args_idx + 1 >= argc) {
                        fprintf(stderr, "cmd_err: missing arg for \"%s\"\n", argv[args_idx]);
                        cmd_use(cmd);
                    }

                    cmd->clist[parm_idx]->value.sval = _strdup(argv[args_idx + 1]);
                    cmd->clist[parm_idx]->flags |= CF_PRESENT;
                    args_idx++;
                }
                break;

            case CT_NONE: { // has no parameter, AKA a switch
                    cmd->clist[parm_idx]->flags |= CF_PRESENT;
                    cmd->clist[parm_idx]->value.bval = true;
                }
                break;

            default:        // unknown type should be impossible.
                fprintf(stderr, "cmd_err: internal error: invalid parameter type\n");
                exit(1);
                break;
        }

    }

    // check for all required params.
    cmd_parameter_t** lst = cmd->clist;
    for(size_t idx = 0; idx < cmd->len; idx++) {
        if(lst[idx]->flags & CF_REQD && !(lst[idx]->flags & CF_PRESENT)) {
            fprintf(stderr, "cmd_err: required parameter not present: \"%s\"\n", lst[idx]->parm);
            cmd_use(cmd);
        }
    }

#if USE_EXCESS != 0
#if EXCESS_REQUIRED != 0
    if(cmd->elen == 0) {
        fprintf(stderr, "cmd_err: required file name list not present\n");
        cmd_use(cmd);
    }
#endif
#endif
}

/*
 * Display text that appears with the "help" parameter. This text is
 * automatically generated from the cmd_line data structure.
 */
void cmd_use(cmd_line cmd)
{

    command_line_t* line = (command_line_t*)cmd;
    cmd_parameter_t** p = line->clist;

#if USE_EXCESS != 0
    fprintf(stderr, "\nuse: %s <parameters> <files>\n", line->prog);
#else
    fprintf(stderr, "\nuse: %s <parameters>\n", line->prog);
#endif
    fprintf(stderr, "%s\n\n", line->docs);
    for(size_t idx = 0; idx < line->len; idx++) {
        switch(p[idx]->type) {
            case CT_BOOL:
                fprintf(stderr, "  %-4s <bool> %s (%s)\n",
                        p[idx]->parm, p[idx]->help,
                        p[idx]->value.bval ? "true" : "false");
                break;
            case CT_NONE:
                fprintf(stderr, "  %-4s -----  %s (%s)\n",
                        p[idx]->parm, p[idx]->help,
                        p[idx]->value.bval ? "true" : "false");
                break;
            case CT_NUM:
                fprintf(stderr, "  %-4s <num>  %s (%d)\n",
                        p[idx]->parm, p[idx]->help,
                        p[idx]->value.ival);
                break;
            case CT_STR:
                fprintf(stderr, "  %-4s <str>  %s (%s)\n",
                        p[idx]->parm, p[idx]->help,
                        p[idx]->value.sval);
                break;
            default: // unknown type should be impossible.
                fprintf(stderr, "cmd_err: internal error: invalid parameter type\n");
                exit(1);
                break;
        }
    }

    fprintf(stderr, "  -h   -----  Print this helpful information.\n\n");
    exit(1);
}

#if USE_EXCESS != 0
/*
 * Reset the excess parameter list prior to iterating it.
 */
void reset_cmd_excess(cmd_line cptr)
{

    command_line_t* cmd = (command_line_t*)cptr;
    cmd->eidx = 0;
}

/*
 * Each time this is called, the next excess parameter is returned. When there
 * are no more entries, then return NULL. Is starting from the beginning is
 * required, then the reset routine must be called first.
 */
char* iterate_cmd_excess(cmd_line cptr)
{

    command_line_t* cmd = (command_line_t*)cptr;
    char* ptr;

    if(cmd->eidx < cmd->elen) {
        ptr = cmd->excess[cmd->eidx];
        cmd->eidx++;
    }
    else {
        ptr = NULL;
    }

    return ptr;
}
#endif

/*
 * This routine is used for debugging and is not a part of the normal user
 * interface.
 */
void dump_cmd_line(cmd_line cptr)
{

    command_line_t* cmd = (command_line_t*)cptr;
    cmd_parameter_t** p = cmd->clist;

    fprintf(stderr, "\nDUMP COMMAND LINE PARAMTERS\n");
    fprintf(stderr, "\n%s\n", cmd->docs);
    for(size_t idx = 0; idx < cmd->len; idx++) {
        switch(p[idx]->type) {
            case CT_BOOL:
                fprintf(stderr, "  %-4s <bool> %s (required=%s) (default=%s)\n",
                        p[idx]->parm, p[idx]->help,
                        p[idx]->flags & CF_REQD ? "true" : "false",
                        p[idx]->value.bval ? "true" : "false");
                break;
            case CT_NONE:
                fprintf(stderr, "  %-4s -----  %s (required=%s) (default=%s)\n",
                        p[idx]->parm, p[idx]->help,
                        p[idx]->flags & CF_REQD ? "true" : "false",
                        p[idx]->value.bval ? "true" : "false");
                break;
            case CT_NUM:
                fprintf(stderr, "  %-4s <num>  %s (required=%s) (default=%d)\n",
                        p[idx]->parm, p[idx]->help,
                        p[idx]->flags & CF_REQD ? "true" : "false",
                        p[idx]->value.ival);
                break;
            case CT_STR:
                fprintf(stderr, "  %-4s <str>  %s (required=%s) (default=%s)\n",
                        p[idx]->parm, p[idx]->help,
                        p[idx]->flags & CF_REQD ? "true" : "false",
                        p[idx]->value.sval);
                break;
            default: // unknown type should be impossible.
                fprintf(stderr, "cmd_err: internal error: invalid parameter type\n");
                exit(1);
                break;
        }
    }
#if USE_EXCESS != 0
    fprintf(stderr, "\nexcess strings:\n");
    reset_cmd_excess(cptr);
    char* str;
    while((str = iterate_cmd_excess(cptr)) != NULL) {
        fprintf(stderr, "    %s\n", str);
    }
#endif
    fprintf(stderr, "\n");
}
