/*
 * type.h
 *
 *  Created on: 18.03.2014
 *      Author: Tobias
 */

#ifndef TYPE_H_
#define TYPE_H_

typedef struct irq_hook {
	//struct irq_hook *next;
	uint32_t (*handler)(struct irq_hook *);
	uint8_t irq;
}irq_hook_t;


#endif /* TYPE_H_ */
