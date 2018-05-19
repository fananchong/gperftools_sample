FROM centos:centos7.3.1611

RUN rpm --import /etc/pki/rpm-gpg/RPM-GPG-KEY-CentOS-7                  && \
    yum install -y gcc gcc-c++ make python-pip initscripts git          && \
    yum install -y libtool-ltdl sudo bzip2.x86_64 which cmake           && \
    yum install -y libstdc++-static glibc-static wget unzip             && \
    yum clean all
    
RUN rpm --import /etc/pki/rpm-gpg/RPM-GPG-KEY-CentOS-7                  && \
    yum install -y autoconf automake libtool && \
    yum clean all
    
VOLUME ["/tmp/home"]
