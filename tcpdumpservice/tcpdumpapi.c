/*
   sniffer.c

   Example packet sniffer using the libpcap packet capture library available
   from http://www.tcpdump.org.
  
   ------------------------------------------

   Copyright © 2012 [Vic Hargrave - http://vichargrave.com]

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

#include <pcap.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <net/ethernet.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>
#include <netinet/ip_icmp.h>
#include <tcpdumpapi.h>

pcap_t* pd;
struct bpf_program bpf;

int linkhdrlen;

char* globalpacket;

/*
 * print data in rows of 16 bytes: offset   hex   ascii
 *
 * 00000   47 45 54 20 2f 20 48 54  54 50 2f 31 2e 31 0d 0a   GET / HTTP/1.1..
 */
void
print_hex_ascii_line(const u_char *payload, int len, int offset)
{

	int i;
	int gap;
	const u_char *ch;

	/* offset */
	// sprintf(globalpacket + strlen(globalpacket), "%05d   ", offset);
	
	/* hex */
	// ch = payload;
	// for(i = 0; i < len; i++) {
	// 	sprintf(globalpacket + strlen(globalpacket), "%02x ", *ch);
	// 	ch++;
	// 	/* print extra space after 8th byte for visual aid */
	// 	if (i == 7)
	// 		sprintf(globalpacket + strlen(globalpacket), " ");
	// }
	// /* print space to handle line less than 8 bytes */
	// if (len < 8)
	// 	sprintf(globalpacket + strlen(globalpacket), " ");
	
	// /* fill hex gap with spaces if not full line */
	// if (len < 16) {
	// 	gap = 16 - len;
	// 	for (i = 0; i < gap; i++) {
	// 		sprintf(globalpacket + strlen(globalpacket), "   ");
	// 	}
	// }
	// sprintf(globalpacket + strlen(globalpacket), "   ");
	
	/* ascii (if printable) */
	ch = payload;
	for(i = 0; i < len; i++) {
		// if (isprint(*ch))
        if ( '!' <*ch && *ch < '~')
			sprintf(globalpacket + strlen(globalpacket), "%c", *ch);
		else
			sprintf(globalpacket + strlen(globalpacket), ".");
		ch++;
	}

	sprintf(globalpacket + strlen(globalpacket), "\n");

return;
}


/*
 * print packet payload data (avoid printing binary data)
 */
void
print_payload(const u_char *payload, int len)
{

	int len_rem = len;
	int line_width = 32;			/* number of bytes per line */
	int line_len;
	int offset = 0;					/* zero-based offset counter */
	const u_char *ch = payload;

	if (len <= 0)
		return;

	/* data fits on one line */
	if (len <= line_width) {
		print_hex_ascii_line(ch, len, offset);
		return;
	}

	/* data spans multiple lines */
	for ( ;; ) {
		/* compute current line length */
		line_len = line_width % len_rem;
		/* print line */
		print_hex_ascii_line(ch, line_len, offset);
		/* compute total remaining */
		len_rem = len_rem - line_len;
		/* shift pointer to remaining bytes to print */
		ch = ch + line_len;
		/* add offset */
		offset = offset + line_width;
		/* check if we have line width chars or less */
		if (len_rem <= line_width) {
			/* print last line and get out */
			print_hex_ascii_line(ch, len_rem, offset);
			break;
		}
	}

return;
}

#define ETHER_ADDR_LEN 6

void hex2mac(const u_char *data, char mac[]){
	char HexTable[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
	int i;
	for (i = 0; i < ETHER_ADDR_LEN; i++){
		mac[3 * i] = HexTable[data[i] >> 4];
		mac[3 * i + 1] = HexTable[data[i] & 0x0F];
		mac[3 * i + 2] = ':';
	}
	mac[17] = 0;
	return mac;
}



pcap_t* open_pcap_socket(char* device, const char* bpfstr)
{
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t* pd;
    uint32_t  srcip, netmask;
    // struct bpf_program  bpf;

    // If no network interface (device) is specfied, get the first one.
    if (!*device && !(device = pcap_lookupdev(errbuf)))
    {
        printf("pcap_lookupdev(): %s\n", errbuf);
        return NULL;
    }
    
    // Open the device for live capture, as opposed to reading a packet
    // capture file.
    if ((pd = pcap_open_live(device, BUFSIZ, 1, 0, errbuf)) == NULL)
    {
        printf("pcap_open_live(): %s\n", errbuf);
        return NULL;
    }

    // Get network device source IP address and netmask.
    if (pcap_lookupnet(device, &srcip, &netmask, errbuf) < 0)
    {
        printf("pcap_lookupnet: %s\n", errbuf);
        return NULL;
    }

    // Convert the packet filter epxression into a packet
    // filter binary.
    if (pcap_compile(pd, &bpf, (char*)bpfstr, 0, netmask))
    {
        printf("pcap_compile(): %s\n", pcap_geterr(pd));
        return NULL;
    }

    // Assign the packet filter to the given libpcap socket.
    if (pcap_setfilter(pd, &bpf) < 0)
    {
        printf("pcap_setfilter(): %s\n", pcap_geterr(pd));
        return NULL;
    }

    return pd;
}

void capture_loop(pcap_t* pd, int packets, pcap_handler func)
{
    int linktype;
 
    // Determine the datalink layer type.
    if ((linktype = pcap_datalink(pd)) < 0)
    {
        printf("pcap_datalink(): %s\n", pcap_geterr(pd));
        return;
    }
 
    // Set the datalink layer header size.
    switch (linktype)
    {
    case DLT_NULL:
        linkhdrlen = 4;
        break;
 
    case DLT_EN10MB:
        linkhdrlen = 14;
        break;
 
    case DLT_SLIP:
    case DLT_PPP:
        linkhdrlen = 24;
        break;
 
    default:
        printf("Unsupported datalink (%d)\n", linktype);
        return;
    }
 
    // Start capturing packets.
    if (pcap_loop(pd, packets, func, 0) < 0)
        printf("pcap_loop failed: %s\n", pcap_geterr(pd));
}

void parse_packet(u_char *user, struct pcap_pkthdr *packethdr, 
                  u_char *packetptr)
{
    static struct ether_header* ether_header;
    static struct ip* iphdr;
    static struct icmphdr* icmphdr;
    static struct tcphdr* tcphdr;
    static struct udphdr* udphdr;
    char iphdrInfo[256], srcip[256], dstip[256];
    unsigned short id, seq;
    unsigned char type, code;
    static const u_char *payload;                    /* Packet payload */
	int size_ip;
	int size_tcp;
	int size_payload;

    // Get mac address from the datalink layer header
    ether_header = (struct ether_header*)(packetptr);
    char srcmac[18];
    char dstmac[18];
    hex2mac( ether_header->ether_shost, srcmac );
    hex2mac( ether_header->ether_dhost, dstmac );
    sprintf(globalpacket + strlen(globalpacket), "Mac %s->%s\n", 
        srcmac, dstmac);
 
    // Skip the datalink layer header and get the IP header fields.
    packetptr += linkhdrlen;
    iphdr = (struct ip*)packetptr;
    strcpy(srcip, inet_ntoa(iphdr->ip_src));
    strcpy(dstip, inet_ntoa(iphdr->ip_dst));
    sprintf(iphdrInfo, "ID:%d TOS:0x%x, TTL:%d IpLen:%d DgLen:%d",
            ntohs(iphdr->ip_id), iphdr->ip_tos, iphdr->ip_ttl,
            4*iphdr->ip_hl, ntohs(iphdr->ip_len));
    size_ip = iphdr->ip_hl*4;
 
    // Advance to the transport layer header then parse and display
    // the fields based on the type of hearder: tcp, udp or icmp.
    packetptr += 4*iphdr->ip_hl;
    switch (iphdr->ip_p)
    {
    case IPPROTO_TCP:
        tcphdr = (struct tcphdr*)packetptr;
        sprintf(globalpacket + strlen(globalpacket), "TCP  %s:%d -> %s:%d\n", srcip, ntohs(tcphdr->source),
               dstip, ntohs(tcphdr->dest));

        sprintf(globalpacket + strlen(globalpacket), "%s\n", iphdrInfo);
        sprintf(globalpacket + strlen(globalpacket), "%c%c%c%c%c%c Seq: 0x%x Ack: 0x%x Win: 0x%x TcpLen: %d\n",
               (tcphdr->urg ? 'U' : '*'),
               (tcphdr->ack ? 'A' : '*'),
               (tcphdr->psh ? 'P' : '*'),
               (tcphdr->rst ? 'R' : '*'),
               (tcphdr->syn ? 'S' : '*'),
               (tcphdr->fin ? 'F' : '*'),
               ntohl(tcphdr->seq), ntohl(tcphdr->ack_seq),
               ntohs(tcphdr->window), 4*tcphdr->doff);
        size_tcp = 4*tcphdr->doff;
    	if (size_tcp < 20) {
			printf("   * Invalid TCP header length: %u bytes\n", size_tcp);
			return;
		}
    	
    	/* define/compute tcp payload (segment) offset */
		payload = (u_char *)(tcphdr + size_tcp);
		
		/* compute tcp payload (segment) size */
		size_payload = ntohs(iphdr->ip_len) - (size_ip + size_tcp);
		
		/*
		 * Print payload data; it might be binary, so don't just
		 * treat it as a string.
		 */
		if (size_payload > 0) {
			sprintf(globalpacket + strlen(globalpacket), "Payload (%d bytes):\n", size_payload);
			//printf("%s", packet);
			print_payload(payload, size_payload);
			// sprintf(globalpacket + strlen(globalpacket), "%s\n", payload);
		}	
        break;
 
    case IPPROTO_UDP:
        udphdr = (struct udphdr*)packetptr;
        sprintf(globalpacket + strlen(globalpacket),"UDP  %s:%d -> %s:%d\n", srcip, ntohs(udphdr->source),
               dstip, ntohs(udphdr->dest));
        sprintf(globalpacket + strlen(globalpacket),"%s\n", iphdrInfo);
        int SIZE_UDP = 8;
    	payload = (u_char *)(udphdr + SIZE_UDP);
		
		/* compute udp payload (segment) size */
		size_payload = ntohs(iphdr->ip_len) - (size_ip + SIZE_UDP);
	         if (size_payload > ntohs(udphdr->len))
	                 size_payload = ntohs(udphdr->len);
		
		/*
		 * Print payload data; it might be binary, so don't just
		 * treat it as a string.
		 */
		if (size_payload > 0) {
			sprintf(globalpacket + strlen(globalpacket),"Payload (%d bytes):\n", size_payload);
			sprintf(globalpacket + strlen(globalpacket),payload, size_payload);
		}
        break;
 
    case IPPROTO_ICMP:
        icmphdr = (struct icmphdr*)packetptr;
        sprintf(globalpacket + strlen(globalpacket),"ICMP %s -> %s\n", srcip, dstip);
        sprintf(globalpacket + strlen(globalpacket),"%s\n", iphdrInfo);
        memcpy(&id, (u_char*)icmphdr+4, 2);
        memcpy(&seq, (u_char*)icmphdr+6, 2);
        memcpy(&type, (u_char*)icmphdr, 1);
        memcpy(&code, (u_char*)icmphdr+1, 1);

        sprintf(globalpacket + strlen(globalpacket),"Type:%d Code:%d ID:%d Seq:%d\n", type, code, 
               ntohs(id), ntohs(seq));
        break;
    }
    // sprintf(globalpacket + strlen(globalpacket), 
        // "+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n\n");
}

void bailout(int signo)
{
    struct pcap_stat stats;
 
    if (pcap_stats(pd, &stats) >= 0)
    {
        sprintf(globalpacket + strlen(globalpacket), "%d packets received\n", stats.ps_recv);
        sprintf(globalpacket + strlen(globalpacket), "%d packets dropped\n\n", stats.ps_drop);
    }
    pcap_freecode(&bpf);
    pcap_close(pd);
    // exit(0);
}

// int main(int argc, char **argv)
// {
//     char interface[256] = "", bpfstr[256] = "";
//     int packets = 0, c, i;
 
//     // Get the command line options, if any
//     while ((c = getopt (argc, argv, "hi:n:")) != -1)
//     {
//         switch (c)
//         {
//         case 'h':
//             printf("usage: %s [-h] [-i ] [-n ] []\n", argv[0]);
//             exit(0);
//             break;
//         case 'i':
//             strcpy(interface, optarg);
//             break;
//         case 'n':
//             packets = atoi(optarg);
//             break;
//         }
//     }

//     // Get the packet capture filter expression, if any.
//     for (i = optind; i < argc; i++)
//     {
//         strcat(bpfstr, argv[i]);
//         strcat(bpfstr, " ");
//     }
 
//     // Open libpcap, set the program termination signals then start
//     // processing packets.
//     if ((pd = open_pcap_socket(interface, bpfstr)))
//     {
//         signal(SIGINT, bailout);
//         signal(SIGTERM, bailout);
//         signal(SIGQUIT, bailout);
//         capture_loop(pd, packets, (pcap_handler)parse_packet);
//         bailout(0);
//     }
//     exit(0);
// }
bool getPataPacketRoutine(char* filter){

    char bpfstr[256] = "";
    int packets = 1, c, i;
    char *dev = NULL;           /* capture device name */
    char errbuf[PCAP_ERRBUF_SIZE];      /* error buffer */
    strcpy(globalpacket, "");
    dev = pcap_lookupdev(errbuf);
    if (dev == NULL) {
        fprintf(stderr, "Couldn't find default device: %s\n",
            errbuf);
        exit(EXIT_FAILURE);
    }
    strcpy(bpfstr, filter);
 
    // Open libpcap, set the program termination signals then start
    // processing packets.
    if ((pd = open_pcap_socket(dev, bpfstr)))
    {
        signal(SIGINT, bailout);
        signal(SIGTERM, bailout);
        signal(SIGQUIT, bailout);
        capture_loop(pd, packets, (pcap_handler)parse_packet);
        bailout(0);
    }
    return true;
}


bool getDataPacketFromIP (char* ip, char* packet){
    char* filter[256];
    sprintf(filter, "src host %s", ip);
    globalpacket = packet;
    getPataPacketRoutine(filter);
    return true;
}

bool getDataPacketToIP (char* ip, char* packet){
    char* filter[256];
    sprintf(filter, "dst host %s", ip);
    globalpacket = packet;
    getPataPacketRoutine(filter);
    return true;
}


bool getDataPacketFromIPAndPort (char* ip, char* port, char* packet){
    char* filter[256];
    sprintf(filter, "src host %s and src port %s", ip, port);
    globalpacket = packet;
    getPataPacketRoutine(filter);
    return true;
}

bool getDataPacketFromPort (char* port, char* packet){
    char* filter[256];
    sprintf(filter, "src port %s", port);
    globalpacket = packet;
    getPataPacketRoutine(filter);
    return true;
}

bool getDataPacketToPort (char* port, char* packet){
    char* filter[256];
    sprintf(filter, "dst port %s", port);
    globalpacket = packet;
    getPataPacketRoutine(filter);
    return true;
}


bool getDataPacketFromIPToIP (char* ip, char* ipto, char* packet){
    char* filter[256];
    sprintf(filter, "src host %s and dst host %s", ip, ipto);
    globalpacket = packet;
    getPataPacketRoutine(filter);
    return true;
}

bool getDataPacketFromMac (char* mac, char* packet){
    char* filter[256];
    sprintf(filter, "ether src %s", mac);
    globalpacket = packet;
    getPataPacketRoutine(filter);
    return true;
}

bool getDataPacketToMac (char* mac, char* packet){
    char* filter[256];
    sprintf(filter, "ether dst %s", mac);
    globalpacket = packet;
    getPataPacketRoutine(filter);
    return true;
}

bool getDataPacketFromMacToMac (char* srcmac, char* desmac, char* packet){
    char* filter[256];
    sprintf(filter, "ether src %s and ether dst %s", srcmac, desmac);
    globalpacket = packet;
    getPataPacketRoutine(filter);
    return true;
}

bool getDataPacketWithProtocol (char* type, char* packet){
    char* filter[256];
    sprintf(filter, "%s", type);
    globalpacket = packet;
    getPataPacketRoutine(filter);
    return true;
}