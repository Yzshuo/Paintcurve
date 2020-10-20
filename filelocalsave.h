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

    //���򱣴�Ͷ�ȡ
    static QString                                  ProgramGroupSave(QString Local_File_Addr, QList<WidgetProjectEditProgramGroup*> Current_View_Group);
    static QList<WidgetProjectEditProgramGroup *>   ProgramGroupLoad(QString Local_File_Addr, QGraphicsScene* scene);
    //���򱣴�Ͷ�ȡ�����ܽ�����
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

    //��Ŀ��Ϣ����Ͷ�ȡ
  //  static QString                                  ProjectInfoSave(QString, Project_Info);
  //  static Project_Info                             ProjectInfoLoad(QString);
    //ʵ����Ϣ����Ͷ�ȡ
    static QString                                  ExperimentInfoSave(QString, ExperimentData);
    static ExperimentData                           ExperimentInfoLoad(QString);
    //���߽����µ����ѡ��
    static QString ToolSoftConfigSave(QString,Q268PCR_TOOL_CONFIG);
    static Q268PCR_TOOL_CONFIG ToolSoftConfigLoad(QString);
};

#endif // FILELOCALSAVE_H
