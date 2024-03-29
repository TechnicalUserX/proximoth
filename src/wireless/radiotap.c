#include <proximoth/wireless/radiotap.h>

byte_t proximoth_radiotap_template[8] = {0x00, 0x00, 0x08,0x00, 0x00, 0x00, 0x00, 0x00};

struct proximoth_radiotap_align_size proximoth_radiotap_namespaces[23] = {0};

int proximoth_radiotap_find_next_boundary(int current_byte_jump){

    int boundary = current_byte_jump + 1;

    while(boundary % 8 != 0){
        boundary++;
    }

    return boundary;
}

int proximoth_radiotap_calculate_byte_jump(int frame_remaining_bytes, int field_alignment, int field_size){

    switch(frame_remaining_bytes){
        case 8:{ // O O O O O O O O O
            return field_size;
        }
        break;

        case 7:{ // X O O O O O O O
            switch(field_alignment){
                case 1:
                    return field_size;
                case 2:
                    return field_size+1;
                case 4:
                    return field_size+3;
                case 8:
                    return field_size+7;
            }

        }
        break;

        case 6:{ // X X O O O O O O 
            switch(field_alignment){
                case 1:
                case 2:
                    return field_size;
                case 4:
                    return field_size+2;
                case 8:
                    return field_size+6;
            }
        }
        break;

        case 5:{ // X X X O O O O O
            switch(field_alignment){
                case 1:
                    return field_size;
                case 2:
                case 4:
                    return field_size+1;
                case 8:
                    return field_size+5;
            }
        }
        break;

        case 4:{ // X X X X O O O O 
            switch(field_alignment){
                case 1:
                case 2:
                case 4:
                    return field_size;
                case 8:
                    return field_size+4;
            }
        }
        break;

        case 3:{ // X X X X X O O O 
            switch(field_alignment){
                case 1:
                    return field_size;
                case 2:
                    return field_size+1;
                case 4:
                case 8:
                    return field_size+3;
            }
        }
        break;

        case 2:{ // X X X X X X O O
            switch(field_alignment){
                case 1:
                case 2:
                    return field_size;
                case 4:
                case 8:
                    return field_size+2;
                
            }

        }
        break;

        case 1:{
            switch(field_alignment){
                case 1:
                    return field_size;
                case 2:
                case 4:
                case 8:
                    return field_size+1;
            }
        }
        break;

    }

    return 0;
}

void proximoth_radiotap_initialize_align_size(struct proximoth_radiotap_align_size radiotap_align_size[23]){
	radiotap_align_size[PROXIMOTH_RADIOTAP_TSFT]                = (struct proximoth_radiotap_align_size){ .align = 8, .size = 8 };
	radiotap_align_size[PROXIMOTH_RADIOTAP_FLAGS]               = (struct proximoth_radiotap_align_size){ .align = 1, .size = 1, };
	radiotap_align_size[PROXIMOTH_RADIOTAP_RATE]                = (struct proximoth_radiotap_align_size){ .align = 1, .size = 1, };
	radiotap_align_size[PROXIMOTH_RADIOTAP_CHANNEL]             = (struct proximoth_radiotap_align_size){ .align = 2, .size = 4, };
	radiotap_align_size[PROXIMOTH_RADIOTAP_FHSS]                = (struct proximoth_radiotap_align_size){ .align = 2, .size = 2, };
	radiotap_align_size[PROXIMOTH_RADIOTAP_DBM_ANTSIGNAL]       = (struct proximoth_radiotap_align_size){ .align = 1, .size = 1, };
	radiotap_align_size[PROXIMOTH_RADIOTAP_DBM_ANTNOISE]        = (struct proximoth_radiotap_align_size){ .align = 1, .size = 1, };
	radiotap_align_size[PROXIMOTH_RADIOTAP_LOCK_QUALITY]        = (struct proximoth_radiotap_align_size){ .align = 2, .size = 2, };
	radiotap_align_size[PROXIMOTH_RADIOTAP_TX_ATTENUATION]      = (struct proximoth_radiotap_align_size){ .align = 2, .size = 2, };
	radiotap_align_size[PROXIMOTH_RADIOTAP_DB_TX_ATTENUATION]   = (struct proximoth_radiotap_align_size){ .align = 2, .size = 2, };
	radiotap_align_size[PROXIMOTH_RADIOTAP_DBM_TX_POWER]        = (struct proximoth_radiotap_align_size){ .align = 1, .size = 1, };
	radiotap_align_size[PROXIMOTH_RADIOTAP_ANTENNA]             = (struct proximoth_radiotap_align_size){ .align = 1, .size = 1, };
	radiotap_align_size[PROXIMOTH_RADIOTAP_DB_ANTSIGNAL]        = (struct proximoth_radiotap_align_size){ .align = 1, .size = 1, };
	radiotap_align_size[PROXIMOTH_RADIOTAP_DB_ANTNOISE]         = (struct proximoth_radiotap_align_size){ .align = 1, .size = 1, };
	radiotap_align_size[PROXIMOTH_RADIOTAP_RX_FLAGS]            = (struct proximoth_radiotap_align_size){ .align = 2, .size = 2, };
	radiotap_align_size[PROXIMOTH_RADIOTAP_TX_FLAGS]            = (struct proximoth_radiotap_align_size){ .align = 2, .size = 2, };
	radiotap_align_size[PROXIMOTH_RADIOTAP_RTS_RETRIES]         = (struct proximoth_radiotap_align_size){ .align = 1, .size = 1, };
	radiotap_align_size[PROXIMOTH_RADIOTAP_DATA_RETRIES]        = (struct proximoth_radiotap_align_size){ .align = 1, .size = 1, };
	radiotap_align_size[PROXIMOTH_RADIOTAP_MCS]                 = (struct proximoth_radiotap_align_size){ .align = 1, .size = 3, }; // 1-1-1
	radiotap_align_size[PROXIMOTH_RADIOTAP_AMPDU_STATUS]        = (struct proximoth_radiotap_align_size){ .align = 4, .size = 8, };
	radiotap_align_size[PROXIMOTH_RADIOTAP_VHT]                 = (struct proximoth_radiotap_align_size){ .align = 2, .size = 12, };
	radiotap_align_size[PROXIMOTH_RADIOTAP_TIMESTAMP]           = (struct proximoth_radiotap_align_size){ .align = 8, .size = 12, }; // 8-2-1-1


}

byte_t* proximoth_radiotap_header_find_field(enum proximoth_radiotap_presence field_index,byte_t* frame_bytes){

    static byte_t* bytes = NULL;

    // If first usage of this function includes a NULL pointer to frame bytes, there is no point to proceed.
    if(bytes == frame_bytes && frame_bytes == NULL){
        return NULL;
    }

    static enum proximoth_radiotap_presence wanted_field = PROXIMOTH_RADIOTAP_DUMMY;
    static size_t wanted_occurence = 1;
    static size_t seen_occurence = 0;

    if(frame_bytes != NULL && bytes != frame_bytes){
        bytes = frame_bytes;
        wanted_occurence = 1;
        wanted_field = field_index;
    }
    else if(frame_bytes == bytes){
        wanted_occurence = 1;
        wanted_field = field_index;
    }
    else if(frame_bytes == NULL && wanted_field == field_index){
        wanted_occurence++;
    }
    else if(frame_bytes == NULL && wanted_field != field_index){
        wanted_occurence = 1;
        wanted_field = field_index;
    }


    size_t present_flags_field_count = 1;
    uint32_t temporary_present_flags = *((uint32_t*)( (bytes+4) + (4*(present_flags_field_count-1)) ));

    while(1){

        if(temporary_present_flags & 1<<PROXIMOTH_RADIOTAP_EXT){
            present_flags_field_count++;
            temporary_present_flags = *((uint32_t*)((bytes+4)+( 4*(present_flags_field_count-1) )));
        }else{
            break;
        }

    }

    int total_byte_jump = (4 + 4*present_flags_field_count);

    seen_occurence = 0;
    size_t present_flags_index = 0;
    uint32_t present_flags = *(   (uint32_t*)(    (bytes+4) + (4*present_flags_index)  )   );

    // Location detection of wanted field...
    int field_index_counter = 0;
    int bit_mask = 1;        

    int search_field = 1<<wanted_field;
    int current_field_size = 0;
    int current_field_alignment = 0;


    while(1){

        if(bit_mask & present_flags){

            if(bit_mask & search_field)
                seen_occurence++;

            if(( (bit_mask & search_field) && (seen_occurence == wanted_occurence) ))
                break;

            current_field_size = proximoth_radiotap_namespaces[field_index_counter].size;
            current_field_alignment = proximoth_radiotap_namespaces[field_index_counter].align;
            int remaining_bytes_for_alignment = proximoth_radiotap_find_next_boundary(total_byte_jump) - total_byte_jump;
            int current_byte_jump = proximoth_radiotap_calculate_byte_jump(remaining_bytes_for_alignment,current_field_alignment,current_field_size);
            total_byte_jump += current_byte_jump;

        }

        field_index_counter++;
        bit_mask <<= 1;

        if( (field_index_counter == 31 ) && (bit_mask & present_flags) && (bit_mask & 1<<PROXIMOTH_RADIOTAP_EXT)){
            // This means there are STILL flags and there are STILL data!
            present_flags_index++;
            present_flags = *((uint32_t*)((bytes+4)+(4*present_flags_index)));
            bit_mask = 1;
            field_index_counter = 0;
            // Going into another loop with new present_flags...
            continue;
        }
        
        if(field_index_counter == 31 && !(bit_mask & present_flags) ){
            // Couldn't find!
            return NULL;
        }
        
    } 
    return bytes + total_byte_jump;
}