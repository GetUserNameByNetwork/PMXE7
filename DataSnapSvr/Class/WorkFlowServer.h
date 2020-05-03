//---------------------------------------------------------------------------

#ifndef WorkFlowServerH
#define WorkFlowServerH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <SysUtils.hpp>
#include <Dialogs.hpp>
#include <ADODB.hpp>
#include <DB.hpp>
#include "BaseServerClass.h"
#include "wsClasses.hpp"
#include "wsDB.hpp"
#include "wsAccAndSysDB.hpp"
#include "wsMain.hpp"
//---------------------------------------------------------------------------
class TWorkFlowServer : public TDMBaseServerClass
{
private:	// User declarations
		TWorkflowInstance* __fastcall GetWorkIns(String FFlowDefID,String WorkInsID);
		void __fastcall InitWorkFlowStudio();
		TWorkflowStudio *WorkflowStudio;
		TWorkflowAccAndSysDB *WorkflowAccAndSysDB;
		TLoginUserRecord *curUser;
private:	// User declarations
		TADOQuery *m_Query;
public:		// User declarations
		__fastcall TWorkFlowServer(TComponent* Owner);
		__fastcall ~TWorkFlowServer();
		//���ص�ǰ������ID
		String ServerWorkFlowStartJSON(TJSONObject* jsonValue);
		String ServerWorkFlowStart(String FlowDefID, String Subject, String Content, String BusWindow, String BusCode, int Rank, String PrjID, String Report);
		//�����Ƿ���óɹ�
		bool  ServerWorkFlowExecJSON(TJSONObject* jsonValue);
		bool  ServerWorkFlowExec(String FlowDefID, String WorkInsID, String TaskInsID, String TskStatus, String TskComments, TJSONArray *VariableList);
		TJSONObject* GetTaskList(String AUserID, int ACompletedState, String AOtherCondition);// AUserID:��ѯԱ��ID��ACompletedState����ѯ����״̬��0-���죬1-����ɣ�2-ȫ����AOtherCondition����ѯ����������
		String GetLogList(int ALogFilterType, String AFilterKey); //ALogFilterType����־���ͣ�1-������־��2-������־��AFilterKey������ʵ��ID������ʵ��ID
		String GetDiagram(int ADiagramType, String ADiagramKey);//ADiagramType:��ѯ����ͼ���ͣ�0-���̶��壬1-����ʵ����2-����ʵ����ADiagramKey:���̶���ID������ʵ��ID��������ʵ��ID��
		TJSONObject* GetTaskdefList(String TaskInsID);
		TJSONObject* GetVariableList(String TaskInsID);
		TStream* GetVariableListDS(String TaskInsID);
		TStream* GetTaskdefListDS(String TaskInsID);
		///����Id��isDef��ʾ�Ƕ���Ļ���ʵ���ġ� ����JPG��ʽ��
		TStream* GetWorkFlowChart(String FlowID,bool isDef);
		TStream* GetTaskListDS(String AUserID, int ACompletedState, String AOtherCondition);
		bool TaskCanUpdate(String TaskInsID);
};
//---------------------------------------------------------------------------
extern PACKAGE TWorkFlowServer *DMWorkFlowServer;
//---------------------------------------------------------------------------
#endif