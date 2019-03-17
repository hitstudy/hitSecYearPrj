/********************************************************
ʵ����Ҫ�õĽӿڣ������պ�ά�����ǵ���Ӻ�����OpenCV������
ʹ�õĺ�������OpenCV �Լ�������������ͷSDK��
											2019/2/26-�Ź�
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
	BOOL bRet = NET_DVR_CaptureJPEGPicture_NEW(UserID, ipChannel, &JpegPara, JpegPicBuffer, a*b, &SizeReturned);//SizeReturned����ͼƬ���ݴ�С

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
	GlobalFree(hGlobal); //�����ƺ�����Ҫ�ͷš���ɫ��TRUE��ʾRelease���Զ��ͷţ�
	*/
	return hGlobal;
}
BOOL ShowJpgGif(CDC* pDC,HGLOBAL& hGlobal,int size, int x, int y)  //��ʱ��ôд�ɣ��Ҿ����������õ�ʱ���ͦ�鷳�ģ�����Ӧ�ò����2019/2/11 -�Ź� 

{
	if (hGlobal == NULL)
	{
		return FALSE;
	}

	// ���ļ��ŵ�����
	IStream *pStm;
	CreateStreamOnHGlobal(hGlobal, TRUE, &pStm);//pStm�������ָ���ˣ�Ϊʲô���ﻹ��Ҫȡ��ַ��������ȡ��ַ�������ã�
	//���һ������ΪIStreamָ��ĵ�ַ����ָ���ڸú���ִ�к�ָ���´����������󡣸ò�������ΪNULL


	// �����м���ͼƬ
	// ��ʾJPEG��GIF��ʽ��ͼƬ��GIFֻ����ʾһ֡����������ʾ������
	// Ҫ��ʾ����GIF��ʹ��ACTIVE�ؼ���
	IPicture *pPic;
	if (OleLoadPicture(pStm, (LONG)size, TRUE, IID_IPicture, (LPVOID*)&pPic) != S_OK)
	{
		GlobalFree(hGlobal);  // �ǵ��ͷ��ڴ�
		return FALSE;
	}

	//��ȡͼ���͸�,ע������Ŀ�͸߲���ͼ��ķֱ���
	OLE_XSIZE_HIMETRIC hmWidth;
	OLE_YSIZE_HIMETRIC hmHeight;
	pPic->get_Width(&hmWidth);
	pPic->get_Height(&hmHeight);

	// ��ͼ���Ⱥ͸߶ȵ�λת��Ϊ���ص�λ
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
		GlobalFree(hGlobal);  // �ǵ��ͷ��ڴ�
		return false;
	}

	pPic->Release();
	GlobalFree(hGlobal);  // �ǵ��ͷ��ڴ�
	return true;
}
