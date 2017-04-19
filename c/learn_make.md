## Makefile

1. Make命令依赖Makefile文件进行构建，或者用命令行参数指定为其他文件名。
	> make -f rules.txt  
	> 或者  
	> make --file=rules.txt  
	> 上面代码指定make命令依据rules.txt文件中的规则进行构建。
	
	例如：  
	make hello -f hello.mk  	
2. Makefile文件的规则:

	```
	<目标>: <前置条件>
	[tab]<命令>
	```
	"目标"必须存在，"前置条件"和"命令"可选，但两者必须至少存在一个。
3. 目标可以是文件名（多个文件名之间用空格分隔），还可以是伪目标。伪目标只是一个标签不会生成文件，可避免跟文件同名。

	```
	.PHONY: clean
	clean:
		-rm *.o
	```
	- 声明clean是"伪目标"之后，make就不会去检查是否存在一个叫做clean的文件，而是每次运行都执行对应的命令。  
	- rm命令前的小减号表示也许某些文件出现问题，但不要管，继续做后面的事。	
4. Make命令运行时没有指定目标，默认会执行Makefile文件的第一个目标。make可以同时指定多个目标，比如make test1 test2。
5. 只要有一个前置文件不存在或者有过更新，“目标就需要重新构建”。
6. 命令中每行命令在一个单独的shell中执行，这些Shell之间没有继承关系。两行命令写在一行要用分号分隔。
7. 回声：make会打印每条命令，然后再执行。
	> 在命令前面加上@，可以关闭回声。通常只在注释和纯显示的echo命令前面加上@
8. 模式匹配：

	```
	%.o: %.c
	```
	等同于：
	
	```
	f1.o: f1.c
	f2.o: f2.c
	...
	```
	使用匹配符%，可以将大量同类型的文件，只用一条规则就完成构建。
9. 命令中使用自定义变量时，变量需要放在$()之中。  
   命令调用shell变量时，需在美元符号前面再加一个美元符号(比如echo $$HOME)。
   
   ```
	test_home:
		echo $$HOME
		echo $(HOME)
   ```
   make test_home 输出：
   
   ```
	echo $HOME
	/Users/zhanglei
	echo /Users/zhanglei
	/Users/zhanglei
   ```
10. 内置变量：  
	$(CC) 指向当前使用的编辑器  
	$(MAKE) 指向当前使用的Make工具
11. 自动变量：  
	`$@` :当前目标  
	`$<` :第一个前置条件  
	`$?` :比目标更新的所有前置条件，之间以空格分隔  
	`$^` :所有前置条件，之间以空格分隔  
	`$*` :匹配符 % 匹配的部分，比如% 匹配 f1.txt 中的f1 ，$* 就表示 f1。  
	`$(@D)`和`$(@F)` :$(@D) 和 $(@F) 分别指向 $@ 的目录名和文件名。比如，$@是 src/input.c，那么$(@D) 的值为 src ，$(@F) 的值为 input.c。  
	`$(<D)`和`$(<F)` :$(<D) 和 $(<F) 分别指向 $< 的目录名和文件名。  
	
	例子：  
	将 src 目录下的 txt 文件，拷贝到 dest 目录下。
	
	```
	dest/%.txt: src/%.txt
   		@[ -d dest ] || mkdir dest
   		cp $< $@
    ```
12. 判断：

	```
	test1:
	ifeq ($(CC),gcc)
    	@echo "y"
    	@echo $(CC)
	else
   		@echo "n"
   		@echo $(CC)
	endif
	```
13. 循环：

	```
	LIST = one two three
	test2:
		for i in $(LIST); do \
			echo $$i; \
		done
	# 等同于
	test2:
		for i in one two three; do \
			echo $i; \
		done
	```
14. 隐含规则：
	- 后缀规则：将一个具有某个后缀的文件(例如.c文件)转换成另一种后缀的文件(例如.o文件)。

		```
		.c.o:
			gcc -c $< -o $@
		```
	- 模式规则：利用模式规则定义更加复杂的依赖性规则。

		```
		%.o:%.c
			gcc -c $< -o $@
		```
15. Makefile文件中可以包含include filenames (filenames可以包含路径和通配符)。
16. gcc -c hello.c -o hello.o  
	gcc hello.o -o hello 



### 参考教程：
[阮一峰的网络日志--Make命令教程](http://www.ruanyifeng.com/blog/2015/02/make.html)