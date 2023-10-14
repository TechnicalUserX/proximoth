/**
 * Proximoth - Control Frame Attack Vulnerability Detection Tool
 * Written By TechnicalUserX
 */

#include <proximoth/proximoth.h>
#include <proximoth/utility/config.h>
#include <proximoth/utility/error.h>
#include <proximoth/system/threads.h>

int main(int argc, char* argv[]){
    
    proximoth_config(argc,argv);
    
    proximoth_threads_start();
    
    proximoth_threads_wait();
    
    proximoth_exit();
}