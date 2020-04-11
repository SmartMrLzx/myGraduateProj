#pragma once

#include "DcmReadInterface.h"

enum T_2DWdt_Type {           //2D����������
	C_Axial = 0,
	C_Coronal = 1,
	C_Sagittal = 2,
};

//��͸���Ȳ���
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

	quint16 width;         //���
	quint16 height;        //�߶�
	qint16 order;          //����
	quint16 total;         //��������
	T_2DWdt_Type imgtype;  //������������

};

struct T_DImageWindowCtrl {               //DICOMͼ�񴰿ڿ���

	int16    windowCenter;		//��λ
	int16    windowWidth;		//����
	real32   rescaleIntercept;	//�Ҷ�תHUֵ�Ľؾ�
	real32   rescaleSlope;		//�Ҷ�תHUֵ��б��

};