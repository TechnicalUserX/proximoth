#include "../include/proximoth_radiotap.hpp"

byte_t ieee80211_radiotap_template[8] = {0x00, 0x00, 0x08,0x00, 0x00, 0x00, 0x00, 0x00};

struct ieee80211_radiotap_align_size rtap_namespace_sizes[23] = {0};

void ieee80211_radiotap_initialize_align_size(struct ieee80211_radiotap_align_size radiotap_align_size[23]){
	radiotap_align_size[IEEE80211_RADIOTAP_TSFT] = { .align = 8, .size = 8, };
	radiotap_align_size[IEEE80211_RADIOTAP_FLAGS] = { .align = 1, .size = 1, };
	radiotap_align_size[IEEE80211_RADIOTAP_RATE] = { .align = 1, .size = 1, };
	radiotap_align_size[IEEE80211_RADIOTAP_CHANNEL] = { .align = 2, .size = 4, };
	radiotap_align_size[IEEE80211_RADIOTAP_FHSS] = { .align = 2, .size = 2, };
	radiotap_align_size[IEEE80211_RADIOTAP_DBM_ANTSIGNAL] = { .align = 1, .size = 1, };
	radiotap_align_size[IEEE80211_RADIOTAP_DBM_ANTNOISE] = { .align = 1, .size = 1, };
	radiotap_align_size[IEEE80211_RADIOTAP_LOCK_QUALITY] = { .align = 2, .size = 2, };
	radiotap_align_size[IEEE80211_RADIOTAP_TX_ATTENUATION] = { .align = 2, .size = 2, };
	radiotap_align_size[IEEE80211_RADIOTAP_DB_TX_ATTENUATION] = { .align = 2, .size = 2, };
	radiotap_align_size[IEEE80211_RADIOTAP_DBM_TX_POWER] = { .align = 1, .size = 1, };
	radiotap_align_size[IEEE80211_RADIOTAP_ANTENNA] = { .align = 1, .size = 1, };
	radiotap_align_size[IEEE80211_RADIOTAP_DB_ANTSIGNAL] = { .align = 1, .size = 1, };
	radiotap_align_size[IEEE80211_RADIOTAP_DB_ANTNOISE] = { .align = 1, .size = 1, };
	radiotap_align_size[IEEE80211_RADIOTAP_RX_FLAGS] = { .align = 2, .size = 2, };
	radiotap_align_size[IEEE80211_RADIOTAP_TX_FLAGS] = { .align = 2, .size = 2, };
	radiotap_align_size[IEEE80211_RADIOTAP_RTS_RETRIES] = { .align = 1, .size = 1, };
	radiotap_align_size[IEEE80211_RADIOTAP_DATA_RETRIES] = { .align = 1, .size = 1, };
	radiotap_align_size[IEEE80211_RADIOTAP_MCS] = { .align = 1, .size = 3, };
	radiotap_align_size[IEEE80211_RADIOTAP_AMPDU_STATUS] = { .align = 4, .size = 8, };
	radiotap_align_size[IEEE80211_RADIOTAP_VHT] = { .align = 2, .size = 12, };
	radiotap_align_size[IEEE80211_RADIOTAP_TIMESTAMP] = { .align = 8, .size = 12, };


}

byte_t* ieee80211_radiotap_header_find_field(enum ieee80211_radiotap_presence field_index,byte_t* frame_bytes){

    static byte_t* bytes = NULL;

    // If first usage of this function includes a NULL pointer to frame bytes, there is no point to proceed.
    if(bytes == frame_bytes and frame_bytes == NULL){
        return NULL;
    }

    static enum ieee80211_radiotap_presence wanted_field = IEEE80211_RADIOTAP_DUMMY;
    static size_t wanted_occurence = 1;
    static size_t seen_occurence = 0;


    if(frame_bytes != NULL and bytes != frame_bytes){
        bytes = frame_bytes;
        wanted_occurence = 1;
        wanted_field = field_index;
    }
    else if(frame_bytes == bytes){
        wanted_occurence = 1;
        wanted_field = field_index;
    }
    else if(frame_bytes == NULL and wanted_field == field_index){
        wanted_occurence++;
    }
    else if(frame_bytes == NULL and wanted_field != field_index){
        wanted_occurence = 1;
        wanted_field = field_index;
    }


    size_t present_flags_field_count = 1;
    uint32_t temporary_present_flags = *((uint32_t*)((bytes+4)+( 4*(present_flags_field_count-1) )));

    while(1){

        if(temporary_present_flags & 1<<IEEE80211_RADIOTAP_EXT){
            present_flags_field_count++;
            temporary_present_flags = *((uint32_t*)((bytes+4)+( 4*(present_flags_field_count-1) )));
        }else{
            break;
        }

    }


    seen_occurence = 0;
    size_t present_flags_index = 0;
    uint32_t present_flags = *(   (uint32_t*)(    (bytes+4) + (4*present_flags_index)  )   );
    // Location detection of wanted field...
    int index_counter = 0;
    int bit_mask = 0b1;        
    int total_byte_jump = 0;

    int search_field = 1<<wanted_field;

    int previous_byte_jump = 0;

    while(1){
        if((bit_mask & present_flags)){

            if(bit_mask & search_field)
                seen_occurence++;


            if(( (bit_mask & search_field) and (seen_occurence == wanted_occurence) ))
                break;



            total_byte_jump += rtap_namespace_sizes[index_counter].size;
            if((total_byte_jump % 2 == 1) && (rtap_namespace_sizes[index_counter].size % 2 == 0)){
                total_byte_jump += 1;
            }

            previous_byte_jump = rtap_namespace_sizes[index_counter].size;

        }




        index_counter++;
        bit_mask <<= 1;

        if( (index_counter == 31 ) and (bit_mask & present_flags) and (bit_mask & 1<<IEEE80211_RADIOTAP_EXT)){
            // This means there are STILL flags and there are STILL data!
            present_flags_index++;
            present_flags = *((uint32_t*)((bytes+4)+(4*present_flags_index)));
            bit_mask = 0b1;
            index_counter = 0;
            // Going into another loop with new present_flags...
            continue;
        }
        
        if(index_counter == 31 and !(bit_mask & present_flags) ){
            // Couldn't find!
            return NULL;
        }
        


    } 

    return bytes + total_byte_jump + 4 + present_flags_field_count*4;
}
