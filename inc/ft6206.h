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

#if defined(RT_VERSION_CHECK) && (RTTHREAD_VERSION >= RT_VERSION_CHECK(4, 1, 1))
#include <rtdevice.h>
#else
#include <touch.h>
#endif

int rt_hw_ft6206_init(const char *name, struct rt_touch_config *cfg);

#endif /* _FT6206_H_ */
