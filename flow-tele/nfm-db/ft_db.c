#include <stdint.h>
#include <assert.h>
#include <string.h>

#include "ft_mem.h"
#include "ft_db.h"


typedef struct _ft_db_key_t {
    uint8_t *   key;
    size_t      key_size;
} ft_db_key_t;

typedef struct _ft_db_elem_t {
    ft_db_key_t key;
    void *      data;
} ft_db_elem_t;

typedef struct _ft_db_t {
    uint32_t    count;
    ft_db_elem_t * array;
    uint32_t    size;
    uint64_t    seq_no;
} ft_db_t;

enum {
    FT_DB_MAX_SIZE_ARRAY = 256,
    FT_DB_MAX_SIZE_DLL   = 0,
};

ft_db_err_t
ft_db_init (ft_db_pt *in_db, size_t max_num_elem)
{
    ft_db_t *db = (ft_db_t *)ft_malloc(sizeof(ft_db_t));
    if ((max_num_elem > 0) && (max_num_elem < FT_DB_MAX_SIZE_ARRAY)) {
        db->array = (ft_db_elem_t *)ft_malloc(max_num_elem*sizeof(ft_db_elem_t));
        db->size = max_num_elem;
    }
    (*in_db) = db;
    return FT_DB_SUCCESS;
}

#define FT_DB_IS_KEY_EQUAL(db_key, in_key, in_key_size) \
    ((db_key.key_size == in_key_size) && \
                (memcmp(db_key.key, in_key, in_key_size)==0)) 

ft_db_err_t
ft_db_find_by_key (ft_db_pt db, void *key, size_t key_size, void **out_data)
{
    int found_index = -1;
    for (int i = 0; i < db->size; i++) {
        if((db->array[i].key.key_size == key_size) && 
                (memcmp(db->array[i].key.key, key, key_size)==0)) {
            found_index = i;
            break;
        }
    }
    if (found_index == -1) {
        return FT_DB_ENTRY_DOESNOTEXIST;
    }

    (*out_data) = db->array[found_index].data;
    return FT_DB_SUCCESS;
}

ft_db_err_t
ft_db_find_by_hndl (ft_db_pt db, ft_db_hndl_t hndl, void **out_data)
{
    int found_index  = -1;

    found_index = hndl&0xFF;

    if (found_index == -1) {
        return FT_DB_ENTRY_DOESNOTEXIST;
    }

    (*out_data) = db->array[found_index].data;
    return FT_DB_SUCCESS;
}

ft_db_err_t
ft_db_insert (ft_db_pt db, void *data, void *key, size_t key_size, ft_db_hndl_t *hndl)
{
    int free_index = -1;

    if ((key_size == 0) || (db == NULL) || (key == NULL)) {
        return FT_DB_INVALID_PARAM;
    }

    for (int i = 0; i < db->size; i++) {
        if((db->array[i].key.key_size == key_size) && 
                (memcmp(db->array[i].key.key, key, key_size)==0)) {
            return FT_DB_ENTRY_EXISTS;
        }
        if ((free_index == -1) && (db->array[i].key.key_size > 0)) {
            free_index = i;
        }
    }

    if (db->count == db->size) {
        return FT_DB_FULL;
    }
    assert((free_index >= 0) && (free_index < db->size));

    db->array[free_index].data = data;
    db->array[free_index].key.key = (uint8_t *)ft_malloc(key_size);
    memcpy(db->array[free_index].key.key, key, key_size);
    db->array[free_index].key.key_size = key_size;
    db->count++;

    (*hndl) = free_index;
    return FT_DB_SUCCESS;
}

ft_db_err_t
ft_db_remove_by_key (ft_db_pt db, void *key, size_t key_size, void **out_data)
{
    int found_index = -1;

    for (int i = 0; i < db->size; i++) {
        if (FT_DB_IS_KEY_EQUAL(db->array[i].key, key, key_size)) {
            found_index = i;
        }
    }
    if (found_index == -1) {
        return FT_DB_ENTRY_DOESNOTEXIST;
    }

    (*out_data) = db->array[found_index].data;
    ft_free((void **)&(db->array[found_index].key.key));
    memset(&db->array[found_index], 0, sizeof(ft_db_elem_t));
    db->count--;

    (*out_data) = NULL;
    return FT_DB_SUCCESS;
}

ft_db_err_t
ft_db_remove_by_hndl (ft_db_pt db, ft_db_hndl_t hndl, void **out_data)
{
    int found_index = -1;
    found_index = hndl&0xFF;

    if (found_index == -1) {
        return FT_DB_ENTRY_DOESNOTEXIST;
    }

    (*out_data) = db->array[found_index].data;
    ft_free((void **)&db->array[found_index].key.key);
    memset(&db->array[found_index], 0, sizeof(ft_db_elem_t));
    db->count--;

    (*out_data) = NULL;
    return FT_DB_SUCCESS;
}

