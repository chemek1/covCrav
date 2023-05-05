#ifndef __FACILITY_
#define __FACILITY_

#include <time.h>
#include <stdbool.h> 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <sys/timex.h>
#include <stdint.h>
#include <unistd.h> 
//#include <ntp.h>
#include <netinet/in.h>
#include "itsnet_btp.h" 

#define MAX_LLA_LEN 8
#define NODE_ID_LEN 8

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



/* TODO Reimplement using the Draft ETSI EN 302 637-3 V1.3.0 (2018-08) */

typedef uint16_t transmission_interval_t;       /* transmission_interval */

typedef uint32_t validity_duration_t;           /* validity_duration */

typedef uint8_t termination_t;                  /* Termination */

typedef uint8_t station_type_t;                 /* station_type */

typedef uint8_t road_type_t;                    /* RoadType */

typedef uint8_t relevance_traffic_direction_t;  /* Relevance Traffic Direction */

typedef uint8_t relevance_distance_t;           /* Relevance distance */

typedef uint8_t cause_code_type_t;              /* cause_code_type */

typedef uint8_t sub_cause_code_type_t;          /* SubCauseCodeType */
/*
   typedef long
    adverse_weather_condition_PrecipitationSubCauseCode_t;

   typedef long adverse_weather_condition_VisibilitySubCauseCode_t;

   typedef long
    adverse_weather_condition_ExtremeWeatherConditionSubCauseCode_t;
   typedef long adverse_weather_condition_AdhesionSubCauseCode_t;
 */
typedef uint16_t path_delta_time_t;             /* path_delta_time */

typedef uint8_t information_quality_t;          /* information_quality */

typedef uint16_t sequence_number_t;             /* SequenceNumber */

typedef uint16_t heading_value_t;               /* heading_value */

typedef uint16_t speed_value_t;                 /* speed_value */

typedef uint8_t heading_confidence_t;           /* heading_confidence */

typedef uint8_t speed_confidence_t;             /* speed_confidence */

typedef int8_t temperature_t;                   /* Temperature */

typedef int8_t lane_position_t;                 /* lane position of the event position*/

typedef uint16_t event_delta_time_t;            /* event delta time */

typedef uint32_t position_of_occupants_t;       /* position_of_occupants */

typedef uint8_t request_response_indication_t;

typedef uint8_t positioning_solution_type_t;  /* positioning_solution_type */

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

typedef uint8_t lateral_acceleration_t ;  

typedef uint8_t vertical_acceleration_t ; 

typedef uint8_t performance_class_t ; 

typedef uint8_t speed_limit_t ; 

typedef uint8_t dalta_latitude_t ; 

typedef uint8_t delta_longitude_t ; 

typedef uint8_t delta_altitude_t ; 

typedef uint8_t yaw_rate_confidence_t ; 

typedef uint8_t curvature_value_t ; 

typedef uint8_t longitudinal_acceleration_value_t ; 

typedef uint8_t logitudinal_acceleration_confidence_t ; 

typedef uint8_t delta_latitude_t ; 

typedef uint8_t delta_altitude_t ; 

typedef uint8_t delta_longitude_t ; 

//typedef uint8_t itsnet_time_stamp ; 

//typedef uint8_t itsnet_latitude ; 

//typedef uint8_t itsnet_longitude ; 

//typedef uint8_t itsnet_speed ; 

//typedef uint8_t itsnet_heading ; 


typedef unsigned char special_transport_type_t ; 

typedef unsigned char light_bar_siren_in_use_t ;

typedef unsigned char acceleration_control_t ; 

typedef unsigned char driving_lanes_status_t ; 

typedef unsigned char emergency_priority_t ; 

typedef unsigned char exterior_lights_t ; 

/*typedef enum speed_confidence {
	speed_confidence_equalOrWithinOneCentimeterPerSec	= 1,
	speed_confidence_equalOrWithinOneMeterPerSec		= 100,
	speed_confidence_outOfRange				= 126,
	speed_confidence_unavailable				= 127
} e_speed_confidence;*/ 

/*typedef enum transmission_interval {
	transmission_interval_oneMilliSecond	= 1,
	transmission_interval_tenSeconds	= 10000
} e_transmission_interval; */ 

/*typedef enum heading_value {
	heading_value_wgs84North	= 0,
	heading_value_wgs84East		= 900,
	heading_value_wgs84South	= 1800,
	heading_value_wgs84West		= 2700,
	heading_value_unavailable	= 3601
} e_heading_value;*/ 

/*typedef enum speed_value {
	speed_value_standstill		= 0,
	speed_value_oneCentimeterPerSec = 1,
	speed_value_unavailable		= 16383
} e_speed_value;*/

/*typedef enum heading_confidence {
	heading_confidence_equalOrWithinZeroPointOneDegree	= 1,
	heading_confidence_equalOrWithinOneDegree		= 10,
	heading_confidence_outOfRange				= 126,
	heading_confidence_unavailable				= 127
} e_heading_confidence;*/

/*typedef enum validity_duration {
	validity_duration_timeOfDetection		= 0,
	validity_duration_oneSecondAfterDetection	= 1
} e_validity_duration;*/

/*typedef enum termination { termination_is_cancellation = 0, Termination_is_negation = 1 } e_termination;*/


/*typedef enum temperature {
	temperature_equalOrSmallerThanMinus60Deg	= -60,
	temperature_oneDegreeCelsius			= 1,
	temperature_equalOrGreaterThan67Deg		= 67
} e_Temperature;*/

/*typedef enum station_type {
	station_type_unknown		= 0,
	station_type_pedestrian		= 1,
	station_type_cyclist		= 2,
	station_type_moped		= 3,
	station_type_motorcycle		= 4,
	station_type_passengerCar	= 5,
	station_type_bus		= 6,
	station_type_lightTruck		= 7,
	station_type_heavyTruck		= 8,
	station_type_trailer		= 9,
	station_type_specialVehicles	= 10,
	station_type_tram		= 11,
	station_type_roadSideUnit	= 15
} e_station_type; */

/*typedef enum road_type {
	road_type_urban_no_structural_separation_to_opposite_lanes		= 0,
	road_type_urban_with_structural_separation_to_opposite_lanes		= 1,
	road_type_non_urban_no_structural_separation_to_opposite_lanes		= 2,
	road_type_non_urban_with_structural_separation_to_opposite_lanes	= 3
} e_road_type;*/ 

/*typedef enum relevance_traffic_direction {
	relevance_traffic_direction_allTrafficDirections	= 0,
	relevance_traffic_direction_upstreamTraffic		= 1,
	relevance_traffic_direction_downstreamTraffic		= 2,
	relevance_traffic_direction_oppositeTraffic		= 3
} e_relevance_traffic_direction;*/

/*typedef enum relevance_distance {
	relevance_distance_lessThan50m		= 0,
	relevance_distance_lessThan100m		= 1,
	relevance_distance_lessThan200m		= 2,
	relevance_distance_lessThan500m		= 3,
	relevance_distance_lessThan1000m	= 4,
	relevance_distance_lessThan5km		= 5,
	relevance_distance_lessThan10km		= 6,
	relevance_distance_over10km		= 7
} e_relevance_distance;*/


/*typedef enum adverse_weather_condition_PrecipitationSubCauseCode {
	adverse_weather_condition_PrecipitationSubCauseCode_unavailable		= 0,
	adverse_weather_condition_PrecipitationSubCauseCode_heavyRain		= 1,
	adverse_weather_condition_PrecipitationSubCauseCode_heavySnowfall	= 2,
	adverse_weather_condition_PrecipitationSubCauseCode_softHail		= 3
} e_AdverseWeatherCondition_PrecipitationSubCauseCode;*/

/*typedef enum adverse_weather_condition_VisibilitySubCauseCode {
	adverse_weather_condition_VisibilitySubCauseCode_unavailable		= 0,
	adverse_weather_condition_VisibilitySubCauseCode_fog			= 1,
	adverse_weather_condition_VisibilitySubCauseCode_smoke			= 2,
	adverse_weather_condition_VisibilitySubCauseCode_heavySnowfall		= 3,
	adverse_weather_condition_VisibilitySubCauseCode_heavyRain		= 4,
	adverse_weather_condition_VisibilitySubCauseCode_heavyHail		= 5,
	adverse_weather_condition_VisibilitySubCauseCode_lowSunGlare		= 6,
	adverse_weather_condition_VisibilitySubCauseCode_sandstorms		= 7,
	adverse_weather_condition_VisibilitySubCauseCode_swarmsOfInsects	= 8
} e_adverse_weather_condition_VisibilitySubCauseCode;*/

/*typedef enum adverse_weather_condition_ExtremeWeatherConditionSubCauseCode {
	adverse_weather_condition_ExtremeWeatherConditionSubCauseCode_unavailable	= 0,
	adverse_weather_condition_ExtremeWeatherConditionSubCauseCode_strongWinds	= 1,
	adverse_weather_condition_ExtremeWeatherConditionSubCauseCode_damagingHail	= 2,
	adverse_weather_condition_ExtremeWeatherConditionSubCauseCode_hurricane		= 3,
	adverse_weather_condition_ExtremeWeatherConditionSubCauseCode_thunderstorm	= 4,
	adverse_weather_condition_ExtremeWeatherConditionSubCauseCode_tornado		= 5,
	adverse_weather_condition_ExtremeWeatherConditionSubCauseCode_blizzard		= 6
} e_adverse_weather_condition_ExtremeWeatherConditionSubCauseCode;*/

/*typedef enum adverse_weather_condition_AdhesionSubCauseCode {
	adverse_weather_condition_AdhesionSubCauseCode_unavailable	= 0,
	adverse_weather_condition_AdhesionSubCauseCode_heavyFrostOnRoad = 1,
	adverse_weather_condition_AdhesionSubCauseCode_fuelOnRoad	= 2,
	adverse_weather_condition_AdhesionSubCauseCode_mudOnRoad	= 3,
	adverse_weather_condition_AdhesionSubCauseCode_snowOnRoad	= 4,
	adverse_weather_condition_AdhesionSubCauseCode_iceOnRoad	= 5,
	adverse_weather_condition_AdhesionSubCauseCode_blackIceOnRoad	= 6,
	adverse_weather_condition_AdhesionSubCauseCode_oilOnRoad	= 7,
	adverse_weather_condition_AdhesionSubCauseCode_looseChippings	= 8,
	adverse_weather_condition_AdhesionSubCauseCode_instantBlackIce	= 9,
	adverse_weather_condition_AdhesionSubCauseCode_roadsSalted	= 10
} e_adverse_weather_condition_AdhesionSubCauseCode;*/

/*typedef enum cause_code_type {
	cause_code_type_reserved					= 0,
	cause_code_type_traffic_condition				= 1,
	cause_code_type_accident					= 2,
	cause_code_type_roadworks					= 3,
	cause_code_type_adverse_weather_conditionAdhesion		= 6,
	cause_code_type_hazardous_location_surfaceCondition		= 9,
	cause_code_type_hazardous_location_obstacleOnTheRoad		= 10,
	cause_code_type_hazardous_location_animalOnTheRoad		= 11,
	cause_code_type_human_presenceOnTheRoad				= 12,
	cause_code_type_wrongWayDriving					= 14,
	cause_code_type_rescueAndRecoveryWorkInProgress			= 15,
	cause_code_type_adverseWeatherCondition_ExtremeWeatherCondition = 17,
	cause_code_type_adverseWeatherCondition_Visibility		= 18,
	cause_code_type_adverseWeatherCondition_Precipitation		= 19,
	cause_code_type_slowVehicle					= 26,
	cause_code_type_dangerousEndOfQueue				= 27,
	cause_code_type_vehicleBreakdown				= 91,
	cause_code_type_postCrash					= 92,
	cause_code_type_humanProblem					= 93,
	cause_code_type_stationaryVehicle				= 94,
	cause_code_type_emergencyVehicleApproaching			= 95,
	cause_code_type_hazardousLocation_DangerousCurve		= 96,
	cause_code_type_collisionRisk					= 97,
	cause_code_type_signalViolation					= 98,
	cause_code_type_dangerousSituation				= 99
} e_cause_code_type;*/

/*typedef enum information_quality {
	information_quality_unavailable = 0,
	information_quality_lowest	= 1,
	information_quality_highest	= 7
} e_information_quality;*/

/*typedef enum path_delta_time { path_delta_time_tenMilliSecondsInPast = 1 } e_path_delta_time;*/

/*typedef enum protocol_version { protocol_version_current_version = 1 } e_protocol_version; */

/*typedef enum lane_position {
	lane_position_offTheRoad		= -1,
	lane_position_hardShoulder		= 0,
	lane_position_outermostDrivingLane	= 1,
	lane_position_secondLaneFromOutside	= 2
} e_lane_position;*/

/*typedef enum position_of_occupants {
	position_of_occupants_row1LeftOccupied	= 0,
	position_of_occupants_row1RightOccupied = 1,
	position_of_occupants_row1MidOccupied	= 2,
	position_of_occupants_row1NotDetectable = 3,
	position_of_occupants_row1NotPresent	= 4,
	position_of_occupants_row2LeftOccupied	= 5,
	position_of_occupants_row2RightOccupied = 6,
	position_of_occupants_row2MidOccupied	= 7,
	position_of_occupants_row2NotDetectable = 8,
	position_of_occupants_row2NotPresent	= 9,
	position_of_occupants_row3LeftOccupied	= 10,
	position_of_occupants_row3RightOccupied = 11,
	position_of_occupants_row3MidOccupied	= 12,
	position_of_occupants_row3NotDetectable = 13,
	position_of_occupants_row3NotPresent	= 14,
	position_of_occupants_row4LeftOccupied	= 15,
	position_of_occupants_row4RightOccupied = 16,
	position_of_occupants_row4MidOccupied	= 17,
	position_of_occupants_row4NotDetectable = 18,
	position_of_occupants_row4NotPresent	= 19
} e_position_of_occupants;*/

/*typedef enum request_response_indication {
	request_response_indication_request	= 0,
	request_response_indication_response	= 1
} e_request_response_indication;*/

/*typedef enum positioning_solution_type {
	positioning_solution_type_noPositioningSolution = 0,
	positioning_solution_type_sGNSS			= 1,
	positioning_solution_type_dGNSS			= 2,
	positioning_solution_type_sGNSSplusDR		= 3,
	positioning_solution_type_dGNSSplusDR		= 4,
	positioning_solution_type_dR			= 5
	                                                  /*
	                                                   * Enumeration is extensible
	                                                   */
//} e_positioning_solution_type; 


/* typedef enum message_id {
	message_id_denm		= 1,
	message_id_cam		= 2,
	message_id_poi		= 3,
	message_id_spat		= 4,
	message_id_map		= 5,
	message_id_ivi		= 6,
	message_id_ev_rsr	= 7
} e_message_id;*/

typedef enum drive_direction {
	forward = 0, 
	backward = 1,
	unavailable = 2, 

} drive_direction_t ; 

/*typedef enum traffic_rule {
	no_passing = 0, 
	no_passing_for_trucks = 1,
	pass_to_right = 2, 
	pass_to_left = 3, 

} traffic_rule_t ; */

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

/*typedef enum dangerous_goods_basic {

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

} dangerous_goods_basic_t ; */

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

/*typedef enum hard_shoulder_status {

	available_for_stopping = 0, 
	closed = 1, 
	available_for_driving = 2, 

} hard_shoulder_status_t ; */

/*typedef enum traffic_class { 
	CLASS00 = 0, 
	CLASS01 = 1, 
	CLASS02 = 2, 
	CLASS03 = 3 
} traffic_class_t ;*/






/* struct list_head {
    struct list_head *next, *prev;
}; */



/*struct itsnet_node_id {
    uint8_t id[NODE_ID_LEN];
};
typedef struct itsnet_node_id itsnet_node_id;*/



typedef struct message {

	itsnet_node_id node_id ; 
	int sequence_number ; 
}message_t ; 

message_t* denm_messages[1000] ; 


typedef struct received_message { 



} received_message_t ; 

/*typedef struct position {
    itsnet_node_id node_id;
    itsnet_time_stamp time_stamp; /** UTC time in seconds, when the GPS data was calculated,NOT the time this message was generated */
   // itsnet_latitude latitude;   /** the latitude of the global position in 1/8 microdegree */
   // itsnet_longitude longitude; /** the longitude of the global position in 1/8 microdegree*/
   // itsnet_speed speed;         /** current speed in 0.01 meters per second*/
   // itsnet_heading heading;     /** current curse in 0.005493247 degrees*/
//} itsnet_position_vector ; 

/*typedef struct its_pdu_header {
	uint8_t protocol_version;
	uint8_t message_id;
	itsnet_node_id station_id;
} its_pdu_header_t; */



/*
 * DF_DeltaReferencePosition as defined in ETSI TS 102 894-2 V1.2.1
 * */
/*typedef struct delta_reference_position {
	int32_t delta_latitude;
	int32_t delta_longitude;
	int16_t deltaAltitude;
}delta_reference_position_t;*/


/*
 * DF_PathPoint as defined in ETSI TS 102 894-2 V1.2.1
 * */
/*typedef struct path_point {
	delta_reference_position_t path_position;
	path_delta_time_t path_delta_time;
} path_point_t;*/
 

/*
 * DF_EventPoint as defined in ETSI TS 102 894-2 V1.2.1 clause A.110.
 * */
/*typedef struct event_point {
	delta_reference_position_t event_position;
	event_delta_time_t event_delta_time;
	information_quality_t information_quality;
} event_point_t;*/


/*DF_EventHistory as defined in ETSI TS 102 894-2 V1.2.1.
 * */
/*typedef struct event_history {
	event_point_t event_pt;
	struct list_head list;
} event_history_t;*/



/*
 * DF_PathHistory as defined in ETSI TS 102 894-2 V1.2.1. Size of the Array shall be 23
   as defined in ETSI EN 302 637-2 V1.3.2.
 * */
/*typedef struct path_history {
	path_point_t path_pt;
	struct list_head list;
} path_history_t;*/

/*
 * DF_Traces as defined in ETSI TS 102 894-2 V1.2.1. Size of the Array shall be 7.
 * */
/*typedef struct traces {
	uint8_t count;
	path_history_t values[7];
} traces_t;*/

/*typedef struct heading {
	heading_value_t heading_value;
	heading_confidence_t heading_confidence;
} heading_t;*/

/*typedef struct speed {
	speed_value_t speed_value;
	speed_confidence_t speed_confidence;
} speed_t;*/

/*
 * DF_ActionID as defined in ETSI TS 102 894-2 V1.2.1.
 * */
/*typedef struct action_id {
	itsnet_node_id originating_station_id;
	sequence_number_t sequence_number;
} action_id_t;*/


/* CauseCode */
/*typedef struct cause_code {
	cause_code_type_t causeCode;
	sub_cause_code_type_t sub_causeCode;
} cause_code_t;*/

/*
 * DF_PositionOfPillars as defined in ETSI TS 102 894-2 V1.2.1
 * */
/*typedef struct position_of_pillars {
	uint8_t count;
	uint8_t values[3];
} position_of_pillars_t;*/

typedef struct road_works_container_extended { /* TODO*/
	/* TODO */
	uint8_t speedLimit;
} road_works_container_extended_t;

typedef struct stationary_vehicle_container { /* TODO */
	/* TODO */
	uint8_t number_of_occupants;
}stationary_vehicle_container_t;

/*typedef struct impact_reduction_container {
	uint8_t height_lon_carr_left;
	uint8_t height_lon_carr_right;
	uint8_t pos_lon_carr_left;
	uint8_t pos_lon_carr_right;
	position_of_pillars_t position_of_pillars;
	uint8_t pos_cent_mass;
	uint8_t wheel_base_vehicle;
	uint8_t turning_radius;
	uint8_t pos_front_ax;
	position_of_occupants_t position_of_occupants;
	uint16_t vehicle_mass;
	request_response_indication_t request_response_indication;
} impact_reduction_container_t; */

/*
 * SituationContainer as defined in ETSI EN 302 637-3 V1.2.2.
 * */
/*typedef struct situation_container {
	information_quality_t information_quality;
	cause_code_t event_type;
	cause_code_t linked_cause /* OPTIONAL */;
	//event_history_t event_history /* OPTIONAL */;
//} situation_container_t;

/*
 * LocationContainer as defined in ETSI EN 302 637-3 V1.2.2.
 * */
/*typedef struct location_container {
	struct speed event_speed /* OPTIONAL */;
	//struct heading event_position_heading /* OPTIONAL */;
	//traces_t traces;
	//road_type_t roadType /* OPTIONAL */;
//} location_container_t;
/*
 * AlacarteContainer as defined in ETSI EN 302 637-3 V1.2.2.
 * */
/*typedef struct alacarte_container {
	lane_position_t lane_position;
	impact_reduction_container_t impact_reduction;
	temperature_t external_temperature;
	road_works_container_extended_t road_works; // not achieved
	positioning_solution_type_t positioning_solution;
	stationary_vehicle_container_t stationary_vehicle_container;
} alacarte_container_t;*/


/*typedef struct management_container {
	action_id_t action_id;
	itsnet_time_stamp detection_time;
	itsnet_time_stamp reference_time;
	termination_t termination /* OPTIONAL */;
	//itsnet_position_vector event_position;
	//double relevance_distance /* OPTIONAL */;
	//relevance_traffic_direction_t relevance_traffic_direction /* OPTIONAL */;
	//validity_duration_t validity_duration /* DEFAULT 600 */;
	//transmission_interval_t transmission_interval /* OPTIONAL */;
	//station_type_t station_type;
//} management_t;

/* decentralized_environmental_notification_message */
/*typedef struct decentralized_environmental_notification_message {
	management_t management;
	situation_container_t situation /* OPTIONAL */;
	//location_container_t location /* OPTIONAL */;
	//alacarte_container_t alacarte /* OPTIONAL */;
//} denm_t;

/* ItsPduHeader */

/* DENM */
/*typedef struct DENM {
	its_pdu_header_t header;
	denm_t payload;
} DENM_t;*/



typedef struct special_transport_container {
	special_transport_type_t type ;
	light_bar_siren_in_use_t light_bar_siren ; 

} special_transport_container_t ; 

typedef struct dangerous_goods_container {
	e_dangerous_goods_basic goods_basic ; 
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
	itsnet_time_stamp expiry_time ;
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
	e_hard_shoulder_status shoulder_status ; /*OPTIONAL*/ 
	driving_lanes_status_t lanes_status ;  /*OPTIONAL*/ 

} closed_lanes_t ; 

typedef struct road_works_container_basic {
	road_works_sub_cause_code_t sub_cause_code ; 
	light_bar_siren_in_use_t light_bar_siren ;
	closed_lanes_t lanes ;  
} road_works_container_basic_t ;

 

typedef struct emergency_container {
	light_bar_siren_in_use_t light_bar_siren ; 
	cause_code_t incident_indication ; /*OPTIONAL*/ 
	emergency_priority_t emergency ; /*OPTIOANL*/ 

} emergency_container_t ;  

typedef struct safety_car_container {
	light_bar_siren_in_use_t light_bar_siren ;  
	cause_code_t incident_indication ; /*OPTIONAL*/ 
	e_traffic_rule traffic ; /*OPTIONAL*/ 
	speed_limit_t limit_speed ;  /*OPTIONAL*/ 

} safety_car_container_t ;  




typedef struct special_vehicle_container {


} special_vehicle_container_t ;


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

typedef struct event_relevance_area {
	relevance_distance_t relevance_distance ;
	relevance_traffic_direction_t relevance_traffic_direction ;  
	
} event_relevance_area_t ; 

typedef struct transmission_trigger_params {

	struct ntptimeval detection_time ; 
	itsnet_position_vector event_position ; 
	validity_duration_t validity_duration ; 
	int repetition_duration ; 
	transmission_interval_t transmission_interval ; 
	int repetition_interval ; 
	situation_container_t situation_informations ; 
	location_container_t location_informations ; 
	event_relevance_area_t relevance_area ; 
	/* destination_area */ 
	itsnet_traffic_class traffic_class ; 
	action_id_t action_id ; 
	
} transmission_trigger_params_t ; 


typedef struct transmission_update_params {
	action_id_t action_id ; 
	itsnet_time_stamp detection_time ; 
	itsnet_position_vector event_position ; 
	validity_duration_t validity_duration ; 
	int repetition_duration ; 
	transmission_interval_t transmission_interval ; 
	int repetition_interval ; 
	situation_container_t situation_informations ; 
	location_container_t location_container ; 
	/* destination area */ 
	itsnet_traffic_class traffic_class ; 
	
} transmission_update_params_t ; 

typedef struct transmssion_termination_params {
	action_id_t action_id ;
	itsnet_time_stamp detection_time ; 
	validity_duration_t validity_duration ; 
	int repetition_duration ; 
	transmission_interval_t transmission_interval ; 
	int repetition_interval ; 
	/* destination_area */ 
	itsnet_traffic_class traffic_class ; 
	
} transmission_termination_params_t ; 


typedef struct transmission_params {

	union { 
		transmission_trigger_params_t trigger_params ; 
		transmission_update_params_t update_params ;
		transmission_termination_params_t termination_params ;  
	
	} params ; 

} transmission_params_t ;

/*typedef struct btp_tx_params {
    int btp_type;
    e_btp_port source_port;
    e_btp_port destination_port;
    enum itsnet_header_type packet_transport_type;
    uint8_t destination_address[MAX_LLA_LEN];
    traffic_class_t traffic_class;
    DENM_t data;
} btp_tx_params_t;*/

typedef enum application_request_type {

	AppDENM_trigger = 0, 
	AppDENM_update, 
	AppDENM_termination, 

} application_request_type_id ; 

denm_t* encode_denm(int signal, action_id_t id, transmission_interval_t transmission_interval) ; 
void decode_denm(denm_t* payload, int *weather, int *seq_nb);

void denm_transmission_management(int signal, application_request_type_id request_id, transmission_params_t* app_params) ; 
denm_t* denm_reception_management(DENM_t* packet);

void trace_recv_denm(denm_t* payload, struct ntptimeval t_recv);
void trace_sent_denm(denm_t* payload, struct ntptimeval t_emit);

cam_t * encode_cam(int signal);
void decode_cam(cam_t* payload);
cam_t* cam_transmission_management(cam_t* payload);
cam_t* cam_reception_management(CAM_t* packet);

//long timeval_to_ms(struct timeval t)

/*static inline uint32_t timeval_to_ms(struct timeval* tv)
{
    return ((uint32_t)(tv->tv_sec * 1000) + (tv->tv_usec / 1000));
} */

#endif 
