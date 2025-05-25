# Reference Model

关于环境配置。
关于代码风格规范。
关于[模块结构/设计思路](doc/rm/rm.md)

关于代码运行：
本地RM make

远程服务器 cpp+sv make







断言：assert
对于同一地址同一主机的BIRsp和BISnp不能同时发生

BIRsp 
    Tag：       | 1  |        |    |3 |
BISnp ：  0x01  |    | 0x01   |    |  |   0x01 
    Tag：  1    |    |   3    |    |  |     5 





可行性：


断言和彩色打印：（初步设想位置在dcoh的接口上）
Req/RwD  |          |  BIsnp*n BIRsp*n     NDS
         | Req/RwD  |  


可以使用 check 模块 向rm的反馈通路的






dve和cpp还不能联调，主要是因为vscode不好attach远程。
g佬的说法是 dve和cpp通过读写文件来进行交互，我觉得接口难写，但是应该都要实现？



BISnpData???  

