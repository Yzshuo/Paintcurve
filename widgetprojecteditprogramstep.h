#ifndef WIDGETPROJECTEDITPROGRAMSTEP_H
#define WIDGETPROJECTEDITPROGRAMSTEP_H

#define     UNIT_WIDTH     120
#define     UNIT_TEMP_MAX     99.0
#define     UNIT_TEMP_MIN     37.0
#define     UNIT_TIME_MAX     1000
#define     UNIT_TIME_MIM     10
#define     UNIT_LOOP_NUM_MAX    99 //ѭ��������
#define     UNIT_LOOP_NUM_MIN    1  //ѭ����С����
#define     UNIT_FLUOR_TIME     8   //���ӫ��ʱ������С����ʱ��


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
    int                     current_step_id = 0;        //��ǰ������ID
    float                   step_pre_temperature;       //��ǰ�������ǰ���¶�
    float                   step_temperature;           //��ǰ�������¶�
    int                     step_time;                  //��ǰ���������ʱ��
    bool                    step_fluoflag;              //��ǰ������ӫ��ɼ���־λ

    void SetStepPos(int,QList<int>);                    //���õ�ǰ�������ڽ����ϵ�λ��
    void ReplotView();                                  //���³�������ʾ
    void SetStepTempTime(float temperature,int time);
    //���ܽ����߲���
//    void PainDissCurve(void);

private:
    QGraphicsScene          *parent_scene;              //����ĸ����ƽ���
    qreal                   parent_scene_height;        //����ĸ����ƽ���߶�
//    QSpinBox                *timeEditer;                //ʱ��༭����
//    QGraphicsProxyWidget	*timeEditerProxy;           //ʱ��༭�ؼ�����
    QDoubleSpinBox 			*temperatureEditer;         //�¶ȱ༭����
    QGraphicsProxyWidget	*tempEditerProxy;           //�¶ȱ༭�ؼ�����
    QLabel                  *step_index;                //��ǰ���������ʾ
    QGraphicsProxyWidget    *stepIndexProxy;            //��ǰ���������ʾ����

    //202020.09.01���ʱ��༭�ؼ���׼���滻ԭ�ȵ�ʱ��༭�ؼ�
    QTimeEdit               *timeEdit;
    QGraphicsProxyWidget	*timeProxy;


     //2020.9.1���ӫ�ⰴť��־
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
    void SignalStepSelect(int);                         //���µ�ǰѡ�еĳ�����ID
    void SignalSetpID(int step);
    void SignalCheckFluor(int step);//���͵�ǰ��Ҫ�ɼ�ӫ�ⲽ���

private slots:
    void SlotEditTemp(double);                          //��ǰ��������¶ȷ����仯
    void SlotEditTime(int);                             //��ǰ�������ʱ�䷢���仯

    void  SlotTimeEdit_TimeChanged(QTime time);
    void SlotBtnclicked(void);
};

#endif // WIDGETPROJECTEDITPROGRAMSTEP_H
