
#include <iostream>
#include <fstream>
#include <string>
#include <sys/types.h>
#include <sys/utsname.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <ifaddrs.h>
#include "mdns.h"
#include <vector>

typedef struct {
	mdns_string_t service;
	mdns_string_t hostname;
	mdns_string_t service_instance;
	mdns_string_t hostname_qualified;
	struct sockaddr_in address_ipv4;
	struct sockaddr_in6 address_ipv6;
	int port;
	mdns_record_t record_ptr;
	mdns_record_t record_srv;
	mdns_record_t record_a;
	mdns_record_t record_aaaa;
	mdns_record_t txt_record[2];
} service_t;

#include "NetworkBonjourBroadcaster.hpp"


extern "C"
{
int GlobalServiceCallback(int sock, const struct sockaddr* from, size_t addrlen, mdns_entry_type_t entry,
                 uint16_t query_id, uint16_t rtype, uint16_t rclass, uint32_t ttl, const void* data,
                 size_t size, size_t name_offset, size_t name_length, size_t record_offset,
                 size_t record_length, void* user_data);

int GlobalServiceCallback(int sock, const struct sockaddr* from, size_t addrlen, mdns_entry_type_t entry,
                 uint16_t query_id, uint16_t rtype, uint16_t rclass, uint32_t ttl, const void* data,
                 size_t size, size_t name_offset, size_t name_length, size_t record_offset,
                 size_t record_length, void* user_data)

{
    twogreencows_core::NetworkBonjourBroadcaster::GetSharedBroadcaster()->ServiceCallback(sock, from, addrlen, entry, query_id, rtype, 
            rclass, ttl, data,size,name_offset, name_length, record_offset, record_length, user_data);
}

}

namespace twogreencows_core
{
    NetworkBonjourBroadcaster* NetworkBonjourBroadcaster::SharedBroadcaster = nullptr;
    
    NetworkBonjourBroadcaster::NetworkBonjourBroadcaster()
    {
        this->ServiceName = "_twogreencowsagent._tcp.local.";
        this->ServicePort = 27512;

        //We get the name without local
        struct utsname infoName;
        uname(&infoName);
        this->Hostname = infoName.nodename;
        std::string localString(".local");
        if (this->Hostname.length() > localString.length()) {
            if (0 == this->Hostname.compare(this->Hostname.length() - localString.length(), localString.length(), localString)) {
                this->Hostname = this->Hostname.substr(0, this->Hostname.size()-localString.size());
            }
        }  

        this->service_address_ipv4s = new std::vector<struct sockaddr_in>();
        this->service_address_ipv6s = new std::vector<struct sockaddr_in6>();
    }

    NetworkBonjourBroadcaster* NetworkBonjourBroadcaster::GetSharedBroadcaster()
    {
        if(nullptr == SharedBroadcaster) {
            SharedBroadcaster = new NetworkBonjourBroadcaster();
        }
        return SharedBroadcaster;
    }

    void NetworkBonjourBroadcaster::StartBonjourService()
    {
        int sockets[32];
        int num_sockets = this->OpenServiceSockets(sockets, sizeof(sockets) / sizeof(sockets[0]));

        if (num_sockets <= 0) {
		printf("Failed to open any client sockets\n");
		return ;
	}
        std::cerr << "Opened " << num_sockets << " sockets for mDNS service"<< std::endl;
        std::cerr << "Service mDNS: " << this->ServiceName << ":" <<  this->ServicePort << std::endl;
	std::cerr << "Hostname: " << this->Hostname << std::endl;

	size_t capacity = 2048;
	void* buffer = malloc(capacity);

	mdns_string_t service_string = (mdns_string_t){this->ServiceName.c_str(), strlen(this->ServiceName.c_str())};
	mdns_string_t hostname_string = (mdns_string_t){this->Hostname.c_str(), strlen(this->Hostname.c_str())};

	// Build the service instance "<hostname>.<_service-name>._tcp.local." string
	char service_instance_buffer[256] = {0};
	snprintf(service_instance_buffer, sizeof(service_instance_buffer) - 1, "%.*s.%.*s",
	         MDNS_STRING_FORMAT(hostname_string), MDNS_STRING_FORMAT(service_string));
	mdns_string_t service_instance_string =
	    (mdns_string_t){service_instance_buffer, strlen(service_instance_buffer)};
        std::cerr << "Service instance string: " <<  service_instance_buffer << std::endl;

	// Build the "<hostname>.local." string
	char qualified_hostname_buffer[256] = {0};
	snprintf(qualified_hostname_buffer, sizeof(qualified_hostname_buffer) - 1, "%.*s.local.",
	         MDNS_STRING_FORMAT(hostname_string));
	mdns_string_t hostname_qualified_string =
	    (mdns_string_t){qualified_hostname_buffer, strlen(qualified_hostname_buffer)};

        std::cerr << "Hostname qualified local: " <<  qualified_hostname_buffer << std::endl;;
	service_t service = {0};
	service.service = service_string;
	service.hostname = hostname_string;
	service.service_instance = service_instance_string;
	service.hostname_qualified = hostname_qualified_string;
	service.address_ipv4 = service_address_ipv4;
	service.address_ipv6 = service_address_ipv6;
	service.port = this->ServicePort;

	// Setup our mDNS records
    // PTR record reverse mapping "<_service-name>._tcp.local." to
	// "<hostname>.<_service-name>._tcp.local."
	service.record_ptr = (mdns_record_t){.name = service.service,
	                                     .type = MDNS_RECORDTYPE_PTR,
	                                     .data.ptr.name = service.service_instance};

	// SRV record mapping "<hostname>.<_service-name>._tcp.local." to
	// "<hostname>.local." with port. Set weight & priority to 0.
	service.record_srv = (mdns_record_t){.name = service.service_instance,
	                                     .type = MDNS_RECORDTYPE_SRV,
	                                     .data.srv.name = service.hostname_qualified,
	                                     .data.srv.port = (uint16_t) service.port,
	                                     .data.srv.priority = 0,
	                                     .data.srv.weight = 0};

	// A/AAAA records mapping "<hostname>.local." to IPv4/IPv6 addresses
	service.record_a = (mdns_record_t){.name = service.hostname_qualified,
	                                   .type = MDNS_RECORDTYPE_A,
	                                   .data.a.addr = service.address_ipv4};

	service.record_aaaa = (mdns_record_t){.name = service.hostname_qualified,
	                                      .type = MDNS_RECORDTYPE_AAAA,
	                                      .data.aaaa.addr = service.address_ipv6};

	// Add two test TXT records for our service instance name, will be coalesced into
	// one record with both key-value pair strings by the library
	service.txt_record[0] = (mdns_record_t){.name = service.service_instance,
	                                        .type = MDNS_RECORDTYPE_TXT,
	                                        .data.txt.key = {MDNS_STRING_CONST("test")},
	                                        .data.txt.value = {MDNS_STRING_CONST("1")}};
	service.txt_record[1] = (mdns_record_t){.name = service.service_instance,
	                                        .type = MDNS_RECORDTYPE_TXT,
	                                        .data.txt.key = {MDNS_STRING_CONST("other")},
	                                        .data.txt.value = {MDNS_STRING_CONST("value")}};

	// Send an announcement on startup of service
	{
		mdns_record_t additional[5] = {0};
		size_t additional_count = 0;
		additional[additional_count++] = service.record_srv;
		if (service.address_ipv4.sin_family == AF_INET)
			additional[additional_count++] = service.record_a;
		if (service.address_ipv6.sin6_family == AF_INET6)
			additional[additional_count++] = service.record_aaaa;
		additional[additional_count++] = service.txt_record[0];
		additional[additional_count++] = service.txt_record[1];

		for (int isock = 0; isock < num_sockets; ++isock)
			mdns_announce_multicast(sockets[isock], buffer, capacity, service.record_ptr, 0, 0,
			                        additional, additional_count);
	}

	// This is a crude implementation that checks for incoming queries
	while (1) {
		int nfds = 0;
		fd_set readfs;
		FD_ZERO(&readfs);
		for (int isock = 0; isock < num_sockets; ++isock) {
			if (sockets[isock] >= nfds)
				nfds = sockets[isock] + 1;
			FD_SET(sockets[isock], &readfs);
		}

		if (select(nfds, &readfs, 0, 0, 0) >= 0) {
			for (int isock = 0; isock < num_sockets; ++isock) {
				if (FD_ISSET(sockets[isock], &readfs)) {
					mdns_socket_listen(sockets[isock], buffer, capacity, GlobalServiceCallback,
					                   &service);
				}
				FD_SET(sockets[isock], &readfs);
			}
		} else {
			break;
		}
	}

	// Send a goodbye on end of service
	{
		mdns_record_t additional[5] = {{{0}}};
		size_t additional_count = 0;
		additional[additional_count++] = service.record_srv;
		if (service.address_ipv4.sin_family == AF_INET)
			additional[additional_count++] = service.record_a;
		if (service.address_ipv6.sin6_family == AF_INET6)
			additional[additional_count++] = service.record_aaaa;
		additional[additional_count++] = service.txt_record[0];
		additional[additional_count++] = service.txt_record[1];

		for (int isock = 0; isock < num_sockets; ++isock)
			mdns_goodbye_multicast(sockets[isock], buffer, capacity, service.record_ptr, 0, 0,
			                        additional, additional_count);
	}

	free(buffer);
	//free(service_name_buffer);

	for (int isock = 0; isock < num_sockets; ++isock)
		mdns_socket_close(sockets[isock]);
	printf("Closed socket%s\n", num_sockets ? "s" : "");        
    }

    void NetworkBonjourBroadcaster:: StopBonjourService()
    {
    }
                        
    int NetworkBonjourBroadcaster::OpenClientSockets(int* sockets, int max_sockets, int port) 
    {
	// When sending, each socket can only send to one network interface
	// Thus we need to open one socket for each interface and address family
	int num_sockets = 0;

	struct ifaddrs* ifaddr = 0;
	struct ifaddrs* ifa = 0;

            //socket
	if (getifaddrs(&ifaddr) < 0)
		printf("Unable to get interface addresses\n");

        std::cerr << "Enumerating Interfaces for port:" << port << std::endl;
	int first_ipv4 = 1;
	int first_ipv6 = 1;
	for (ifa = ifaddr; ifa; ifa = ifa->ifa_next) {
		if (!ifa->ifa_addr)
			continue;

                if (0 != ::strncmp(ifa->ifa_name, "en", 2)) 
                        continue;
                std::cerr << " + Interface: " << ifa->ifa_name << ", family :  " << int{ifa->ifa_addr->sa_family } << std::endl;
		if (ifa->ifa_addr->sa_family == AF_INET) {
			struct sockaddr_in* saddr = (struct sockaddr_in*)ifa->ifa_addr;
			if (saddr->sin_addr.s_addr != htonl(INADDR_LOOPBACK)) {
                         
                                this->service_address_ipv4s->push_back(*saddr);
                                std::cerr << ifa->ifa_name << "  --> IPV4" <<std::endl;
				int log_addr = 0;
				if (first_ipv4) {
					service_address_ipv4 = *saddr;
					first_ipv4 = 0;
					log_addr = 1;
				}
				has_ipv4 = 1;
				if (num_sockets < max_sockets) {
					saddr->sin_port = htons(port);
                                        std::cerr << "OPEN IPV4" << port << std::endl;
					int sock = mdns_socket_open_ipv4(saddr);
					if (sock >= 0) {
						sockets[num_sockets++] = sock;
						log_addr = 1;
					} else {
						log_addr = 0;
					}
				}
				//if (log_addr) {
					char buffer[128];
					mdns_string_t addr = ipv4_address_to_string(buffer, sizeof(buffer), saddr,
					                                            sizeof(struct sockaddr_in));
                                        std::cerr << "== INFO  IPv4 address: " <<  addr.str << std::endl;
				//}
			}
		} else if (ifa->ifa_addr->sa_family == AF_INET6) {
                    
                        std::cerr << ifa->ifa_name << " --IPV6" <<std::endl;
			struct sockaddr_in6* saddr = (struct sockaddr_in6*)ifa->ifa_addr;
			static const unsigned char localhost[] = {0, 0, 0, 0, 0, 0, 0, 0,
			                                          0, 0, 0, 0, 0, 0, 0, 1};
			static const unsigned char localhost_mapped[] = {0, 0, 0,    0,    0,    0, 0, 0,
			                                                 0, 0, 0xff, 0xff, 0x7f, 0, 0, 1};
			if (memcmp(saddr->sin6_addr.s6_addr, localhost, 16) &&
			    memcmp(saddr->sin6_addr.s6_addr, localhost_mapped, 16)) {

                                
                                this->service_address_ipv6s->push_back(*saddr);
				int log_addr = 0;
				if (first_ipv6) {
					service_address_ipv6 = *saddr;
					first_ipv6 = 0;
					log_addr = 1;
				}
				has_ipv6 = 1;
				if (num_sockets < max_sockets) {
					saddr->sin6_port = htons(port);
					int sock = mdns_socket_open_ipv6(saddr);
					if (sock >= 0) {
						sockets[num_sockets++] = sock;
						log_addr = 1;
					} else {
						log_addr = 0;
					}
				}
				//if (log_addr) {
					char buffer[128];
					mdns_string_t addr = ipv6_address_to_string(buffer, sizeof(buffer), saddr,
					                                            sizeof(struct sockaddr_in6));
                                        std::cerr << " == INFO Local IPv6 address: " <<  addr.str << std::endl ;
				//}
			}
		}  
                

	}

	freeifaddrs(ifaddr);


	return num_sockets;
    }

    int NetworkBonjourBroadcaster::OpenServiceSockets(int* sockets, int max_sockets)
    {
        int num_sockets = 0;

	// Call the client socket function to enumerate and get local addresses,
	// but not open the actual sockets
	this->OpenClientSockets(NULL, 0, 0);

	if (num_sockets < max_sockets) {
		struct sockaddr_in sock_addr;
		memset(&sock_addr, 0, sizeof(struct sockaddr_in));
		sock_addr.sin_family = AF_INET;
		sock_addr.sin_addr.s_addr = INADDR_ANY;
		sock_addr.sin_port = htons(MDNS_PORT);
#ifdef __APPLE__
		sock_addr.sin_len = sizeof(struct sockaddr_in);
#endif
		int sock = mdns_socket_open_ipv4(&sock_addr);
		if (sock >= 0)
			sockets[num_sockets++] = sock;
	}

	if (num_sockets < max_sockets) {
		struct sockaddr_in6 sock_addr;
		memset(&sock_addr, 0, sizeof(struct sockaddr_in6));
		sock_addr.sin6_family = AF_INET6;
		sock_addr.sin6_addr = in6addr_any;
		sock_addr.sin6_port = htons(MDNS_PORT);
#ifdef __APPLE__
		sock_addr.sin6_len = sizeof(struct sockaddr_in6);
#endif
		int sock = mdns_socket_open_ipv6(&sock_addr);
		if (sock >= 0)
			sockets[num_sockets++] = sock;
	}

	return num_sockets;    
    }
    
    int NetworkBonjourBroadcaster::ServiceCallback(int sock, const struct sockaddr* from, size_t addrlen, mdns_entry_type_t entry,
                 uint16_t query_id, uint16_t rtype, uint16_t rclass, uint32_t ttl, const void* data,
                 size_t size, size_t name_offset, size_t name_length, size_t record_offset,
                 size_t record_length, void* user_data)
    {
        
        (void)sizeof(ttl);
	if (entry != MDNS_ENTRYTYPE_QUESTION)
		return 0;

	const char dns_sd[] = "_services._dns-sd._udp.local.";
	const service_t* service = (const service_t*)user_data;

	mdns_string_t fromaddrstr = ip_address_to_string(addrbuffer, sizeof(addrbuffer), from, addrlen);

	size_t offset = name_offset;
	mdns_string_t name = mdns_string_extract(data, size, &offset, namebuffer, sizeof(namebuffer));

	const char* record_name = 0;
	if (rtype == MDNS_RECORDTYPE_PTR)
		record_name = "PTR";
	else if (rtype == MDNS_RECORDTYPE_SRV)
		record_name = "SRV";
	else if (rtype == MDNS_RECORDTYPE_A)
		record_name = "A";
	else if (rtype == MDNS_RECORDTYPE_AAAA)
		record_name = "AAAA";
	else if (rtype == MDNS_RECORDTYPE_ANY)
		record_name = "ANY";
	else
		return 0;
	printf("Query %s %.*s\n", record_name, MDNS_STRING_FORMAT(name));

	if ((name.length == (sizeof(dns_sd) - 1)) &&
	    (strncmp(name.str, dns_sd, sizeof(dns_sd) - 1) == 0)) {
		if ((rtype == MDNS_RECORDTYPE_PTR) || (rtype == MDNS_RECORDTYPE_ANY)) {
			// The PTR query was for the DNS-SD domain, send answer with a PTR record for the
			// service name we advertise, typically on the "<_service-name>._tcp.local." format

			// Answer PTR record reverse mapping "<_service-name>._tcp.local." to
			// "<hostname>.<_service-name>._tcp.local."
			mdns_record_t answer = {
			    .name = name, .type = MDNS_RECORDTYPE_PTR, .data.ptr.name = service->service};

			// Send the answer, unicast or multicast depending on flag in query
			uint16_t unicast = (rclass & MDNS_UNICAST_RESPONSE);
			printf("  --> answer %.*s (%s)\n", MDNS_STRING_FORMAT(answer.data.ptr.name),
			       (unicast ? "unicast" : "multicast"));

			if (unicast) {
				mdns_query_answer_unicast(sock, from, addrlen, sendbuffer, sizeof(sendbuffer),
				                          query_id, (mdns_record_type_t) rtype, name.str, name.length, answer, 0, 0, 0,
				                          0);
			} else {
				mdns_query_answer_multicast(sock, sendbuffer, sizeof(sendbuffer), answer, 0, 0, 0,
				                            0);
			}
		}
	} else if ((name.length == service->service.length) &&
	           (strncmp(name.str, service->service.str, name.length) == 0)) {
		if ((rtype == MDNS_RECORDTYPE_PTR) || (rtype == MDNS_RECORDTYPE_ANY)) {
			// The PTR query was for our service (usually "<_service-name._tcp.local"), answer a PTR
			// record reverse mapping the queried service name to our service instance name
			// (typically on the "<hostname>.<_service-name>._tcp.local." format), and add
			// additional records containing the SRV record mapping the service instance name to our
			// qualified hostname (typically "<hostname>.local.") and port, as well as any IPv4/IPv6
			// address for the hostname as A/AAAA records, and two test TXT records

			// Answer PTR record reverse mapping "<_service-name>._tcp.local." to
			// "<hostname>.<_service-name>._tcp.local."
			mdns_record_t answer = service->record_ptr;

			mdns_record_t additional[5] = {0};
			size_t additional_count = 0;

			// SRV record mapping "<hostname>.<_service-name>._tcp.local." to
			// "<hostname>.local." with port. Set weight & priority to 0.
			additional[additional_count++] = service->record_srv;

			// A/AAAA records mapping "<hostname>.local." to IPv4/IPv6 addresses
			if (service->address_ipv4.sin_family == AF_INET)
				additional[additional_count++] = service->record_a;
			if (service->address_ipv6.sin6_family == AF_INET6)
				additional[additional_count++] = service->record_aaaa;

			// Add two test TXT records for our service instance name, will be coalesced into
			// one record with both key-value pair strings by the library
			additional[additional_count++] = service->txt_record[0];
			additional[additional_count++] = service->txt_record[1];

			// Send the answer, unicast or multicast depending on flag in query
			uint16_t unicast = (rclass & MDNS_UNICAST_RESPONSE);
			printf("  --> answer %.*s (%s)\n",
			       MDNS_STRING_FORMAT(service->record_ptr.data.ptr.name),
			       (unicast ? "unicast" : "multicast"));

			if (unicast) {
				mdns_query_answer_unicast(sock, from, addrlen, sendbuffer, sizeof(sendbuffer),
				                          query_id, (mdns_record_type_t)rtype, name.str, name.length, answer, 0, 0,
				                          additional, additional_count);
			} else {
				mdns_query_answer_multicast(sock, sendbuffer, sizeof(sendbuffer), answer, 0, 0,
				                            additional, additional_count);
			}
		}
	} else if ((name.length == service->service_instance.length) &&
	           (strncmp(name.str, service->service_instance.str, name.length) == 0)) {
		if ((rtype == MDNS_RECORDTYPE_SRV) || (rtype == MDNS_RECORDTYPE_ANY)) {
			// The SRV query was for our service instance (usually
			// "<hostname>.<_service-name._tcp.local"), answer a SRV record mapping the service
			// instance name to our qualified hostname (typically "<hostname>.local.") and port, as
			// well as any IPv4/IPv6 address for the hostname as A/AAAA records, and two test TXT
			// records

			// Answer PTR record reverse mapping "<_service-name>._tcp.local." to
			// "<hostname>.<_service-name>._tcp.local."
			mdns_record_t answer = service->record_srv;

			mdns_record_t additional[5] = {0};
			size_t additional_count = 0;

			// A/AAAA records mapping "<hostname>.local." to IPv4/IPv6 addresses
			if (service->address_ipv4.sin_family == AF_INET)
				additional[additional_count++] = service->record_a;
			if (service->address_ipv6.sin6_family == AF_INET6)
				additional[additional_count++] = service->record_aaaa;

			// Add two test TXT records for our service instance name, will be coalesced into
			// one record with both key-value pair strings by the library
			additional[additional_count++] = service->txt_record[0];
			additional[additional_count++] = service->txt_record[1];

			// Send the answer, unicast or multicast depending on flag in query
			uint16_t unicast = (rclass & MDNS_UNICAST_RESPONSE);
			printf("  --> answer %.*s port %d (%s)\n",
			       MDNS_STRING_FORMAT(service->record_srv.data.srv.name), service->port,
			       (unicast ? "unicast" : "multicast"));

			if (unicast) {
				mdns_query_answer_unicast(sock, from, addrlen, sendbuffer, sizeof(sendbuffer),
				                          query_id, (mdns_record_type_t)rtype, name.str, name.length, answer, 0, 0,
				                          additional, additional_count);
			} else {
				mdns_query_answer_multicast(sock, sendbuffer, sizeof(sendbuffer), answer, 0, 0,
				                            additional, additional_count);
			}
		}
	} else if ((name.length == service->hostname_qualified.length) &&
	           (strncmp(name.str, service->hostname_qualified.str, name.length) == 0)) {
		if (((rtype == MDNS_RECORDTYPE_A) || (rtype == MDNS_RECORDTYPE_ANY)) &&
		    (service->address_ipv4.sin_family == AF_INET)) {
			// The A query was for our qualified hostname (typically "<hostname>.local.") and we
			// have an IPv4 address, answer with an A record mappiing the hostname to an IPv4
			// address, as well as any IPv6 address for the hostname, and two test TXT records

			// Answer A records mapping "<hostname>.local." to IPv4 address
			mdns_record_t answer = service->record_a;

			mdns_record_t additional[5] = {0};
			size_t additional_count = 0;

			// AAAA record mapping "<hostname>.local." to IPv6 addresses
			if (service->address_ipv6.sin6_family == AF_INET6)
				additional[additional_count++] = service->record_aaaa;

			// Add two test TXT records for our service instance name, will be coalesced into
			// one record with both key-value pair strings by the library
			additional[additional_count++] = service->txt_record[0];
			additional[additional_count++] = service->txt_record[1];

			// Send the answer, unicast or multicast depending on flag in query
			uint16_t unicast = (rclass & MDNS_UNICAST_RESPONSE);
			mdns_string_t addrstr = ip_address_to_string(
			    addrbuffer, sizeof(addrbuffer), (struct sockaddr*)&service->record_a.data.a.addr,
			    sizeof(service->record_a.data.a.addr));
			printf("  --> answer %.*s IPv4 %.*s (%s)\n", MDNS_STRING_FORMAT(service->record_a.name),
			       MDNS_STRING_FORMAT(addrstr), (unicast ? "unicast" : "multicast"));

			if (unicast) {
				mdns_query_answer_unicast(sock, from, addrlen, sendbuffer, sizeof(sendbuffer),
				                          query_id, (mdns_record_type_t)rtype, name.str, name.length, answer, 0, 0,
				                          additional, additional_count);
			} else {
				mdns_query_answer_multicast(sock, sendbuffer, sizeof(sendbuffer), answer, 0, 0,
				                            additional, additional_count);
			}
		} else if (((rtype == MDNS_RECORDTYPE_AAAA) || (rtype == MDNS_RECORDTYPE_ANY)) &&
		           (service->address_ipv6.sin6_family == AF_INET6)) {
			// The AAAA query was for our qualified hostname (typically "<hostname>.local.") and we
			// have an IPv6 address, answer with an AAAA record mappiing the hostname to an IPv6
			// address, as well as any IPv4 address for the hostname, and two test TXT records

			// Answer AAAA records mapping "<hostname>.local." to IPv6 address
			mdns_record_t answer = service->record_aaaa;

			mdns_record_t additional[5] = {{0}};
			size_t additional_count = 0;

			// A record mapping "<hostname>.local." to IPv4 addresses
			if (service->address_ipv4.sin_family == AF_INET)
				additional[additional_count++] = service->record_a;

			// Add two test TXT records for our service instance name, will be coalesced into
			// one record with both key-value pair strings by the library
			additional[additional_count++] = service->txt_record[0];
			additional[additional_count++] = service->txt_record[1];

			// Send the answer, unicast or multicast depending on flag in query
			uint16_t unicast = (rclass & MDNS_UNICAST_RESPONSE);
			mdns_string_t addrstr =
			    ip_address_to_string(addrbuffer, sizeof(addrbuffer),
			                         (struct sockaddr*)&service->record_aaaa.data.aaaa.addr,
			                         sizeof(service->record_aaaa.data.aaaa.addr));
			printf("  --> answer %.*s IPv6 %.*s (%s)\n",
			       MDNS_STRING_FORMAT(service->record_aaaa.name), MDNS_STRING_FORMAT(addrstr),
			       (unicast ? "unicast" : "multicast"));

			if (unicast) {
				mdns_query_answer_unicast(sock, from, addrlen, sendbuffer, sizeof(sendbuffer),
				                          query_id, (mdns_record_type_t)rtype, name.str, name.length, answer, 0, 0,
				                          additional, additional_count);
			} else {
				mdns_query_answer_multicast(sock, sendbuffer, sizeof(sendbuffer), answer, 0, 0,
				                            additional, additional_count);
			}
		}
	}
    
	return 0;
    }

 
}
