## 常见库与函数

** `<memory>` ** - 智能指针（`std::unique_ptr`，`std::shared_ptr`） 替代`new`和`delete`，防止内存泄漏。



## 常用工具

### `gdb` - GNU 调试器，命令行调试 C++ 程序。

### Misc

* `c++filt` 用于解码 C++ 符号名（demangle）。
* `addr2line` 将地址转换为源代码文件和行号。
* `nm` 列出目标文件中的符号。
* `ldd` 

### 将 C++ 代码和汇编对应的工具

- Compiler Explorer (Godbolt)  
  - 在线查看编译器输出并把源代码与生成的汇编并排显示，支持多种编译器/选项。网址：https://godbolt.org

- objdump（本地，可对可执行文件/目标文件按源代码反汇编）  
  - 典型用法（Intel 语法，显示源代码，demangle）：  
    ```
    g++ -g -O2 file.cpp -o a.out
    objdump -d -S --demangle -M intel a.out > a.out.asm
    ```
  - 如果需要针对单个目标文件：`objdump -d -S file.o`

- 直接由编译器生成汇编（便于带注释的汇编）  
  - g++：  
    ```
    g++ -g -O2 -S -fverbose-asm -masm=intel file.cpp -o file.s
    ```
  - clang：同理可用 `clang++ -g -O2 -S -fverbose-asm -masm=intel`

- 其他有用的提示  
  - 要得到可映射到源代码的输出，编译时加 `-g`（生成调试信息）。  
  - 优化（-O2/-O3）会导致函数内联、代码重排或消失，影响一一对应；调试逐步查看可先用 `-O0` 或禁用内联（`-fno-inline`）。  
  - 对于已运行地址想查源行，可配合 `addr2line` 或 `eu-addr2line`：`addr2line -e a.out 0xADDRESS`。  
  - 对大型工程推荐先用 Compiler Explorer 做快速实验，本地用 objdump 或编译器的 `-S` 生成详细结果。

## 常用调试函数

* `__attribute__((noinline))` - 防止函数内联，方便调试。

## 一些问题

没问题！面向对象（OOP）里的 `static` 和 `this` 确实比较抽象。我们用**“图纸”**和**“实物”**的比喻，配合图解来彻底搞懂它。

### 核心概念比喻

1. **类 (Class)** = **图纸** (比如“汽车设计图”)。
2. **对象/实例 (Object/Instance)** = **造出来的真车** (比如“你的那辆红色宝马”，“我的那辆蓝色宝马”)。
3. **非静态成员 (普通函数/变量)** = **车里的功能** (比如“踩油门”，“调节座椅”)。每辆车都有自己独立的油门和座椅。
4. **静态成员 (Static)** = **图纸上的标注** 或者 **工厂的公共服务** (比如“查询这款车的出厂年份”，“查询这款车的总销量”)。这不属于某辆具体的车，而是属于这个型号。

---

### 图解：普通函数 vs 静态函数

#### 1. 普通成员函数 (有 `this`)

假设我们有一个 `Car` 类，有一个 `SetSpeed` 函数。

```cpp
class Car {
    int speed; // 成员变量
    void SetSpeed(int s) { speed = s; }
};

Car myCar;
myCar.SetSpeed(100);
```

当你调用 `myCar.SetSpeed(100)` 时，实际上编译器把它转换成了这样：

```text
+----------------+       (隐式传递)        +-----------------------------------+
| myCar (对象A)   | ---------------------> | Car::SetSpeed(Car* this, int s) |
| 内存地址: 0x100 |       this = 0x100     | {                                 |
| speed: 0       |                        |    this->speed = s;               |
+----------------+                        | }                                 |
                                          +-----------------------------------+
```

* **`this` 指针**：就是 `myCar` 的地址。函数通过 `this` 才知道要修改**哪一辆车**的速度。
* **`const` 的作用**：如果你写 `void GetSpeed() const`，就是承诺：“我只读 `this->speed`，绝不修改它”。

#### 2. 静态成员函数 (无 `this`)

现在看 `Input` 类。

```cpp
class Input {
    static Window* s_Window; // 静态变量
    static bool IsKeyPressed(int key);
};

Input::IsKeyPressed(KEY_A);
```

```text
+----------------+                        +-----------------------------------+
| Input (类/图纸) | ---------------------> | Input::IsKeyPressed(int key)      |
| (不需要对象)    |       (没有 this!)     | {                                 |
|                |                        |    // 只能访问静态变量             |
| static s_Window| <--------------------- |    check(s_Window, key);          |
+----------------+                        | }                                 |
                                          +-----------------------------------+
```

* **没有 `this`**：因为你不需要创建一个 `Input` 对象就能调用它。它不属于任何具体的“车”，它是“工厂”提供的服务。
* **为什么不能加 `const`？**：
  * `const` 的意思是：“不修改**当前对象** (`this`)”。
  * 静态函数连 `this` 都没有，它根本不知道“当前对象”是谁，所以承诺“不修改当前对象”是毫无意义的逻辑悖论。

![Static vs Non-Static](image/cpp/001.svg)

* **蓝色路径 (普通函数)**：必须依赖具体的对象（Obj1 或 Obj2）。`const` 锁住的是 Obj1 或 Obj2 的数据。
* **粉色路径 (静态函数)**：直接从类访问，不经过对象。没有对象，就没有 `const` 修饰的对象。

这就是为什么你的 `Input::IsKeyPressed` 不需要（也不能）加 `const` 的原因！
