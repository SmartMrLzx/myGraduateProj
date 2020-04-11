#pragma once

#include <QWidget>
#include <qslider.h>
#include <qlabel.h>
#include <qimage.h>
#include <qpixmap.h>
#include "datatypedef.h"
#include <stdlib.h>
#include <string.h>



class SingleSliceViewWdt : public QWidget             //������ͼ����
{
	Q_OBJECT

public:
	SingleSliceViewWdt(QWidget *parent, T_2DWdt_Type type, DICOMPropertyData* mpDicomImgPty);
	~SingleSliceViewWdt();


protected:

	void SetVolumeData(DICOMPropertyData* mpDicomImgPty);            //����������
	void CreateWdts();                                               //��������
	void resizeEvent(QResizeEvent* event);                           //�ض����С
	void paintEvent(QPaintEvent* event);                             //����

	uchar* GetDcmImage(T_DImgPropety *pty,                           //��ȡDCMͼ��
					   T_DImageWindowCtrl *mPtDcmPty);

	uchar DataExchange(float temp);                                  //����ת��


	void ProSliderValueChangeEvent(int val);                         //����ֵ�仯

private:
	QSlider* mpScrollNextImgSlider;            //����
	QLabel* mpInfoLbl;                         //��ǩ
	QLabel* mpSliceImg;                        

	//int16* mpVolumeDataPtr;
	T_DImgPropety mProperty;             //DICOMͼ������               
	T_DImageWindowCtrl mPtDcmPty;
	QImage* mpCellSliceImg;              //����image

	DICOMPropertyData* m_pDicomImgPty;

	T_2DWdt_Type mWdtType;        //��ǰ2D����������

	bool isUpdateSlice;        //����Ƿ��������

};
