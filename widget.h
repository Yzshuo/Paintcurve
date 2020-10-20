#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include<QPainter>
#include<QPen>
#include<QDebug>
#include <QGraphicsScene>
#include<QListView>

#define GROUP_MAX 9
#define STEP_MAX  9

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();

protected:
    void resizeEvent(QResizeEvent *event);
//     virtual void paintEvent(QPaintEvent *event) ;
//private slots:
//    void on_pushButton_clicked();

//   QList<WidgetProgramGroup*>   Current_View_Group;     //包含的程序组数量
   QGraphicsScene               Current_Scene;          //当前绘制界面
   int                          Select_Group_ID = 0;    //当前选中的程序组ID
   int                          Select_Step_ID = 0;     //当前选中的程序步骤ID
private:
   QList<int>    GetGroupCount();             //获取程序组和程序步骤的列表
private slots:


      void on_Btn_insert_group_clicked();

      void on_Btn_insert_step_clicked();

      void on_Btn_step_delete_clicked();

      void on_Btn_group_delete_clicked();

      void on_Btn_dissol_clicked();

      void on_Btn_save_clicked();

      void on_Btn_open_clicked();

//      void on_pushButton_clicked();



      void on_comboBox_currentIndexChanged(int index);

private:
    Ui::Widget *ui;
    int curve_X;
    bool check=false;
};

#endif // WIDGET_H
