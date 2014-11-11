参考：http://blog.csdn.net/musicvs/article/details/8166572

说明：
我们使用Cocos2d自身集成的toLua++工具，将C++类注册到Lua中使用。
工程目录Client目录下新建一个目录ScriptSupport目录，该目录下包含
LuaClient.cpp : tolua++生成的中间文件，注册c++类代码
LuaClient.h ： 客户端自己定义类的头文件包含在这里
SpriteFactory.pkg ： 我自己写的一个例子（pkg文件说明参考如上网址）
LuaClient.pkg ：写好的SpriteFactory.pkg一定要添加到LuaClient.pkg文件中。

步骤：
1 	客户端添加相关的c++类
2 	编写相应的pkg文件
3  	包含自定义的pkg到LuaClient.pkg文件中
4 	运行 tolua.bat 批处理命令
5 	LuaClient.h 中包含对应的.h文件
6 	Lua中调用相关类方法
