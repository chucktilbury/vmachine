/**
 * @file strings.c
 * @brief String objects.
 *
 * @version 0.1
 * @date 2022-04-22
 *
 */
#include "common.h"
#include "vmachine.h"

typedef struct _string_ {
    char* str;
    size_t index;
    struct _string_* left;
    struct _string_* right;
} String;

typedef struct {
    char** list;
    size_t cap;
    size_t len;
    String* strs;
} StrStore;

static StrStore strs;

/**
 * @brief Initialize the string store.
 */
void createStrStore()
{
    strs.cap = 0x01 << 3;
    strs.len = 0;
    strs.list = _alloc_ds_array(char*, strs.cap);

    String* str = _alloc_ds(String);
    str->str = (char*)_strdup("ERROR");
    str->index = 0;

    strs.list[0] = str->str;
    strs.len = 1;
    strs.strs = str;
    // add it to the tree as the root
}

static void destroy(String* node)
{
    if(node->left != NULL)
        destroy(node->left);
    if(node->right != NULL)
        destroy(node->right);

    if(node->str != NULL)
        _free(node->str);
    _free(node);
}

/**
 * @brief Free the memory associated with a string store.
 */
void destroyStrStore()
{
    if(strs.list != NULL) {
        if(strs.strs != NULL)
            destroy(strs.strs);
        _free(strs.list);
    }
}

/**
 * @brief Find a node in the tree. If the node is not found, then return
 * 0, which represents no node.
 *
 * @param node
 * @param str
 *
 * @return size_t
 */
static size_t find(String* node, const char* str)
{
    int val = strcmp(node->str, str);
    if(val < 0) {
        if(node->left != NULL)
            return find(node->left, str);
        else
            return 0;
    }
    else if(val > 0) {
        if(node->right != NULL)
            return find(node->right, str);
        else
            return 0;
    }
    else
        return node->index;
}

/**
 * @brief Add a node to the tree. If the node already exists, then do nothing.
 *
 * @param tree
 * @param node
 *
 */
static void add(String* tree, String* node)
{
    int val = strcmp(tree->str, node->str);
    if(val < 0) {
        if(tree->left != NULL)
            add(tree->left, node);
        else
            tree->left = node;
    }
    else if(val > 0) {
        if(tree->right != NULL)
            add(tree->right, node);
        else
            tree->right = node;
    }
    // else do nothing because we check for the node first.
}

/**
 * @brief Find the lowest number index that we can use and return it.
 *
 * @return size_t
 */
static size_t get_index()
{
    size_t i = 0;
    for(/* empty */; i < strs.len; i++) {
        if(strs.list[i] == NULL)
            break;
    } // fixme: inefficient search for an empty slot.

    if(i < strs.len) {
        //printf("first return index: %d\n", i);
        return i;   // found an empty slot;
    }
    else {
        if(strs.len+1 > strs.cap) {
            strs.cap <<= 1;
            strs.list = _realloc_ds_array(strs.list, char*, strs.cap);
        }
        i = strs.len;
        strs.len++; // for clarity and debugging
        //printf("second return index: %d\n", i);
        return i;
    }
}

/**
 * @brief Convert an index into a string using the list.
 *
 * @param idx
 *
 * @return const char*
 */
static const char* idx_to_str(size_t index)
{
    if(index > strs.len) {
        genericError("invalid string index: %lX", index);
        return NULL;
    }

    if(strs.list[index] == NULL) {
        genericError("attempt to read a deleted string: %d", index);
        return NULL;
    }

    return strs.list[index];
}

/**
 * @brief Add a string to the string store, checking for duplicates. If one is
 * found then return the index of the original. Otherwise, return the index of
 * the string in the store.
 *
 * @param str
 *
 * @return size_t
 */
size_t addStr(const char* str)
{
    size_t idx = find(strs.strs, str);

    if(idx == 0) {
        String* strobj = _alloc_ds(String);
        strobj->str = (char*)_strdup(str);
        strobj->index = idx = get_index();
        strs.list[strobj->index] = strobj->str;
        add(strs.strs, strobj);
    }

    return idx;
}

/**
 * @brief Remove the node that has the string from the tree. Cannot remove
 * the root node. (and should not)
 *
 * @param tree
 * @param str
 */
static size_t delete(String* tree, const char* str)
{
    int val = 0;
    if(str == NULL)
        return 0;

    if(tree->left != NULL) {
        val = strcmp(tree->left->str, str);
        if(val < 0)
            return delete(tree->left, str);
        else if(val > 0)
            return delete(tree->right, str);
        else {
            String* node = tree->left;
            tree->left = NULL;

            // add orphaned nodes back into the tree.
            if(node->left != NULL)
                add(strs.strs, node->left);
            if(node->right != NULL)
                add(strs.strs, node->right);

            // free the memory
            _free(node->str);
            val = node->index;
            _free(node);
            return (size_t)val;
        }
    }

    if(tree->right != NULL) {
        val = strcmp(tree->right->str, str);
        if(val < 0)
            return delete(tree->left, str);
        else if(val > 0)
            return delete(tree->right, str);
        else {
            String* node = tree->right;
            tree->right = NULL;

            // add orphaned nodes back into the tree.
            if(node->left != NULL)
                add(strs.strs, node->left);
            if(node->right != NULL)
                add(strs.strs, node->right);

            // free the memory
            _free(node->str);
            val = node->index;
            _free(node);
            return (size_t)val;
        }
    }

    return 0;   // node was not found
}

/**
 * @brief Delete a string from the string store. Allows the slot to be reused.
 *
 * @param index
 *
 */
void deleteStr(size_t index)
{
    size_t idx = delete(strs.strs, idx_to_str(index));
    if(idx != 0)
        strs.list[index] = NULL;
}

/**
 * @brief Return the C string from the string store, or NULL if the index is
 * that of a deleted or invalid slot.
 *
 * @param index
 *
 * @return const char*
 */
const char* getStr(size_t index)
{
    //printf("getStr(): %lu\n", index);
    return idx_to_str(index);
}

/**
 * @brief Return the length of the string from the store, or 0 if the index
 * is that of an invalid string.
 *
 * @param index
 *
 * @return size_t
 */
size_t getStrLen(size_t index)
{
    const char* str = idx_to_str(index);
    if(str != NULL)
        return strlen(str);
    else
        return 0;
}

/**
 * @brief Compare the literal string to the string at the index in the store.
 * Returns the result of strcmp().
 *
 * @param index
 * @param str
 *
 * @return int
 */
int compareStr(size_t index, const char* str)
{
    const char* str1 = idx_to_str(index);
    if(str1 != NULL)
        return strcmp(str1, str);
    else
        return -1;
}

/**
 * @brief Concatenate the two strings into one and create a new string in
 * the store with the result. Return the index of the result.
 *
 * @param left
 * @param right
 *
 * @return size_t
 */
size_t arithAddStr(size_t left, size_t right)
{
    const char* str1 = getStr(left);
    size_t str1_len = strlen(str1);
    const char* str2 = getStr(right);
    size_t str2_len = strlen(str2);

    char* outs = _malloc(str1_len + str2_len + 1);
    memcpy(outs, str1, str1_len);
    memcpy(&outs[str1_len], str2, str2_len);

    size_t val = addStr(outs);
    _free(outs);

    return val;
}

/**
 * @brief Serialize an individual string node.
 *
 * @param node
 * @param fp
 *
 */
static void save_node(String* node, FILE* fp)
{
    size_t len = strlen(node->str);
    fwrite(&len, sizeof(len), 1, fp);
    fwrite(&node->index, sizeof(node->index), 1, fp);
    fwrite(node->str, sizeof(char), len, fp);
}

/**
 * @brief Recursively access each string node.
 *
 * @param node
 * @param fp
 *
 */
static void save(String* node, FILE* fp)
{
    if(node->left != NULL)
        save(node->left, fp);
    if(node->right != NULL)
        save(node->right, fp);

    save_node(node, fp);
}

/**
 * @brief Write the entire string store to disk.
 *
 * @param fp
 */
void saveStrStore(FILE* fp)
{
    fwrite(&strs.len, sizeof(strs.len), 1, fp);
    save(strs.strs, fp);
}

/**
 * @brief Read the entire string store from disk and init the global with
 * the data.
 *
 * @param fp
 */
void loadStrStore(FILE* fp)
{
    size_t len;

    fread(&len, sizeof(len), 1, fp);
    //printf("len: %lu\n", len);
    strs.len = len;
    strs.cap = 1;
    while(strs.cap < strs.len+1)
        strs.cap <<= 1;

    strs.list = _alloc_ds_array(char*, strs.cap);

    String* str = _alloc_ds(String);
    str->str = (char*)_strdup("ERROR");
    str->index = 0;

    strs.list[0] = str->str;
    strs.len = 1;
    strs.strs = str;

    size_t read_len;
    size_t index;
    for(size_t i = 0; i < len; i++) {
        fread(&read_len, sizeof(read_len), 1, fp);
        fread(&index, sizeof(index), 1, fp);
        char* buf = _malloc(read_len+1);
        fread(buf, sizeof(char), read_len, fp);

        String* str = _alloc_ds(String);
        str->str = buf;
        str->index = index;
        add(strs.strs, str);
        strs.list[index] = buf;
        strs.len++;
    }
}

static void dump(String* node)
{
    if(node->left != NULL)
        dump(node->left);
    if(node->right != NULL)
        dump(node->right);

    printf("  %lu: \"%s\"\n", node->index, node->str);
}

/**
 * @brief Dump the string nodes to stdout for debugging.
 */
void dumpStrStore(int level)
{
    if(level <= getTraceLevel()) {
        printf("\nString Store:\n");
        dump(strs.strs);
    }
}
