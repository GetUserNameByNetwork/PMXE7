//---------------------------------------------------------------------------

#pragma hdrstop

#include "CheckService.h"
#include "ServerCtl.h"
#include "SHConst.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
TCheckServiceProc::TCheckServiceProc(TDSServer *AServer,TJSONArray *AServerList,String AComputerIP,int AIndex)
{
   FServer=AServer;
   FServerList=AServerList;
   FComputerIP=AComputerIP;
   SIndex=AIndex;
}
void __fastcall TCheckServiceProc::Execute()   //���������ŷ�����Ϣ
{

   FreeOnTerminate=true;

  System::TMonitor::Enter(FServerList);
   try
   {
	   TJSONObject *jNode=(TJSONObject *)(FServerList->Items[SIndex]);
	   String FServerName=jNode->GetValue("Name")->Value();
	   int PreServiceStatus=jNode->GetValue("ServiceStatus")->Value().ToInt();
	   int ServiceStatus;
	   String status="Ȩ�޲���";
	   int retval=SCQueryServiceStatus(NULL,FServerName.c_str(),ServiceStatus);
	   if(retval!=0)
	   {
		  ServiceStatus=-1;
		  if(retval==2)
			status=L"δ��װ" ;
	   }
	   else //==0
	   {
		  switch(ServiceStatus)
		  {
			 case SERVICE_STOPPED:
			 status=L"��ֹͣ" ;
			 break;
			 case SERVICE_START_PENDING:
			 status=L"��������" ;
			 break;
			 case SERVICE_STOP_PENDING:
			 status=L"����ֹͣ";
			 break;
			 case SERVICE_RUNNING:
			 status=L"������" ;
			 break;
			 case SERVICE_CONTINUE_PENDING:
			 status=L"���ڼ���";
			 break;
			 case SERVICE_PAUSE_PENDING:
			 status=L"������ͣ" ;
			 break;
			 case SERVICE_PAUSED:
			 status=L"����ͣ" ;
			 break;
			 default:
			 status=L"δ֪" ;
			 break;

		  }
		}
	   if(PreServiceStatus!=ServiceStatus)
	   {    //�޸ķ�����״̬
			jNode->RemovePair("ServiceStatus");
			jNode->AddPair("ServiceStatus",ServiceStatus);
			//������Ϣ
			TJSONObject *JSONMsg = new  TJSONObject();
			JSONMsg->AddPair(new TJSONPair("messageType", "servercontrol"));
			JSONMsg->AddPair(new TJSONPair("computerip", FComputerIP));
			JSONMsg->AddPair(new TJSONPair("servername", FServerName));
			JSONMsg->AddPair(new TJSONPair("datetime", Now().FormatString("YYYY-MM-DD HH:MM:SS")));
			JSONMsg->AddPair(new TJSONPair("msg", status));
		   FServer->BroadcastMessage(SMessageChannel, SMsgCallback, JSONMsg);
	   }
   }
   __finally
   {
	 System::TMonitor::Exit(FServerList);
   }

}