# AFEPack 安装指南 2024年完全安装AFEPack旧版本
安装所需要的依赖文件都在北大网盘里
[北大网盘分享安装AFEPack所需要的文件](https://disk.pku.edu.cn/link/AA98F6841A162F49899E3A551611941561)

综合网上介绍的各种经验介绍AFEPack在Ubuntu16.04下的安装成功经验。
参考的链接有
```
# 北京大学数学科学学院平台上的安装版本

#github 上介绍的安装方式
https://github.com/wangheyu/AFEPack
https://github.com/lake1115/AFEPack
```

## 配置环境简介
Windows10操作系统 
```
版本	Windows 10 专业版
版本号	22H2
安装日期	2024/3/6
操作系统内部版本	19045.4291
体验	Windows Feature Experience Pack 1000.19056.1000.0
处理器	AMD Ryzen 7 5700X 8-Core Processor                3.40 GHz
机带 RAM	32.0 GB
```
WSL2 下安装的ubuntu16.04子系统
配置WSL2的过程可以在网上搜索，比较全面了不再赘述。
由于ubuntu16.04的微软的应用商店里已经下架。该文件在提供的材料文件中。下载以.appx后缀的文件后，在打开方式里选择应用安装程序，弹出终端后，稍等几分钟后就进入了设置用户名和密码。

## 安装流程
### 配置ubuntu
从效率的角度配置完成后，把ubuntu换为国产源。这样下载配置文件的速度更快。

### 设置gcc 和 g++ 的版本

由于gcc 和 g++的在ubuntu是5.4.0比较高。这里把gcc 和 g++ 的版本换成低版本的4.9.3.选择Ubuntu16.04原因也是该版本的源提供的gcc版本比较低。

在命令行中键入
```shell
sudo apt-get install build-essential
sudo apt-get install gcc-4.9 g++-4.9 gfortran-4.9
```
这里可以看到系统中安装了两个版本的gcc,把gcc4.9设置为gcc的默认值,在命令行中
```shell
sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-4.9 49 --slave /usr/bin/g++ g++ /usr/bin/g++-4.9
sudo update-alternatives --install /usr/bin/gfortran gfortran /usr/bin/gfortran-4.9 49
```
然后把gcc4.9的优先级设定最高，分别在命令行中执行以下命令，设置优先级
```shell
sudo update-alternatives --config gcc
sudo update-alternatives --config gfortran
```

### 安装mpi
之前遇到过高版本的mpi出国的问题，这里选择的mpi版本是openmpi-2.0.4.
在命令行中依次输入
```shell
tar -zxvf openmpi-2.0.4.tar.gz #解压缩
cd openmpi-2.0.4 #进入目录
CC=/usr/bin/gcc-4.9 CXX=/usr/bin/g++-4.9 FC=/usr/bin/gfortran-4.9 ./configure --prefix=/home/yxc/software/openmpi
make
make install
```
一些说明，安装openmpi的参数含义是
- `CC` 指示gcc编译器
- `CXX` 指示g++编译器
- `--prefix=` 指示安装openmpi的目录
- 如果在个人服务器上出现权限不足的问题，记得在`make,make install`中前加`sudo`
- 其他在安装openmpi用到的参数，在命令行中输入`./configure --help`查看
- 安装mpi的时间稍稍有点长，大约需要几分钟。
**配置环境变量**

请参考我的`.bashrc`文件
```shell
# openmpi
MPI_HOME=/home/yxc/software/openmpi
export PATH=${MPI_HOME}/bin:$PATH
export LD_LIBRARY_PATH=$MPI_HOME/lib:$LD_LIBRARY_PATH
export INCLUDE=${MPI_HOME}/include:$INCLUDE
export CPATH=${MPI_HOME}/include:$CPATH
export MANPATH=${MANPATH}/share/man:$MANPATH
```
注意路径`/home/yxc/software`是我个人电脑的路径，其他设备在安装时，记得更换。

### 安装boost1.50.0
Boost库是为C++语言标准库提供扩展的一些C++程序库的总称。这里安装的版本是1.50.0.
在命令行中依次输入
```
tar -jxvf boost_1_50_0.tar.bz2 #解压缩文件
cd boost_1_50_0/ #进入目录 下面是安装
./bootstrap.sh --prefix=/home/yxc/software/boost
./b2 --with=all -j 12 install
./b2 install
```
参数说明
- `--prefix=`是boost的安装目录
- `-j 12`是开12线程并行编译，因为我的cpu是8核心16线程，为了减少编译的时间可以根据自己的cpu选择合适的数字。
- 如果出现权限不足的问题，记得在`./b2`中前加`sudo`

**配置环境变量**
参考我的文件配置环境变量
```shell
# boost
BOOST_DIR=/home/yxc/software/boost
export CPLUS_INCLUDE_PATH=${BOOST_DIR}/include:$CPLUS_INCLUDE_PATH
export LD_LIBRARY_PATH=${BOOST_DIR}/lib:$LD_LIBRARY_PATH
export LIBRARY_PATH=${BOOST_DIR}/lib:$LIBRARY_PATH
```
在boost安装完成后，下面是一个小例子，来验证是否已经安装成功
源文件`example.cpp`
```cpp
#include <iostream>
#include <boost/algorithm/string.hpp>

int main() {
    std::string str = "Hello, World!";
    boost::to_upper(str);
    std::cout << str << std::endl;
    return 0;
}
```
编译和运行
```shell
g++ -o example example.cpp -lboost_system -lboost_filesystem
./example
```

### 安装deal.ii 8.1.0
deal.ii是一种自适应有限元的软件包，由于8.1.0是比较老的版本，现在已经不容易找到了，请在依赖文件中下载。AFEPack中会用到deal.ii线性代数部分，以及一些求解器。

安装deal.ii之前先安装cmake
```
sudo apt-get install cmake 
```
我的cmake的版本是3.5.1.

在命令行中依次执行下面的命令,使用cmake进行安装。
```shell
tar -zxvf dealii-8.1.0.tar.gz 
cd deal.II/
mkdir build
cd build/
export BOOST_DIR=/home/yxc/software/boost
cmake -DCMAKE_INSTALL_PREFIX=/home/yxc/software/dealii -DDEAL_II_WITH_MPI=on -DDEAL_II_WITH_THREADS=off -DCMAKE_C_COMPILER=/home/yxc/software/openmpi/bin/mpicc -DCMAKE_CXX_COMPILER=/home/yxc/software/openmpi/bin/mpicxx -DCMAKE_Fortran_COMPILER=/home/yxc/software/openmpi/bin/mpifort -DDEAL_II_WITH_PETSC=off -DDEAL_II_WITH_BOOST=on ../
make -j 8
make install
```
这一部的安装容易出错，一定要注意下面几点
- 参数`-DCMAKE_C_COMPILER`,`-DCMAKE_CXX_COMPILER`,`-DCMAKE_Fortran_COMPILER`是对应mpi编译器的路径，根据你mpi安装的位置决定。
- `-DCMAKE_INSTALL_PREFIX`后面是deal.ii的安装路径
- 参数必不可少`-DDEAL_II_WITH_MPI=on -DDEAL_II_WITH_THREADS=off mpifort -DDEAL_II_WITH_PETSC=off -DDEAL_II_WITH_BOOST=on`
- `cmake`命令最后的`../`必不可少，它是来指定CmakeList的路径；
- deal.ii中自带了一个1.49.0版本的boost一定要用外置的1.50.0的boost. boost的cmake相关参数的设定在路径`/deal.II/cmake/configure/configure_boost.cmake`请参考这里的设定。

作为参考，下面是我的build目录下cmake之后的`detailed.log `文件,
```
###
#
#  deal.II configuration:
#        CMAKE_BUILD_TYPE:       DebugRelease
#        BUILD_SHARED_LIBS:      ON
#        CMAKE_INSTALL_PREFIX:   /home/yxc/software/dealii
#        CMAKE_SOURCE_DIR:       /home/yxc/software/deal.II (Version 8.1.0)
#        CMAKE_BINARY_DIR:       /home/yxc/software/deal.II/build
#        CMAKE_CXX_COMPILER:     GNU 4.9.3 on platform Linux x86_64
#                                /home/yxc/software/openmpi/bin/mpicxx
#        CMAKE_C_COMPILER:       /home/yxc/software/openmpi/bin/mpicc
#        CMAKE_Fortran_COMPILER: /home/yxc/software/openmpi/bin/mpifort
#        CMAKE_GENERATOR:        Unix Makefiles
#
#  Compiler flags used for this build:
#        CMAKE_CXX_FLAGS:              -pedantic -fpic -Wall -Wpointer-arith -Wwrite-strings -Wsynth -Wsign-compare -Wswitch -Wno-unused-local-typedefs -Wno-long-long -Wno-deprecated -Wno-deprecated-declarations -std=c++11
#        DEAL_II_CXX_FLAGS_RELEASE:    -O2 -funroll-loops -funroll-all-loops -fstrict-aliasing -Wno-unused
#        DEAL_II_CXX_FLAGS_DEBUG:      -Og -ggdb -Wa,--compress-debug-sections
#        DEAL_II_LINKER_FLAGS:         -Wl,--as-needed -rdynamic  -Wl,-rpath  -Wl,/home/yxc/software/openmpi/lib  -Wl,--enable-new-dtags
#        DEAL_II_LINKER_FLAGS_RELEASE: 
#        DEAL_II_LINKER_FLAGS_DEBUG:   -ggdb
#
#  Configured Features (DEAL_II_ALLOW_BUNDLED = ON, DEAL_II_ALLOW_AUTODETECTION = ON):
#      ( DEAL_II_WITH_64BIT_INDICES = OFF )
#      ( DEAL_II_WITH_ARPACK = OFF )
#        DEAL_II_WITH_BOOST set up with external dependencies
#            BOOST_VERSION = 1.50.0
#            BOOST_DIR = /home/yxc/software/boost
#            BOOST_INCLUDE_DIRS = /home/yxc/software/boost/include
#            BOOST_LIBRARIES = /home/yxc/software/boost/lib/libboost_serialization.so;/home/yxc/software/boost/lib/libboost_system.so
#        DEAL_II_WITH_FUNCTIONPARSER set up with bundled packages
#      ( DEAL_II_WITH_HDF5 = OFF )
#      ( DEAL_II_WITH_LAPACK = OFF )
#      ( DEAL_II_WITH_METIS = OFF )
#        DEAL_II_WITH_MPI set up with external dependencies
#            MPI_VERSION = 3.1
#            OMPI_VERSION = 2.0.4
#            MPI_CXX_COMPILER = /home/yxc/software/openmpi/bin/mpicxx
#            MPI_CXX_COMPILER = /home/yxc/software/openmpi/bin/mpicxx
#            MPI_CXX_COMPILE_FLAGS = 
#            MPI_CXX_INCLUDE_PATH = /home/yxc/software/openmpi/include
#            MPI_CXX_LIBRARIES = /home/yxc/software/openmpi/lib/libmpi.so
#            MPI_CXX_LINK_FLAGS =  -Wl,-rpath  -Wl,/home/yxc/software/openmpi/lib  -Wl,--enable-new-dtags
#      ( DEAL_II_WITH_MUMPS = OFF )
#      ( DEAL_II_WITH_NETCDF = OFF )
#      ( DEAL_II_WITH_P4EST = OFF )
#      ( DEAL_II_WITH_PETSC = OFF )
#      ( DEAL_II_WITH_SLEPC = OFF )
#      ( DEAL_II_WITH_THREADS = OFF )
#      ( DEAL_II_WITH_TRILINOS = OFF )
#      ( DEAL_II_WITH_UMFPACK = OFF )
#      ( DEAL_II_WITH_ZLIB = OFF )
#
#  Component configuration:
#        DEAL_II_COMPONENT_COMPAT_FILES
#      ( DEAL_II_COMPONENT_DOCUMENTATION = OFF )
#        DEAL_II_COMPONENT_EXAMPLES
#        DEAL_II_COMPONENT_MESH_CONVERTER
#      ( DEAL_II_COMPONENT_PARAMETER_GUI = OFF )
#
###
```


**配置环境变量**
请参考我的文件`.bashrc`
```sh
#dealii
export CPLUS_INCLUDE_PATH=/home/yxc/software/dealii/include:$CPLUS_INCLUDE_PATH
export CPLUS_INCLUDE_PATH=/home/yxc/software/dealii/include/deal.II:$CPLUS_INCLUDE_PATH
```

### 安装AFEPack
安装AFEPack是旧版本的，大概在2019年不再更新。文件可以在王何宇老师的主页上下载。https://github.com/wangheyu/AFEPack

新版本的在逐步去掉对deal.ii依赖，完善了更多的功能，有杨凡意的版本和胡光辉老师的版本
```
https://github.com/yfygod/AFEPack
```
在我提供的依赖材料中也有.
首先安装必要的文件
```sh
sudo apt-get install autoconf 
sudo apt-get install automake 
```
依次在命令行中执行下面的命令，解压文件后，在`AFEPack-master`依次执行，
```
aclocal
autoconf
automake 
env CC="/home/yxc/software/openmpi/bin/mpicc" CXX="/home/yxc/software/openmpi/bin/mpicxx" CXXFLAGS="-std=c++0x" ./configure --prefix=/home/yxc/software/AFEPack --with-mpi=yes --with-dealii=/home/yxc/software/dealii --with-boost=/home/yxc/software/boost
make 
make install 
```
参数说明
- `--prefix`是AFEpack的安装路径
- `--with-mpi=yes`表示并行版本的安装，为了初学可以设置为`no`
- `--with-dealii=/home/yxc/software/dealii`指示dealii的安装目录
- `--with-boost=/home/yxc/software/boost`指示boost的安装目录

在安装完成后还需要设置模板单元的位置，方便起见，我都把它们加入到了环境变量
```sh
export CPLUS_INCLUDE_PATH=/home/yxc/software/AFEPack-master/library/include:$CPLUS_INCLUDE_PATH
# 安装路径
export AFEPACK_PATH=/home/yxc/software/AFEPack
# 2d triangle 
export AFEPACK_TEMPLATE_PATH=/home/yxc/software/AFEPack-master/template/triangle:$AFEPACK_TEMPLATE_PATH
# 1d interval
export AFEPACK_TEMPLATE_PATH=/home/yxc/software/AFEPack-master/template/interval:$AFEPACK_TEMPLATE_PATH
# 3d tetrahedron
export AFEPACK_TEMPLATE_PATH=/home/yxc/software/AFEPack-master/template/tetrahedron:$AFEPACK_TEMPLATE_PATH
```

为了检验AFEPack是否安装成功，可以在`AFEPack-master/example/possion_equation`目录下，分别执行
```sh
make #编译文件
~/software/AFEPack/bin/easymesh D #使用easymesh剖分网格
./main #D 执行程序
```
注意,我这里`easymesh`的位置是AFEPack的安装路径下bin目录下，觉得麻烦的可以加到环境变量中。

结果如下
```sh
Building degree of freedom for the FEM space ...
        total 506 degree of freedom found.
AMGSolver initializing ... OK! grid levels: 3
AMGSolver begin with initial residual 19.7721 ...
        converge with residual 3.02858e-08 at step 6.

L2 error = 0.00396625
```
这里表示安装成功了。

**配置动态链接库搜索路径**

AFEPack安装完成后，在安装路径`lib`的目录下有两个动态库文件,
```
libAFEPack_mpi.so  libAFEPack.so
```
一定要把这个路径加到动态链接库的搜索路径中，否则在编译时会找不到动态链接库。










