#pragma once

#include <string>
#include "mdns.h"
#include "vector"

namespace twogreencows_core
{
    class  NetworkBonjourBroadcaster
    {
        private:
            static NetworkBonjourBroadcaster *SharedBroadcaster;
            NetworkBonjourBroadcaster();
            std::string ServiceName;
            int ServicePort;
            std::string Hostname;

            std::vector<struct sockaddr_in> *service_address_ipv4s;
            std::vector<struct sockaddr_in6> *service_address_ipv6s;

            struct sockaddr_in service_address_ipv4;
            struct sockaddr_in6 service_address_ipv6;

            int has_ipv4;
            int has_ipv6;
            int OpenClientSockets(int* sockets, int max_sockets, int port); 
            int OpenServiceSockets(int* sockets, int max_sockets);
        public:
            static NetworkBonjourBroadcaster *GetSharedBroadcaster();

            char sendbuffer[1024];
            char addrbuffer[1024];
            char namebuffer[1024];

            int ServiceCallback(int sock, const struct sockaddr* from, size_t addrlen, mdns_entry_type_t entry,
                 uint16_t query_id, uint16_t rtype, uint16_t rclass, uint32_t ttl, const void* data,
                 size_t size, size_t name_offset, size_t name_length, size_t record_offset,
                 size_t record_length, void* user_data);

            void StartBonjourService();
            void StopBonjourService();
                        
    };
}
