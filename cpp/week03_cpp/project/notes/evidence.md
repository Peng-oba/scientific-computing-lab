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
  - 审查题 2：否决“公式实现错了”的归因。未重建时的输出不足以判断新源码；最小核查是重建 `material_demo` 后从明确路径运行，比较输出。上述文件和输出供本人独立核对。
- D2 空测试筛选与实际数量：
  - 空筛选：`ctest --test-dir build\debug -R '^does_not_exist$' --output-on-failure` 输出 `No tests were found!!!`；实际 0 个测试，退出码 0。
  - 零测试报错：`ctest --test-dir build\debug -R '^does_not_exist$' --output-on-failure --no-tests=error` 输出 `No tests were found!!!` 和 `Errors while running CTest`；实际 0 个测试，退出码 1。
  - 完整测试：`ctest --test-dir build\debug --output-on-failure --no-tests=error` 执行 `material.normal`、`material.boundary`、`material.invalid`、`material.repeated` 共 4 个测试；4/4 通过，退出码 0。此时源码已恢复 `E=1000`。
  - 审查题 1：否决“退出码为 0 就可接受修改”。最小核查是确认筛选名称和实际执行数，并用 `--no-tests=error` 防止空测试被当作成功。4/4 仅覆盖上述四例，不能证明其他输入或真实 OGS 行为；本人的接受结论待独立核对命令输出后填写。
- D3 三类测试关键判据：
- D3 同一回归用例的红→绿：
- D3 放宽容差的假通过与否决理由：
- D4 可证伪假设、实际 debugger 观测和最小修复：
- D5 UAF / OOB / UB 的真实报告及修复对照：
- D6 实际 OGS target→入口→函数→断言，以及运行或阻塞：
- D7 新输入预测与运行：
- 我能独立解释的内容：
- AI 已完成但我仍不能解释的内容：
- 尚未运行/无法验证的内容：
