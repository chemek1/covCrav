/***************************************************************************
 *   ITSNET  Intelligent Transport System networking Stack                 *
 * 									     *
 ** Copyright(C)2010 ESPRIT							
 * 	        "École supérieure privée d'ingénierie et de technologie"   
 *                                                                         
 *   barghich@gmail.com                                                    *
 *   anouar.chemek@gmail.com                                               *
 *  							                     *
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
 * @file itsnet.c
 * itsnet code.
 * @author hichem BARGAOUI
 * @author anouar Chemek
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <asm/types.h>
#include <errno.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#include "beaconing.h"
#include "geo_broadcast.h" 
#include "dll_sap.h"
#include "itsnet.h"
#include "itsnet_listener.h"
#include "itsnet_parser.h"
#include "position_sensor.h"
#include "routing_manager.h"
#include "tqueue.h"
//#include "itsnet_ip.h"
#include "itsnet_pseudonym.h"
#include "location_table.h"
#include "position_sensor.h"

LIST_HEAD(neighbor_list);

/**
 * clean memory
 * @return an integer
 */

int itsnet_cleanup()
{
}

/**
 * main function
 * @param argc
 * @param argv
 */
int itsnet_main(int argc, char** argv)
{

    if(taskqueue_init() < 0) {
        printf("taskqueue_init failed\n");
        exit(1);
    }

    if(dll_sap_init(NULL) < 0) {
        printf("dll_sap__init failed\n");
        exit(1);
    }

    if(itsnet_listener_init() < 0) {

        printf("position_sensor_init failed\n");
        exit(1);
    }

    /*	if (itsnet_iplistener_init() < 0)
                {
                        printf("itsnet_iplistener_init failed\n");

                        exit(1);
                }
    */

    itsnet_init_conf(); 

    return 0;
}
/**
 *
 */

void itsnet_init_conf()
{
    set_ego_node_id(NodeId_parse());
    
    //itsnet_beacon_send();
    
    
    // préparer le packet 
    /*struct itsnet_position_vector pos;
    static struct timeval t_emit;
    pos = get_position_vector() ;
    int result ;  
    uint8_t brodcast_mac[MAX_LLA_LEN];
    brodcast_mac[0]=0xff;
    brodcast_mac[1]=0xff;
    brodcast_mac[2]=0xff;
    brodcast_mac[3]=0xff;
    brodcast_mac[4]=0xff;
    brodcast_mac[5]=0xff;
    //memcpy((uint8_t*)brodcast_mac, (uint8_t*)BroadcastMac_parse()->address, MAX_LLA_LEN);
 
    struct itsnet_packet *p ;
    p = (itsnet_packet*)malloc(sizeof(itsnet_packet));
    
    
     if(p == NULL) {
        printf("erreur allocation \n");
        
     }
     else {
     
     	memset(p, 0, sizeof(itsnet_packet));
      	
        p->basic_header.itsnet_version_next_header = HI_NIBBLE(1);
        p->basic_header.itsnet_version_next_header |= LO_NIBBLE(common_header);
        p->basic_header.itsnet_lifetime=lt_base_10s;
        p->basic_header.itsnet_rhl=10;
        
        p->common_header.itsnet_header_type_subtype = HI_NIBBLE(itsnet_geobroadcast_id);
        p->common_header.itsnet_header_type_subtype |= LO_NIBBLE(0);
        p->common_header.traffic_class = CLASS01;
        p->common_header.flags = 0;
        p->common_header.flags = Mobile << 7;
        //p->common_header.payload_lenght = htons(1514);
        //p->common_header.payload_lenght = htons(sizeof(*p));
        p->common_header.payload_lenght = 0;
        p->common_header.max_hop_limit = 255;
        p->common_header.reserved=0;
        p->common_header.itsnet_next_header = HI_NIBBLE(BTP_B);
        p->common_header.itsnet_next_header |= LO_NIBBLE(0);
        
        p->payload.itsnet_geobroadcast.forwarder_position_vector = pos;
        p->payload.itsnet_geobroadcast.forwarder_position_vector.node_id = *(get_ego_node_id()) ;
        
        result = itsnet_geobroadcast_send(p) ; 
        
        
     } */ 
     
       
}
