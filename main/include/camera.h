#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "camera_pins.h"
#include <esp_camera.h>
#include <esp_log.h>
#include <stdio.h>
#include "_debug.h"

#if defined(__cplusplus)
extern "C"{
#endif
extern void camera_config(void);

#if defined(__cplusplus)
}
#endif