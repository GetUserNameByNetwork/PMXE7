//---------------------------------------------------------------------------

#ifndef LoginUserH
#define LoginUserH
#include <Classes.hpp>
#include <SysUtils.hpp>
#include <Dialogs.hpp>
#include <ADODB.hpp>
#include <DB.hpp>
#include <Datasnap.DBClient.hpp>
#include "BaseServerClass.h"
#include <Datasnap.DSAuth.hpp>
#include "MessageManage.h"
//---------------------------------------------------------------------------
class TLoginUser : public TDMBaseServerClass
{
private:
		TClientDataSet *FLoginListDataSet;
		TADOQuery *m_Query;
		TADOQuery *b_Query;
		TADOQuery *LogQuery;
		TMessageManage * FFMessageManage;
		TLoginUserRecord * curUser;
protected:
		void  CreateLoginListDataSet();
		void __fastcall	NotifySendMissMessage(TLoginUserRecord *user,TADOConnection *AccConn);
public:
		__fastcall TLoginUser(TComponent* Owner);
		__fastcall ~TLoginUser();
public:
		String  GetSysConnectionString();
	   //	bool  IsValidUser(String UserCode,String Password,String AccbookID,String &msg);
		bool  ChangePassword(String UserCode,String OldPassword,String NewPassword,String &msg);
		bool  UpdateCurUserCallbackClientID(String const Value);
		void  Logout();

		void  SetUsersToFollow(TJSONArray* Users);
		bool  SendMessageToFollow(String const Msg);
		bool  SendMessageToAll(String const Msg);
		bool  SendMessageToUser(String const Msg, String const UserID);
		bool  SendMessageToDepartment(String const Msg, String const DepartmentID);
		bool  SendMessageToGroup(String const Msg, String const GroupID);
		bool  SendCMDToFollow(String const cmdStr);
		bool  SendCMDToAll(String const cmdStr);
		bool  SendCMDToUser(String const cmdStr, String const UserID);
		bool  SendCMDToDepartment(String const cmdStr, String const DepartmentID);
		bool  SendCMDToGroup(String const cmdStr, String const GroupID);
		bool  SendMgrMessage(int itype, String const Msg);

		TStream*  GetAccList(); //�����������б�
		TStream*  GetCurUserInfo();
		TStream*  ListOfLoginUser();
		TStream*  ListOfAllLoginUser();
		TStream*  GetClassList();
		TStream*  GetAutoCodeFields();
		TStream*  GetModuleData();
		TStream*  RefreshGlobeVar();
		TStream*  GetModuleRight();
		 //���ز�Ʒ��Ϣ��ftp,mail,������Ϣ��ϵͳ�����Ӵ�
		TJSONObject*  GetAccInforJSON();
		TJSONObject*  GetAccListJSON();
		TJSONObject*  GetCurUserJSON();
		TJSONArray*   GetUserList();
		TJSONArray*   GetAllUserList();
		TJSONObject*  GetClassListJO();
		TJSONObject*  GetAutoCodeFieldsJO();
		TJSONObject*  GetModuleDataJO();
		TJSONObject*  RefreshGlobeVarJO();
		TJSONObject*  GetModuleRightJO();
		TJSONArray*   GetDepartmentAndUsersList();
		String  UserIdToName(String UserId);
		//����������Ϣ��Ϣ
		TJSONObject* GetServerInfo();
		TJSONObject* GetFtpSetting();
		TJSONObject* GetMailSetting();
		TJSONObject* GetSmsSetting();
		//���ع������������б�
		TJSONArray*  GetWorkFlowServerList();
		//���ض��ŷ�������Ϣ
		TJSONArray*  GetSmsServerList();
};
#endif