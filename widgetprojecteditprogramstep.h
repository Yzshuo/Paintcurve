#ifndef WIDGETPROJECTEDITPROGRAMSTEP_H
#define WIDGETPROJECTEDITPROGRAMSTEP_H

#define     UNIT_WIDTH     120
#define     UNIT_TEMP_MAX     99.0
#define     UNIT_TEMP_MIN     37.0
#define     UNIT_TIME_MAX     1000
#define     UNIT_TIME_MIM     10
#define     UNIT_LOOP_NUM_MAX    99 //循环最大次数
#define     UNIT_LOOP_NUM_MIN    1  //循环最小次数
#define     UNIT_FLUOR_TIME     8   //检测荧光时所需最小保持时间


#include <QObject>
#include <QGraphicsItem>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QGraphicsScene>
#include <QGraphicsProxyWidget>
#include <QPainter>
#include <QLabel>
#include<QDebug>
#include<QGraphicsSceneMouseEvent>
#include<QRadioButton>
#include<QButtonGroup>
#include<QTimeEdit>
#include<QTime>
#include<QPushButton>

class WidgetProjectEditProgramStep : public QObject,public QGraphicsItem
{
    Q_OBJECT
    Q_INTERFACES(QGraphicsItem)
public:
    WidgetProjectEditProgramStep(QGraphicsScene *parent);
    ~WidgetProjectEditProgramStep();
    int                     current_step_id = 0;        //当前程序步骤ID
    float                   step_pre_temperature;       //当前程序步骤的前置温度
    float                   step_temperature;           //当前程序步骤温度
    int                     step_time;                  //当前程序步骤持续时间
    bool                    step_fluoflag;              //当前程序步骤荧光采集标志位

    void SetStepPos(int,QList<int>);                    //设置当前程序步骤在界面上的位置
    void ReplotView();                                  //更新程序步骤显示
    void SetStepTempTime(float temperature,int time);
    //画溶解曲线部分
//    void PainDissCurve(void);

private:
    QGraphicsScene          *parent_scene;              //传入的父绘制界面
    qreal                   parent_scene_height;        //传入的父绘制界面高度
//    QSpinBox                *timeEditer;                //时间编辑部件
//    QGraphicsProxyWidget	*timeEditerProxy;           //时间编辑控件代理
    QDoubleSpinBox 			*temperatureEditer;         //温度编辑部件
    QGraphicsProxyWidget	*tempEditerProxy;           //温度编辑控件代理
    QLabel                  *step_index;                //当前程序步骤号显示
    QGraphicsProxyWidget    *stepIndexProxy;            //当前程序步骤号显示代理

    //202020.09.01添加时间编辑控件，准备替换原先的时间编辑控件
    QTimeEdit               *timeEdit;
    QGraphicsProxyWidget	*timeProxy;


     //2020.9.1添加荧光按钮标志
//     QRadioButton           *radbtn;
//     QGraphicsProxyWidget	*radbtnProxy;
//     QButtonGroup           *radgroup;

     QPushButton            *btn;
     QGraphicsProxyWidget   *btnproxy;


     bool fluoflagcheck=false;

protected:
    QRectF	boundingRect()const;
    void	paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void    mousePressEvent(QGraphicsSceneMouseEvent *event);
    void    mouseMoveEvent(QGraphicsSceneMouseEvent *event);

signals:
    void SignalStepSelect(int);                         //更新当前选中的程序步骤ID
    void SignalSetpID(int step);
    void SignalCheckFluor(int step);//发送当前需要采集荧光步骤号

private slots:
    void SlotEditTemp(double);                          //当前程序步骤的温度发生变化
    void SlotEditTime(int);                             //当前程序步骤的时间发生变化

    void  SlotTimeEdit_TimeChanged(QTime time);
    void SlotBtnclicked(void);
};

#endif // WIDGETPROJECTEDITPROGRAMSTEP_H
