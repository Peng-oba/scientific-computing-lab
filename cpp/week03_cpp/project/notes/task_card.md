# 本周任务卡：由学习者先填写，再交给 agent

- 任务目标：
    建立第三周 AI 协作开发练习的可追溯基线, 明确 updateStress 的输入语义、非法输入行为、允许修改范围和验收证据
- 实际项目根目录 / Git 根目录：
    项目目录: E:\scientific-computing-lab\cpp\week03_cpp\project
    Git 根目录: E:\scientific-computing-lab
- 当前分支、commit 与未提交状态：
- 相关文件（先给最小集合）：
    include/lab/material.hpp
    src/material.cpp
    app/main.cpp
    tests/material_tests.cpp
    CMakeLists.txt
    notes/task_card.md
    notes/checkpoint.md
- 输入的物理/数值含义：
    modulus 是弹性模量, strain 是总应变, stress = modulus * strain
- 单位与可接受范围：
- 状态更新约定：
    每次调用用当前总应变重新计算并覆盖 state.stress, 不累加旧应力.
- 非法输入的行为与失败后状态：
    modulus 非有限或 <= 0, 或 strain 非有限时, 抛 std::invalid_argument, state.stress 保持调用前状态.
- 允许修改的文件/行为：
    今天只修改 notes/task_card.md 和 Git 演练需要的 notes/checkpoint.md.
    不修改 src/material.cpp、include/lab/material.hpp、测试逻辑、CMakeLists.txt
- 保持不变的约定与判据：
- 完成条件：
    1. 普通 CTest 4/4 通过
    2. material_demo 输出 first=10 和 second=10
    3. 能说明一次提交和一次 revert 分别做了什么。
- 需要的原始证据：

| 输入情况 | 我预先给出的结果/失败条件 | 独立依据 | 运行后实际结果 |
|---|---|---|---|
| 正常 | | | |
| 边界 | | | |
| 非法 | | | |
| 重复调用 | | | |

我愿意接受什么修改，为什么：

什么观察会使我否决当前结论：
