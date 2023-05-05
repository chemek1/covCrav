/***************************************************************************
 *            covcrav_denm.c
 *
 *  mer. janvier 27 12:44:11 2021
 *  Copyright  2021  Anouar Chemek
 *  <user@host>
 ****************************************************************************/
/*
 * covcrav_denm.c
 *
 * Copyright (C) 2021 - Anouar Chemek
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
 
#include <time.h> 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <sys/timex.h>
#include <stdint.h>
#include <unistd.h>
#include <netinet/in.h>
//#include <ntp.h>
#include "facility.h"







int ID_EMIT = 0;
int count = 0 ;


itsnet_position_vector pos_vector ; 




DENM_t* encode_denm(application_request_type_id request_id, transmission_params_t* app_params) 
{	
	 
	uint16_t period = 1000;
	itsnet_position_vector pos_vector; 

	pos_vector.node_id.id[0] = 0 ; 
	pos_vector.node_id.id[1] = 0 ; 
	pos_vector.node_id.id[2] = 0 ; 
	pos_vector.node_id.id[3] = 0 ; 
	pos_vector.node_id.id[4] = 0 ; 
	pos_vector.node_id.id[5] = 0 ; 
	pos_vector.node_id.id[6] = 0 ; 
	pos_vector.node_id.id[7] = 0 ; 

	pos_vector.time_stamp = 10 ; 
	pos_vector.latitude = 1 ; 
	pos_vector.longitude = 2 ;
	pos_vector.speed = 3 ; 
	pos_vector.heading = 4 ; 
	
	/* set the referenceTime (time at which a DENM is generated) */ 
	
	ntp_gettime(&referenceTime) ;
	
	
	static struct ntptimeval probe_emit;
	memset(&probe_emit, 0, sizeof(probe_emit)) ; 
	if ( ntp_gettime(&probe_emit) == -1 ) {
		printf("erreur lors de l'appel de ntp_gettime avec probe_emit \n") ; 
		exit(EXIT_FAILURE);
	}
	
	denm_t* payload = (denm_t*)malloc(sizeof(denm_t))  ;
	DENM_t* DENM = (DENM_t*)malloc(sizeof(DENM_t)) ; 
	its_pdu_header* its_app_header = (its_pdu_header*)malloc(sizeof(its_pdu_header)) ;
	
	 
	
	switch(request_id) {
		case AppDENM_trigger_id :
			/* generate new sequence number (new action id ) ; */  
			ID_EMIT++ ; 
			
			/* management container encoding */ 
			payload->management.action_id.originating_station_id = pos_vector.node_id;
    			payload->management.action_id.sequence_number = ID_EMIT;
    			payload->management.station_type = station_type_passengerCar;
			payload->management.detection_time = timeval_to_ms(&app_params->params.trigger_params.detection_time);
			payload->management.reference_time = timeval_to_ms(&referenceTime); 
			payload->management.event_position = pos_vector;
			payload->management.relevance_distance = app_params->params.trigger_params.relevance_area.relevance_distance ;
			payload->management.relevance_traffic_direction = app_params->params.trigger_params.relevance_area.relevance_traffic_direction ; 
			// payload->management.termination=1; // For the application request type AppDENM_trigger, the termination DE shall not be included
			payload->management.validity_duration = app_params->params.trigger_params.validity_duration ; 
			
			if (app_params->params.trigger_params.transmission_interval != 0) {
			
				payload->management.transmission_interval = app_params->params.trigger_params.transmission_interval ; 
			}
			
			/* situation container encoding */ 
			payload->situation = app_params->params.trigger_params.situation_informations ;
			
			/* location container encoding */  
			payload->location = app_params->params.trigger_params.location_informations ; 
			
			/* alacarte container encoding  */ 
			payload->alacarte = app_params->params.trigger_params.alacarte_informations ; 
			
			/* construct pdu header */ 
			its_app_header->message_id = message_id_denm;
    			its_app_header->protocol_version = protocol_version_current_version;
    			its_app_header->station_id = pos_vector.node_id;
    			
    			/*construct DENM */ 
    			memcpy((void*)&DENM->header, (void*)its_app_header, sizeof(its_pdu_header_t));
    			memcpy((void*)&DENM->payload, (void*)payload, sizeof(denm_t));
   			break ; 
   			 			
   		case AppDENM_update_id : 
   			/*management container encoding */ 
   			payload->management.action_id = app_params->params.update_params.action_id ; 
    			payload->management.station_type = station_type_passengerCar;
			payload->management.detection_time = timeval_to_ms(&app_params->params.update_params.detection_time);
			payload->management.reference_time = timeval_to_ms(&referenceTime); 
			payload->management.event_position = pos_vector;
			payload->management.relevance_distance = app_params->params.update_params.relevance_area.relevance_distance ;
			payload->management.relevance_traffic_direction = app_params->params.update_params.relevance_area.relevance_traffic_direction ; 
			// payload->management.termination=1; // For the application request type AppDENM_update, the termination DE shall not be included
			payload->management.validity_duration = app_params->params.update_params.validity_duration ; 
			if (app_params->params.update_params.transmission_interval != 0) {
						
				payload->management.transmission_interval = app_params->params.update_params.transmission_interval ; 
			} 
			
			/* situation container encoding */ 
			payload->situation = app_params->params.update_params.situation_informations ;
			
			/* location container encoding */  
			payload->location = app_params->params.update_params.location_informations ; 
			
			/* alacarte container encoding  */ 
			payload->alacarte = app_params->params.update_params.alacarte_informations ; 
			
			/* construct pdu header */ 
			its_app_header->message_id = message_id_denm;
    			its_app_header->protocol_version = protocol_version_current_version;
    			its_app_header->station_id = pos_vector.node_id;
    			
    			/*construct DENM */ 
    			memcpy((void*)&DENM->header, (void*)its_app_header, sizeof(its_pdu_header_t));
    			memcpy((void*)&DENM->payload, (void*)payload, sizeof(denm_t));
   			break ; 
			
		case AppDENM_termination_id : 
			/*management container encoding */ 
   			payload->management.action_id = app_params->params.termination_params.action_id ; 
    			payload->management.station_type = station_type_passengerCar;
			payload->management.detection_time = timeval_to_ms(&app_params->params.termination_params.detection_time);
			payload->management.reference_time = timeval_to_ms(&referenceTime); 
			payload->management.event_position = pos_vector;
			payload->management.relevance_distance = app_params->params.termination_params.relevance_area.relevance_distance ;
			payload->management.relevance_traffic_direction = app_params->params.termination_params.relevance_area.relevance_traffic_direction ; 
			payload->management.termination = 
			payload->management.validity_duration = app_params->params.update_params.validity_duration ; 
			payload->management.transmission_interval = app_params->params.update_params.transmission_interval ;
			
			/* situation container encoding */ 
			payload->situation = app_params->params.update_params.situation_informations ;
			
			/* location container encoding */  
			payload->location = app_params->params.update_params.location_informations ; 
			
			/* alacarte container encoding  */ 
			payload->alacarte = app_params->params.update_params.alacarte_informations ; 
			
			/* construct pdu header */ 
			its_app_header->message_id = message_id_denm;
    			its_app_header->protocol_version = protocol_version_current_version;
    			its_app_header->station_id = pos_vector.node_id;
    			
    			/*construct DENM */ 
    			memcpy((void*)&DENM->header, (void*)its_app_header, sizeof(its_pdu_header_t));
    			memcpy((void*)&DENM->payload, (void*)payload, sizeof(denm_t));
   			break ; 
   			
   		
   		return DENM ; 
			
   			
	
	
	
	
	
	
	
	
	
	
	}
	
	
	
	
	
	
	

	
    	
 
}






action_id_t* denm_transmission_management(int signal, application_request_type_id request_id, transmission_params_t* app_params) /* TODO: implement inside a separate management UNIT */
{
	
	typedef struct message message ;
	
	uint8_t station_id[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	
	pos_vector.node_id.id[0] = 0 ; 
	pos_vector.node_id.id[1] = 0 ; 
	pos_vector.node_id.id[2] = 0 ; 
	pos_vector.node_id.id[3] = 0 ; 
	pos_vector.node_id.id[4] = 0 ; 
	pos_vector.node_id.id[5] = 0 ; 
	pos_vector.node_id.id[6] = 0 ; 
	pos_vector.node_id.id[7] = 0 ; 

	pos_vector.time_stamp = 10 ; 
	pos_vector.latitude = 1 ; 
	pos_vector.longitude = 2 ;
	pos_vector.speed = 3 ; 
	pos_vector.heading = 4 ; 
	
	btp_tx_params_t tx_params ; 
	
	int T_O_validity; 
	
	int expiration_time ; 
	
	static struct ntptimeval current_time ; 
	
	static struct ntptimeval referenceTime ;  
	
	
	int T_Repetition ; 
	int T_RepetitionDuration ; 
	
	action_id_t id ; 
	transmission_interval_t tr_interval ; 
	
	denm_t* new_denm = (denm_t*)malloc(sizeof(denm_t));
	memset(new_denm, 0, sizeof(denm_t)) ;
	
	DENM_t* new_DENM = (DENM_t*)malloc(sizeof(DENM_t)) ; 
	memset(new_DENM, 0, sizeof(DENM_t)) ; 
	
	memset(&current_time, 0, sizeof(current_time)) ; 
	
	memset(&referenceTime, 0, sizeof(referenceTime)) ;  
	
	
	
	 

	 
	
	
switch(request_id) {
	case AppDENM_trigger_id :
	
		/* calculate expiration time for T_O_validity */ 
		if (app_params->params.trigger_params.validity_duration != 0) {
			expiration_time = timeval_to_ms(&app_params->params.trigger_params.detection_time) + app_params->params.trigger_params.validity_duration ; 
		} 
		else {
			expiration_time = timeval_to_ms(&app_params->params.trigger_params.detection_time) + 600000 ; 
		}
		
		printf("expiration time = %d \n", expiration_time) ; 
		
		ntp_gettime(&current_time); 
		
		if ( timeval_to_ms(&current_time) > expiration_time ) {
			printf(" detection time is in the past ") ; 
			exit(1) ; 
		} 
		/* construct DENM */ 
		
		new_DENM = encode_denm(request_id, app_params) ; 
		
		/* create new entry in the denm transmission messages table */ 
		
		
		denm_tx_messages->count++; 
		memcpy(denm_tx_message->denm_messages[denm_tx_messages->count]->node_id, (void*)&pos_vector.node_id, sizeof(pos_vector.node_id)) ; 
		memcpy(denm_tx_messages.denm_messages[denm_tx_messages->count]->sequence_number, (void*)&ID_EMIT, sizeof(ID_EMIT)) ; 
		denm_tx_messages.denm_messages[denm_tx_messages->count]->state = ACTIVE ; 
		
	
		printf("new entery is created \n") ; 
		
		/* start T_O_validity timer */ 
		if (app_params->params.trigger_params.repetition_duration > 0 && app_params->params.trigger_params.repetition_interval > 0) {
			T_RepetitionDuration = timeval_to_ms(&app_params->params.trigger_params.detection_time) + app_params->params.trigger_params.repetition_duration ; 
			T_Repetition = app_params->params.trigger_params.repetition_interval ; 
		}
		 
		printf("T_RepetitionDuration = %d \n", T_RepetitionDuration) ; 
		printf("T_Repetition = %d \n", T_Repetition) ; 
		
		/* send action id to the its-s application requesting */ 
		T_O_validity = expiration_time ;
		printf ("T_O_validity = %d \n", T_O_validity) ;
		ntp_gettime(&referenceTime) ;  
		while ( timeval_to_ms(&referenceTime) < T_O_validity) {
			while ( timeval_to_ms(&referenceTime) < T_RepetitionDuration ) {
				if ( T_Repetition == 0 ) { 
					/* send to btp */ 
					
					tx_params.btp_type = 0 ;
					tx_params.source_port = btp_port_DENM ;  
					tx_params.destination_port =  btp_port_DENM ; 
					tx_params.packet_transport_type = 0 ; 
					memcpy(tx_params.destination_address, station_id, sizeof(station_id)) ;  
					tx_params.traffic_class = 3 ; 
					tx_params.data = *new_DENM ;
					
					message* msg ; 
					msg = (struct message*)malloc(sizeof(struct message)) ; 

					memset(msg, 0, sizeof(struct message)) ; 

					msg = btp_data_request(tx_params) ;
					
					
					printf("packet sent to btp \n") ; 
					T_Repetition = app_params->params.trigger_params.repetition_interval ; 
				} else {
					T_Repetition-= 1000 ; 
				}
				T_RepetitionDuration-= 1000 ; 
			}
			ntp_gettime(&referenceTime) ; 
			sleep(1) ;  
		}
		break ; 
		
	case AppDENM_update:
		
		/* calculate expiration time for T_O_validity */ 
		if (app_params->params.update_params.validity_duration != 0) {
			expiration_time = timeval_to_ms(&app_params->params.update_params.detection_time) + app_params->params.update_params.validity_duration ; 
		} 
		else {
			expiration_time = timeval_to_ms(&app_params->params.update_params.detection_time) + 600000 ; 
		}
		
		printf("expiration time = %d \n", expiration_time) ; 
		
		ntp_gettime(&current_time); 
		
		if ( timeval_to_ms(&current_time) > expiration_time ) {
			printf(" detection time is in the past ") ; 
			exit(1) ; 
		} 
		
		printf ("no error in ntp_gettime for current_time \n") ;
		 
		/* compare action_id with other entries in messages table */ 
		int cpt = 0 ; 
		while(denm_tx_messages->denm_messages[cpt]!= NULL && denm_tx_messages->denm_messages[cpt]->node_id != app_params->params.update_params.action_id.node_id && denm_tx_messages.denm_messages[cpt]->sequence_number != app_params->params.updtae_params.sequence_number) {
			cpt++ ; 
		}
		
		
		if (denm_tx_messages->denm_messages[cpt]->node_id == app_params->params.update_params.action_id.node_id && denm_tx_messages->denm_messages[cpt]->sequence_number == app_params->params.update_params.action_id.sequence_number) {
		
		
			/* construct DENM */ 
			new_DENM = encode_denm(request_id, app_params) ; 
		
			// update entry in table messages 
			
			
			/* start T_O_validity timer */ 
		if (app_params->params.trigger_params.repetition_duration > 0 && app_params->params.trigger_params.repetition_interval > 0) {
			T_RepetitionDuration = timeval_to_ms(&app_params->params.trigger_params.detection_time) + app_params->params.trigger_params.repetition_duration ; 
			T_Repetition = app_params->params.trigger_params.repetition_interval ; 
		}
		
		 
		printf("T_RepetitionDuration = %d \n", T_RepetitionDuration) ; 
		printf("T_Repetition = %d \n", T_Repetition) ; 
		
		/* send action id to the its-s application requesting */ 
		T_O_validity = expiration_time ;
		printf ("T_O_validity = %d \n", T_O_validity) ;
		ntp_gettime(&referenceTime) ;  
		while ( timeval_to_ms(&referenceTime) < T_O_validity) {
			while ( timeval_to_ms(&referenceTime) < T_RepetitionDuration ) {
				if ( T_Repetition == 0 ) { 
					/* send to btp */ 
					
					tx_params.btp_type = 0 ;
					tx_params.source_port = btp_port_DENM ; 
					tx_params.destination_port = btp_port_DENM ; 
					tx_params.packet_transport_type = 0 ; 
					memcpy(tx_params.destination_address, station_id, sizeof(station_id)) ;  
					tx_params.traffic_class = 3 ; 
					tx_params.data = *new_DENM ;
					
					message* msg ; 
					msg = (struct message*)malloc(sizeof(struct message)) ; 

					memset(msg, 0, sizeof(struct message)) ; 

					msg = btp_data_request(tx_params) ;
					
					
					
					printf("packet sent to btp \n") ; 
					T_Repetition = app_params->params.trigger_params.repetition_interval ; 
				} else {
					T_Repetition-= 1000 ; 
				}
				T_RepetitionDuration-= 1000 ; 
			}
			ntp_gettime(&referenceTime) ; 
			sleep(1) ;  
		}
		break ;
			
		}
		
	
		printf(" \n ") ;  
		break ; 
		
		
		
		
		
	case AppDENM_termination_id : 
		/* calculate the expiratio time of T_O_validity */
		
		if (app_params->params.termination_params.validity_duration != 0) {
			expiration_time = timeval_to_ms(&app_params->params.termination_params.detection_time) + app_params->params.termination_params.validity_duration ; 
		} 
		else {
			expiration_time = timeval_to_ms(&app_params->params.termination_params.detection_time) + 600000 ; 
		}
		
		printf("expiration time = %d \n", expiration_time) ; 
	
		ntp_gettime(&current_time); 
		
		if ( timeval_to_ms(&current_time) > expiration_time ) {
			printf(" detection time is in the past ") ; 
			exit(1) ; 
		} 
		
		printf ("no error in ntp_gettime for current_time \n") ; 
			
		int cpt = 0 ; 
		while( (denm_tx_messages->denm_messages[cpt]!= NULL && denm_tx_messages->denm_messages[cpt]->node_id != app_params->params.termination_params.action_id.node_id && denm_tx_messages.denm_messages[cpt]->sequence_number != app_params->params.termination_params.sequence_number) || ( denm_rx_messages->denm_messages[cpt]!=NULL && denm_rx_messages->denm_messages[cpt]->node_id != app_params->params_termination_params.action_id.node_id && denm_rx_messages[cpt]->sequence_number != app_params->params_termination_params.action_id.sequence_numbe)) {
			cpt++ ; 
		}
		
		if(denm_tx_messages->denm_messages[cpt]->node_id == app_params->params.termination_params.action_id.node_id && denm_tx_messages->denm_messages[cpt]->sequence_number == app_params->params.termination_params.action_id.sequence_number && denm_tx_messages->denm_messages[cpt]->state == ACTIVE) {
			denm_tx_messages->denm_messages[cpt]->state = isCancellation ; 
	
		}
		
		else if (denm_rx_messages->denm_messages[cpt]->node_id == app_params->params.termination_params.action_id.node_id && denm_rx_messages->denm_messages[cpt]->sequence_number == app_params->params.termination_params.action_id.sequence_number && denm_rx_messages->denm_messages[cpt]->state == ACTIVE ) {
		
			denm_rx_messages->denm_messages[cpt]->state = isNegation ; 
		
		}  
		else 
			exit(1) ;
			
			
		/* set referenceTime */
		
		if (app_params->params.termination_params 
			
		/*construct DENM */
		
		new_DENM = encode_denm(request_id, app_params) ; 
	
	
	
	
	
}
	
	// return new_DENM ; 
	
}



/* interface with management entity 5.4.3 */ 

/* interface with security entity 5.4.4 */ 



denm_t* denm_reception_management(DENM_t* packet) {

	int T_R_validity // indicates the end of DENM validity ; 
	
	int expiration_time ;
	
	denm_t* payload; 
	payload = (denm_t*)malloc(sizeof(denm_t)) ; 
	memset(payload,0, sizeof(denm_t)) ; 
	
	payload = decode_denm() 
	
	/* calculate expiration_time for T_R_validity */  
	
	if (app_params->params.trigger_params.validity_duration != 0) {
			expiration_time = timeval_to_ms(&app_params->params.trigger_params.detection_time) + app_params->params.trigger_params.validity_duration ; 
		} 
	else {
			expiration_time = timeval_to_ms(&app_params->params.trigger_params.detection_time) + 600000 ; 
		}
	if ( timeval_to_ms(&current_time) > expiration_time ) {
			printf(" detection time is in the past ") ; 
			exit(1) ; 
		} 
	
	
	/* lookup entries in the receiving messages table with the received action id  */
	
	
	int count = 0 // à déplacer dans une structure qui contient le tableau ; 
	while(received_messages!=NULL && received_messages->action_id != payload->management.action_id) {
	
		received_messages++ ; 
	} 
	
	/* if entry doesn't exist in the table */ 
	
	if ( received_messages->action_id != payload->management.action_id) {
		if( payload->management.termination != 0) 
		//discard the denm
			exit(1) ; 
		
		
		/* create an entry in the table */ 
		received_messages++ ; 
		received_messages->action_id = payload->management.action_id ; 
		received_messages->reference_time = payload->management.reference_time ; 
		received_messages->termination = payload->management.termination ; 
		received_messages->detection_time = payload->management.detection_time ; 
		
		// set the state to ACTIVE 

	}
	
	
	
	else {
	
		/* check if the received reference time is less than the entry reference time or the received detection time is less than the entry detection time */ 
		if (payload->management.reference_time < received_messages->reference_time || payload->management.detection_time == received_messages->detection_time ) 
			//discard denm
			exit(1) ; 
		/* update the entry */
		received_messages->action_id = payload->management.action_id ; 
		received_messages->reference_time = payload->management.reference_time ; 
		received_messages->termination = payload->management.termination ; 
		received_messages->detection_time = payload->management.detection_time ; 
		
		// set entry state according to the termination value 
		
	
		
	
	}
	
	
	/* start T_R_validity */ 
	
	T_R_validity = expiration_time ;
	
	ntp_gettime(&currentTime) ; 
	
		while (timeval_to_ms(&currentTime) < T_R_validity) {
		
			T_R_validity-= 1000 ; 
		
		} 
		
	// delete entry 
	
	
	return payload ; 
	
	
	
	



}









