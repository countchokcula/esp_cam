#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

//CPP extensions
#include <iostream>
#include <sstream>
#include <esp_camera.h>
#include <esp_wifi.h>
#include "camera_pins.h"
//#include "cam_fb_t.pb.h"

using namespace std;

//Vars
static camera_config_t config;
camera_fb_t *fb = nullptr;
stringstream ss;



extern "C" {
    void camera_config(void);
    int start_wifi();
    void get_client(wifi_sta_list_t *sta);
    void app_main(void);
}
void app_main(void)
{
    wifi_sta_list_t sta;

    camera_config();    
    if(start_wifi() < 0){
        return; // an error has occured
    }

    int *active_clients;
    active_clients = &sta.num;
    while (1) {
        
        while(*active_clients > 0){
            get_client(&sta); // gets number of active_clients
            fb = esp_camera_fb_get();
            if(!fb){
                //ESP_LOGE(TAG, "Camera Capture Failed");
                cout << "Camera Capture failed " << endl;
            } else{
                cout << "len: " << fb->len << endl;
            }

            esp_camera_fb_return(fb);
            vTaskDelay(1000 / portTICK_PERIOD_MS);
        }
        

        vTaskDelay(2000 / portTICK_PERIOD_MS);
    }
}

void get_client(wifi_sta_list_t *sta){
    esp_err_t err;
    if((err = esp_wifi_ap_get_sta_list(sta)) != ESP_OK){
         cout << "Error getting station lists: " << "" << endl;
    }
}
int start_wifi(){
    
    esp_err_t err;
    wifi_init_config_t wifi_config = WIFI_INIT_CONFIG_DEFAULT();
    if((err = esp_wifi_init(&wifi_config)) != ESP_OK){
        cout << "Wifi initializaton error: " << "" << endl;
        return -1;
    }
    if((err = esp_wifi_set_mode(WIFI_MODE_AP)) != ESP_OK){
        cout << "Wifi set mode error: " << "code" << endl;
        return -1;
    }
    if((err = esp_wifi_start()) != ESP_OK){
        cout << "Wifi start error(Different from intialization error): " << "code" << endl;
        return -1;
    }
    
    cout << "Wifi has started at address: " << "addr" << endl;
    return 0;
}
void camera_config(){
    config.ledc_channel =  LEDC_CHANNEL_0;
    config.ledc_timer   =  LEDC_TIMER_0;
    config.pin_d0       =  Y2_GPIO_NUM;
    config.pin_d1       =  Y3_GPIO_NUM;
    config.pin_d2       =  Y4_GPIO_NUM;
    config.pin_d3       =  Y5_GPIO_NUM;
    config.pin_d4       =  Y6_GPIO_NUM;
    config.pin_d5       =  Y7_GPIO_NUM;
    config.pin_d6       =  Y8_GPIO_NUM;
    config.pin_d7       =  Y9_GPIO_NUM;
    config.pin_xclk     =  XCLK_GPIO_NUM;
    config.pin_pclk     =  PCLK_GPIO_NUM;
    config.pin_vsync    =  VSYNC_GPIO_NUM;
    config.pin_href     =  HREF_GPIO_NUM;
    config.pin_sscb_sda =  SIOD_GPIO_NUM;
    config.pin_sscb_scl =  SIOC_GPIO_NUM;
    config.pin_pwdn     =  PWDN_GPIO_NUM;
    config.pin_reset    =  RESET_GPIO_NUM;
    config.xclk_freq_hz =  20000000;
    config.pixel_format =  PIXFORMAT_JPEG;
    /*if(psramFound()){
        config.frame_size = FRAMESIZE_UXGA;
        config.jpeg_quality = 10;
        config.fb_count = 2;
    } else {
        config.frame_size = FRAMESIZE_SVGA;
        config.jpeg_quality = 12;
        config.fb_count = 1;
    }*/
    config.frame_size = FRAMESIZE_SVGA;
        config.jpeg_quality = 12;
        config.fb_count = 1;
    // camera init
    esp_err_t err = esp_camera_init(&config);
    if (err != ESP_OK) {
       cout << "Camera init failed with error " << err << endl;
        return;
    } else{
      cout << "Camera Success!" << endl;
    }
    sensor_t * s = esp_camera_sensor_get();
    // drop down frame size for higher initial frame rate
     s->set_framesize(s, FRAMESIZE_QVGA);
    return;
}