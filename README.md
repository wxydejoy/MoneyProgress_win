<!--
 * @Author: weiekko weiekko@gmail.com
 * @Date: 2023-04-08 19:52:07
 * @LastEditors: weiekko weiekko@gmail.com
 * @LastEditTime: 2023-05-28 00:59:15
 * @FilePath: \MoneyProgress_win\README.md
 * @Description: 
 * 
 * Copyright (c) 2023 by weiekko weiekko@gmail.com, All Rights Reserved. 
-->
# MoneyProgress_win
MoneyProgress_win
## 截图
![uTools_1683466686757](https://user-images.githubusercontent.com/81625961/236680926-2b2a745f-36f1-4745-8d14-fe6c7095b207.png)
![uTools_1683466721134](https://user-images.githubusercontent.com/81625961/236680921-2770f18e-b8aa-4488-9084-938c7faaa3ab.png)
![uTools_1683466714371](https://user-images.githubusercontent.com/81625961/236680925-66f33ac4-f0a0-43ab-92a5-e57878ac3b0b.png)


## 使用方式

esc 隐藏主界面
a 作者信息

## 感谢mac版本作者
https://github.com/Lakr233/MoneyProgress


## 开机启动

1、按下win+R快捷键，打开运行窗口，输入shell:startup，点击确定

2、打开启动文件夹，最后把想要添加的启动项快捷方式直接拖进这个文件夹

3、然后就可以看到我们将qq设置为启动项了。

4、我们还可以进入任务管理器中看看我们有没有添加成功。


& scripts\windows-mingw-publish.ps1 ${env:archiveName} ${env:targetName}
           $name = ${env:archiveName}
           echo "::set-output name=packageName::$name"       