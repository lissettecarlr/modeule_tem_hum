#温湿度模块封装
#模块功能简介
##1 网络
启动后首先将检查wifi信息表中是否有数据，如果有则使用它连接网络，再连接服务器。其余所有情况都将会使模块作为服务器。
##2 命令
###2.1 启动
每三秒数据更新一次，仅仅在数据正确的情况下才会上传。在没接收到停止命令时会一直上传。
###2.2 停止
仅仅在处于启动状态时有功效，停止上传
###2.3 复位
清除保存的WIFI信息，并且重启模块
###2.4得到WIFI消息
在接收到该命令后，再进入接收模块，如果收到WIFI信息则将其保存退出，如果在10秒内未接收到或者接收到数据格式错误也将退出
###2.5模式切换
变更为服务器模式
###2.6.存活确认
接收到该命令后返回一组数据位全为FF的一帧数据
##3 待机状态
该状态下每三秒更新一次传感器，每10秒发送一次存在确认
