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

static StringObj* strings;

static inline StringObj* idx_to_strobj(size_t idx)
{
    Object obj = getObj(idx);
    if(obj.type != OBJ_STRING) {
        genericError("expected a string object, but got a %s", objTypeToStr(obj.type));
        return NULL;
    }

    // return the string object
    return (StringObj*)obj.obj;
}

static StringObj* find(StringObj* obj, const char* str)
{
    int cmp = strcmp(obj->str, str);
    if(cmp > 0) {
        if(obj->left != NULL)
            return find(obj->left, str);
        else
            return NULL;
    }
    else if(cmp < 0) {
        if(obj->right != NULL)
            return find(obj->right, str);
        else
            return NULL;
    }
    else
        return obj;
}

static void add(StringObj* tree, StringObj* node)
{
    int cmp = strcmp(tree->str, node->str);
    if(cmp > 0) {
        if(tree->left != NULL)
            add(tree->left, node);
        else
            tree->left = node;
    }
    else if(cmp < 0) {
        if(tree->right != NULL)
            add(tree->right, node);
        else
            tree->right = node;
    }
    else {
        // never should happen because we check first
        genericError("unexpected string object error");
    }
}

size_t addStr(const char* str)
{
    // first see if the string already exists. If it does, then return it.
    StringObj* sobj = find(strings, str);
    if(sobj != NULL)
        return sobj->obj_index;

    // create a new string object
    sobj = _alloc_ds(StringObj);
    sobj->len = strlen(str);
    sobj->str = _strdup(str);

    // place the string into the string store
    Object obj;
    obj.type = OBJ_STRING;
    obj.obj = sobj;
    sobj->obj_index = addObj(obj);
    add(strings, sobj);

    // return the index of the object in the object store
    return sobj->obj_index;
}

void deleteStrObj(size_t index)
{
    // remove the string from the tree
    // remove the object
}

const char* getStrObj(size_t index)
{
    // return the string text
    StringObj* sobj = idx_to_strobj(index);
    return sobj->str;
}

size_t getStrObjLen(size_t index)
{
    StringObj* sobj = idx_to_strobj(index);
    return sobj->len;
}

bool compareStrObj(size_t index, const char* str)
{
    // return true if the strings are the same.
    StringObj* sobj = idx_to_strobj(index);
    return (strcmp(sobj->str, str) == 0)? true: false;
}

// concatenate the str to the existing string object.
size_t catStrObj(size_t index, const char* str)
{

}

// save the string to a file.
void saveStrObj(StringObj* obj, FILE* fp)
{

}

// load a string from a file.
StringObj* loadStrObj(FILE* fp)
{

}
