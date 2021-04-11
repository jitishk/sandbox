#ifndef FT_DB_H
#define FT_DB_H

typedef enum {
    FT_DB_SUCCESS = 0,
    FT_DB_ENTRY_EXISTS = 1,
    FT_DB_FULL = 2,
    FT_DB_INVALID_PARAM = 3,
    FT_DB_ENTRY_DOESNOTEXIST = 4,
} ft_db_err_t;

typedef struct _ft_db_t *ft_db_pt;

typedef uint64_t ft_db_hndl_t;


ft_db_err_t
ft_db_insert (ft_db_pt db, void *data, void *key, size_t key_size, ft_db_hndl_t *hndl);

ft_db_err_t
ft_db_init (ft_db_pt *in_db, size_t max_num_elem);

ft_db_err_t
ft_db_find_by_key (ft_db_pt db, void *key, size_t key_size, void **out_data);

ft_db_err_t
ft_db_find_by_hndl (ft_db_pt db, ft_db_hndl_t hndl, void **out_data);

ft_db_err_t
ft_db_remove_by_key (ft_db_pt db, void *key, size_t key_size, void **out_data);

ft_db_err_t
ft_db_remove_by_hndl (ft_db_pt db, ft_db_hndl_t hndl, void **out_data);

#endif /* end of include guard: FT_DB_H */
