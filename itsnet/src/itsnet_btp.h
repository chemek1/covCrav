#ifndef _ITSNET_BTP_H_
#define _ITSNET_BTP_H_ 1

#include "itsnet_common.h" 


#include "denm.h" 



#define BTPA 0
#define BTPB 1

typedef uint16_t destination_port_t; /* destination port type */
typedef uint16_t max_packet_lifetime_t; 
typedef uint64_t certificate_Id_t;
typedef uint8_t ssp_length_t;
typedef uint16_t rem_packet_lifetime_t;
typedef uint8_t rem_hop_limit_t;
typedef uint32_t destination_port_info_t ; 



//updated : ETSI TS 103 248 V2.1.1 (2021-08)
typedef enum btp_port {
    btp_Port_UNSPECIFIED = 0,
    btp_port_CAM = 2001,
    btp_port_DENM = 2002,
    btp_port_MAPEM = 2003,
    btp_port_SPATEM = 2004,
    btp_port_SAEM = 2005,
    btp_port_IVIM = 2006,
    btp_port_SREM = 2007,
    btp_port_SSEM = 2008,
    btp_port_CPM = 2009,
    btp_port_EVCSN = 2010,
    btp_port_TPG = 2011,
    btp_port_EV_RSR = 2012,
    btp_port_RTCMEM = 2013,
    btp_port_CTLM = 2014,
    btp_port_CRLM = 2015,
    btp_port_EC_AT = 2016,
    btp_port_MCDM = 2017,
    btp_port_VAM = 2018,
    btp_port_IMZM = 2019
} e_btp_port;

/*typedef struct data {   
        DENM_t* denm;        
} data_t;*/



typedef struct btp_tx_params {
    int btp_type;
    e_btp_port source_port;
    e_btp_port destination_port;
    enum itsnet_header_type packet_transport_type; //header
    uint8_t destination_address[MAX_LLA_LEN]; 
    uint8_t traffic_class; //header
    DENM_t data;
} btp_tx_params_t;

typedef struct btp_rx_params {

	e_btp_port source_port; 
	e_btp_port destination_port ; 
	enum itsnet_header_type packet_transport_type ; 
	uint8_t destination_address[MAX_LLA_LEN] ; 
	itsnet_position_vector source_position_vector ; 
	uint8_t traffic_class ; 
	int length ; 
	DENM_t data ;
} btp_rx_params_t ; 

typedef struct btp_header {
    e_btp_port source_port;
    e_btp_port destination_port;
} btp_header_t;


typedef struct btp_b_header {
	
	e_btp_port destination_port ;
	//uint32_t destination_port ; 
	destination_port_info_t destination_port_info ; 
	
} btp_b_header_t ;

typedef struct btp_b_pdu {
	btp_b_header_t btp_header ; 
	DENM_t payload ; 
	//size_t payload_length ; 
} btp_b_pdu_t ;

typedef struct btp_pdu {
    btp_header_t btp_header;
    DENM_t payload;
    //size_t payload_length;
} btp_pdu_t;

struct message* btp_data_request(btp_tx_params_t params);
void btp_data_indication(btp_rx_params_t params) ; 

#endif 



