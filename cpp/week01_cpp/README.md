# 第一周跟学包

先读《第一周_每日跟学执行手册.md》的第1天，在自己的练习目录写预测和程序；不要先浏览全部参考答案。

文件说明：

- 第一周_每日跟学执行手册.md：每天的分钟安排、概念、实验、预期结果、验收和补练规则。
- 第一周_自我监测与源码笔记模板.md：7天记录页、实验表、10条源码笔记、AI对照、周末验收。
- 练习起点/material_state.cpp：第5天带TODO的起点，未完成时返回退出码2。
- 参考答案/day1.cpp：四种传参和指针重新指向。
- 参考答案/day2.cpp：安全的生命周期观察。
- 参考答案/day3.cpp：通过CONST_CASE=1..8选择一个const编译案例。
- 参考答案/day4.cpp：复制与移动的最小观察。
- 参考答案/material_state.cpp：第5天六项实验和两个trial应用对照。
- 参考答案/lifetime_fault.cpp：独立的故意错误示例，可选用AddressSanitizer观察。
- 验证记录.md：编制时的检查结果，不代替你自己动手验证。

下面的命令假设终端位于本包根目录；先创建build目录。中文目录可直接在WSL中使用。

```bash
mkdir -p build
g++ -std=c++17 -Wall -Wextra -pedantic -O0 -g 参考答案/day1.cpp -o build/day1
./build/day1
```

第2天、第4天分别替换文件名和输出名即可。不要在编译命令中添加-DNDEBUG，否则参考程序内的assert检查可能被禁用。

第3天，C1、C3、C8应编译通过；C2、C4、C5、C6、C7应编译失败。逐个改宏值：

```bash
g++ -std=c++17 -Wall -Wextra -pedantic -DCONST_CASE=1 参考答案/day3.cpp -o build/day3
./build/day3
```

第5天先检查你本机的Eigen路径。下面只在/usr/include/eigen3确实包含Eigen/Dense时适用：

```bash
test -f /usr/include/eigen3/Eigen/Dense
g++ -std=c++17 -Wall -Wextra -pedantic -O0 -g -I/usr/include/eigen3 \
  练习起点/material_state.cpp -o build/material_state
./build/material_state
```

独立完成后才对照参考答案。若运行未完成的起点，显示TODO并返回2是预期行为；需要自己补齐函数和M1-M6检查，不要为消除提示而直接把return 2改为return 0。

可选的故意内存错误观察，预计程序诊断后退出：

```bash
g++ -std=c++17 -O0 -g -fsanitize=address -fno-omit-frame-pointer \
  参考答案/lifetime_fault.cpp -o build/lifetime_fault
./build/lifetime_fault
```

这些是教学代码，不包含你的OGS源码，不是统计损伤本构实现。第6天应打开你本地的真实OGS版本，按手册填写源码证据。
