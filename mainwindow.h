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
	void CreateWdts();                             //创建部件
	
	void resizeEvent(QResizeEvent* event);         //重定义尺寸
	void paintEvent(QPaintEvent* event);           //绘制

	void renderWindow();                           //绘制窗口

private:
	QList<SingleSliceViewWdt*> mThreeSliceVisualWdts;       //三个切片窗口部件
	QVTKWidget* mp3DVisualWdt;                              //三维图像窗口部件

	DICOMPropertyData* mpDicomImgPty;                       //DICOM文件属性信息
	vtkVolume* mpVolume;                                    //类似于几何渲染中的vtkActor，用于表示渲染场景中的对象
	vtkImageImport* mpImgImport;                            
	vtkSmartVolumeMapper* mpWholeVolMapper;                 //定义一个体绘制Mapper
	vtkPiecewiseFunction* mpCurOpacityFun;                  //不透明度映射函数

	vtkRenderer* mpRenderer;                                //绘制器

	char* mInfoDir;               //信息路径
	

};

#endif // MAINWINDOW_H
