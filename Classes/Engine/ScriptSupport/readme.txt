参考：http://blog.csdn.net/musicvs/article/details/8166572

说明：
我们使用Cocos2d自身集成的toLua++工具，将C++类注册到Lua中使用。
工程目录Engine目录下新建一个目录ScriptSupport目录，该目录下包含
LuaEngine.cpp : tolua++生成的中间文件，注册c++类代码
LuaEngine.h ： 客户端自己定义类的头文件包含在这里
LuaEngine.pkg ：写好的SpriteFactory.pkg一定要添加到LuaClient.pkg文件中。

步骤：
1 	客户端添加相关的c++类
2 	编写相应的cpk文件
3  	包含自定义的cpk到LuaEngine.pkg 文件中
4 	运行 tolua.bat 批处理命令
5 	LuaEngine.h 中包含对应的.h文件
6 	Lua中调用相关类方法
