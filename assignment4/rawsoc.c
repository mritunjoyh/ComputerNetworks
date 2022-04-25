// raw_sock.c
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
//#include<netinet.h>
#include<netinet/ip.h>
#include<netinet/tcp.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netinet/if_ether.h>
int main() {
    // Structs that contain source IP addresses
    struct sockaddr_in source_socket_address, dest_socket_address;

    int ps;

    // Allocate string buffer to hold incoming packet data
    unsigned char *buffer = (unsigned char *)malloc(65536);
    // Open the raw socket
    int sock = socket (PF_INET, SOCK_RAW, IPPROTO_TCP);
    if(sock == -1)
    {
        //socket creation failed, may be because of non-root privileges
        perror("Failed to create socket");
        exit(1);
    }
    while(1)
    {
      ps = recvfrom(sock , buffer , 65536 , 0 , NULL, NULL);
      if (ps == -1) {
        printf("Failed to get packets\n");
        return 1;
      }
      memset(buffer,0,65536);
      struct iphdr *ip_pack = (struct iphdr *)buffer;
      struct ethhdr *eth_pack = (struct ethhdr *)(buffer);
      struct tcphdr *tcp_pack = (struct tcphdr *)(buffer+ ip_pack->ihl*4);
      
      printf("Incoming Packet (Ethernet): \n");
      printf("Source Address\t: %.2X-%.2X-%.2X-%.2X-%.2X-%.2X\n",eth_pack->h_source[0],eth_pack->h_source[1],eth_pack->h_source[2],eth_pack->h_source[3],eth_pack->h_source[4],eth_pack->h_source[5]);
      printf("Destination Address\t: %.2X-%.2X-%.2X-%.2X-%.2X-%.2X\n\n",eth_pack->h_source[0],eth_pack->h_dest[1],eth_pack->h_dest[2],eth_pack->h_dest[3],eth_pack->h_dest[4],eth_pack->h_dest[5]);
      //printf("Protocol            : %u \n",(unsigned short)eth_pack->h_proto);
      
      memset(&source_socket_address, 0, sizeof(source_socket_address));
      source_socket_address.sin_addr.s_addr = ip_pack->saddr;
      memset(&dest_socket_address, 0, sizeof(dest_socket_address));
      dest_socket_address.sin_addr.s_addr = ip_pack->daddr;

      printf("Incoming Packet (IP): \n");
      printf("Packet Size (bytes): %d\n",ntohs(ip_pack->tot_len));
      printf("Source Address: %s\n", (char *)inet_ntoa(source_socket_address.sin_addr));
      printf("Destination Address: %s\n", (char *)inet_ntoa(dest_socket_address.sin_addr));
      printf("Identification: %d\n\n", ntohs(ip_pack->id));

      
      printf("Incoming Packet (IPPROTO_TCP): \n");
      printf("Source Address: %x\n", tcp_pack->source);
      printf("Destination Address: %x\n\n",tcp_pack->dest);

    }
    return 0;
}
