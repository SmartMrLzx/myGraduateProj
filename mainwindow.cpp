#include "mainwindow.h"
#include <QStyleOption>
#include <QPainter>

#define Volume_Or_Surface_Rendering 0

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{

	QString str = "./Data";                               //设置文件路径
	QByteArray ba = str.toLatin1();
	mInfoDir = ba.data();
	mpDicomImgPty = new DICOMPropertyData;                //创建体素数据指针变量
	DcmReadInterface::DcmReader(mInfoDir);                //使用DcmReader载入路径
	DcmReadInterface::GetPatientInfo(*mpDicomImgPty);     //获取病人信息
	DcmReadInterface::GetVolumeData(*mpDicomImgPty);      //获取体素数据
	
	this->resize(800, 600);         //重置尺寸
	CreateWdts();                   //创建部件
	renderWindow();                 //通过绘制窗口绘制
}

MainWindow::~MainWindow()
{

}

void MainWindow::CreateWdts()
{
	T_2DWdt_Type type[] = { C_Axial, C_Coronal, C_Sagittal };
	int size = sizeof(type) / sizeof(T_2DWdt_Type);
	for (int i = 0; i < size; i++)
	{
		SingleSliceViewWdt* wdt = new SingleSliceViewWdt(this, type[i], mpDicomImgPty);
		mThreeSliceVisualWdts.append(wdt);
	}

}

void MainWindow::resizeEvent(QResizeEvent * event)
{
	if (!mThreeSliceVisualWdts.isEmpty())
	{

		double wdt1_pos[4] = { 0,0,0.4,0.5 };
		double wdt2_pos[4] = { 0.4,0,0.4,0.5 };
		double wdt3_pos[4] = { 0,0.5,0.4,0.5 };

		double* wdts_resize_pos[] =
		{
			wdt1_pos, wdt2_pos, wdt3_pos
		};
		int width = this->width();
		int height = this->height();

		int size = mThreeSliceVisualWdts.size();

		for (int i = 0; i < size; i++)
		{
			mThreeSliceVisualWdts.at(i)->setGeometry(wdts_resize_pos[i][0]* width, wdts_resize_pos[i][1]* height,
				wdts_resize_pos[i][2]* width, wdts_resize_pos[i][3]* height);
		}

	}

	if (mp3DVisualWdt)
	{
		int width = this->width();
		int height = this->height();
		mp3DVisualWdt->setGeometry(0.4*width, 0.5*height, 0.4*width, 0.5*height);
	}

	QWidget::resizeEvent(event);

}

void MainWindow::paintEvent(QPaintEvent * event)
{
	QStyleOption opt;
	opt.init(this);
	QPainter p(this);
	style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
	QWidget::paintEvent(event);
}

void MainWindow::renderWindow()
{

#if Volume_Or_Surface_Rendering
	mpImgImport = vtkImageImport::New();                                   //vtk图像输入接口
	mpImgImport->SetImportVoidPointer(mpDicomImgPty->ptrData);             //设置输入接口的空指针
	mpImgImport->SetDataExtent(0, mpDicomImgPty->xSize - 1, 0, mpDicomImgPty->ySize - 1, 0, mpDicomImgPty->zSize - 1);      //设置数据大小
	mpImgImport->SetWholeExtent(0, mpDicomImgPty->xSize - 1, 0, mpDicomImgPty->ySize - 1, 0, mpDicomImgPty->zSize - 1);     //设置总大小
	mpImgImport->SetDataOrigin(0, 0, 0);               //设置起点位置
	mpImgImport->SetDataSpacing(mpDicomImgPty->xSpacing, mpDicomImgPty->ySpacing, mpDicomImgPty->zSpacing);          //设置间隔
	mpImgImport->Update();

	mpWholeVolMapper = vtkSmartVolumeMapper::New();
	mpWholeVolMapper->SetInputData(mpImgImport->GetOutput());
	mpWholeVolMapper->SetBlendModeToComposite();                                     //设置
	mpWholeVolMapper->InteractiveAdjustSampleDistancesOff();                         //关闭交互式调整采样距离功能
	mpWholeVolMapper->AutoAdjustSampleDistancesOff();                                //关闭自动条件采样距离功能
	mpWholeVolMapper->Update();

	vtkColorTransferFunction *colorFun = vtkColorTransferFunction::New();            //颜色映射函数
	vtkPiecewiseFunction* opacityFun = vtkPiecewiseFunction::New();                  //vtkPiecewiseFunction类定义标量线性分段函数，其支持两种设置方式。第一种是直接添加断点，第二种则是添加一条线段。
	colorFun->AddRGBPoint(-3024, 0, 0, 0, 0.5, 0.0);                        //
	colorFun->AddRGBPoint(-16, .98, .78, .61);                              //第一个参数表示像素灰度值，第二至第四个参数为映射的RGB分量
	colorFun->AddRGBPoint(-10, 0.0, 0.0, 0.21);
	colorFun->AddRGBSegment(10, .99, .42, 0.1, 50, .99, .42, 0.1);          //前四个参数为第一个RGB断点，后四个参数为第二个RGB断点。
	colorFun->AddRGBPoint(60, 0.73, 0.25, 0.30, 0.49, .61);
	colorFun->AddRGBPoint(641, .90, .82, .56, .5, 0.0);
	colorFun->AddRGBPoint(1000, 0.91, 0.91, 0.91);
	colorFun->AddRGBPoint(3071, 1, 1, 1, .5, 0.0);
	opacityFun->AddPoint(-3024, 0.0, 0.5, 0.0);              //
	opacityFun->AddPoint(-600, 0);                           //第一个参数为自变量，这里指灰度值；第二个参数则是映射值，这里指不透明度。
	opacityFun->AddPoint(-500, 0.2, .49, .61);
	opacityFun->AddPoint(-300, 0.2);
	opacityFun->AddSegment(-101, 0, 300, 0);                 //添加两个断点(-101,0)和(300,0)，组成一条线段
	opacityFun->AddSegment(641, 0.5, 1000, 0.5);
	opacityFun->AddPoint(3071, 0.0);

	vtkVolumeProperty* volproperty = vtkVolumeProperty::New();         //用于设置体绘制的属性设置，决定体绘制的渲染效果
	volproperty->SetColor(colorFun);                                   //该函数用于设置颜色传输函数
	volproperty->SetScalarOpacity(opacityFun);                         //该函数用于设置灰度不透明度函数
	volproperty->SetInterpolationTypeToLinear();                       //线性差值
	volproperty->ShadeOn();                  //打开体绘制的阴影效果(Shading)
	volproperty->SetAmbient(0.1);            //设置环境光系数
	volproperty->SetDiffuse(0.9);            //设置散射光系数
	volproperty->SetSpecular(0.2);           //设置反射光系数
	volproperty->SetSpecularPower(10.0);     //设置高光强度，用于控制体绘制的外观平滑程度

	mpVolume = vtkVolume::New();
	mpVolume->SetMapper(mpWholeVolMapper);             //该函数用于连接vtkSmartVolumeMapper对象，并根据不同的体绘制算法获取其内部生成的图元数据。
	mpVolume->SetProperty(volproperty);                //该函数用于设置vtkVolumeProperty对象。其中vtkVolumeProperty用来设置体绘制的颜色和不透明度函数以及阴影信息。
	mpVolume->Update();

	mpRenderer = vtkRenderer::New();
	mpRenderer->AddVolume(mpVolume);
	mpRenderer->ResetCamera(mpVolume->GetBounds());

	double *center = mpVolume->GetCenter();
	vtkCamera *camera = mpRenderer->GetActiveCamera();
	camera->SetViewUp(0, 0, 1);
	camera->SetFocalPoint(center[0], center[1], center[2]);
	camera->SetPosition(center[0], center[1] - 300, center[2]);
	camera->SetViewAngle(30.0);
	camera->ComputeViewPlaneNormal();

	vtkRenderWindow*renWin = vtkRenderWindow::New();
	renWin->AddRenderer(mpRenderer);
	vtkRenderWindowInteractor*iren = vtkRenderWindowInteractor::New();
	vtkInteractorStyleTrackballCamera*style = vtkInteractorStyleTrackballCamera::New();
	iren->SetInteractorStyle(style);
	iren->SetRenderWindow(renWin);

	mp3DVisualWdt = new QVTKWidget(this);
	mp3DVisualWdt->SetRenderWindow(renWin);
	iren->Initialize();
	renWin->Render();
	mp3DVisualWdt->update();
	
#else
	vtkDICOMImageReader*Dcmreader = vtkDICOMImageReader::New();
	Dcmreader->SetDirectoryName(mInfoDir);
	Dcmreader->Update();

	vtkMarchingCubes* boneExtractor = vtkMarchingCubes::New();
	boneExtractor->SetInputConnection(Dcmreader->GetOutputPort());
	boneExtractor->SetValue(0, 200); //设置提取的等值信息

	 //剔除旧的或废除的数据单元，提高绘制速度
	vtkStripper *boneStripper = vtkStripper::New(); //三角带连接
    //注意：vtk6.0以后的版本，管道的连接函数修改为了SetInputConnection()和GetOutputPort().
	boneStripper->SetInputConnection(boneExtractor->GetOutputPort());

	//建立映射
	vtkPolyDataMapper *boneMapper = vtkPolyDataMapper::New();
	//注意：vtk6.0以后的版本，管道的连接函数修改为了SetInputConnection()和GetOutputPort().
	boneMapper->SetInputConnection(boneStripper->GetOutputPort());
	
	//建立角色
	vtkActor *bone = vtkActor::New();
	bone->SetMapper(boneMapper);
	bone->GetProperty()->SetColor(191.0 / 255.0, 200.0 / 255.0, 200.0 / 255.0);
	bone->GetProperty()->SetSpecular(.3);
	bone->GetProperty()->SetSpecularPower(20);
	
	//标准句子
	//定义绘制器
	mpRenderer = vtkRenderer::New();
	//定义绘制窗口
	vtkRenderWindow *renWin = vtkRenderWindow::New();
	renWin->AddRenderer(mpRenderer);
	//定义窗口交互器
	vtkRenderWindowInteractor *iren = vtkRenderWindowInteractor::New();
	iren->SetRenderWindow(renWin);
	vtkInteractorStyleTrackballCamera*style = vtkInteractorStyleTrackballCamera::New();
	iren->SetInteractorStyle(style);

	//创建一个camera
	vtkCamera *aCamera = vtkCamera::New();
	aCamera->SetViewUp(0, 0, -1);
	aCamera->SetPosition(0, 1, 0);
	aCamera->SetFocalPoint(0, 0, 0);

	mpRenderer->AddActor(bone);
	mpRenderer->SetActiveCamera(aCamera);
	mpRenderer->ResetCamera();
	aCamera->Dolly(1.5);
	mpRenderer->SetBackground(0, 0, 0);
	mpRenderer->ResetCameraClippingRange();

	mp3DVisualWdt = new QVTKWidget(this);
	mp3DVisualWdt->SetRenderWindow(renWin);
	iren->Initialize();
	mpRenderer->Render();
	mp3DVisualWdt->update();

#endif

}
