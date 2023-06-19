/***************************************************************************
 *   ITSNET  Intelligent Transport System networking Stack                 *
 * 									   *
 ** Copyright(C)2010 ESPRIT											   *
 * 	        "École supérieure privée d'ingénierie et de technologie"       *
 *                                                                         *
 *   barghich@gmail.com                                                    *
 *   anouar.chemek@gmail.com                                               *
 *  							                                           *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

/**
 * @file itsnet_listener.c
 * itsnet listener code.
 * @author hichem BARGAOUI
 * @author anouar Chemek
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <arpa/inet.h>
#include <asm/types.h>
#include <errno.h>
#include <pthread.h>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/un.h>
#include <unistd.h>

#include "geo_anycast.h"
#include "geo_broadcast.h"
#include "beaconing.h" 
#include "geo_unicast.h"
#include "itsnet_header.h"
#include "itsnet_listener.h"
#include "itsnet_parser.h"
#include "itsnet_pseudonym.h"
#include "itsnet_types.h"
#include "location_table.h"
#include "position_sensor.h"

#define SOCK_PATH "/tmp/upper_layer_socket"
#define SOCK_ITSNET_PATH "/tmp/itsnet_socket"
#define SOCK_BUG_PATH "/tmp/bug_socket"

static pthread_mutex_t mutex;
static pthread_t itsnet_listener;

static int sock;

unsigned char byte = 0xb4 ; 


/**
 * send_socket
 * @param message  *m
 * @return static int
 */
static int send_socket(message* m)
{
    int t, rc;
    int length, len;
    struct sockaddr_un client_sockaddr;

    sock = socket(AF_UNIX, SOCK_DGRAM, 0);
    if(sock == -1) {
        perror("socket");
        return 1;
    }

    memset(&client_sockaddr, 0, sizeof(struct sockaddr_un));
    client_sockaddr.sun_family = AF_UNIX;
    strcpy(client_sockaddr.sun_path, SOCK_PATH);
    len = sizeof(client_sockaddr);

    length = sizeof(struct message);

    sendto(sock, m, sizeof(message), 0, (struct sockaddr*)&client_sockaddr, sizeof(struct sockaddr_un));
    free(m);
    close(sock);
    return 0;
}

/**
 * itsnet_configure_node_id_confirm_send
 * @param struct message  *m
 * @return int
 */

int itsnet_configure_node_id_confirm_send()
{
    int result;
    struct message * m;
    m=(message*)malloc(sizeof(message));
    memset(m,0,sizeof(message));
    m->msg_hdr.sid = itsnet_management_sap;
    m->msg_hdr.opcode = itsnet_confirm;
    m->msg_hdr.aid = itsnet_configure_node_id;
    result = send_socket(m);
    return result;
}

/**
 * itsnet_get_node_id_confirm_send
 * @param void
 * @return int
 */
int itsnet_get_node_id_confirm_send()
{

    int result;
    struct itsnet_node_id node_id;
    struct message *m;
    int res;

    m = (message*)malloc(sizeof(message));
    memset(m,0,sizeof(message));
    m->msg_hdr.sid = itsnet_management_sap;
    m->msg_hdr.opcode = itsnet_confirm;
    m->msg_hdr.aid = itsnet_get_node_id;

    // memcpy(&(m->payload.itsnet_get_node_id_conf.node_id),get_ego_node_id(),NODE_ID_LEN);
    m->payload.itsnet_get_node_id_conf.node_id = *(get_ego_node_id());

    res = send_socket(m);

    return res;
}

/**
 * itsnet_configure_node_id_request_receive
 * @param struct message *m
 * @return int
 */

int itsnet_configure_node_id_request_receive(struct message* m)
{

    int result;
    struct message* msg;
    struct itsnet_node_id new_id;
    struct itsnet_node_id old_id;

    // memcpy(&old_node_id,get_ego_node_id(),NODE_ID_LEN);
    old_id = *(get_ego_node_id());
    set_ego_node_id(&(m->payload.itsnet_configure_node_id_req.node_id));
    msg = (message*)malloc(sizeof(message));
    // memset(msg, 0, sizeof(message));
    // memcpy(msg->payload.itsnet_configure_node_id_conf.old_node_id,old_node_id,NODE_ID_LEN);
    msg->payload.itsnet_configure_node_id_conf.old_id = old_id;
    // memcpy(&(msg->payload.itsnet_configure_node_id_conf.new_node_id),get_ego_node_id(),NODE_ID_LEN);
    msg->payload.itsnet_configure_node_id_conf.new_id = *(get_ego_node_id());
    msg->payload.itsnet_configure_node_id_conf.result = 1;
    printf("NODEID:%hhn\n", get_ego_node_id()->id);
    result = itsnet_configure_node_id_confirm_send(msg);
    return result;
}

/**
 * itsnet_get_node_id_request_receive
 * @param struct message *m
 * @return int
 */

int itsnet_get_node_id_request_receive(struct message* m)
{
    int result;
    result = itsnet_get_node_id_confirm_send();
    return result;
}

/**
 * itsnet_get_neighbor_list_confirm_send
 * @param struct message *m
 * @return int
 */
int itsnet_get_neighbor_list_confirm_send()
{

    struct message* m;
    int result, neighbor_list_size, j;
    j = 0;
    neighbor_list_size = 0;
    const itsnet_node* node;
    itsnet_position_vector neighbor_list_pos[MAX_NEIGHBOR_LIST];
    struct list_head* node_list_entry;

    m = (message*)malloc(sizeof(message));
    memset(m,0,sizeof(message));

    m->msg_hdr.sid = itsnet_management_sap;
    m->msg_hdr.opcode = itsnet_confirm;
    m->msg_hdr.aid = itsnet_get_neighbor_list;

    list_for_each(node_list_entry, &neighbor_list)
    {
        itsnet_node* i;
        i = list_entry(node_list_entry, struct itsnet_node, list);
        neighbor_list_pos[j] = i->pos_vector;
        neighbor_list_size += 1;
        j += 1;
    }

    m->payload.itsnet_get_neighbor_list_conf.neighbor_list_size = neighbor_list_size;

    for(j = 0; j < neighbor_list_size; j++) {

        m->payload.itsnet_get_neighbor_list_conf.neighbor_list_pos[j] = neighbor_list_pos[j];
    }

    result = send_socket(m);

    return result;
}

/**
 * itsnet_get_neighbor_list_request_receive
 * @param struct message *m
 * @return int
 */
int itsnet_get_neighbor_list_request_receive(struct message* m)
{
    int result;
    result = itsnet_get_neighbor_list_confirm_send(m);
    return result;
}

/**
 * itsnet_get_position_vector_confirm_send
 * @param struct message *m
 * @return int
 */

int itsnet_get_position_vector_confirm_send()
{
    struct message* m;
    struct itsnet_position_vector pos;
    int res;
    pos = get_position_vector();
    m=(message*)malloc(sizeof(message));
    memset(m,0,sizeof(message));
    m->msg_hdr.sid = itsnet_management_sap;
    m->msg_hdr.opcode = itsnet_confirm;
    m->msg_hdr.aid = itsnet_get_position_vector;
    m->payload.itsnet_get_position_vector_conf.position_vector = pos;
    res = send_socket(m);
    return res;
}

/**
 * itsnet_get_position_vector_request_receive
 * @param struct message *m
 * @return int
 */

int itsnet_get_position_vector_request_receive(struct message* m)
{

    int result;
    result = itsnet_get_position_vector_confirm_send();
    return result;
}

/**
 * itsnet_position_sensor_confirm_send
 * @param struct message *m
 * @return int
 */

int itsnet_position_sensor_confirm_send()
{

    return 0;
}

/**
 * itsnet_position_sensor_request_receive
 * @param struct message *m
 * @return int
 */
int itsnet_position_sensor_request_receive(struct message* m)
{
    pthread_mutex_lock(&mutex);

    position_data_update(m->payload.itsnet_position_sensor_req.position_information);

    pthread_mutex_unlock(&mutex);

    return 0;
}

/**
 * itsnet_event_indication_send
 * @param struct message *m
 * @return int
 */

int itsnet_event_indication_send(struct message* m)
{
    int res;
    res = send_socket(m);
    return res;
}

/**
 * itsnet_unicast_confirm_send
 * @param message
 * @return int
 */

int itsnet_unicast_confirm_send(int res)
{

    struct message* m;

    m = (message*)malloc(sizeof(message));
    memset(m,0,sizeof(message));
    m->msg_hdr.sid = itsnet_transport_sap;
    m->msg_hdr.opcode = itsnet_confirm;
    m->msg_hdr.aid = itsnet_unicast;

    m->payload.itsnet_unicast_conf.result = res;

    res = send_socket(m);
    return res;
}

/**
 * itsnet_geoanycast_confirm_send
 * @param message
 * @return int
 */

int itsnet_geoanycast_confirm_send(int res)
{

    struct message* m;

    m = (message*)malloc(sizeof(message));
    memset(m,0,sizeof(message));
    m->msg_hdr.sid = itsnet_transport_sap;
    m->msg_hdr.opcode = itsnet_confirm;
    m->msg_hdr.aid = itsnet_geoanycast;

    m->payload.itsnet_geoanycast_conf.result = res;

    res = send_socket(m);
    return res;
}

/**
 * itsnet_geobroadcast_confirm_send
 * @param message
 * @return int
 */
int itsnet_geobroadcast_confirm_send(int res)
{

    struct message* m;

    m = (message*)malloc(sizeof(message));

    memset(m,0,sizeof(message));

    m->msg_hdr.sid = itsnet_transport_sap;
    m->msg_hdr.opcode = itsnet_confirm;
    m->msg_hdr.aid = itsnet_geobroadcast;

    m->payload.itsnet_geobroadcast_conf.result = res;

    res = send_socket(m);
    return res;
}

/**
 * itsnet_geounicast_request_receive
 * @param struct message *m
 * @return int
 */

int itsnet_geounicast_request_receive(struct message* m)
{
    int result;

    struct itsnet_packet* p;

    struct itsnet_position_vector pos;

    pos = get_position_vector();

    p = (itsnet_packet*)malloc(sizeof(itsnet_packet));
    itsnet_node_id nodeId;

    if(p == NULL) {
        printf("erreur allocation \n");
        result = -1;
    } else {

        /*See Section 9 : C2CNet packets format*/
        memset(p, 0, sizeof(itsnet_packet));
        p->basic_header.itsnet_version_next_header = HI_NIBBLE(1);
        p->basic_header.itsnet_version_next_header |= LO_NIBBLE(common_header);
        p->basic_header.itsnet_lifetime=lt_base_10s;
        p->basic_header.itsnet_rhl=10;
        
        p->common_header.itsnet_header_type_subtype = HI_NIBBLE(itsnet_unicast_id);
        p->common_header.itsnet_header_type_subtype |= LO_NIBBLE(UNSPECIFIED);
        p->common_header.traffic_class = CLASS01;
       
        p->common_header.flags = 0;
        p->common_header.flags = Mobile << 7;
        //p->common_header.payload_lenght = htons(1514);
        //p->common_header.payload_lenght = htons(sizeof(*p));
        p->common_header.payload_lenght = 0;
        p->common_header.max_hop_limit = 255;
        p->common_header.reserved=0;

        
        p->payload.itsnet_unicast.forwarder_position_vector = pos;
        p->payload.itsnet_unicast.source_position_vector = pos;
        p->payload.itsnet_unicast.source_position_vector.node_id = *(get_ego_node_id());
        /*get information from application ( Data and node destination, "see message structure"  )*/
        p->payload.itsnet_unicast.dest_latitude = m->payload.itsnet_unicast_req.destination_latitude;
        p->payload.itsnet_unicast.dest_longitude = m->payload.itsnet_unicast_req.destination_longitude;
        p->payload.itsnet_unicast.dest_node_id = m->payload.itsnet_unicast_req.destination_node_id;
        memcpy(p->payload.itsnet_unicast.payload, m->payload.itsnet_unicast_req.data, ITSNET_DATA_SIZE);

        result = itsnet_geounicast_send(p);
    }
    //	result=itsnet_geobroadcast_confirm_send(result);
    return result;
}

/**
 * itsnet_geoanycast_request_receive
 * @param struct message *m
 * @return int
 */

int itsnet_geoanycast_request_receive(struct message* m)
{

    int result;
    struct itsnet_packet* p;
    struct itsnet_position_vector pos;
    pos = get_position_vector();
    p = (itsnet_packet*)malloc(sizeof(itsnet_packet));
    itsnet_node_id nodeId;

    if(p == NULL) {
        printf("erreur allocation \n");
        result = -1;
    } else {
        memset(p, 0, sizeof(itsnet_packet));
        p->basic_header.itsnet_version_next_header = HI_NIBBLE(1);
        p->basic_header.itsnet_version_next_header |= LO_NIBBLE(common_header);
        p->basic_header.itsnet_lifetime=lt_base_10s;
        p->basic_header.itsnet_rhl=10;
        
        p->common_header.itsnet_header_type_subtype = HI_NIBBLE(itsnet_geoanycast_id);
        p->common_header.itsnet_header_type_subtype |= LO_NIBBLE(m->payload.itsnet_geoanycast_req.geo_area.geo_area_type);
        p->common_header.traffic_class = CLASS01;
       
        p->common_header.flags = 0;
        p->common_header.flags = Mobile << 7;
        //p->common_header.payload_lenght = htons(1514);
        //p->common_header.payload_lenght = htons(sizeof(*p));
        p->common_header.payload_lenght = 0;
        p->common_header.max_hop_limit = 255;
        p->common_header.reserved=0;

        p->payload.itsnet_geoanycast.source_position_vector = pos;
        p->payload.itsnet_geoanycast.source_position_vector.node_id = *(get_ego_node_id());
        p->payload.itsnet_geoanycast.dest_latitude = m->payload.itsnet_geoanycast_req.geo_area.latitude;
        p->payload.itsnet_geoanycast.dest_longitude = m->payload.itsnet_geoanycast_req.geo_area.longitude;
        p->payload.itsnet_geoanycast.geo_area_size = m->payload.itsnet_geoanycast_req.geo_area.geo_area_size;
        memcpy(p->payload.itsnet_geoanycast.payload, m->payload.itsnet_geoanycast_req.data, ITSNET_DATA_SIZE);

        result = itsnet_geoanycast_send(p);
    }
    //	result=itsnet_geobroadcast_confirm_send(result);
    return result;
}

/**
 * itsnet_geobroadcast_request_receive
 * @param struct message *m
 * @return int
 */

int itsnet_geobroadcast_request_receive(struct message* m)
{

    int result;
    struct itsnet_packet* p;
    struct itsnet_position_vector pos;
    static struct timeval t_emit;
    pos = get_position_vector();
    p = (itsnet_packet*)malloc(sizeof(itsnet_packet));
    struct itsnet_node_id nodeId;
    
    
    
    

    if(p == NULL) {
        printf("erreur allocation \n");
        result = -1;
    } else {

        memset(p, 0, sizeof(itsnet_packet));
        
        p->basic_header.itsnet_version_next_header = HI_NIBBLE(1);
        p->basic_header.itsnet_version_next_header |= LO_NIBBLE(common_header);
        p->basic_header.itsnet_lifetime=lt_base_10s;
        p->basic_header.itsnet_rhl=10;
        
        p->common_header.itsnet_header_type_subtype = HI_NIBBLE(itsnet_geobroadcast_id);
        p->common_header.itsnet_header_type_subtype |= LO_NIBBLE(m->payload.itsnet_geobroadcast_req.geo_area.geo_area_type);
        p->common_header.traffic_class = CLASS01;
       
        p->common_header.flags = 0;
        p->common_header.flags = Mobile << 7;
        //p->common_header.payload_lenght = htons(1514);
        //p->common_header.payload_lenght = htons(sizeof(*p));
        p->common_header.payload_lenght = 0;
        p->common_header.max_hop_limit = 255;
        p->common_header.reserved=0;
        
        
        p->payload.itsnet_geobroadcast.forwarder_position_vector = pos;
        p->payload.itsnet_geobroadcast.forwarder_position_vector.node_id = *(get_ego_node_id());
        p->common_header.itsnet_next_header = HI_NIBBLE(BTP_B);
        p->common_header.itsnet_next_header |= LO_NIBBLE(0);

        p->payload.itsnet_geobroadcast.source_position_vector = pos;
        p->payload.itsnet_geobroadcast.dest_latitude = m->payload.itsnet_geobroadcast_req.geo_area.latitude;
        p->payload.itsnet_geobroadcast.dest_longitude = m->payload.itsnet_geobroadcast_req.geo_area.longitude;
        p->payload.itsnet_geobroadcast.geo_area_size = m->payload.itsnet_geobroadcast_req.geo_area.geo_area_size;

        memcpy(p->payload.itsnet_geobroadcast.payload,m->payload.itsnet_geobroadcast_req.data,1482);
        memcpy(p->payload.itsnet_geobroadcast.payload, m->payload.itsnet_geobroadcast_req.data, ITSNET_DATA_SIZE); 
        gettimeofday(&t_emit, NULL);
        result = itsnet_geobroadcast_send(p);
        trace_emit_packet(t_emit);
    }
    result=itsnet_geobroadcast_confirm_send(result);
    return result;
}

void decimalToHexadecimal(int decimal) {
    char hexadecimal[100];
    int index = 0;
    
    while (decimal > 0) {
        int remainder = decimal % 16;
        
        if (remainder < 10) {
            hexadecimal[index] = remainder + '0';
        } else {
            hexadecimal[index] = remainder - 10 + 'A';
        }
        
        decimal /= 16;
        index++;
    }
    
    printf("La valeur en hexadécimal est : ");
    
    for (int i = index - 1; i >= 0; i--) {
        printf("%c", hexadecimal[i]);
    }
    
    printf("\n");
}


int itsnet_btp_request_receive(struct message* m) {
	
	
	long x2 = 45 ; 
	int result;
    	//struct itsnet_packet* p;
    	
    	struct itsnet_btp_packet* p ; 
    	
    	struct itsnet_position_vector pos;
    	static struct timeval t_emit;
    	pos = get_position_vector();
    	uint8_t brodcast_mac[MAX_LLA_LEN];
    	long x = sizeof(btp_b_pdu_t) ; 
    	
    	x = htonl(x) ;
	printf("\nAfter htonl() \n") ; 
	printf("x in hex : %x\n", x); 
    	unsigned char *ptr = (char*)&x ; 
    	printf("ptr = %x %x \n", ptr[2], ptr[3]) ;
	
	brodcast_mac[0]=0xff;
    	brodcast_mac[1]=0xff;
    	brodcast_mac[2]=0xff;
    	brodcast_mac[3]=0xff;
    	brodcast_mac[4]=0xff;
    	brodcast_mac[5]=0xff;
    	
    	long x1 = htonl(btp_port_DENM) ;
	unsigned char *ptr1 = (char*)&x1 ;
	
	x2 = htonl(x2) ;
	unsigned char *ptr2 = (char*)&x2 ;  
	 
    	
    	
   p = (struct itsnet_btp_packet*)malloc(sizeof(struct itsnet_btp_packet));
    
    if(p == NULL) {
        printf("erreur allocation \n");
    } else  {
    
    	memset(p, 0, sizeof(struct itsnet_btp_packet));
        
        p->basic_header.itsnet_version_next_header = HI_NIBBLE(1);
        p->basic_header.itsnet_version_next_header |= LO_NIBBLE(common_header);
        p->basic_header.itsnet_lifetime=lt_base_10s;
        p->basic_header.itsnet_rhl=0;
        

        p->common_header.itsnet_next_header =  HI_NIBBLE(BTP_B); /** 4bits common header next header */
        p->common_header.itsnet_next_header |=  LO_NIBBLE(0); /** 4bits reserved must be set to zero */
        p->common_header.itsnet_header_type_subtype =  HI_NIBBLE(itsnet_geobroadcast_id);
        p->common_header.itsnet_header_type_subtype |= LO_NIBBLE(UNSPECIFIED);
            
        p->common_header.traffic_class = CLASS03;
        p->common_header.flags = 0;
        p->common_header.flags  =  Mobile << 7;
        p->common_header.payload_lenght = (ptr2[3] << 8) | ptr2[2];
        //p->common_header.payload_lenght = (ptr[3] << 8) | ptr[2];   
        p->common_header.max_hop_limit = 0;
        p->common_header.reserved= 0;
        
        /* remplissage du payload du geobroadcast */ 
        
        p->itsnet_geobroadcast_btp.sequence_number = 0x4 ;  
        p->itsnet_geobroadcast_btp.reserved_1 = 0x3 ; 
        p->itsnet_geobroadcast_btp.source_position_vector=pos ; 
        p->itsnet_geobroadcast_btp.dest_latitude = 15 ; 
        p->itsnet_geobroadcast_btp.dest_longitude = 10 ; 
        p->itsnet_geobroadcast_btp.radius = 4; 
        p->itsnet_geobroadcast_btp.distance_b = 5 ; 
        p->itsnet_geobroadcast_btp.angle = 1 ; 
        p->itsnet_geobroadcast_btp.reserved_2 = 2;
        
        /*p->payload.itsnet_geobroadcast_btp.sequence_number = 4 ;  
        p->payload.itsnet_geobroadcast_btp.reserved_1 = 3 ; 
        p->payload.itsnet_geobroadcast_btp.source_position_vector=pos ; 
        p->payload.itsnet_geobroadcast_btp.dest_latitude = 15 ; 
        p->payload.itsnet_geobroadcast_btp.dest_longitude = 10 ; 
        p->payload.itsnet_geobroadcast_btp.radius = 4; 
        p->payload.itsnet_geobroadcast_btp.distance_b = 5 ; 
        p->payload.itsnet_geobroadcast_btp.angle = 0 ; 
        p->payload.itsnet_geobroadcast_btp.reserved_2 = 0;*/
        
        //p->payload.itsnet_geobroadcast_btp.btp = m->payload.btp ; 
        
        /* remplissage du btp_pdu */ 
                
        /* remplir tous les champs du packets */
        	
        //p->btp.btp_header.source_port = 2002 ;
        
        
        /*printf(" received message : \n") ; 
        printf(" btp header : \n") ;*/  
        
        //p->btp = m->payload.btp ;
        
        //p->btp.btp_header = {(ptr1[3] << 8) | ptr1[2], 2}
        
        
        printf("=============== DENM IN ITSNET ================= \n"); 
	printf("=============== ITS PDU HEADER ====================== \n") ; 
	printf("protocol version = %d | message id = %d | station id = ", m->payload.btp.payload.header.protocol_version, m->payload.btp.payload.header.message_id); 
	for(int i = 0; i<8; i++) 
		printf("%d \t", m->payload.btp.payload.header.station_id.id[i]) ; 
	printf("\n") ; 
	printf("============== PAYLOAD ============================== \n") ; 
	printf("------------ management container ------------------ \n") ; 
	printf("station type = %d | detection time = %d | reference time = %d | relevance distance = %d | relevance traffic direction = %d | validity duration = %d \n", m->payload.btp.payload.payload.management.station_type, m->payload.btp.payload.payload.management.detection_time, m->payload.btp.payload.payload.management.reference_time, m->payload.btp.payload.payload.management.relevance_distance, m->payload.btp.payload.payload.management.relevance_traffic_direction, m->payload.btp.payload.payload.management.validity_duration) ; 
	printf("===================================================== \n");
        
        p->btp.btp_header.destination_port =  (ptr1[3] << 8) | ptr1[2];
        p->btp.btp_header.destination_port_info = 258 ;
        p->btp.payload.header.protocol_version = 3 ; 
        p->btp.payload.header.message_id = 1 ; 
         
        //p->btp.payload.header.station_id = 1065800 ; 
        //p->btp.payload.payload.management.action_id.originating_station_id = 5 ; 
	p->btp.payload.payload.management.action_id.sequence_number = 1 ; 

        gettimeofday(&t_emit, NULL);
        //result = itsnet_geobroadcast_send(p);
        printf("result = %d \n ", result) ; 
        //printf("btp packet is sent \n") ;	
        //itsnet_beacon_send_btp(p) ;
        
        /* afficher les champs reçus */ 
        
        //printf("btp source port : %d octets \n", m->payload.btp.btp_header.source_port ) ; 
        
        /*printf("taille du packet dans itsnet_listener:   %d octets \n", sizeof(itsnet_packet)) ;
        
        printf("taille de p : %d octets \n", sizeof(*p)) ;
        
        printf("taille de GBC packet + btp_pdu : %d octets \n", sizeof(struct itsnet_geobroadcast_btp_t) + sizeof(btp_pdu_t)) ;  
        
        //printf("taille des champs de p après remplissage : %d octets \n", sizeof(p->basic_header) + sizeof(p->common_header) + sizeof(p->payload.itsnet_geobroadcast_btp)) ;  
        
        printf("taille de : common_header + basic_header + itsnet_geobroadcast_btp : %d \n", sizeof(struct itsnet_common_header) + sizeof(struct itsnet_basic_header) + sizeof(struct itsnet_geobroadcast_btp_t )) ; 
        
        printf("taille du btp_pdu : %d octets \n", sizeof(btp_pdu_t)) ; 
        
        printf("taille du btp_pdu reçu : %d octets \n", sizeof(m->payload.btp)) ; 
        
        printf("taille du btp header dans le message : %d octets \n", sizeof(m->payload.btp.btp_header)) ;
        
        printf("taille du btp payload dans le message : %d octets \n", sizeof(m->payload.btp.payload)) ; 
        
        //printf("taille du btp length dans le message : %d octets \n", sizeof(m->payload.btp.payload_length)) ;
        
        printf("taille du btp pdu dans le message : header + payload + payload_length %d \n", sizeof(m->payload)) ;
        
        printf(" ******************************* \n") ; 
        
        printf("taille du btp header : %d octets \n", sizeof(btp_header_t)); 
        
        printf("taille du paylod : %d octets  \n", sizeof(DENM_t)) ;
        
        printf("taille du packet denm %d \n", sizeof(DENM_t)) ;
        
        printf("taille du btp pdu : %d \n ", sizeof(btp_pdu_t)) ; */
        
        printf("taille du btp_pdu de type B : %d \n", sizeof (btp_b_pdu_t)) ; 
          
        result = itsnet_btp_packet_send(p, brodcast_mac);
	//result = itsnet_packet_send(p, brodcast_mac);
          
        trace_emit_packet(t_emit);
        
    
        
        	
        return result ;  
        
}

}

/**
 * itsnet_unicast_indication_send
 * @param struct itsnet_packet *p
 * @return int
 */
int itsnet_unicast_indication_send(struct itsnet_packet* p)
{
    int res;
    struct message* m;

    m = (message*)malloc(sizeof(message));

    memset(m, 0, sizeof(message));

    m->msg_hdr.sid = itsnet_transport_sap;
    m->msg_hdr.aid = itsnet_unicast;
    m->msg_hdr.opcode = itsnet_indication;

    memcpy(m->payload.itsnet_unicast_ind.data, p->payload.itsnet_unicast.payload, MES_PAYLOAD_LEN);
    // memcpy(m->payload.itsnet_unicast_ind.source_node_id,p->payload.itsnet_unicast.source_position_vector.id,NODE_ID_LEN);
    m->payload.itsnet_unicast_ind.source_node_id = p->payload.itsnet_unicast.source_position_vector.node_id;
    res = send_socket(m);
    return res;
}

/**
 * itsnet_geoanycast_indication_send
 * @param struct itsnet_packet *p
 * @return int
 */

int itsnet_geoanycast_indication_send(struct itsnet_packet* p)
{
    int res;
    struct message* m;

    m = (message*)malloc(sizeof(message));

    memset(m,0,sizeof(message));
    m->msg_hdr.sid = itsnet_transport_sap;
    m->msg_hdr.aid = itsnet_geoanycast;
    m->msg_hdr.opcode = itsnet_indication;

    memcpy(m->payload.itsnet_geoanycast_ind.data, p->payload.itsnet_geoanycast.payload, MES_PAYLOAD_LEN);
    // memcpy(m->payload.itsnet_geoanycast_ind.source_node_id,p->payload.itsnet_geoanycast.source_position_vector.id,NODE_ID_LEN);
    m->payload.itsnet_geoanycast_ind.source_node_id = p->payload.itsnet_geoanycast.source_position_vector.node_id;
    res = send_socket(m);
    return res;
}

/**
 * itsnet_geobroadcast_indication_send
 * @param struct itsnet_packet *p
 * @return int
 */
int itsnet_geobroadcast_indication_send(struct itsnet_packet* p)
{
    int res;
    struct message* m;

    m = (message*)malloc(sizeof(message));

    memset(m,0,sizeof(message));

    m->msg_hdr.sid = itsnet_transport_sap;
    m->msg_hdr.aid = itsnet_geobroadcast;
    m->msg_hdr.opcode = itsnet_indication;

    memcpy(m->payload.itsnet_geobroadcast_ind.data, p->payload.itsnet_geobroadcast.payload, MES_PAYLOAD_LEN);

    m->payload.itsnet_geobroadcast_ind.source_node_pos.node_id = p->payload.itsnet_geobroadcast.source_position_vector.node_id;
    m->payload.itsnet_geobroadcast_ind.forwarder_node_pos.node_id = p->payload.itsnet_geobroadcast.forwarder_position_vector.node_id;
    m->payload.itsnet_geobroadcast_ind.source_node_pos = p->payload.itsnet_geobroadcast.source_position_vector;
    m->payload.itsnet_geobroadcast_ind.forwarder_node_pos = p->payload.itsnet_geobroadcast.forwarder_position_vector;
    res = send_socket(m);
    return res;
}

/**
 * itsnet_get_security_param_request_receive
 * @param struct message *m
 * @return int
 */

int itsnet_get_security_param_request_receive(struct message* m)
{

    /** TODO*/
    return 0;
}

/**
 * itsnet_configure_security_param_request_receive
 * @param struct message *m
 * @return int
 */

int itsnet_configure_security_param_request_receive(struct message* m)
{

    /** TODO*/
    return 0;
}



/**
 * socket_recv
 * @param struct message *m
 * @return void
 */
void socket_recv(struct message* m)
{

    int result = 1;
    
    printf("aid =%d \n", m->msg_hdr.aid) ; 
    //printf("station type before switch = %d \n", m->payload.btp.payload.payload.management.station_type) ; 
    printf("detection time before switch = %d \n", m->payload.btp.payload.payload.management.detection_time) ; 

    switch(m->msg_hdr.aid) {
    case itsnet_configure_node_id:
        //printf("configure node _ sizeof m:%ld\n", sizeof(*m));
        result = itsnet_configure_node_id_request_receive(m);
        break;
    case itsnet_get_node_id:
        result = itsnet_get_node_id_request_receive(m);
        // printf("itsnet_get_node_id  \n");
        break;
    case itsnet_get_neighbor_list:
        result = itsnet_get_neighbor_list_request_receive(m);
        //printf("itsnet_get_neighbor_list \n");
        break;
    case itsnet_get_position_vector:
        result = itsnet_get_position_vector_request_receive(m);
        //printf("itsnet_get_position_vector \n");
        break;
    case itsnet_position_sensor:
        result = itsnet_position_sensor_request_receive(m);
        //printf("itsnet_position_sensor \n");
        break;
    case itsnet_unicast:
        result = itsnet_geounicast_request_receive(m);
        // printf("itsnet_unicast \n");
        break;
    case itsnet_geoanycast:
        // printf("itsnet_geoanycast \n");
        result = itsnet_geoanycast_request_receive(m);
        break;
    case itsnet_geobroadcast:
        //printf("itsnet_geobroadcast \n");
        result = itsnet_geobroadcast_request_receive(m);
        break;
    case itsnet_get_security_param:
        result = itsnet_get_security_param_request_receive(m);
        // printf("itsnet_get_security_param \n");
        break;
    case itsnet_configure_security_param:
        result = itsnet_configure_security_param_request_receive(m);
        // printf("itsnet_configure_security_param \n");
        break;
    case itsnet_btp_request: 
    	result = itsnet_btp_request_receive(m) ;
    	//result = itsnet_geobroadcast_request_receive(m) ;
    	printf("station type in socket_recv = %d \n", m->payload.btp.payload.payload.management.station_type) ;
    	printf("itsnet_btp_request \n") ;
    	break ;   
    default:
        printf("unknown type\n");
        break;
    }
    free(m);
}

/**
 * itsnet listener init
 * @param void
 * @return int
 */
int itsnet_listener_init(void)
{
    pthread_mutexattr_t mattrs;
    pthread_mutexattr_init(&mattrs);
    pthread_mutexattr_settype(&mattrs, PTHREAD_MUTEX_TIMED_NP);

    if(pthread_mutex_init(&mutex, &mattrs) || pthread_create(&itsnet_listener, NULL, listener, NULL))
        return -1;
    return 0;
}

/**
 * server
 * @param int client_socket
 * @return int
 */

int server(int client_socket)
{

    struct message* msg;
    while(1) {
        msg = (struct message*)malloc(sizeof(message));
        memset(msg,0,sizeof(message));
        recv(client_socket, msg, sizeof(message), 0);
        
  
        socket_recv(msg);
    }
    
}

/**
 * listener
 * @param void
 * @return void
 */
void* listener(void* arg)
{
    int s, s2, len;
    unsigned int t;
    struct sockaddr_un local, remote;

    if((s = socket(AF_UNIX, SOCK_DGRAM, 0)) == -1) {
        perror("socket");
    }

    local.sun_family = AF_UNIX;
    strcpy(local.sun_path, SOCK_ITSNET_PATH);
    len = sizeof(local);
    unlink(local.sun_path);

    if(bind(s, (struct sockaddr*)&local, len) == -1) {
        perror("bind");
        close(s);
        exit(1);
    }
    server(s);
    close(s);
    unlink(SOCK_ITSNET_PATH);
}

/**
 *  itsnet listener cleanup
 * @param void
 * @return void
 */
void itsnet_listener_cleanup(void)
{
}

void trace_emit_packet(struct timeval t_emit)
{

    static FILE* f_emit;
    /*static denm* BUFF;
    BUFF = malloc(sizeof(struct denm));*/
    f_emit = fopen("/root/emit_dll.log", "a");
    /*memcpy((void*)BUFF, (void*)p->payload.itsnet_geobroadcast.payload, sizeof(struct denm));*/
    fprintf(f_emit, "TIME %lld\n", timeval_to_us(&t_emit));
    fclose(f_emit);
    /*free(BUFF);*/
}
