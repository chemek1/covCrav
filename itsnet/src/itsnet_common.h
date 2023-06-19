#ifndef _ITSNET_COMMON_H_
#define _ITSNET_COMMON_H_ 1

#include <stdint.h> 
#include <itsnet_constant.h>

typedef uint32_t itsnet_latitude;     /** latitude of the ITS station */
typedef uint32_t itsnet_longitude;    /** longitude of the ITS station*/
typedef uint8_t itsnet_speed;        /** speed of the ITS station*/ /*** updated (était sur 16 bits */
typedef uint8_t itsnet_heading;      /** heading of the ITS station*/ /*** updated (était sur 16 bits) */ 
typedef uint32_t itsnet_time_stamp;
typedef uint8_t itsnet_traffic_class;
typedef uint8_t itsnet_position_accuracy_indicator ; /*** updated ****/ 

enum itsnet_header_type {

    itsnet_any_id = 0,      /** value of packet unspecified */
    itsnet_beacon_id,       /** value of packet Beacon */
    itsnet_unicast_id,      /** value of packet Geo-unicast  */
    itsnet_geoanycast_id,   /** value of packet Geo-anycast */
    itsnet_geobroadcast_id, /** value of packet Geo-broadcast */
    itsnet_tsb_id,          /** value of packet Topologically-scoped broadcast */
    itsnet_ls_id            /** value of packet Location service */
};

struct itsnet_node_id {
    uint8_t id[NODE_ID_LEN];
};
typedef struct itsnet_node_id itsnet_node_id;

struct itsnet_position_vector {
    itsnet_node_id node_id;
    itsnet_time_stamp
    time_stamp; /** UTC time in seconds, when the GPS data was calculated,NOT the time this message was generated */ //header
    itsnet_latitude latitude;   /** the latitude of the global position in 1/8 microdegree */ //header
    itsnet_longitude longitude; /** the longitude of the global position in 1/8 microdegree*/ //header
    itsnet_speed speed;         /** current speed in 0.01 meters per second*/ //header
    itsnet_heading heading;     /** current curse in 0.005493247 degrees*/ //header
    itsnet_position_accuracy_indicator position_accuracy ; 
};
typedef struct itsnet_position_vector itsnet_position_vector;


#endif 

