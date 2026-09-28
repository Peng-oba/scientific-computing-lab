# Day 6：引入 Eigen

本项目使用本机已有的 `E:/Eigen/eigen-3.4.0`，编译器为
`D:/MinGW15.2/mingw64/bin/g++.exe`，C++ 标准为 C++17。
`E:/Eigen/eigen-3.3.9` 保留，但不加入本项目的搜索路径。

Eigen 的基础使用只需要头文件，无须编译、安装或链接额外的 Eigen 库。
源码中现有的写法已经正确：

```cpp
#include <Eigen/Dense>
```

搜索路径必须是包含 `Eigen` 文件夹的目录，即 `E:/Eigen/eigen-3.4.0`，
编译器据此找到 `E:/Eigen/eigen-3.4.0/Eigen/Dense`。
不要把搜索路径设成 `E:/Eigen` 或 `E:/Eigen/eigen-3.4.0/Eigen`。

## 在 VS Code 中使用

1. 用 VS Code 打开整个 `E:/scientific-computing-lab` 文件夹。
2. 若已安装 Microsoft C/C++ 扩展，`.vscode/c_cpp_properties.json` 会为它提供
   Eigen 头文件路径、编译器位置和代码提示配置。
3. 按 `Ctrl+Shift+B` 执行 `Build day6 with Eigen`，
   自动创建 `build` 目录并生成 `build/day6_material_state.exe`。
4. 在菜单“终端 → 运行任务”中选择 `Run day6 with Eigen`，即可先编译再运行。
5. 使用 Microsoft C/C++ 扩展时，在“运行和调试”中选择
   `Debug day6 with Eigen`，按 `F5` 调试，或按 `Ctrl+F5` 不调试运行。
   `.vscode/launch.json` 已配置本机 GDB，并在启动前调用编译任务。

代码提示配置与真正的编译参数是两回事；`.vscode/tasks.json` 中的 `-I`
负责让编译器找到 Eigen。

### 使用 Code Runner 的 Run Code

项目的 `.vscode/settings.json` 已为 Code Runner 单独配置 C++ 编译命令，
包含 `-I 'E:/Eigen/eigen-3.4.0'`、C++17 和 UTF-8 参数。
在当前使用的 PowerShell 终端中，打开 C++ 文件后点击 **Run Code**
（或按 `Ctrl+Alt+N`）即可编译运行。运行前自动保存当前文件。
编译产物为 `build/<源文件名>.exe`，仅在编译成功后运行。

这项设置作用于本工作区的 C++ 文件；需用 VS Code 打开整个仓库根目录。
若仍显示旧的 `g++ 文件名 -o 文件名` 命令，可执行命令面板中的
`Developer: Reload Window` 后重试。

Code Runner 不会自动读取 `tasks.json` 或 `c_cpp_properties.json` 的编译参数。
此前 Run Code 报 `Eigen/Dense: No such file or directory`，正是因为它的默认命令
没有 `-I` 参数，而非源码中 `#include <Eigen/Dense>` 写错。

当前 `day6_material_state.cpp` 已有 `main()`，运行输出为 `0.1`。
旧任务的 `-c` 只编译、不链接，生成的 `.o` 不能直接运行；现已移除该参数，
改为生成 `.exe`。源码保留原样，可继续完成练习。

## 在 PowerShell 中编译

在仓库根目录执行：

```powershell
New-Item -ItemType Directory -Force -Path build | Out-Null
& 'D:/MinGW15.2/mingw64/bin/g++.exe' -std=c++17 -Wall -Wextra -g -finput-charset=UTF-8 -fexec-charset=UTF-8 -I 'E:/Eigen/eigen-3.4.0' 'cpp/cpp_memory_week1/day6_material_state.cpp' -o 'build/day6_material_state.exe'
if ($LASTEXITCODE -eq 0) { & './build/day6_material_state.exe' }
```

使用结构体时应初始化 `damage`、`kappa`，并给动态向量指定长度，例如：

```cpp
MaterialState state{0.0, 0.0, Eigen::VectorXd::Zero(6)};
state.stress(0) = 100.0;
```

## 切换版本或更换电脑

若要使用 3.3.9，将 `.vscode/c_cpp_properties.json`、`.vscode/tasks.json`
和 `.vscode/settings.json` 中的 `E:/Eigen/eigen-3.4.0`
同时改为 `E:/Eigen/eigen-3.3.9`，然后重新编译。一次只配置一个版本。
换电脑后也要更新这三个文件中的 Eigen 路径与编译器路径，以及
`launch.json` 中的 GDB 路径。

所有新增文本文件均使用 UTF-8，编译任务显式指定 UTF-8 输入和执行字符集。
编译产物放在已由 `.gitignore` 忽略的 `build/` 目录。

参考：[Eigen 官方入门说明](https://eigen.tuxfamily.org/dox/GettingStarted.html)。
