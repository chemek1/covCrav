#include <time.h>
#include <stdio.h>
#include <stdlib.h> 
#include <stdbool.h> 
#include <unistd.h> 
#include "facility.h" 
 

int signal = -1



cam_t * encode_cam(int signal) 
{

	cam_t* new_cam = (cam_t*)malloc(sizeof(cam_t));
	memset(new_cam, 0, sizeof(cam_t)) ; 
	
	return new_cam ; 
} 

cam_t* decode_cam(CAM_t* packet, int* signal) {

	its_pdu_header_t* cam_header ; 
	cam_t* cam_payload ; 
	
	cam_header = (its_pdu_header_t*)malloc(sizeof(its_pdu_header_t)) ; 
	memset(cam_header, 0, sizeof(its_pdu_header_t)) ; 
	
	cam_payload = (cam_t*)malloc(sizeof(cam_t*)); 
	memset(cam_payload, 0, sizeof(cam_t)) ; 
	
	memcpy((void*)cam_header, (void*)&packet->header, sizeof(its_pdu_header_t)) ; 
	memcpy((void*)cam_header, (void*)&packet->payload, sizeof(cam_t)) ; 
	
	// determiner le signal 
	
	return cam_payload ;


} 


bool position_has_changed(uint16_t altitude1, uint16_t altitude2, uint16_t longitude1, uint16_t longitude2) {

return ((altitde1 != altitude2) || (longitude1 != longitude2)) ; 

}

cam_t* cam_transmission_management(int signal) {

	clock_t start, finish ; 
	
	itsnet_heading heading1, heading2 ;
	itsnet_speed speed1, speed2 ;  
	
	double elapsedTimeSinceLastCAM ; 

	int T_CheckGenCam = 5000 ; // set the check interval to 5 seconds 
	int T_GenCamMax = 60000 ;  // set the maximum cam generation interval to 60 seconds 
	int T_GenCamDCC = 30000 ; // Set the DCC-specific CAM generation interval to 30 seconds 
	int T_GenCam = T_GenCamMax ;  
	
	
	int N_GenCam = 3 ; // set the maximum and default number of consecutives CAMs ; 
	
	int nConsecutivesCams ; // the current number of consecutives CAMs 
	
	
	CAM_t* cam_packet ; 
	cam_t* cam_payload ; 
	its_pdu_header_t* cam_header ; 
	
	cam_packet = (CAM_t*)malloc(sizeof(CAM_t)) ; 
	memset(cam_packet, 0, sizeof(CAM_t)) ; 
	
	cam_payload = (cam_t*)malloc(sizeof(cam_t)) ; 
	memset(cam_payload, 0, sizeof(cam_t)) ; 
	
	cam_header = (its_pdu_header_t*)malloc(sizeof(its_pdu_header_t)) ; 
	memset(cam_header, 0, sizeof(its_pdu_header_t)) ; 
	
	
	if (signal == -1) {
	
	int timer = T_CheckGenCam ; // start the timer 
	
		while(1) {
		
			heading1 = pos_vector.heading ; 
			speed1 = pos_vector.speed ; 
			altitude1 = pos_vector.altitude ; 
			longitude1 = pos_vector.longitude ; 
		
			if (timer == 0) {
			
			
			
				finish = clock() ; 
				
				elapsedTimeSinceLastCAM = (double)(finish - start)/CLOCKS_PER_SEC ;
				
				heading2 = pos_vector.heading ; 
				speed2 = pos_vector.speed ;
				altitude2 = pos_vector.altitude ; 
				longitude2 = pos_vector.longitude ;  
				
				/* check condition 1 */  
				
				if (elapsedTimeSinceLastCAM >= T_GenCamDCC && ( (abs(heading2-heading1) != 0) || (abs(speed2-speed1) != 0)  || position_has_changed())) 
				
				{
			
					cam_payload = encode_cam(-1) ;
					header->protocol_version = protocol_version_current_version ;  
					header->message_id = message_id_cam ; 
					header->station_id = pos_vector.node_id ; 
		
					memcpy((void*)&cam_packet->header, (void*)cam_header, sizeof(its_pdu_header_t)) ; 
					memcpy((void*)&cam_packet->payload, (void*)cam_payload, sizeof(cam_t)) ;
					
					/* set btp params */ 
					
					/* send cam packet to btp */ 
					
					start = clock() ; 	
						
				} 
				
				
				
				/* check condition 2 */ 
			
				else if (elapsedTimeSinceLastCAM >= T_GenCam && elapsedTimeSinceLastCAM >= T_GenCam_Dcc ) {
		         
		         		cam_payload = encode_cam(-1) ; 
		         		
					header->protocol_version = protocol_version_current_version ;  
					header->message_id = message_id_cam ; 
					header->station_id = pos_vector.node_id ; 
		
					memcpy((void*)&cam_packet->header, (void*)cam_header, sizeof(its_pdu_header_t)) ; 
					memcpy((void*)&cam_packet->payload, (void*)cam_payload, sizeof(cam_t)) ;
					
					/* set btp params */ 
					
					/* send cam packet to btp (can be assigned to applications layer )  */ 
					
					start = clock() ; 
		
		} 
		
		timer-=1000 ; 
		sleep(1) ; 
		
	}
	
	
}
	
	else { // facilities request from upper layer (applications layer) 
	
		cam_payload = encode_cam(signal) ; 
		header->protocol_version = protocol_version_current_version ;  
		header->message_id = message_id_cam ; 
		header->station_id = pos_vector.node_id ; 
		
		memcpy((void*)&cam_packet->header, (void*)cam_header, sizeof(its_pdu_header_t)) ; 
		memcpy((void*)&cam_packet->payload, (void*)cam_payload, sizeof(cam_t)) ; 
		
	
	}
	
	/* set btp params*/ 
	
	/* send cam_packet to btp */
	 
	
	/* save data for next cam generation */ 
	
	//save_data () ; 
	
	nConsecutivesCams++ ; 
	
	if (nConsecutivesCams >= N_GenCam) {
	
		T_GenCam = T_GenCamMax ; 
		nConsecutivesCams = 0 ; 
	} 
	else 
		T_GenCam = elapsedTimeSinceLastTime ; 
	
	}
		 	
}


void cam_reception_management(CAM_t* packet, int* signal) {

	cam_t* new_cam ; 
	new_cam = (cam_t*)malloc(sizeof(cam_t)) ; 
	memset(new_cam, 0, sizeof(cam_t)) ; 
	
	new_cam = decode_cam(packet, signal) ; 
	
	/* pass cam to applications layer */ 
	

} 





