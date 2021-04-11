#include <stdio.h>


#include "ft_db.h"

int main(int argc, const char *argv[])
{
    ft_db_pt db;
    int max_num = 100;

    ft_db_init(&db, max_num);

    int data = 64;
    int *key = &data;
    ft_db_handle_t handle;

    ft_db_insert(db, &data, key, sizeof(*key), &handle);

    int *found_data;

    ft_db_find_by_key(db, &data, key, sizeof(*key), &found_data);
    printf("data:%d key:%d handle:%llu found_data:%d", data, *key, handle, *found_data);

    ft_db_find_by_hndl(db, handle, &found_data);
    printf("data:%d key:%d handle:%llu found_data:%d", data, *key, handle, *found_data);

    ft_db_remove_by_key(db, key, sizeof(*key));
    return 0;
}
