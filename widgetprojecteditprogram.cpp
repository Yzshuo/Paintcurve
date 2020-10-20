#include "widgetprojecteditprogram.h"
#include "ui_widgetprojecteditprogram.h"



#pragma execution_character_set("utf-8")
WidgetProjectEditProgram::WidgetProjectEditProgram(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WidgetProjectEditProgram)
{
    ui->setupUi(this);
    Func_UIinit();

    Current_View_Group.clear();

    m_MeltPointer = nullptr;
    m_MeltText= nullptr;

    m_Melt_Rect_line= nullptr;
    m_Melt_line_1= nullptr;
    m_Melt_line_2= nullptr;
    m_Melt_line_3= nullptr;
    m_Melt_line_4= nullptr;

    start_temp= nullptr;
    end_temp= nullptr;
    wid_start= nullptr;
    wid_end= nullptr;

    grad_temp=nullptr;
    last_temp= nullptr;
    wid_grad= nullptr;
    wid_last= nullptr;
    grad_text= nullptr;
    last_text= nullptr;
    rate_temp= nullptr;
    wid_rate= nullptr;
    rate_text= nullptr;


    TemDpistart=0.0;
    TemDpiend=0.0;
    group_total=0;
    group_endtemp=0.0;
    disscurve_data.disscurve_check=false;
    disscurve_data.diss_sele=0;
    disscurve_data.start_temp=0.0;
    disscurve_data.end_temp=0.0;
    disscurve_data.temp_rate=0.0;
    disscurve_data.temp_grad=0.0;
    disscurve_data.sleep_time=0;
}

WidgetProjectEditProgram::~WidgetProjectEditProgram()
{
    if(m_MeltPointer != nullptr)
    {
        delete m_MeltPointer;
        delete m_Melt_Rect_line;
        delete m_Melt_line_1;
        delete m_Melt_line_2;
        delete m_Melt_line_3;
        delete m_Melt_line_4;
        delete m_MeltText;
//        delete m_MeltStartTemText;
//        delete m_MeltEndTemText;
        delete start_temp;
        delete end_temp;
//        delete wid_start;
//        delete wid_end;

        m_MeltPointer=nullptr;
        m_Melt_Rect_line = nullptr;
        m_Melt_line_1 = nullptr;
        m_Melt_line_2 = nullptr;
        m_Melt_line_3 = nullptr;
        m_Melt_line_4 = nullptr;
        m_MeltText = nullptr;
//        m_MeltStartTemText = nullptr;
//        m_MeltEndTemText = nullptr;
        start_temp=nullptr;
        end_temp=nullptr;
//        wid_start=nullptr;
//        wid_end=nullptr;
    }

    delete ui;
}
//初始化界面，绑定画布大小
void WidgetProjectEditProgram::Func_UIinit()
{
    Current_Scene.setSceneRect(ui->graphicsView->rect());
    ui->graphicsView->setScene(&Current_Scene);
}
//禁用画布。仅做显示作用
void WidgetProjectEditProgram::SetEnableView(bool flag)
{
    ui->graphicsView->setEnabled(flag);
}
//设置画布内容
void WidgetProjectEditProgram::SetEditProgram(QString fileaddr)
{
//    Current_View_Group = FileLocalSave::ProgramGroupLoad(fileaddr, &Current_Scene);
//    for (auto tmp_node:Current_View_Group) {
//        connect(tmp_node,SIGNAL(SignalGroupSelect(int,int)),this,SLOT(SlotChangeGroupSelect(int, int)));
//    }
    //测试
//    int                 MeltFlag=0;               //<溶解曲线标志位
//    double              MeltStartTem=0;           //<溶解曲起始温度
//    double              MeltEndTem=0;             //<溶解曲线终止
    DISSCURVE_TYPESELE mdiss_type;
//    Current_View_Group = FileLocalSave::ProgramGroupLoad(fileaddr, &Current_Scene,MeltFlag,MeltStartTem,MeltEndTem);
    Current_View_Group = FileLocalSave::ProgramGroupLoad(fileaddr, &Current_Scene,mdiss_type);
    for (auto tmp_node:Current_View_Group)
    {
        connect(tmp_node,SIGNAL(SignalGroupSelect(int,int)),this,SLOT(SlotChangeGroupSelect(int, int)));
        connect(tmp_node,SIGNAL(SignalGroupStep(int,int)),this,SLOT(SlotChangeGroupStep(int, int)));
        //2020.10.19添加检测当前程序组是否有荧光检测
        connect(tmp_node,SIGNAL(SignalCheckFluo(int,int)),this,SLOT(SLotCheckFluo(int,int)));

        for (auto temp_step:tmp_node->Current_View_Step)
        {
            connect(temp_step,SIGNAL(SignalSetpID(int)),tmp_node,SLOT(SlotChangeStep(int)));
        }
    }


    SetGroupPosUpdate();
//    qDebug()<<"MeltFlag="<<MeltFlag<<"MeltStartTem="<<MeltStartTem<<"MeltEndTem="<<MeltEndTem;

//    mdiss_type.diss_sele=MeltFlag;
//    mdiss_type.start_temp=MeltStartTem;
//    mdiss_type.end_temp=MeltEndTem;
    if(mdiss_type.disscurve_check)
    {
        InsertDissol(mdiss_type);
    }
}
//更新程序组位置
void WidgetProjectEditProgram::SetGroupPosUpdate()
{
    auto index_list = GetGroupCount();
    for (auto tmp_group:Current_View_Group)
    {
        tmp_group->SetGroupPos(index_list);
    }
}
//获取总程序组数量
QList<int> WidgetProjectEditProgram::GetGroupCount()
{
    QList<int> count_list;
    for (auto tmp_group:Current_View_Group)
    {
        count_list.append(tmp_group->Current_View_Step.count());
    }
    return count_list;
}
//清除画布内容
void WidgetProjectEditProgram::ClearScene()
{
    for(auto node_group:Current_View_Group)
    {
        for(auto node_step:node_group->Current_View_Step)
        {
            delete node_step;
            node_group->Current_View_Step.removeOne(node_step);
        }
        delete node_group;
        Current_View_Group.removeOne(node_group);
    }
    ClearDissol();
}

void WidgetProjectEditProgram::ResetScene()
{
    //重新确定图像绘制的区域
   Current_Scene.setSceneRect(ui->graphicsView->rect().x(),ui->graphicsView->rect().y(),ui->graphicsView->rect().width()-2,ui->graphicsView->rect().height()-2);

}
//插入溶解曲线
void WidgetProjectEditProgram::InsertDissol(DISSCURVE_TYPESELE mdiss_type)
{

    if(m_MeltPointer == nullptr)
    {
/*
       auto temp= Current_View_Group[Current_View_Group.size()-1]->Current_View_Step[Current_View_Group[Current_View_Group.size()-1]->Current_View_Step.size()-1]->step_temperature;
     //  qDebug()<<"追后一步设定温度"<<temp;
       auto TemDpistart =1.0- (30.0)/100;
       auto TemDpiend =1.0- (70.0)/100;
   //     qDebug()<<"分辨率"<<TemDpistart<<TemDpiend;
//        m_MeltPointer = Current_Scene.addRect(group_total*120+120, -Current_Scene.height(), 240, Current_Scene.height());//创建一个矩形并添加到场景中
        m_MeltPointer = Current_Scene.addRect(group_total*120, 0, 240, Current_Scene.height());//创建一个矩形并添加到场景中

        m_MeltPointer->setBrush(QColor(255, 255, 255));

        m_Melt_Rect_line = Current_Scene.addLine(group_total*120,-2,group_total*120,Current_Scene.height()+2,QPen(Qt::black, 2));

        m_Melt_line_1 = Current_Scene.addLine(group_total*120, (int)((1.0-(temp-20.0)/100)*heit), group_total*120+60, (int)((TemDpistart)*heit),QPen(Qt::red, 1, Qt::DotLine));
        m_Melt_line_2 = Current_Scene.addLine(group_total*120+60, (int)((TemDpistart)*heit), group_total*120+120, (int)((TemDpistart)*heit),QPen(Qt::red, 2));
        m_Melt_line_3 = Current_Scene.addLine(group_total*120+120, (int)((TemDpistart)*heit), group_total*120+180, (int)((TemDpiend)*heit),QPen(Qt::red, 1, Qt::DotLine));
        m_Melt_line_4 = Current_Scene.addLine(group_total*120+180, (int)((TemDpiend)*heit), group_total*120+240, (int)((TemDpiend)*heit),QPen(Qt::red, 2));

        m_MeltText = Current_Scene.addText(tr("溶解曲线"));
        m_MeltText->setPos(group_total*120+10,20);
        m_MeltText->setDefaultTextColor(QColor(Qt::red));

        m_MeltStartTemText = Current_Scene.addText(tr("%1").arg(50));
        m_MeltStartTemText->setPos(group_total*120+82,(int)((TemDpistart)*heit)-20);
        m_MeltStartTemText->setDefaultTextColor(QColor(Qt::black));

        m_MeltEndTemText = Current_Scene.addText(tr("%1").arg(90));
        m_MeltEndTemText->setPos(group_total*120+202,(int)((TemDpiend)*heit)-20);
        m_MeltEndTemText->setDefaultTextColor(QColor(Qt::black));
*/

        QList<int> group_size;
        group_total=0;
        group_size.clear();
        group_size= GetGroupCount();
        if(group_size.size() == 0)
        {
            qDebug()<<"程序组未添加！";
            //添加程序组
            SlotInsertGroup();
            group_total=1;
        }
        else
        {
            for (auto lin_size:group_size)
            {
                group_total=group_total+lin_size;
            }
        }

    //   qDebug()<<"InsertDissol group_total="<<group_total<<"group_total*120="<<group_total*120;
       auto heit= Current_Scene.height();

        group_endtemp= static_cast<double>(Current_View_Group[Current_View_Group.size()-1]->Current_View_Step[Current_View_Group[Current_View_Group.size()-1]->Current_View_Step.size()-1]->step_temperature);
      //  qDebug()<<"最后一步设定的温度"<<temp;
        TemDpistart =1.0- (mdiss_type.start_temp-20)/100;
        TemDpiend =1.0- (mdiss_type.end_temp-20)/100;

         m_MeltPointer = Current_Scene.addRect(group_total*120, 0, 240, Current_Scene.height());//创建一个矩形并添加到场景中

         m_MeltPointer->setBrush(QColor(255, 255, 255));

         m_Melt_Rect_line = Current_Scene.addLine(group_total*120,-2,group_total*120,Current_Scene.height()+2,QPen(Qt::black, 2));

         //绘制溶解曲线线条
         m_Melt_line_1 = Current_Scene.addLine(group_total*120,(1.0-(group_endtemp-20.0)/100)*heit, group_total*120+60, TemDpistart*heit,QPen(Qt::red, 1, Qt::DotLine));
         m_Melt_line_2 = Current_Scene.addLine(group_total*120+60, TemDpistart*heit, group_total*120+120, TemDpistart*heit,QPen(Qt::red, 2));
         m_Melt_line_3 = Current_Scene.addLine(group_total*120+120, TemDpistart*heit, group_total*120+180,TemDpiend*heit,QPen(Qt::red, 1, Qt::DotLine));
         m_Melt_line_4 = Current_Scene.addLine(group_total*120+180, TemDpiend*heit, group_total*120+240, TemDpiend*heit,QPen(Qt::red, 2));

         //绘制显示标题控件
         m_MeltText = Current_Scene.addText(tr("溶解曲线"));
         m_MeltText->setPos(group_total*120+95,10);
         m_MeltText->setDefaultTextColor(QColor(Qt::red));
         //判断当前是速率溶解还是梯度溶解
         if(1 == mdiss_type.diss_sele)
         {
             //速率溶解
             rate_temp=new QDoubleSpinBox();
             rate_temp->setValue(mdiss_type.temp_rate);
             rate_temp->setFixedSize(QSize(UNIT_WIDTH/2, 20));
             rate_temp->setSingleStep(0.1);
             connect(rate_temp, SIGNAL(valueChanged(double)), this, SLOT(LineChanged_rate(double)));
             wid_rate=Current_Scene.addWidget(rate_temp);
             wid_rate->setPos(group_total*120+60,40);

             rate_text = Current_Scene.addText(tr("速率溶解"));
             rate_text->setPos(group_total*120+5,40);
             rate_text->setTextWidth(80);
             rate_text->setDefaultTextColor(QColor(Qt::red));

         }
         else if(2 == mdiss_type.diss_sele)
         {
             //梯度溶解
             grad_text = Current_Scene.addText(tr("升温梯度"));
             grad_text->setPos(group_total*120+5,40);
             grad_text->setTextWidth(80);
             grad_text->setDefaultTextColor(QColor(Qt::red));

             grad_temp=new QDoubleSpinBox();
             grad_temp->setValue(mdiss_type.temp_grad);
             grad_temp->setFixedSize(QSize(UNIT_WIDTH/2, 20));
             grad_temp->setSingleStep(0.1);
             connect(grad_temp, SIGNAL(valueChanged(double)), this, SLOT(LineChanged_grad(double)));
             wid_grad=Current_Scene.addWidget(grad_temp);
             wid_grad->setPos(group_total*120+60,40);


             last_text = Current_Scene.addText(tr("维持时间"));
             last_text->setPos(group_total*120+125,40);
             last_text->setTextWidth(80);
             last_text->setDefaultTextColor(QColor(Qt::red));

             last_temp=new QSpinBox();
             last_temp->setValue(mdiss_type.sleep_time);
             last_temp->setFixedSize(QSize(UNIT_WIDTH/2, 20));
             connect(last_temp, SIGNAL(valueChanged(int)), this, SLOT(LineChanged_last(int)));
             wid_last=Current_Scene.addWidget(last_temp);
             wid_last->setPos(group_total*120+180,40);

         }



        //绘制溶解曲线起始与终止值
        start_temp=new QDoubleSpinBox();
        start_temp->setValue(mdiss_type.start_temp);
        start_temp->setFixedSize(QSize(UNIT_WIDTH/2, 20));
        start_temp->setSingleStep(0.1);
        connect(start_temp, SIGNAL(valueChanged(double)), this, SLOT(LineChanged_start(double)));
        wid_start= Current_Scene.addWidget(start_temp);
//        wid_start->setPos(group_total*120+202,(int)((TemDpiend)*heit)-80);
        wid_start->setPos(group_total*120+60,TemDpistart*heit-30);


        end_temp=new QDoubleSpinBox();
        end_temp->setValue(mdiss_type.end_temp);
        end_temp->setFixedSize(QSize(UNIT_WIDTH/2, 20));
        end_temp->setSingleStep(0.1);
        connect(end_temp, SIGNAL(valueChanged(double)), this, SLOT(LineChanged_end(double)));
        wid_end= Current_Scene.addWidget(end_temp);
//        wid_end->setPos(group_total*120+60,(int)((TemDpistart)*heit)-80);
        wid_end->setPos(group_total*120+180,TemDpiend*heit-30);


        MeltFlag=mdiss_type.diss_sele;
        MeltStartTem = mdiss_type.start_temp;
        MeltEndTem  = mdiss_type.end_temp;

        disscurve_data.disscurve_check=true;
        disscurve_data.diss_sele=mdiss_type.diss_sele;
        disscurve_data.start_temp=mdiss_type.start_temp;
        disscurve_data.end_temp=mdiss_type.end_temp;
        disscurve_data.temp_rate=mdiss_type.temp_rate;
        disscurve_data.temp_grad=mdiss_type.temp_grad;
        disscurve_data.sleep_time=mdiss_type.sleep_time;

    }

}

void WidgetProjectEditProgram::ClearDissol()
{
//    qDebug()<<"清除溶解曲线";

    if(m_MeltPointer != nullptr)
    {
        delete m_MeltPointer;
        delete m_Melt_Rect_line;
        delete m_Melt_line_1;
        delete m_Melt_line_2;
        delete m_Melt_line_3;
        delete m_Melt_line_4;
        delete m_MeltText;

        delete start_temp;
        delete end_temp;
//        delete wid_start;
//        delete wid_end;

        m_MeltPointer=nullptr;
        m_Melt_Rect_line = nullptr;
        m_Melt_line_1 = nullptr;
        m_Melt_line_2 = nullptr;
        m_Melt_line_3 = nullptr;
        m_Melt_line_4 = nullptr;
        m_MeltText = nullptr;

        start_temp=nullptr;
        end_temp=nullptr;
//        wid_start=nullptr;
//        wid_end=nullptr;

        MeltFlag=false;
         disscurve_data.disscurve_check=false;
    }
    if(grad_temp !=nullptr)
    {
        delete grad_temp;
        grad_temp=nullptr;
        delete  grad_text;
        grad_text=nullptr;

        delete last_temp;
        last_temp=nullptr;
        delete last_text;
        last_text=nullptr;
    }
    if(rate_temp !=nullptr)
    {
        delete rate_temp;
        rate_temp=nullptr;
        delete rate_text;
        rate_text=nullptr;
    }



}

void WidgetProjectEditProgram::GetProgram(QList<WidgetProjectEditProgramGroup *> &Current_Group, DISSCURVE_TYPESELE &typesele)
{
    Current_Group=Current_View_Group;
//    MeFlag=MeltFlag;
//    MeStartTem= MeltStartTem ;
//    MeEndTem= MeltEndTem ;
    typesele.disscurve_check=disscurve_data.disscurve_check;
    typesele.diss_sele=disscurve_data.diss_sele;
    typesele.start_temp=disscurve_data.start_temp;
    typesele.end_temp= disscurve_data.end_temp;
    typesele.temp_rate=disscurve_data.temp_rate;
    typesele.temp_grad=disscurve_data.temp_grad;
    typesele.sleep_time=disscurve_data.sleep_time;
}

void WidgetProjectEditProgram::SlotInsertGroup_Front()
{
    //判断当前程序组个数是否超过最大值
    if(Current_View_Group.size() >= 9)
    {
       // qDebug()<<"程序组超过最大值不可添加"<<Current_View_Group.size();
        return ;
    }
    else
    {
       // qDebug()<<"程序组未超过最大值可添加"<<Current_View_Group.size();
    }

    //重新确定图像绘制的区域
   Current_Scene.setSceneRect(ui->graphicsView->rect().x(),ui->graphicsView->rect().y(),ui->graphicsView->rect().width()-2,ui->graphicsView->rect().height()-2);

   WidgetProjectEditProgramGroup *group_node = new WidgetProjectEditProgramGroup(&Current_Scene);
   connect(group_node,SIGNAL(SignalGroupSelect(int,int)),this,SLOT(SlotChangeGroupSelect(int, int)));
   connect(group_node,SIGNAL(SignalGroupStep(int,int)),this,SLOT(SlotChangeGroupStep(int, int)));
//   2020.10.19添加检测当前程序组是否有荧光检测
   connect(group_node,SIGNAL(SignalCheckFluo(int,int)),this,SLOT(SLotCheckFluo(int,int)));

   qDebug()<<"Current_View_Group.size()"<<Current_View_Group.size();
   //判断当前程序组是否为空
   if(Current_View_Group.size() == 0)
   {
       group_node->Current_View_Step.last()->step_pre_temperature = group_node->Current_View_Step.last()->step_temperature;
       group_node->current_group_id = Current_View_Group.count() + 1;

       Current_View_Group.append(group_node);
       group_node->SetGroupPos(GetGroupCount());
   }
   else
   {
       if(Select_Group_ID == 0 && Select_Step_ID == 0)
       {
           //后移选中及之后的程序组
           for (auto tmp_group:Current_View_Group)
           {
               if (tmp_group->current_group_id >= 1)
               {
                   tmp_group->current_group_id = tmp_group->current_group_id + 1;
                   auto index_list = GetGroupCount();
                   index_list.insert(1,1);
                   tmp_group->SetGroupPos(index_list);
               }
           }

           group_node->current_group_id = 1 ;
           Current_View_Group.insert(0,group_node);
           group_node->SetGroupPos(GetGroupCount());
           //设置当前程序组的前置温度
           group_node->Current_View_Step.last()->step_pre_temperature = group_node->Current_View_Step.last()->step_temperature;
           //设置下一程序组的前置温度
           Current_View_Group.at(1)->Current_View_Step.at(0)->step_pre_temperature=group_node->Current_View_Step.last()->step_temperature;
       }
       else
       {
           //后移选中及之后的程序组
           for (auto tmp_group:Current_View_Group)
           {
               if (tmp_group->current_group_id >= Select_Group_ID)
               {
                   tmp_group->current_group_id = tmp_group->current_group_id + 1;
                   auto index_list = GetGroupCount();
                   index_list.insert(Select_Group_ID,1);
                   tmp_group->SetGroupPos(index_list);
               }
           }

           group_node->current_group_id = Select_Group_ID ;
           Current_View_Group.insert(Select_Group_ID-1,group_node);
           group_node->SetGroupPos(GetGroupCount());
           //设置当前程序组的前置温度
           if(Select_Group_ID == 1)
           {
               group_node->Current_View_Step.last()->step_pre_temperature = group_node->Current_View_Step.last()->step_temperature;
           }
           else
           {
               group_node->Current_View_Step.last()->step_pre_temperature = Current_View_Group.at(Select_Group_ID-2)->Current_View_Step.last()->step_temperature;
           }
           //设置下一程序组的前置温度
           Current_View_Group.at(Select_Group_ID)->Current_View_Step.at(0)->step_pre_temperature=group_node->Current_View_Step.last()->step_temperature;
       }
       /*
       //后移选中及之后的程序组
       for (auto tmp_group:Current_View_Group)
       {
           if (tmp_group->current_group_id >= Select_Group_ID)
           {
               tmp_group->current_group_id = tmp_group->current_group_id + 1;
               auto index_list = GetGroupCount();
               index_list.insert(Select_Group_ID,1);
               tmp_group->SetGroupPos(index_list);
           }
       }

       group_node->current_group_id = Select_Group_ID ;
       Current_View_Group.insert(Select_Group_ID-1,group_node);
       group_node->SetGroupPos(GetGroupCount());
       //设置当前程序组的前置温度
       if(Select_Group_ID == 1)
       {
           group_node->Current_View_Step.last()->step_pre_temperature = group_node->Current_View_Step.last()->step_temperature;
       }
       else
       {
           group_node->Current_View_Step.last()->step_pre_temperature = Current_View_Group.at(Select_Group_ID-2)->Current_View_Step.last()->step_temperature;
       }
       //设置下一程序组的前置温度
       Current_View_Group.at(Select_Group_ID)->Current_View_Step.at(0)->step_pre_temperature=group_node->Current_View_Step.last()->step_temperature;
*/
   }

   /*
   //判断当前选中的程序组号是不是第一个程序组
   if(Select_Group_ID == 0 && Select_Step_ID == 0)
   {
       group_node->current_group_id = Current_View_Group.count() + 1;
       Current_View_Group.append(group_node);
       group_node->SetGroupPos(GetGroupCount());
   }
   else
   {
       //当前选中的不是
   }
*/





   //更新溶解曲线位置
   DissCurvePointUpdate();
}

void WidgetProjectEditProgram::SlotInsertGroup_After()
{
    //判断当前程序组个数是否超过最大值
    if(Current_View_Group.size() >= 9)
    {
       // qDebug()<<"程序组超过最大值不可添加"<<Current_View_Group.size();
        return ;
    }
    else
    {
       // qDebug()<<"程序组未超过最大值可添加"<<Current_View_Group.size();
    }

    //重新确定图像绘制的区域
   Current_Scene.setSceneRect(ui->graphicsView->rect().x(),ui->graphicsView->rect().y(),ui->graphicsView->rect().width()-2,ui->graphicsView->rect().height()-2);

   WidgetProjectEditProgramGroup *group_node = new WidgetProjectEditProgramGroup(&Current_Scene);
   connect(group_node,SIGNAL(SignalGroupSelect(int,int)),this,SLOT(SlotChangeGroupSelect(int, int)));
   connect(group_node,SIGNAL(SignalGroupStep(int,int)),this,SLOT(SlotChangeGroupStep(int, int)));
//   2020.10.19添加检测当前程序组是否有荧光检测
   connect(group_node,SIGNAL(SignalCheckFluo(int,int)),this,SLOT(SLotCheckFluo(int,int)));
   //插入时，考虑当前选中的位置，后面的位置需要进行重定位
   if(Select_Group_ID == 0 && Select_Step_ID == 0)
   {
       group_node->current_group_id = Current_View_Group.count() + 1;
       Current_View_Group.append(group_node);
       group_node->SetGroupPos(GetGroupCount());
   }
   else
   {
       if(Select_Group_ID > Current_View_Group.count())
       {
           group_node->current_group_id = Current_View_Group.count() + 1;
           Current_View_Group.append(group_node);
           group_node->SetGroupPos(GetGroupCount());
       }
       else
       {
           for (auto tmp_group:Current_View_Group)
           {
               if (tmp_group->current_group_id > Select_Group_ID)
               {
                   tmp_group->current_group_id = tmp_group->current_group_id + 1;
                   auto index_list = GetGroupCount();
                   index_list.insert(Select_Group_ID,1);
                   tmp_group->SetGroupPos(index_list);
               }
           }
           group_node->current_group_id = Select_Group_ID + 1;
           Current_View_Group.insert(Select_Group_ID,group_node);
           group_node->SetGroupPos(GetGroupCount());
       }
   }

   //更新溶解曲线位置
   DissCurvePointUpdate();


}

//void WidgetProjectEditProgram::GetProgram(QList<WidgetProjectEditProgramGroup *> &Current_Group, int &MeFlag, double &MeStartTem, double &MeEndTem)
//{
//    Current_Group=Current_View_Group;
//    MeFlag=MeltFlag;
//    MeStartTem= MeltStartTem ;
//    MeEndTem= MeltEndTem ;
//}
//插入程序组
void WidgetProjectEditProgram::SlotInsertGroup()
{
    //判断当前程序组个数是否超过最大值
    if(Current_View_Group.size() >= 9)
    {
       // qDebug()<<"程序组超过最大值不可添加"<<Current_View_Group.size();
        return ;
    }
    else
    {
       // qDebug()<<"程序组未超过最大值可添加"<<Current_View_Group.size();
    }


     //重新确定图像绘制的区域
    Current_Scene.setSceneRect(ui->graphicsView->rect().x(),ui->graphicsView->rect().y(),ui->graphicsView->rect().width()-2,ui->graphicsView->rect().height()-2);

    WidgetProjectEditProgramGroup *group_node = new WidgetProjectEditProgramGroup(&Current_Scene);
    connect(group_node,SIGNAL(SignalGroupSelect(int,int)),this,SLOT(SlotChangeGroupSelect(int, int)));
    connect(group_node,SIGNAL(SignalGroupStep(int,int)),this,SLOT(SlotChangeGroupStep(int, int)));
    //2020.10.19添加检测当前程序组是否有荧光检测
    connect(group_node,SIGNAL(SignalCheckFluo(int,int)),this,SLOT(SLotCheckFluo(int,int)));

//    if(!Current_View_Group.isEmpty())
//    {
//        group_node->Current_View_Step.last()->step_pre_temperature = group_node->Current_View_Step.last()->step_temperature;
//    }

    //插入时，考虑当前选中的位置，后面的位置需要进行重定位
    if(Select_Group_ID == 0 && Select_Step_ID == 0)
    {
        group_node->current_group_id = Current_View_Group.count() + 1;
        Current_View_Group.append(group_node);
        group_node->SetGroupPos(GetGroupCount());
    }
    else
    {
        if(Select_Group_ID > Current_View_Group.count())
        {
            group_node->current_group_id = Current_View_Group.count() + 1;
            Current_View_Group.append(group_node);
            group_node->SetGroupPos(GetGroupCount());
        }
        else
        {
            for (auto tmp_group:Current_View_Group)
            {
                if (tmp_group->current_group_id > Select_Group_ID)
                {
                    tmp_group->current_group_id = tmp_group->current_group_id + 1;
                    auto index_list = GetGroupCount();
                    index_list.insert(Select_Group_ID,1);
                    tmp_group->SetGroupPos(index_list);
                }
            }
            group_node->current_group_id = Select_Group_ID + 1;
            Current_View_Group.insert(Select_Group_ID,group_node);
            group_node->SetGroupPos(GetGroupCount());
        }
    }

    //更新溶解曲线位置
    DissCurvePointUpdate();
}

void WidgetProjectEditProgram::SlotInsertGroup_Last()
{
    //判断当前程序组个数是否超过最大值
    if(Current_View_Group.size() >= 9)
    {
       // qDebug()<<"程序组超过最大值不可添加"<<Current_View_Group.size();
        return ;
    }
    else
    {
       // qDebug()<<"程序组未超过最大值可添加"<<Current_View_Group.size();
    }


     //重新确定图像绘制的区域
    Current_Scene.setSceneRect(ui->graphicsView->rect().x(),ui->graphicsView->rect().y(),ui->graphicsView->rect().width()-2,ui->graphicsView->rect().height()-2);

    WidgetProjectEditProgramGroup *group_node = new WidgetProjectEditProgramGroup(&Current_Scene);
    connect(group_node,SIGNAL(SignalGroupSelect(int,int)),this,SLOT(SlotChangeGroupSelect(int, int)));
    connect(group_node,SIGNAL(SignalGroupStep(int,int)),this,SLOT(SlotChangeGroupStep(int, int)));
    //2020.10.19添加检测当前程序组是否有荧光检测
    connect(group_node,SIGNAL(SignalCheckFluo(int,int)),this,SLOT(SLotCheckFluo(int,int)));
    //判断当前程序组是否为空
    if(Current_View_Group.size() == 0)
    {
        group_node->Current_View_Step.last()->step_pre_temperature = group_node->Current_View_Step.last()->step_temperature;
        group_node->current_group_id = Current_View_Group.count() + 1;

        Current_View_Group.append(group_node);
        group_node->SetGroupPos(GetGroupCount());
    }
    else
    {
        //讲程序中插入最后
    }

}
//插入程序步骤
void WidgetProjectEditProgram::SlotInsertStep()
{
   // qDebug()<<"Select_Group_ID"<<Select_Group_ID<<"Select_Step_ID"<<Select_Step_ID;




    if(Select_Group_ID == 0 && Select_Step_ID == 0)
    {
        return;
    }
    //判断选中的程序组中步骤是否超过最大值
    if(Current_View_Group.at(Select_Group_ID-1)->Current_View_Step.size() >= 9)
    {
       // qDebug()<<"步骤超过最大值"<<Current_View_Group.at(Select_Group_ID-1)->Current_View_Step.size();
        return;
    }
    else
    {
       // qDebug()<<"步骤未超过最大值"<<Current_View_Group.at(Select_Group_ID-1)->Current_View_Step.size();
    }
    if(Select_Group_ID > Current_View_Group.count())
    {
        return;
    }
    else
    {//插入步骤前，需要考虑以及存在的程序组重新定位问题。
        for (auto tmp_group:Current_View_Group)
        {
            if (tmp_group->current_group_id > Select_Group_ID) {
                auto index_list = GetGroupCount();
                index_list.replace(Select_Group_ID-1,index_list.at(Select_Group_ID-1)+1);
                tmp_group->SetGroupPos(index_list);
            }
        }
        Current_View_Group.at(Select_Group_ID-1)->InsertStep(Select_Step_ID,GetGroupCount());//插入step步骤

        //获取上一个步骤设置的温度
        Current_View_Group.at(Select_Group_ID-1)->Current_View_Step.at(Select_Step_ID)->step_pre_temperature=
                Current_View_Group.at(Select_Group_ID-1)->Current_View_Step.at(Select_Step_ID-1)->step_temperature;
        //设置下一个步骤的前置温度
       if (Current_View_Group.at(Select_Group_ID-1)->Current_View_Step.count() >= Select_Step_ID+2)
       {
           Current_View_Group.at(Select_Group_ID-1)->Current_View_Step.at(Select_Step_ID+1)->step_pre_temperature=
                   Current_View_Group.at(Select_Group_ID-1)->Current_View_Step.at(Select_Step_ID)->step_temperature;

       }
       if(Select_Group_ID < Current_View_Group.count())
       {
           int count= Current_View_Group.at(Select_Group_ID-1)->Current_View_Step.count();
           Current_View_Group.at(Select_Group_ID)->Current_View_Step.at(0)->step_pre_temperature=
           Current_View_Group.at(Select_Group_ID-1)->Current_View_Step.at(count-1)->step_temperature;
       }


    }
    //更新溶解曲线位置
    DissCurvePointUpdate();
}
//删除程序步骤
void WidgetProjectEditProgram::SlotDeleteStep()
{

    if(Select_Group_ID == 0 && Select_Step_ID == 0)
    {
        return;
    }

    if(Select_Group_ID > Current_View_Group.count())
    {
        Select_Group_ID = 0;
        Select_Step_ID = 0;
        return;
    }

    if(Select_Group_ID <= Current_View_Group.count())
    {    //删除步骤前，需要考虑已经存在的程序组重新定位问题。
        auto group = Current_View_Group.at(Select_Group_ID-1);
        if(Select_Step_ID > group->Current_View_Step.count())
        {
            Select_Group_ID = 0;
            Select_Step_ID = 0;
            return;
        }
        if(group->Current_View_Step.count() == 1)
        {
            delete group->Current_View_Step.at(Select_Step_ID-1);
            group->Current_View_Step.removeAt(Select_Step_ID-1);
            delete Current_View_Group.at(Select_Group_ID-1);
            Current_View_Group.removeAt(Select_Group_ID-1);
            for (int i=Select_Group_ID-1; i<Current_View_Group.count(); i++) {
                Current_View_Group.at(i)->current_group_id -= 1;
                Current_View_Group.at(i)->SetGroupPos(GetGroupCount());
            }
            Select_Group_ID = 0;
            Select_Step_ID = 0;
        }
        else if(group->Current_View_Step.count() > 1)
        {

         //   qDebug()<<"é?3yID="<<Select_Step_ID<<"2??èμ?×üêy"<<group->Current_View_Step.count();
            //设置下一步骤前置温度
            if(group->Current_View_Step.count() > 2 && (group->Current_View_Step.count() > Select_Step_ID) &&(Select_Step_ID != 1))
            {
                Current_View_Group.at(Select_Group_ID-1)->Current_View_Step.at(Select_Step_ID)->step_pre_temperature=
                        Current_View_Group.at(Select_Group_ID-1)->Current_View_Step.at(Select_Step_ID-2)->step_temperature;
             //   qDebug()<<"Select_Step_ID !=1 Select_Group_ID!=1"<<Select_Group_ID<<Select_Step_ID;
            }
            else if( (Select_Step_ID == 1) && (Select_Group_ID == 1) )
            {
              //  qDebug()<<"Select_Step_ID ==1 Select_Group_ID ==1"<<Select_Group_ID<<Select_Step_ID;
                Current_View_Group.at(Select_Group_ID-1)->Current_View_Step.at(Select_Step_ID)->step_pre_temperature=UNIT_TEMP_MIN;
            }
            delete group->Current_View_Step.at(Select_Step_ID-1);
            group->Current_View_Step.removeAt(Select_Step_ID-1);
            for (int i=Select_Step_ID-1; i<group->Current_View_Step.count(); i++)
            {
                group->Current_View_Step.at(i)->current_step_id -= 1;
                group->Current_View_Step.at(i)->SetStepPos(Select_Group_ID,GetGroupCount());
            }

        }
    }
    SetGroupPosUpdate();
    Select_Group_ID=0;
    Select_Step_ID=0;
    DissCurvePointUpdate();
}

void WidgetProjectEditProgram::SlotDeleteParogSeg()
{
    //
//    qDebug()<<"删除程序段";
    if(Select_Group_ID == 0 && Select_Step_ID == 0){
        return;
    }

    if(Select_Group_ID > Current_View_Group.count()){
        Select_Group_ID = 0;
        Select_Step_ID = 0;
        return;
    }
//    qDebug()<<"Select_Group_ID="<<Select_Group_ID<<"Select_Step_ID"<<Select_Step_ID;
    //判断所选的程序组ID位置
    int size=-1;
    if(Select_Group_ID == Current_View_Group.count())
    {
        int size=Current_View_Group.at(Select_Group_ID-1)->Current_View_Step.count();
         for (int i=size;i>0;i--)
         {
             delete Current_View_Group.at(Select_Group_ID-1)->Current_View_Step.at(i-1);
            Current_View_Group.at(Select_Group_ID-1)->Current_View_Step.removeAt(i-1);

         }
         delete Current_View_Group.at(Select_Group_ID-1);
         Current_View_Group.removeAt(Select_Group_ID-1);

    }
    else
    {
        //更新信息
        if( (Select_Group_ID != 1) && (Current_View_Group.count() > 2) )
        {
//            Current_View_Group.at(Select_Group_ID-1)->Current_View_Step.at(Select_Step_ID)->step_pre_temperature=30;
            int count=Current_View_Group.at(Select_Group_ID-2)->Current_View_Step.count();
            Current_View_Group.at(Select_Group_ID)->Current_View_Step.at(0)->step_pre_temperature=
                    Current_View_Group.at(Select_Group_ID-2)->Current_View_Step.at(count -1 )->step_temperature;
          //  qDebug()<<"前置温度"<< Current_View_Group.at(Select_Group_ID)->Current_View_Step.at(0)->step_pre_temperature;
          //  qDebug()<<"最后一个步骤温度"<< Current_View_Group.at(Select_Group_ID-2)->Current_View_Step.at(count -1 )->step_temperature;

        }
        else
        {
             Current_View_Group.at(Select_Group_ID)->Current_View_Step.at(0)->step_pre_temperature=UNIT_TEMP_MIN;
        }
        //获取所选程序组包含的步骤数
        size=Current_View_Group.at(Select_Group_ID-1)->Current_View_Step.count();
        //删除程序组
        for (int i=size;i>0;i--)
        {
            delete Current_View_Group.at(Select_Group_ID-1)->Current_View_Step.at(i-1);
           Current_View_Group.at(Select_Group_ID-1)->Current_View_Step.removeAt(i-1);

        }
        delete Current_View_Group.at(Select_Group_ID-1);
        Current_View_Group.removeAt(Select_Group_ID-1);
        //更新程序组ID
        //更新显示位置
        for (int i=Select_Group_ID-1; i<Current_View_Group.count(); i++)
        {
            Current_View_Group.at(i)->current_group_id -= 1;
            Current_View_Group.at(i)->SetGroupPos(GetGroupCount());

        }

    }
    Select_Group_ID=0;
    Select_Step_ID=0;
    DissCurvePointUpdate();

}
//设置荧光标志位
void WidgetProjectEditProgram::SlotSetFluoStep()
{
    if(Current_View_Group.size() == 0)
    {
        return;
    }
    if(Select_Step_ID == 0 || Select_Group_ID == 0)
    {
        QMessageBox::information(this,"ERROR","Please Select Step!");
        return;
    }
    auto group = Current_View_Group.at(Select_Group_ID-1);
    auto step = group->Current_View_Step.at(Select_Step_ID-1);
    if(step->step_fluoflag == false)
    {
        step->step_fluoflag = true;
    }else {
        step->step_fluoflag = false;
    }
    step->ReplotView();
}
//导入已存在的程序
void WidgetProjectEditProgram::SlotImportProgram()
{
    Current_Scene.setSceneRect(ui->graphicsView->rect().x(),ui->graphicsView->rect().y(),ui->graphicsView->rect().width()-2,ui->graphicsView->rect().height()-2);
//    auto button = QMessageBox::information(this,tr("Import"),tr("Press YES to Export Data OR Press NO to Import Data?"),QMessageBox::Yes|QMessageBox::No|QMessageBox::Cancel);
//    auto button = QMessageBox::information(this,tr("导入导出选择"),tr("选择Yes导出实验程序，选择No导入实验程序?"),QMessageBox::Yes|QMessageBox::No|QMessageBox::Cancel);
       auto button = QMessageBox::information(this,tr("导入选择"),tr("选择Yes导入实验程序，选择No放弃导入实验程序?"),QMessageBox::Yes|QMessageBox::No);

    switch (button) {
    case QMessageBox::Yes:
    {
        ClearScene();
        //auto fileaddr = QFileDialog::getOpenFileName(this,tr("选择打开文件"),"./Project",tr("文件类型(*.pro)"));
       // SetEditProgram(fileaddr);

        break;
    }

    case QMessageBox::No:
//    {
//        ClearScene();
//        auto fileaddr = QFileDialog::getOpenFileName(this,"Select Open File","./Project","Project Files(*.pro)");
//        SetEditProgram(fileaddr);
//        break;
//    }
    case QMessageBox::Cancel:
        break;
    default:
        break;
    }
}
//确认当前选中的程序步骤和组ID
void WidgetProjectEditProgram::SlotChangeGroupSelect(int group, int step)
{
//    Select_Group_ID = group;
//    Select_Step_ID = step;
//    qDebug()<<"???Dμ?3ìDò?DID:"<<Select_Group_ID<<"???Dμ?2??èID:"<<Select_Step_ID;
//    auto step_count = Current_View_Group.at(Select_Group_ID-1)->Current_View_Step.count();
//    auto group_count = Current_View_Group.count();

//    auto temp = Current_View_Group.at(Select_Group_ID-1)->Current_View_Step.at(Select_Step_ID-1)->step_temperature;

//    if(step_count > Select_Step_ID)
//    {
//        Current_View_Group.at(Select_Group_ID-1)->Current_View_Step.at(Select_Step_ID)->step_pre_temperature = temp;
//        Current_View_Group.at(Select_Group_ID-1)->Current_View_Step.at(Select_Step_ID)->ReplotView();
//    }else {
//        if(group_count > Select_Group_ID)
//        {
//            Current_View_Group.at(Select_Group_ID)->Current_View_Step.at(0)->step_pre_temperature = temp;
//            Current_View_Group.at(Select_Group_ID)->Current_View_Step.at(0)->ReplotView();
//        }
//    }
    auto step_count = Current_View_Group.at(group-1)->Current_View_Step.count();
    auto group_count = Current_View_Group.count();

    auto temp = Current_View_Group.at(group-1)->Current_View_Step.at(step-1)->step_temperature;

    if(step_count > step)
    {
        Current_View_Group.at(group-1)->Current_View_Step.at(step)->step_pre_temperature = temp;
        Current_View_Group.at(group-1)->Current_View_Step.at(step)->ReplotView();
    }else {
        if(group_count > group)
        {
            Current_View_Group.at(group)->Current_View_Step.at(0)->step_pre_temperature = temp;
            Current_View_Group.at(group)->Current_View_Step.at(0)->ReplotView();
        }
    }
    Check_group_id(group,step);

}

void WidgetProjectEditProgram::SlotChangeGroupStep(int group, int step)
{
    Select_Group_ID = group;
    Select_Step_ID = step;
    qDebug()<<"Select_Step_ID Select_Group_ID"<<Select_Group_ID<<Select_Step_ID;
}

void WidgetProjectEditProgram::LineChanged_start(double vale)
{

    if(vale < 30.0)
    {
        return;
    }
    TemDpistart =1.0- (vale-20)/100;
    auto heit=Current_Scene.height();

    wid_start->setPos(group_total*120+60,TemDpistart*heit-30);
    MeltStartTem = vale;
    disscurve_data.start_temp=vale;

    m_Melt_line_1->setLine(group_total*120,(1.0-(group_endtemp-20.0)/100)*heit,group_total*120+60,TemDpistart*heit);
    m_Melt_line_2->setLine(group_total*120+60,TemDpistart*heit,group_total*120+120,TemDpistart*heit);
    m_Melt_line_3->setLine(group_total*120+120,TemDpistart*heit,group_total*120+180,TemDpiend*heit);

}

void WidgetProjectEditProgram::LineChanged_end(double vale)
{

    if(vale < 30.0)
    {
        return;
    }
    TemDpiend =1.0- (vale-20)/100;
    auto heit=Current_Scene.height();
    wid_end->setPos(group_total*120+180,TemDpiend*Current_Scene.height()-30);

    MeltEndTem  = vale;
    disscurve_data.end_temp=vale;

    m_Melt_line_3->setLine(group_total*120+120,TemDpistart*heit,group_total*120+180,TemDpiend*heit);
    m_Melt_line_4->setLine(group_total*120+180,TemDpiend*heit, group_total*120+240,TemDpiend*heit);

}

void WidgetProjectEditProgram::LineChanged_grad(double vale)
{
    disscurve_data.temp_grad=vale;
}

void WidgetProjectEditProgram::LineChanged_last(int vale)
{
    disscurve_data.sleep_time=vale;
}

void WidgetProjectEditProgram::LineChanged_rate(double vale)
{
    disscurve_data.temp_rate=vale;

}

void WidgetProjectEditProgram::SLotCheckFluo(int group, int step)
{
    qDebug()<<"group"<<group<<"step"<<step;
//    qDebug()<<Current_View_Group[group-1]->Current_View_Step.count();
    for (int i=0;i<Current_View_Group[group-1]->Current_View_Step.count();i++)
    {
        if(Current_View_Group[group-1]->Current_View_Step.at(i)->step_fluoflag == true)
        {
            qDebug()<<"存在应该，不能设置";
            QMessageBox::warning(this,"提示信息",tr("荧光检测存在，不能设置"),QMessageBox::Ok);
            return;
        }
    }
    //设置改步骤进行采集荧光
    Current_View_Group[group-1]->Current_View_Step.at(step-1)->step_fluoflag=true;

}

void WidgetProjectEditProgram::Check_group_id(int group, int id)
{

    //判断当前溶解曲线是否存在
    if(m_MeltPointer == nullptr)
    {
        return;
    }
    //判断当前程序组是否为最后一个程序组
    if(Current_View_Group.size() == group)
    {
        //判读是否是最后一个步骤
        if(Current_View_Group[group-1]->Current_View_Step.size() == id)
        {
            //更新溶解曲线第一个线的虚线
            auto heit=Current_Scene.height();
            group_endtemp=static_cast<double>(Current_View_Group[Current_View_Group.size()-1]->Current_View_Step[Current_View_Group[Current_View_Group.size()-1]->Current_View_Step.size()-1]->step_temperature);
            m_Melt_line_1->setLine(group_total*120,(1.0-(group_endtemp-20.0)/100)*heit,group_total*120+60,TemDpistart*heit);
        }
    }
}

void WidgetProjectEditProgram::DissCurvePointUpdate()
{

    if(m_MeltPointer != nullptr)
    {

        QList<int> group_size;
        group_total=0;
        group_size.clear();
        group_size= GetGroupCount();
        for (auto lin_size:group_size)
        {
            group_total=group_total+lin_size;
        }
        if(group_total == 0)
        {
            //当前程序组已清空，删除溶解曲线
          //  qDebug()<<"DissCurvePointUpdate group_total="<<group_total;
            ClearDissol();
            return;
        }
       auto heit= Current_Scene.height();
      // qDebug()<<"DissCurvePointUpdate group_total="<<group_total<<"group_total*120="<<group_total*120;

        group_endtemp= static_cast<double>(Current_View_Group[Current_View_Group.size()-1]->Current_View_Step[Current_View_Group[Current_View_Group.size()-1]->Current_View_Step.size()-1]->step_temperature);

//        m_MeltPointer->setPos(group_total*120, 100);
         m_MeltPointer->setPos((group_total-1)*120, 0);
         m_MeltPointer->setBrush(QColor(255, 255, 255));

//        m_Melt_Rect_line->setLine(group_total*120,-2,group_total*120,Current_Scene.height()+12);
//        m_Melt_Rect_line->setPen(QPen(Qt::black, 2));
         if(m_Melt_Rect_line != nullptr)
         {
             delete  m_Melt_Rect_line;
             m_Melt_Rect_line=nullptr;
         }
         m_Melt_Rect_line = Current_Scene.addLine(group_total*120,-2,group_total*120,Current_Scene.height()+2,QPen(Qt::black, 2));


         //绘制溶解曲线线条
         m_Melt_line_1->setLine(group_total*120,(1.0-(group_endtemp-20.0)/100)*heit, group_total*120+60, TemDpistart*heit);
         m_Melt_line_2->setLine(group_total*120+60, TemDpistart*heit, group_total*120+120, TemDpistart*heit);
         m_Melt_line_3->setLine(group_total*120+120, TemDpistart*heit, group_total*120+180,TemDpiend*heit);
         m_Melt_line_4->setLine(group_total*120+180, TemDpiend*heit, group_total*120+240, TemDpiend*heit);

         //绘制显示标题控件;
//        m_MeltText->setPos(group_total*120+10,20);
         m_MeltText->setPos(group_total*120+95,10);

        wid_start->setPos(group_total*120+60,TemDpistart*heit-30);
        wid_end->setPos(group_total*120+180,TemDpiend*heit-30);
        if(1 == disscurve_data.diss_sele)
        {
            rate_text->setPos(group_total*120+5,40);
            wid_rate->setPos(group_total*120+60,40);
        }
        else if(2 == disscurve_data.diss_sele)
        {
            grad_text->setPos(group_total*120+5,40);
            wid_grad->setPos(group_total*120+60,40);

            last_text->setPos(group_total*120+125,40);
            wid_last->setPos(group_total*120+180,40);

        }


    }
}
