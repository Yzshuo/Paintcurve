#include "widget.h"
#include "ui_widget.h"


#pragma execution_character_set("utf-8")
Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);


    ui->comboBox->setView(new QListView());




}

Widget::~Widget()
{
    delete ui;
}

void Widget::resizeEvent(QResizeEvent *event)
{


}
/*
void Widget::paintEvent(QPaintEvent *event)
{
    int count = 0;
    float row = 0;
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);   //���������
    painter.setRenderHint(QPainter::TextAntialiasing);
    int w = this->width();             //��ͼ������
    int h = this->height();            //��ͼ���߶�
    int min = h-20,max = 20;
    QPen pen(QColor(Qt::blue),1);
    painter.setPen(pen);
//-----------------------------���̶ȳ�-------------------------------
    QPainterPath path;
    float step = static_cast<float>((min-max)/100.0);
    for(row = min;row > max-1;row -= step)
    {
        if(count % 10 == 0)
        {
            path.moveTo(10,static_cast<qreal>(row));
            path.lineTo(30,static_cast<qreal>(row));
            painter.drawText(32,static_cast<int>(row+5),QString("%1").arg(count));
        }
        else if(count % 5 == 0){
            path.moveTo(10,static_cast<qreal>(row));
            path.lineTo(25,static_cast<qreal>(row));
        }
        else {
            path.moveTo(10,static_cast<qreal>(row));
            path.lineTo(20,static_cast<qreal>(row));
        }
        count++;
    }
    path.moveTo(10,min);
    path.lineTo(10,max);
    painter.drawPath(path);
    //-----------------------------������-------------------------------
    QPen penl(QColor(Qt::red),1);
    painter.setPen(penl);

    float scale = step;
    float value = 35;
    QPainterPath pathLine;

     float maxl=1.0;
     pathLine.moveTo(100+1,static_cast<qreal>(-scale+min));
     for(int i=0;i<curve_X;i++)
     {
         float scale = i*step;
         float value = i;
         if(value > maxl)
             maxl=value;
         float rightPos = (value*(w-300)/maxl);
         QPointF p(static_cast<qreal>(100+i),static_cast<qreal>(-scale+min));
         pathLine.lineTo(p);
        // qDebug()<<"pathLine";
        // painter.drawText(static_cast<int>(100+rightPos+5),static_cast<int>(-scale+min),QString("%1").arg(static_cast<double>(value)));
     }
    painter.drawPath(pathLine);

}

void Widget::on_pushButton_clicked()
{
    curve_X=curve_X+10;
    this->update();
}
*/
//���������
void Widget::on_Btn_insert_group_clicked()
{
    //�жϵ�ǰ����������Ƿ񳬹����ֵ
//    if(ui->widget->Current_View_Group.size() >= (GROUP_MAX - 1))
//    {
//        qDebug()<<"�����鳬�����ֵ��������"<<ui->widget->Current_View_Group.size();
//    }
//    else
//    {
////        qDebug()<<"������δ�������ֵ������"<<ui->widget->Current_View_Group.size();

//    }
    ui->widget->SlotInsertGroup();


}
//��ȡ�ܳ���������
QList<int> Widget::GetGroupCount()
{
    QList<int> count_list;

    return count_list;
}
//���������
void Widget::on_Btn_insert_step_clicked()
{

    ui->widget->SlotInsertStep();
}
//ɾ��������
void Widget::on_Btn_step_delete_clicked()
{
    ui->widget->SlotDeleteStep();
}
//ɾ�������
void Widget::on_Btn_group_delete_clicked()
{
    ui->widget->SlotDeleteParogSeg();
}

void Widget::on_Btn_dissol_clicked()
{
    DISSCURVE_TYPESELE mdiss_type;
    mdiss_type.diss_sele=1;
    mdiss_type.start_temp=85.6;
    mdiss_type.end_temp=36.5;
    mdiss_type.temp_rate=1.5;
    mdiss_type.temp_grad=10;
    mdiss_type.sleep_time=6;
    ui->widget->InsertDissol(mdiss_type);
}

void Widget::on_Btn_save_clicked()
{
    QList<WidgetProjectEditProgramGroup*>  Current_View_Group;
    DISSCURVE_TYPESELE typesele;
    ui->widget->GetProgram(Current_View_Group,typesele);
    FileLocalSave::ProgramGroupSave("���Ա���", Current_View_Group,typesele);
}

void Widget::on_Btn_open_clicked()
{
    ui->widget->Func_UIinit();
    ui->widget->ClearScene();
    ui->widget->SetEditProgram(QDir::currentPath()+"/Project/���Ա���.pro");
}
/*
void Widget::on_pushButton_clicked()
{

    if(check)
    {
//        QPixmap  ico(":/images/Icon-Execute.png");
//        ui->pushButton->setIcon(ico);
//        ui->pushButton->setIconSize(QSize(30,30));
        qDebug()<<"check"<<check;
        check=!check;

    }
    else
    {
//        QPixmap  ico(":/images/Icon-Pause.png");
//        ui->pushButton->setIcon(ico);
//        ui->pushButton->setIconSize(QSize(30,30));
        qDebug()<<"check"<<check;
        check=!check;
    }

}
*/



void Widget::on_comboBox_currentIndexChanged(int index)
{
    switch (index)
    {
    case 0:
        qDebug()<<ui->comboBox->currentText()<<"0";
        break;
    case 1:
        qDebug()<<ui->comboBox->currentText()<<"1";
        //��ѡ����ǰ�����SlotInsertGroup_Front
        ui->widget->SlotInsertGroup_Front();
        break;
    case 2:
        qDebug()<<ui->comboBox->currentText()<<"2";
        //��ѡ����������
        ui->widget->SlotInsertGroup();
        break;
    case 3:
        //����������SlotInsertGroup_Last
        qDebug()<<ui->comboBox->currentText()<<"3";
        ui->widget->SlotInsertGroup_Last();
        break;
    default:
        break;


    }
    ui->comboBox->setCurrentIndex(0);
}