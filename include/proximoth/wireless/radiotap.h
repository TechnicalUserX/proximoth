#ifndef PROXIMOTH_RADIOTAP_H
#define PROXIMOTH_RADIOTAP_H

#include <proximoth/proximoth.h>

typedef unsigned char byte_t;

struct proximoth_radiotap_header {

	uint8_t version;

	uint8_t padding;

	uint16_t length;

	uint32_t present_flags;
};

struct proximoth_radiotap_align_size {
	uint8_t align:4, size:4;
};

extern byte_t proximoth_radiotap_template[8];

extern struct proximoth_radiotap_align_size proximoth_radiotap_namespaces[23];

enum proximoth_radiotap_presence {
	PROXIMOTH_RADIOTAP_TSFT = 0,
	PROXIMOTH_RADIOTAP_FLAGS = 1,
	PROXIMOTH_RADIOTAP_RATE = 2,
	PROXIMOTH_RADIOTAP_CHANNEL = 3,
	PROXIMOTH_RADIOTAP_FHSS = 4,
	PROXIMOTH_RADIOTAP_DBM_ANTSIGNAL = 5,
	PROXIMOTH_RADIOTAP_DBM_ANTNOISE = 6,
	PROXIMOTH_RADIOTAP_LOCK_QUALITY = 7,
	PROXIMOTH_RADIOTAP_TX_ATTENUATION = 8,
	PROXIMOTH_RADIOTAP_DB_TX_ATTENUATION = 9,
	PROXIMOTH_RADIOTAP_DBM_TX_POWER = 10,
	PROXIMOTH_RADIOTAP_ANTENNA = 11,
	PROXIMOTH_RADIOTAP_DB_ANTSIGNAL = 12,
	PROXIMOTH_RADIOTAP_DB_ANTNOISE = 13,
	PROXIMOTH_RADIOTAP_RX_FLAGS = 14,
	PROXIMOTH_RADIOTAP_TX_FLAGS = 15,
	PROXIMOTH_RADIOTAP_RTS_RETRIES = 16,
	PROXIMOTH_RADIOTAP_DATA_RETRIES = 17,
	PROXIMOTH_RADIOTAP_MCS = 19,
	PROXIMOTH_RADIOTAP_AMPDU_STATUS = 20,
	PROXIMOTH_RADIOTAP_VHT = 21,
	PROXIMOTH_RADIOTAP_TIMESTAMP = 22,
	PROXIMOTH_RADIOTAP_RADIOTAP_NAMESPACE = 29,
	PROXIMOTH_RADIOTAP_VENDOR_NAMESPACE = 30,
	PROXIMOTH_RADIOTAP_EXT = 31,
	PROXIMOTH_RADIOTAP_DUMMY = 32
};

int proximoth_radiotap_find_next_boundary(int current_byte_jump);

int proximoth_radiotap_calculate_byte_jump(int frame_remaining_bytes, int field_alignment, int field_size);

void proximoth_radiotap_initialize_align_size(struct proximoth_radiotap_align_size radiotap_align_size[23]);

byte_t* proximoth_radiotap_header_find_field(enum proximoth_radiotap_presence field_index,byte_t* frame_bytes);


#endif