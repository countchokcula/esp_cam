#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 
#include <esp_log.h>
#include <_debug.h>

#if defined(__cplusplus)
extern "C"{
#endif

#define PORT 23
#define IP "255.255.255.255"

typedef struct descriptor_t {
    int sock;
    struct sockaddr_in addr;
    const char* ip;
    unsigned short port;
    int permission;
    socklen_t ip_len;
} descriptor_t;

descriptor_t init_socket();

#if defined(__cplusplus)
}
#endif