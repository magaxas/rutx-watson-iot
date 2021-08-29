#pragma once

#define MAX_FORMAT_SIZE 32
#define MAX_JSON_SIZE 1024

int init_stats(struct ubus_context **ctx);
int fetch_stats(struct ubus_context *ctx, char *stats);

void free_stats(struct ubus_context *ctx);
