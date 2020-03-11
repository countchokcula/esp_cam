//C extensions
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <cstring>

//CPP extensions
#include <iostream>
#include <sstream>

//esp
#include <esp_camera.h>
#include <esp_wifi.h>
#include <esp_log.h>
#include <esp_netif.h>

//Components
#include <libtelnet.h>

//Custom
#include "camera_pins.h"
//#include "cam_fb_t.pb.h"

#define SSID "ESP"
#define PASS "p"

using namespace std;

//debugging
static const char* ERR_TAG = "Error: ";
static const char* INFO_TAG = "INFO: ";
//Vars
static camera_config_t config;
camera_fb_t *fb = nullptr;
stringstream ss;



extern "C" { // functions that "C" can see

    void nvs_flash_init(void);
    void camera_config(void);
    esp_err_t esp_netif_init(void);
    void start_softap_wifi(void);
    void get_client(wifi_sta_list_t *sta);
    void app_main(void);
}
void app_main(void)
{


    //local vars
    wifi_sta_list_t *sta_list = new wifi_sta_list_t;



     // initializations
    nvs_flash_init(); // ??
    camera_config(); // starts camera
    
    start_softap_wifi(); // starts wifi connection


    
    while (1) {
        get_client(sta_list); // get active clients
        while(sta_list->num > 0){ // once clients disconnect, shut off camera
            //ESP_LOGI(TAG, "clients: %d", sta_list->num);
            fb = esp_camera_fb_get();
            if(!fb){
                ESP_LOGE(ERR_TAG, "Camera Capture Failed");
                return;
            } else{
                /* typedef struct {
                    uint8_t * buf;              !< Pointer to the pixel data
                    size_t len;                 !< Length of the buffer in bytes 
                    size_t width;               !< Width of the buffer in pixels 
                    size_t height;              !< Height of the buffer in pixels
                    pixformat_t format;         !< Format of the pixel data 
                } camera_fb_t;
                */
                ESP_LOGI(INFO_TAG, "Camera len: %d", fb->len);
                vTaskDelay(10 / portTICK_PERIOD_MS); // 10MS DELAY
            }

            esp_camera_fb_return(fb);
            fb = nullptr;
            get_client(sta_list); // get active clients
        }
        vTaskDelay(2000 / portTICK_PERIOD_MS);
    }
    delete(sta_list);
}

void get_client(wifi_sta_list_t *sta){ //NOTE: Purpose of this function is to use the sta->num to get active clients and put it in sta
    ESP_ERROR_CHECK( esp_wifi_ap_get_sta_list(sta) );
}
void start_softap_wifi(){
    ESP_ERROR_CHECK( esp_netif_init() ); // sets up tcp/ip
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    esp_netif_create_default_wifi_ap();

    wifi_init_config_t wifi_config = WIFI_INIT_CONFIG_DEFAULT();
    wifi_config_t ap_config = {
        .ap = {
            {.ssid = SSID},
            {.password = PASS},
            .ssid_len = strlen(SSID),
            .channel = 1,
            .authmode = WIFI_AUTH_OPEN,
            .ssid_hidden = 0,
            .max_connection = 1,
            .beacon_interval = 100 
        }
    };
    ESP_ERROR_CHECK( esp_wifi_init(&wifi_config) );
    ESP_ERROR_CHECK( esp_wifi_set_mode(WIFI_MODE_AP) );
    ESP_ERROR_CHECK( esp_wifi_set_config(WIFI_IF_AP, &ap_config) );
    ESP_ERROR_CHECK( esp_wifi_start() );

    ESP_LOGI(INFO_TAG, "WIFI success, SSID: %s pass %s", SSID, PASS);
    return;
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
