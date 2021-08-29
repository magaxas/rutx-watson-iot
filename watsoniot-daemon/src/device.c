#include "main.h"

int init_device(IoTPDevice **device, IoTPConfig **config, char **conf)
{
    int rc = IOTPRC_SUCCESS;

    //Init config
    rc = IoTPConfig_create(config, NULL);
    if (rc != IOTPRC_SUCCESS) {
        syslog(LOG_ERR, "Failed to init config: %s", IOTPRC_toString(rc));
    }

    const char *cnf[PROPERTIES_COUNT][2] = {
        {IoTPConfig_identity_typeId, conf[1]},
        {IoTPConfig_identity_deviceId, conf[2]},
        {IoTPConfig_identity_orgId, conf[3]},
        {IoTPConfig_auth_token, conf[4]}
    };

    //Initializing settings
    for (int i = 0; i < PROPERTIES_COUNT; i++) {
        rc = IoTPConfig_setProperty(*config, cnf[i][0], cnf[i][1]);
        if (rc != IOTPRC_SUCCESS) {
            syslog(LOG_ERR, "Error while setting: %s = %s", cnf[i][0], cnf[i][1]);
            syslog(LOG_ERR, "Reason: %s", IOTPRC_toString(rc));
        }
    }

    //Creating device
    rc = IoTPDevice_create(device, *config);
    if (rc != IOTPRC_SUCCESS) {
        syslog(LOG_ERR, "Error while creating device: %s", IOTPRC_toString(rc));
    }

    //Connecting to the platform
    rc = IoTPDevice_connect(*device);
    if (rc != IOTPRC_SUCCESS) {
        syslog(
            LOG_ERR,
            "Error occured while connecting to IoT Platform: %s",
            IOTPRC_toString(rc)
        );
    }

    syslog(LOG_INFO, "Device was initialized.");
    return rc;
}

int send_data(IoTPDevice *device, char *stats)
{
    if (!device) {
        syslog(LOG_INFO, "Device is null!");
        return IOTPRC_INVALID_HANDLE;
    }

    int rc = IoTPDevice_sendEvent(
        device, "status", stats, "json", QoS0, NULL
    );

    syslog(
        LOG_INFO,
        "Attempted to send data to IoT platform. Status: %s",
        IOTPRC_toString(rc)
    );
    syslog(LOG_DEBUG, "Sent data: %s", stats);

    return rc;
}

void free_device(IoTPDevice *device, IoTPConfig *config)
{
    int rc = IOTPRC_SUCCESS;

    if (device) {
        rc = IoTPDevice_disconnect(device);
        rc = IoTPDevice_destroy(device);
    }
    if (config) rc = IoTPConfig_clear(config);

    if (rc != IOTPRC_SUCCESS) {
        syslog(
            LOG_ERR,
            "Error from free_device: %s",
            IOTPRC_toString(rc)
        );
    }
}
