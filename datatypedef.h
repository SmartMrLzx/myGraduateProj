#pragma once

#include "DcmReadInterface.h"

enum T_2DWdt_Type {           //2D部件的类型
	C_Axial = 0,
	C_Coronal = 1,
	C_Sagittal = 2,
};

//不透明度参数
enum OpacityParameter
{
	CT_OpacityValue = 0,
	Opacity = 1,
	Midpoint = 2,
	Sharpness = 3
};

typedef signed short int16;
typedef float real32;


struct T_DImgPropety {           //DICOM Image Propety

	quint16 width;         //宽度
	quint16 height;        //高度
	qint16 order;          //次序
	quint16 total;         //总切面数
	T_2DWdt_Type imgtype;  //所属切面类型

};

struct T_DImageWindowCtrl {               //DICOM图像窗口控制

	int16    windowCenter;		//窗位
	int16    windowWidth;		//窗宽
	real32   rescaleIntercept;	//灰度转HU值的截距
	real32   rescaleSlope;		//灰度转HU值的斜率

};