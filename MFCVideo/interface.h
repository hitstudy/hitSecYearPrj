/********************************************************
声明需要用的接口，便于日后维护，记得添加海康和OpenCV依赖库
封装的函数来自OpenCV 以及海康网络摄像头SDK。
											2019/2/26-张弓
********************************************************/

#pragma once//编译注解，保证该文件只被编译一次，并非所有编译器支持，跨平台应使用C标准注解 2019/2/26-张弓
#include "HCNetSDK.h"
#include "PlayM4.h"
const int ipChannel = 1;//1是默认值，demo里直接赋值为1了，这里先写成const int 2019/2/26-张弓

/*******************getPic*****************************
方法说明：  返回存有jpeg图像数据的内存首地址和内存大小
返回值：	char数组指针，数据在内存的首地址，返回值为NULL说明获取失败
参数		int &PicSize	用来返回图片大小，若值为0说明获取失败
参数		LONG UserID		登陆函数的返回值
参数		int ipChannel	设备通道号（？）说明见第十行
2019/2/26-张弓 
********************************************************/
char *getPic(int &PicSize, LONG UserID,int ipChannel);


/*****************getGlobalHandle************************
方法说明：	从一个内存指针中获得具有图片数据的句柄
返回值：	存有图片数据的内存的句柄
参数		void* pmemdata 		具有图片数据的内存的指针
参数		Long len 			内存大小
2019/2/26-张弓
********************************************************/
HGLOBAL  getGlobalHandle(void *  pMemData, long  len);


/********************************************************
方法说明:    从一个内存句柄中显示图片到指定窗口
参数说明:    CDC * pDC 			设备环境对象
参数说明:    &HGlobal			内存句柄
参数说明:	 int size			内存大小（加载时应读取的字节数）
参数说明:    int x    			要显示的X位置
参数说明:    int y          	要显示的Y位置
返回值:      BOOL             	成功返回TRUE,否则返回FALSE
2019/2/26-张弓
********************************************************/
BOOL ShowJpgGif(CDC* pDC, HGLOBAL& hGlobal, int size, int x, int y);