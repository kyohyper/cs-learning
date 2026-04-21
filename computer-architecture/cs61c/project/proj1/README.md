# CS61C Project 1 康威的生命游戏（已完成）

可以用 viu 在终端打印 ppm 图像<br>
安装 viu：`cargo install viu`

原文档对`rule`没有解释清楚，这里解释一下。`rule`中包含18个有效bit，前九个bit设置活细胞周围有9,8,7,...,0个活细胞时继续存活，后九个bit设置死细胞周围有9,8,7,...,0个活细胞时复活。

有一个问题是，frame.csh最后调用的convert貌似有些bug，当我尝试用`frame.csh glider 0x1808 100`创建一个100帧的gif时，convert报了`segmentation fault`，暂时没有找到解决方法。（convert版本：1.3.42 2023-09-23）