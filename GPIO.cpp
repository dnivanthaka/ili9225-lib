#include <iostream>
#include <vector>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <stdint.h>


#include "GPIO.h"

GPIO::GPIO()
{

}

GPIO::~GPIO()
{

}

int GPIO::setup(uint8_t pin, GPIO_DIRECTION mode)
{
    int fd;
    char buff[BUFF_SIZE];
    ssize_t bytes_written;
    
    //Check if its already open, reuse exported pin
    ::snprintf(buff, BUFF_SIZE, "/sys/class/gpio%d/direction", pin);
    if(::open(buff, O_RDONLY) < 0){
    
        if(ENOENT == errno){
    
            fd = ::open("/sys/class/gpio/export", O_WRONLY);
            if(fd < 0){
                // Log the error here
                ::exit(EXIT_FAILURE);
            }
            
            bytes_written = ::snprintf(buff, BUFF_SIZE, "%d", pin);
            
            if(::write(fd, buff, bytes_written) < 0){
                // Log the error here
                ::exit(EXIT_FAILURE);
            }
            
            ::close(fd);
        }
    }
    
    //Setting mode
    ::sprintf(buff, "/sys/class/gpio/gpio%d/direction", pin);
    
    fd = ::open(buff, O_WRONLY);
    
    if(fd < 0){
        // Log the error here
        ::exit(EXIT_FAILURE);
    }
    
    if(mode == OUTPUT){
        bytes_written = ::snprintf(buff, BUFF_SIZE, "out");
    }else{
        bytes_written = ::snprintf(buff, BUFF_SIZE, "in");
    }
    
    if(::write(fd, buff, bytes_written) < 0){
        // Log the error here
        ::exit(EXIT_FAILURE);
    }
    
    ::close(fd);

    return 0;
}

int GPIO::cleanup(uint8_t pin)
{
    int fd;
    char buff[BUFF_SIZE];
    ssize_t bytes_written;
    
    fd = ::open("/sys/class/gpio/unexport", O_WRONLY);
    if(fd < 0){
        // Log the error here
        ::exit(EXIT_FAILURE);
    }
    
    bytes_written = ::snprintf(buff, BUFF_SIZE, "%d", pin);
    
    if(::write(fd, buff, bytes_written) < 0){
        // Log the error here
        ::exit(EXIT_FAILURE);
    }
    
    ::close(fd);

    return 0;
}

int GPIO::write(uint8_t pin, uint8_t value)
{
    int fd;
    char buff[BUFF_SIZE];
    char value_str[3];

    ::snprintf(buff, BUFF_SIZE, "/sys/class/gpio/gpio%d/value", pin);
    
	fd = ::open(buff, O_WRONLY);
	if (fd < 0) {
		// Log the error here
        ::exit(EXIT_FAILURE);
	}
	
	if(value == 1){
        ::snprintf(buff, 1, "1");
    }else{
        ::snprintf(buff, 1, "0");
    }
 
	if (::write(fd, buff, 1) < 0) {
		// Log the error here
        ::exit(EXIT_FAILURE);
	}
 
	::close(fd);
 
    return 0;
}

int GPIO::read(uint8_t pin)
{
    int fd;
    char buff[BUFF_SIZE];
    char value_str[3];

    ::snprintf(buff, BUFF_SIZE, "/sys/class/gpio/gpio%d/value", pin);
    
	fd = ::open(buff, O_RDONLY);
	if (fd < 0) {
		// Log the error here
        ::exit(EXIT_FAILURE);
	}
 
	if (::read(fd, value_str, 3) < 0) {
		// Log the error here
        ::exit(EXIT_FAILURE);
	}
 
	::close(fd);
 
    return(::atoi(value_str));
}
