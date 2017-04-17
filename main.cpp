#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>
#include <string>
#include <sstream>

static const std::string devFs = "/dev/spidev";
static uint8_t mode = 3;
static uint8_t bits = 8;
static uint32_t speed = 1000000;
static uint16_t delay;

static int spi_open(unsigned int bus, unsigned int dev){
    int ret, fd;
    
    std::stringstream ss;
    
    ss << devFs << bus << "." << dev;
    std::string tmp = ss.str();

	fd = ::open(tmp.c_str(), O_RDWR);
	if (fd < 0){
		::exit(EXIT_FAILURE);
    }

	/*
	 * spi mode
	 */
	ret = ::ioctl(fd, SPI_IOC_WR_MODE, &mode);
	if (ret == -1){
		::exit(EXIT_FAILURE);
    }

	ret = ::ioctl(fd, SPI_IOC_RD_MODE, &mode);
	if (ret == -1){
		::exit(EXIT_FAILURE);
    }

	/*
	 * bits per word
	 */
	ret = ::ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &bits);
	if (ret == -1){
		::exit(EXIT_FAILURE);
	}

	ret = ::ioctl(fd, SPI_IOC_RD_BITS_PER_WORD, &bits);
	if (ret == -1){
		::exit(EXIT_FAILURE);
	}

	/*
	 * max speed hz
	 */
	ret = ::ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed);
	if (ret == -1){
		::exit(EXIT_FAILURE);
	}

	ret = ::ioctl(fd, SPI_IOC_RD_MAX_SPEED_HZ, &speed);
	if (ret == -1){
		::exit(EXIT_FAILURE);
	}


	return fd;
}

static void spi_close(int fd)
{
    close(fd);
}

static int spi_transfer(int fd, char *tx, char *rx, int len)
{
	int ret;
	
	struct spi_ioc_transfer tr;
	tr.tx_buf = (unsigned long)tx,
	tr.rx_buf = (unsigned long)rx,
	tr.len = len,
	tr.delay_usecs = delay,
	tr.speed_hz = 0,
	tr.bits_per_word = 0,

	ret = ::ioctl(fd, SPI_IOC_MESSAGE(1), &tr);
	if (ret == 1){
		::exit(EXIT_FAILURE);
    }
		
    return ret;
}

int main(void)
{
    int ret = 0;
    int fd;
    
    delay = 0;
    
    fd = spi_open(0, 0);
    
	

	return ret;
}
