/******************************************************************************\*
**  Copyright (C), 2016-, Shanghai Jingyu Intelligent Control Co., Ltd.
**  File name:    Globals.h
**  Author:       Version:        Date:
**  Liu Shengju   1.1.1.0         16/..
**  Description:    // ������ϸ˵���˳����ļ���ɵ���Ҫ���ܣ�������ģ��
**                  // �����Ľӿڣ����ֵ��ȡֵ��Χ�����弰������Ŀ�
**                  // �ơ�˳�򡢶����������ȹ�ϵ
**  Others:         // �������ݵ�˵��
**  Function List:  // ��Ҫ�����б�ÿ����¼Ӧ���������������ܼ�Ҫ˵��
**    1. ....
**  History:        // �޸���ʷ��¼�б�ÿ���޸ļ�¼Ӧ�����޸����ڡ��޸�
**                  // �߼��޸����ݼ���
**    1. Date:
**       Author:
**       Modification:
**    2. ...
\*******************************************************************************/


#ifndef _KIN_GLOBAL_PARAMETER_H_
#define _KIN_GLOBAL_PARAMETER_H_

#include <QtCore>
#include "SystemLog.h"
#include<QColor>

//
#define PROCESS_FAIL            -1
#define Assert_Enable			1
#define Assert_Kinbio  {\
    if (Assert_Enable)\
{\
    qDebug() << "Runing: "__FILE__ << __LINE__ << endl; \
    }\
    }


//--------------------------------------ģ������ �� ͨ������--------------------------------
typedef enum EModuleTypeTag
{
    MODULE_TYPE_REV         = 0,
    MODULE_TYPE_TEMP_CTRL   = 1,              // �¿�ģ��
    MODULE_TYPE_FLOR_SAMPLE = 2,                // ӫ��ɼ�ģ��
    MODULE_TYPE_SYSTEM_ET   = 3,                  // ����ϵͳ�� �ȸǡ������¶ȵȵ�
    MODULE_TYPE_MAX
}E_ModuleType;

typedef enum E_ExperimentStepTag
{
    EXP_STEP_IDLE = 1,
    EXP_STEP_PREPARE,
    EXP_STEP_DNA_AMP,
    EXP_STEP_DNA_MELT,
    EXP_STEP_MAX
}E_ExperimentStep;

// ----------------�¿س���-------------------------
typedef enum E_TempFunIDTag
{
    TEMP_FUN_ID_OPEN_CLOSE      = 1,
    TEMP_FUN_ID_STATUS_CFG      = 2,
    TEMP_FUN_ID_TARGET_TEMP_CFG = 3,
    TEMP_FUN_ID_TEMP_UPTO_REPORT= 4,
    TEMP_FUN_ID_MODULE_PAR_CFG  =5,
    TEMP_FUN_ID_EXPERIMENT_PAR_CFG = 6,
    TEMP_FUN_ID_TEMP_REPORT     = 7,
    TEMP_FUN_ID_ERR_REPORT      = 8,
    TEMP_FUN_ID_MAX
}E_TempFunID;

typedef enum E_TempCtrlStatusTag
{
    TEMP_STATUS_DNA_AMP             = 1,
    TEMP_STATUS_DNA_MELT            = 2,
    TEMP_STATUS_TEC_UNABLE_CTRL     = 3,
    TEMP_STATUS_HOT_LID_UNABLE_CTRL = 4,
    TEMP_STATUS_SAMPLE_FLOR_START   = 5,
    TEMP_STATUS_SAMPLE_FLOR_STORP   = 6,
    TEMP_STATUS_MODULE_DEBUG        = 7,
    TEMP_STATUS_MAX
}E_TempCtrlStatus;

typedef enum E_TempModuleParTag
{
    TEMP_MOD_PAR_TEMP_MAKE_UP = 1,
    TEMP_MOD_PAR_TEMP_CTRL_KP = 2,
    TEMP_MOD_PAR_TEMP_CTRL_KI = 3,
    TEMP_MOD_PAR_TEMP_CTRL_KD = 4,
    TEMP_MOD_PAR_SOFTWARE_VER = 5,
    TEMP_MOD_PAR_HARDWARE_VER = 6,
    TEMP_MOD_PAR_PRODUCTION_DATE = 7,
    TEMP_MOD_PAR_MAX
}E_TempModulePar;

typedef enum E_ExperimentTempParTag
{
    EXP_TEMP_PAR_MELT_TYPE  = 1,    // �ܽ���������
    EXP_TEMP_PAR_INC_RATE   = 2,     // ��������
    EXP_TEMP_PAR_DEC_RATE   = 3,     // ��������
    EXP_TEMP_PAR_START_TEMP = 4,   // �ܽ���ʼ�¶�
    EXP_TEMP_PAR_END_TEMP   = 5,     // �ܽ���ֹ�¶�
    EXP_TEMP_PAR_MAX_TEMP   = 6,
    EXP_TEMP_PAR_MAX
}E_ExperimentTempPar;

// ------------------ӫ��ģ�������-------------------
typedef enum FluoFunIDTag
{
    FLOR_FUN_ID_RESERVE      = 0, //����
    FLOR_FUN_ID_START_SAMPLE = 1, //��ʼ�ɼ�ӫ��
    FLOR_FUN_ID_FLOR_REPORT  = 2, //����ӫ��ֵ
    FLOR_FUN_ID_ADC_REPORT   = 3, //����Ƶ��
    FLOR_FUN_ID_LED_INT_REPORT = 4, //
    FLOR_FUN_ID_MOD_PAR_CFG  = 5,  //ģ���������
    FLOR_FUN_ID_ERROR_REPORT = 6,  //����״̬����
    FLOR_FUN_ID_CONTINUE_NU  = 7,  //������������
    FLOR_FUN_ID_MOD_TEST     = 8,  //
    FLOR_FUN_ID_MAX//
}FluoFunID;

// ģ��������ã�FLOR_FUN_ID_MOD_PAR_CFG��
typedef enum E_FluoModParCmdTag
{
    FLOR_MOD_PAR_CMD_RESERVE        = 0, //����
    FLOR_MOD_PAR_CMD_LED_INTENSITY  = 1,
    FLOR_MOD_PAR_CMD_ADC_RATE       = 2,
    FLOR_MOD_PAR_CMD_ADC_NU         = 3,
    FLOR_MOD_PAR_CMD_LED_ON_GAP     = 4,
    FLOR_MOD_PAR_CMD_DELAY_TIME     = 5,
    FLOR_MOD_PAR_CMD_CHANNEL_NO     = 6,
    FLOR_MOD_PAR_CMD_CALCULATE_NU   = 7,
    FLOR_MOD_PAR_SOFTWARE_VER       = 8,
    FLOR_MOD_PAR_HARDWARE_VER       = 9,
    FLOR_MOD_PAR_PRODUCTION_DATE    = 10,
    FLOR_MOD_PAR_CMD_MAX
}E_FluoModParCmd;



// ------------------����ģ�������-------------------
typedef enum MainFunIDTag
{
    SYSTEM_CMD_COM_LINK             = 1, //��������
    SYSTEM_CMD_FAN_CTRL             = 2, //���ȿ���
    SYSTEM_CMD_REPORT_FAN_STATUS    = 3,
    SYSTEM_CMD_HOT_LID_CTRL         = 4, //�ȸǿ���
    SYSTEM_CMD_REPORT_HOT_LID_STATUS= 5, //�ȸ�״̬����
    SYSTEM_CMD_EXPERIMENT_STEP      = 6,//ʵ�����֪ͨ
    SYSTEM_CMD_SYSTEM_PAR_WR        = 7, // ����������д
    SYSTEM_CMD_HOT_LID_TEMP_REPORT  = 8, // �ȸ��¶ȵĶ�д   ����ʱʹ�á�
    SYSTEM_CMD_TEMP_COMPENSATE_WR   = 9, // �¶Ȳ���������д
    SYSTEM_CMD_ERROR_REPORT         = 10,
    SYSTEM_CMD_MAX
}MainFunID;

// ʵ����̵Ĵ�������
typedef enum  E_DnaExperimentStepTag
{
    EXPERIMENT_STEP_STANDBY         = 0,
    EXPERIMENT_STEP_PRE_PROCESS     = 1,//
    EXPERIMENT_STEP_AMPLIFYING      = 2,
    EXPERIMENT_STEP_MELTING         = 3,
    EXPERIMENT_STEP_SAMPLE_FLOR_START = 4,
    EXPERIMENT_STEP_SAMPLE_FLOR_STOP= 5,
    EXPERIMENT_STEP_DEBUG           = 6,
    EXPERIMENT_STEP_MAX
}E_DnaExperimentStep;

// Byte6-������룺 bit0��bit3Ϊ�����Ӵ��룬 bit4��bit7Ϊģ�����
typedef enum  E_ErrorCodeTag
{
    // �¿ذ����
    ERROR_CODE_TEMP_CTRL_MOD_CAN_COM_FAILED     = 0X11,
    ERROR_CODE_TEMP_CTRL_MOD_TEMP_INC_FAILED    = 0X12,
    ERROR_CODE_TEMP_CTRL_MOD_TEMP_DEC_FAILED    = 0X13,
    ERROR_CODE_TEMP_CTRL_MOD_TEMP_INC_SLOWLY    = 0X14,
    ERROR_CODE_TEMP_CTRL_MOD_TEMP_DEC_SLOWLY    = 0X15,
    ERROR_CODE_TEMP_CTRL_MOD_RIPPLE_BIG         = 0X16,
    ERROR_CODE_TEMP_CTRL_MOD_TEMP_SENSE_INVALID = 0X17,

    //  ӫ��ɼ�ģ�����
    ERROR_CODE_FLOR_SAMPLE_MOD_CAN_COM_FAILED   = 0X21,
    ERROR_CODE_FLOR_SAMPLE_MOD_ADC_COM_FAILED   = 0X22,
    ERROR_CODE_FLOR_SAMPLE_MOD_ADC_ERROR        = 0X23,
    ERROR_CODE_FLOR_SAMPLE_MOD_CALCULATE_SLOWLY = 0X24,
    ERROR_CODE_FLOR_SAMPLE_MOD_LED_UNLIGHTED    = 0X25,
    ERROR_CODE_FLOR_SAMPLE_MOD_LED_INTENSITY_LOW= 0X26,
    ERROR_CODE_FLOR_SAMPLE_MOD_LED_SLOWLY       = 0X27,

    // �������
    ERROR_CODE_MAIN_BORAD_CAN_COM_FAILED        = 0X31,
    ERROR_CODE_MAIN_BORAD_UART_COM_FAILED       = 0X32,
    ERROR_CODE_MAIN_BORAD_HOT_LID_INC_FAILED    = 0X33,
    ERROR_CODE_MAIN_BORAD_HOT_LID_CTRL_FAILED   = 0X34,
    ERROR_CODE_MAIN_BORAD_TEMP_SENSER_INVALID   = 0X35,
    ERROR_CODE_MAIN_BORAD_FAN_NO_WORK           = 0X36,
    ERROR_CODE_MAIN_BORAD_SINK_TEMP_SENSER_INVALID = 0X37,
}E_ErrorCode;

//=====================================����Ķ���==============================================================
//------------------------------- command main Id---------------------------------------------------
#define CMD_MAIN_ID_DAN_AMP_MELT_METHOD_MAN   0X01
#define CMD_MAIN_ID_EXPERIMENT_MANAGMENT      0X02
#define CMD_MAIN_ID_EXPERIMENT_REVIEW         0X03
#define CMD_MAIN_ID_INSTUMENT_MANAGMENT       0X04
#define CMD_MAIN_ID_DNA_EXTRACT_METHOD_MAN    0X05
#define CMD_MAIN_ID_UART_DATA_PACKET          0X06

//-------------------------------command sub Id----------------------------------------------------
//   Uart data packet communication sub Id
#define UART_DATA_PACKET_CMD_SUB_ID_TX       0X01
#define UART_DATA_PACKET_CMD_SUB_ID_RX       0X02

#define M_READ                                  0
#define M_WRITE                                 1

//���ݹ�����
union DataContent
{
    quint8  m_uint8data[4];
    quint32 m_uint32data;
    float   m_floatdata;
};

// ����ͨ��Э��İ�ͷ
typedef struct SUartCommandTag
{    
    quint32 unStartTag      : 8;    //0xFF
    quint32 unDataLen_empty : 8;
    quint32 unDataLen       : 8;
    quint32 unCrc           : 8;

    quint32 unFunId         : 4;
    quint32 unRorW          : 1;
    quint32 unNodeType      : 3;
    quint32 unStatusAck     : 8;   //0���� 1�ظ�
    quint32 unCh            : 4;
    quint32 unModuleType    : 4;
    quint32 unSubCmd        : 8;
}SUartCommand;

typedef union UUartCommandTag
{
    SUartCommand sUartCmd;
    quint8       ucCmd[8];
}UUartCommand;

typedef struct SCmdStructTag
{
    quint32 unCmdMainId   : 8;            // main   cmd
    quint32 unCmdSubId    : 8;            // sub    cmd
    quint32 unCmdSonId    : 8;            // Son    cmd
    quint32 unOtherId     : 8;            //
}SCmdStruct;


typedef struct SRequestCommandInfoTag
{ 
    SCmdStruct			sCmdId;			    // ������
    UUartCommand        uUartCmd;
    QString				strCmdContent;		// ��������  QString        ��ʽ
    QByteArray			baCmdContent;		// ��������  QByteArray�� �����Ǵ����ϴ���ӫ��ֵ������
    quint32             unOtherInfo;        // ͨ������  ����ָ�룬����PCR�����ļ��ķ���
}SRequestCmdInfo;

static float data_temp_1;
struct Table_Data
{
    int table_num;
    QList<bool> celldata_list;
};

struct CellInfo{
    QString SampleName;//��������
    QString ID;//Ψһ��ʾ��
    QString Name;//��������
    QString Sex;//�Ա�
    QString Age;//����
    QString SampleType;//�걾����
    QString SampleDate;//��������
    QString DoctorName;//�ͼ�ҽ��
    QString DoctorRoom;//�ͼ����
    QString DoctorID;//������
    QString DoctorRemarks;//�ٴ����
    QString Remarks;//��ע

    //���
    QString ProperName;//����
    QString Passage;//ͨ��
    QString DyesTuff;//Ⱦ��
    QString Target;//Ŀ��
    QString Color;//��ɫ
    QString Type;//����
    QString Ct;//Ct
    QString AverCt;//ƽ��Ct
    QString CtSD;//Ct SD
    QString CtCv;//Ct CV(%)
    QString Concen;//Ũ��
    QString AverConcen;//ƽ��Ũ��
    QString CnSd;//Cn SD
    QString CnCv;//Cn CV(%)
    QString Rn;//Rn
    QString InterRes;//�м���
    QString Conclu;//����
    QString BedNumber;//������
    QString AdmissNumber;//סԺ��
    QString OutpatNumber;//�����

};

struct CellData{
    QString     Reapore;//��Ӧ��
    QString     CellName;//ѡ�����Ŀ��
    int         SampleType      = -1;//ѡ����Ʒ����
    QString     SampleAttr;//��Ʒ����
    QString     SampleName;//��Ʒ����
    bool        SampleRepeat    = false;//�����Ƿ�ѡ��
    int         SampleRepeatID  = 0;//���ܱ��
};

struct CellUnit{
    CellData cell_unit_data;
    CellInfo cell_unit_info;
};
#define  ROW    8
#define  COL    6
struct ExperimentData
{
    QString     EquipmentType;//�����ͺ�
    QString     EquipmentID;//�������
    QString     Exp_Name;//ʵ������
    QString     Exp_SaveAddr;//����·��
    int         Exp_Type        = 0;//ʵ������
    QString     Exp_User;//������Ա
    QString     Exp_UserCheck;//�����Ա
    QString     Exp_Remarks;//��ע
    int         Exp_Modules     = 0;//��Ӧģ����ģ��A����B
    bool        Exp_Hotlid      = false;//�Ƿ�ѡ���ȸ�
    int         Exp_TemControl  = 0;//ģ����»����Թܿ���
    int         Exp_ChannelScan = 0;//ɨ��ͨ������
    CellUnit    Exp_Table[(ROW*COL)];
};
//-----------------------------------------����ʹ�ýṹ�嶨��------------------------------------------------------------//
//ѡ����Թܽṹ��
typedef struct Test_Tube
{
    QList<int> Passageway;//ѡ���ͨ��
    QList<QString> Passageway_Name;//ͨ������
    QString  TestTube_Name; //�Թ���
}Q268PCR_TEST_TUBE;
//ʵ������ṹ��
typedef struct Exper_Para
{
    QString Passageway_Name;//ͨ������
    QString  TestTube_Name; //�Թ���
    QString Passageway;//ѡ���ͨ����
    QString Anatype;//��������
    int Linestart;//�������
    int Lineend;//�����յ�
    QString Lineopti;//�����Ż�
    bool Autovalue;//�Զ���ֵ
    double Manualvalue;//�ֶ���ֵ
    QString Numberwave;//�����˲�
}Q268PCR_EXPER_PARA;
//�����ж������������������ݽṹ��
typedef struct Resue_Dete
{
    QString SampeType;//��Ʒ����
    QString CheckObject;//���Ŀ��
    int     ObjectVale;//��ֵ(Ŀ��)
    QString ObjectCompara;//�ȽϷ�(Ŀ��)
    QString DataType;//��������
    QString DataCompara;//�ȽϷ�(��������)
    int     DataVale;//��ֵ(��������)
   // bool    ResueDete;//�Ƿ�ѡ����ж�
    QString Conclusion;//����
    QColor  Color;//��ɫ
}Q268PCR_RESUE_DETE;
typedef struct Senior_Rule
{
   // bool    SeniorRule;//�Ƿ�ѡ�߼�����
    QString Path;//ѡ�õĸ߼������ļ�·��
    QString Interpre;//����ͽ���

}Q268PCR_SENIOR_RULE;


typedef  struct Field_Sett
{
    bool ItemCheck;
    QString ItemName;

}Q268PCR_FIELD_SETTING;

typedef  struct Tool_Config
{

   //����ʽ
    int Sortorder;
   //����ѡ��ʽ
    int Dataselect;
   //�������
    int Tablezoom;
   //��ʾ���������
    bool Sampleinter;
   //Ũ��ֵ�Կ�ѧ��������ʾ
    bool Sciencoun;
   //Y����ʾ��ʽ
    int Ydispalymode;
   //������ʽ
    int Curvestyle;
   //���������㷨
    int Amplcurvealgor;
   //ѡ�е�������ʾ����׼������
    bool Sampledisplaycurve;
   //��������ͼ����ʾ��ֵ��
    bool  Amplcurvethres;
   //�ڷ�ֵ��������ʾ��ֵ��
    bool Peakvalue;
   //���͵���
    bool Peakinvwe;
   //���ʵ�鷴Ӧ��ѡ��ʽ
    int Porechoose;
    //����Ctrl+����ѡ��
    bool Allowhole;
   //ͨ��ѡ��ʽ
    int Passagechoose;
   //�ȸ�����
    int Hotsetting;
   //����������ʾ
    bool Datadisplay;
   //������ʾ,����������
    bool Uiparameter;
   //�Զ���װ�
    bool Customhole;
   //�Զ��嵼��ʵ������
    bool Customexperdata;
   //�Զ�����ѡ��
    int Autoexporyop;
   //�����ļ���ʽ
    QString Fileformat;




}Q268PCR_TOOL_CONFIG;

typedef  struct Disscurve_typesele
{
    bool disscurve_check;//�Ƿ�����ܽ�����
    int diss_sele;//�ܽ�����ģʽ(1:�����ܽ⣬2:�ݶ��ܽ�)
    double  start_temp;//�ܽ�������ʼ�¶�
    double  end_temp;//�ܽ�������ֹ�¶�
    double  temp_rate;//�ܽ�������������
    double  temp_grad;//�ܽ����������ݶ�
    int  sleep_time;//�ܽ����������ݶ�ά��ʱ��(�¶ȵ�����ά��ʱ��)

}DISSCURVE_TYPESELE;

#endif /* _KIN_GLOBAL_PARAMETER_H_  */

/*----------------------------------------- (C) COPYRIGHT 2011  ---END OF FILE----------------------------------*/

