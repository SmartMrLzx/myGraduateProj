#include "SingleSliceViewWdt.h"
#include <QStyleOption>
#include <QPainter>

SingleSliceViewWdt::SingleSliceViewWdt(QWidget *parent, T_2DWdt_Type type, DICOMPropertyData* mpDicomImgPty)
	: QWidget(parent) ,mWdtType(type), m_pDicomImgPty(mpDicomImgPty)
{
	isUpdateSlice = false;                 //���������״̬����Ϊfalse
	SetVolumeData(m_pDicomImgPty);         //����������
	CreateWdts();                          //��������
}

SingleSliceViewWdt::~SingleSliceViewWdt()
{

}

void SingleSliceViewWdt::SetVolumeData(DICOMPropertyData* mpDicomImgPty)
{
	mProperty.imgtype = mWdtType;       //������������

	switch (mProperty.imgtype)
	{
		case T_2DWdt_Type::C_Axial:                            //�����
		{
			mProperty.width = mpDicomImgPty->xSize;
			mProperty.height = mpDicomImgPty->ySize;
			mProperty.total = mpDicomImgPty->zSize;
		}break;
		case T_2DWdt_Type::C_Coronal:                         //��״��
		{
			mProperty.width = mpDicomImgPty->xSize;
			mProperty.height = mpDicomImgPty->zSize;
			mProperty.total = mpDicomImgPty->ySize;
		}break;
		case T_2DWdt_Type::C_Sagittal:                        //ʸ״��
		{
			mProperty.width = mpDicomImgPty->ySize;
			mProperty.height = mpDicomImgPty->zSize;
			mProperty.total = mpDicomImgPty->xSize;
		}break;
		default:
			break;
	}

	mProperty.order = mProperty.total / 2;

	mPtDcmPty.rescaleIntercept = mpDicomImgPty->rescaleIntercept;
	mPtDcmPty.rescaleSlope = mpDicomImgPty->rescaleSlope;
	mPtDcmPty.windowCenter = mpDicomImgPty->windowCenter;
	mPtDcmPty.windowWidth = mpDicomImgPty->windowWidth;
}

void SingleSliceViewWdt::CreateWdts()
{
	mpScrollNextImgSlider = new QSlider(this);                //�����»���
	mpScrollNextImgSlider->setOrientation(Qt::Vertical);      //��ֱ�������
	mpScrollNextImgSlider->setRange(0, mProperty.total-1);    //���÷�Χ
	mpScrollNextImgSlider->setSingleStep(1);                  //���õ�������
	mpScrollNextImgSlider->setObjectName("mpScrollNextImgSlider");        //���ö�����
	mpScrollNextImgSlider->setStyleSheet(QString("#mpScrollNextImgSlider") + "{background-color: transparent;border:0.5px solid rgb(72,76,81);}");   //�������
	mpScrollNextImgSlider->setValue(mProperty.order);         //���õ�ǰ�����ֵ��������λ��

	connect(mpScrollNextImgSlider, &QSlider::valueChanged, this, &SingleSliceViewWdt::ProSliderValueChangeEvent);         //����ֵ�ı�ʱ�������ź�

	mpInfoLbl = new QLabel(this);       //��Ϣ��ǩ
	QString text = QString("c:") + QString::number(mProperty.order + 1)  +"\n"
		+ QString("t:") + QString::number(mProperty.total);
	mpInfoLbl->setText(text);
	mpInfoLbl->setObjectName("mpInfoLbl");
	mpInfoLbl->setStyleSheet(QString("#mpInfoLbl") + "{background-color: transparent;border:0.5px solid rgb(72,76,81);}");
	mpInfoLbl->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

	mpSliceImg = new QLabel(this);
	mpSliceImg->setObjectName("wdt1");
	mpSliceImg->setStyleSheet(QString("#wdt1") + "{background-color: rgb(24,33,45);border:0.5px solid rgb(72,76,81);}");

	uchar* initial_img = GetDcmImage(&mProperty, &mPtDcmPty);    //ͼ������
	mpCellSliceImg = new QImage(initial_img, mProperty.width, mProperty.height, QImage::Format_Grayscale8);     //����QImage::Format_Grayscale8 ����ɫͼת���ɻҶ�ͼ

}

void SingleSliceViewWdt::resizeEvent(QResizeEvent * event)
{

	int width = this->width();
	int height = this->height();

	if (mpSliceImg)
	{
		mpSliceImg->setGeometry(0, 0, width*0.95, height);        //��ͼ����

		QImage img = mpCellSliceImg->scaled(mpSliceImg->width(), mpSliceImg->height());

		mpSliceImg->setPixmap(QPixmap::fromImage(img));

		mpSliceImg->update();

	}

	if (mpInfoLbl)
	{
		mpInfoLbl->setGeometry(width*0.95, 0, width*0.05, height*0.1);       //��Ϣ��ǩλ��
	}

	if (mpScrollNextImgSlider)
	{
		mpScrollNextImgSlider->setGeometry(width*0.95, height*0.1, width*0.05, height*0.9);      //����λ��
	}

	//QWidget::resizeEvent(event);
}

void SingleSliceViewWdt::paintEvent(QPaintEvent * event)
{
	if (isUpdateSlice)
	{
		QImage img = mpCellSliceImg->scaled(mpSliceImg->width(), mpSliceImg->height());
		mpSliceImg->setPixmap(QPixmap::fromImage(img));
		mpSliceImg->update();
		isUpdateSlice = false;
	}

	QStyleOption opt;
	opt.init(this);
	QPainter p(this);
	style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);     //

	//QWidget::paintEvent(event);
}


uchar* SingleSliceViewWdt::GetDcmImage(T_DImgPropety *pty, T_DImageWindowCtrl *mPtDcmPty)
{
	T_DImgPropety *pPty = pty;
	if (pPty->order >= pPty->total)     //����ų��������򷵻ؿ�ָ��
		return NULL;

	qint16  uWC = mPtDcmPty->windowCenter;     
	qint16  uWW = mPtDcmPty->windowWidth;

	double Min = uWC - uWW / 2;
	double Max = uWC + uWW / 2;

	signed short *pData = m_pDicomImgPty->ptrData;

	switch (pty->imgtype)
	{
	case T_2DWdt_Type::C_Axial:        //�����
	{
		long fSize = pPty->width*pPty->height;

		uchar *ubuffer = (uchar*)malloc(fSize);


		long zlocation = pPty->order* fSize;


		for (int j = 0; j < pPty->height; ++j)//��
		{
			for (int i = 0; i< pPty->width; ++i)//��
			{
				signed short temp = (pData[zlocation + i * pPty->height + j] * mPtDcmPty->rescaleSlope
					+ mPtDcmPty->rescaleIntercept);
				float temp1 = (temp - Min) * 255 / (Max - Min);
				ubuffer[i*pPty->height + j] = DataExchange(temp1);


			}
		}
		return ubuffer;
	}
	break;
	case T_2DWdt_Type::C_Coronal:      //��״��
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
	case T_2DWdt_Type::C_Sagittal:         //ʸ״��
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
	mProperty.order = val;
	uchar* next_img = GetDcmImage(&mProperty, &mPtDcmPty);
	mpCellSliceImg = new QImage(next_img, mProperty.width, mProperty.height, QImage::Format_Grayscale8);

	isUpdateSlice = true;

	QString text = QString("c:") + QString::number(mProperty.order + 1)  +"\n"
		+ QString("t:") + QString::number(mProperty.total);
	mpInfoLbl->setText(text);
	mpInfoLbl->update();
}
