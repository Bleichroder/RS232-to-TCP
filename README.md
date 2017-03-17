# RS232 to TCP
用于将串口消息转为tcp消息，适用于串口消息的远距离传输和多台传输

## transmit system
* 一个tcp的client-server系统，负责将server端收到的串口消息通过tcp包发给client端，再由client端将tcp包转为串口消息发出，client端收到返回的串口消息后打包成tcp包返回给server端，server端再将其转为串口消息返回给连接的COM口，可以有多个client，即可实现串口消息的复制和多发  
</br>

* 使用方法：
 1. 将编译好的server程序（位于transfer_tcp_server文件夹内）和client程序（位于transfer_tcp_client文件夹内）分别安装到目标机器上
 2. 配置server程序配套的config.ini，选择与server程序连接的COM口，打开server程序
 3. 配置client程序配套的config.ini，输入server程序所在机器的IP地址，选择与client程序连接的COM口，打开client程序
 4. 发送给server程序的串口命令需满足以下格式：  
 | 之后要发送的数据长度（4位） | 目标机器的IP（8位16进制数） | 指令 |  
 例：  
 0020c0a8039d0008MW01ENMD  
 0020：之后数据长度为20位  
 c0a8039d：目标机器IP，即192.168.3.157  
 0008MW01ENMD：指令
 5. 注：串口发送的数据和tcp包数据均采用utf-8编码

## transmit server
* 只包含tcp的server部分，负责将收到的串口消息转为tcp包，或将收到的tcp包转为串口消息发送给连接的COM口，使用需配置配套的config.ini，对发送的消息没有格式要求，只进行转发，注意需要采用utf-8编码，可以搭配自己编写的tcp client进行数据发送
