#include <proximoth/utility/error.h>

char proximoth_error_buffer[256] = {0};

proximoth_error_t proximoth_error;

const char* proximoth_error_get_message(proximoth_error_t error_code){

    switch(error_code){

        case PROXIMOTH_SUCCESS:
            return "Success.";
        case PROXIMOTH_ERROR_INTERFACE_NOT_FOUND:
            return "Specified interface not found.";
        case PROXIMOTH_ERROR_INTERFACE_NO_MONITOR:
            return "Interface is not in monitor mode.";
        case PROXIMOTH_ERROR_INTERFACE_CANNOT_OPEN:
            return "Cannot open interface, are you root?";
        case PROXIMOTH_ERROR_INTERFACE_CANNOT_GET_STATE:
            return "Cannot retrieve the state of the interface.";
        case PROXIMOTH_ERROR_INTERFACE_CANNOT_GET_MODE:
            return "Cannot retrieve the operating mode of the interface.";
        case PROXIMOTH_ERROR_INTERFACE_CANNOT_GET_CHANNEL:
            return "Cannot retrieve the channel of the interface.";
        case PROXIMOTH_ERROR_INTERFACE_NOT_UP:
            return "Interface is not up.";
        case PROXIMOTH_ERROR_TARGET_MAC_INVALID:
            return "Provided MAC address is invalid.";
        case PROXIMOTH_ERROR_FILE_CANNOT_CREATE:
            return "Cannot create output file.";
        case PROXIMOTH_ERROR_THREADS_CANNOT_START:
            return "Couldn't start the threads.";
        case PROXIMOTH_ERROR_BSSID_MAC_INVALID:
            return "Invalid BSSID MAC format.";
        case PROXIMOTH_ERROR_OPTION_INVALID:
            return "Option is invalid.";
        case PROXIMOTH_ERROR_OPTION_NEEDS_ARG:
            return "Option needs an argument.";
        case PROXIMOTH_ERROR_ARGUMENT_INVALID:
            return "Argument is invalid.";
        case PROXIMOTH_ERROR_INTERFACE_NOT_SPECIFIED:
            return "An interface must be specified.";
        case PROXIMOTH_ERROR_PROCESS_CHILD_BAD_KILL:
            return "Child process cannot be terminated.";
        case PROXIMOTH_ERROR_THREADS_BAD_JOIN:
            return "A thread could not be joined.";
        case PROXIMOTH_ERROR_PCAP_FILE_CANNOT_CREATE:
            return "PCAP file cannot be created.";
        case PROXIMOTH_ERROR_SIGNAL_BAD_INIT:
            return "sigaction() error.";
        case PROXIMOTH_ERROR_IOCTL:
            return "ioctl() error.";
        case PROXIMOTH_ERROR_SEMAPHORE_CANNOT_INIT:
            return "Cannot initialize semaphore.";
        case PROXIMOTH_ERROR_SOCKET_CANNOT_CREATE:
            return "Cannot create socket.";
        case PROXIMOTH_ERROR_INJECT:
            return "Injection error.";
        case PROXIMOTH_ERROR_SNIFF:
            return "Sniffing error.";
        case PROXIMOTH_ERROR_ATEXIT:
            return "atexit() cannot be set.";
        case PROXIMOTH_ERROR_LOCALE:
            return "Cannot set locale.";
        case PROXIMOTH_ERROR_LOCALTIME:
            return "localtime() error.";
        default:
            return "Unknown error.";
    }
}
