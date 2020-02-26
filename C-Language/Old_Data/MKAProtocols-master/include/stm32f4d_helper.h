// definition of registers' addresses
#define RCCBASE 0x40023800
#define RCC_CR (RCCBASE+0x00)
#define RCC_PLLCFGR (RCCBASE+0x04)
#define RCC_CFGR (RCCBASE+0x08)
#define RCC_AHB1ENR (RCCBASE+0x30)
#define RCC_APB1ENR (RCCBASE+0x40)
#define RCC_AHB2ENR (RCCBASE+0x34)
#define RCC_APB2ENR (RCCBASE+0x44)

// Flash memory
#define FLASH_ACR 0x40023C00
// GPIO A
#define GPIOA_BASE 0x40020000
#define GPIOA_MODER (GPIOA_BASE+0x00)
#define GPIOA_AFRL (GPIOA_BASE+0x20)
#define GPIOA_OTYPER (GPIOA_BASE+0x04)
#define GPIOA_AFRL (GPIOA_BASE+0x20)
#define GPIOA_AFRH (GPIOA_BASE+0x24)
#define GPIOA_OTYPER (GPIOA_BASE+0x04)
#define GPIOA_OSPEED (GPIOA_BASE+0x08)
#define GPIOA_PUPDR (GPIOA_BASE+0x0C)
#define GPIOA_BSRR (GPIOA_BASE+0x18)
#define GPIOA_IDR (GPIOA_BASE+0x10)
// GPIO B
#define GPIOB_BASE 0x40020400
#define GPIOB_MODER (GPIOB_BASE+0x00)
#define GPIOB_AFRL (GPIOB_BASE+0x20)
#define GPIOB_AFRH (GPIOB_BASE+0x24)
#define GPIOB_OTYPER (GPIOB_BASE+0x04)
#define GPIOB_OSPEED (GPIOB_BASE+0x08)
#define GPIOB_PUPDR (GPIOB_BASE+0x0C)
#define GPIOB_BSRR (GPIOB_BASE+0x18)
// GPIO C
#define GPIOC_BASE 0x40020800
#define GPIOC_MODER (GPIOC_BASE+0x00)
#define GPIOC_AFRL (GPIOC_BASE+0x20)
#define GPIOC_AFRH (GPIOC_BASE+0x24)
#define GPIOC_OTYPER (GPIOC_BASE+0x04)
#define GPIOC_OSPEED (GPIOC_BASE+0x08)
#define GPIOC_PUPDR (GPIOC_BASE+0x0C)
#define GPIOC_BSRR (GPIOC_BASE+0x18)
// GPIO D
#define GPIOD_BASE 0x40020C00
#define GPIOD_MODER (GPIOD_BASE+0x00)
#define GPIOD_AFRL (GPIOD_BASE+0x20)
#define GPIOD_AFRH (GPIOD_BASE+0x24)
#define GPIOD_OTYPER (GPIOD_BASE+0x04)
#define GPIOD_OSPEED (GPIOD_BASE+0x08)
#define GPIOD_PUPDR (GPIOD_BASE+0x0C)
#define GPIOD_BSRR (GPIOD_BASE+0x18)
// USART 1
#define USART1_BASE 0x40011000
#define USART1_SR (USART1_BASE+0x00)
#define USART1_DR (USART1_BASE+0x04)
#define USART1_BRR (USART1_BASE+0x08)
#define USART1_CR1 (USART1_BASE+0x0C)
#define USART1_CR2 (USART1_BASE+0x10)
#define USART1_CR3 (USART1_BASE+0x14)
#define USART1_GTPR (USART1_BASE+0x18)
// UART 6
#define USART6_BASE 0x40011400
#define USART6_SR (USART6_BASE+0x00)
#define USART6_DR (USART6_BASE+0x04)
#define USART6_BRR (USART6_BASE+0x08)
#define USART6_CR1 (USART6_BASE+0x0C)
#define USART6_CR2 (USART6_BASE+0x10)
#define USART6_CR3 (USART6_BASE+0x14)
#define USART6_GTPR (USART6_BASE+0x18)

// --- RNG only for stm32f4d07 ---
#define RNG_BASE 0x50060800
#define RNG_CR (RNG_BASE+0x00)
#define RNG_SR (RNG_BASE+0x04)
#define RNG_DR (RNG_BASE+0x08)
// --- only for stm32f4d07 ---

// GPIOs 
# define OFF       -1
# define ON        0
# define PORTA     1 
# define PORTB     2
# define PORTC     3
# define PORTD     4

// CLOCK SPEED 
# define CLOCK8   (2<<24)|(1<<22)|(3<<16)|(64<<6)|(8<<0)
# define CLOCK16  (2<<24)|(1<<22)|(2<<16)|(48<<6)|(4<<0)
# define CLOCK32  (4<<24)|(1<<22)|(2<<16)|(96<<6)|(4<<0)
# define CLOCK42  (7<<24)|(1<<22)|(3<<16)|(168<<6)|(4<<0)
# define CLOCK64  (8<<24)|(1<<22)|(2<<16)|(192<<6)|(4<<0)
# define CLOCK120 (5<<24)|(1<<22)|(((2>>1)-1)<<16)|(120<<6)|(4<<0)
# define CLOCK168 (7<<24)|(1<<22)|(((2>>1)-1)<<16)|(210<<6)|(5<<0)

// UART SPEED
# define UART9600  9600
# define UART19200 19200
 
#define UART_BUF_MAX_DIM 1000

# define UART1 1
# define UART6 6

extern volatile int reader;
extern volatile int writer;
extern volatile int counter;
extern volatile int uart_buf[UART_BUF_MAX_DIM];

void handler_reset();

// initialize the clock of the board: 8MHz HSE, 168MHz pllgen 48MHz pll usb
// specific to stm32fd07, doesn't work on stm32f4d01
void clock_init (void);

// initialize random number generator
// RNG only present on stm32f4d07
void rnd_init (void);

// initialize the PD12,PD13,PD14,PD15 (LEDs) and PB1,PB11,PA7,PC1
void init(void);

void init_gpio_output(int port,int number);

// init uart peripheral
int uart_init ();

// random function to be added as an entropy source
// generates 4 random bytes through hardware RNG and store them in output
// in case of error it returns POLARSSL_ERR_ENTROPY_SOURCE_FAILED 
//int random(void *data, unsigned char *output, unsigned int len, unsigned int *olen);

void turnGPIO(int port,int number,int mode);

// send one byte via uart
void uart_putc (unsigned int x,int uart);

// receive one byte via uart
unsigned int uart_getc (void);

// send len byes via uart
void uart_string (unsigned char *s,unsigned char len,int uart);

// receive len bytes via uart
void uart_getstring(unsigned char *buf,unsigned int len);

// --- IMPLEMENTATION OF NEEDED STANDARD FUNCITON OF THE LIBC ---
// Functions implemented so far: memcpy, strlen, memset, strcasecmp
void *memcpy(void *dest, const void *src, unsigned int n);
unsigned int strlen(const char *str);
void *memset(void *s, int c, unsigned int n);
int strcasecmp(const char *s1, const char *s2);
