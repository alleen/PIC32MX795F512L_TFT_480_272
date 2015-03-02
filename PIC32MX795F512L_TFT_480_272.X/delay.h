/* 
 * File:   delay.h
 * Author: root
 *
 * Created on 2015年1月9日, 下午 2:28
 */

#ifndef DELAY_H
#define	DELAY_H

#ifdef	__cplusplus
extern "C" {
#endif

void msleep(unsigned long long t);
void usleep( unsigned long long t);


#ifdef	__cplusplus
}
#endif

#endif	/* DELAY_H */

