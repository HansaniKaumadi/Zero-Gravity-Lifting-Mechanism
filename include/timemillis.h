#ifndef TIMEMILLIS_H
#define TIMEMILLIS_H


void timer2_ctc_100hz_init(void);
void clear_flag(void);
bool get_flag(void);

void millis_init(void);
unsigned long millis(void);


#endif
