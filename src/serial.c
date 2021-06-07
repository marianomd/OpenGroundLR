#include "serial.h"
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>

void serial_init(void)
{
	/* Enable clocks for GPIO port A (for GPIO_USART1_TX) and USART1. */
	rcc_periph_clock_enable(RCC_GPIOA);
	rcc_periph_clock_enable(RCC_USART1);

	/* USART1 pins: PA9/TX PA10/RX */
	gpio_mode_setup(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO9 | GPIO10);
	gpio_set_af(GPIOA, GPIO_AF1, GPIO9 | GPIO10);

	/* Setup UART parameters. */
	usart_set_baudrate(USART1, 115200);
	usart_set_databits(USART1, 8);
	usart_set_parity(USART1, USART_PARITY_NONE);
	usart_set_stopbits(USART1, USART_STOPBITS_1);
	//usart_set_stopbits(USART1, USART_CR2_STOP_1_0BIT);
	usart_set_mode(USART1, USART_MODE_TX_RX);
	usart_set_flow_control(USART1, USART_FLOWCONTROL_NONE);

	/* Finally enable the USART. */
	usart_enable(USART1);
}

void phex4(uint8_t c) {
	usart_send_blocking(P_USART, (uint8_t)(c + ((c < 10) ? '0' : 'A' - 10)));
}

void phex(uint8_t c) {
	phex4((c>>4));
	phex4((c&15));
}

void phex16(uint16_t c) {
	phex((uint8_t)(c>>8));
	phex((uint8_t)c);
}

void phex24(uint32_t c) {
	phex16((uint16_t)((c>>8)&0xFFFF));
	phex((uint8_t)c);
}

void phex32(uint32_t c) {
	phex16((uint16_t)(c>>16));
	phex16((uint16_t)(c&0xFFFF));
}

void pent(void) {
	usart_send_blocking(P_USART, '\r');
	usart_send_blocking(P_USART, '\n');
}

void pstr(char *s) {
	uint16_t i = 0;
	while(s[i]) {
		usart_send_blocking(P_USART, (uint8_t)(s[i++]));
	}
}

// vim: tabstop=4:shiftwidth=4:noexpandtab
