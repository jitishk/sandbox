#include <glib.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct _key_t {
    uint8_t key[5];
} key_t;

typedef struct _payload_t {
    key_t key;
    uint8_t value[100];
} payload_t;


void print_data(gpointer in_key, gpointer in_payload, gpointer params)
{
    key_t *key = (key_t *) in_key;
    payload_t *payload = (payload_t *) in_payload;

    if (!key || !payload) {
        printf("null key (%p) or payload (%p)\n", key, payload);
        return;
    }

    printf("%d: %s\n", key->key[4], payload->value);
}

int main(int argc, const char *argv[])
{
    GHashTable *hash = g_hash_table_new(NULL, NULL);
    payload_t *payload;
    key_t *key;
    for (int i=0; i<100; i++) {
        key = (key_t *) malloc(sizeof(key_t));
        key->key[4] = i;
        payload = (payload_t *)malloc(sizeof(payload_t));
        payload->key.key[4] = i;
        snprintf((char *)payload->value, sizeof(payload->value), "payload %d", i);
        g_hash_table_insert(hash, &payload->key, payload);
    }
    g_hash_table_foreach(hash, print_data, NULL);
    return 0;
}

