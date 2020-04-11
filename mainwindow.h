//#ifndef INITIAL_OPENGL
//#define INITIAL_OPENGL
//#include <vtkAutoInit.h>
//VTK_MODULE_INIT(vtkRenderingOpenGL2)
//VTK_MODULE_INIT(vtkInteractionStyle)
//VTK_MODULE_INIT(vtkRenderingVolumeOpenGL2)
//VTK_MODULE_INIT(vtkRenderingFreeType)
//VTK_MODULE_INIT(vtkRenderingContextOpenGL)
//#endif
#include "vtkAutoInit.h"
#define vtkRenderingCore_AUTOINIT 2(vtkInteractionStyle, vtkRenderingOpenGL2)
#define vtkRenderingVolume_AUTOINIT 1(vtkRenderingVolumeOpenGL2)
VTK_MODULE_INIT(vtkRenderingFreeType)


#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVTKWidget.h>
#include <QWidget>
#include <QList>
#include <QEvent>

#include "vtkAxesActor.h"
#include "vtkTextProperty.h"
#include "vtkActor.h"
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkInteractorStyleTrackballCamera.h"
#include "vtkTransform.h"
#include "vtkAssembly.h"
#include "vtkSphereSource.h"
#include "vtkPolyDataMapper.h"
#include "vtkProperty.h"

#include "vtkImageImport.h"
#include "vtkSmartVolumeMapper.h"
#include "vtkColorTransferFunction.h"
#include "vtkPiecewiseFunction.h"
#include "vtkVolumeProperty.h"
#include "vtkCamera.h"

#include "vtkDICOMImageReader.h"
#include "vtkMarchingCubes.h"
#include "vtkStripper.h"

#include "datatypedef.h"
#include "DcmReadInterface.h"
#include "SingleSliceViewWdt.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
	void CreateWdts();                             //��������
	
	void resizeEvent(QResizeEvent* event);         //�ض���ߴ�
	void paintEvent(QPaintEvent* event);           //����

	void renderWindow();                           //���ƴ���

private:
	QList<SingleSliceViewWdt*> mThreeSliceVisualWdts;       //������Ƭ���ڲ���
	QVTKWidget* mp3DVisualWdt;                              //��άͼ�񴰿ڲ���

	DICOMPropertyData* mpDicomImgPty;                       //DICOM�ļ�������Ϣ
	vtkVolume* mpVolume;                                    //�����ڼ�����Ⱦ�е�vtkActor�����ڱ�ʾ��Ⱦ�����еĶ���
	vtkImageImport* mpImgImport;                            
	vtkSmartVolumeMapper* mpWholeVolMapper;                 //����һ�������Mapper
	vtkPiecewiseFunction* mpCurOpacityFun;                  //��͸����ӳ�亯��

	vtkRenderer* mpRenderer;                                //������

	char* mInfoDir;               //��Ϣ·��
	

};

#endif // MAINWINDOW_H
