## Device

![Device](device.png)

  - **红色**是 G-FAM(PBR Flit) 模式下的流程
  - **黄色**是 LD-FAM(68B Flit, 256B Flit) 模式下的流程
  - **绿色**是 两个模式下的公共流程

> ??? 现为 **APB** 接口 可能会被 **共享变量池** 取代

![device_in_cpp](device_in_cpp.png)

### 一、DCOH v1.0 版本
![dcoh](dcoh.png)
之前的DCOH的状态机的草图。

### 二、DCOH 疑问
![dcoh_q](dcoh_question.png)