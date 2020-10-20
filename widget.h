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

//   QList<WidgetProgramGroup*>   Current_View_Group;     //�����ĳ���������
   QGraphicsScene               Current_Scene;          //��ǰ���ƽ���
   int                          Select_Group_ID = 0;    //��ǰѡ�еĳ�����ID
   int                          Select_Step_ID = 0;     //��ǰѡ�еĳ�����ID
private:
   QList<int>    GetGroupCount();             //��ȡ������ͳ�������б�
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
