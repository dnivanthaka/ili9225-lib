#ifndef GPIO_H
#define GPIO_H

#define GPIO_FS "/sys/class/gpio"
#define BUFF_SIZE 65

typedef struct{
    uint8_t pin;
    uint8_t mode;
    uint8_t val;
}gpio_t;

enum GPIO_DIRECTION {
    OUTPUT = 0, INPUT = 1
};

class GPIO{
    public:
        GPIO();
        ~GPIO();
        int setup(uint8_t pin, GPIO_DIRECTION mode);
        int cleanup(uint8_t pin);
        int write(uint8_t pin, uint8_t value);
        int read(uint8_t pin);
        
    private:
        std::vector<gpio_t> gpioList;

};
#endif
