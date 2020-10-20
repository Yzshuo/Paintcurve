#include "filelocalsave.h"


#pragma execution_character_set("utf-8")
FileLocalSave::FileLocalSave()
{

}
//保存程序数据，目标后缀为pro
QString FileLocalSave::ProgramGroupSave(QString save_name, QList<WidgetProjectEditProgramGroup *> Current_View_Group)
{
    QJsonObject Program;
    QJsonArray ProgramGroupArray;

    for(auto tmp_group:Current_View_Group)
    {
        QJsonObject ProgramGroup;
        QJsonArray ProgramStepArray;
        ProgramGroup.insert("Group_ID",tmp_group->current_group_id);
        ProgramGroup.insert("Group_Cycle",tmp_group->group_cycle_num);
        for(auto tmp_step:tmp_group->Current_View_Step)
        {
            QJsonObject ProgramStep;
            ProgramStep.insert("Step_ID",tmp_step->current_step_id);
            ProgramStep.insert("Step_PRE_TEM",static_cast<double>(tmp_step->step_pre_temperature));
            ProgramStep.insert("Step_CUR_TEM",static_cast<double>(tmp_step->step_temperature));
            ProgramStep.insert("Step_Time",tmp_step->step_time);
            ProgramStep.insert("Step_Fluo",tmp_step->step_fluoflag);
            ProgramStepArray.append(ProgramStep);
        }
        ProgramGroup.insert("Step",ProgramStepArray);
        ProgramGroupArray.append(ProgramGroup);
    }
    Program.insert("Program",ProgramGroupArray);

    auto            SaveAddr = QDir::currentPath() + "/Project/" + save_name + ".pro";
    QFile           LocalFile(SaveAddr);
    QJsonDocument   document(Program);

    if (LocalFile.open(QFile::WriteOnly | QFile::Truncate))
    {
        LocalFile.write(document.toJson(QJsonDocument::Compact));
        LocalFile.close();
    }

    return SaveAddr;
}
//读取程序数据，目标后缀为pro
QList<WidgetProjectEditProgramGroup *> FileLocalSave::ProgramGroupLoad(QString Local_File_Addr, QGraphicsScene* scene)
{
    QFile LocalFile(Local_File_Addr);
    QJsonObject		jsonObject;

    if (LocalFile.open(QFile::ReadOnly | QIODevice::Text))
    {
        auto bytes = LocalFile.readAll();
        auto str = QString::fromUtf8(bytes);
        jsonObject = QJsonDocument::fromJson(str.toUtf8()).object();
    }

    QList<WidgetProjectEditProgramGroup *> open_program;

    QJsonArray ProgramGroupArray = jsonObject["Program"].toArray();
    for(QJsonValue tmp_group:ProgramGroupArray)
    {
        WidgetProjectEditProgramGroup *node_group = new WidgetProjectEditProgramGroup(scene);
        QJsonObject ProgramGroup = tmp_group.toObject();
        node_group->current_group_id    = ProgramGroup["Group_ID"].toInt();
        node_group->group_cycle_num     = ProgramGroup["Group_Cycle"].toInt();
        QJsonArray ProgramStepArray     = ProgramGroup["Step"].toArray();
        for(int i=0; i<ProgramStepArray.count(); i++)
        {
            auto tmp_step = ProgramStepArray.at(i);
            if(i == 0)
            {
                QJsonObject ProgramStep = tmp_step.toObject();
                node_group->Current_View_Step.at(0)->current_step_id        = ProgramStep["Step_ID"].toInt();
                node_group->Current_View_Step.at(0)->step_pre_temperature   = static_cast<float>(ProgramStep["Step_PRE_TEM"].toDouble());
                node_group->Current_View_Step.at(0)->step_temperature       = static_cast<float>(ProgramStep["Step_CUR_TEM"].toDouble());
                node_group->Current_View_Step.at(0)->step_time              = ProgramStep["Step_Time"].toInt();
                node_group->Current_View_Step.at(0)->step_fluoflag          = ProgramStep["Step_Fluo"].toBool();
                // node_group->Current_View_Step.at(0)->SlotEditTemp(node_group->Current_View_Step.at(0)->step_temperature);
                node_group->Current_View_Step.at(0)->ReplotView();
            }else {
                WidgetProjectEditProgramStep *node_step = new WidgetProjectEditProgramStep(scene);
                QObject::connect(node_step,SIGNAL(SignalStepSelect(int)),node_group,SLOT(SlotChangeStepSelect(int)));
                QJsonObject ProgramStep         = tmp_step.toObject();
                node_step->current_step_id      = ProgramStep["Step_ID"].toInt();
                node_step->step_pre_temperature = static_cast<float>(ProgramStep["Step_PRE_TEM"].toDouble());
                node_step->step_temperature     = static_cast<float>(ProgramStep["Step_CUR_TEM"].toDouble());
                node_step->step_time            = ProgramStep["Step_Time"].toInt();
                node_step->step_fluoflag        = ProgramStep["Step_Fluo"].toBool();
                node_group->Current_View_Step.append(node_step);
                node_step->ReplotView();
            }
        }
        open_program.append(node_group);
    }
    return open_program;
}

//QString FileLocalSave::ProgramGroupSave(QString save_name,
//                                        QList<WidgetProjectEditProgramGroup *> Current_View_Group,
//                                        int MeltFlag, double MeltStartTem, double MeltEndTem)
QString FileLocalSave::ProgramGroupSave(QString save_name,QList<WidgetProjectEditProgramGroup*> Current_View_Group,
                                                                     DISSCURVE_TYPESELE typesele)
{
    QJsonObject Program;
    QJsonArray ProgramGroupArray;

    for(auto tmp_group:Current_View_Group)
    {
        QJsonObject ProgramGroup;
        QJsonArray ProgramStepArray;
        ProgramGroup.insert("Group_ID",tmp_group->current_group_id);
        ProgramGroup.insert("Group_Cycle",tmp_group->group_cycle_num);
        for(auto tmp_step:tmp_group->Current_View_Step)
        {
            QJsonObject ProgramStep;
            ProgramStep.insert("Step_ID",tmp_step->current_step_id);
            ProgramStep.insert("Step_PRE_TEM",static_cast<double>(tmp_step->step_pre_temperature));
            ProgramStep.insert("Step_CUR_TEM",static_cast<double>(tmp_step->step_temperature));
            ProgramStep.insert("Step_Time",tmp_step->step_time);
            ProgramStep.insert("Step_Fluo",tmp_step->step_fluoflag);
            ProgramStepArray.append(ProgramStep);
        }
        ProgramGroup.insert("Step",ProgramStepArray);
        ProgramGroupArray.append(ProgramGroup);
    }
    Program.insert("Program",ProgramGroupArray);

    Program.insert("MeltFlag",typesele.disscurve_check);
    Program.insert("diss_sele",typesele.diss_sele);
    Program.insert("start_temp",typesele.start_temp);
    Program.insert("end_temp",typesele.end_temp);
    Program.insert("temp_rate",typesele.temp_rate);
    Program.insert("temp_grad",typesele.temp_grad);
    Program.insert("sleep_time",typesele.sleep_time);
//    qDebug()<<"save Group MeltFlag="<<MeltFlag<<"MeltStartTem="<<MeltStartTem<<"MeltEndTem="<<MeltEndTem;

    auto            SaveAddr = QDir::currentPath() + "/Project/" + save_name + ".pro";
    QFile           LocalFile(SaveAddr);
    QJsonDocument   document(Program);

    if (LocalFile.open(QFile::WriteOnly | QFile::Truncate))
    {
        LocalFile.write(document.toJson(QJsonDocument::Compact));
        LocalFile.close();
    }

    return SaveAddr;
}

//QList<WidgetProjectEditProgramGroup *> FileLocalSave::ProgramGroupLoad(QString Local_File_Addr,
//                                                                       QGraphicsScene *scene,
//                                                                       int &MeltFlag, double &MeltStartTem, double &MeltEndTem)
QList<WidgetProjectEditProgramGroup *> FileLocalSave::ProgramGroupLoad(QString Local_File_Addr, QGraphicsScene* scene,DISSCURVE_TYPESELE &typesele)
{
    QFile LocalFile(Local_File_Addr);
    QJsonObject		jsonObject;

    if (LocalFile.open(QFile::ReadOnly | QIODevice::Text))
    {
        auto bytes = LocalFile.readAll();
        auto str = QString::fromUtf8(bytes);
        jsonObject = QJsonDocument::fromJson(str.toUtf8()).object();
    }

    QList<WidgetProjectEditProgramGroup *> open_program;

    QJsonArray ProgramGroupArray = jsonObject["Program"].toArray();
    for(QJsonValue tmp_group:ProgramGroupArray)
    {
        WidgetProjectEditProgramGroup *node_group = new WidgetProjectEditProgramGroup(scene);
        QJsonObject ProgramGroup = tmp_group.toObject();
        node_group->current_group_id    = ProgramGroup["Group_ID"].toInt();
        node_group->group_cycle_num     = ProgramGroup["Group_Cycle"].toInt();
        QJsonArray ProgramStepArray     = ProgramGroup["Step"].toArray();
        for(int i=0; i<ProgramStepArray.count(); i++)
        {
            auto tmp_step = ProgramStepArray.at(i);
            if(i == 0)
            {
                QJsonObject ProgramStep = tmp_step.toObject();
                node_group->Current_View_Step.at(0)->current_step_id        = ProgramStep["Step_ID"].toInt();
                node_group->Current_View_Step.at(0)->step_pre_temperature   = static_cast<float>(ProgramStep["Step_PRE_TEM"].toDouble());
                node_group->Current_View_Step.at(0)->step_temperature       = static_cast<float>(ProgramStep["Step_CUR_TEM"].toDouble());
                node_group->Current_View_Step.at(0)->step_time              = ProgramStep["Step_Time"].toInt();
                node_group->Current_View_Step.at(0)->step_fluoflag          = ProgramStep["Step_Fluo"].toBool();
                // node_group->Current_View_Step.at(0)->SlotEditTemp(node_group->Current_View_Step.at(0)->step_temperature);
                node_group->Current_View_Step.at(0)->ReplotView();
            }else {
                WidgetProjectEditProgramStep *node_step = new WidgetProjectEditProgramStep(scene);
                QObject::connect(node_step,SIGNAL(SignalStepSelect(int)),node_group,SLOT(SlotChangeStepSelect(int)));
                QJsonObject ProgramStep         = tmp_step.toObject();
                node_step->current_step_id      = ProgramStep["Step_ID"].toInt();
                node_step->step_pre_temperature = static_cast<float>(ProgramStep["Step_PRE_TEM"].toDouble());
                node_step->step_temperature     = static_cast<float>(ProgramStep["Step_CUR_TEM"].toDouble());
                node_step->step_time            = ProgramStep["Step_Time"].toInt();
                node_step->step_fluoflag        = ProgramStep["Step_Fluo"].toBool();
                node_group->Current_View_Step.append(node_step);
                node_step->ReplotView();
            }
        }
        open_program.append(node_group);
    }

//    QJsonObject Project;



    typesele.disscurve_check=jsonObject["MeltFlag"].toBool();
    typesele.diss_sele=jsonObject["diss_sele"].toInt();
    typesele.start_temp=jsonObject["start_temp"].toDouble();
    typesele.end_temp=jsonObject["end_temp"].toDouble();
    typesele.temp_rate=jsonObject["temp_rate"].toDouble();
    typesele.temp_grad=jsonObject["temp_grad"].toDouble();
    typesele.sleep_time=jsonObject["sleep_time"].toInt();

//    qDebug()<<"load Group MeltFlag="<<MeltFlag<<"MeltStartTem="<<MeltStartTem<<"MeltEndTem="<<MeltEndTem;

    return open_program;
}
/*
//保存程序数据，目标后缀为prj
QString FileLocalSave::ProjectInfoSave(QString save_name, Project_Info project_info)
{
    QJsonObject Project;
    int size=0;
    QString passageway;
    QString passageway_name;
    passageway.clear();
    passageway_name.clear();
    Project.insert("Project_Name",project_info.baseinfo.project_name);
    Project.insert("Project_Version",project_info.baseinfo.project_version);
    Project.insert("Project_Type",project_info.baseinfo.project_type);
    Project.insert("Project_Size",project_info.baseinfo.project_size);
    Project.insert("Project_Unit",project_info.baseinfo.project_unit);
    Project.insert("Project_Remarks",project_info.baseinfo.project_remarks);
    Project.insert("Project_Arrangement",project_info.baseinfo.arrangement);
//    Project.insert("Project_Table_Num",project_info.baseinfo.project_tabledata.table_num);

//    for(int i=0; i<project_info.baseinfo.project_tabledata.celldata_list.count(); i++)
//    {
//        auto tmp_group = project_info.baseinfo.project_tabledata.celldata_list.at(i);
//        Project.insert(QString("Project_%1_Table").arg(i),tmp_group);
//    }
    //添加新的信息保存到文件中
    //检测目标信息获取检测目标信息有多少行 保存检测目标信息
    size=project_info.testture.count();
    Project.insert("testture_sizeof",size);
    size=0;
    for (auto teture:project_info.testture)
    {
        passageway.clear();
        passageway_name.clear();
        Project.insert("testture_TestTube_Name_"+QString("%1").arg(size),teture.TestTube_Name);

        for (auto pasgew:teture.Passageway)
        {
            passageway+=QString("%1").arg(pasgew)+",";
        }
        for (auto pasgewname:teture.Passageway_Name)
        {
            passageway_name+=pasgewname+",";
        }
        Project.insert("testture_Passageway_"+QString("%1").arg(size),passageway);
        Project.insert("testture_Passageway_Name_"+QString("%1").arg(size),passageway_name);
     //   qDebug()<<"保存信息 passageway="<<passageway<<"passageway_name="<<passageway_name<<"TestTube_Name="<<teture.TestTube_Name;
        size++;
    }
    //保存实验参数信息 TabWid_basic_parameter
    size=project_info.experpapa.count();
    Project.insert("experpapa_sizeof",size);
    for (int i=0;i<size;i++)
    {
         Project.insert("experpapa_Passageway_Name_"+ QString("%1").arg(i),project_info.experpapa.at(i).Passageway_Name);
         Project.insert("experpapa_TestTube_Name_"+   QString("%1").arg(i),project_info.experpapa.at(i).TestTube_Name);
         Project.insert("experpapa_Passageway_"+      QString("%1").arg(i),project_info.experpapa.at(i).Passageway);
         Project.insert("experpapa_Anatype_"+         QString("%1").arg(i),project_info.experpapa.at(i).Anatype);
         Project.insert("experpapa_Linestart_"+       QString("%1").arg(i),project_info.experpapa.at(i).Linestart);
         Project.insert("experpapa_Lineend_"+         QString("%1").arg(i),project_info.experpapa.at(i).Lineend);
         Project.insert("experpapa_Lineopti_"+        QString("%1").arg(i),project_info.experpapa.at(i).Lineopti);
         Project.insert("experpapa_Autovalue_"+       QString("%1").arg(i),project_info.experpapa.at(i).Autovalue);
         Project.insert("experpapa_Manualvalue_"+     QString("%1").arg(i),project_info.experpapa.at(i).Manualvalue);
         Project.insert("experpapa_Numberwave_"+      QString("%1").arg(i),project_info.experpapa.at(i).Numberwave);
    }
    //保存实验质控信息
    size=project_info.experresuedete.count();
    Project.insert("experresuedete_sizeof",size);
    for (int i=0;i<size;i++)
    {
         Project.insert("experresuedete_SampeType_"+ QString("%1").arg(i),project_info.experresuedete.at(i).SampeType);
         Project.insert("experresuedete_CheckObject_"+ QString("%1").arg(i),project_info.experresuedete.at(i).CheckObject);
         Project.insert("experresuedete_ObjectVale_"+ QString("%1").arg(i),project_info.experresuedete.at(i).ObjectVale);
         Project.insert("experresuedete_ObjectCompara_"+ QString("%1").arg(i),project_info.experresuedete.at(i).ObjectCompara);
         Project.insert("experresuedete_DataType_"+ QString("%1").arg(i),project_info.experresuedete.at(i).DataType);
         Project.insert("experresuedete_DataCompara_"+ QString("%1").arg(i),project_info.experresuedete.at(i).DataCompara);
         Project.insert("experresuedete_DataVale_"+ QString("%1").arg(i),project_info.experresuedete.at(i).DataVale);
         //以下数据无效
        // Project.insert("experresuedete_SampeType_"+ QString("%1").arg(i),project_info.experresuedete.at(i).ResueDete);
         Project.insert("experresuedete_Conclusion_"+ QString("%1").arg(i),project_info.experresuedete.at(i).Conclusion);
//         int rgb_r=project_info.experresuedete.at(i).Color.red();
//         int rgb_g=project_info.experresuedete.at(i).Color.green();
//         int rgb_b=project_info.experresuedete.at(i).Color.blue();

         Project.insert("experresuedete_Color_"+ QString("%1").arg(i),
                        QString("%1").arg(project_info.experresuedete.at(i).Color.red())+","+
                        QString("%1").arg(project_info.experresuedete.at(i).Color.green())+","+
                        QString("%1").arg(project_info.experresuedete.at(i).Color.blue()));
    }
    //保存样品质控信息 typeresuedete;
    size=project_info.typeresuedete.count();
    Project.insert("typeresuedete_sizeof",size);
    for (int i=0;i<size;i++)
    {
         Project.insert("typeresuedete_SampeType_"+     QString("%1").arg(i),project_info.typeresuedete.at(i).SampeType);
         Project.insert("typeresuedete_CheckObject_"+   QString("%1").arg(i),project_info.typeresuedete.at(i).CheckObject);
         Project.insert("typeresuedete_ObjectVale_"+    QString("%1").arg(i),project_info.typeresuedete.at(i).ObjectVale);
         Project.insert("typeresuedete_ObjectCompara_"+ QString("%1").arg(i),project_info.typeresuedete.at(i).ObjectCompara);
         Project.insert("typeresuedete_DataType_"+      QString("%1").arg(i),project_info.typeresuedete.at(i).DataType);
         Project.insert("typeresuedete_DataCompara_"+   QString("%1").arg(i),project_info.typeresuedete.at(i).DataCompara);
         Project.insert("typeresuedete_DataVale_"+      QString("%1").arg(i),project_info.typeresuedete.at(i).DataVale);

         Project.insert("typeresuedete_Conclusion_"+ QString("%1").arg(i),project_info.typeresuedete.at(i).Conclusion);
         Project.insert("typeresuedete_Color_"+ QString("%1").arg(i),
                        QString("%1").arg(project_info.typeresuedete.at(i).Color.red())+","+
                        QString("%1").arg(project_info.typeresuedete.at(i).Color.green())+","+
                        QString("%1").arg(project_info.typeresuedete.at(i).Color.blue()));
    }
 //保存结果质控信息 resueresuedete;
    size=project_info.resueresuedete.count();
    Project.insert("resueresuedete_sizeof",size);
    for (int i=0;i<size;i++)
    {
         Project.insert("resueresuedete_SampeType_"+     QString("%1").arg(i),project_info.resueresuedete.at(i).SampeType);
         Project.insert("resueresuedete_CheckObject_"+   QString("%1").arg(i),project_info.resueresuedete.at(i).CheckObject);
         Project.insert("resueresuedete_ObjectVale_"+    QString("%1").arg(i),project_info.resueresuedete.at(i).ObjectVale);
         Project.insert("resueresuedete_ObjectCompara_"+ QString("%1").arg(i),project_info.resueresuedete.at(i).ObjectCompara);
         Project.insert("resueresuedete_DataType_"+      QString("%1").arg(i),project_info.resueresuedete.at(i).DataType);
         Project.insert("resueresuedete_DataCompara_"+   QString("%1").arg(i),project_info.resueresuedete.at(i).DataCompara);
         Project.insert("resueresuedete_DataVale_"+      QString("%1").arg(i),project_info.resueresuedete.at(i).DataVale);
         //以下数据无效
         Project.insert("resueresuedete_Conclusion_"+    QString("%1").arg(i),project_info.resueresuedete.at(i).Conclusion);
         Project.insert("resueresuedete_Color_"+         QString("%1").arg(i),
                        QString("%1").arg(project_info.resueresuedete.at(i).Color.red())+","+
                        QString("%1").arg(project_info.resueresuedete.at(i).Color.green())+","+
                        QString("%1").arg(project_info.resueresuedete.at(i).Color.blue()));
    }
    //保存定量检测范围
    size=project_info.quanrange.count();
    Project.insert("quanrange_sizeof",size);
//    qDebug()<<"大小="<<size;
    for (int i=0;i<size;i++)
    {
         Project.insert("quanrange_"+QString("%1").arg(i),project_info.quanrange.at(i));
//         qDebug()<<"project_info.quanrange="<<project_info.quanrange.at(i);
    }
    //保存交叉干扰值
    size=project_info.crossinter.count();
    Project.insert("crossinter_sizeof",size);
//    qDebug()<<"crossinter_sizeof 大小="<<size;
    for (int i=0;i<size;i++)
    {
         Project.insert("crossinter_"+QString("%1").arg(i),project_info.crossinter.at(i));
//         qDebug()<<"project_info.crossinter="<<project_info.crossinter.at(i);
    }

    //保存高级规则
     Project.insert("seniorrull_Path",project_info.seniorrull.Path);
     Project.insert("seniorrull_Interpre",project_info.seniorrull.Interpre);

    auto            SaveAddr = QDir::currentPath() + "/Project/" + save_name + ".prj";
    QFile           LocalFile(SaveAddr);
    QJsonDocument   document(Project);

    if (LocalFile.open(QFile::WriteOnly | QFile::Truncate))
    {
        LocalFile.write(document.toJson(QJsonDocument::Compact));
        LocalFile.close();
    }

    return SaveAddr;
}
//读取程序数据，目标后缀为prj
Project_Info FileLocalSave::ProjectInfoLoad(QString Local_File_Addr)
{
    QFile LocalFile(Local_File_Addr);
    QJsonObject		jsonObject;
    QString passgew="";
    QStringList listpass;
    listpass.clear();
    int testture_sizeof=-1;
    QList<Q268PCR_TEST_TUBE> testture;//检测目标信息
    Q268PCR_TEST_TUBE tsture;
    Q268PCR_EXPER_PARA experpapa;//实验参数
    Q268PCR_RESUE_DETE resuedete;//结果判定


    testture.clear();

    if (LocalFile.open(QFile::ReadOnly | QIODevice::Text))
    {
        auto bytes = LocalFile.readAll();
        auto str = QString::fromUtf8(bytes);
        jsonObject = QJsonDocument::fromJson(str.toUtf8()).object();
    }

    Project_Info project_info;

    project_info.baseinfo.project_name      = jsonObject["Project_Name"].toString();
    project_info.baseinfo.project_version   = jsonObject["Project_Version"].toString();
    project_info.baseinfo.project_type      = jsonObject["Project_Type"].toInt();
    project_info.baseinfo.project_size      = jsonObject["Project_Size"].toInt();
    project_info.baseinfo.project_unit      = jsonObject["Project_Unit"].toString();
    project_info.baseinfo.project_remarks   = jsonObject["Project_Remarks"].toString();
    project_info.baseinfo.arrangement       = jsonObject["Project_Arrangement"].toInt();
    //project_info.baseinfo.project_tabledata.table_num = jsonObject["Project_Table_Num"].toInt();

//    for(int i=0; i<jsonObject.count()-7; i++)
//    {
//        QString obj_name = QString("Project_%1_Table").arg(i);
//        auto tmp_group = jsonObject[obj_name].toBool();
//        project_info.baseinfo.project_tabledata.celldata_list.insert(i,tmp_group);
//    }

    //获取检测目标信息
    testture_sizeof=jsonObject["testture_sizeof"].toInt();
  //  qDebug()<<"readAll testture_sizeof"<<testture_sizeof;
    for (int i=0;i<testture_sizeof;i++)
    {
        tsture.Passageway.clear();
        tsture.TestTube_Name="";
        tsture.Passageway_Name.clear();

        tsture.TestTube_Name=jsonObject["testture_TestTube_Name_"+QString("%1").arg(i)].toString();
      //  qDebug()<<"tsture.TestTube_Name"<<tsture.TestTube_Name;
        passgew=jsonObject["testture_Passageway_"+QString("%1").arg(i)].toString();
        listpass=passgew.split(',',QString::SkipEmptyParts);
       // qDebug()<<"Passageway passgew"<<passgew<<"listpass ="<<listpass;
        for (auto str:listpass)
        {
            tsture.Passageway.append(str.toInt());
        }
       // qDebug()<<"tsture.Passageway"<<tsture.Passageway;
        passgew=jsonObject["testture_Passageway_Name_"+QString("%1").arg(i)].toString();
        listpass=passgew.split(',', QString::SkipEmptyParts);
      //  qDebug()<<"Passageway passgew"<<passgew<<"listpass ="<<listpass;
        for (auto str:listpass)
        {
            tsture.Passageway_Name.append(str);
        }
        // qDebug()<<"tsture.Passageway_Name"<<tsture.Passageway_Name;
        project_info.testture.append(tsture);
    }
    //获取实验参数信息
    testture_sizeof=jsonObject["experpapa_sizeof"].toInt();
 //   qDebug()<<"readAll experpapa_sizeof"<<testture_sizeof;
    for(int i=0;i<testture_sizeof;i++)
    {
        experpapa.Passageway_Name= jsonObject["experpapa_Passageway_Name_"+QString("%1").arg(i)].toString();
        experpapa.TestTube_Name=   jsonObject["experpapa_TestTube_Name_"+QString("%1").arg(i)].toString();
        experpapa.Passageway=      jsonObject["experpapa_Passageway_"+QString("%1").arg(i)].toString();
        experpapa.Anatype=         jsonObject["experpapa_Anatype_"+QString("%1").arg(i)].toString();
        experpapa.Linestart=       jsonObject["experpapa_Linestart_"+QString("%1").arg(i)].toInt();
        experpapa.Lineend=         jsonObject["experpapa_Lineend_"+QString("%1").arg(i)].toInt();
        experpapa.Lineopti=        jsonObject["experpapa_Lineopti_"+QString("%1").arg(i)].toString();
        experpapa.Autovalue=       jsonObject["experpapa_Autovalue_"+QString("%1").arg(i)].toBool();
        experpapa.Manualvalue=     jsonObject["experpapa_Manualvalue_"+QString("%1").arg(i)].toDouble();
        experpapa.Numberwave=      jsonObject["experpapa_Numberwave_"+QString("%1").arg(i)].toString();
        project_info.experpapa.append(experpapa);
    }
    //获取实验质控;resuedete
    testture_sizeof=jsonObject["experresuedete_sizeof"].toInt();
 //   qDebug()<<"readAll experresuedete_sizeof"<<testture_sizeof;
    for(int i=0;i<testture_sizeof;i++)
    {
        resuedete.SampeType=     jsonObject["experresuedete_SampeType_"+QString("%1").arg(i)].toString();
        resuedete.CheckObject=   jsonObject["experresuedete_CheckObject_"+QString("%1").arg(i)].toString();
        resuedete.ObjectVale=    jsonObject["experresuedete_ObjectVale_"+QString("%1").arg(i)].toInt();
        resuedete.ObjectCompara= jsonObject["experresuedete_ObjectCompara_"+QString("%1").arg(i)].toString();
        resuedete.DataType=      jsonObject["experresuedete_DataType_"+QString("%1").arg(i)].toString();
        resuedete.DataCompara=   jsonObject["experresuedete_DataCompara_"+QString("%1").arg(i)].toString();
        resuedete.DataVale=      jsonObject["experresuedete_DataVale_"+QString("%1").arg(i)].toInt();
        resuedete.Conclusion=    jsonObject["experresuedete_Conclusion_"+QString("%1").arg(i)].toString();
        QString rgb=             jsonObject["experresuedete_Color_"+QString("%1").arg(i)].toString();
        listpass=rgb.split(',',QString::SkipEmptyParts);
        QColor solo(listpass.at(0).toInt(),listpass.at(1).toInt(),listpass.at(2).toInt());
        resuedete.Color=solo;
        project_info.experresuedete.append(resuedete);
    }
    //获取样品质控
    testture_sizeof=jsonObject["typeresuedete_sizeof"].toInt();
  //  qDebug()<<"readAll typeresuedete_sizeof"<<testture_sizeof;
    for(int i=0;i<testture_sizeof;i++)
    {
        resuedete.SampeType=     jsonObject["typeresuedete_SampeType_"+QString("%1").arg(i)].toString();
        resuedete.CheckObject=   jsonObject["typeresuedete_CheckObject_"+QString("%1").arg(i)].toString();
        resuedete.ObjectVale=    jsonObject["typeresuedete_ObjectVale_"+QString("%1").arg(i)].toInt();
        resuedete.ObjectCompara= jsonObject["typeresuedete_ObjectCompara_"+QString("%1").arg(i)].toString();
        resuedete.DataType=      jsonObject["typeresuedete_DataType_"+QString("%1").arg(i)].toString();
        resuedete.DataCompara=   jsonObject["typeresuedete_DataCompara_"+QString("%1").arg(i)].toString();
        resuedete.DataVale=      jsonObject["typeresuedete_DataVale_"+QString("%1").arg(i)].toInt();
        resuedete.Conclusion=    jsonObject["typeresuedete_Conclusion_"+QString("%1").arg(i)].toString();
        QString rgb=             jsonObject["typeresuedete_Color_"+QString("%1").arg(i)].toString();
        listpass=rgb.split(',',QString::SkipEmptyParts);
        QColor solo(listpass.at(0).toInt(),listpass.at(1).toInt(),listpass.at(2).toInt());
        resuedete.Color=solo;
        project_info.typeresuedete.append(resuedete);
    }
    //获取结果质控
    testture_sizeof=jsonObject["resueresuedete_sizeof"].toInt();
  //  qDebug()<<"readAll resueresuedete_sizeof"<<testture_sizeof;
    for(int i=0;i<testture_sizeof;i++)
    {
        resuedete.SampeType=     jsonObject["resueresuedete_SampeType_"+QString("%1").arg(i)].toString();
        resuedete.CheckObject=   jsonObject["resueresuedete_CheckObject_"+QString("%1").arg(i)].toString();
        resuedete.ObjectVale=    jsonObject["resueresuedete_ObjectVale_"+QString("%1").arg(i)].toInt();
        resuedete.ObjectCompara= jsonObject["resueresuedete_ObjectCompara_"+QString("%1").arg(i)].toString();
        resuedete.DataType=      jsonObject["resueresuedete_DataType_"+QString("%1").arg(i)].toString();
        resuedete.DataCompara=   jsonObject["resueresuedete_DataCompara_"+QString("%1").arg(i)].toString();
        resuedete.DataVale=      jsonObject["resueresuedete_DataVale_"+QString("%1").arg(i)].toInt();
        resuedete.Conclusion=    jsonObject["resueresuedete_Conclusion_"+QString("%1").arg(i)].toString();
        QString rgb=             jsonObject["resueresuedete_Color_"+QString("%1").arg(i)].toString();
        listpass=rgb.split(',',QString::SkipEmptyParts);
        QColor solo(listpass.at(0).toInt(),listpass.at(1).toInt(),listpass.at(2).toInt());
        resuedete.Color=solo;
        project_info.resueresuedete.append(resuedete);
    }

    //获取定量检测范围数据
    testture_sizeof=jsonObject["quanrange_sizeof"].toInt();
 //   qDebug()<<"readAll quanrange_sizeof"<<testture_sizeof;
    for(int i=0;i<testture_sizeof;i++)
    {
        passgew=jsonObject["quanrange_"+QString("%1").arg(i)].toString();
        project_info.quanrange.append(passgew);
    //    qDebug()<<"readAll passgew"<<passgew;
    }
    //获取交叉干扰
    testture_sizeof=jsonObject["crossinter_sizeof"].toInt();
    for(int i=0;i<testture_sizeof;i++)
    {

        passgew=jsonObject["crossinter_"+QString("%1").arg(i)].toString();
//        listpass=passgew.split(',',QString::SkipEmptyParts);
//        passgew="";
//        for (auto str:listpass)
//        {
//           passgew=passgew+str;
//        }
        project_info.crossinter.append(passgew);
     //   qDebug()<<"readAll project_info.crossinter"<<passgew;
    }



    //获取高级规则
    project_info.seniorrull.Path=     jsonObject["seniorrull_Path"].toString();
    project_info.seniorrull.Interpre= jsonObject["seniorrull_Interpre"].toString();


    return project_info;
}
*/
//保存实验数据，目标后缀为exp
QString FileLocalSave::ExperimentInfoSave(QString save_name, ExperimentData exp_data)
{
    QJsonObject Experiment;
    Experiment.insert("EquipmentType",exp_data.EquipmentType);
    Experiment.insert("EquipmentID",exp_data.EquipmentID);
    Experiment.insert("Exp_Name",exp_data.Exp_Name);
    Experiment.insert("Exp_SaveAddr",exp_data.Exp_SaveAddr);
    Experiment.insert("Exp_Type",QString::number(exp_data.Exp_Type));
    Experiment.insert("Exp_User",exp_data.Exp_User);
    Experiment.insert("Exp_UserCheck",exp_data.Exp_UserCheck);
    Experiment.insert("Exp_Remarks",exp_data.Exp_Remarks);
    Experiment.insert("Exp_Modules",QString::number(exp_data.Exp_Modules));
    Experiment.insert("Exp_Hotlid",exp_data.Exp_Hotlid);
    Experiment.insert("Exp_TemControl",QString::number(exp_data.Exp_TemControl));
    Experiment.insert("Exp_ChannelScan",QString::number(exp_data.Exp_ChannelScan));

    QJsonArray ExpTableArray;
    for(int i=0; i<(ROW*COL); i++)
    {
        QJsonObject ExpTable;
        ExpTable.insert("Exp_Table_Reapore",exp_data.Exp_Table[i].cell_unit_data.Reapore);
        ExpTable.insert("Exp_Table_CellName",exp_data.Exp_Table[i].cell_unit_data.CellName);
        ExpTable.insert("Exp_Table_Data_SampleType",QString::number(exp_data.Exp_Table[i].cell_unit_data.SampleType));
        ExpTable.insert("Exp_Table_SampleAttr",exp_data.Exp_Table[i].cell_unit_data.SampleAttr);
        ExpTable.insert("Exp_Table_Data_SampleName",exp_data.Exp_Table[i].cell_unit_data.SampleName);
        ExpTable.insert("Exp_Table_SampleRepeat",exp_data.Exp_Table[i].cell_unit_data.SampleRepeat);
        ExpTable.insert("Exp_Table_SampleRepeatID",QString::number(exp_data.Exp_Table[i].cell_unit_data.SampleRepeatID));

        ExpTable.insert("Exp_Table_SampleName",exp_data.Exp_Table[i].cell_unit_info.SampleName);
        ExpTable.insert("Exp_Table_ID",exp_data.Exp_Table[i].cell_unit_info.ID);
        ExpTable.insert("Exp_Table_Name",exp_data.Exp_Table[i].cell_unit_info.Name);
        ExpTable.insert("Exp_Table_Sex",exp_data.Exp_Table[i].cell_unit_info.Sex);
        ExpTable.insert("Exp_Table_Age",exp_data.Exp_Table[i].cell_unit_info.Age);
        ExpTable.insert("Exp_Table_SampleType",exp_data.Exp_Table[i].cell_unit_info.SampleType);
        ExpTable.insert("Exp_Table_SampleDate",exp_data.Exp_Table[i].cell_unit_info.SampleDate);
        ExpTable.insert("Exp_Table_DoctorName",exp_data.Exp_Table[i].cell_unit_info.DoctorName);
        ExpTable.insert("Exp_Table_DoctorRoom",exp_data.Exp_Table[i].cell_unit_info.DoctorRoom);
        ExpTable.insert("Exp_Table_DoctorID",exp_data.Exp_Table[i].cell_unit_info.DoctorID);
        ExpTable.insert("Exp_Table_DoctorRemarks",exp_data.Exp_Table[i].cell_unit_info.DoctorRemarks);
        ExpTable.insert("Exp_Table_Remarks",exp_data.Exp_Table[i].cell_unit_info.Remarks);

        ExpTable.insert("Exp_Table_ProperName",exp_data.Exp_Table[i].cell_unit_info.ProperName);
        ExpTable.insert("Exp_Table_Passage",exp_data.Exp_Table[i].cell_unit_info.Passage);
        ExpTable.insert("Exp_Table_DyesTuff",exp_data.Exp_Table[i].cell_unit_info.DyesTuff);
        ExpTable.insert("Exp_Table_Target",exp_data.Exp_Table[i].cell_unit_info.Target);
        ExpTable.insert("Exp_Table_Color",exp_data.Exp_Table[i].cell_unit_info.Color);
        ExpTable.insert("Exp_Table_Type",exp_data.Exp_Table[i].cell_unit_info.Type);
        ExpTable.insert("Exp_Table_Ct",exp_data.Exp_Table[i].cell_unit_info.Ct);
        ExpTable.insert("Exp_Table_AverCt",exp_data.Exp_Table[i].cell_unit_info.AverCt);
        ExpTable.insert("Exp_Table_CtSD",exp_data.Exp_Table[i].cell_unit_info.CtSD);
        ExpTable.insert("Exp_Table_CtCv",exp_data.Exp_Table[i].cell_unit_info.CtCv);

        ExpTable.insert("Exp_Table_Concen",exp_data.Exp_Table[i].cell_unit_info.Concen);
        ExpTable.insert("Exp_Table_AverConcen",exp_data.Exp_Table[i].cell_unit_info.AverConcen);
        ExpTable.insert("Exp_Table_CnSd",exp_data.Exp_Table[i].cell_unit_info.CnSd);
        ExpTable.insert("Exp_Table_CnCv",exp_data.Exp_Table[i].cell_unit_info.CnCv);
        ExpTable.insert("Exp_Table_Rn",exp_data.Exp_Table[i].cell_unit_info.Rn);
        ExpTable.insert("Exp_Table_InterRes",exp_data.Exp_Table[i].cell_unit_info.InterRes);
        ExpTable.insert("Exp_Table_Conclu",exp_data.Exp_Table[i].cell_unit_info.Conclu);
        ExpTable.insert("Exp_Table_BedNumber",exp_data.Exp_Table[i].cell_unit_info.BedNumber);
        ExpTable.insert("Exp_Table_AdmissNumber",exp_data.Exp_Table[i].cell_unit_info.AdmissNumber);
        ExpTable.insert("Exp_Table_OutpatNumber",exp_data.Exp_Table[i].cell_unit_info.OutpatNumber);

        ExpTableArray.append(ExpTable);
    }
    Experiment.insert("Exp_Table",ExpTableArray);

    auto            SaveAddr = QDir::currentPath() + "/Experiment/" + save_name + ".exp";
    QFile           LocalFile(SaveAddr);
    QJsonDocument   document(Experiment);

    if (LocalFile.open(QFile::WriteOnly | QFile::Truncate))
    {
        LocalFile.write(document.toJson(QJsonDocument::Compact));
        LocalFile.close();
    }

    return SaveAddr;
}
//读取实验数据，目标后缀为exp
ExperimentData FileLocalSave::ExperimentInfoLoad(QString Local_File_Addr)
{
    QFile LocalFile(Local_File_Addr);
    QJsonObject		jsonObject;

    if (LocalFile.open(QFile::ReadOnly | QIODevice::Text))
    {
        auto bytes = LocalFile.readAll();
        auto str = QString::fromUtf8(bytes);
        jsonObject = QJsonDocument::fromJson(str.toUtf8()).object();
    }

    ExperimentData m_Experiment;

    m_Experiment.EquipmentType = jsonObject["EquipmentType"].toString();
    m_Experiment.EquipmentID = jsonObject["EquipmentID"].toString();
    m_Experiment.Exp_Name = jsonObject["Exp_Name"].toString();
    m_Experiment.Exp_SaveAddr = jsonObject["Exp_SaveAddr"].toString();
    m_Experiment.Exp_Type = jsonObject["Exp_Type"].toString().toInt();
    m_Experiment.Exp_User = jsonObject["Exp_User"].toString();
    m_Experiment.Exp_UserCheck = jsonObject["Exp_UserCheck"].toString();
    m_Experiment.Exp_Remarks = jsonObject["Exp_Remarks"].toString();
    m_Experiment.Exp_Modules = jsonObject["Exp_Modules"].toString().toInt();
    m_Experiment.Exp_Hotlid = jsonObject["Exp_Hotlid"].toBool();
    m_Experiment.Exp_TemControl = jsonObject["Exp_TemControl"].toString().toInt();
    m_Experiment.Exp_ChannelScan = jsonObject["Exp_ChannelScan"].toString().toInt();

    QJsonArray ExpTableArray = jsonObject["Exp_Table"].toArray();
    for(int i=0; i<(ROW*COL); i++)
    {
        QJsonObject cell_unit = ExpTableArray.at(i).toObject();
         m_Experiment.Exp_Table[i].cell_unit_data.Reapore = cell_unit["Exp_Table_Reapore"].toString();
        m_Experiment.Exp_Table[i].cell_unit_data.CellName = cell_unit["Exp_Table_CellName"].toString();
        m_Experiment.Exp_Table[i].cell_unit_data.SampleType = cell_unit["Exp_Table_Data_SampleType"].toString().toInt();
        m_Experiment.Exp_Table[i].cell_unit_data.SampleAttr = cell_unit["Exp_Table_SampleAttr"].toString();
        m_Experiment.Exp_Table[i].cell_unit_data.SampleName = cell_unit["Exp_Table_Data_SampleName"].toString();
        m_Experiment.Exp_Table[i].cell_unit_data.SampleRepeat = cell_unit["Exp_Table_SampleRepeat"].toBool();
        m_Experiment.Exp_Table[i].cell_unit_data.SampleRepeatID = cell_unit["Exp_Table_SampleRepeatID"].toString().toInt();

        m_Experiment.Exp_Table[i].cell_unit_info.SampleName = cell_unit["Exp_Table_SampleName"].toString();
        m_Experiment.Exp_Table[i].cell_unit_info.ID = cell_unit["Exp_Table_ID"].toString();
        m_Experiment.Exp_Table[i].cell_unit_info.Name = cell_unit["Exp_Table_Name"].toString();
        m_Experiment.Exp_Table[i].cell_unit_info.Sex = cell_unit["Exp_Table_Sex"].toString();
        m_Experiment.Exp_Table[i].cell_unit_info.Age = cell_unit["Exp_Table_Age"].toString();
        m_Experiment.Exp_Table[i].cell_unit_info.SampleType = cell_unit["Exp_Table_SampleType"].toString();
        m_Experiment.Exp_Table[i].cell_unit_info.SampleDate = cell_unit["Exp_Table_SampleDate"].toString();
        m_Experiment.Exp_Table[i].cell_unit_info.DoctorName = cell_unit["Exp_Table_DoctorName"].toString();
        m_Experiment.Exp_Table[i].cell_unit_info.DoctorRoom = cell_unit["Exp_Table_DoctorRoom"].toString();
        m_Experiment.Exp_Table[i].cell_unit_info.DoctorID = cell_unit["Exp_Table_DoctorID"].toString();
        m_Experiment.Exp_Table[i].cell_unit_info.DoctorRemarks = cell_unit["Exp_Table_DoctorRemarks"].toString();
        m_Experiment.Exp_Table[i].cell_unit_info.Remarks = cell_unit["Exp_Table_Remarks"].toString();

        m_Experiment.Exp_Table[i].cell_unit_info.ProperName = cell_unit["Exp_Table_ProperName"].toString();
        m_Experiment.Exp_Table[i].cell_unit_info.Passage = cell_unit["Exp_Table_Passage"].toString();
        m_Experiment.Exp_Table[i].cell_unit_info.DyesTuff = cell_unit["Exp_Table_DyesTuff"].toString();
        m_Experiment.Exp_Table[i].cell_unit_info.Target = cell_unit["Exp_Table_Target"].toString();
        m_Experiment.Exp_Table[i].cell_unit_info.Color = cell_unit["Exp_Table_Color"].toString();
        m_Experiment.Exp_Table[i].cell_unit_info.Type = cell_unit["Exp_Table_Type"].toString();
        m_Experiment.Exp_Table[i].cell_unit_info.Ct = cell_unit["Exp_Table_Ct"].toString();
        m_Experiment.Exp_Table[i].cell_unit_info.AverCt = cell_unit["Exp_Table_AverCt"].toString();
        m_Experiment.Exp_Table[i].cell_unit_info.CtSD = cell_unit["Exp_Table_CtSD"].toString();
        m_Experiment.Exp_Table[i].cell_unit_info.CtCv = cell_unit["Exp_Table_CtCv"].toString();

        m_Experiment.Exp_Table[i].cell_unit_info.Concen = cell_unit["Exp_Table_Concen"].toString();
        m_Experiment.Exp_Table[i].cell_unit_info.AverConcen = cell_unit["Exp_Table_AverConcen"].toString();
        m_Experiment.Exp_Table[i].cell_unit_info.CnSd = cell_unit["Exp_Table_CnSd"].toString();
        m_Experiment.Exp_Table[i].cell_unit_info.CnCv = cell_unit["Exp_Table_CnCv"].toString();
        m_Experiment.Exp_Table[i].cell_unit_info.Rn = cell_unit["Exp_Table_Rn"].toString();
        m_Experiment.Exp_Table[i].cell_unit_info.InterRes = cell_unit["Exp_Table_InterRes"].toString();
        m_Experiment.Exp_Table[i].cell_unit_info.Conclu = cell_unit["Exp_Table_Conclu"].toString();
        m_Experiment.Exp_Table[i].cell_unit_info.BedNumber = cell_unit["Exp_Table_BedNumber"].toString();
        m_Experiment.Exp_Table[i].cell_unit_info.AdmissNumber = cell_unit["Exp_Table_AdmissNumber"].toString();
        m_Experiment.Exp_Table[i].cell_unit_info.OutpatNumber = cell_unit["Exp_Table_OutpatNumber"].toString();

    }

    return m_Experiment;
}
//工具界面下的软件选项保存sof
QString FileLocalSave::ToolSoftConfigSave(QString save_name, Q268PCR_TOOL_CONFIG exp_data)
{
    QJsonObject Experiment;

     Experiment.insert("Tool_config_Sortorder",exp_data.Sortorder);
     Experiment.insert("Tool_config_Dataselect",exp_data.Dataselect);
     Experiment.insert("Tool_config_Tablezoom",exp_data.Tablezoom);
     Experiment.insert("Tool_config_Sampleinter",exp_data.Sampleinter);
     Experiment.insert("Tool_config_Sciencoun",exp_data.Sciencoun);

     Experiment.insert("Tool_config_Ydispalymode",exp_data.Ydispalymode);
     Experiment.insert("Tool_config_Curvestyle",exp_data.Curvestyle);
     Experiment.insert("Tool_config_Amplcurvealgor",exp_data.Amplcurvealgor);
     Experiment.insert("Tool_config_Sampledisplaycurve",exp_data.Sampledisplaycurve);
     Experiment.insert("Tool_config_Amplcurvethres",exp_data.Amplcurvethres);

     Experiment.insert("Tool_config_Peakvalue",exp_data.Peakvalue);
     Experiment.insert("Tool_config_Peakinvwe",exp_data.Peakinvwe);
     Experiment.insert("Tool_config_Porechoose",exp_data.Porechoose);;
      Experiment.insert("Tool_config_Allowhole",exp_data.Allowhole);

     Experiment.insert("Tool_config_Passagechoose",exp_data.Passagechoose);
     Experiment.insert("Tool_config_Hotsetting",exp_data.Hotsetting);

     Experiment.insert("Tool_config_Datadisplay",exp_data.Datadisplay);
     Experiment.insert("Tool_config_Uiparameter",exp_data.Uiparameter);
     Experiment.insert("Tool_config_Customhole",exp_data.Customhole);
     Experiment.insert("Tool_config_Customexperdata",exp_data.Customexperdata);
     Experiment.insert("Tool_config_Autoexporyop",exp_data.Autoexporyop);
     Experiment.insert("Tool_config_Fileformat",exp_data.Fileformat);
    auto            SaveAddr = QDir::currentPath() + "/SoftConfig/" + save_name + ".sof";
    QFile           LocalFile(SaveAddr);
    QJsonDocument   document(Experiment);

    if (LocalFile.open(QFile::WriteOnly | QFile::Truncate))
    {
        LocalFile.write(document.toJson(QJsonDocument::Compact));
        LocalFile.close();
    }
    return SaveAddr;
}
//读取工具界面下的软件选项sof
Q268PCR_TOOL_CONFIG FileLocalSave::ToolSoftConfigLoad(QString Local_File_Addr)
{
    QFile LocalFile(Local_File_Addr);
    QJsonObject		jsonObject;

    if (LocalFile.open(QFile::ReadOnly | QIODevice::Text))
    {
        auto bytes = LocalFile.readAll();
        auto str = QString::fromUtf8(bytes);
        jsonObject = QJsonDocument::fromJson(str.toUtf8()).object();
    }

    Q268PCR_TOOL_CONFIG m_Experiment;
    m_Experiment.Sortorder=jsonObject["Tool_config_Sortorder"].toInt();
    m_Experiment.Dataselect=jsonObject["Tool_config_Dataselect"].toInt();
    m_Experiment.Tablezoom=jsonObject["Tool_config_Tablezoom"].toInt();
    m_Experiment.Sampleinter=jsonObject["Tool_config_Sampleinter"].toBool();
    m_Experiment.Sciencoun=jsonObject["Tool_config_Sciencoun"].toBool();

    m_Experiment.Ydispalymode=jsonObject["Tool_config_Ydispalymode"].toInt();
    m_Experiment.Curvestyle=jsonObject["Tool_config_Curvestyle"].toInt();
    m_Experiment.Amplcurvealgor=jsonObject["Tool_config_Amplcurvealgor"].toInt();
    m_Experiment.Sampledisplaycurve=jsonObject["Tool_config_Sampledisplaycurve"].toBool();
    m_Experiment.Amplcurvethres=jsonObject["Tool_config_Amplcurvethres"].toBool();

    m_Experiment.Peakvalue=jsonObject["Tool_config_Peakvalue"].toBool();
    m_Experiment.Peakinvwe=jsonObject["Tool_config_Peakinvwe"].toBool();
    m_Experiment.Porechoose=jsonObject["Tool_config_Porechoose"].toInt();
    m_Experiment.Allowhole=jsonObject["Tool_config_Allowhole"].toInt();
    m_Experiment.Passagechoose=jsonObject["Tool_config_Passagechoose"].toInt();
    m_Experiment.Hotsetting=jsonObject["Tool_config_Hotsetting"].toInt();

    m_Experiment.Datadisplay=jsonObject["Tool_config_Datadisplay"].toBool();
    m_Experiment.Uiparameter=jsonObject["Tool_config_Uiparameter"].toBool();
    m_Experiment.Customhole=jsonObject["Tool_config_Customhole"].toBool();
    m_Experiment.Customexperdata=jsonObject["Tool_config_Customexperdata"].toBool();
    m_Experiment.Autoexporyop=jsonObject["Tool_config_Autoexporyop"].toInt();
    m_Experiment.Fileformat=jsonObject["Tool_config_Fileformat"].toString();



    return m_Experiment;
}
