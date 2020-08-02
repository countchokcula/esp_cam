#include "socket.h"
descriptor_t init_socket(){
    int sock;                         /* Socket */
    struct sockaddr_in broadcastAddr; /* Broadcast address */
    const char* broadcastIP = IP;                /* IP broadcast address */
    unsigned short broadcastPort = 23;     /* Server port */
    int broadcastPermission = 1;          /* Socket opt to set permission to broadcast */
    socklen_t broad_len = sizeof(broadcastAddr);

    /* Create socket for sending/receiving datagrams */
    if ((sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
        ESP_LOGE(ERROR_TAG, "socket() failed");

    /* Set socket to allow broadcast */
    if (setsockopt(sock, SOL_SOCKET, SO_BROADCAST, (void *) &broadcastPermission, 
          sizeof(broadcastPermission)) < 0)
        ESP_LOGE(ERROR_TAG, "setsockopt() failed");

    /* Construct local address structure */
    memset(&broadcastAddr, 0, broad_len);   /* Zero out structure */
    broadcastAddr.sin_family = AF_INET;                 /* Internet address family */
    broadcastAddr.sin_addr.s_addr = inet_addr(broadcastIP);/* Broadcast IP address */
    broadcastAddr.sin_port = htons(broadcastPort);         /* Broadcast port */
    return (descriptor_t){
        sock,
        broadcastAddr,
        broadcastIP,
        broadcastPort,
        broadcastPermission,
        broad_len
    };
}