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
    int                                         current_group_id = 0;           //当前程序组编号
    int                                         group_cycle_num = 1;            //当前程序组循环次数
    QList<WidgetProjectEditProgramStep*>        Current_View_Step;              //当前程序组携带的程序步骤

    void    SetGroupPos(QList<int>);                //设置程序组位置
    void    InsertStep(int,QList<int>);             //在指定位置插入程序步骤

private:
    QGraphicsScene          *parent_scene;          //传入的父绘制界面
    QGraphicsLineItem		*m_line;                //线部件
    QSpinBox				*m_CycleEditer;         //循环次数编辑器
    QGraphicsProxyWidget	*m_CycleEditerProxy;    //循环次数编辑器代理
    QLabel                  *GroupIndex;            //当前程序组号显示
    QGraphicsProxyWidget    *GroupIndexProxy;       //当前程序组号显示代理

    QLineEdit               *GroupLend;
    QGraphicsProxyWidget    *GroupLendProxy;

signals:
    void SignalGroupSelect(int,int);                //获取当前选中的程序步骤
    void SignalGroupStep(int,int);                //鼠标当前选中的组号和程序步骤号
    void SignalCheckFluo(int,int);                //鼠标当前选中的组号和程序步骤号

private slots:
    void SlotEditCycle(int);                        //当前程序组的循环次数发生变化
    void SlotChangeStepSelect(int);                 //获取当前选中的程序步骤
    void SlotChangeStep(int step);                 //获取当前选中的程序步骤
    //采集荧光步骤
    void SLotCheckFluor(int step);

};

#endif // WIDGETPROJECTEDITPROGRAMGROUP_H
