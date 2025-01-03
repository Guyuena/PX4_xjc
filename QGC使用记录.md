# QGC

<img src="./images/image-20241114103152523.png" alt="image-20241114103152523" style="zoom:33%;" />

<img src="./images/image-20241114103244631.png" alt="image-20241114103244631" style="zoom:33%;" />



```
Builtin Apps:
  sh                            thoneflow                     
  sercon                        tone_alarm                    
  bmp280                        fw_autotune_attitude_control  
  bmp388                        mc_autotune_attitude_control  
  bmp581                        attitude_estimator_q          
  dps310                        battery_status                
  lps22hb                       camera_feedback               
  lps33hw                       commander                     
  ms5611                        control_allocator             
  spl06                         dataman                       
  batt_smbus                    ekf2                          
  board_adc                     esc_battery                   
  bst                           send_event                    
  camera_capture                fake_gps                      
  camera_trigger                flight_mode_manager           
  cdcacm_autostart              fw_att_control                
  cm8jl65                       fw_pos_control                
  ms4525do                      fw_rate_control               
  ms5525dso                     gyro_calibration              
  sdp3x                         gyro_fft                      
  gy_us42                       land_detector                 
  leddar_one                    landing_target_estimator      
  lightware_laser_i2c           load_mon                      
  lightware_laser_serial        local_position_estimator      
  srf02                         logger                        
  teraranger                    mag_bias_estimator            
  tf02pro                       manual_control                
  ulanding_radar                mavlink                       
  vl53l0x                       mc_att_control                
  vl53l1x                       mc_hover_thrust_estimator     
  dshot                         mc_pos_control                
  frsky_telemetry               mc_rate_control               
  gimbal                        navigator                     
  gps                           rc_update                     
  hmc5883                       rover_pos_control             
  hott_sensors                  sensors                       
  hott_telemetry                temperature_compensation      
  bmi088                        uxrce_dds_client              
  icp101xx                      vtol_att_control              
  icp201xx                      actuator_test                 
  irlock                        bl_update                     
  ll40ls                        dmesg                         
  ak09916                       dumpfile                      
  ak8963                        gpio                          
  bmm150                        hardfault_log                 
  ist8308                       i2cdetect                     
  ist8310                       led_control                   
  lis3mdl                       mft                           
  lsm303agr                     mtd                           
  mmc5983ma                     nshterm                       
  qmc5883l                      param                         
  iis2mdc                       perf                          
  vcm1193l                      reboot                        
  mpc2520                       sd_bench                      
  paa3905                       sd_stress                     
  paw3902                       system_time                   
  pmw3901                       top                           
  pwm_out                       listener                      
  px4flow                       tune_control                  
  rc_input                      uorb                          
  rgbled                        usb_connected                 
  rgbled_is31fl3195             ver                           
  rgbled_lp5562                 work_queue                    
  rgbled_ncp5623c               nsh                           
  rm3100                        serdis  

```



述内容列出了一个嵌入式系统（如NuttX或PX4等环境）中的**Builtin Apps**，即内置应用程序。这些应用程序包含了丰富的模块，涵盖传感器、飞行控制、导航、通信、系统管理等多个功能领域。以下是对这些应用的分类和简要分析：

### 1. **传感器模块**

- 包括多种传感器驱动程序，通常用于获取环境数据和状态信息：
  - **气压/高度传感器**：`bmp280`, `bmp388`, `bmp581`, `dps310`, `lps22hb`, `ms5611`, `spl06`等。
  - **激光/雷达传感器**：`lightware_laser_i2c`, `lightware_laser_serial`, `teraranger`, `ulanding_radar`, `vl53l0x`, `vl53l1x`等。
  - **IMU/磁力计传感器**：`hmc5883`, `bmi088`, `ak09916`, `ak8963`, `bmm150`, `ist8308`, `ist8310`, `lis3mdl`, `lsm303agr`, `qmc5883l`等。
  - **其他**：`srf02`（声呐），`dshot`（电机信号传输），`gps`（全球定位系统），用于位置和环境感知。

### 2. **飞行控制与姿态控制模块**

- 这些模块用于控制飞行器的姿态、位置和速度，是无人机或机器人等应用的核心组件：
  - **固定翼控制**：`fw_att_control`, `fw_pos_control`, `fw_rate_control`等。
  - **多旋翼控制**：`mc_att_control`, `mc_pos_control`, `mc_rate_control`, `mc_hover_thrust_estimator`等。
  - **VTOL（垂直起降飞行器）控制**：`vtol_att_control`。
  - **姿态估计器**：`attitude_estimator_q`, `ekf2`（扩展卡尔曼滤波器），用于精确计算飞行器的姿态。

### 3. **定位与导航模块**

- 用于计算飞行器或机器人在环境中的位置，并帮助实现导航功能：
  - **位置估计**：`local_position_estimator`, `landing_target_estimator`。
  - **导航控制**：`navigator`，用于路径规划和执行。
  - **地面检测**：`land_detector`，检测是否接近地面以辅助降落。

### 4. **通信与数据传输模块**

- 包含用于通信的驱动和协议支持，通常用于数据传输、远程控制和遥测：
  - **遥测**：`mavlink`（无人机通信协议），`frsky_telemetry`，`hott_telemetry`，用于将飞行器数据发送至地面站。
  - **通信控制**：`sercon`, `uxrce_dds_client`（DDS通信）。
  - **数据存储和管理**：`dataman`（数据管理），`dumpfile`（文件转储）。

### 5. **系统监控与管理**

- 这些模块用于监视系统的状态、执行日志记录和系统操作：
  - **状态监控**：`battery_status`（电池状态），`esc_battery`（电子调速器电池），`system_time`，`top`（系统资源监视）。
  - **日志记录**：`logger`，`hardfault_log`（硬故障日志）。
  - **事件管理**：`send_event`，可用于系统事件处理。
  - **性能监视**：`perf`，用于监视系统性能。

### 6. **其他实用工具**

- 一些通用工具，用于各种辅助和配置功能：
  - **控制分配器**：`control_allocator`，在多传感器或多控制器系统中分配控制信号。
  - **GPIO控制**：`gpio`，控制通用输入输出端口。
  - **LED控制**：`led_control`，控制设备上的LED。
  - **其他控制**：`tune_control`（音调控制），`camera_capture`（相机拍摄），`camera_trigger`（相机触发）。

### 7. **调试和系统测试**

- 这些模块用于调试和测试系统的各个部分，确保系统正常运行：
  - **I2C总线检测**：`i2cdetect`，扫描I2C设备。
  - **存储测试**：`sd_bench`, `sd_stress`，用于测试SD卡的性能和稳定性。
  - **系统调试**：`dmesg`（系统日志），`reboot`（系统重启），`listener`（监听器）。

### 8. **嵌入式系统管理**

- 用于系统配置和管理的模块，通常用于配置参数和控制系统行为：
  - **参数管理**：`param`，用于设置和获取系统参数。
  - **进程管理**：`nshterm`（NSH终端），`ver`（系统版本信息）。
  - **任务队列**：`work_queue`，用于在嵌入式系统中管理任务队列和优先级。

### 总结

该系统的内置应用程序涵盖了**传感器管理**、**飞行与姿态控制**、**导航**、**通信**、**系统监控**等多个方面的功能模块。这些应用程序可以满足无人机、机器人或其他复杂嵌入式系统的需求，适用于从数据采集、系统控制到通信、监控的全方位任务。



# mavlink status

在NSH终端中，输入以下命令可以查看MAVLink的串口状态：

```
mavlink status
```

该命令会输出MAVLink实例的状态信息，包括它们所绑定的串口（例如`/dev/ttyS1`或`/dev/ttyACM0`）、波特率、数据传输状态等信息。

```bash
nsh> mavlink status

instance #0:
    mavlink chan: #0
    type:        GENERIC LINK OR RADIO
    flow control: OFF
    rates:
      tx: 578.2 B/s
      txerr: 0.0 B/s
      tx rate mult: 0.292
      tx rate max: 1200 B/s
      rx: 0.0 B/s
      rx loss: 0.0%
    FTP enabled: YES, TX enabled: YES
    mode: Normal
    Forwarding: On
    MAVLink version: 1
    transport protocol: serial (/dev/ttyS1 @57600)

instance #1:
    GCS heartbeat valid
    mavlink chan: #1
    type:        USB CDC
    flow control: OFF
    rates:
      tx: 21612.3 B/s
      txerr: 0.0 B/s
      tx rate mult: 1.000
      tx rate max: 100000 B/s
      rx: 46.9 B/s
      rx loss: 0.0%
    Received Messages:
      sysid:255, compid:190, Total: 3388 (lost: 0)
    FTP enabled: YES, TX enabled: YES
    mode: Onboard
    Forwarding: On
    MAVLink version: 2
    transport protocol: serial (/dev/ttyACM0 @2000000)
    ping statistics:
      last: 33.30 ms
      mean: 31.58 ms
      max: 35419.20 ms
      min: 0.15 ms
      dropped packets: 0
nsh> 
```

1. **实例 #0**:

   - **MAVLink 通道**: #0

   - **类型**: GENERIC LINK OR RADIO（通用连接或无线电）

   - **流量控制**: 关闭

   - 传输速率

     :

     - 发送（tx）: 578.2 B/s
     - 错误发送（txerr）: 0.0 B/s
     - 最大传输速率（tx rate max）: 1200 B/s
     - 接收（rx）: 0.0 B/s
     - 接收丢失（rx loss）: 0.0%

   - **FTP**: 启用

   - **模式**: Normal

   - **MAVLink 版本**: 1

   - **传输协议**: 串口 `/dev/ttyS1`，波特率 57600

   - **用途**: 这一实例通常用于无线电通信或远程链路。

2. **实例 #1**:

   - **GCS 心跳**: 有效

   - **MAVLink 通道**: #1

   - **类型**: USB CDC（USB 通信设备类）

   - **流量控制**: 关闭

   - 传输速率

     :

     - 发送（tx）: 21612.3 B/s
     - 错误发送（txerr）: 0.0 B/s
     - 最大传输速率（tx rate max）: 100000 B/s
     - 接收（rx）: 46.9 B/s
     - 接收丢失（rx loss）: 0.0%

   - 接收的消息

     :

     - 系统 ID（sysid）: 255
     - 组件 ID（compid）: 190
     - 总数（Total）: 3388（丢失: 0）

   - **FTP**: 启用

   - **模式**: Onboard

   - **MAVLink 版本**: 2

   - **传输协议**: 串口 `/dev/ttyACM0`，波特率 2000000

   - Ping 统计

     :

     - 最后一次: 33.30 ms
     - 平均: 31.58 ms
     - 最大: 35419.20 ms
     - 最小: 0.15 ms
     - 丢包数: 0

   - **用途**: 这一实例连接到地面控制站（GCS），用于 USB 数据通信。

**总结**

- **实例 #0** 使用 `/dev/ttyS1` 串口，波特率为 57600，通常用于无线电通信。
- **实例 #1** 使用 `/dev/ttyACM0` 串口，波特率为 2000000，连接地面控制站（GCS），通过 USB 传输数据





# gps status

```bash
nsh> gps status
INFO [gps] Main GPS
INFO [gps] protocol: UBX
INFO [gps] status: NOT OK, port: /dev/ttyS0, baudrate: 0
INFO [gps] sat info: disabled
INFO [gps] rate reading:       0 B/s
```

GPS 状态信息

1. 主 GPS（Main GPS）
   - **协议**: 使用 UBX 协议（Ublox 协议），这是一种常见的 GPS 通信协议。
   - **状态**: NOT OK，这表明当前 GPS 连接或配置存在问题，无法正常工作。
   - **串口端口**: `/dev/ttyS0`，即主 GPS 模块连接在 `/dev/ttyS0` 串口。
   - **波特率**: `0`，说明 GPS 模块的波特率未正确设置，通常情况下应设置为9600、38400等标准波特率。
   - **卫星信息**: disabled，表示卫星信息采集功能已禁用，系统不会采集或显示卫星状态。
   - **读取速率**: `0 B/s`，表示当前没有数据从 GPS 模块接收。



#  i2cdetect

```
nsh> i2cdetect
Scanning I2C bus: 1
   0 1 2 3 4 5 6 7 8 9 a b c d e f
00: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
10: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
20: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
30: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
40: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
50: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
60: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
70: -- -- -- -- -- -- -- 77 -- -- -- -- -- -- -- -- 
nsh> 
```

2C 扫描结果分析

- I2C 地址表分为 0x00 到 0x7F 共 128 个地址。
- 在这些地址中，只有 `0x77` 地址有设备响应，其他地址均未检测到任何设备（用 `--` 表示）。

设备分析

- 地址 0x77

  :

  - 在 I2C 总线上，地址 `0x77` 通常对应的是气压计（例如，BMP280、BMP388 等）或其他传感器模块。具体设备类型需要根据硬件配置或 PX4 参数进行确认。





# dmesg

![image-20241114110438812](./images/image-20241114110438812.png)



**/dev/ttyS0**: 用于主 GPS 模块。

**/dev/ttyS4**: 用于遥控（RC）输入驱动。

**/dev/ttyS1**: 用于 MAVLink 通信（无线电通信），波特率为 57600。

**/dev/ttyACM0**: 用于 USB MAVLink 通信，波特率为 2000000，传输速率较高，适用于地面控制站（GCS）。



```
WARN  [health_and_arming_checks] Preflight Fail: Attitude failure (pitch)
WARN  [health_and_arming_checks] Preflight Fail: Attitude failure (roll)
```

- **解释**：系统在起飞前检查中检测到姿态故障，涉及俯仰（pitch）和横滚（roll）方向的姿态错误。

- 可能原因

  ：

  - 传感器（IMU）数据异常，导致姿态估计不正确。
  - IMU 校准问题，或者外部干扰引起的姿态偏移。

- **影响**：由于这些错误，系统将阻止起飞，除非问题得到解决。需要重新校准 IMU 或检查传感器状态。



## Board extras: /etc/init.d/rc.board_extras

表示 PX4 启动脚本运行了一个名为 `rc.board_extras` 的附加配置文件。这是一个额外的脚本，用于初始化与特定硬件板（Board）相关的额外功能或配置。

详细解释

1. **位置和名称**：
   - `rc.board_extras` 文件位于 `/etc/init.d/` 目录下，这是 PX4 系统存储各种启动和配置脚本的目录。
   - `rc.board_extras` 文件通常用于配置特定硬件的附加功能，因此它因板（board）不同而有所差异。
2. **用途**：
   - 该脚本加载一些可选或附加的配置项和驱动，这些配置通常不是 PX4 启动的核心部分，但可能对特定硬件功能至关重要。
   - 例如，可以在此脚本中配置附加的传感器驱动、板载 LED、蜂鸣器、外接串口设备、其他通信接口（如 USB、CAN 等）等硬件资源。
3. **与其他启动脚本的关系**：
   - `rc.board_extras` 通常在主要启动脚本（例如 `rcS` 或 `rc.board_defaults`）之后运行。主要启动脚本完成核心系统初始化后，再运行这个附加脚本以启用其他设备或功能。
   - 在 PX4 的启动流程中，`rc.board_extras` 的加载是可选的，根据硬件需求加载额外的驱动和配置。
4. **自定义与扩展**：
   - 如果您有定制需求，可以编辑或扩展 `rc.board_extras` 脚本，以包含额外的初始化逻辑，比如启用特定 GPIO、加载额外传感器、配置特定通信端口等。
   - 在不同的硬件平台或自定义硬件配置中，您可以修改此文件以适应特定需求。

总结

`Board extras: /etc/init.d/rc.board_extras` 表示 PX4 系统加载了一个特定的附加配置脚本，用于完成板级的附加硬件初始化。此脚本可以在系统启动时加载其他非核心驱动程序和配置，以支持更多板载功能。







##  `rc.` 配置脚本

在 PX4 的启动过程中，会运行一系列的 `rc.` 配置脚本，这些脚本负责初始化系统的各个部分，包括板级硬件、传感器、参数配置、飞行模式等。以下是常见的 `rc.` 文件及其作用：

### 1. **rcS**

- **位置**：`/etc/init.d/rcS`

- **作用**：这是 PX4 的主启动脚本，启动时首先加载并执行 `rcS`。它负责加载其他配置脚本、初始化系统主要功能，并设置飞控的工作环境。

- 主要流程

  ：

  - 初始化基本系统环境。
  - 根据硬件配置加载必要的模块和驱动。
  - 启动其他辅助脚本（如 `rc.board_defaults`、`rc.board_sensors` 等）。
  - 配置飞行模式、参数等系统设置。

![image-20241114113420972](./images/image-20241114113420972.png)





**编译配置**

![image-20241114133504753](./images/image-20241114133504753.png)

![image-20241114133655484](./images/image-20241114133655484.png)







### 2. **rc.board_defaults**

- **位置**：`/etc/init.d/rc.board_defaults`

- **作用**：该脚本用于设置板级硬件的默认配置。主要定义 PX4 控制器的基础硬件资源，如 PWM 输出、串口配置、IO 端口、LED 控制等。

- 主要内容

  ：

  - 定义硬件资源的初始状态。
  - 设置 PWM 输出配置和频率。
  - 配置板载 LED、蜂鸣器等硬件。

### 3. **rc.board_sensors**

- **位置**：`/etc/init.d/rc.board_sensors`

- **作用**：用于初始化板载传感器。不同的飞控板可能配备不同的传感器，因此该脚本根据具体硬件配置加载相关传感器驱动程序。

- 主要内容

  ：

  - 初始化 IMU（加速度计、陀螺仪）传感器。
  - 初始化气压计、磁力计、GPS 等传感器。
  - 配置传感器的校准和偏移量参数。

### 4. **rc.board_extras**

- **位置**：`/etc/init.d/rc.board_extras`

- **作用**：加载板级的额外配置，通常是特定飞控板的附加功能，如外部设备支持或特定通信接口。

- 主要内容

  ：

  - 加载附加的外部设备驱动。
  - 启用额外的串口、CAN 总线、USB 设备等。

### 5. **rc.autostart**

- **位置**：`/etc/init.d/rc.autostart`

- **作用**：此脚本根据自动启动 ID 选择并加载特定的飞行器配置（airframe configuration）。自动启动 ID 是根据飞行器类型和硬件需求设置的。

- 主要内容

  ：

  - 根据飞行器的类型（如多旋翼、固定翼、VTOL）加载相应的配置。
  - 调用适当的启动脚本（如 `/etc/init.d/airframes/` 中的脚本）来配置飞行模式和控制参数。

### 6. **rc.mc_defaults** 和 **rc.fw_defaults**

- **位置**：`/etc/init.d/rc.mc_defaults`（多旋翼）和 `/etc/init.d/rc.fw_defaults`（固定翼）

- **作用**：分别为多旋翼和固定翼飞行器配置默认参数。根据飞行器类型加载适当的飞控参数和配置。

- 主要内容

  ：

  - 加载多旋翼和固定翼的默认控制参数。
  - 设置姿态控制、位置控制和其他控制参数。

### 7. **rc.airmode**

- **位置**：`/etc/init.d/rc.airmode`

- **作用**：用于配置 PX4 的 Air Mode（空中模式），主要用于多旋翼飞行器，以提高在低油门下的姿态控制能力。

- 主要内容

  ：

  - 启用或禁用 Air Mode。
  - 配置控制器参数，确保在低油门状态下飞控能维持姿态控制。

### 8. **rc.logging**

- **位置**：`/etc/init.d/rc.logging`

- **作用**：控制日志记录的配置。用于配置飞行数据的记录方式和参数。

- 主要内容

  ：

  - 启用或禁用日志记录功能。
  - 配置日志记录的模式（如手动、自动）和文件存储路径。

### 9. **rc.mavlink**

- **位置**：`/etc/init.d/rc.mavlink`

- **作用**：用于配置 MAVLink 通信，包括数据流速率和模式。MAVLink 是飞控系统和地面站之间的通信协议。

- 主要内容

  ：

  - 配置 MAVLink 通信端口和波特率。
  - 设置数据流速率和传输模式（如 GCS 通信或 OSD）。

### 10. **Airframe-specific Scripts（特定机架配置脚本）**

- **位置**：`/etc/init.d/airframes/`

- **作用**：每种飞行器都有独特的配置脚本，这些脚本根据机架（airframe）加载特定的控制参数和设置。`rc.autostart` 脚本会调用这些脚本。

- 主要内容

  ：

  - 根据机架类型（如四旋翼、六旋翼、固定翼等）加载特定控制器参数。
  - 设置电机映射、控制增益和其他飞行器特定的参数。

**总结**

PX4 启动过程中会依次加载多个 `rc.` 脚本文件，以确保系统按照飞行器类型和硬件配置完成初始化。常用的包括 `rcS`（主脚本）、`rc.board_defaults`（板级配置）、`rc.board_sensors`（传感器配置）、`rc.autostart`（飞行器类型配置）等，此外还有特定功能和飞行模式的脚本，如 `rc.airmode` 和 `rc.logging`。这些脚本配合使用，形成了 PX4 系统的完整初始化流程。

















# 查看所有后台运行的传感器及其状态

 PX4 的 NuttShell (NSH) 环境中，可以使用以下方法查看所有后台运行的传感器及其状态：

### 1. 使用 `listener` 命令

- `listener` 命令可以查看特定传感器的当前状态和数据。虽然 `listener` 命令本身不会列出所有的传感器，但可以结合具体传感器的名称查看状态。

- 例如，查看加速度计的状态：

  ```
  listener sensor_accel
  ```

- 其它常见的传感器包括：

  ```
  listener sensor_gyro
  listener sensor_mag
  listener sensor_baro
  listener sensor_gps
  ```

- 通过查看这些传感器的数据输出，可以确认哪些传感器在后台运行并获得其当前数据。

### 2. 使用 `uorb top` 命令

- 在 PX4 系统中，`uORB` 是用于发布和订阅消息的中间件，传感器会向其发布数据。

- 输入以下命令查看所有 uORB 消息的实时状态，包括传感器的发布情况：

  ```
  uorb top
  ```

- 该命令会显示所有正在发布消息的 uORB 主题，传感器数据的主题通常以 `sensor_` 开头，如 `sensor_accel`、`sensor_gyro`、`sensor_mag` 等。

- 通过观察这些主题的更新速率，可以确定后台运行的传感器。

### 3. 使用 `work_queue` 命令

- `work_queue` 命令可以查看所有正在后台运行的任务，其中也包括传感器驱动任务。

- 运行以下命令查看任务队列状态：

  ```
  work_queue
  ```

- 该命令会列出所有在 PX4 系统中运行的任务和其更新频率，通过查看带有 `sensor` 字样的任务，可以了解当前活跃的传感器任务。

### 4. 使用 `top` 命令

- `top` 命令可以列出当前运行的所有进程，显示它们的 CPU 和内存使用情况。

- 输入以下命令查看系统中运行的进程：

  ```
  top
  ```

- 在输出中，查看带有 `sensor` 或其他传感器相关名称的进程，可以了解正在运行的传感器进程及其资源占用情况。

### 5. 查看启动脚本 `rcS`

- 在 PX4 的启动脚本 `rcS` 中，通常会配置哪些传感器在启动时运行。

- 通过查看 `/etc/init.d/rcS` 文件，可以找到与传感器相关的启动指令，比如 `gps start`、`sensors start` 等，从而确认系统中启用了哪些传感器。

- 可以使用以下命令查看 rcS的内容：

  ```
  
  cat /etc/init.d/rcS
  ```

### 



# 查看串口



ls /dev

![image-20241114105830435](./images/image-20241114105830435.png)





