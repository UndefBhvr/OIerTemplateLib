# OIerTemplateLib

## 关于为什么有了别人的OI WIKI/Template还写这个

虽然网上模板很多，但多数~~抄起来比较麻烦~~代码复用性不好。

因此在我看完某大佬的红黑树模板以后...

就立志写一个通用的OI常见算法库...

就有了这个咕咕咕的项目。

这个库目前还很不完善（几乎莫得代码），不过会慢慢完善起来。

文档等比较空的时候会慢慢写起来。

---

## 内容简介

以算法和数据结构为主。

由于新的C++标准十分有趣，也会有一些涉及。

例如目前已经更新的`oitl_concepts`库

这个库以算法/数据结构的通用性为目的，因此使用了一些OIer很少用的编程技巧（

但是最核心的代码不会出现过于晦涩的东西

最终目标是形成STL algorithm/container的超集 ~~，以后会写些模板元编程什么的搞个类型系统~~

~~概念系统也会慢慢完善的~~

---

## 代码标准

各种东西的接口会尽量参考STL、pd_ds，时间复杂度尽量跟STL（如果有）保持一致。

引入了少量较高版本C++标准的特性（如C++11类型萃取，C++20概念）

为了向后兼容，代码可读性会有一定的下降（因为大量的条件编译

**如果要在不使用其他oitl库文件的情况下单独使用一个文件，请在最前面加上预处理指令**

`#define _OITL_DEPENDENCE_FREE`

优先保证在gcc下可以编译通过。其次保证clang。再次保证msvc。

保证在gcc的--std=c++03/11/14/17/2a/20下可以编译通过。

尽可能保证在Windows/Linux下正常运行。

尽量保持内存安全。~~但是不考虑线程安全和异常安全~~

（即使我退役了，我也会抽空继续维护的233

代码应该可读性比GNU STL略好，等加完注释和文档会有一定的参考价值。

---

## 文件结构

前期将以数据结构为主。

* 每个数据结构会被放入一个`.hpp`头文件。
* 同一类数据结构会放在一个文件夹下，如`pairing_heap.hpp`和`leftist_heap`在`priority_queue`下
* 所有数据结构文件夹放在`data_structure`下

算法类似。在`algorithm`文件夹内有所有算法的文件。

**但由于算法不通过类进行封装，算法的实现细节被放在`algorithm/detail`下**

**`algorithm`下的文件只有接口，没有具体实现**

命名空间统一为`oitl`

直接`include`或打开复制粘贴就能用。

---



如果你有比较强的码力并对C++（注意不是C with STL）有一定了解，可以跟我一起搞（

我的QQ是1756304680，坐标sxyz，欢迎交朋友（

---

## 目前已经基本上实现的：

### 数据结构：

### 1.fhq_treap

支持std::set的一些基本操作（将来会完全），同时支持rank相关查询。

### 2.pairing_heap

支持std::priority_queue的（几乎）全部操作，同时支持快速减小关键字和合并。

### 3.leftist_heap

支持std::priority_queue的（几乎）全部操作，同时支持~~快速~~减小关键字和合并。

### 4.link_cut_tree

支持动态树的连边、删边、单点修改、路径查询。

### 5.pairing_heap：变种

实现了一种配对堆的变种，其`decrease_key`均摊为O(log log n)，理论上优于朴素配对堆，实际上效率相仿。

* 这个数据结构并没有在论文中给出名字...考虑到作者名字叫Amr Elmasry，名字暂时写为`elmasry_pairing_heap`。
* 论文地址：https://core.ac.uk/download/pdf/204209612.pdf
* 由于~~我懒~~时间所限，`modify()`（以小根堆为例）暂时不支持增大关键字

### 算法：
### 1.sort
接受一对随机访问迭代器和一个二元谓词。内部实现为内省排序。

基于快排，在排序区间长度小于等于16时转为插入排序

当递归深度大于2log(n)，为了避免退化，将转为堆排序。

---

已经计划实现的数据结构：

### 1.可持久化平衡树

将实现一个类似`ext/rope`的东西和一个可以快速复制的`set`。

### 2.rank_pairing_heap

将实现一个均摊复杂度与Fib堆等同的优先队列。


### 3.vector

略。

### 4.AA树

将实现一个代码相对简单的红黑树变种。

### 5.真·Rope

将实现一个Rope（

ext/rope是个平衡树，而且数据结构的名字就叫rope

不是块状链表也不是红黑树...

---

This is a template library for OIers. You can find some data structures and algorithms.

Now this library is not perfect(in fact, it includes few things).

But after it comes better, you'll be able to use it just like STL or pb_ds.

The code is not as good as industrial code. So if you are good at coding, please contact me.
