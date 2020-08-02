#include "wifi.h"


void get_client(wifi_sta_list_t *sta){ //NOTE: Purpose of this function is to use the sta_list.num to get active clients and put it in sta
    ESP_ERROR_CHECK( esp_wifi_ap_get_sta_list(sta) );
}

void start_softap_wifi(){
    ESP_ERROR_CHECK( esp_netif_init() ); // sets up tcp/ip
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    esp_netif_create_default_wifi_ap();

    wifi_init_config_t wifi_config = WIFI_INIT_CONFIG_DEFAULT();
    wifi_config_t ap_config = {
        .ap = {
            .ssid = SSID,
            .password = PASS,
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
