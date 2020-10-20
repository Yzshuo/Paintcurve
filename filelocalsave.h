#ifndef FILELOCALSAVE_H
#define FILELOCALSAVE_H

#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QDateTime>
#include <QDir>
#include <QFile>
#include"Globals.h"
#include "widgetprojecteditprogramgroup.h"
//#include "widgetprojectedit.h"
//#include "widgetexperimentedit.h"

class FileLocalSave
{
public:
    FileLocalSave();

    //程序保存和读取
    static QString                                  ProgramGroupSave(QString Local_File_Addr, QList<WidgetProjectEditProgramGroup*> Current_View_Group);
    static QList<WidgetProjectEditProgramGroup *>   ProgramGroupLoad(QString Local_File_Addr, QGraphicsScene* scene);
    //程序保存和读取包括溶解曲线
//    static QString                                  ProgramGroupSave(QString Local_File_Addr,
//                                                                     QList<WidgetProjectEditProgramGroup*> Current_View_Group,
//                                                                     int                 MeltFlag,
//                                                                     double               MeltStartTem,
//                                                                     double               MeltEndTem);
    //    static QList<WidgetProjectEditProgramGroup *>   ProgramGroupLoad(QString Local_File_Addr, QGraphicsScene* scene,
    //                                                                     int                 &MeltFlag,
    //                                                                     double               &MeltStartTem,
    //                                                                     double               &MeltEndTem);
    static QString                                  ProgramGroupSave(QString Local_File_Addr,
                                                                     QList<WidgetProjectEditProgramGroup*> Current_View_Group,
                                                                     DISSCURVE_TYPESELE typesele);
    static QList<WidgetProjectEditProgramGroup *>   ProgramGroupLoad(QString Local_File_Addr, QGraphicsScene* scene,DISSCURVE_TYPESELE &typesele);

    //项目信息保存和读取
  //  static QString                                  ProjectInfoSave(QString, Project_Info);
  //  static Project_Info                             ProjectInfoLoad(QString);
    //实验信息保存和读取
    static QString                                  ExperimentInfoSave(QString, ExperimentData);
    static ExperimentData                           ExperimentInfoLoad(QString);
    //工具界面下的软件选项
    static QString ToolSoftConfigSave(QString,Q268PCR_TOOL_CONFIG);
    static Q268PCR_TOOL_CONFIG ToolSoftConfigLoad(QString);
};

#endif // FILELOCALSAVE_H
