#pragma once

#define UPDATE_INTERVAL 20 

//System includes
#include <stdio.h>
#include <signal.h>
#include <syslog.h>

//Libraries
#include <iotp_device.h>
#include <libubox/blobmsg_json.h>
#include <libubus.h>

//Source
#include "stats.h"
#include "device.h"
