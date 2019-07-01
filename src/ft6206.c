/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author        Notes
 * 2019-06-23     WillianChan   the first version
 */

#include <rtthread.h>
#include <rtdevice.h>
#include "touch.h"
#include <string.h>

#define DBG_TAG "ft6206"
#define DBG_LVL DBG_INFO
#include <rtdbg.h>

#define IIC_RETRY_NUM 2

static struct rt_i2c_client *ft6206_client;

static void ft6206_write_reg(struct rt_i2c_client *dev,
                             rt_uint8_t addr,
                             rt_uint8_t *buffer,
                             rt_size_t length)
{
    rt_uint8_t *send_buffer = rt_malloc(length + 1);

    RT_ASSERT(send_buffer);

    send_buffer[0] = addr;
    memcpy(send_buffer + 1, buffer, length);

    struct rt_i2c_msg msgs[] =
    {
        {
            .addr   = dev->client_addr,
            .flags  = RT_I2C_WR,
            .len    = length + 1,
            .buf    = send_buffer,
        }
    };

    length = rt_i2c_transfer(dev->bus, msgs, 1);
    rt_free(send_buffer);
    send_buffer = RT_NULL;
}

static int ft6206_read_reg(struct rt_i2c_client *dev,
                           rt_uint8_t addr,
                           rt_uint8_t *buffer,
                           rt_size_t length)
{
    int ret = -1;
    int retries = 0;

    struct rt_i2c_msg msgs[] =
    {
        {
            .addr   = dev->client_addr,
            .flags  = RT_I2C_WR,
            .len    = 1,
            .buf    = &addr,
        },
        {
            .addr   = dev->client_addr,
            .flags  = RT_I2C_RD,
            .len    = length,
            .buf    = buffer,
        },
    };

    while (retries < IIC_RETRY_NUM)
    {
        ret = rt_i2c_transfer(dev->bus, msgs, 2);
        if (ret == 2)break;
        retries++;
    }

    if (retries >= IIC_RETRY_NUM)
    {
        LOG_E("%s i2c read error: %d", __func__, ret);
        return -1;
    }

    return ret;
}

/* Universal adaptation will be done soon */
static rt_err_t ft6206_get_info(struct rt_i2c_client *dev, struct rt_touch_info *info)
{
    info->range_x = 800;
    info->range_y = 480;
    info->point_num = 1;

    return RT_EOK;
}

/* Support will be forthcoming */
static rt_err_t ft6206_control(struct rt_touch_device *device, int cmd, void *data)
{
    if (cmd == RT_TOUCH_CTRL_GET_ID)
    {
        return RT_EOK;
    }

    if (cmd == RT_TOUCH_CTRL_GET_INFO)
    {
        return ft6206_get_info(ft6206_client, data);
    }

    switch(cmd)
    {
        case RT_TOUCH_CTRL_SET_X_RANGE: /* set x range */
        {
            break;
        }
        case RT_TOUCH_CTRL_SET_Y_RANGE: /* set y range */
        {
            break;
        }
        case RT_TOUCH_CTRL_SET_X_TO_Y: /* change x y */
        {
            break;
        }
        case RT_TOUCH_CTRL_SET_MODE: /* change int trig type */
        {
            break;
        }
        default:
        {
            break;
        }
    }

    return -RT_ERROR;
}

static rt_size_t ft6206_read_point(struct rt_touch_device *touch, void *buf, rt_size_t read_num)
{
    int ret = -1;
    uint8_t point_num = 0;
    static uint8_t s_tp_down = 0;
    struct rt_touch_data *read_data = (struct rt_touch_data *)buf;
    uint8_t point[6];
    ret = ft6206_read_reg(ft6206_client, 0x02, &point_num, 1);
    if (ret < 0)
    {
        return 0;
    }
    
    if (point_num == 0)
    {
        if (s_tp_down)
        {
            s_tp_down = 0;
            read_data[0].event = RT_TOUCH_EVENT_UP;
            return 1;
        }
        read_data[0].event = RT_TOUCH_EVENT_NONE;
        return 0;
    }
    
    ret = ft6206_read_reg(ft6206_client, 0x03, point, 6);
    if (ret < 0)
    {
        return 0;
    }

    read_data[0].timestamp = rt_touch_get_ts();
    /* Width information acquisition is about to be realized */
    read_data[0].width = 1;
    read_data[0].x_coordinate = (point[2]&0x0F) << 8 | point[3];
    read_data[0].y_coordinate = (point[0]&0x0F) << 8 | point[1];
    read_data[0].track_id = 0;
		
    LOG_D("%d %d", read_data[0].x_coordinate, read_data[0].y_coordinate);
    
    if (s_tp_down)
    {
        read_data[0].event = RT_TOUCH_EVENT_MOVE;
        return 1;
    }
    read_data[0].event = RT_TOUCH_EVENT_DOWN;
    s_tp_down = 1;
    
    return 1;
}

static struct rt_touch_ops touch_ops =
{
    .touch_readpoint = ft6206_read_point,
    .touch_control = ft6206_control,
};

int rt_hw_ft6206_init(const char *name, struct rt_touch_config *cfg)
{
    rt_touch_t touch_device = RT_NULL;

    touch_device = (rt_touch_t)rt_calloc(1, sizeof(struct rt_touch_device));

    ft6206_client = (struct rt_i2c_client *)rt_calloc(1, sizeof(struct rt_i2c_client));
    ft6206_client->bus = (struct rt_i2c_bus_device *)rt_device_find(cfg->dev_name);

    if (ft6206_client->bus == RT_NULL)
    {
        LOG_E("Can't find device\n");
        return -RT_ERROR;
    }

    if (rt_device_open((rt_device_t)ft6206_client->bus, RT_DEVICE_FLAG_RDWR) != RT_EOK)
    {
        LOG_E("open device failed\n");
        return -RT_ERROR;
    }

    ft6206_client->client_addr = 0x2A;

    /* register touch device */
    touch_device->info.type = RT_TOUCH_TYPE_CAPACITANCE;
    touch_device->info.vendor = RT_TOUCH_VENDOR_FT;
    rt_memcpy(&touch_device->config, cfg, sizeof(struct rt_touch_config));
    touch_device->ops = &touch_ops;

    rt_hw_touch_register(touch_device, name, RT_DEVICE_FLAG_INT_RX, RT_NULL);

    LOG_I("touch device ft6206 init success\n");

    return 0;
}
