#ifndef __CAM_
#define __CAM_

#include <stdbool.h> 
#include "facility.h" 


/*vehicle length and width values */ 

#define ten_centimeters 1 
#define OUT_OF_RANGE 1022
//#define unavailable 1023

/**/

/*longitudinal and confidence acceleration values */

#define point_one_meter_per_sec_squared_forward 1
#define point_one_meter_per_sec_squared_backward -1
//#define unavailable 161 

/**/

/* curvature values */

#define straight 0 
#define reciprocal_of_1meter_radius_to_right -30000 
#define reciprocal_of_1meter_radius_to_left 30000
//#define unavailable 30001

/**/

/*yaw rate value values */

#define straight 0 
#define deg_sec-000-01_to_right -1
#define deg_sec-000-01_to_left 1
//#define unavailable 32767

/**/

/*yaw rate confidence values */

#define deg_sec_000_01 0
#define deg_sec_000_05 1
#define deg_sec_000_10 2
#define deg_sec_001_00 3
#define deg_sec_005_00 4
#define deg_sec_010_00 5
#define deg_sec_100_00 6
#define OUT_OF_RANGE 7
//#define unavailable 8

/**/

/* time stamp values */

#define utc_start_of_2004 0
#define one_ms_after_utc_start_of_2004 1

/**/

/* protected zone latitude and longitude values */ 

#define one_microdegree_north 10
#define one_microdegree_south -10
//#define unavailable 900000001 

/**/

/* road works sub cause code values */ 

//#define unvailable 0
#define major_road_works 1 
#define road_marking_work 2
#define slow_moving_maintenance 3
#define short_terms_statioanry_road_works 4
#define street_cleaning 5 
#define winter_service 6  

/**/

/* delta altitude values */ 

#define one_centimeter_up 1
#define one_centimeter_down -1
//#define unavailable 12800 

/**/

/* delta latitude values */ 

#define one_micro_degree_north 10
#define one_micro_degree_south -10


/**/

/* delta longitude values */ 

#define one_micro_degree_east 10
#define one_micro_degree_west -10


/**/

/* path delta time values */ 

#define ten_milliseconds_in_past 1 

/**/

/* longitudinal acceleration value and confidence values */ 

#define point_one_meter_per_sec_squared_forward 1 
#define point_one_meter_per_sec_squared_backward -1
//#define unavailable 161 


/**/ 



typedef uint8_t vehicle_length_value_t ; 

typedef uint8_t vehicle_width_t ; 

typedef uint8_t station_type_t;

typedef uint16_t generation_delta_time_t ; 

typedef uint16_t heading_value_t; 

typedef uint8_t heading_confidence_t;  

typedef uint16_t speed_value_t; 

typedef uint8_t speed_confidence_t; 
typedef uint8_t yaw_rate_value_t ;  

typedef uint8_t expiry_time_t ;  

typedef uint8_t protected_zone_latitude_t ; 

typedef uint8_t protected_zone_longitude_t ;

typedef uint8_t road_works_sub_cause_code_t ;

typedef uint8_t lane_position_t ; 

typedef uint8_t lateral_acceleration_t ;  

typedef uint8_t vertical_acceleration_t ; 

typedef uint8_t performance_class_t ; 

typedef uint8_t speed_limit_t ; 

typedef uint8_t dalta_latitude_t ; 

typedef uint8_t delta_longitude_t ; 

typedef uint8_t delta_altitude_t ; 

typedef uint8_t path_delta_time_t ; 

typedef uint8_t yaw_rate_confidence_t ; 

typedef uint8_t curvature_value_t ; 

typedef uint8_t longitudinal_acceleration_value_t ; 

typedef uint8_t logitudinal_acceleration_confidence_t ; 

typedef uint8_t delta_latitude_t ; 

typedef uint8_t delta_altitude_t ; 

typedef uint8_t delta_longitude_t ; 

typedef unsigned char special_transport_type_t ; 

typedef unsigned char light_bar_siren_in_use_t ;

typedef unsigned char acceleration_control_t ; 

typedef unsigned char driving_lanes_status_t ; 

typedef unsigned char emergency_priority_t ; 

typedef unsigned char exterior_lights_t ; 


typedef enum drive_direction {
	forward = 0, 
	backward = 1,
	unavailable = 2, 

} drive_direction_t ; 

typedef enum traffic_rule {
	no_passing = 0, 
	no_passing_for_trucks = 1,
	pass_to_right = 2, 
	pass_to_left = 3, 

} traffic_rule_t ; 

typedef enum vehicle_role {
	unspecified = 0, 
	public_transport = 1,
	special_transport = 2,
	dangerous_goods = 3,
	road_work = 4, 
	rescue = 5,
	emergency = 6, 
	safety_car = 7, 
	agriculture = 8, 
	commercial = 9, 
	military = 10, 
	road_operator = 11, 
	taxi = 12,
	reserved1 = 13,
	reserved2= 14, 
	reserved3 = 15,  

} vehicle_role_t ; 

typedef enum dangerous_goods_basic {

	explosives1 = 0, 
	explosives2 = 1, 
	explosives3 = 2, 
	explosives4 = 3, 
	explosives5 = 4, 
	explosives6 = 5, 
	flammable_gases = 6, 
	non_flammable_gases = 7, 
	toxic_gases = 8, 
	flammable_liquids = 9,
	flammable_solids = 10, 
	substances_liable_to_spontaneous_combustion = 11, 
	substances_emitting_flammable_gases_upon_contact_with_water = 12, 
	oxdizing_substances = 13, 
	organic_peroxides = 14, 
	toxic_substances = 15,
	infectious_substances = 16,
	radioactive_material = 17, 
	corrosive_substances = 18,
	miscellaneous_dangerous_substances = 19, 

} dangerous_goods_basic_t ;

typedef enum curvature_confidence {
	one_per_meter_0_00002 = 0, 
	one_per_meter_0_0001 = 1,
	one_per_meter_0_0005 = 2,
	one_per_meter_0_002 = 3, 
	one_per_meter_0_01 = 4,
	one_per_meter_0_1 = 5,
	out_of_range = 6, 
	//unavailable = 7, 
	
} curvature_confidence_t; 

typedef enum vehicle_length_confidence_indication {
	no_traiter_present = 0 , 
	traiter_present_with_known_length = 1 , 
	traiter_present_with_unkown_length = 2 ,
	traiter_presence_is_unkown = 3 ,
	//unavailable = 4, 

} vehicle_length_confidence_indication_t ; 

typedef enum curvature_calculation_mode {
	yaw_rate_used = 0,
	yaw_rate_not_used = 1,
	//unavailable = 2, 

} curvature_calculation_mode_t ; 

typedef enum protected_zone_type {
	cen_dsrc_trolling = 0 , 
} protected_zone_type_t ; 

typedef enum hard_shoulder_status {

	available_for_stopping = 0, 
	closed = 1, 
	available_for_driving = 2, 

} hard_shoulder_status_t ; 




typedef struct special_transport_container {
	special_transport_type_t type ;
	light_bar_siren_in_use_t light_bar_siren ; 

} special_transport_container_t ; 

typedef struct dangerous_goods_container {
	dangerous_goods_basic_t goods_basic ; 
} dangerous_goods_container_t ; 

typedef struct pt_activation {


} pt_activation_t ; 

typedef struct public_transport_container {
	bool embarkation ; 
	pt_activation_t activation ; /*OPTIONAL*/

} public_transport_container_t ; 

typedef struct protected_zone_radius {


} protected_zone_radius_t ; 

typedef struct protected_zone_id {

} protected_zone_id_t ; 

typedef struct protected_communication_zone {
	protected_zone_type_t zone_type ; 
	expiry_time_t time_stamp_its ;
	protected_zone_latitude_t latitude ;
	protected_zone_longitude_t longitude ; 
	protected_zone_radius_t radius ; /*OPTIONAL*/
	protected_zone_id_t id ; /*OPTIONAl*/ 

} protected_communication_zone_t ; 



typedef struct protected_communication_zones_rsu {
	protected_communication_zone_t comm_zone ; 

} protected_communication_zones_rsu_t ; 

typedef struct rsu_container_high_frequency {
	protected_communication_zones_rsu_t comm_zones_rsu ;

} rsu_container_high_frequency_t ;  


typedef struct yaw_rate {
	yaw_rate_value_t rate_value ;
	yaw_rate_confidence_t rate_confidence ; 

} yaw_rate_t ; 



typedef struct curvature {
	curvature_value_t curv_value ; 
	curvature_confidence_t curv_confidence ; 

} curvature_t ;  




typedef struct longitudinal_acceleration {
	longitudinal_acceleration_value_t longitudinal_acceleration_value ;  
	logitudinal_acceleration_confidence_t acceleration_confidence ; 


} longitudinal_acceleration_t ;  

typedef struct vehicle_length {
	vehicle_length_value_t length_value ; 
	vehicle_length_confidence_indication_t length_confidence_indication ; 


} vehicle_length_t ; 

typedef struct basic_container {
	station_type_t station_type ; 
	itsnet_position_vector reference_position ; 
	
} basic_container_t ;

typedef struct heading {
	heading_value_t heading_value;
	heading_confidence_t heading_confidence;
} heading_t; 

typedef struct speed {
	speed_value_t speed_value;
	speed_confidence_t speed_confidence;
} speed_t;

typedef struct steering_wheel_angle {

} steering_wheel_angle_t ; 

typedef struct cen_dsrc_tolling_zone {

} cen_dsrc_tolling_zone_t ; 

typedef struct basic_vehicle_container_high_frequency {
	heading_t vehicle_heading ; 
	speed_t vehicle_speed ; 
	drive_direction_t direction ; 
	vehicle_length_t length ; 
	vehicle_width_t width ;
	longitudinal_acceleration_t long_acceleration ; 
	curvature_t curv ; 
	curvature_calculation_mode_t curv_calculation ; 
	yaw_rate_t rate ; 
	acceleration_control_t ctrl_acceleration ; /*OPTIONAL*/
	lane_position_t lane_pos ; /*OPTIONAL*/
	steering_wheel_angle_t steering_angle ;  /*OPTIONAL*/
	lateral_acceleration_t lat_acceleration ; /*OPTIONAL*/  
	vertical_acceleration_t vert_acceleration ; /*OPTIONAL*/
	performance_class_t performance ; /*OPTIONAL*/ 
	cen_dsrc_tolling_zone_t tolling_zone ; /*OPTIONAL*/ 
	

} basic_vehicle_container_high_frequency_t ; 





typedef struct high_frequency_container {
	union {
		basic_vehicle_container_high_frequency_t basic_vehicle_high_frequency ;  
		rsu_container_high_frequency_t rsu_high_frequency ;	
	} high_frequency_t ; 

} high_frequency_container_t ; 

typedef struct closed_lanes {
	hard_shoulder_status_t shoulder_status ; /*OPTIONAL*/ 
	driving_lanes_status_t lanes_status ;  /*OPTIONAL*/ 

} closed_lanes_t ; 

typedef struct road_works_container_basic {
	road_works_sub_cause_code_t sub_cause_code ; 
	light_bar_siren_in_use_t light_bar_siren ;
	closed_lanes_t lanes ;  
} road_works_container_basic_t ;

typedef struct cause_code {

} cause_code_t ; 

typedef struct emergency_container {
	light_bar_siren_in_use_t light_bar_siren ; 
	cause_code_t incident_indication ; /*OPTIONAL*/ 
	emergency_priority_t emergency ; /*OPTIOANL*/ 

} emergency_container_t ;  

typedef struct safety_car_container {
	light_bar_siren_in_use_t light_bar_siren ;  
	cause_code_t incident_indication ; /*OPTIONAL*/ 
	traffic_rule_t traffic ; /*OPTIONAL*/ 
	speed_limit_t limit_speed ;  /*OPTIONAL*/ 

} safety_car_container_t ;  




typedef struct special_vehicle_container {


} special_vehicle_container_t ;

typedef struct delta_reference_position {
	delta_latitude_t latitude ; 
	delta_longitude_t longitude ; 
	delta_altitude_t altitude ;  

} delta_reference_position_t ; 

typedef struct path_point {

	delta_reference_position_t path_position ;  
	path_delta_time_t path_time ; /*OPTIONAL*/ 

} path_point_t ;  

typedef struct path_history {

	path_point_t path_pt ;

} path_history_t ; 


typedef struct basic_vehicle_container_low_frequency {

	vehicle_role_t role ; 
	exterior_lights_t extr_lights ;
	path_history_t path_hist ;  

} basic_vehicle_container_low_frequency_t ; 

typedef struct low_frequency_container {
	union {
		basic_vehicle_container_low_frequency_t basic_container_low_frequency ; 
	} u ;

} low_frequency_container_t ;

typedef struct its_pdu_header {
	uint8_t protocol_version;
	uint8_t message_id;
	itsnet_node_id station_id;
} its_pdu_header_t; 

typedef struct cooperative_awarness_message {
	generation_delta_time_t generation_delta_time ; 
	basic_container_t basic ; 
	high_frequency_container_t high_frequency ; 
	low_frequency_container_t low_frequency ; /*OPTIONAL*/
	special_vehicle_container_t special_vehicle ; /*OPTIONAL*/ 

} cam_t ; 


typedef struct CAM {
	its_pdu_header_t header ; 
	cam_t paylaod ; 
} CAM_t ; 

cam_t * encode_cam(int signal);
void decode_cam(cam_t* payload);
cam_t* denm_transmission_management(cam_t* payload);
cam_t* denm_reception_management(CAM_t* packet);

#endif 
