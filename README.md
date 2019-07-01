# FT6206

## 简介

ft6206 软件包提供了使用触摸芯片 ft6206 基本功能，并且本软件包已经对接到了 Touch 框架，通过 Touch 框架，开发者可以快速的将此触摸芯片驱动起来。
## 支持情况

| **FT6206 触摸芯片** | **支持情况** |
| :-------: | :--------: |
| I2C 通讯接口 | √ |
| 中断的工作模式 | √ |
| 轮询的工作模式 | √ |

## 使用说明

### 软件包依赖

- RT-Thread 4.0.0+
```
 \ | /
- RT -     Thread Operating System
 / | \     4.0.2 build Jun 23 2019
 2006 - 2019 Copyright by rt-thread team
 msh >
```
- Touch 组件，在 menuconfig 中开启 Touch 组件的路径如下：
```
RT-Thread Components  --->
    Device Drivers  --->
        [*] Using Touch device drivers
```
- I2C 驱动：ft6206 设备使用 I2C 进行数据通讯，需要系统 I2C 驱动支持，在 menuconfig 中开启 I2C 驱动的路径如下：
```
Hardware Drivers Config  --->
    On-chip Peripheral Drivers  --->
        [*] Enable I2C1 BUS (software simulation)  --->
```

### 获取软件包

使用 ft6206 软件包需要在 RT-Thread 的包管理中选中它，具体路径如下：

```
RT-Thread online packages  --->
    peripheral libraries and drivers  --->
      touch drivers  --->
        ft6206: touch ic ft6206 for rt-thread
          Version (latest)  --->
```
配置完成后，使用 `pkgs --update` 更新软件包。

### 使用软件包

ft6206 软件包初始化函数如下所示：

```
int rt_hw_ft6206_init(const char *name, struct rt_touch_config *cfg)
```

该函数需要由用户调用，函数主要完成的功能有：

- 设备配置和初始化（根据传入的配置信息，配置接口设备和中断引脚）；
- 注册相应的传感器设备，完成 ft6206 设备的注册；

#### 初始化示例

```.c
#define FT6206_IRQ_PIN GET_PIN(J, 5)
int rt_hw_ft6206_port(void)
{
    struct rt_touch_config config;
    
    config.dev_name = "i2c1";
    config.irq_pin.pin  = FT6206_IRQ_PIN;
    config.irq_pin.mode = PIN_MODE_INPUT_PULLDOWN;
    
    rt_hw_ft6206_init("ft6206", &config);
    
    return 0;
}
INIT_ENV_EXPORT(rt_hw_ft6206_port);
```

## 注意事项

- ft6206 最大只支持两个触摸点，该软件包目前仅提供一个触点
- 在初始化示例中，需要根据自己板子上的实际连接情况，修改 I2C 的设备名和 ft6206 的中断引脚

## 联系人信息

- 维护人：[Willian Chan](https://github.com/willianchanlovegithub) 
- 软件包主页：<https://github.com/RT-Thread-packages/ft6206>