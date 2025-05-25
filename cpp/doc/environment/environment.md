# 环境配置
>本地操作系统默认是 ***Windows OS***

>***Common Environment*** 中的环境必须配置

>***SAMT Environment***、***BUPT Environment*** 中的环境二选一配置

## 一、Common Environment

（1）下载 **Visual Studio Code v1.98.2** [下载链接](https://alist.bricknas.top/d/nas/Share/VSCode-win32-x64-1.98.2.zip?sign=6aqgc0gJYLDeOKjYnUh2OqTPraIz9fJxtPD6b4clBV0=:0)

（2）下载以下插件：
- **vscode插件 Chinese (Simplified) (简体中文) Language Pack for Visual Studio Code**
- **vscode插件 Remote - SSH**
- **vscode插件 C/C++**
- **vscode插件 GitLens — Git supercharged**

## 二、SAMT Environment

### 2.1 VPN
（1）下载 
**FortiClient v7.0+** [下载链接](
https://links.fortinet.com/forticlient/win/vpnagent)
> 用于 VPN 连接的 **IP 地址、用户名、密码** 见 **微信群** <br>

> 下载时可能要连接外网，下载完成后如果报错见：[VPN Error Case.pdf](./VPN%20Error%20Case.pdf)

### 2.2 免密连接和可视化界面
> 远程主机的 **IP 地址、用户名、密码** 见 **微信群**

（1）运行 **Windows PowerShell** ，执行以下命令生成生成公钥和私钥，一路回车直至成功生成：
  ```
  ssh-keygen -t rsa
  ```

  （2）执行以下命令打印公钥信息，并将公钥信息复制：
  ```
  cat ~/.ssh/id_rsa.pub
  ```
  
  （3）执行以下命令，并输入密码：
  ```
  ssh 用户名@172.32.9.153
  ```
  > **用户名**和**密码**因人而异

  （4）执行以下命令：
  ```
  mkdir -p ~/.ssh
  chmod 700 ~/.ssh
  cat <<EOF >> ~/.ssh/authorized_keys
  PUBKEY
  EOF
  chmod 600 ~/.ssh/authorized_keys
  cp -r /d2/ic/cxl3x_memory_controller/03_cxl_subsys/uvm_share/environment/.vscode-server ~/
  ```
  > `PUBKEY` 是第（2）步中复制的公钥信息

（5）运行vscode → vscode最左边竖向的栏中 → 远程资源管理器 → 远程(隧道/SSH) → SSH → "SSH"右侧的"新建远程" → 输入以下命令并选择"C:\User\...\.ssh\config"：
  ```
  ssh 用户名@172.32.9.153
  ```
  > **用户名**因人而异

（6）下载 **RealVNC Viewer v7.13.0+** [下载链接](https://downloads.realvnc.com/download/file/viewer.files/VNC-Viewer-7.13.1-Windows.exe)
> 需要用 **dve** 的可以下载

### 2.3 SAMT 环境介绍和配置

> **make v3.8.2** <br>
> **vcs/V-2023.12_for_urg** <br>
> **gcc v11.3.0** <br>
> **gdb v10.1** <br>

（1）在远程执行以下命令：
```
cat <<EOF >> ~/.bashrc
source /d1/system/modules/module.bashrc
module add vcs/V-2023.12urg
export PATH=/d2/ic/cxl3x_memory_controller/03_cxl_subsys/uvm_share/environment/gdb-10.1/bin:\$PATH
export LD_LIBRARY_PATH=/usr/local/lib/gcc/x86_64-pc-linux-gnu/11.3.0/../../../../lib64:\$LD_LIBRARY_PATH
EOF
```
（2）**vscode插件 C/C++** 要在远程安装，其他自行选择
（3）**vscode插件 C/C++** 有 bug ，需要在本地先安装 gcc gdb 和远端相同的版本或以上

## 三、BUPT Environment

### 3.1 可选的开发方式
（1）下载以下插件：
- **vscode插件 GitLens — Git supercharged v17.0.3**

  > 见：[git协作开发](git.md)

（2）synopsys/vcs-mx/O-2018.09-SP2-5