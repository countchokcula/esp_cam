#include <stdlib.h> // Required for libtelnet.h
#include <esp_log.h>
#include "libtelnet.h"
#include <lwip/def.h>
#include <lwip/sockets.h>
#include <errno.h>
#include <string.h>
#include "sdkconfig.h"

#if defined(__cplusplus)
extern "C" {
#endif

void telnet_esp32_listenForClients(void (*callbackParam)(uint8_t *buffer, size_t size));
void telnet_esp32_sendData(uint8_t *buffer, size_t size);
int telnet_esp32_vprintf(const char *fmt, va_list va);

#if defined(__cplusplus)
} /* extern "C" */
#endif