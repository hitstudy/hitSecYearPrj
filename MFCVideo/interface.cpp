/********************************************************
实现需要用的接口，便于日后维护，记得添加海康和OpenCV依赖库
使用的函数来自OpenCV 以及海康网络摄像头SDK。
											2019/2/26-张弓
********************************************************/
#include "stdafx.h"
#include "interface.h"



 char * getPic(int & PicSize, LONG UserID, int ipChannel)
{
	int a = 600, b = 370 * 2;
	FILE *file = NULL;

	NET_DVR_JPEGPARA JpegPara;
	JpegPara.wPicQuality = 0;
	JpegPara.wPicSize = 0xff;

	char *JpegPicBuffer = new char[a*b];

	DWORD  SizeReturned = 0;
	BOOL bRet = NET_DVR_CaptureJPEGPicture_NEW(UserID, ipChannel, &JpegPara, JpegPicBuffer, a*b, &SizeReturned);//SizeReturned返回图片数据大小

	if (!bRet)
	{
		PicSize = 0;
		return NULL;
	}
	else
	{
		PicSize = SizeReturned;
		return JpegPicBuffer;
	}
	return nullptr;
}
HGLOBAL  getGlobalHandle(void *  pMemData, long  len)
{
	HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, len);
	if (hGlobal == 0)
	{
		return NULL;
	}
	void *  pData = GlobalLock(hGlobal);
	if (pData == 0)
	{
		return NULL;
	}
	memcpy(pData, pMemData, len);
	GlobalUnlock(hGlobal);

	/*IStream *  pStream = NULL;
	if (CreateStreamOnHGlobal(hGlobal, TRUE, &pStream) == S_OK)
	{
		CImage image;
		if (SUCCEEDED(image.Load(pStream)))
		{
			//   ..

		}
		pStream->Release();
	}
	GlobalFree(hGlobal); //这里似乎不需要释放。红色的TRUE表示Release会自动释放！
	*/
	return hGlobal;
}
BOOL ShowJpgGif(CDC* pDC,HGLOBAL& hGlobal,int size, int x, int y)  //暂时这么写吧，我觉得这样调用的时候会挺麻烦的，但是应该不会错2019/2/11 -张弓 

{
	if (hGlobal == NULL)
	{
		return FALSE;
	}

	// 将文件放到流中
	IStream *pStm;
	CreateStreamOnHGlobal(hGlobal, TRUE, &pStm);//pStm本身就是指针了，为什么这里还是要取地址？这里是取地址还是引用？
	//最后一个参数为IStream指针的地址，该指针在该函数执行后指向新创建的流对象。该参数不能为NULL


	// 从流中加载图片
	// 显示JPEG和GIF格式的图片，GIF只能显示一帧，还不能显示动画，
	// 要显示动画GIF请使用ACTIVE控件。
	IPicture *pPic;
	if (OleLoadPicture(pStm, (LONG)size, TRUE, IID_IPicture, (LPVOID*)&pPic) != S_OK)
	{
		GlobalFree(hGlobal);  // 记得释放内存
		return FALSE;
	}

	//获取图像宽和高,注意这里的宽和高不是图像的分辨率
	OLE_XSIZE_HIMETRIC hmWidth;
	OLE_YSIZE_HIMETRIC hmHeight;
	pPic->get_Width(&hmWidth);
	pPic->get_Height(&hmHeight);

	// 将图像宽度和高度单位转化为像素单位
   //#define HIMETRIC_PER_INCH 2540
   //int nPicWidth =  MulDiv(hmWidth, GetDeviceCaps(GetDC()->m_hDC, LOGPIXELSX),2540);
   //int nPicHeight = MulDiv(hmHeight, GetDeviceCaps(GetDC()->m_hDC, LOGPIXELSY),2540);


	//HRESULT Render(
	//    HDC hdc, //Handle of device context on which to render the image
	//    long x,  //Horizontal position of image in hdc
	//    long y,  //Vertical position of image in hdc
	//    long cx, //Horizontal dimension of destination rectangle
	//    long cy, //Vertical dimension of destination rectangle
	//    OLE_XPOS_HIMETRIC xSrc,      //Horizontal offset in source picture
	//    OLE_YPOS_HIMETRIC ySrc,      //Vertical offset in source picture
	//    OLE_XSIZE_HIMETRIC cxSrc,    //Amount to copy horizontally in source picture
	//    OLE_YSIZE_HIMETRIC cySrc,    //Amount to copy vertically in source picture
	//    LPCRECT prcWBounds           //Pointer to position of destination for a metafile hdc
	//    );

	//use render function display image
	RECT rtWnd;
	pDC->GetWindow()->GetWindowRect(&rtWnd);
	int iWndWidth = rtWnd.right - rtWnd.left;
	int iWndHeight = rtWnd.bottom - rtWnd.top;

	if (FAILED(pPic->Render(*pDC, x, y, iWndWidth, iWndHeight, 0, hmHeight, hmWidth, -hmHeight, NULL)))
	{
		pPic->Release();
		GlobalFree(hGlobal);  // 记得释放内存
		return false;
	}

	pPic->Release();
	GlobalFree(hGlobal);  // 记得释放内存
	return true;
}
