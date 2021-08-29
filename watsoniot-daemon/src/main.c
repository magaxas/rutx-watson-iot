#include "main.h"

static int running = 1;

void handle_signal(int sig, siginfo_t *siginfo, void *context)
{
    syslog(LOG_INFO, "Stopping daemon...");
    running = 0;
}

void main_loop(struct ubus_context *ctx, IoTPDevice *device, char *stats) {
    while(running) {
        if(fetch_stats(ctx, stats) != UBUS_STATUS_OK) {
            syslog(LOG_ERR, "Failed to get data from UBUS!");
        }

        send_data(device, stats);
        sleep(UPDATE_INTERVAL);
    }
}

int main(int argc, char **argv)
{
    openlog(NULL, LOG_CONS | LOG_PERROR, LOG_DAEMON);
    syslog(LOG_INFO, "Started watsoniot-daemon...");

    struct ubus_context *ctx = NULL;
    IoTPConfig *config = NULL;
    IoTPDevice *device = NULL;
    struct sigaction act;
    char *stats = NULL;

    memset(&act, '\0', sizeof(act));
    act.sa_sigaction = &handle_signal;
    act.sa_flags = SA_SIGINFO;

    if (argc != PROPERTIES_COUNT + 1) {
        syslog(LOG_ERR, "Passed invalid amount of arguments: %d", argc);
        goto end;
    }
    else if (sigaction(SIGINT, &act, NULL) < 0
        || sigaction(SIGTERM, &act, NULL) < 0) {
        syslog(LOG_ERR, "Failed to register sigaction!");
        goto end;
    }
    else if (init_device(&device, &config, argv) != IOTPRC_SUCCESS) {
        syslog(LOG_ERR, "Failed to initialize device!");
        goto end;
    }
	else if (init_stats(&ctx) != 0) {
        syslog(LOG_ERR, "Failed to initialize stats!");
        goto end;
    }

    stats = (char*) malloc(sizeof(char*) * MAX_JSON_SIZE);
    main_loop(ctx, device, stats);

end:
	free_stats(ctx);
    free_device(device, config);
    if (!stats) free(stats);

    syslog(LOG_INFO, "Exiting watsoniot-daemon...");
    closelog();

	return 0;
}
