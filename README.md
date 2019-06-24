# FT6206

## 简介

ft6206 软件包提供了使用触摸芯片 ft6206 基本功能，并且本软件包已经对接到了 Touch 框架，通过 Touch 框架，开发者可以快速的将此触摸芯片驱动起来。
## 支持情况

| 包含设备 | 触摸芯片 |  
| ------- | -------- | 
| **通讯接口** |  |      
| IIC          | √ | 
| **工作模式** |  |     
| 中断         | × | 
| 轮询         | √ |        

## 使用说明

### 依赖

- RT-Thread 4.0.0+
- Touch 组件
- I2C 驱动：ft6206 设备使用 I2C 进行数据通讯，需要系统 I2C 驱动支持；

### 获取软件包

使用 ft6206 软件包需要在 RT-Thread 的包管理中选中它，具体路径如下：

```
RT-Thread online packages  --->
  peripheral libraries and drivers  --->
    touch drivers  --->
      ft6206: touch ic ft6206 for rt-thread
              Version (latest)  --->
```
**Version**：软件包版本选择

### 使用软件包

ft6206 软件包初始化函数如下所示：

```
int rt_hw_ft6206_init(const char *name, struct rt_touch_config *cfg)
```

该函数需要由用户调用，函数主要完成的功能有，

- 设备配置和初始化（根据传入的配置信息，配置接口设备和中断引脚）；
- 注册相应的传感器设备，完成 ft6206 设备的注册；

#### 初始化示例

```.c
int rt_hw_ft6206_port(void)
{
    struct rt_touch_config config;
    config.dev_name = "i2c1";
		config.irq_pin.pin  = GET_PIN(J, 5);
    config.irq_pin.mode = PIN_MODE_INPUT_PULLDOWN;

    rt_hw_ft6206_init("ft6206", &config);
	
    ft6206_sample("ft6206", 480, 272);

    return 0;
}
INIT_ENV_EXPORT(rt_hw_ft6206_port);
```

## 注意事项

暂无

## 联系人信息

维护人:

- [Willian Chan](https://github.com/willianchanlovegithub) 

- 主页：<https://github.com/RT-Thread-packages/ft6206>