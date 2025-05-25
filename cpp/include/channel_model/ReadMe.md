
# Channel Model

## 一、"硬件中接口" 映射到 "Cpp Model 中类成员"

> 下述的 **Disabled** 指的是：在 **Cpp Model** 中，未将其选择为类成员

## CPI

|        | Channel | Name            | Bit      | FlitMode           | Placement of Definition                                           | Description |
| :----  | :------ | :-------------- | :------- | :--------------    | :---------------------------------------------------------------- | :----------------------------- |
|        |         |                 |          |                    | CPI -> GLOBAL Layer | Disabled
|        |         |                 |          |                    | CPI -> GLOBAL Layer | Disabled
|
|        |         |                 |          |                    | CPI -> REQ  Layer -> VALID | Disabled since it is Handshake
|        |         |                 |          |                    | CPI -> REQ  Layer -> FLOW_C | Disabled since it is Flow Control
|**S2M** |**BISnp**|**opcode**       | **4**    | **256B, PBR**      | CPI -> REQ  Layer -> HDR -> req_header[3:0] Opcode (see Table 4-7 left)
|**S2M** |**BISnp**|**ldid**         | **12**   | **256B**           | CPI -> REQ  Layer -> HDR -> req_header[15:4] BI-ID | name is changed in Cpp Model
|**S2M** |**BISnp**|**bitag**        | **12**   | **256B, PBR**      | CPI -> REQ  Layer -> HDR -> req_header[27:16] BITag
|        |         |                 |          |                    | CPI -> REQ  Layer -> HDR -> req_header[28] AddressPartity | Disabled since it is in sv
|**S2M** |**BISnp**|**address**      | **46**   | **256B, PBR**      | CPI -> REQ  Layer -> HDR -> req_header[74:29] Address
|**S2M** |**BISnp**|**flitmode**     | **2**    | **256B, PBR**      | CPI -> REQ  Layer -> HDR -> req_header[76:75] Flit Mode
|        |         |                 |          |                    | CPI -> REQ  Layer -> HDR -> req_header[77] Epoch Valid | Disabled since it is Epoch
|        |         |                 |          |                    | CPI -> REQ  Layer -> HDR -> req_header[78] Epoch ID | Disabled since it is Epoch
|        |         |                 |          |                    | CPI -> REQ  Layer -> HDR -> req_header[79+] Port ID | Disabled since it is Epoch
|        |         |                 |          |                    | CPI -> REQ  Layer -> HDR -> req_cmd_parity |  Disabled since it is in sv
|**S2M** |**BISnp**|**spid**         | **12**   | **PBR**            | CPI -> REQ  Layer -> HDR -> req_spid
|**S2M** |**BISnp**|**dpid**         | **12**   | **PBR**            | CPI -> REQ  Layer -> HDR -> req_dpid
|        |         |                 |          |                    | CPI -> REQ  Layer -> CREDIT |  Disabled since it is Flow Control
|
|        |         |                 |          |                    | CPI -> RSP  Layer -> VALID | Disabled since it is Handshake
|        |         |                 |          |                    | CPI -> RSP  Layer -> FLOW_C | Disabled since it is Flow Control
|**S2M** |**NDR**  |**opcode**       | **3**    | **68B, 256B, PBR** | CPI -> RSP  Layer -> HDR -> rsp_header[2:0] Opcode (see Table 4-17 left)
|**S2M** |**NDR**  |**metafield**    | **2**    | **68B, 256B, PBR** | CPI -> RSP  Layer -> HDR -> rsp_header[4:3] MetaField
|**S2M** |**NDR**  |**metavalue**    | **2**    | **68B, 256B, PBR** | CPI -> RSP  Layer -> HDR -> rsp_header[6:5] MetaValue
|**S2M** |**NDR**  |**tag**          | **16**   | **68B, 256B, PBR** | CPI -> RSP  Layer -> HDR -> rsp_header[22:7] Tag
|**S2M** |**NDR**  |**ldid**         | **4**    | **68B, 256B**      | CPI -> RSP  Layer -> HDR -> rsp_header[26:23] LDID
|        |         |                 |          |                    | CPI -> RSP  Layer -> HDR -> rsp_header[28:27] Devlaod | Disabled
|**S2M** |**NDR**  |**flitmode**     | **2**    | **68B, 256B, PBR** | CPI -> RSP  Layer -> HDR -> rsp_header[30:29] Flit Mode
|        |         |                 |          |                    | CPI -> RSP  Layer -> HDR -> rsp_header[31] Epoch Valid | Disabled since it is Epoch
|        |         |                 |          |                    | CPI -> RSP  Layer -> HDR -> rsp_header[32] Epoch ID | Disabled since it is Epoch
|        |         |                 |          |                    | CPI -> RSP  Layer -> HDR -> rsp_header[33+] Port ID | Disabled since it is Epoch
|        |         |                 |          |                    | CPI -> RSP  Layer -> HDR -> req_cmd_parity | Disabled since it is in sv
|**S2M** |**NDR**  |**spid**         | **12**   | **PBR**            | CPI -> RSP  Layer -> HDR -> rsp_spid | undefinition in CXL spec
|**S2M** |**NDR**  |**dpid**         | **12**   | **PBR**            | CPI -> RSP  Layer -> HDR -> rsp_dpid
|        |         |                 |          |                    | CPI -> RSP  Layer -> CREDIT |  Disabled since it is Flow Control
|
|        |         |                 |          |                    | CPI -> DATA Layer -> VALID | Disabled since it is Handshake
|        |         |                 |          |                    | CPI -> DATA Layer -> FLOW_C | Disabled since it is Flow Control
|**S2M** |**DRS**  |**opcode**       | **3**    | **68B, 256B, PBR** | CPI -> DATA Layer -> HDR -> rsp_header[2:0] Opcode (see Table 4-12 left)
|**S2M** |**DRS**  |**metafield**    | **2**    | **68B, 256B, PBR** | CPI -> DATA Layer -> HDR -> rsp_header[5:4] MetaField
|**S2M** |**DRS**  |**metavalue**    | **2**    | **68B, 256B, PBR** | CPI -> DATA Layer -> HDR -> rsp_header[7:6] MetaValue
|**S2M** |**DRS**  |**tag**          | **16**   | **68B, 256B, PBR** | CPI -> DATA Layer -> HDR -> rsp_header[31:16] Tag
|**S2M** |**DRS**  |**ldid**         | **4**    | **68B, 256B**      | CPI -> DATA Layer -> HDR -> rsp_header[35:32] LD-ID
|        |         |                 |          |                    | CPI -> DATA Layer -> HDR -> rsp_header[37:36] Devlaod | Disabled
|**S2M** |**DRS**  |**flitmode**     | **2**    | **68B, 256B, PBR** | CPI -> DATA Layer -> HDR -> rsp_header[39:38] Flit Mode
|        |         |                 |          |                    | CPI -> DATA Layer -> HDR -> rsp_header[40] Epoch Valid | Disabled since it is Epoch |
|        |         |                 |          |                    | CPI -> DATA Layer -> HDR -> rsp_header[41] Epoch ID | Disabled since it is Epoch |
|        |         |                 |          |                    | CPI -> DATA Layer -> HDR -> rsp_header[42+] Port ID | Disabled since it is Epoch |
|**S2M** |**DRS**  |**spid**         | **12**   | **PBR**            | CPI -> DATA Layer -> HDR -> rsp_spid | undefinition in CXL spec
|**S2M** |**DRS**  |**dpid**         | **12**   | **PBR**            | CPI -> DATA Layer -> HDR -> rsp_dpid
|**S2M** |**DRS**  |**databody**     | **512**  | **68B, 256B, PBR** | CPI -> DATA Layer -> PAYLOAD -> data_body
|**S2M** |**DRS**  |**enable**       | **64**   | **68B, 256B, PBR** | CPI -> DATA Layer -> PAYLOAD -> data_byte_enable
|**S2M** |**DRS**  |**poison**       | **1**    | **68B, 256B, PBR** | CPI -> DATA Layer -> PAYLOAD -> data_poison
|        |         |                 |          |                    | CPI -> DATA Layer -> PAYLOAD -> data_partity | Disabled since it is in sv
|        |         |                 |          |                    | CPI -> DATA Layer -> EOP | Disabled since it is Handshake
|        |         |                 |          |                    | CPI -> DATA Layer -> CREDIT | Disabled since it is Flow Control
|**S2M** |**DRS**  |**trp**          | **1**    | **256B, PBR**      | not in CPI and CXL3.2 add it
|**S2M** |**DRS**  |**emd**          | **32**   | **256B, PBR**      | not in CPI and CXL3.2 add it
|
|        |         |                 |          |                    | CPI -> REQ  Layer -> VALID | Disabled since it is Handshake
|        |         |                 |          |                    | CPI -> REQ  Layer -> FLOW_C | Disabled since it is Flow Control
|**M2S** |**Req**  |**memopcode**    | **4**    | **68B, 256B, PBR** | CPI -> REQ  Layer -> HDR -> req_header[3:0] MemOpcode (see Table 4-7 right)
|**M2S** |**Req**  |**tag**          | **16**   | **68B, 256B, PBR** | CPI -> REQ  Layer -> HDR -> req_header[19:4] Tag
|        |         |                 |          |                    | CPI -> REQ  Layer -> HDR -> req_header[21:20] TC | Disabled
|**M2S** |**Req**  |**snptype**      | **3**    | **68B, 256B, PBR** | CPI -> REQ  Layer -> HDR -> req_header[24:22] SnpType
|**M2S** |**Req**  |**address5**     | **1**    | **68B**            | CPI -> REQ  Layer -> HDR -> req_header[25] Address
|**M2S** |**Req**  |**metafield**    | **2**    | **68B, 256B, PBR** | CPI -> REQ  Layer -> HDR -> rsp_header[27:26] MetaField
|**M2S** |**Req**  |**metavalue**    | **2**    | **68B, 256B, PBR** | CPI -> REQ  Layer -> HDR -> rsp_header[29:28] MetaValue
|        |         |                 |          |                    | CPI -> REQ  Layer -> HDR -> req_header[30] AddressPartity | Disabled since it is in sv
|**M2S** |**Req**  |**address**      | **46**   | **68B, 256B, PBR** | CPI -> REQ  Layer -> HDR -> req_header[76:31] Address
|**S2M** |**Req**  |**ldid**         | **4**    | **68B, 256B**      | CPI -> REQ  Layer -> HDR -> rsp_header[80:77] LDID
|**M2S** |**Req**  |**flitmode**     | **2**    | **68B, 256B, PBR** | CPI -> REQ  Layer -> HDR -> req_header[82:81] FlitMode
|        |         |                 |          |                    | CPI -> REQ  Layer -> HDR -> req_cmd_parity |  Disabled since it is in sv
|**M2S** |**Req**  |**spid**         | **12**   | **PBR**            | CPI -> REQ  Layer -> HDR -> req_spid
|**M2S** |**Req**  |**dpid**         | **12**   | **PBR**            | CPI -> REQ  Layer -> HDR -> req_dpid
|        |         |                 |          |                    | CPI -> REQ  Layer -> CREDIT |  Disabled since it is Flow Control
|
|        |         |                 |          |                    | CPI -> RSP  Layer -> VALID | Disabled
|        |         |                 |          |                    | CPI -> RSP  Layer -> FLOW_C | Disabled since it is Flow Control
|**M2S** |**BIRsp**|**opcode**       | **4**    | **256B, PBR**      | CPI -> RSP  Layer -> HDR -> rsp_header[3:0] Opcode (see Table 4-17 right)
|**M2S** |**BIRsp**|**ldid**         | **12**   | **256B**           | CPI -> RSP  Layer -> HDR -> rsp_header[15:4] BI-ID | name is changed in Cpp Model
|**M2S** |**BIRsp**|**bitag**        | **12**   | **256B, PBR**      | CPI -> RSP  Layer -> HDR -> rsp_header[27:16] BITag
|        |         |                 |          |                    | CPI -> RSP  Layer -> HDR -> rsp_header[29:28] LowAddr | Disabled
|**M2S** |**BIRsp**|**flitmode**     | **2**    | **256B, PBR**      | CPI -> RSP  Layer -> HDR -> rsp_header[31:30] Flit Mode
|        |         |                 |          |                    | CPI -> RSP  Layer -> HDR -> req_cmd_parity | Disabled since it is in sv
|**M2S** |**BIRsp**|**spid**         | **12**   | **PBR**            | CPI -> RSP  Layer -> HDR -> rsp_spid
|**M2S** |**BIRsp**|**dpid**         | **12**   | **PBR**            | CPI -> RSP  Layer -> HDR -> rsp_dpid
|        |         |                 |          |                    | CPI -> RSP  Layer -> CREDIT |  Disabled since it is Flow Control
|
|        |         |                 |          |                    | CPI -> DATA Layer -> VALID | Disabled
|        |         |                 |          |                    | CPI -> DATA Layer -> FLOW_C | Disabled since it is Flow Control
|**M2S** |**RwD**  |**memopcode**    | **4**    | **256B, PBR**      | CPI -> DATA Layer -> HDR -> req_header[3:0] MemOpcode (see Table 4-12 right)
|**M2S** |**RwD**  |**metafield**    | **2**    | **256B, PBR**      | CPI -> DATA Layer -> HDR -> rsp_header[5:4] MetaField
|**M2S** |**RwD**  |**metavalue**    | **2**    | **256B, PBR**      | CPI -> DATA Layer -> HDR -> rsp_header[7:6] MetaValue
|**M2S** |**RwD**  |**snptype**      | **3**    | **256B, PBR**      | CPI -> DATA Layer -> HDR -> req_header[10:8] SnpType
|        |         |                 |          |                    | CPI -> DATA Layer -> HDR -> req_header[12:11] TC | Disabled
|        |         |                 |          |                    | CPI -> DATA Layer -> HDR -> req_header[15] AddressPartity | Disabled since it is in sv
|**M2S** |**RwD**  |**tag**          | **16**   | **68B, 256B, PBR** | CPI -> DATA Layer -> HDR -> req_header[54:39] Tag
|**M2S** |**RwD**  |**address**      | **46**   | **68B, 256B, PBR** | CPI -> DATA Layer -> HDR -> req_header[77:55][38:16] Address
|**S2M** |**RwD**  |**ldid**         | **4**    | **68B, 256B**      | CPI -> DATA Layer -> HDR -> rsp_header[81:78] LDID
|**M2S** |**RwD**  |**flitmode**     | **2**    | **68B, 256B, PBR** | CPI -> DATA Layer -> HDR -> req_header[83:82] FlitMode
|**M2S** |**RwD**  |**spid**         | **12**   | **PBR**            | CPI -> DATA Layer -> HDR -> rsp_spid
|**M2S** |**RwD**  |**dpid**         | **12**   | **PBR**            | CPI -> DATA Layer -> HDR -> rsp_dpid
|**M2S** |**RwD**  |**databody**     | **512**  | **68B, 256B, PBR** | CPI -> DATA Layer -> PAYLOAD -> data_body
|**M2S** |**RwD**  |**enable**       | **64**   | **68B, 256B, PBR** | CPI -> DATA Layer -> PAYLOAD -> data_byte_enable
|**M2S** |**RwD**  |**poison**       | **1**    | **68B, 256B, PBR** | CPI -> DATA Layer -> PAYLOAD -> data_poison
|        |         |                 |          |                    | CPI -> DATA Layer -> PAYLOAD -> data_partity | Disabled since it is in sv
|        |         |                 |          |                    | CPI -> DATA Layer -> EOP | Disabled
|        |         |                 |          |                    | CPI -> DATA Layer -> CREDIT | Disabled since it is Flow Control
|**M2S** |**RwD**  |**trp**          | **1**    | **256B, PBR**      | not in CPI and CXL3.2 add it
|**M2S** |**RwD**  |**emd**          | **32**   | **256B, PBR**      | not in CPI and CXL3.2 add it

## UIO
> 硬件中是 2 通道，Cpp Model 中被拆分为 3 通道并重新命名

|        | Channel | Name            | Bit      | FlitMode           | Placement of Definition                                           | Description |
| :----  | :------ | :-------------- | :------- | :--------------    | :---------------------------------------------------------------- | :----------------------------- |
| **M2S** | **RWAddr** | **opcode**    | **1**     | **PBR** | | 1:write; 0:read
| **M2S** | **RWAddr** | **spid**      | **12**    | **PBR** |
| **M2S** | **RWAddr** | **dpid**      | **12**    | **PBR** |
| **M2S** | **RWAddr** | **address**   | **46**    | **PBR** |
| **M2S** | **RWAddr** | **data**      | **512**   | **PBR** |
| **M2S** | **RWAddr** | **metavalue** | **2**     | **PBR** |
| **M2S** | **RWAddr** | **tagidx**    | **8**     | **PBR** |
|
| **S2M** | **RdData** | **rddata**    | **512**   | **PBR** |
| **S2M** | **RdData** | **rderr**     | **1**     | **PBR** |
| **S2M** | **RdData** | **rdtagidx**  | **8**     | **PBR** |
|
| **S2M** | **WrRsp**  | **wrerr**     | **1**     | **PBR** |
| **S2M** | **WrRsp**  | **wrtagidx**  | **8**     | **PBR** |

## AXI-FULL
> 硬件中是 5 通道，Cpp Model 中被合并为 4 通道并重新命名，其中 AXI-FULL 的 AWC 和 WC 被合并

> 以下 **S2D** **D2S** 的 **D** 是 **DDR_SubSystem** <br>

|        | Channel | Name            | Bit      | FlitMode           | Placement of Definition                                           | Description |
| :----  | :------ | :-------------- | :------- | :--------------    | :---------------------------------------------------------------- | :----------------------------- |
|  **S2D**  | **RdAddr**  |  **id**      | **4**     | **68B, 256B, PBR** |  |
|  **S2D**  | **RdAddr**  |  **address** | **64**    | **68B, 256B, PBR** |  |
|  **S2D**  | **RdAddr**  |  **user**    | **64**    | **68B, 256B, PBR** |  | ?
|
|  **D2S**  | **RdData**  |  **id**      | **4**     | **68B, 256B, PBR** |  |
|  **D2S**  | **RdData**  |  **data**    | **512**   | **68B, 256B, PBR** |  |
|  **D2S**  | **RdData**  |  **derr**    | **64**    | **68B, 256B, PBR** |  | ?
|  **D2S**  | **RdData**  |  **resp**    | **2**     | **68B, 256B, PBR** |  |
|  **D2S**  | **RdData**  |  **user**    | **42**    | **68B, 256B, PBR** |  |
|
|  **S2D**  | **WrAddr**  |  **id**      | **4**     | **68B, 256B, PBR** |  |
|  **S2D**  | **WrAddr**  |  **address** | **64**    | **68B, 256B, PBR** |  |
|  **S2D**  | **WrAddr**  |  **data**    | **512**   | **68B, 256B, PBR** |  |
|  **S2D**  | **WrAddr**  |  **derr**    | **64**    | **68B, 256B, PBR** |  | ?
|  **S2D**  | **WrAddr**  |  **strb**    | **64**    | **68B, 256B, PBR** |  |
|  **S2D**  | **WrAddr**  |  **auser**   | **42**    | **68B, 256B, PBR** |  | Disabled
|  **S2D**  | **WrAddr**  |  **user**    | **42**    | **68B, 256B, PBR** |  |
|
|  **D2S**  | **WrRsp**   |  **id**      | **4**     | **68B, 256B, PBR** |  |
|  **D2S**  | **WrRsp**   |  **resp**    | **2**     | **68B, 256B, PBR** |  |
|  **D2S**  | **WrRsp**   |  **user**    | **64**    | **68B, 256B, PBR** |  | ?


## AXI_LITE, APB

> 用共享变量池代替

## CDC
> ?

# 二、代码实现

目前：各 channel_msg 的写法统一，但还有如下缺点：
- 比如 tag 和 address 都是 unsigned 类，channel_msg 实例化时，调用的构造函数时 tag 和 address 输反，构造函数不会报错
- channel_msg 的代码的重复性偏高
> 为了保证代码可读性，目前没有做优化的计划