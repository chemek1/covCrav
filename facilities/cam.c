#include "facility.h" 


cam_t * encode_cam(int signal=0) 
{	
	

	cam_t* new_cam = (cam_t*)malloc(sizeof(cam_t));
	memset(new_cam, 0, sizeof(cam_t)) ; 
	
	return new_cam ; 
} 

void decode_cam(cam_t* payload) {


} 
cam_t* cam_transmission_management(cam_t* payload) {
	cam_t* new_cam = (cam_t*)malloc(sizeof(cam_t));
	memset(new_cam, 0, sizeof(cam_t)) ; 
	
	return new_cam ;

}
cam_t* cam_reception_management(CAM_t* packet) {
	cam_t* new_cam = (cam_t*)malloc(sizeof(cam_t));
	memset(new_cam, 0, sizeof(cam_t)) ; 
	
	return new_cam ;

}
