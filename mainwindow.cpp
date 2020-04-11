#include "mainwindow.h"
#include <QStyleOption>
#include <QPainter>

#define Volume_Or_Surface_Rendering 0

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{

	QString str = "./Data";
	QByteArray ba = str.toLatin1();
	mInfoDir = ba.data();
	mpDicomImgPty = new DICOMPropertyData;
	DcmReadInterface::DcmReader(mInfoDir);
	DcmReadInterface::GetPatientInfo(*mpDicomImgPty);
	DcmReadInterface::GetVolumeData(*mpDicomImgPty);
	
	this->resize(800, 600);
	CreateWdts();
	renderWindow();
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
	mpImgImport = vtkImageImport::New();
	mpImgImport->SetImportVoidPointer(mpDicomImgPty->ptrData);
	mpImgImport->SetDataExtent(0, mpDicomImgPty->xSize - 1, 0, mpDicomImgPty->ySize - 1, 0, mpDicomImgPty->zSize - 1);      //�������ݴ�С
	mpImgImport->SetWholeExtent(0, mpDicomImgPty->xSize - 1, 0, mpDicomImgPty->ySize - 1, 0, mpDicomImgPty->zSize - 1);
	mpImgImport->SetDataOrigin(0, 0, 0);               //�������λ��
	mpImgImport->SetDataSpacing(mpDicomImgPty->xSpacing, mpDicomImgPty->ySpacing, mpDicomImgPty->zSpacing);          //���ü��
	mpImgImport->Update();

	mpWholeVolMapper = vtkSmartVolumeMapper::New();
	mpWholeVolMapper->SetInputData(mpImgImport->GetOutput());
	mpWholeVolMapper->SetBlendModeToComposite();                                     //����
	mpWholeVolMapper->InteractiveAdjustSampleDistancesOff();                         //�رս���ʽ�����������빦��
	mpWholeVolMapper->AutoAdjustSampleDistancesOff();                                //�ر��Զ������������빦��
	mpWholeVolMapper->Update();

	vtkColorTransferFunction *colorFun = vtkColorTransferFunction::New();            //��ɫӳ�亯��
	vtkPiecewiseFunction* opacityFun = vtkPiecewiseFunction::New();                  //vtkPiecewiseFunction�ඨ��������Էֶκ�������֧���������÷�ʽ����һ����ֱ����Ӷϵ㣬�ڶ����������һ���߶Ρ�
	colorFun->AddRGBPoint(-3024, 0, 0, 0, 0.5, 0.0);                        //
	colorFun->AddRGBPoint(-16, .98, .78, .61);                              //��һ��������ʾ���ػҶ�ֵ���ڶ������ĸ�����Ϊӳ���RGB����
	colorFun->AddRGBPoint(-10, 0.0, 0.0, 0.21);
	colorFun->AddRGBSegment(10, .99, .42, 0.1, 50, .99, .42, 0.1);          //ǰ�ĸ�����Ϊ��һ��RGB�ϵ㣬���ĸ�����Ϊ�ڶ���RGB�ϵ㡣
	colorFun->AddRGBPoint(60, 0.73, 0.25, 0.30, 0.49, .61);
	colorFun->AddRGBPoint(641, .90, .82, .56, .5, 0.0);
	colorFun->AddRGBPoint(1000, 0.91, 0.91, 0.91);
	colorFun->AddRGBPoint(3071, 1, 1, 1, .5, 0.0);
	opacityFun->AddPoint(-3024, 0.0, 0.5, 0.0);              //
	opacityFun->AddPoint(-600, 0);                           //��һ������Ϊ�Ա���������ָ�Ҷ�ֵ���ڶ�����������ӳ��ֵ������ָ��͸���ȡ�
	opacityFun->AddPoint(-500, 0.2, .49, .61);
	opacityFun->AddPoint(-300, 0.2);
	opacityFun->AddSegment(-101, 0, 300, 0);                 //��������ϵ�(-101,0)��(300,0)�����һ���߶�
	opacityFun->AddSegment(641, 0.5, 1000, 0.5);
	opacityFun->AddPoint(3071, 0.0);

	vtkVolumeProperty* volproperty = vtkVolumeProperty::New();         //������������Ƶ��������ã���������Ƶ���ȾЧ��
	volproperty->SetColor(colorFun);                                   //�ú�������������ɫ���亯��
	volproperty->SetScalarOpacity(opacityFun);                         //�ú����������ûҶȲ�͸���Ⱥ���
	volproperty->SetInterpolationTypeToLinear();                       //���Բ�ֵ
	volproperty->ShadeOn();                  //������Ƶ���ӰЧ��(Shading)
	volproperty->SetAmbient(0.1);            //���û�����ϵ��
	volproperty->SetDiffuse(0.9);            //����ɢ���ϵ��
	volproperty->SetSpecular(0.2);           //���÷����ϵ��
	volproperty->SetSpecularPower(10.0);     //���ø߹�ǿ�ȣ����ڿ�������Ƶ����ƽ���̶�

	mpVolume = vtkVolume::New();
	mpVolume->SetMapper(mpWholeVolMapper);             //�ú�����������vtkSmartVolumeMapper���󣬲����ݲ�ͬ��������㷨��ȡ���ڲ����ɵ�ͼԪ���ݡ�
	mpVolume->SetProperty(volproperty);                //�ú�����������vtkVolumeProperty��������vtkVolumeProperty������������Ƶ���ɫ�Ͳ�͸���Ⱥ����Լ���Ӱ��Ϣ��
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
	boneExtractor->SetValue(0, 200); //������ȡ�ĵ�ֵ��Ϣ

	 //�޳��ɵĻ�ϳ������ݵ�Ԫ����߻����ٶ�
	vtkStripper *boneStripper = vtkStripper::New(); //���Ǵ�����
    //ע�⣺vtk6.0�Ժ�İ汾���ܵ������Ӻ����޸�Ϊ��SetInputConnection()��GetOutputPort().
	boneStripper->SetInputConnection(boneExtractor->GetOutputPort());

	//����ӳ��
	vtkPolyDataMapper *boneMapper = vtkPolyDataMapper::New();
	//ע�⣺vtk6.0�Ժ�İ汾���ܵ������Ӻ����޸�Ϊ��SetInputConnection()��GetOutputPort().
	boneMapper->SetInputConnection(boneStripper->GetOutputPort());
	
	//������ɫ
	vtkActor *bone = vtkActor::New();
	bone->SetMapper(boneMapper);
	bone->GetProperty()->SetColor(191.0 / 255.0, 200.0 / 255.0, 200.0 / 255.0);
	bone->GetProperty()->SetSpecular(.3);
	bone->GetProperty()->SetSpecularPower(20);
	
	//��׼����
	//���������
	mpRenderer = vtkRenderer::New();
	//������ƴ���
	vtkRenderWindow *renWin = vtkRenderWindow::New();
	renWin->AddRenderer(mpRenderer);
	//���崰�ڽ�����
	vtkRenderWindowInteractor *iren = vtkRenderWindowInteractor::New();
	iren->SetRenderWindow(renWin);
	vtkInteractorStyleTrackballCamera*style = vtkInteractorStyleTrackballCamera::New();
	iren->SetInteractorStyle(style);

	//����һ��camera
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
