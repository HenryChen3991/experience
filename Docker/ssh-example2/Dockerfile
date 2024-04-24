# 使用官方的 Ubuntu 20.04 镜像作为基础镜像
FROM ubuntu:20.04

# 设置环境变量来定义用户名和密码
ENV USERNAME=henry
ENV PASSWORD=gemtek

# 更新 apt 并安装 openssh-server、sudo 和 net-tools
RUN apt-get update && \
    apt-get upgrade && \
    apt-get install -y openssh-server sudo net-tools && \
    apt-get clean && \
    rm -rf /var/lib/apt/lists/*

# 创建指定用户名的用户并设置密码
RUN useradd -m -s /bin/bash $USERNAME && \
    echo "$USERNAME:$PASSWORD" | chpasswd

# 将用户添加到 sudo 组
RUN adduser $USERNAME sudo

# 允许 SSH 登录密码认证
RUN sed -i 's/#PasswordAuthentication yes/PasswordAuthentication yes/' /etc/ssh/sshd_config

# 创建 SSH 需要的目录
RUN mkdir -p /var/run/sshd

# 暴露 SSH 端口
EXPOSE 22

# 设置启动命令为启动 SSH 服务
CMD ["/usr/sbin/sshd", "-D"]
