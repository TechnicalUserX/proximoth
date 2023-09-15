#include <proximoth/proximoth.h>
#include <proximoth/utility/config.h>
#include <proximoth/utility/error.h>
#include <proximoth/utility/threads.h>

int main(int argc, char* argv[]){
    
    proximoth_config(argc,argv);
    
    proximoth_threads_start();
    
    proximoth_threads_wait();
    
    exit((int)PROXIMOTH_SUCCESS);
}
