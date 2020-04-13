#ifndef __DSReaderDefine_h
#define __DSReaderDefine_h

#include <string>
#include<vector>
#ifdef __cplusplus
#define DLLExport  extern "C" __declspec(dllexport)
#else
#define DLLExport  extern "C"
#endif

typedef void ( *CallBackFunction)(int step, int proc);

#ifdef __cplusplus
	extern "C" {
#endif


#ifndef UNICODE                    // UNICODE 2 need  define this
	typedef  char     DSR_CHAR; 
#else
	typedef  wchar_t  DSR_CHAR;
#endif


//            ״̬��Ϣ
// --------------------------------------------------------------------

//#define DCMLIB_S_OK                    0       // �ɹ�
//#define DCMLIB_S_FAILURE               1       // ʧ��
//#define DCMLIB_S_WORKING               2       // ִ����
//
//#define DCMLIB_E_UNEXPECTED           0x81     // �쳣
//#define DCMLIB_E_INTERNALE            0x82     // �ڲ�����
//#define DCMLIB_E_OUT_OF_MEM           0x83     // �ڴ治��
//#define DCMLIB_E_SERVER_NOT_RESPONE   0x84     // ����������Ӧ
//#define DCMLIB_E_FAILED_TO_EXEC       0x85     // ִ��ʧ��
//#define DCMLIB_E_ACCESS_DENIED        0x86     // �ܾ�����
//#define DCMLIB_E_INPUT_FILENOFOUND    0x87     // �����ļ�������
//
//
//#define DCMLIB_F_NETWORK              0x90     // ��������ʧ�ܣ���ȷ������������������������������
//#define DCMLIB_F_INVALID_OBJ          0x91     // ����ָ����Ч
//#define DCMLIB_F_INVALID_QUERY        0x92     // ��Ч�Ĳ�ѯ����

/***********************************************************/
//���巵�ص�״̬��Ϣ
/***********************************************************/
	enum E_GDCM_Info
	{
		E_OK,				//�ɹ�
		E_DIR_NULL,			//·��Ϊ��
		E_FILE_FAIL,		//�ļ���ȡʧ��
		E_SLICE_FAIL,		//��Ƭ��ȡʧ��
		E_SLICE_ONLY_ONE,	//��Ƭֻ��һ��
		E_SLICE_TOO_LESS,	//��Ƭ��������
		E_BUFF_NULL,		//�ڴ�Ϊ��
		E_DSREADER_NULL		//DSReaderΪ��
	};
/***********************************************************/
//������������
/***********************************************************/
	typedef signed char int8;
	typedef signed short int16;
	typedef signed int int32;
	typedef long long int int64;

	typedef unsigned char uint8;
	typedef unsigned short uint16;
	typedef unsigned int uint32;
	typedef unsigned long long int uint64;

	typedef float real32;
	typedef double real64;


//#define BasicMsgTextSize 23

typedef	struct 
{
	int SeriesID;				//���е����
	int SeriesNumber;			//���к�
	std::vector<std::string*> *filenames;//��ͬ���е��ļ���
	int16 *pSingleData;			//����ͼ������
	int16 ImgSizeX;				//��Ƭͼ��Ŀ��
	int16 ImgSizeY;				//��Ƭͼ��ĸ߶�
	real32 ImgSpacingX;		    //x��������ؼ��
	real32 ImgSpacingY;			//y��������ؼ��
	real32 ImgThickness;		//ͼ����
	int16    windowCenter;		//��λ
	int16    windowWidth;		//����
	real32    rescaleIntercept;	//�Ҷ�תHUֵ�Ľؾ�
	real32    rescaleSlope;		//�Ҷ�תHUֵ��б��

	std::string  SeriesDescription;	//��������
	std::string  AccessionNumber;	//����
	std::string  admissionid;		//����ID
	std::string  name;				//��������
	std::string  gender;			//�����Ա�
	std::string  age;				//��������

	std::string  InstitutionName;	//���ҽԺ
	std::string  part;				//��鲿λ
	std::string  date;				//�������
	std::string  AcquisitionTime;	//���ʱ��
	std::string  doctor;			//ҽ������
	std::string  modality;			//��������ģ̬
	int16  XrayTubeCurrent;			//�ع����
	int16  KVP;						//�ع��ѹ
	
}DifferentSeries;

typedef struct {
	int16  dataType;			//���ݷ������ͣ��������Ϊ�з��Ż����޷���
	int16   xSize;				//��Ƭͼ��Ŀ��
	int16   ySize;				//��Ƭͼ��ĸ߶�
	int16   zSize;				//��Ƭͼ�������
	real32   xSpacing;			//x��������ؼ��
	real32   ySpacing;			//y��������ؼ��
	real32   zSpacing;			//z��������ؼ��
	int16    windowCenter;		//��λ
	int16    windowWidth;		//����
	real32    rescaleIntercept;	//�Ҷ�תHUֵ�Ľؾ�
	real32    rescaleSlope;		//�Ҷ�תHUֵ��б��
	int16*    ptrData;			//������ָ��
/**************�޸ĺ����Ϣ--2017/7/12--��֪��***************/
	std::string  admissionid;	//����ID
	std::string  name;			//��������
	std::string  gender;		//�����Ա�
	std::string  age;			//��������
	std::string  part;			//��鲿λ
	std::string  date;			//�������
	std::string  doctor;		//ҽ������
/**************���ģ̬��Ϣ--2017/7/26--��֪��***************/
	std::string  modality;		//��������ģ̬
/**************��ӵ���ͼ��������Ϣ--2017/7/27--��֪��***************/
	int16*	pSingleData;		//����ͼ������ָ��
/**************��ӵ���ͼ��������Ϣ--2018/2/1--��֪��***************/
	std::string  BirthDate;		//���˳�������
	std::string  Manufacturer;	//ɨ���豸
	std::string  AccessionNumber;	//����
	std::string  InstitutionName;	//���ҽԺ
	std::string  AcquisitionTime;	//���ʱ��
	int16  PatientSize;				//�������
	int16  PatientWeight;			//��������
	int16  ExposureTime;			//�ع�ʱ��
	int16  XrayTubeCurrent;			//�ع����
	int16  KVP;						//�ع��ѹ

} DSR_VolumeInfo;

#ifdef __cplusplus
	}
#endif





#endif
