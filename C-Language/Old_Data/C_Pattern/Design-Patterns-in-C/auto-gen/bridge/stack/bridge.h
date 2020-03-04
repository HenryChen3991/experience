/**
 * bridge.h  2014-05-02 01:32:58
 * anonymouse(anonymouse@email)
 *
 * Copyright (C) 2000-2014 All Right Reserved
 * 
 * THIS CODE AND INFORMATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
 * KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
 * PARTICULAR PURPOSE.
 *
 * Auto generate for Design Patterns in C
 */
#ifndef __BRIDGE_H__
#define __BRIDGE_H__

#include "stack_hanoi.h"
#include "stack_fifo.h"
#include "stack_impl_array.h"
#include "stack_impl_list.h"

struct bridge {
};

void bridge_init(struct bridge *);

void bridge_main_entry(void);

#endif /* __BRIDGE_H__ */
