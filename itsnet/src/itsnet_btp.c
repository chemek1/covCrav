/***************************************************************************
 *            its_btp_types.c
 *
 *  sam. avril 10 16:15:14 2021
 *  Copyright  2021  anouar chemek
 *  <user@host>
 ****************************************************************************/
/*
 * its_btp_types.c
 *
 * Copyright (C) 2021 - anouar chemek
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/time.h>
#include <sys/timex.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include "itsnet_btp.h" 
#include "itsnet_header.h" 
#include "itsnet_types.h" 

 
extern struct itsnet_position_vector pos_vector;

double GEO_AREA = 300;
uint16_t period = 500;
int nbr_packet = 1000;

#define SOCK_ITSNET_PATH "/tmp/itsnet_socket"
  
struct message* btp_data_request(btp_tx_params_t params) {

int length, len, sock ; 

//contruction of btp_udp

btp_pdu_t* p ; 
p = (btp_pdu_t*)malloc(sizeof(btp_pdu_t)) ; 
memset(p,0,sizeof(btp_pdu_t)) ; 
if (p == NULL) 
	printf("allocation error ! \n") ; 
else {
	p->btp_header.destination_port = params.destination_port ; 
	if(params.btp_type == BTPA) 
		p->btp_header.source_port = params.source_port ; 
	else 
		p->btp_header.source_port = btp_Port_UNSPECIFIED ; 
	p->payload = params.data ; 
	p->payload_length = sizeof(params.data) ; 
	
	//construction of message 
	
	struct message* msg ; 
	
	msg = (message*)malloc(sizeof(message)) ; 
	
	if (memset(msg, 0, sizeof(message)) == NULL)
		printf("error initializing message \n ") ; 
	
	
	
	msg->msg_hdr.sid = itsnet_transport_sap ; 
	msg->msg_hdr.opcode = itsnet_request ; 
	msg->msg_hdr.aid = itsnet_btp_request ; 
	
	/*msg->payload.itsnet_geobroadcast_req.geo_area.latitude = 1;
	msg->payload.itsnet_geobroadcast_req.geo_area.longitude = 1;
	msg->payload.itsnet_geobroadcast_req.geo_area.geo_area_size = GEO_AREA;*/ 
	
	msg->payload.btp = *p ; 
	

	
	//send message to GN protocol and transport 
	
	struct sockaddr_un sockaddr ; 
	
	sock = socket(AF_UNIX, SOCK_DGRAM, 0) ; 
	if(sock == -1) {
		perror("socket") ;
		printf("error in creating socket \n") ; 
	} 
	
	if ( memset(&sockaddr, 0, sizeof(struct sockaddr_un)) == NULL)  
		printf("error in initializing sockaddr \n") ; 
	sockaddr.sun_family = AF_UNIX ; 
	strcpy(sockaddr.sun_path, SOCK_ITSNET_PATH) ; 
	
	len = sizeof(struct sockaddr_un);
    	length = sizeof(struct message);
    	
    	int bytes_sent = sendto(sock, msg, length, 0, (struct sockaddr*)&sockaddr, len) ;

    	if ( bytes_sent < 0 ) {
    	
    	switch (errno) {
            case EACCES:
                fprintf(stderr, "Access to socket denied\n");
                break;
            case EAGAIN || EWOULDBLOCK :
            //case EWOULDBLOCK:
                fprintf(stderr, "Socket is non-blocking and the operation would block\n");
                break;
            case EINTR:
                fprintf(stderr, "Sendto was interrupted by a signal\n");
                break;
            case EINVAL:
                fprintf(stderr, "Invalid argument passed to sendto\n");
                break;
            case EISCONN:
                fprintf(stderr, "Socket is already connected\n");
                break;
            case EMSGSIZE:
                fprintf(stderr, "Message too long for specified protocol\n");
                break;
            case ENOTCONN:
                fprintf(stderr, "Socket is not connected\n");
                break;
            case EPIPE:
                fprintf(stderr, "Socket has been shut down\n");
                break;
            default:
                fprintf(stderr, "Sendto failed with error: %s\n", strerror(errno));
                break;
        }
        exit(EXIT_FAILURE);
    }
    	
    	
    	
    	
    		
    	
    	/*printf("geo area : %d \n", msg->payload.itsnet_geobroadcast_req.geo_area.geo_area_size) ;
	printf("latitude : %d \n", msg->payload.itsnet_geobroadcast_req.geo_area.latitude) ;
	printf("longitude : %d \n", msg->payload.itsnet_geobroadcast_req.geo_area.longitude) ; 
    		
printf("source port : %d \n", msg->payload.btp.btp_header.source_port) ; 
printf("destination port : %d \n", msg->payload.btp.btp_header.destination_port) ;  */

    	close(sock);
	
	//send_socket(msg); 
	
	
	return msg ; 
	
	
	
}

}


void btp_data_indication(btp_rx_params_t params) {


e_btp_port src_port, dest_port ; 





}













