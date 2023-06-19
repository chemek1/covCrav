 
#include <time.h> 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <sys/timex.h>
#include <stdint.h>
#include <unistd.h>
#include <netinet/in.h>
//#include <libntp.h>
#include "facilities.h"
#include "itsnet_btp.h"
#include "itsnet_types.h"  


itsnet_position_vector pos_vector ; 


uint8_t ID_EMIT = 0;
uint8_t count = 0 ;

transmitted_message_t messages;

transmitted_message_t* const denm_messages = &messages;

received_message_t rx_messages ; 

received_message_t* const denm_rx_messages = &rx_messages ; 



int compare_node_id(struct itsnet_node_id node_id1, struct itsnet_node_id node_id2) {

	for (int i = 0; i < NODE_ID_LEN; i++) {
		//printf("node_id1.id[%d] = %d et node_id2.id[%d] = %d \n", i, node_id1.id[i], i, node_id2.id[i]) ;  
       	 if (node_id1.id[i] != node_id2.id[i]) {
       	 	return 0 ; //structures are not equal 
       	 }
            	
    }
    return 1 ; // Structures are equal
}


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
	
	static struct timeval referenceTime ;
	
	ntp_gettime(&referenceTime) ;
	
	
	static struct timeval probe_emit;
	memset(&probe_emit, 0, sizeof(probe_emit)) ; 
	if ( ntp_gettime(&probe_emit) == -1 ) {
		printf("erreur lors de l'appel de ntp_gettime avec probe_emit \n") ; 
		exit(EXIT_FAILURE);
	}
	
	denm_t* payload = (denm_t*)malloc(sizeof(denm_t))  ;
	DENM_t* DENM = (DENM_t*)malloc(sizeof(DENM_t)) ; 
	its_pdu_header_t* its_app_header = (its_pdu_header_t*)malloc(sizeof(its_pdu_header_t)) ;
	
	 
	
	switch(request_id) {
		case AppDENM_trigger_id :
			/* generate new sequence number (new action id ) ; */  
			//ID_EMIT++ ; 
			
			/* management container encoding */ 
			payload->management.action_id.node_id = pos_vector.node_id;
    			payload->management.action_id.sequence_number = ID_EMIT;
    			payload->management.station_type = station_type_passengerCar;
			payload->management.detection_time = timeval_to_ms(app_params->params.trigger_params.detection_time);
			payload->management.reference_time = timeval_to_ms(referenceTime); 
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
			//payload->alacarte = app_params->params.trigger_params.alacarte_informations ; 
			
			/* construct pdu header */ 
			its_app_header->message_id = message_id_denm;
    			its_app_header->protocol_version = protocol_version_current_version;
    			its_app_header->station_id = pos_vector.node_id;
    			
    			
    			
    			/*construct DENM */ 
    			memcpy((void*)&DENM->header, (void*)its_app_header, sizeof(its_pdu_header_t));
    			memcpy((void*)&DENM->payload, (void*)payload, sizeof(denm_t));
    			
    			
			return DENM ; 
   			break ; 
   			 			
   		case AppDENM_update_id : 
   			/*management container encoding */ 
   			payload->management.action_id = app_params->params.update_params.action_id ; 
    			payload->management.station_type = station_type_passengerCar;
			payload->management.detection_time = timeval_to_ms(app_params->params.update_params.detection_time);
			payload->management.reference_time = timeval_to_ms(referenceTime); 
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
			//payload->alacarte = app_params->params.update_params.alacarte_informations ; 
			
			/* construct pdu header */ 
			its_app_header->message_id = message_id_denm;
    			its_app_header->protocol_version = protocol_version_current_version;
    			its_app_header->station_id = pos_vector.node_id;
    			
    			/*construct DENM */ 
    			memcpy((void*)&DENM->header, (void*)its_app_header, sizeof(its_pdu_header_t));
    			memcpy((void*)&DENM->payload, (void*)payload, sizeof(denm_t));
    			return DENM ;
   			break ; 
			
		case AppDENM_termination_id : 
			/*management container encoding */ 
   			payload->management.action_id = app_params->params.termination_params.action_id ; 
    			payload->management.station_type = station_type_passengerCar;
			payload->management.detection_time = timeval_to_ms(app_params->params.termination_params.detection_time);
			payload->management.reference_time = timeval_to_ms(referenceTime); 
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
			//payload->alacarte = app_params->params.update_params.alacarte_informations ; 
			
			/* construct pdu header */ 
			its_app_header->message_id = message_id_denm;
    			its_app_header->protocol_version = protocol_version_current_version;
    			its_app_header->station_id = pos_vector.node_id;
    			
    			/*construct DENM */ 
    			memcpy((void*)&DENM->header, (void*)its_app_header, sizeof(its_pdu_header_t));
    			memcpy((void*)&DENM->payload, (void*)payload, sizeof(denm_t));
    			return DENM ; 
   			break ; 
   		default : break ; 	
   		
   		//return DENM ; 
			
   			
	}
	
}


action_id_t* denm_trigger(int signal, application_request_type_id request_id, transmission_params_t* app_params) {

	typedef struct message message ;
	
	int cpt ;
	
	action_id_t* action_id = (action_id_t*)malloc(sizeof(action_id_t)) ; 
	
	if (action_id == NULL){
		printf("error in allocating action_id \n") ; 
		exit(1) ;
	}
	
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
	
	//denm_transmission_messages_t* denm_tx_messages ; 
	
	int T_O_validity; 
	
	int expiration_time ; 
	
	struct timeval current_time ; 
	
	struct timeval referenceTime ;  
	
	
	int T_Repetition ; 
	int T_RepetitionDuration ; 
	
	action_id_t id ; 
	transmission_interval_t tr_interval ; 
	
	denm_t* new_denm = (denm_t*)malloc(sizeof(denm_t));
	memset(new_denm, 0, sizeof(denm_t)) ;
	
	DENM_t* new_DENM = (DENM_t*)malloc(sizeof(DENM_t)) ; 
	memset(new_DENM, 0, sizeof(DENM_t)) ; 
	
	//memset(&current_time, 0, sizeof(current_time)) ; 
	
	//memset(&referenceTime, 0, sizeof(referenceTime)) ; 
	
	ntp_gettime(&current_time) ; 
	printf("current_time : %d \n", timeval_to_ms(current_time)) ;  
	
	/* calculate expiration time for T_O_validity */ 
	if (app_params->params.trigger_params.validity_duration != 0) {
		printf("detection time = %d \n", timeval_to_ms(app_params->params.trigger_params.detection_time)) ; 
		printf("validity duration : %d \n", app_params->params.trigger_params.validity_duration) ; 
			
		//expiration_time = 20000 ;
		expiration_time = timeval_to_ms(app_params->params.trigger_params.detection_time) + app_params->params.trigger_params.validity_duration ;     
	} 
	else {
		expiration_time = timeval_to_ms(app_params->params.trigger_params.detection_time) + 600000 ; 
	}
		
	printf("expiration time = %d \n", expiration_time) ;	
	printf("difference between current time and expiration time %d \n", timeval_to_ms(current_time) - expiration_time) ;	
	int n = timeval_to_ms(current_time) - expiration_time ; 	
	if (n > 0) {
		printf("difference between current time and expiration time %d \n", timeval_to_ms(current_time) - expiration_time) ;
		printf("detection time is in the past \n ") ; 
		exit(1) ; 
	} 
	/* construct DENM */ 
		
	new_DENM = encode_denm(request_id, app_params) ; 
	
	printf("DENM is created \n") ; 
	
	if (denm_messages == NULL) {
		printf("error in allocating denm_messages (denm_trigger.c) \n"); 
		exit(1) ; 
	}
	
	//printf("denm_messages is well allocated with the address : %p \n", (void*)&denm_messages) ; 
	
	for(int i = 0; i<8; i++) {
		denm_messages[count].node_id.id[i] = app_params->params.trigger_params.event_position.node_id.id[i] ;
		action_id->node_id.id[i] = app_params->params.trigger_params.event_position.node_id.id[i] ;   				
	}
	
	action_id->sequence_number = ID_EMIT ; 
	denm_messages[count].sequence_number = ID_EMIT ;
	denm_messages[count].state = isACTIVE ;
	
	printf("new entery is created \n") ;
	
	count++ ; 
	ID_EMIT++ ;
	
	
	
	/*printf("new entry after creation : \n") ; 
	printf("node_id.id[0] = : %d \n", denm_messages[count].node_id.id[0]); 
	printf("sequence number : %d \n", denm_messages[count].sequence_number) ; 
	printf("state : %d \n", denm_messages[count].state = isACTIVE) ; 
	
	printf("repetition duration received from app params %d\n", app_params->params.trigger_params.repetition_duration);
	
	printf("repetition interval received from app params %d\n", app_params->params.trigger_params.repetition_interval); */
	
	 	
	 /* start T_O_validity timer */ 
	if (app_params->params.trigger_params.repetition_duration > 0 && app_params->params.trigger_params.repetition_interval > 0) {
		T_RepetitionDuration = timeval_to_ms(app_params->params.trigger_params.detection_time) + app_params->params.trigger_params.repetition_duration ; 
		T_Repetition = app_params->params.trigger_params.repetition_interval ; 
	}
		 
	printf("T_RepetitionDuration = %d \n", T_RepetitionDuration) ; 
		
	printf("T_Repetition = %d \n", T_Repetition) ; 
		
	/*send action id to the its-s application requesting */ 
	T_O_validity = expiration_time ;
	printf ("T_O_validity = %d \n", T_O_validity) ;
	
	
	ntp_gettime(&referenceTime) ;
	
	printf("reference time : %d\n", timeval_to_ms(referenceTime)) ;
	
	int m = timeval_to_ms(referenceTime) - T_O_validity ;
	
	int d = timeval_to_ms(referenceTime) - T_RepetitionDuration ;
	
	
	while ( d < 0 ) {
		if ( m < 0) // DENM still valid 
		{
			if (T_Repetition == 0) {
				
				//printf("T_Repetition = %d \n", T_Repetition) ; 	
			
				/* send to btp */ 
					
				tx_params.btp_type = 0 ;
				tx_params.source_port = btp_port_DENM ;  
				tx_params.destination_port =  btp_port_DENM ; 
				tx_params.packet_transport_type = 0 ; 
				memcpy(tx_params.destination_address, station_id, sizeof(station_id)) ;  
				tx_params.traffic_class = 3 ; 
				tx_params.data = *new_DENM ;
				
				printf("tx_params_data done \n") ; 
					
				message* msg ; 
				msg = (struct message*)malloc(sizeof(struct message)) ; 
				if(msg == NULL) {
					printf("error allocating msg \n") ; 
				
				}

				memset(msg, 0, sizeof(struct message)) ; 

				msg = btp_data_request(tx_params) ;
				
				printf("=============== DENM in app ================= \n"); 
				printf("=============== ITS PDU HEADER ====================== \n") ; 
				printf("protocol version = %d | message id = %d | station id = ", msg->payload.btp.payload.header.protocol_version, msg->payload.btp.payload.header.message_id); 
				for(int i = 0; i<8; i++) 
					printf("%d \t", msg->payload.btp.payload.header.station_id.id[i]) ; 
				printf("\n") ; 
				printf("============== PAYLOAD ============================== \n") ; 
				printf("------------ management container ------------------ \n") ; 
				printf("station type = %d | detection time = %d | reference time = %d | relevance distance = %d | relevance traffic direction = %d | validity duration = %d \n", msg->payload.btp.payload.payload.management.station_type, msg->payload.btp.payload.payload.management.detection_time, msg->payload.btp.payload.payload.management.reference_time, msg->payload.btp.payload.payload.management.relevance_distance, msg->payload.btp.payload.payload.management.relevance_traffic_direction, msg->payload.btp.payload.payload.management.validity_duration) ; 
				printf("===================================================== \n"); 
				
				
				
						
				printf("packet sent to btp \n") ;
					
				T_Repetition = app_params->params.trigger_params.repetition_interval ; 
				printf("T_Repetition = %d \n", T_Repetition) ; 
			
			
			}
			
			else {
				T_Repetition-=1000 ; 
				sleep(1) ;
			}
			
		}
		
		else {
		
			printf("denm is not valid anymore \n") ; 
		}
		
	
	//printf("T_RepetitionDuration = %d \n", T_RepetitionDuration); 
	ntp_gettime(&referenceTime) ; 
	printf("referenceTime = %d \n", timeval_to_ms(referenceTime)) ;
	m = timeval_to_ms(referenceTime) - T_O_validity ;
	d = timeval_to_ms(referenceTime) - T_RepetitionDuration ;
		
}


return action_id ; 

}

action_id_t* denm_update(int signal, application_request_type_id request_id, transmission_params_t* app_params) {

typedef struct message message ;
	
	int cpt ;
	
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
	
	//denm_transmission_messages_t* denm_tx_messages ; 
	
	int T_O_validity; 
	
	int expiration_time ; 
	
	struct timeval current_time ; 
	
	struct timeval referenceTime ;  
	
	
	int T_Repetition ; 
	int T_RepetitionDuration ; 
	
	action_id_t id ; 
	transmission_interval_t tr_interval ; 
	
	denm_t* new_denm = (denm_t*)malloc(sizeof(denm_t));
	memset(new_denm, 0, sizeof(denm_t)) ;
	
	DENM_t* new_DENM = (DENM_t*)malloc(sizeof(DENM_t)) ; 
	memset(new_DENM, 0, sizeof(DENM_t)) ; 
	
	//memset(&current_time, 0, sizeof(current_time)) ; 
	
	//memset(&referenceTime, 0, sizeof(referenceTime)) ; 
	
	ntp_gettime(&current_time) ; 
	printf("current_time : %d \n", timeval_to_ms(current_time)) ;  
	
	


	/* calculate expiration time for T_O_validity */ 
	if (app_params->params.update_params.validity_duration != 0) {
		printf("detection time = %d \n", timeval_to_ms(app_params->params.update_params.detection_time)) ; 
		printf("validity duration : %d \n", app_params->params.update_params.validity_duration) ; 
			
		//expiration_time = 20000 ;
		expiration_time = timeval_to_ms(app_params->params.update_params.detection_time) + app_params->params.update_params.validity_duration ;     
	} 
	else {
		expiration_time = timeval_to_ms(app_params->params.update_params.detection_time) + 600000 ; 
	}
		
	printf("expiration time = %d \n", expiration_time) ;	
	printf("difference between current time and expiration time %d \n", timeval_to_ms(current_time) - expiration_time) ;	
	int n = timeval_to_ms(current_time) - expiration_time ; 	
	if (n > 0) {
		printf("difference between current time and expiration time %d \n", timeval_to_ms(current_time) - expiration_time) ;
		printf("detection time is in the past \n ") ; 
		exit(1) ; 
	} 
	
	
	if(denm_messages == NULL) {
		printf("denm_messages is not allocated in denm_update.c \n"); 
		exit(1); 
	}
	printf("denm_messages is allocated in denm_update.c with the address : %p \n", (void*)&denm_messages) ;
	
	
	cpt = 0 ; 
	
	printf("denm_messages[0].node_id.id[0] = %d \n",denm_messages[0].node_id.id[0] ) ; 
	printf("app_params->params.update_params.action_id.node_id.id[0] = %d \n", app_params->params.update_params.action_id.node_id.id[0]) ; 
	
	printf("sequence_number = %d \n", denm_messages[0].sequence_number); 
	printf("sequence number in app_params = %d \n", denm_messages[0].sequence_number);  
	
	/*printf("comparaison result for cpt = 0 : %d \n", compare_node_id(denm_messages[0].node_id, app_params->params.update_params.action_id.node_id)) ; 
	printf("denm_messages[0].sequence_number = %d \n", denm_messages[0].sequence_number ) ;
	printf("app_params->params.update_params.action_id.sequence_number = %d \n", app_params->params.update_params.action_id.sequence_number) ;*/ 
	
	bool test ; 
	
	test = compare_node_id(denm_messages[cpt].node_id, app_params->params.update_params.action_id.node_id)==0 && denm_messages[cpt].sequence_number != app_params->params.update_params.action_id.sequence_number ; 
	
	if(test ==  true) 
		printf("test is true \n") ; 
	else 
		printf("test is false \n") ; 
	
	
	while(compare_node_id(denm_messages[cpt].node_id, app_params->params.update_params.action_id.node_id)==0 && denm_messages[cpt].sequence_number != app_params->params.update_params.action_id.sequence_number) { 
	
		printf("cpt = %d \n") ; 
	
		cpt++ ; 
	
	}
	
	
	//int r = compare_node_id(denm_messages[cpt].node_id, app_params->params.update_params.action_id.node_id) ; 
	//printf("result of the function compare_node_id %d \n", r) ; 
	
	
	
	printf("cpt value = %d \n", cpt) ; 
	
	if (compare_node_id(denm_messages[cpt].node_id, app_params->params.update_params.action_id.node_id) == 1 && denm_messages[cpt].sequence_number == app_params->params.update_params.action_id.sequence_number) {
	
		printf("entry is found in denm_messages \n") ; 
		
		/* construct DENM */ 
		new_DENM = encode_denm(request_id, app_params) ; 
		
		printf("new DENM is created \n") ; 
			
		// update entry in table messages 
						
		/* start T_O_validity timer */ 
		if (app_params->params.update_params.repetition_duration > 0 && app_params->params.update_params.repetition_interval > 0) {
			T_RepetitionDuration = timeval_to_ms(app_params->params.update_params.detection_time) + app_params->params.update_params.repetition_duration ; 
			T_Repetition = app_params->params.update_params.repetition_interval ; 
		}
				
				 
		printf("T_RepetitionDuration = %d \n", T_RepetitionDuration) ; 
		printf("T_Repetition = %d \n", T_Repetition) ;
		
		/* send action id to the its-s application requesting */ 
		T_O_validity = expiration_time ;
		printf ("T_O_validity = %d \n", T_O_validity) ;
		ntp_gettime(&referenceTime) ; 
		
		printf("reference time : %d\n", timeval_to_ms(referenceTime)) ;
		
		int m = timeval_to_ms(referenceTime) - T_O_validity ;
	
		int d = timeval_to_ms(referenceTime) - T_RepetitionDuration ;
		
		while (d < 0) {
			
			if ( m < 0) {
			
				if (T_Repetition == 0) {
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

				//msg = btp_data_request(tx_params) ;
								
				printf("packet sent to btp \n") ; 
				
				T_Repetition = app_params->params.update_params.repetition_interval ; 
					
				
				}
				
				else {
				T_Repetition-=1000 ; 
				sleep(1) ;
			}
				
				
			
			
			
			
			
			
			
			}
			
			else {
				printf("denm is not valid anymore \n") ; 
			
			
			}
	ntp_gettime(&referenceTime) ; 
	printf("referenceTime = %d \n", timeval_to_ms(referenceTime)) ;
	m = timeval_to_ms(referenceTime) - T_O_validity ;
	d = timeval_to_ms(referenceTime) - T_RepetitionDuration ;
		
		
		}
		
	}
	
	else {
		printf("entry is not found \n") ;
	
	
	}
	
}



action_id_t* denm_termination(int signal, application_request_type_id request_id, transmission_params_t* app_params) {

int cpt ;
	
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
	
	//denm_transmission_messages_t* denm_tx_messages ; 
	
	int T_O_validity; 
	
	int expiration_time ; 
	
	struct timeval current_time ; 
	
	struct timeval referenceTime ;  
	
	
	int T_Repetition ; 
	int T_RepetitionDuration ; 
	
	action_id_t id ; 
	transmission_interval_t tr_interval ; 
	
	denm_t* new_denm = (denm_t*)malloc(sizeof(denm_t));
	memset(new_denm, 0, sizeof(denm_t)) ;
	
	DENM_t* new_DENM = (DENM_t*)malloc(sizeof(DENM_t)) ; 
	memset(new_DENM, 0, sizeof(DENM_t)) ; 
	
	//memset(&current_time, 0, sizeof(current_time)) ; 
	
	//memset(&referenceTime, 0, sizeof(referenceTime)) ; 
	
	ntp_gettime(&current_time) ; 
	printf("current_time : %d \n", timeval_to_ms(current_time)) ;  
	
	
	
	/* calculate expiration time for T_O_validity */ 



	if (app_params->params.termination_params.validity_duration != 0) {
		printf("detection time = %d \n", timeval_to_ms(app_params->params.termination_params.detection_time)) ; 
		printf("validity duration : %d \n", app_params->params.termination_params.validity_duration) ; 
			
		//expiration_time = 20000 ;
		expiration_time = timeval_to_ms(app_params->params.termination_params.detection_time) + app_params->params.termination_params.validity_duration ;     
	} 
	else {
		expiration_time = timeval_to_ms(app_params->params.termination_params.detection_time) + 600000 ; 
	}
		
	printf("expiration time = %d \n", expiration_time) ;	
	printf("difference between current time and expiration time %d \n", timeval_to_ms(current_time) - expiration_time) ;	
	int n = timeval_to_ms(current_time) - expiration_time ; 	
	if (n > 0) {
		printf("difference between current time and expiration time %d \n", timeval_to_ms(current_time) - expiration_time) ;
		printf("detection time is in the past \n ") ; 
		exit(1) ; 
	} 
	
	if(denm_rx_messages == NULL) {
		printf("denm_rx_messages is not allocated in denm_update.c \n"); 
		exit(1); 
	}
	printf("denm_rx_messages is allocated in denm_termination.c with the address : %p \n", (void*)&denm_rx_messages) ;
	
	
	cpt = 0 ;
	
	printf("denm_messages[1].node_id[0] = %d \n", denm_messages[1].node_id.id[0]) ; 
	printf("denm_messages[1].sequence_number = %d \n", denm_messages[1].sequence_number) ;
	
	printf("comparaison result for denm_messages : %d \n", compare_node_id(denm_messages[1].node_id, app_params->params.termination_params.action_id.node_id));
	
	printf("denm_rx_messages[1].action_id.node_id[0] = %d \n", denm_rx_messages[1].action_id.node_id.id[0]) ;
	printf("denm_rx_messages[1].action_id.sequence_number = %d \n", denm_rx_messages[1].action_id.sequence_number) ; 
	
	printf("comparaison result for denm_rx_messages : %d \n", compare_node_id(denm_rx_messages[1].action_id.node_id, app_params->params.termination_params.action_id.node_id)) ; 
	
	
	
	bool result, result1, result2 ; 
	
	result1 = (compare_node_id(denm_messages[1].node_id, app_params->params.termination_params.action_id.node_id)==1 && denm_messages[1].sequence_number == app_params->params.termination_params.action_id.sequence_number) ;
	
	result2 = (compare_node_id(denm_rx_messages[1].action_id.node_id, app_params->params.termination_params.action_id.node_id) ==1  && denm_rx_messages[1].action_id.sequence_number == app_params->params.termination_params.action_id.sequence_number) ;
	
	
	if (result1 == true) 
		printf("result1 is true \n") ; 
	else 
		printf("result1 is false \n") ; 
		
	if (result2 == true) 
		printf("result2 is true \n") ; 
	else 
		printf("result2 is false \n") ; 
	 
	
	result = !result1 && !result2 ; 
	

	if (result == true) 
		printf("result is true \n") ; 
	else 
		printf("result is false \n") ; 
		
		
	while (!(compare_node_id(denm_messages[cpt].node_id, app_params->params.termination_params.action_id.node_id)==1 && denm_messages[cpt].sequence_number == app_params->params.termination_params.action_id.sequence_number) && !(compare_node_id(denm_rx_messages[cpt].action_id.node_id, app_params->params.termination_params.action_id.node_id) ==1  && denm_rx_messages[cpt].action_id.sequence_number == app_params->params.termination_params.action_id.sequence_number)) {
		
		printf("cpt in while : %d \n", cpt) ; 
		cpt++ ;
	
	
	}
	
	printf("cpt value after while : %d \n", cpt) ; 
	
	
	if(compare_node_id(denm_messages[cpt].node_id, app_params->params.termination_params.action_id.node_id) == 1 || denm_messages[cpt].sequence_number == app_params->params.termination_params.action_id.sequence_number && denm_messages[cpt].state == isACTIVE) {
			denm_messages[cpt].state = isCancellation ; 
			printf("denm_messages[%d]->state is set to %d \n", cpt, isCancellation) ;
			
			
	}
		 
	else if (compare_node_id(denm_rx_messages[cpt].action_id.node_id, app_params->params.termination_params.action_id.node_id) == 1 || denm_rx_messages[cpt].action_id.sequence_number == app_params->params.termination_params.action_id.sequence_number && denm_rx_messages[cpt].state == ACTIVE ) {
		
			denm_rx_messages[cpt].state = NEGATED ; 
			printf("denm_rx_messages[%d]->state is set to %d \n", cpt, NEGATED); 
		
		} 
	else 
		exit(1) ;
		
	new_DENM = encode_denm(request_id, app_params) ; 
	
	printf("new DENM is created \n") ; 


}








action_id_t* denm_transmission_management(int signal, application_request_type_id request_id, transmission_params_t* app_params) {

action_id_t* act_id = (action_id_t*)malloc(sizeof(action_id_t)) ; 

switch(request_id) {

	case AppDENM_trigger_id : 
		return denm_trigger(signal,request_id, app_params) ;
		//act_id = denm_trigger(signal,request_id, app_params) ; 
		//printf("sequence number in transmission_management = %d \n", act_id->sequence_number) ; 	
		break ; 
	case AppDENM_update_id:
		denm_update(signal, request_id, app_params) ;
		break ; 
	case AppDENM_termination_id:
		denm_termination(signal, request_id, app_params) ;
		break ; 
	default : break ; 
	
	
				
}

return act_id ; 

}



denm_t* denm_reception_management(DENM_t* packet) {

	int T_R_validity // indicates the end of DENM validity ; 
	
	int expiration_time ;
	
	denm_t* payload; 
	payload = (denm_t*)malloc(sizeof(denm_t)) ; 
	memset(payload,0, sizeof(denm_t)) ; 
	
	payload = decode_denm() 
	
	/* calculate expiration_time for T_R_validity */
	
	
	if (payload->management.validity_duration != 0) 
		expiration_time = timeval_to_ms(&payload->management.detection_time) + payload->management.validity_duration ; 
	else 
		expiration_time = timeval_to_ms(&payload->management.detection_time) + 600000 ; 
		
		
	if ( timeval_to_ms(&current_time) > expiration_time ) {
			printf(" detection time is in the past ") ; 
			exit(1) ; 
	} 
	
	
	/* lookup entries in the receiving messages table with the received action id  */
	
	
	int cpt = 0 ; 
		while(denm_rx_messages->denm_messages[cpt]!= NULL && denm_rx_messages->denm_messages[cpt]->node_id != app_params->params.update_params.action_id.node_id && 				denm_rx_messages.denm_messages[cpt]->sequence_number != app_params->params.updtae_params.sequence_number) {
			cpt++ ; 
		}
	
	
	
	/* if entry doesn't exist in the table */ 
	
	if ( received_messages->action_id != payload->management.action_id) {
		if( payload->management.termination != 0) 
		//discard the denm
			exit(1) ; 
		
		
		/* create an entry in the table */ 
		cpt++ ; 
		denm_rx_messages->count++
		denm_rx_messages->denm_messages[cpt]->action_id.node_id = payload->management.action_id.node_id ; 
		denm_rx_messages->denm_messages[cpt]->action_id.sequence_number = payload->management.action_id.sequence_number ; 
		denm_rx_messages->denm_messages[cpt]->reference_time = payload->management.reference_time ;
		denm_rx_messages->denm_messages[cpt]->termination = payload->management.termination ;
		denm_rx_messages->denm_messages[cpt]->detection_time = payload->management.detection_time ;   
		
		// set the state to ACTIVE 
		denm_rx_messages->denm_messages[cpt]->state = ACTIVE ; 

	}
	
	
	
	else {
	
		/* check if the received reference time is less than the entry reference time or the received detection time is less than the entry detection time */ 
		if (payload->management.reference_time < denm_rx_messages->denm_messages[cpt]->reference_time || payload->management.detection_time < denm_rx_messages->denm_messages[cpt]->detection_time) 
			//discard denm
			exit(1) ; 
			
		if (payload->management.reference_time == denm_rx_messages->denm_messages[cpt]->reference_time && payload->management.detection_time == denm_rx_messages->denm_messages[cpt]->detection_time && payload->management.termination == denm_rx_messages->denm_messages[cpt]->termination) 
			//discard denm 
			exit(1) ; 
		 
				
		/* update the entry */
		
		denm_rx_messages->denm_messages[cpt]->action_id.node_id = payload->management.action_id.node_id ; 
		denm_rx_messages->denm_messages[cpt]->action_id.sequence_number = payload->management.action_id.sequence_number ;
		denm_rx_messages->denm_messages[cpt]->reference_time = payload->management.reference_time ;
		denm_rx_messages->denm_messages[cpt]->detection_time = payload->management.detection_time ;
		denm_rx_messages->denm_messages[cpt]->termination = payload->management.termination ;    
		
		
		
		
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


