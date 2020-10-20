#ifndef WIDGETPROJECTEDITPROGRAMGROUP_H
#define WIDGETPROJECTEDITPROGRAMGROUP_H

#include <QObject>
#include <QGraphicsScene>
#include <QGraphicsLineItem>
#include <QSpinBox>
#include <QGraphicsProxyWidget>
#include<QLineEdit>
#include "widgetprojecteditprogramstep.h"

class WidgetProjectEditProgramGroup : public QObject
{
    Q_OBJECT
public:
    explicit WidgetProjectEditProgramGroup(QGraphicsScene *parent);
    ~WidgetProjectEditProgramGroup();
    int                                         current_group_id = 0;           //��ǰ��������
    int                                         group_cycle_num = 1;            //��ǰ������ѭ������
    QList<WidgetProjectEditProgramStep*>        Current_View_Step;              //��ǰ������Я���ĳ�����

    void    SetGroupPos(QList<int>);                //���ó�����λ��
    void    InsertStep(int,QList<int>);             //��ָ��λ�ò��������

private:
    QGraphicsScene          *parent_scene;          //����ĸ����ƽ���
    QGraphicsLineItem		*m_line;                //�߲���
    QSpinBox				*m_CycleEditer;         //ѭ�������༭��
    QGraphicsProxyWidget	*m_CycleEditerProxy;    //ѭ�������༭������
    QLabel                  *GroupIndex;            //��ǰ���������ʾ
    QGraphicsProxyWidget    *GroupIndexProxy;       //��ǰ���������ʾ����

    QLineEdit               *GroupLend;
    QGraphicsProxyWidget    *GroupLendProxy;

signals:
    void SignalGroupSelect(int,int);                //��ȡ��ǰѡ�еĳ�����
    void SignalGroupStep(int,int);                //��굱ǰѡ�е���źͳ������
    void SignalCheckFluo(int,int);                //��굱ǰѡ�е���źͳ������

private slots:
    void SlotEditCycle(int);                        //��ǰ�������ѭ�����������仯
    void SlotChangeStepSelect(int);                 //��ȡ��ǰѡ�еĳ�����
    void SlotChangeStep(int step);                 //��ȡ��ǰѡ�еĳ�����
    //�ɼ�ӫ�ⲽ��
    void SLotCheckFluor(int step);

};

#endif // WIDGETPROJECTEDITPROGRAMGROUP_H
