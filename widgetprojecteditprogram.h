#ifndef WIDGETPROJECTEDITPROGRAM_H
#define WIDGETPROJECTEDITPROGRAM_H

#include <QWidget>
#include <QGraphicsScene>
#include <QMessageBox>
#include "widgetprojecteditprogramgroup.h"
#include "widgetprojecteditprogramstep.h"
#include "filelocalsave.h"
#include <QDebug>
#include<QLineEdit>

#define NONE_FLAG           0
#define ADD_GROUP_FLAG      1
#define ADD_STEP_FLAG       2

namespace Ui {
class WidgetProjectEditProgram;
}

class WidgetProjectEditProgram : public QWidget
{
    Q_OBJECT

public:
    explicit WidgetProjectEditProgram(QWidget *parent = nullptr);
    ~WidgetProjectEditProgram();

//    struct PCRPro
//    {
//        int                 MeltFlag;               //<�ܽ����߱�־λ
//        float               MeltStartTem;           //<�ܽ�����ʼ�¶�
//        float               MeltEndTem;             //<�ܽ�������ֹ
//        QList<WidgetProjectEditProgramGroup*>       Current_View_Group;     //�����ĳ���������
//    };
    QList<WidgetProjectEditProgramGroup*>       Current_View_Group;     //�����ĳ���������
    QGraphicsScene                              Current_Scene;          //��ǰ���ƽ���
    int                                         Select_Group_ID = 0;    //��ǰѡ�еĳ�����ID
    int                                         Select_Step_ID = 0;     //��ǰѡ�еĳ�����ID
public:
    void          Func_UIinit();               //��ʼ�����棬�󶨽���Rect
    void          SetEnableView(bool);         //ʹ����ʾ����
    void          SetEditProgram(QString);     //�򿪱��صĳ����ļ�
    void          SetGroupPosUpdate();         //���³���������ϵ���ʾλ��
    QList<int>    GetGroupCount();             //��ȡ������ͳ�������б�
    void          ClearScene();                //����������������
    void          ResetScene(void);            //����ȷ����ͼ�����С
    void          InsertDissol(DISSCURVE_TYPESELE mdiss_type);//�����ܽ�����
    void          ClearDissol(void);//����ܽ�����
    //��ȡ���������Լ�ӫ������
//    void GetProgram(QList<WidgetProjectEditProgramGroup*>  &Current_View_Group,
//                    int &MeltFlag,
//                    double &MeltStartTem,
//                    double &MeltEndTem);
    void GetProgram(QList<WidgetProjectEditProgramGroup*>  &Current_View_Group,DISSCURVE_TYPESELE &typesele);

public slots:
     void SlotInsertGroup_Front();                 //���������(ǰ��)
    void SlotInsertGroup();                 //���������(���)
    void SlotInsertGroup_Last();                 //���������(���)
    void SlotInsertStep();                  //���������
    void SlotDeleteStep();                  //ɾ��������
    void SlotDeleteParogSeg();              //ɾ�������
    void SlotSetFluoStep();                 //����ӫ��ɼ���־
    void SlotImportProgram();               //�������
    void SlotChangeGroupSelect(int,int);    //���µ�ǰѡ�еĳ�����
    void SlotChangeGroupStep(int group, int step);    //���µ�ǰѡ�еĳ���źͳ������

    void LineChanged_start(double vale);
    void LineChanged_end(double vale);

    void LineChanged_grad(double vale);//�仯�ݶ�
    void LineChanged_last(int vale);//ά��ʱ��
    void LineChanged_rate(double vale);//��������
    //�����������Ƿ���ӫ����
    void SLotCheckFluo(int group, int step);

private:
    Ui::WidgetProjectEditProgram *ui;

     QGraphicsRectItem	    *m_MeltPointer = nullptr;
     QGraphicsTextItem		*m_MeltText;                //��ʾ�۽��������ƿռ�
//     QGraphicsTextItem		*m_MeltStartTemText;        //
//     QGraphicsTextItem		*m_MeltEndTemText;          //
     QGraphicsLineItem      *m_Melt_Rect_line;
     QGraphicsLineItem		*m_Melt_line_1;             ///<
     QGraphicsLineItem		*m_Melt_line_2;             ///<
     QGraphicsLineItem		*m_Melt_line_3;             ///<
     QGraphicsLineItem		*m_Melt_line_4;             ///<

     //��ʼ��ֵֹ�༭�ؼ�
     QDoubleSpinBox  *start_temp;
     QDoubleSpinBox  *end_temp;
     QGraphicsProxyWidget *wid_start;
     QGraphicsProxyWidget *wid_end;
     //�ݶ��ܽ����߱༭�ؼ�
     QDoubleSpinBox  *grad_temp;//�ݶ��¶�
     QSpinBox  *last_temp;//ά��ʱ��
     QGraphicsProxyWidget *wid_grad;
     QGraphicsProxyWidget *wid_last;
     QGraphicsTextItem	*grad_text;
     QGraphicsTextItem	*last_text;
     //�����ܽ����߱༭�ؼ�
     QDoubleSpinBox  *rate_temp;//�¶�����
     QGraphicsProxyWidget *wid_rate;
     QGraphicsTextItem	*rate_text;

     //������ʾλ�÷ֱ���
     double TemDpistart ;//�ܽ�������ʼ�¶ȶ�Ӧ�ķֱ���
     double TemDpiend ;//�ܽ�������ֹ�¶ȶ�Ӧ�ķֱ���
     //��Ӧ���ܽ�����ƫ�ƾ���
     int    group_total;
     double group_endtemp;//�������������趨�¶�

     int                 MeltFlag;               //<�ܽ����߱�־λ(�Ƿ������ܽ�����)
     double              MeltStartTem;           //<�ܽ�����ʼ�¶�
     double              MeltEndTem;             //<�ܽ�������ֹ

     DISSCURVE_TYPESELE  disscurve_data;//�ܽ����߲���
private:
     //���ݴ���ĳ�����źͲ�������жϵ�һ�������Ƿ��л���ʼ����
     void Check_group_id(int group,int id);
     //�����ܽ�����λ��
     void DissCurvePointUpdate(void);



};

#endif // WIDGETPROJECTEDITPROGRAM_H