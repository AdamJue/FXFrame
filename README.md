# FXFrameTest

---
FXFrame是一个基于插件的跨平台框架，用于开发跨平台桌面应用程序。目前主要的功能中包含：事件循环，定时器回调。当前版本移除了网络传输功能。
本项目参考了 https://github.com/ketoo/NoahGameFrame
另外还有 https://github.com/dzgycbs/VanFrame
重新整理了整个项目的架构，用CMake构建，使用vcpkg管理库。

## 编译之前 (因为移除了网络功能，这一步骤不需要了)
在编译之前，请更新生成一下proto文件。 
- Windows下我已经配置好了，如果没有修改不需要生成，如果需要生成，请在FxMessageDefine中执行以下命令文件：cpp.bat
- 在Ubuntu中，需要先执行一下生成，请执行以下命令：cpp.sh (如果没有权限运行该文件，请执行chmod +x cpp.sh)

## 编译流程 
- 设置vcpkg路径(如果习惯使用CMakePresets.json，也可以在CMakePresets.json直接配置)
 
**clion中配置 build settings**
``` 
-DCMAKE_TOOLCHAIN_FILE=/CHANGE TO YOU vcpkg INSTALL PATH/scripts/buildsystems/vcpkg.cmake
```
- 重载CMakeLists.txt，等待vcpkg安装库完成
- 按次序编译（优先编译FXMessageDefine）

## 运行
运行的一种方法当然是使用IDE启动，但是也可以使用命令行启动。当前启动的时候，我们需要一些附加参数，比如启动gui如下：
```
Server=GUI ID=1 ../../config/GUIPlugin.xml
```

这个参数可以添加在IDE中的运行参数中。
另外为了方便运行，在config目录中，我添加了对应平台下的启动脚本run_gui.bat、run_mesh_server.bat还有Ubuntu下的run_gui.sh、run_mesh_server.sh，
在编译完成之后，可以复制到对应目录下，然后运行。

---
FXFrame is a plugin based cross-platform framework used for developing cross-platform desktop applications. The main functions currently include event loop and timer callback. The current version has removed the network transmission function.
This project refers to https://github.com/ketoo/NoahGameFrame
Additionally, there are https://github.com/dzgycbs/VanFrame
Reorganized the architecture of the entire project, built it using CMake, and managed the library using vcpkg.

## Before compilation (Because the network function has been removed, this step is no longer necessary)
Before compiling, please update and generate the proto file.
- I have already configured it under Windows. If there are no modifications, it does not need to be generated. If it needs to be generated, please execute the following command file in FxMessage Define: cpp.bat
- In Ubuntu, it is necessary to first perform the generation. Please execute the following command: cpp.sh (If you do not have permission to run the file, please execute chmod+x cpp.sh)

    

## Compile
- Set the vcpkg path (if you are accustomed to using CMakePresets.json, you can also configure it directly in CMakePresets.json)

**Configure build settings in clion**
``` 
-DCMAKE_TOOLCHAIN_FILE=/CHANGE TO YOU vcpkg INSTALL PATH/scripts/buildsystems/vcpkg.cmake
```
- Reload CMakeLists.txt, wait for vcpkg to install the library
- Compile in order (priority to compile FXMessageDefine)

## Run
One of the methods to run is to start it using the IDE, but you can also start it using the command line.
When running, we need some additional parameters, such as starting gui as follows:
```
Server=GUI ID=1 ../../config/GUIPlugin.xml
```
  
This parameter can be added to the runtime parameters in the IDE.
  In addition, for the convenience of running, I have added startup scripts run_gui.bat, run_mesh_server.bat for the corresponding platform in the config directory, as well as run_gui.sh for Ubuntu run_mesh_server.sh，
  After compilation is completed, it can be copied to the corresponding directory and then run.
