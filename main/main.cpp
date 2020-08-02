//C extensions
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <cstring>

//CPP extensions
#include <iostream>
#include <sstream>

//esp
#include <esp_log.h>
#include <esp_netif.h>

//src components
#include "camera.h"
#include "wifi.h"
#include "_debug.h"
#include "socket.h"
#include <msgpack.hpp>
#include <string>

using namespace std;

#define PB_BUF_LEN 128
extern "C" { // functions that "C" can see
    //telnet

    //normal
    void nvs_flash_init(void);
    esp_err_t esp_netif_init(void);
    void app_main(void);
}
void app_main(void)
{
    wifi_sta_list_t sta_list;
     // initializations
    nvs_flash_init(); // ??
    camera_config(); // starts camera
    start_softap_wifi(); // starts wifi connection
    
    //pointer to camera frame buffer
    camera_fb_t *fb = nullptr;
   
    //sockets
    descriptor_t descriptor = init_socket();
    stringstream ss;
    
    while (1) {
        get_client(&sta_list); // get active clients
        while(sta_list.num > 0){ // once clients disconnect, shut off camera
            fb = esp_camera_fb_get();
            if(!fb){
                ESP_LOGE(ERROR_TAG, "Camera Capture Failed");
                return;
            } else{
                    
                ESP_LOGI(INFO_TAG, "Address %s ", descriptor.ip);
                //sendto(descriptor.sock, &pb_buf, pb_stream.bytes_written,
                ///     0, (struct sockaddr *) &descriptor.addr, descriptor.ip_len); //send as buffer
                ESP_LOGI(INFO_TAG, "Sent message!");
                vTaskDelay(1000 / portTICK_PERIOD_MS); // 1S DELAY
            }
             
            esp_camera_fb_return(fb);
            fb = nullptr;
            
            get_client(&sta_list); // get active clients
        }
        vTaskDelay(2000 / portTICK_PERIOD_MS);
    }
}
