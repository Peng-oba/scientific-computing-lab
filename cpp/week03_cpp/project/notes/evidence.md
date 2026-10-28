# 第三周证据索引：AI 执行与本人掌握分开记录

每项填写：任务/预期 → 版本与命令 → 实际结果 → 我亲自核对的证据 → 接受/否决/未定及理由 → 限制。

- D1 任务卡与独立预期：

- D1 修改提交与 revert 提交：

- D2 source→target→产物，以及旧程序反例：
  - 版本与路径：基线提交 `fe4dc45`；项目根目录 `E:\scientific-computing-lab\cpp\week03_cpp\project`；Ninja/Debug 构建目录 `build\debug`；实际运行的程序为 `E:\scientific-computing-lab\cpp\week03_cpp\project\build\debug\material_demo.exe`。
  - 文件证据：`CMakeLists.txt` 的 `add_library(material src/material.cpp)`、`add_executable(material_demo app/main.cpp)`、`target_link_libraries(material_demo PRIVATE material)`；`build\debug\build.ninja` 的依赖链为 `src/material.cpp.obj → libmaterial.a → material_demo.exe`。
  - 配置与基线：`cmake -S . -B build\debug -G Ninja -DCMAKE_BUILD_TYPE=Debug` 退出码 0，配置/生成完成；`ninja -C build\debug` 退出码 0，输出 `no work to do`。源码 `app/main.cpp` 为 `E=1000`、总应变 `0.01`；`.\build\debug\material_demo.exe` 退出码 0，输出 `first=10`、`second=10`。
  - 旧程序反例：仅把源码模量改成 `E=2000`，不构建就运行 `.\build\debug\material_demo.exe`，退出码 0，仍输出 `first=10`、`second=10`；这是旧产物，CTest 测试数不适用。随后 `ninja -C build\debug material_demo` 退出码 0，日志显示编译 `app/main.cpp.obj` 并链接 `material_demo.exe`；再次运行程序，退出码 0，输出 `first=20`、`second=20`。
  - 恢复：只将模量改回 `E=1000`；`ninja -C build\debug material_demo` 退出码 0，程序运行退出码 0，输出恢复为 `first=10`、`second=10`；`git diff --exit-code -- app/main.cpp` 退出码 0。

- D2 空测试筛选与实际数量：
  - 空筛选：`ctest --test-dir build\debug -R '^does_not_exist$' --output-on-failure` 输出 `No tests were found!!!`；实际 0 个测试，退出码 0。
  - 零测试报错：`ctest --test-dir build\debug -R '^does_not_exist$' --output-on-failure --no-tests=error` 输出 `No tests were found!!!` 和 `Errors while running CTest`；实际 0 个测试，退出码 1。
  - 完整测试：`ctest --test-dir build\debug --output-on-failure --no-tests=error` 执行 `material.normal`、`material.boundary`、`material.invalid`、`material.repeated` 共 4 个测试；4/4 通过，退出码 0。此时源码已恢复 `E=1000`。

- D3 三类测试关键判据：
- D3 同一回归用例的红→绿：
- D3 放宽容差的假通过与否决理由：
- D4 可证伪假设、实际 debugger 观测和最小修复：
- D5 UAF / OOB / UB 的真实报告及修复对照：
  - 源码判断：
    1. UAF：`faults/uaf.cpp:9` 的 `delete[] values` 使 `borrowed` 悬垂；第 12 行读取 `borrowed[0]` 才触发释放后使用。
    2. OOB：`faults/oob.cpp:8` 中无额外参数时 `argc=1`，故 `index=3`；长度为 3 的数组仅允许索引 0、1、2，第 11 行读取越界。
    3. UB：`faults/overflow.cpp:9` 先在有符号 `int` 中计算 `maximum + 1`，超出可表示范围，属于未定义行为。
  - 已有原始报告（`build/sanitize-win/*.err.log`，按当前源码行号核对）：
    - UAF：`uaf.err.log` 报 `heap-use-after-free`，读发生在 `faults/uaf.cpp:12`，释放在第 9 行，分配在第 5 行。
    - OOB：`oob.err.log` 报 `heap-buffer-overflow`，读发生在 `faults/oob.cpp:11`，分配在第 5 行。
    - UB：`overflow.err.log` 报 `signed integer overflow`，指出 `2147483647 + 1` 不能用 `int` 表示，位置为 `faults/overflow.cpp:9`。
  - 三个故障程序的实际退出码：现有日志未记录，待分别运行并填写；不能由诊断文本推定具体数值。
  - 最小修复前后对照：待选择至少一个故障，保留修复前日志和退出码，修改后只重建该目标，再记录新输出、诊断和退出码。
  - 检查范围：三个故障目标不属于正常 CTest；正常测试无 sanitizer 报告也不能证明所有路径安全或数值算法正确。
- D6 实际 OGS target→入口→函数→断言，以及运行或阻塞：
- D7 新输入预测与运行：
- 我能独立解释的内容：
- AI 已完成但我仍不能解释的内容：
- 尚未运行/无法验证的内容：
