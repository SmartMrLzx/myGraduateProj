#include "SingleSliceViewWdt.h"
#include <QStyleOption>
#include <QPainter>

SingleSliceViewWdt::SingleSliceViewWdt(QWidget *parent, T_2DWdt_Type type, DICOMPropertyData* mpDicomImgPty)
	: QWidget(parent) ,mWdtType(type), m_pDicomImgPty(mpDicomImgPty)
{
	isUpdateSlice = false;                 //将切面更新状态设置为false
	SetVolumeData(m_pDicomImgPty);         //设置体数据
	CreateWdts();                          //创建部件
}

SingleSliceViewWdt::~SingleSliceViewWdt()
{

}

void SingleSliceViewWdt::SetVolumeData(DICOMPropertyData* mpDicomImgPty)
{
	mProperty.imgtype = mWdtType;       //读入切面类型

	switch (mProperty.imgtype)
	{
		case T_2DWdt_Type::C_Axial:                            //横断面
		{
			mProperty.width = mpDicomImgPty->xSize;
			mProperty.height = mpDicomImgPty->ySize;
			mProperty.total = mpDicomImgPty->zSize;
		}break;
		case T_2DWdt_Type::C_Coronal:                         //冠状面
		{
			mProperty.width = mpDicomImgPty->xSize;
			mProperty.height = mpDicomImgPty->zSize;
			mProperty.total = mpDicomImgPty->ySize;
		}break;
		case T_2DWdt_Type::C_Sagittal:                        //矢状面
		{
			mProperty.width = mpDicomImgPty->ySize;
			mProperty.height = mpDicomImgPty->zSize;
			mProperty.total = mpDicomImgPty->xSize;
		}break;
		default:
			break;
	}

	mProperty.order = mProperty.total / 2;                               //位置在总数的一半

	mPtDcmPty.rescaleIntercept = mpDicomImgPty->rescaleIntercept;        //读取灰度转HU值的截距
	mPtDcmPty.rescaleSlope = mpDicomImgPty->rescaleSlope;                //读取灰度转HU值的斜率
	mPtDcmPty.windowCenter = mpDicomImgPty->windowCenter;                //读取窗位
	mPtDcmPty.windowWidth = mpDicomImgPty->windowWidth;                  //读取窗宽
}

void SingleSliceViewWdt::CreateWdts()
{
	mpScrollNextImgSlider = new QSlider(this);                //创建新滑块
	mpScrollNextImgSlider->setOrientation(Qt::Vertical);      //竖直方向放置
	mpScrollNextImgSlider->setRange(0, mProperty.total-1);    //设置范围
	mpScrollNextImgSlider->setSingleStep(1);                  //设置单步距离
	mpScrollNextImgSlider->setObjectName("mpScrollNextImgSlider");        //设置对象名
	mpScrollNextImgSlider->setStyleSheet(QString("#mpScrollNextImgSlider") + "{background-color: transparent;border:0.5px solid rgb(72,76,81);}");   //设置外观
	mpScrollNextImgSlider->setValue(mProperty.order);         //设置当前滑块的值，即切面位置

	connect(mpScrollNextImgSlider, &QSlider::valueChanged, this, &SingleSliceViewWdt::ProSliderValueChangeEvent);         //滑块值改变时，发送信号

	mpInfoLbl = new QLabel(this);       //信息标签
	QString text = QString("c:") + QString::number(mProperty.order + 1)  +"\n"
		+ QString("t:") + QString::number(mProperty.total);                       //标签为：c：位置  \n   t:总数
	mpInfoLbl->setText(text);           //设置标签文字
	mpInfoLbl->setObjectName("mpInfoLbl");         //设置对象名为mpInfoLabel
	mpInfoLbl->setStyleSheet(QString("#mpInfoLbl") + "{background-color: transparent;border:0.5px solid rgb(72,76,81);}");    //设置样式
	mpInfoLbl->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);           //水平居中，垂直居中

	mpSliceImg = new QLabel(this);            //图像Label
	mpSliceImg->setObjectName("wdt1");        //设置对象名
	mpSliceImg->setStyleSheet(QString("#wdt1") + "{background-color: rgb(24,33,45);border:0.5px solid rgb(72,76,81);}");      //设置样式

	uchar* initial_img = GetDcmImage(&mProperty, &mPtDcmPty);    //图像数据
	mpCellSliceImg = new QImage(initial_img, mProperty.width, mProperty.height, QImage::Format_Grayscale8);     //参数QImage::Format_Grayscale8 将彩色图转
	                                                                                                            //换成灰度图
	                                                                                                            //设置图像大小为窗口大小

}

void SingleSliceViewWdt::resizeEvent(QResizeEvent * event)
{

	int width = this->width();           //获取当前窗口的宽度
	int height = this->height();         //获取当前窗口的高度

	if (mpSliceImg)      //如果有切面图像
	{
		mpSliceImg->setGeometry(0, 0, width*0.95, height);        //绘图区域：(0,0)，宽：width*0.95，高：height

		QImage img = mpCellSliceImg->scaled(mpSliceImg->width(), mpSliceImg->height());     //缩放图像

		mpSliceImg->setPixmap(QPixmap::fromImage(img));            //将图像载入imagLabel

		mpSliceImg->update();                                      //显示图像

	}

	if (mpInfoLbl)
	{
		mpInfoLbl->setGeometry(width*0.95, 0, width*0.05, height*0.1);       //信息标签位置：(width*0.95,0)，宽：width*0.05，高：height*0.1
	}

	if (mpScrollNextImgSlider)
	{
		mpScrollNextImgSlider->setGeometry(width*0.95, height*0.1, width*0.05, height*0.9);      //滑块位置：(width*0.95,height*0.1)，宽：width*0.05
		                                                                                         //高：height*0.9
	}

	//QWidget::resizeEvent(event);
}

void SingleSliceViewWdt::paintEvent(QPaintEvent * event)
{
	if (isUpdateSlice)       //如果要更新切面
	{
		QImage img = mpCellSliceImg->scaled(mpSliceImg->width(), mpSliceImg->height());       //缩放图像
		mpSliceImg->setPixmap(QPixmap::fromImage(img));              //将图像载入imagLabel
		mpSliceImg->update();                                        //显示图像
		isUpdateSlice = false;                //将更新图像判据置否
	}

	QStyleOption opt;
	opt.init(this);
	QPainter p(this);
	style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);     //

	//QWidget::paintEvent(event);
}


uchar* SingleSliceViewWdt::GetDcmImage(T_DImgPropety *pty, T_DImageWindowCtrl *mPtDcmPty)         //获取dcm图像
{
	T_DImgPropety *pPty = pty;          //读取切面相关数据
	if (pPty->order >= pPty->total)     //若序号超过总数则返回空指针
		return NULL;

	qint16  uWC = mPtDcmPty->windowCenter;    //读取窗位   
	qint16  uWW = mPtDcmPty->windowWidth;     //读取窗宽

	double Min = uWC - uWW / 2;          //设置最小值
	double Max = uWC + uWW / 2;          //设置最大值
 
	signed short *pData = m_pDicomImgPty->ptrData;        //读取体数据指针

	switch (pty->imgtype)              //读取切面类型
	{
	case T_2DWdt_Type::C_Axial:        //横断面
	{
		long fSize = pPty->width*pPty->height;     //计算尺寸=宽*高

		uchar *ubuffer = (uchar*)malloc(fSize);    //利用malloc函数开辟所需尺寸的空间


		long zlocation = pPty->order* fSize;       //z轴位置=次序*尺寸


		for (int j = 0; j < pPty->height; ++j)//＞              //遍历
		{
			for (int i = 0; i< pPty->width; ++i)//∨
			{
				signed short temp = (pData[zlocation + i * pPty->height + j] * mPtDcmPty->rescaleSlope
					+ mPtDcmPty->rescaleIntercept);                //像素值*slope+intercept
				float temp1 = (temp - Min) * 255 / (Max - Min);        //转换
				ubuffer[i*pPty->height + j] = DataExchange(temp1);     //第i行第j列的像素


			}
		}
		return ubuffer;
	}
	break;
	case T_2DWdt_Type::C_Coronal:      //冠状面
	{
		long fSize = pPty->width*pPty->height;

		unsigned char *ubuffer = (unsigned char*)malloc(fSize);
		long locationx = pPty->width*pPty->total;
		long locationy = pPty->order*pPty->width;

		for (int k = 0; k < pPty->height; ++k)
		{
			for (int i = 0; i < pPty->width; ++i)
			{

				signed short temp = pData[(pPty->height - 1 - k)*locationx + locationy + i] + mPtDcmPty->rescaleSlope
					+ mPtDcmPty->rescaleIntercept;
				float temp1 = (temp - Min) * 255 / (Max - Min);

				ubuffer[k*pPty->width + i] = DataExchange(temp1);


			}
		}

		return ubuffer;
	}

	break;
	case T_2DWdt_Type::C_Sagittal:         //矢状面
	{
		long fSize = pPty->width*pPty->height;
		unsigned char *ubuffer = (unsigned char*)malloc(fSize);

		long locationx = pPty->width*pPty->total;

		for (int k = 0; k < pPty->height; ++k)
		{
			for (int j = 0; j < pPty->width; ++j)
			{

				signed short temp = pData[locationx* (pPty->height - k - 1) + pPty->order + pPty->total*j] * mPtDcmPty->rescaleSlope
					+ mPtDcmPty->rescaleIntercept;
				float temp1 = (temp - Min) * 255 / (Max - Min);
				ubuffer[k*pPty->width + j] = DataExchange(temp1);

			}
		}
		return ubuffer;
	}
	break;
	default: break;
	}


	return NULL;
}


unsigned char SingleSliceViewWdt::DataExchange(float temp)
{
	unsigned char value = 0;

	if ((temp>0) && (temp<255))
	{
		value = (unsigned char)temp;
	}
	else if (temp>255)
	{
		value = 255;
	}
	else if (temp<0)
	{
		value = 0;
	}
	return value;
}

void SingleSliceViewWdt::ProSliderValueChangeEvent(int val)
{
	mProperty.order = val;     //读取滑块值
	uchar* next_img = GetDcmImage(&mProperty, &mPtDcmPty);       //读取下一张切面
	mpCellSliceImg = new QImage(next_img, mProperty.width, mProperty.height, QImage::Format_Grayscale8);

	isUpdateSlice = true;       //切面更新判据置true

	QString text = QString("c:") + QString::number(mProperty.order + 1)  +"\n"
		+ QString("t:") + QString::number(mProperty.total);
	mpInfoLbl->setText(text);
	mpInfoLbl->update();
}
