#ifndef COOGLEIOT_CONFIG_H
#define COOGLEIOT_CONFIG_H

//#define COOGLEIOT_DEBUG

#define COOGLEIOT_VERSION "1.3.0"

#ifndef COOGLEIOT_SPIFFS_LOGFILE
#define COOGLEIOT_SPIFFS_LOGFILE "/coogleiot-log.txt"
#endif

#ifndef COOGLEIOT_LOGFILE_MAXSIZE
#define COOGLEIOT_LOGFILE_MAXSIZE 32768 // 32k
#endif

#ifndef COOGLEIOT_STATUS_INIT
#define COOGLEIOT_STATUS_INIT 500
#endif

#ifndef COOGLEIOT_STATUS_WIFI_INIT
#define COOGLEIOT_STATUS_WIFI_INIT 250
#endif

#ifndef COOGLEIOT_STATUS_MQTT_INIT
#define COOGLEIOT_STATUS_MQTT_INIT 100
#endif

#define COOGLEIOT_MAGIC_BYTES "ciot"

#ifndef COOGLEIOT_AP
#define COOGLEIOT_AP "COOGLEIOT_"
#endif

#ifndef COOGLEIOT_AP_DEFAULT_PASSWORD
#define COOGLEIOT_AP_DEFAULT_PASSWORD "coogleiot"
#endif

#ifndef COOGLEIOT_DEFAULT_MQTT_CLIENT_ID
#define COOGLEIOT_DEFAULT_MQTT_CLIENT_ID "coogleIoT"
#endif

#ifndef COOGLEIOT_DEFAULT_MQTT_PORT
#define COOGLEIOT_DEFAULT_MQTT_PORT 1883
#endif

#ifndef COOGLEIOT_TIMEZONE_OFFSET
#define COOGLEIOT_TIMEZONE_OFFSET ((3600 * 5) * -1) // Default Timezone is -5 UTC (America/New York)
#endif

#ifndef COOGLEIOT_DAYLIGHT_OFFSET
#define COOGLEIOT_DAYLIGHT_OFFSET 0 // seconds
#endif

#ifndef COOGLEIOT_NTP_SERVER_1
#define COOGLEIOT_NTP_SERVER_1 "pool.ntp.org"
#endif

#ifndef COOGLEIOT_NTP_SERVER_2
#define COOGLEIOT_NTP_SERVER_2 "time.nist.gov"
#endif

#ifndef COOGLEIOT_NTP_SERVER_3
#define COOGLEIOT_NTP_SERVER_3 "time.google.com"
#endif

#ifndef COOGLEIOT_HEARTBEAT_MS
#define COOGLEIOT_HEARTBEAT_MS 30000
#endif

#ifndef COOGLEIOT_FIRMWARE_UPDATE_CHECK_MS
#define COOGLEIOT_FIRMWARE_UPDATE_CHECK_MS 54000000  // 15 Minutes in Milliseconds
#endif

#ifndef COOGLEIOT_DNS_PORT
#define COOGLEIOT_DNS_PORT 53
#endif

#ifndef COOGLEIOT_EEPROM_EEPROM_SIZE
#define COOGLE_EEPROM_EEPROM_SIZE 1024
#endif

#ifndef COOGLEIOT_WEBSERVER_PORT
#define COOGLEIOT_WEBSERVER_PORT 80
#endif

#ifndef COOGLEIOT_MAX_WIFI_ATTEMPTS
#define COOGLEIOT_MAX_WIFI_ATTEMPTS 10
#endif

#ifndef COOGLEIOT_MAX_MQTT_ATTEMPTS
#define COOGLEIOT_MAX_MQTT_ATTEMPTS 10
#endif

#ifndef COOGLEIOT_DEVICE_TOPIC
#define COOGLEIOT_DEVICE_TOPIC "/coogleiot/devices"
#endif

#ifdef COOGLEIOT_DEBUG
#define COOGLEEEPROM_DEBUG
#endif

#endif
