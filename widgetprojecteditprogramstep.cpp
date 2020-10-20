#include "widgetprojecteditprogramstep.h"

#pragma execution_character_set("utf-8")
WidgetProjectEditProgramStep::WidgetProjectEditProgramStep(QGraphicsScene *parent)
{
    step_pre_temperature = UNIT_TEMP_MIN;
    step_temperature = UNIT_TEMP_MIN;
    step_time = 0;
    step_fluoflag = false;


    parent_scene = parent;
    parent_scene_height = parent_scene->height();
   // qDebug()<<"parent_scene_height"<<parent_scene_height;
/*
    timeEditer = new QSpinBox();//���ٱ༭ʱ��Ŀռ�
    timeEditer->setButtonSymbols(QAbstractSpinBox::NoButtons);
    timeEditer->setRange(0, 1000);
    timeEditer->setSingleStep(1);
    timeEditer->setValue(10);
    timeEditer->setFixedSize(QSize(UNIT_WIDTH/2, 20));
    //���ٱ༭ʱ��Ŀռ�
    timeEditerProxy = parent_scene->addWidget(timeEditer);
    timeEditerProxy->setParentItem(this);
*/
    //���ٱ༭�¶ȵĿռ�
    temperatureEditer = new QDoubleSpinBox();
    temperatureEditer->setButtonSymbols(QAbstractSpinBox::NoButtons);
    temperatureEditer->setSingleStep(0.1);
    temperatureEditer->setDecimals(1);
    temperatureEditer->setValue(UNIT_TEMP_MIN);
    temperatureEditer->setFixedSize(QSize(UNIT_WIDTH/2, 20));
    temperatureEditer->setMaximum(UNIT_TEMP_MAX);
    temperatureEditer->setMinimum(UNIT_TEMP_MIN);
    tempEditerProxy = parent_scene->addWidget(temperatureEditer);
    tempEditerProxy->setParentItem(this);
    //---------------------------2020.09.01���ʱ��༭�ؼ�---------------------------------------------------
    timeEdit=new QTimeEdit();
    timeEdit->setDisplayFormat("mm:ss");
    timeEdit->setButtonSymbols(QTimeEdit::NoButtons);
    timeEdit->setFixedSize(UNIT_WIDTH/2,20);
    timeProxy=parent_scene->addWidget(timeEdit);
    connect(timeEdit,&QTimeEdit::timeChanged,this,&WidgetProjectEditProgramStep::SlotTimeEdit_TimeChanged);
    timeProxy->setParentItem(this);

    //------------------------------------------------------------------------------
    //������ʾ����ŵĿռ�
    step_index = new QLabel();
//    step_index->setFixedSize(QSize(UNIT_WIDTH, 20));
    step_index->setFixedSize(QSize(UNIT_WIDTH+1, 20));
    step_index->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    stepIndexProxy = parent_scene->addWidget(step_index);
    stepIndexProxy->setParentItem(this);
    stepIndexProxy->setPos(0,parent_scene_height-20);
    //���ӫ���ǰ�ť
    btn=new QPushButton();
    btn->setFixedSize(15,15);
    btn->setFlat(false);
    QPixmap  ico(":/images/Icon-light-Gray.png");
    btn->setIcon(ico);
    btn->setIconSize(QSize(15,15));
    btn->setStyleSheet("background:transparent");
    btnproxy=parent_scene->addWidget(btn);

    connect(btn,&QPushButton::clicked,this,&WidgetProjectEditProgramStep::SlotBtnclicked);
    btnproxy->setPos(UNIT_WIDTH*0.25, parent_scene_height-100);
    btnproxy->setParentItem(this);



    //�����¶ȱ༭����ʽ
    temperatureEditer->setStyleSheet(QLatin1String(
        "  QDoubleSpinBox{\n"
        "	   font: 9pt Arial ;\n"
        "		selection-background-color: rgba(255, 0, 0, 10);\n"
        "		selection-color: rgb(0, 0, 0);\n"
        "  }\n"
        "  QDoubleSpinBox::up-button {\n"
        "      subcontrol-origin: border;\n"
        "      subcontrol-position: top right;\n"
        "      width: 6px;\n"
        "      border-width: 0px;\n"
        "      subcontrol-origin: border;\n"
        "      subcontrol-position: bottom right;\n"
        "\n"
        "      width: 6px;\n"
        "      border-width: 1px;\n"
        "      border-top-width: 0;\n"
        "  }\n"
        "  QDoubleSpinBox::down-button {\n"
        "      subcontrol-origin: border;\n"
        "      subcontrol-position: bottom right;\n"
        "\n"
        "      width: 6px;\n"
        "      border-width: 1px;\n"
        "      border-top-width: 0;\n"
        "  }"));
    //����ʱ��༭����ʽ
 /*   timeEditer->setStyleSheet(QLatin1String(
        "  QTimeEdit{\n"
        "	   font: 8pt Arial ;\n"
        "		selection-background-color: rgba(255, 0, 0, 10);\n"
        "		selection-color: rgb(0, 0, 0);\n"
        "  }\n"
        "  QTimeEdit::up-button {\n"
        "      subcontrol-origin: border;\n"
        "      subcontrol-position: top right;\n"
        "      width: 6px;\n"
        "      border-width: 1px;\n"
        "  }\n"
        "  QTimeEdit::down-button {\n"
        "      subcontrol-origin: border;\n"
        "      subcontrol-position: bottom right; \n"
        "\n"
        "      width: 6px;\n"
        "      border-width: 1px;\n"
        "      border-top-width: 0;\n"
        "  }"));
*/
//    //���ñ�־
    setFlag(QGraphicsItem::ItemIsFocusable);
    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemIsSelectable);
    setAcceptedMouseButtons(Qt::LeftButton);

    parent_scene->addItem(this);

    //�����źźͲۺ���
    connect(temperatureEditer, SIGNAL(valueChanged(double)), this, SLOT(SlotEditTemp(double)));
 //   connect(timeEditer, SIGNAL(valueChanged(int)), this, SLOT(SlotEditTime(int)));

}

WidgetProjectEditProgramStep::~WidgetProjectEditProgramStep()
{
//    delete timeEditer;                //<ʱ��༭����
//    timeEditer=nullptr;
    delete temperatureEditer;         //<�¶ȱ༭����
    temperatureEditer=nullptr;
    delete step_index;
    step_index=nullptr;
    delete timeEdit;
    timeEdit=nullptr;

//    delete radbtn;
//    radbtn=nullptr;
//    delete radgroup;
//    radgroup=nullptr;

    delete btn;
    btn=nullptr;


}
//���ó�����λ��
void WidgetProjectEditProgramStep::SetStepPos(int current_group_id,QList<int> index_list)
{
    step_index->setText(tr("���� %1").arg(current_step_id));

    int index = 0;
    for (int i=0; i<current_group_id-1; i++)
    {
        index = index + index_list.at(i);
    }

    qreal x = (index+current_step_id-1)*120;
    this->setPos(x,this->y());
    this->update();
}
//ˢ�»�ͼ
void WidgetProjectEditProgramStep::ReplotView()
{
    this->update();
}

void WidgetProjectEditProgramStep::SetStepTempTime(float temperature, int time)
{
//    timeEditer->setValue(time);
//    QTime timel(time);
    int m=time/60;
    int s=time-60*m;
//    qDebug()<<"mm"<<m<<"s"<<s;
    QTime timel=QTime(0,m,s,0);

    timeEdit->setTime(timel);
    temperatureEditer->setValue(static_cast<double>(temperature));
}

//void WidgetProjectEditProgramStep::PainDissCurve()
//{

//}
//�󶨻�ͼ�����С
QRectF WidgetProjectEditProgramStep::boundingRect() const
{
    return QRectF(0, 0, UNIT_WIDTH, parent_scene_height);
}
//���ƻ�������
void WidgetProjectEditProgramStep::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{

    step_index->setText(tr("���� %1").arg(current_step_id));

    float temp_DPI = 1-(step_temperature-20)/100;
    float pre_temp_DPI = 1-(step_pre_temperature-20)/100;

    painter->setBrush(QColor(255, 0, 0, 20));
    painter->drawRect(0, 0, UNIT_WIDTH, parent_scene_height-1);
    //����ѡ�б���
    if (isSelected())
    {
        painter->setBrush(QColor(100, 255, 100, 100));
        painter->drawRect(0, 0, UNIT_WIDTH,parent_scene_height-1);
    }
    //�������¶���
    painter->setPen(QPen(Qt::red, 2));
    painter->drawLine(UNIT_WIDTH/2, parent_scene_height*temp_DPI, UNIT_WIDTH, parent_scene_height*temp_DPI);
    //����ӫ����ʾλ��
  //  radbtnProxy->setPos(UNIT_WIDTH*0.75-radbtn->size().width()/2, parent_scene_height*temp_DPI-radbtn->size().height()/2);
    btnproxy->setPos(UNIT_WIDTH*0.75-btn->size().width()/2, parent_scene_height*temp_DPI-btn->size().height()/2);

    //���¶�����������
    painter->setPen(QPen(Qt::red, 1, Qt::DotLine));
    painter->drawLine(0, parent_scene_height*pre_temp_DPI, UNIT_WIDTH/2, parent_scene_height*temp_DPI);
    //��ӫ����
    if(step_fluoflag)
    {
        QPixmap  ico(":/images/Icon-light-Green.png");
        btn->setIcon(ico);
        btn->setIconSize(QSize(15,15));
    }
    else
    {
        QPixmap  ico(":/images/Icon-light-Gray.png");
        btn->setIcon(ico);
        btn->setIconSize(QSize(15,15));
    }

    //���²���Ԫ��λ��
    tempEditerProxy->setPos(UNIT_WIDTH*0.5, parent_scene_height*temp_DPI - 30);
  //  timeEditerProxy->setPos(UNIT_WIDTH*0.5, parent_scene_height*temp_DPI + 10);
    timeProxy->setPos(UNIT_WIDTH*0.5, parent_scene_height*temp_DPI + 10);

}
//��ȡ��ǰ������ĳ����轹��
void WidgetProjectEditProgramStep::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    setFocus();
  //  emit SignalStepSelect(current_step_id);
    emit SignalSetpID(current_step_id);
//    qDebug()<<"current_step_id"<<current_step_id;

    update();
}

void WidgetProjectEditProgramStep::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (!isSelected())
    {
//        qDebug()<<"δѡ��";
        return;
    }
    //��ȡ��һ��λ�ú͵�ǰλ�õ�ƫ��
     QPointF diffpos=event->lastPos() - event->pos();
     //����ƫ���Ӧ���¶ȱ仯��
//qDebug()<<"�仯�¶�Ϊ"<<diffpos.y()<<parent_scene_height<<diffpos.y()/parent_scene_height<<diffpos.y()/parent_scene_height*69;

    step_temperature=step_temperature+static_cast<float>(diffpos.y()/parent_scene_height*69);
  //  qDebug()<<static_cast<int>(step_temperature);
    if(static_cast<double>(step_temperature) >= UNIT_TEMP_MAX)
    {
        step_temperature=UNIT_TEMP_MAX;
    }
    if(static_cast<double>(step_temperature) <= UNIT_TEMP_MIN)
    {
        step_temperature=UNIT_TEMP_MIN;
    }
    temperatureEditer->setValue(static_cast<double>(step_temperature));
    update();


}
//�������¶ȷ����仯
void WidgetProjectEditProgramStep::SlotEditTemp(double tem)
{
    if(tem >UNIT_TEMP_MAX)
    {
       //  temperatureEditer->setValue(static_cast<double>(step_temperature));
        return;
    }
    if(tem < UNIT_TEMP_MIN)
    {
       // temperatureEditer->setValue(static_cast<double>(step_temperature));
        return;
    }
    step_temperature = static_cast<float>(tem);
    temperatureEditer->setValue(tem);
    update();
    emit SignalStepSelect(current_step_id);


}
//������ʱ�䷢���仯
void WidgetProjectEditProgramStep::SlotEditTime(int time)
{
    /*
    if(time >UNIT_TIME_MAX)
    {
        return;
    }
    if(time <UNIT_TIME_MIM)
    {
        return;
    }
    step_time = time;
    timeEditer->setValue(step_time);
    update();
    emit SignalStepSelect(current_step_id);
    */
}

void WidgetProjectEditProgramStep::SlotTimeEdit_TimeChanged(QTime time)
{
 //   qDebug()<<"time"<<time.hour()<<time.minute()<<time.second()<<time.msec();
    step_time=time.minute()*60+time.second();
  //  qDebug()<<"step_time"<<step_time<<time.minute()*60;
    update();
//    emit SignalStepSelect(current_step_id);
    if(step_time < UNIT_FLUOR_TIME)
    {
        if(step_fluoflag)
        {
            step_fluoflag=false;
        }
//        step_fluoflag=!step_fluoflag;
    }

}

void WidgetProjectEditProgramStep::SlotBtnclicked()
{
//    if(fluoflagcheck)
//    {
//        QPixmap  ico(":/images/Icon-light-Gray.png");
//        btn->setIcon(ico);
//        btn->setIconSize(QSize(15,15));
//        fluoflagcheck=!fluoflagcheck;
//    }
//    else
//    {
//        QPixmap  ico(":/images/Icon-light-Green.png");
//        btn->setIcon(ico);
//        btn->setIconSize(QSize(15,15));
//        fluoflagcheck=!fluoflagcheck;
//    }
    //�ж�ʱ��
    if(step_time < UNIT_FLUOR_TIME)
    {
        qDebug()<<"ʱ�䲻���޷����ӫ��";
        return ;
    }
    //�жϵ�ǰ�����������������Ƿ���ڲɼ�ӫ��(�����ź�)
    if(step_fluoflag)
    {
        step_fluoflag=false;
    }
    else
    {
        SignalCheckFluor(current_step_id);
    }



//    step_fluoflag=!step_fluoflag;





}
