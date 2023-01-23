#ifndef PROXIMOTH_RADIOTAP_H
#define PROXIMOTH_RADIOTAP_H

#include "proximoth.h"

typedef unsigned char byte_t;

struct ieee80211_radiotap_header {

	uint8_t version;

	uint8_t padding;

	uint16_t length;

	uint32_t present_flags;
};

struct ieee80211_radiotap_align_size {
	uint8_t align:4, size:4;
};

extern byte_t ieee80211_radiotap_template[8];

extern struct ieee80211_radiotap_align_size ieee80211_radiotap_namespaces[23];

enum ieee80211_radiotap_presence {
	IEEE80211_RADIOTAP_TSFT = 0,
	IEEE80211_RADIOTAP_FLAGS = 1,
	IEEE80211_RADIOTAP_RATE = 2,
	IEEE80211_RADIOTAP_CHANNEL = 3,
	IEEE80211_RADIOTAP_FHSS = 4,
	IEEE80211_RADIOTAP_DBM_ANTSIGNAL = 5,
	IEEE80211_RADIOTAP_DBM_ANTNOISE = 6,
	IEEE80211_RADIOTAP_LOCK_QUALITY = 7,
	IEEE80211_RADIOTAP_TX_ATTENUATION = 8,
	IEEE80211_RADIOTAP_DB_TX_ATTENUATION = 9,
	IEEE80211_RADIOTAP_DBM_TX_POWER = 10,
	IEEE80211_RADIOTAP_ANTENNA = 11,
	IEEE80211_RADIOTAP_DB_ANTSIGNAL = 12,
	IEEE80211_RADIOTAP_DB_ANTNOISE = 13,
	IEEE80211_RADIOTAP_RX_FLAGS = 14,
	IEEE80211_RADIOTAP_TX_FLAGS = 15,
	IEEE80211_RADIOTAP_RTS_RETRIES = 16,
	IEEE80211_RADIOTAP_DATA_RETRIES = 17,
	IEEE80211_RADIOTAP_MCS = 19,
	IEEE80211_RADIOTAP_AMPDU_STATUS = 20,
	IEEE80211_RADIOTAP_VHT = 21,
	IEEE80211_RADIOTAP_TIMESTAMP = 22,
	IEEE80211_RADIOTAP_RADIOTAP_NAMESPACE = 29,
	IEEE80211_RADIOTAP_VENDOR_NAMESPACE = 30,
	IEEE80211_RADIOTAP_EXT = 31,
	IEEE80211_RADIOTAP_DUMMY = 32
};

int ieee80211_radiotap_find_next_boundary(int current_byte_jump);

int ieee80211_radiotap_calculate_byte_jump(int frame_remaining_bytes, int field_alignment, int field_size);

void ieee80211_radiotap_initialize_align_size(struct ieee80211_radiotap_align_size radiotap_align_size[23]);

byte_t* ieee80211_radiotap_header_find_field(enum ieee80211_radiotap_presence field_index,byte_t* frame_bytes);


#endif