#include <esp_log.h>
#include <esp_wifi.h>
#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <esp_wifi.h>
#include <esp_log.h>
#include "_debug.h"

#define SSID "ESP"
#define PASS "p"

#if defined(__cplusplus)
extern "C" {
#endif
extern void get_client(wifi_sta_list_t *);
extern void start_softap_wifi();
#if  defined(__cplusplus)
}
#endif