#include "widgetprojecteditprogramgroup.h"


#pragma execution_character_set("utf-8")
WidgetProjectEditProgramGroup::WidgetProjectEditProgramGroup(QGraphicsScene *parent)
{
    parent_scene = parent;
    m_line = new QGraphicsLineItem(0,-2,0, parent_scene->sceneRect().height()+2);
    m_line->setPen(QPen(Qt::red, 2));
    parent_scene->addItem(m_line);

    m_CycleEditer = new QSpinBox();
//    m_CycleEditer->setPrefix(tr("Cycle Times:"));
    m_CycleEditer->setPrefix(tr("循环次数:"));
    m_CycleEditer->setFixedSize(QSize(UNIT_WIDTH, 20));
    m_CycleEditer->setRange(UNIT_LOOP_NUM_MIN, UNIT_LOOP_NUM_MAX);
    m_CycleEditer->setSingleStep(1);

    m_CycleEditerProxy = parent_scene->addWidget(m_CycleEditer);
    m_CycleEditerProxy->setZValue(2);
    m_CycleEditerProxy->setPos(0, 20);
    m_CycleEditerProxy->setAcceptedMouseButtons(Qt::LeftButton);
    connect(m_CycleEditer, SIGNAL(valueChanged(int)), this, SLOT(SlotEditCycle(int)));

//    GroupIndex = new QLabel();
//    GroupIndex->setFixedSize(QSize(UNIT_WIDTH, 20));
//    GroupIndex->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
//    GroupIndexProxy = parent_scene->addWidget(GroupIndex);
//    GroupIndexProxy->setPos(0,0);

    WidgetProjectEditProgramStep *step_node = new WidgetProjectEditProgramStep(parent_scene);
   // step_node->addstep(parent_scene);
    connect(step_node,SIGNAL(SignalStepSelect(int)),this,SLOT(SlotChangeStepSelect(int)));
    connect(step_node,SIGNAL(SignalSetpID(int)),this,SLOT(SlotChangeStep(int)));
    //2020.10.19添加
    connect(step_node,SIGNAL(SignalCheckFluor(int)),this,SLOT(SLotCheckFluor(int)));
    step_node->current_step_id = 1;
    Current_View_Step.append(step_node);
   // qDebug()<<"parent_scene->sceneRect().height()"<<parent_scene->sceneRect().height();


    GroupLend=new QLineEdit();
    GroupLend->setFixedSize(QSize(UNIT_WIDTH, 20));
    GroupLend->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    GroupLend->setText("程序组1");
    GroupLend->setStyleSheet("background:transparent;border-width:0;border-style:outset");
    GroupLendProxy = parent_scene->addWidget(GroupLend);
    GroupLendProxy->setPos(0,0);

}

WidgetProjectEditProgramGroup::~WidgetProjectEditProgramGroup()
{
    deleteLater();
    delete m_line;                            //<线部件
    delete m_CycleEditer;                     //<循环编辑器
//    delete GroupIndex;
    delete GroupLend;
}
//设置程序组位置
void WidgetProjectEditProgramGroup::SetGroupPos(QList<int> index_list)
{
//    GroupIndex->setText(tr("程序组 %1").arg(current_group_id));
    GroupLend->setText(tr("程序组 %1").arg(current_group_id));
    m_CycleEditer->setValue(group_cycle_num);

    int index = 0;
    for (int i=0; i<current_group_id-1; i++)
    {
        index = index + index_list.at(i);
    }

    qreal x = (index)*120;
    m_line->setPos(x,m_line->pos().y());
    m_CycleEditerProxy->setPos(x,m_CycleEditerProxy->y());
//    GroupIndexProxy->setPos(x,GroupIndexProxy->y());
    GroupLendProxy->setPos(x,GroupLendProxy->y());
    for(auto step_tmp:Current_View_Step)
    {
        step_tmp->SetStepPos(current_group_id,index_list);
        step_tmp->SetStepTempTime(step_tmp->step_temperature,step_tmp->step_time);
    }
}
//插入程序步骤
void WidgetProjectEditProgramGroup::InsertStep(int select_step_ID, QList<int> index_list)
{
  //  qDebug()<<"select_step_ID"<<select_step_ID<<index_list.size()<<Current_View_Step.size();
    WidgetProjectEditProgramStep *step_node = new WidgetProjectEditProgramStep(parent_scene);
    connect(step_node,SIGNAL(SignalStepSelect(int)),this,SLOT(SlotChangeStepSelect(int)));
    connect(step_node,SIGNAL(SignalSetpID(int)),this,SLOT(SlotChangeStep(int)));
    //2020.10.19添加
    connect(step_node,SIGNAL(SignalCheckFluor(int)),this,SLOT(SLotCheckFluor(int)));

    for (auto tmp_step:Current_View_Step)
    {
        if(tmp_step->current_step_id > select_step_ID)
        {
            tmp_step->current_step_id = tmp_step->current_step_id + 1;
            tmp_step->SetStepPos(current_group_id,index_list);
        }
    }

    step_node->current_step_id = select_step_ID + 1;

    Current_View_Step.insert(select_step_ID,step_node);
    step_node->SetStepPos(current_group_id,index_list);

}
//程序组循环次数发生变化
void WidgetProjectEditProgramGroup::SlotEditCycle(int cycle_num)
{
    group_cycle_num = cycle_num;
}
//获取当前选中的程序组ID
void WidgetProjectEditProgramGroup::SlotChangeStepSelect(int step)
{
    emit SignalGroupSelect(current_group_id,step);
//    emit SignalGroupStep(current_group_id,step);
  //  qDebug()<<"SlotChangeStepSelect";
}

void WidgetProjectEditProgramGroup::SlotChangeStep(int step)
{
    emit SignalGroupStep(current_group_id,step);
    //  qDebug()<<"SlotChangeStep";
}

void WidgetProjectEditProgramGroup::SLotCheckFluor(int step)
{
    qDebug()<<"step"<<step<<current_group_id;
    SignalCheckFluo(current_group_id,step);
}
