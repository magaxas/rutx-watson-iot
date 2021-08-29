#pragma once

#define PROPERTIES_COUNT 4

int init_device(IoTPDevice **device, IoTPConfig **config, char **conf);
int send_data(IoTPDevice *device, char *stats);

void free_device(IoTPDevice *device, IoTPConfig *config);
