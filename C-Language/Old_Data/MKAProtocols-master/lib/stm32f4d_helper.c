# include "stm32f4d_helper.h"
# include "polarssl/entropy.h"

# define TRAS_SPEED UART19200
# define SPEED CLOCK16

void PUT32 ( unsigned int, unsigned int );
void PUT16 ( unsigned int, unsigned int );
void PUT8 ( unsigned int, unsigned int );
unsigned int GET32 ( unsigned int );
unsigned int GET16 ( unsigned int );

extern unsigned long _data_flash;
extern unsigned long _data_begin;
extern unsigned long _data_end;
extern unsigned long _bss_begin;
extern unsigned long _bss_end;
extern unsigned long _stack_end;

volatile int counter = 0;
volatile int writer = 0; 
volatile int reader = 0;
volatile int uart_buf[UART_BUF_MAX_DIM];

void handler_reset(void)
{
    unsigned long *source;
    unsigned long *destination;

    // Copying data from Flash to RAM
    source = &_data_flash;
    for (destination = &_data_begin; destination < &_data_end;)
    {
        *(destination++) = *(source++);
    }   
    // default zero to undefined variables
    for (destination = &_bss_begin; destination < &_bss_end;)
    {
        *(destination++) = 0;
    }

	// init clock, leds, rng, uart
	clock_init();
	init();
	rnd_init();
	uart_init(); 
}

void clock_init() {
    unsigned int ra;

    //enable HSE
    ra=GET32(RCC_CR);
    ra&=~(0xF<<16);
    PUT32(RCC_CR,ra);
    ra|=1<<16;
    PUT32(RCC_CR,ra);
    while(1)
    {
        if(GET32(RCC_CR)&(1<<17)) break;
    }
	switch(SPEED) {
		case CLOCK64:
			PUT32(RCC_CFGR,0x00001001);
			break;
		case CLOCK120:
			PUT32(RCC_CFGR,0x00009401);
			break;
		case CLOCK168:
			PUT32(RCC_CFGR,0x0000D801);
			break;
    	default: 
			PUT32(RCC_CFGR,0x00000001); // system clock = hse
			break;
	}
	
    //slow flash accesses down otherwise it will crash
	if ((SPEED) == (CLOCK168))
   		PUT32(FLASH_ACR,0x00000105);
	else if ((SPEED) == (CLOCK120))
		PUT32(FLASH_ACR,0x00000103);
	else if ((SPEED) == (CLOCK64))
		PUT32(FLASH_ACR,0x00000102);
	else if (((SPEED) == (CLOCK32)) || ((SPEED) == (CLOCK42)))
		PUT32(FLASH_ACR,0x00000101);
	else if (((SPEED) == (CLOCK8)) || ((SPEED) == (CLOCK16)))
		PUT32(FLASH_ACR,0x00000100);

	// setup pll: PLLvco = CLKIN*(PLLN/PLLM), PLLout = PLLvco/PLLP, PLLrng = PLLvco/PLLQ
    PUT32(RCC_PLLCFGR,SPEED);

	// enable pll
	ra=GET32(RCC_CR);
  	ra|=(1<<24);
    PUT32(RCC_CR,ra);
	//wait for pll lock
    while(1)
    {
        if(GET32(RCC_CR)&(1<<25)) break;
    }
	
	switch(SPEED) {
		case CLOCK64:
			PUT32(RCC_CFGR,0x00001002);
			break;
		case CLOCK120:
			PUT32(RCC_CFGR,0x00009402);
			break;
		case CLOCK168:
			PUT32(RCC_CFGR,0x0000D802);
			break;
    	default: 
			PUT32(RCC_CFGR,0x00000002); // system clock = pll
			break;
	}

    //if you didnt set the flash wait states you may crash here

	//wait for it to use the pll
    while(1)
    {
        if((GET32(RCC_CFGR)&0xC) == 0x8) break;
    }
 }

// initialize random number generator
// RNG only present on stm32f4d07
void rnd_init ( void )
{
    unsigned int ra;

    ra=GET32(RCC_AHB2ENR);
    ra|=1<<6;
    PUT32(RCC_AHB2ENR,ra);

    ra=GET32(RNG_CR);
    ra|=4;
    PUT32(RNG_CR,ra);
}

void init() {
	unsigned int ra;

	ra = GET32(RCC_AHB1ENR);
    ra |= ((1<<0)|(1<<1)|(1<<2)|(1<<3)); //enable ports A,B, C and D
    PUT32(RCC_AHB1ENR,ra);

    init_gpio_output(PORTD,12);
	init_gpio_output(PORTD,13);
	init_gpio_output(PORTD,14);
	init_gpio_output(PORTD,15);

	//init_gpio_output(PORTA,6);
	init_gpio_output(PORTB,0);
	init_gpio_output(PORTB,1);
	init_gpio_output(PORTB,11);
	init_gpio_output(PORTC,1);
}

void init_gpio_output(int port,int number) {
	unsigned int ra;

	if (port == PORTA) {
		ra = GET32(GPIOA_MODER);
    	ra &= ~(3<<(number*2));
    	ra |= (1<<(number*2));
    	PUT32(GPIOA_MODER,ra);

		ra = GET32(GPIOA_OTYPER);
    	ra &= ~(1<<number);
		PUT32(GPIOA_OTYPER,ra);
	} else if (port == PORTB) {
		ra = GET32(GPIOB_MODER);
    	ra &= ~(3<<(number*2));
    	ra |= (1<<(number*2));
    	PUT32(GPIOB_MODER,ra);

		ra = GET32(GPIOB_OTYPER);
    	ra &= ~(1<<number);
		PUT32(GPIOB_OTYPER,ra);
	} else if (port == PORTC) {
		ra = GET32(GPIOC_MODER);
    	ra &= ~(3<<(number*2));
    	ra |= (1<<(number*2));
    	PUT32(GPIOC_MODER,ra);
		
		ra = GET32(GPIOC_OTYPER);
    	ra &= ~(1<<number);
		PUT32(GPIOC_OTYPER,ra);
	} else if (port == PORTD) {
		ra = GET32(GPIOD_MODER);
    	ra &= ~(3<<(number*2));
    	ra |= (1<<(number*2));
    	PUT32(GPIOD_MODER,ra);
			
		ra = GET32(GPIOD_OTYPER);
    	ra &= ~(1<<number);
		PUT32(GPIOD_OTYPER,ra);
	}	
}

void uart1_init() {
	unsigned int ra;

	ra=GET32(RCC_AHB1ENR);
    ra|=1<<1; //enable port B
    PUT32(RCC_AHB1ENR,ra);

	ra=GET32(RCC_APB2ENR);
    ra|=1<<4; //enable USART1
    PUT32(RCC_APB2ENR,ra);

    //PB6 USART1_TX
    //PB7 USART1_RX
	
	ra=GET32(GPIOB_MODER);
    ra|= (2<<12);
    ra|= (2<<14);
    PUT32(GPIOB_MODER,ra);
    ra=GET32(GPIOB_OTYPER);
    ra&=(1<<6);
    ra&=(1<<7);
    PUT32(GPIOB_OTYPER,ra);
    ra=GET32(GPIOB_AFRL);
    ra|=(7<<24);
    ra|=(7<<28);
    PUT32(GPIOB_AFRL,ra);

// Baud rate from clock frequency: BR = Fck/(16*USARTDIV)
// USARTDIV = Fck/(BR*16)
    switch(SPEED) {
		case CLOCK8:
			if (TRAS_SPEED == UART9600)
				PUT32(USART1_BRR,(52<<4)|(1<<0));
			else if (TRAS_SPEED == UART19200)
				PUT32(USART1_BRR,(26<<4)|(0<<0));
			break;
		case CLOCK16:
			if (TRAS_SPEED == UART9600)
				PUT32(USART1_BRR,(104<<4)|(2<<0));
			else if (TRAS_SPEED == UART19200)
				PUT32(USART1_BRR,(52<<4)|(1<<0));
			break;
		case CLOCK32:
			if (TRAS_SPEED == UART9600)
				PUT32(USART1_BRR,(208<<4)|(5<<0));
			else if (TRAS_SPEED == UART19200)
				PUT32(USART1_BRR,(104<<4)|(2<<0));
			break;
		case CLOCK42:
			if (TRAS_SPEED == UART9600)
				PUT32(USART1_BRR,(273<<4)|(7<<0));
			else if (TRAS_SPEED == UART19200)
				PUT32(USART1_BRR,(136<<4)|(11<<0));
			break;
		case CLOCK64:
			if (TRAS_SPEED == UART9600)
				PUT32(USART1_BRR,(416<<4)|(10<<0));
			else if (TRAS_SPEED == UART19200)
				PUT32(USART1_BRR,(208<<4)|(5<<0));
			break;
		case CLOCK120:
			if (TRAS_SPEED == UART9600)
				PUT32(USART1_BRR,(390<<4)|(10<<0));
			else if (TRAS_SPEED == UART19200)
				PUT32(USART1_BRR,(195<<4)|(5<<0));
			break;
		case CLOCK168:
			if (TRAS_SPEED == UART9600)
				PUT32(USART1_BRR,(136<<4)|(11<<0));
			else if (TRAS_SPEED == UART19200)
				PUT32(USART1_BRR,(68<<4)|(5<<0));
			break;
	}
	PUT32(USART1_CR1,(1<<13)|(1<<3)|(1<<2)|(1<<5));
	PUT32(0xE000E104,0x00000020); // enable USART1 interrupt
}

void uart6_init() {
	unsigned int ra;

	ra=GET32(RCC_AHB1ENR);
    ra|=1<<2; //enable port C
    PUT32(RCC_AHB1ENR,ra);

	ra=GET32(RCC_APB2ENR);
    ra|=1<<5; //enable USART6
    PUT32(RCC_APB2ENR,ra);

	//PC6 USART6_TX
    //PC7 USART6_RX 
	
	ra = GET32(GPIOC_MODER);
	ra |= (2<<12);
    ra |= (2<<14); 
    PUT32(GPIOC_MODER,ra);
    ra = GET32(GPIOC_OTYPER);
	ra &= (1<<6);
    ra &= (1<<7);
    PUT32(GPIOC_OTYPER,ra);
    ra = GET32(GPIOC_AFRL);
	ra |=(8<<24);
    ra |=(8<<28);
    PUT32(GPIOC_AFRL,ra);

    // Baud rate from clock frequency: BR = Fck/(16*USARTDIV)
// USARTDIV = Fck/(BR*16)
    switch(SPEED) {
		case CLOCK8:
			if (TRAS_SPEED == UART9600)
				PUT32(USART6_BRR,(52<<4)|(1<<0));
			else if (TRAS_SPEED == UART19200)
				PUT32(USART6_BRR,(26<<4)|(0<<0));
			break;
		case CLOCK16:
			if (TRAS_SPEED == UART9600)
				PUT32(USART6_BRR,(104<<4)|(2<<0));
			else if (TRAS_SPEED == UART19200)
				PUT32(USART6_BRR,(52<<4)|(1<<0));
			break;
		case CLOCK32:
			if (TRAS_SPEED == UART9600)
				PUT32(USART6_BRR,(208<<4)|(5<<0));
			else if (TRAS_SPEED == UART19200)
				PUT32(USART6_BRR,(104<<4)|(2<<0));
			break;
		case CLOCK42:
			if (TRAS_SPEED == UART9600)
				PUT32(USART6_BRR,(273<<4)|(7<<0));
			else if (TRAS_SPEED == UART19200)
				PUT32(USART6_BRR,(136<<4)|(11<<0));
			break;
		case CLOCK64:
			if (TRAS_SPEED == UART9600)
				PUT32(USART6_BRR,(416<<4)|(10<<0));
			else if (TRAS_SPEED == UART19200)
				PUT32(USART6_BRR,(208<<4)|(5<<0));
			break;
		case CLOCK120:
			if (TRAS_SPEED == UART9600)
				PUT32(USART6_BRR,(390<<4)|(10<<0));
			else if (TRAS_SPEED == UART19200)
				PUT32(USART6_BRR,(195<<4)|(5<<0));
			break;
		case CLOCK168:
			if (TRAS_SPEED == UART9600)
				PUT32(USART6_BRR,(136<<4)|(11<<0));
			else if (TRAS_SPEED == UART19200)
				PUT32(USART6_BRR,(68<<4)|(5<<0));
			break;
	}
	PUT32(USART6_CR1,(1<<13)|(1<<2)|(1<<5)|(1<<3));
	PUT32(0xE000E108,0x00000080); // enable USART6 interrupt
}

int uart_init () {
    uart1_init();
	uart6_init();
	return 0;
}

void uart1_handler() {
	int data,sr;

	sr = GET32(USART1_SR);
	if (sr & (1<<3))
	{
		turnGPIO(PORTD,14,ON);
		while (1) {}
	}
	else {
		data = GET32(USART1_DR);
		if (counter < UART_BUF_MAX_DIM) {
			if ((writer < UART_BUF_MAX_DIM)) {
				uart_buf[writer] = data;
				writer++; 
			}
			else {
				writer = 0;
				uart_buf[writer] = GET32(USART6_DR);
			}
			counter++;
		}
	}
}

void uart6_handler() {
	int data,sr;

	sr = GET32(USART6_SR);
	if (sr & (1<<3))
	{
		turnGPIO(PORTD,14,ON);
		while (1) {}
	}
	else {
		data = GET32(USART6_DR);
		if (counter < UART_BUF_MAX_DIM) {
			if ((writer < UART_BUF_MAX_DIM)) {
				uart_buf[writer] = data;
				writer++; 
			}
			else {
				writer = 0;
				uart_buf[writer] = GET32(USART6_DR);
			}
			counter++;
		}
	}
}

int random(void *data, unsigned char *output, unsigned int len, unsigned int *olen) 
{
	unsigned int ra;

    while(1)
    {
        ra=GET32(RNG_SR);
        if(ra & 1) break;
        if(ra & 0x66)
        {
			turnGPIO(PORTD,14,ON);
            return(POLARSSL_ERR_ENTROPY_SOURCE_FAILED);
        }
    }
    if(ra & 0x66)
    {
		turnGPIO(PORTD,14,ON);
        return(POLARSSL_ERR_ENTROPY_SOURCE_FAILED);
    }
	ra = GET32(RNG_DR);
	if ((int) len <= 4) {
		memcpy(output,&ra,len);
		*olen = len;
	}
	else {
		memcpy(output,&ra,4);
		*olen = 4;
	}
    return(0);	
}

void turnGPIO(int port,int number,int mode) {
	unsigned int ra,andop,orop;

	if (mode == ON) {
		andop = ~(1<<(number+16));
		orop = (1<<number);
	} else {
		andop = ~(1<<number);
		orop = (1<<(number+16));
	}

	if (port == PORTA) { 
		ra = GET32(GPIOA_BSRR);
		ra &= andop;
		ra |= orop;
		PUT32(GPIOA_BSRR,ra);
	}
	if (port == PORTB) { 
		ra = GET32(GPIOB_BSRR);
		ra &= andop;
		ra |= orop;
		PUT32(GPIOB_BSRR,ra);
	}
	else if (port == PORTC) { 
		ra = GET32(GPIOC_BSRR);
		ra &= andop;
		ra |= orop;
		PUT32(GPIOC_BSRR,ra);
	}
	else if (port == PORTD) { 
		ra = GET32(GPIOD_BSRR);
		ra &= andop;
		ra |= orop;
		PUT32(GPIOD_BSRR,ra);
	}
}

void uart_putc ( unsigned int x, int uart )
{
	if (uart == UART1) {
    	while (( GET32(USART1_SR) & (1<<7)) == 0) continue;
    	PUT32(USART1_DR,x);
	}
	else if (uart == UART6) {
		while (( GET32(USART6_SR) & (1<<7)) == 0) continue;
    	PUT32(USART6_DR,x);
	}
}

unsigned int uart_getc ( void )
{
	int data;	

	while (counter == 0) continue;
	data = uart_buf[reader];
	reader++;
	if (reader == UART_BUF_MAX_DIM) reader = 0;
	counter--;	
	return data;
}

void uart_string (unsigned char *s,unsigned char len,int uart)
{
	unsigned char *cur = s;

    while ((cur-s) < len) {
        uart_putc(*cur,uart);
		cur++;
	}
}

void uart_getstring(unsigned char *buf,unsigned int len) {
	int i = 0;
	unsigned int rec;
	
	while (i < len) {
		rec = uart_getc();
		buf[i] = rec;
		i++;
	}
}

// SELF IMPLEMENTATION OF FUNCTIONS FROM THE C STDLIB

void *memcpy(void *dest, const void *src, unsigned int n)
{
	char *dp = dest;
    const char *sp = src;
    while (n--)
        *dp++ = *sp++;
    return dest;
}

unsigned int strlen(const char *s) 
{
    const char *p = s;
    while (*s) ++s;
    return s - p;
}

void *memset(void *s, int c, unsigned int n)
{
    unsigned char* p=s;
    while(n--)
        *p++ = (unsigned char)c;
    return s;
}

static const unsigned char charmap[] = {
	'\000', '\001', '\002', '\003', '\004', '\005', '\006', '\007',
	'\010', '\011', '\012', '\013', '\014', '\015', '\016', '\017',
	'\020', '\021', '\022', '\023', '\024', '\025', '\026', '\027',
	'\030', '\031', '\032', '\033', '\034', '\035', '\036', '\037',
	'\040', '\041', '\042', '\043', '\044', '\045', '\046', '\047',
	'\050', '\051', '\052', '\053', '\054', '\055', '\056', '\057',
	'\060', '\061', '\062', '\063', '\064', '\065', '\066', '\067',
	'\070', '\071', '\072', '\073', '\074', '\075', '\076', '\077',
	'\100', '\141', '\142', '\143', '\144', '\145', '\146', '\147',
	'\150', '\151', '\152', '\153', '\154', '\155', '\156', '\157',
	'\160', '\161', '\162', '\163', '\164', '\165', '\166', '\167',
	'\170', '\171', '\172', '\133', '\134', '\135', '\136', '\137',
	'\140', '\141', '\142', '\143', '\144', '\145', '\146', '\147',
	'\150', '\151', '\152', '\153', '\154', '\155', '\156', '\157',
	'\160', '\161', '\162', '\163', '\164', '\165', '\166', '\167',
	'\170', '\171', '\172', '\173', '\174', '\175', '\176', '\177',
	'\200', '\201', '\202', '\203', '\204', '\205', '\206', '\207',
	'\210', '\211', '\212', '\213', '\214', '\215', '\216', '\217',
	'\220', '\221', '\222', '\223', '\224', '\225', '\226', '\227',
	'\230', '\231', '\232', '\233', '\234', '\235', '\236', '\237',
	'\240', '\241', '\242', '\243', '\244', '\245', '\246', '\247',
	'\250', '\251', '\252', '\253', '\254', '\255', '\256', '\257',
	'\260', '\261', '\262', '\263', '\264', '\265', '\266', '\267',
	'\270', '\271', '\272', '\273', '\274', '\275', '\276', '\277',
	'\300', '\301', '\302', '\303', '\304', '\305', '\306', '\307',
	'\310', '\311', '\312', '\313', '\314', '\315', '\316', '\317',
	'\320', '\321', '\322', '\323', '\324', '\325', '\326', '\327',
	'\330', '\331', '\332', '\333', '\334', '\335', '\336', '\337',
	'\340', '\341', '\342', '\343', '\344', '\345', '\346', '\347',
	'\350', '\351', '\352', '\353', '\354', '\355', '\356', '\357',
	'\360', '\361', '\362', '\363', '\364', '\365', '\366', '\367',
	'\370', '\371', '\372', '\373', '\374', '\375', '\376', '\377',
};

int strcasecmp(const char *s1, const char *s2)
{
	register const unsigned char *cm = charmap, 
		*us1 = (const unsigned char *)s1, *us2 = (const unsigned char *)s2;

	while (cm[*us1] == cm[*us2++])
		if (*us1++ == '\0')
			return (0);
	return (cm[*us1] - cm[*--us2]);
}
