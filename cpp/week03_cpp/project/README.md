# 第三周：AI 协作开发练习项目

配合 AI 协作修订版手册使用。学习者负责预期、约束、证据核查与结论；agent 可以完成导航、样板代码、构建、测试和日志整理。本项目为教学参考，不是完整材料模型或 OGS 的替代实现。

## 执行顺序

- 第 1 天：填写 `notes/task_card.md`，建立 Git 基线并检查正确程序。
- 第 2 天：核查 source→target→产物，演示旧二进制和零测试筛选。
- 第 3 天：用 `exercises/material_tests_starter.cpp` 替换参考测试，agent 根据你预先定义的行为补齐三个 TODO，由你核查断言；完成红→绿和容差假通过实验。
- 第 4 天：用 `exercises/accumulation.patch` 引入小故障，练习可证伪假设、真实 debugger 观察和修复。
- 第 5 天：分别构建、运行、解释并修复 sanitizer 故障程序。
- 第 6 天：到自己的真实 OGS 环境只读追踪测试；本包不含 OGS。
- 第 7 天：先独立预测新输入，再运行确认，完成验收。

## 正确基线

```bash
cmake -S . -B build/debug -G Ninja -DCMAKE_BUILD_TYPE=Debug
cmake --build build/debug --parallel 2
ctest --test-dir build/debug --output-on-failure --no-tests=error
./build/debug/material_demo
```

Linux/WSL、C++17、CMake ≥3.20、Ninja。正确参考基线为普通测试 4/4，通过时 demo 输出 first=10、second=10。构建和测试命令可以交给 agent，你仍需核对实际目录、目标和测试数量。

## GoogleTest 等价用例

已有 GTest CMake 包时：

```bash
cmake -S . -B build/gtests -G Ninja -DCMAKE_BUILD_TYPE=Debug -DLAB_WITH_GTEST=ON
cmake --build build/gtests --parallel 2
ctest --test-dir build/gtests --output-on-failure --no-tests=error
```

参考项目预期 8 个用例（普通4＋GoogleTest4）。项目不自动联网下载依赖；自定义前缀可通过 CMAKE_PREFIX_PATH 提供。环境未跑通时如实记为待补。

## sanitizer

```bash
cmake -S . -B build/sanitize -G Ninja -DCMAKE_BUILD_TYPE=Debug -DLAB_ENABLE_SANITIZERS=ON
cmake --build build/sanitize --parallel 2
ctest --test-dir build/sanitize --output-on-failure --no-tests=error
cmake --build build/sanitize --target fault_uaf fault_oob fault_overflow --parallel 2
```

`fault_uaf`、`fault_oob`、`fault_overflow` 是故意出错的独立程序，不属于正常 CTest。按手册分别运行并记录退出码、报告和修复证据。

若明确遇到 LeakSanitizer 的 ptrace 环境限制，参照手册的单次处理，并注明未验证泄漏。不要用关闭地址检测来消除真实内存故障。

## 行为约定

输入是总应变，stress=E×strain，覆盖旧应力；E 有限且大于0，应变有限、允许负值；非法输入抛 invalid_argument 且原状态不变。应力和E同单位。教学输入保证乘积可表示，没有覆盖所有极端浮点范围。

`notes/agent_working_rules.template.md` 是可选择采用的上下文模板，没有自动安装为项目指令。现有项目约定应根据真实情况合并。

不提供机器相关构建产物或预设 Git 历史。请在独立目录初始化仓库，或并入已有练习仓库；避免嵌套仓库。故障注入只用于小项目。
