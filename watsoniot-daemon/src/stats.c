#include "main.h"

enum {
    TOTAL_MEMORY,
    FREE_MEMORY,
    SHARED_MEMORY,
    BUFFERED_MEMORY,
    __MEMORY_MAX,
};

enum {
    MEMORY_DATA,
    __INFO_MAX,
};

static const struct blobmsg_policy memory_policy[__MEMORY_MAX] = {
    [TOTAL_MEMORY] = { .name = "total", .type = BLOBMSG_TYPE_INT64 },
    [FREE_MEMORY] = { .name = "free", .type = BLOBMSG_TYPE_INT64 },
    [SHARED_MEMORY] = { .name = "shared", .type = BLOBMSG_TYPE_INT64 },
    [BUFFERED_MEMORY] = { .name = "buffered", .type = BLOBMSG_TYPE_INT64 },
};

static const struct blobmsg_policy info_policy[__INFO_MAX] = {
    [MEMORY_DATA] = { .name = "memory", .type = BLOBMSG_TYPE_TABLE },
};

typedef struct {
    char str[MAX_FORMAT_SIZE];
} format_str_t;

static format_str_t get_formatted_size(uint64_t size)
{
    int div = 0, rem = 0;
    const char *sizes[] = {
        "B", "kB", "MB", "GB"
    };

    while (size >= 1024 && div < (sizeof(sizes) / sizeof(*sizes))) {
        rem = (size % 1024);
        div++;
        size /= 1024;
    }

    format_str_t fs;
    sprintf(
        fs.str,
        "%.2f %s",
        (float) size + (float) rem / 1024.f,
        sizes[div]
    );

    return fs;
}

static void board_cb(struct ubus_request *req, int type, struct blob_attr *msg)
{
    struct blob_attr *tb[__INFO_MAX];
    struct blob_attr *memory[__MEMORY_MAX];

    if (blobmsg_parse(info_policy, __INFO_MAX, tb, blob_data(msg), blob_len(msg)) != 0) {
        syslog(LOG_ERR, "Parse failed!");
        return;
    }

    if (!tb[MEMORY_DATA]) {
        syslog(LOG_WARNING, "No data received!\n");
        return;
    }

    blobmsg_parse(
        memory_policy,
        __MEMORY_MAX,
        memory,
        blobmsg_data(tb[MEMORY_DATA]),
        blobmsg_len(tb[MEMORY_DATA])
    );

    static struct blob_buf bb;
    blobmsg_buf_init(&bb);

    blobmsg_add_string(
        &bb,
        "Total memory",
        get_formatted_size(blobmsg_get_u64(memory[TOTAL_MEMORY])).str
    );
    blobmsg_add_string(
        &bb,
        "Free memory",
        get_formatted_size(blobmsg_get_u64(memory[FREE_MEMORY])).str
    );
    blobmsg_add_string(
        &bb,
        "Shared memory",
        get_formatted_size(blobmsg_get_u64(memory[SHARED_MEMORY])).str
    );
    blobmsg_add_string(
        &bb,
        "Buffered memory",
        get_formatted_size(blobmsg_get_u64(memory[BUFFERED_MEMORY])).str
    );

    char *stats = (char*) req->priv;
    char *json = blobmsg_format_json(bb.head, true);
    strcpy(stats, json);
    free(json);
}

int init_stats(struct ubus_context **ctx)
{
    *ctx = ubus_connect(NULL);
    if (!*ctx) {
        syslog(LOG_ERR, "Failed connection to ubus!\n");
        return -1;
    }
    
    return UBUS_STATUS_OK;
}

int fetch_stats(struct ubus_context *ctx, char *stats)
{
    if (!ctx) {
        syslog(LOG_ERR, "UBUS context is null!");
        return -1;
    }

    int rc = UBUS_STATUS_OK;
    uint32_t id;

    rc = ubus_lookup_id(ctx, "system", &id);
    rc = ubus_invoke(ctx, id, "info", NULL, board_cb, stats, 3000);
    
    if (rc != UBUS_STATUS_OK) {
        syslog(LOG_ERR, "Cannot request memory info from procd: %d", rc);
    }

    return rc;
}

void free_stats(struct ubus_context *ctx)
{
    if (ctx) ubus_free(ctx);
}
