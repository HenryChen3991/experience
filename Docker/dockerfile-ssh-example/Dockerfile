FROM ubuntu:20.04
WORKDIR /

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get -y update
RUN apt-get -y install openssh-server
CMD echo "hello dockerUser"
RUN mkdir /var/run/sshd

#指定root密码
RUN /bin/echo 'root:gemtek'|chpasswd
RUN /bin/sed -i 's/.*session.*required.*pam_loginuid.so.*/session optional pam_loginuid.so/g' /etc/pam.d/sshd
RUN /bin/echo -e "LANG=\"en_US.UTF-8\"" > /etc/default/local

# ssh setting
RUN echo "root:gemtek" >> ~/passwdfile && \
    chpasswd -c SHA512 < ~/passwdfile && \
    rm ~/passwdfile && \
    sed -i "s/#Port.*/Port 22/" /etc/ssh/sshd_config && \
    sed -i "s/#PermitRootLogin.*/PermitRootLogin yes/" /etc/ssh/sshd_config && \
    sed -i "s/#PasswordAuthentication.*/PasswordAuthentication yes/" /etc/ssh/sshd_config

# expose the port 22(which is the default port of ssh)
EXPOSE 22

# set entrypoint to restart ssh automatically
CMD /usr/sbin/sshd -D


