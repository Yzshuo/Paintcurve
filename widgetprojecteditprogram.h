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
//        int                 MeltFlag;               //<溶解曲线标志位
//        float               MeltStartTem;           //<溶解曲起始温度
//        float               MeltEndTem;             //<溶解曲线终止
//        QList<WidgetProjectEditProgramGroup*>       Current_View_Group;     //包含的程序组数量
//    };
    QList<WidgetProjectEditProgramGroup*>       Current_View_Group;     //包含的程序组数量
    QGraphicsScene                              Current_Scene;          //当前绘制界面
    int                                         Select_Group_ID = 0;    //当前选中的程序组ID
    int                                         Select_Step_ID = 0;     //当前选中的程序步骤ID
public:
    void          Func_UIinit();               //初始化界面，绑定界面Rect
    void          SetEnableView(bool);         //使能显示区域
    void          SetEditProgram(QString);     //打开本地的程序文件
    void          SetGroupPosUpdate();         //更新程序组界面上的显示位置
    QList<int>    GetGroupCount();             //获取程序组和程序步骤的列表
    void          ClearScene();                //清楚绘制区域的内容
    void          ResetScene(void);            //重新确定绘图区域大小
    void          InsertDissol(DISSCURVE_TYPESELE mdiss_type);//插入溶解曲线
    void          ClearDissol(void);//清除溶解曲线
    //获取程序中组以及荧光数据
//    void GetProgram(QList<WidgetProjectEditProgramGroup*>  &Current_View_Group,
//                    int &MeltFlag,
//                    double &MeltStartTem,
//                    double &MeltEndTem);
    void GetProgram(QList<WidgetProjectEditProgramGroup*>  &Current_View_Group,DISSCURVE_TYPESELE &typesele);

public slots:
     void SlotInsertGroup_Front();                 //插入程序组(前插)
    void SlotInsertGroup();                 //插入程序组(后插)
    void SlotInsertGroup_Last();                 //插入程序组(最后)
    void SlotInsertStep();                  //插入程序步骤
    void SlotDeleteStep();                  //删除程序步骤
    void SlotDeleteParogSeg();              //删除程序段
    void SlotSetFluoStep();                 //设置荧光采集标志
    void SlotImportProgram();               //导入程序
    void SlotChangeGroupSelect(int,int);    //更新当前选中的程序步骤
    void SlotChangeGroupStep(int group, int step);    //更新当前选中的程序号和程序步骤号

    void LineChanged_start(double vale);
    void LineChanged_end(double vale);

    void LineChanged_grad(double vale);//变化梯度
    void LineChanged_last(int vale);//维持时间
    void LineChanged_rate(double vale);//升温速率
    //检测程序组中是否有荧光检测
    void SLotCheckFluo(int group, int step);

private:
    Ui::WidgetProjectEditProgram *ui;

     QGraphicsRectItem	    *m_MeltPointer = nullptr;
     QGraphicsTextItem		*m_MeltText;                //显示熔解曲线名称空间
//     QGraphicsTextItem		*m_MeltStartTemText;        //
//     QGraphicsTextItem		*m_MeltEndTemText;          //
     QGraphicsLineItem      *m_Melt_Rect_line;
     QGraphicsLineItem		*m_Melt_line_1;             ///<
     QGraphicsLineItem		*m_Melt_line_2;             ///<
     QGraphicsLineItem		*m_Melt_line_3;             ///<
     QGraphicsLineItem		*m_Melt_line_4;             ///<

     //起始终止值编辑控件
     QDoubleSpinBox  *start_temp;
     QDoubleSpinBox  *end_temp;
     QGraphicsProxyWidget *wid_start;
     QGraphicsProxyWidget *wid_end;
     //梯度溶解曲线编辑控件
     QDoubleSpinBox  *grad_temp;//梯度温度
     QSpinBox  *last_temp;//维持时间
     QGraphicsProxyWidget *wid_grad;
     QGraphicsProxyWidget *wid_last;
     QGraphicsTextItem	*grad_text;
     QGraphicsTextItem	*last_text;
     //速率溶解曲线编辑控件
     QDoubleSpinBox  *rate_temp;//温度速率
     QGraphicsProxyWidget *wid_rate;
     QGraphicsTextItem	*rate_text;

     //绘制显示位置分辨率
     double TemDpistart ;//溶解曲线起始温度对应的分辨率
     double TemDpiend ;//溶解曲线终止温度对应的分辨率
     //对应的溶解曲线偏移距离
     int    group_total;
     double group_endtemp;//程序组最后步骤的设定温度

     int                 MeltFlag;               //<溶解曲线标志位(是否添加溶解曲线)
     double              MeltStartTem;           //<溶解曲起始温度
     double              MeltEndTem;             //<溶解曲线终止

     DISSCURVE_TYPESELE  disscurve_data;//溶解曲线参数
private:
     //根据传入的程序组号和步骤号来判断第一个虚线是否切换起始坐标
     void Check_group_id(int group,int id);
     //更新溶解曲线位置
     void DissCurvePointUpdate(void);



};

#endif // WIDGETPROJECTEDITPROGRAM_H
