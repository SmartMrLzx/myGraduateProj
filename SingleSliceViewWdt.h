#pragma once

#include <QWidget>
#include <qslider.h>
#include <qlabel.h>
#include <qimage.h>
#include <qpixmap.h>
#include "datatypedef.h"
#include <stdlib.h>
#include <string.h>



class SingleSliceViewWdt : public QWidget             //切面视图部件
{
	Q_OBJECT

public:
	SingleSliceViewWdt(QWidget *parent, T_2DWdt_Type type, DICOMPropertyData* mpDicomImgPty);
	~SingleSliceViewWdt();


protected:

	void SetVolumeData(DICOMPropertyData* mpDicomImgPty);            //设置体数据
	void CreateWdts();                                               //创建部件
	void resizeEvent(QResizeEvent* event);                           //重定义大小
	void paintEvent(QPaintEvent* event);                             //绘制

	uchar* GetDcmImage(T_DImgPropety *pty,                           //读取DCM图像
					   T_DImageWindowCtrl *mPtDcmPty);

	uchar DataExchange(float temp);                                  //数据转换


	void ProSliderValueChangeEvent(int val);                         //滑块值变化

private:
	QSlider* mpScrollNextImgSlider;            //滑块
	QLabel* mpInfoLbl;                         //标签
	QLabel* mpSliceImg;                        

	//int16* mpVolumeDataPtr;
	T_DImgPropety mProperty;             //DICOM图像属性               
	T_DImageWindowCtrl mPtDcmPty;        //DICOM图像窗位窗宽控制
	QImage* mpCellSliceImg;              //切面image

	DICOMPropertyData* m_pDicomImgPty;   //DICOM图像数据

	T_2DWdt_Type mWdtType;        //当前2D部件的类型

	bool isUpdateSlice;        //检查是否更新切面

};
