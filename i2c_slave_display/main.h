#ifndef MAIN_H_
#define MAIN_H_

//#define I2C_ADDR 0x27
#define I2C_ADDR 0x48

int main(void);
static inline char S_to_binary_(const char *);
char bcd27seg(char);
void delay(volatile unsigned int);
void delay_ms(volatile unsigned int);

#endif /*MAIN_H_*/
