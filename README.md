# PX4_xjc

px4开发学习

用来把扫地机底板的各种信息数据通过窜口传输到px4中



| 内容                             | 作者 | 日期       |
| -------------------------------- | ---- | ---------- |
| 实现从串口接收到扫地机的底板数据 | xjc  | 2025-01-03 |
|                                  |      |            |
|                                  |      |            |
|                                  |      |            |









## 注意

1、使用的串口uart要注意px4中的配置，避免和其他串口冲突
这里使用了uart4，也可按照自己的需求更改

重点注意在px4中开发 c++的支持甚少，主要还是用C比较稳妥，尽量用nuttx支持的api

**PX4串口映射**

-  TELEM1 -> UART2
-  TELEM2 -> UART4
-  TELEM3-> UART7 (ESC Telemetry)
-  TELEM4 -> UART8
-  GPS1 -> UART1
-  GPS2 -> UART3
-  RC -> UART5

<img src="./images/image-20250103152358935.png" alt="image-20250103152358935" style="zoom: 50%;" />



<img src="./images/image-20250103152515972.png" alt="image-20250103152515972" style="zoom:67%;" />







## 使用

1、配置

![image-20250103152552588](./images/image-20250103152552588.png)

2、增项

![image-20250103152622408](./images/image-20250103152622408.png)



3、编译

![image-20250103152649464](./images/image-20250103152649464.png)



4、烧录

![image-20250103152717721](./images/image-20250103152717721.png)



插拔

![image-20250103152732714](./images/image-20250103152732714.png)

![image-20250103152742746](./images/image-20250103152742746.png)

![image-20250103152759490](./images/image-20250103152759490.png)









5、验证

![image-20250103153103585](./images/image-20250103153103585.png)

![image-20250103153121046](./images/image-20250103153121046.png)







