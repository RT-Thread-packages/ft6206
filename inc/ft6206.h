/*
 * Copyright (c) 2006-2022, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-07-08     liYony       the first version
 */

#ifndef _FT6206_H_
#define _FT6206_H_

#include "touch.h"

int rt_hw_ft6206_init(const char *name, struct rt_touch_config *cfg);

#endif /* _FT6206_H_ */
