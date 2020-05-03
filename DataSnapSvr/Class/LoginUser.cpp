//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "LoginUser.h"
#include "FSServerContainer.h"
#include "SHConst.h"
#include "DecEnc.h"
#include "ChatRoomServer.h"
#include "XMLData.h"
#pragma package(smart_init)
//---------------------------------------------------------------------------
__fastcall TLoginUser::TLoginUser(TComponent* Owner)
	:TDMBaseServerClass(Owner)
{
	FLoginListDataSet   = new TClientDataSet(NULL);
	m_Query = new TADOQuery(NULL);
	m_Query->ParamCheck = false;
	m_Query->EnableBCD = false;
	m_Query->CommandTimeout = 60;

	b_Query = new TADOQuery(NULL);
	b_Query->ParamCheck = false;
	b_Query->EnableBCD = false;
	b_Query->CommandTimeout = 60;

	LogQuery = new TADOQuery(NULL);
	LogQuery->ParamCheck = false;

	m_Query->Connection = AccConnection;
	b_Query->Connection = SysConnection;
	LogQuery->Connection= 	b_Query->Connection;
	FFMessageManage=NULL;
	curUser = GetCurLoginUser();
}
//---------------------------------------------------------------------------
 void TLoginUser::CreateLoginListDataSet()
{
	FLoginListDataSet->Close();
	FLoginListDataSet->FieldDefs->Clear();

	TFieldDef *pNewDef = FLoginListDataSet->FieldDefs->AddFieldDef();
	pNewDef->Name = "ThreadSessionId";
	pNewDef->DataType = ftInteger;
	pNewDef->Required = true;

 	pNewDef = FLoginListDataSet->FieldDefs->AddFieldDef();
	pNewDef->Name = "CallBackClientID";
	pNewDef->DataType = ftString;
	pNewDef->Size = 40;

	pNewDef = FLoginListDataSet->FieldDefs->AddFieldDef();
	pNewDef->Name = "UserLoginCode";
	pNewDef->DataType = ftString;
	pNewDef->Size = 20;

	pNewDef = FLoginListDataSet->FieldDefs->AddFieldDef();
	pNewDef->Name = "UserID";
	pNewDef->DataType = ftString;
	pNewDef->Size = 40;

	pNewDef = FLoginListDataSet->FieldDefs->AddFieldDef();
	pNewDef->Name = "UserCode";
	pNewDef->DataType = ftString;
	pNewDef->Size = 40;

	pNewDef = FLoginListDataSet->FieldDefs->AddFieldDef();
	pNewDef->Name = "UserName";
	pNewDef->DataType = ftString;
	pNewDef->Size = 100;

	pNewDef = FLoginListDataSet->FieldDefs->AddFieldDef();
	pNewDef->Name = "IP";
	pNewDef->DataType = ftString;
	pNewDef->Size = 50;

	pNewDef = FLoginListDataSet->FieldDefs->AddFieldDef();
	pNewDef->Name = "ClientPort";
	pNewDef->DataType = ftString;
	pNewDef->Size = 50;

	pNewDef = FLoginListDataSet->FieldDefs->AddFieldDef();
	pNewDef->Name = "Protocol";
	pNewDef->DataType = ftString;
	pNewDef->Size = 50;

	pNewDef = FLoginListDataSet->FieldDefs->AddFieldDef();
	pNewDef->Name = "AccBookID";
	pNewDef->DataType = ftString;
	pNewDef->Size = 20;

	pNewDef = FLoginListDataSet->FieldDefs->AddFieldDef();
	pNewDef->Name = "LoginTime";
	pNewDef->DataType = ftDateTime;

	pNewDef = FLoginListDataSet->FieldDefs->AddFieldDef();
	pNewDef->Name = "AccDbType";
	pNewDef->DataType = ftInteger;

	pNewDef = FLoginListDataSet->FieldDefs->AddFieldDef();
	pNewDef->Name = "SysDbType";
	pNewDef->DataType = ftInteger;

	FLoginListDataSet->IndexDefs->Clear();
	FLoginListDataSet->IndexDefs->Add("tableindex", "ThreadSessionId", TIndexOptions() << ixPrimary << ixUnique << ixCaseInsensitive);
	FLoginListDataSet->CreateDataSet();
}
//---------------------------------------------------------------------------
__fastcall TLoginUser::~TLoginUser()
{
	delete FLoginListDataSet;
	delete m_Query;
	delete b_Query;
	delete LogQuery;
	if(FFMessageManage)
	 delete FFMessageManage;
	FFMessageManage=NULL;
}
//---------------------------------------------------------------------------
/*bool  TLoginUser::IsValidUser(String UserCode,String Password,String AccbookID,String &msg)
{
	return DMServerContainer->IsValidUser(UserCode,Password,AccbookID,msg);
}*/
//---------------------------------------------------------------------------
void  TLoginUser::Logout()
{
	TDSSession *threadSession = TDSSessionManager::Instance->GetThreadSession();
	if(threadSession)
		DMServerContainer->RemoveUser(threadSession->Id);
}
//---------------------------------------------------------------------------
String  TLoginUser::GetSysConnectionString()
{
	return DMServerContainer->SysADOConnection->ConnectionString;
}
//---------------------------------------------------------------------------
bool  TLoginUser::ChangePassword(String UserCode,String OldPassword,String NewPassword,String &msg)
{
	bool bresult = false;
	String OPassword = EncryptAES(OldPassword);
	String NPassword = EncryptAES(NewPassword);
	if(NewPassword.Trim().IsEmpty())
	{
		msg = "�¿����Ϊ�գ�";
		return bresult;
	}

	m_Query->Close();
	m_Query->SQL->Text= "select * from tbUser where Upper(UserIDCode)="+QuotedStr(UserCode.UpperCase())+" and UserPassword="+QuotedStr(OPassword);
	m_Query->Open();
	if(m_Query->RecordCount==1)
	{
		m_Query->Edit();
		m_Query->FieldValues["UserPassword"] = NPassword;
		m_Query->Post();
		bresult = true;
	}
	else
	{
		msg = "����������û������޸�ʧ��!";
	}
	return bresult;
}
//---------------------------------------------------------------------------
bool  TLoginUser::UpdateCurUserCallbackClientID(String const Value)
{
	String FCallbackIdTrimmed = Trim(Value);
	if(FCallbackIdTrimmed == EmptyStr) return false;
  //	TLoginUserRecord * curUser = GetCurLoginUser();
  	if(curUser==NULL) return false;
	curUser->CallBackClientID = FCallbackIdTrimmed;
   //	DMServerContainer->NotifyUserChange(curUser,true);
	DMServerContainer->NotifyUserChange(curUser,true);
	//����������Ϣ
	NotifySendMissMessage(curUser,m_Query->Connection);
	return true;
}
//---------------------------------------------------------------------------
void  TLoginUser::SetUsersToFollow(TJSONArray* Users)
{
   //	TLoginUserRecord * curUser = GetCurLoginUser();
	if(curUser==NULL||Users==NULL) return;
	TJSONArray* jUsers=(TJSONArray*) Users->Clone();
	TJSONString *jsonstring;
	curUser->FollowList->Clear();
	for(int i=0;i<jUsers->Count;i++)
	{
		jsonstring = (TJSONString*)jUsers->Items[i];
		curUser->FollowList->Add(jsonstring->Value());
	}
}
//---------------------------------------------------------------------------
bool  TLoginUser::SendMessageToFollow(String const Msg)
{
	bool result = false;
	if(FFMessageManage==NULL)
	   FFMessageManage = new TMessageManage(this);
	result = FFMessageManage->SendMessageToFollow(Msg);
	return result;
}
//---------------------------------------------------------------------------
bool  TLoginUser::SendMessageToAll(String const Msg)
{
	bool result = false;
	if(FFMessageManage==NULL)
	   FFMessageManage = new TMessageManage(this);
  result = FFMessageManage->SendMessageToAll(Msg);
  return result;
}
//---------------------------------------------------------------------------
bool  TLoginUser::SendMessageToUser(String const Msg, String const UserID)
{
	bool result = false;
	if(FFMessageManage==NULL)
	FFMessageManage = new TMessageManage(this);
	result = FFMessageManage->SendMessageToUser(Msg,UserID);
	return result;
}
//---------------------------------------------------------------------------
bool  TLoginUser::SendMessageToDepartment(String const Msg, String const DepartmentID)
{
	bool result = false;
	if(FFMessageManage==NULL)
	 FFMessageManage = new TMessageManage(this);
	result = FFMessageManage->SendMessageToDepartment(Msg,DepartmentID);
	return result;
}
//---------------------------------------------------------------------------
bool  TLoginUser::SendMessageToGroup(String const Msg, String const GroupID)
{
	bool result = false;
	if(FFMessageManage==NULL)
	 FFMessageManage = new TMessageManage(this);
	result = FFMessageManage->SendMessageToGroup(Msg,GroupID);
	return result;
}
//---------------------------------------------------------------------------
bool  TLoginUser::SendCMDToFollow(String const cmdStr)
{
	bool result = false;
	if(FFMessageManage==NULL)
	   FFMessageManage = new TMessageManage(this);
	result = FFMessageManage->SendCMDToFollow(cmdStr);
	return result;
}
//---------------------------------------------------------------------------
bool  TLoginUser::SendCMDToAll(String const cmdStr)
{
	bool result = false;
	if(FFMessageManage==NULL)
	  FFMessageManage = new TMessageManage(this);
	result = FFMessageManage->SendCMDToAll(cmdStr);
	return result;
}
//---------------------------------------------------------------------------
bool  TLoginUser::SendCMDToUser(String const cmdStr, String const UserID)
{
	bool result = false;
	if(FFMessageManage==NULL)
	  FFMessageManage = new TMessageManage(this);
	result = FFMessageManage->SendCMDToUser(cmdStr,UserID);
	return result;
}
//---------------------------------------------------------------------------
bool  TLoginUser::SendCMDToDepartment(String const cmdStr, String const DepartmentID)
{
	bool result = false;
	if(FFMessageManage==NULL)
		FFMessageManage = new TMessageManage(this);
	result = FFMessageManage->SendCMDToDepartment(cmdStr,DepartmentID);
	 return result;
}
//---------------------------------------------------------------------------
bool  TLoginUser::SendCMDToGroup(String const cmdStr, String const GroupID)
{
	bool result = false;
	if(FFMessageManage==NULL)
	 FFMessageManage = new TMessageManage(this);
	result = FFMessageManage->SendCMDToGroup(cmdStr,GroupID);
    return result;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
TStream*  TLoginUser::GetClassList()
{
	b_Query->Close();
	b_Query->SQL->Text = "select SyMCOwnCode,SyMCCode,SyMCTitle,SyMLTitle from SyMiddleClass left join SyMiddleLib on SyMiddleClass.SyMCOwnCode=SyMiddleLib.SyMLCode where SyMCSelfDefCode=1 order by SyMCOwnCode";
	b_Query->Open();
	return AdoDataSetToCDStream(b_Query);
//	TADODataSetStream *ADOStream = new TADODataSetStream(b_Query);
//	ADOStream->Position = 0;
//	return ADOStream;
}
//---------------------------------------------------------------------------
TJSONObject*  TLoginUser::GetClassListJO()
{
	String SQL = " select SyMCOwnCode,SyMCCode,SyMCTitle,SyMLTitle"
							 " from SyMiddleClass left join SyMiddleLib on SyMiddleClass.SyMCOwnCode=SyMiddleLib.SyMLCode"
							 " where SyMCSelfDefCode=1 order by SyMCOwnCode";
	return DataSetSysToJsonObject(SQL);
}
//---------------------------------------------------------------------------
TStream*  TLoginUser::GetAutoCodeFields()
{
	b_Query->Close();
	b_Query->SQL->Text = "select SyMFClassCode,SyMFFieldName,SyMFDataType,SyMFNo,SyMFTitle from SyMCMasterField where SyMFPartOfCode=1 order by SyMFClassCode,SyMFNo";
	b_Query->Open();
	return AdoDataSetToCDStream(b_Query);
//	TADODataSetStream *ADOStream = new TADODataSetStream(b_Query);
//	ADOStream->Position = 0;
//	return ADOStream;
}
//---------------------------------------------------------------------------
TJSONObject*  TLoginUser::GetAutoCodeFieldsJO()
{
	String SQL = " select SyMFClassCode,SyMFFieldName,SyMFDataType,SyMFNo,SyMFTitle"
							 " from SyMCMasterField where SyMFPartOfCode=1 order by SyMFClassCode,SyMFNo";
	return DataSetSysToJsonObject(SQL);
}
//---------------------------------------------------------------------------
TStream*  TLoginUser::GetModuleData()
{
   //	TLoginUserRecord *curUser = GetCurLoginUser();
	String SQL;
	switch (curUser->AccDbType)
	{
		case 1:
			SQL = "exec GetModule " + QuotedStr(curUser->UserCode);
			break;
		case 2:
			SQL = "{call PKG_Foresight_PMS.GetModule(" + QuotedStr(curUser->UserCode) + ")}";
			break;
	}
	m_Query->Close();
	m_Query->SQL->Text = SQL;
	m_Query->Open();
	return AdoDataSetToCDStream(m_Query);
//	TADODataSetStream *ADOStream = new TADODataSetStream(m_Query);
//	ADOStream->Position = 0;
//	return ADOStream;
}
//---------------------------------------------------------------------------
TJSONObject*  TLoginUser::GetModuleDataJO()
{
  //	TLoginUserRecord *curUser = GetCurLoginUser();
	String SQL;
	switch (curUser->AccDbType)
	{
		case 1:
			SQL = "exec GetModule " + QuotedStr(curUser->UserCode);
			break;
		case 2:
			SQL = "{call PKG_Foresight_PMS.GetModule(" + QuotedStr(curUser->UserCode) + ")}";
			break;
	}
	return DataSetToJsonObject(SQL);
}
//---------------------------------------------------------------------------
TStream*  TLoginUser::RefreshGlobeVar()
{
//	TLoginUserRecord *curUser = GetCurLoginUser();
	TMemoryStream *ms = new TMemoryStream();
	TMsgStream *sxs = new TMsgStream();
	try
	{
		sxs->ValueAttribute["status"]   = "s";
		sxs->ItemAttribute["accdbtype"] = curUser->AccDbType;
		sxs->ItemAttribute["username"]  = curUser->UserName;
		m_Query->Close();
		m_Query->SQL->Text = " select tbUser.*, tbemployeedata.* from tbUser"
												 " left join tbemployeedata on Upper(tbUser.UserIDCode)=Upper(tbemployeedata.EmpNum)"
												 " where Upper(UserLogonCode)=" + QuotedStr(curUser->UserLoginCode.UpperCase());
		m_Query->Open();
		sxs->ItemAttribute["qqid"]      = m_Query->FieldByName("UserLanguage")->AsString;
		sxs->ItemAttribute["qqpassword"]= m_Query->FieldByName("UserPassword")->AsString;
		sxs->ItemAttribute["usercode"]  = m_Query->FieldByName("UserIDCode")->AsString;
		sxs->ItemAttribute["userempid"] = m_Query->FieldByName("EmpPKID")->AsString;
		sxs->ItemAttribute["prjlevel"]  = DMServerContainer->PrjLevel;
		sxs->ItemAttribute["systemer"]  = 0;
		m_Query->Close();
		m_Query->SQL->Text ="select * from vi_SysGlobVar";
		m_Query->Open();
		sxs->LoadMasterFromStream(AdoDataSetToCDStream(m_Query));
		sxs->SaveToStream(ms);
		ms->Position = 0;
	}
	__finally
	{
		delete sxs;
	}
	return ms;
}
//---------------------------------------------------------------------------
TJSONObject*  TLoginUser::RefreshGlobeVarJO()
{
//  TLoginUserRecord *curUser = GetCurLoginUser();
	TJSONObject *JSONObject = new TJSONObject();
	JSONObject->AddPair(new TJSONPair("Status","s"));
	JSONObject->AddPair(new TJSONPair("AccDbType",curUser->AccDbType));
	JSONObject->AddPair(new TJSONPair("UserName",curUser->UserName));
	m_Query->Close();
	m_Query->SQL->Text = " select tbUser.*, tbemployeedata.* from tbUser"
											 " left join tbemployeedata on Upper(tbUser.UserIDCode)=Upper(tbemployeedata.EmpNum)"
											 " where Upper(UserLogonCode)=" + QuotedStr(curUser->UserLoginCode.UpperCase());
	m_Query->Open();
	if(m_Query->RecordCount > 0)
	{
		JSONObject->AddPair(new TJSONPair("QQID",m_Query->FieldByName("UserLanguage")->AsString));
		JSONObject->AddPair(new TJSONPair("QQPassword",m_Query->FieldByName("UserPassword")->AsString));
		JSONObject->AddPair(new TJSONPair("UserCode",m_Query->FieldByName("UserIDCode")->AsString));
		JSONObject->AddPair(new TJSONPair("UserEmpID",m_Query->FieldByName("EmpPKID")->AsString));
	}
	JSONObject->AddPair(new TJSONPair("PrjLevel",DMServerContainer->PrjLevel));
	JSONObject->AddPair(new TJSONPair("Systemer",0));
	TJSONObject *MstJson = DataSetToJsonObject("select * from vi_SysGlobVar");
	JSONObject->AddPair("Master",(TJSONObject*)(MstJson->Clone()));
	return JSONObject;
}
//---------------------------------------------------------------------------
TStream*  TLoginUser::GetModuleRight()
{
 //	TLoginUserRecord *curUser = GetCurLoginUser();
	String SQL;
	switch (curUser->AccDbType)
	{
		case 1:
			SQL = "exec GetRight " + QuotedStr(curUser->UserCode);
			break;
		case 2:
			SQL = "{call PKG_Foresight_PMS.GetRight(" + QuotedStr(curUser->UserCode) + ")}";
			break;
	}

	m_Query->Close();
	m_Query->SQL->Text = SQL;
	m_Query->Open();
	return AdoDataSetToCDStream(m_Query);
//	TADODataSetStream *ADOStream = new TADODataSetStream(m_Query);
//	ADOStream->Position = 0;
//	return ADOStream;
}
//---------------------------------------------------------------------------
TJSONObject*  TLoginUser::GetModuleRightJO()
{
//	TLoginUserRecord *curUser = GetCurLoginUser();
	String SQL;
	switch (curUser->AccDbType)
	{
		case 1:
			SQL = "exec GetRight " + QuotedStr(curUser->UserCode);
			break;
		case 2:
			SQL = "{call PKG_Foresight_PMS.GetRight(" + QuotedStr(curUser->UserCode) + ")}";
			break;
	}
	return DataSetToJsonObject(SQL);
}
//---------------------------------------------------------------------------
TStream*  TLoginUser::GetAccList()
{
	TMemoryStream* ms = new TMemoryStream();
	TMsgStream* msg = DMServerContainer->GetAccInformation();
	msg->SaveToStream(ms);
	ms->Position = 0;
	return ms;
}
//---------------------------------------------------------------------------
TJSONObject*  TLoginUser::GetAccListJSON()
{
	return DMServerContainer->GetAccListJSON();
}
//---------------------------------------------------------------------------
TStream*  TLoginUser::GetCurUserInfo()
{
 //	TLoginUserRecord *curUser = GetCurLoginUser();
	m_Query->Close();
	m_Query->SQL->Text = " select tbUser.*, tbemployeedata.* from tbUser"
											 " left join tbemployeedata on Upper(tbUser.UserIDCode)=Upper(tbemployeedata.EmpNum)"
											 " where Upper(UserLogonCode)=" + QuotedStr(curUser->UserLoginCode.UpperCase());
	m_Query->Open();
	return AdoDataSetToCDStream(m_Query);
//	TADODataSetStream *ADOStream = new TADODataSetStream(m_Query);
//	ADOStream->Position = 0;
//	return ADOStream;
}
//---------------------------------------------------------------------------
TJSONObject*  TLoginUser::GetCurUserJSON()
{
 //	TLoginUserRecord *curUser = GetCurLoginUser();
	if(curUser)
		return curUser->ObjectToJSON();
	else
		return NULL;
}
//---------------------------------------------------------------------------
TStream*  TLoginUser::ListOfLoginUser()
{
	CreateLoginListDataSet();
	DMServerContainer->ListOfLoginUser(FLoginListDataSet,false);
	TMemoryStream *cdStream=new TMemoryStream();
	FLoginListDataSet->SaveToStream(cdStream);
//	TADODataSetStream *ADOStream = new TADODataSetStream(FLoginListDataSet);
	cdStream->Position = 0;
	return cdStream;
}
//---------------------------------------------------------------------------
TStream*  TLoginUser::ListOfAllLoginUser()
{
	CreateLoginListDataSet();
	DMServerContainer->ListOfLoginUser(FLoginListDataSet);
	TMemoryStream *cdStream=new TMemoryStream();
	FLoginListDataSet->SaveToStream(cdStream);
//	TADODataSetStream *ADOStream = new TADODataSetStream(FLoginListDataSet);
	cdStream->Position = 0;
	return cdStream;
}
//---------------------------------------------------------------------------
TJSONArray*  TLoginUser::GetUserList()
{
	return DMServerContainer->GetLoginUserList(false);
}
//---------------------------------------------------------------------------
TJSONArray*  TLoginUser::GetAllUserList()
{
	return DMServerContainer->GetLoginUserList();
}
//---------------------------------------------------------------------------
TJSONArray*  TLoginUser::GetDepartmentAndUsersList()
{
	//ע�⣬�����㷨û�п���Blob�ֶΡ�
	TJSONArray *retArray = new TJSONArray();
	TJSONObject *itemJson;

	m_Query->Close();
	m_Query->SQL->Text = " select corpognpkid as id,corpogndeptname as name,corpognfkid_upper as parent,corpognsortcode as sortcode,corpogndeptnum as code,1 as type from tbcorporationorganize"
											 " union all"
											 " select emppkid as id,empname as name,(select corpognpkid from tbcorporationorganize where corpognpkid=empdeptcodefkid ) as parent,(select corpognsortcode+corpognpkid from tbcorporationorganize where corpognpkid=empdeptcodefkid)as sortcode,empnum as code,2 as type from tbemployeedata"
											 " order by sortcode,code";
	m_Query->Open();
	while(!m_Query->Eof)
	{
		itemJson = new TJSONObject();
		for(int i=0;i<m_Query->FieldCount;i++)
		{
			itemJson->AddPair(new TJSONPair(m_Query->Fields->Fields[i]->FieldName,m_Query->Fields->Fields[i]->AsString));
		}
		retArray->AddElement(itemJson);
		m_Query->Next();
	}
	return retArray;
}
//---------------------------------------------------------------------------
TJSONObject*  TLoginUser::GetAccInforJSON()
{
	return DMServerContainer->GetAccInforJSON();
}
//---------------------------------------------------------------------------
String TLoginUser::UserIdToName(String UserId)
{
	String UserName="";
	m_Query->Close();
	m_Query->SQL->Text = " select EmpName from tbEmployeeData where EmpPKID ='"+UserId+"'";
	m_Query->Open();
	if(m_Query->RecordCount>0)
		UserName = m_Query->FieldByName("EmpName")->AsString;
	return UserName;
}
//---------------------------------------------------------------------------
bool TLoginUser::SendMgrMessage(int itype, String const Msg)
{
	bool result = false;
	if(FFMessageManage==NULL)
	   FFMessageManage = new TMessageManage(this);
	result = FFMessageManage->SendMgrMessage(itype,Msg);
	return result;
}
//---------------------------------------------------------------------------
//������������Ϣ
//[TRoleAuth('admin')]
TJSONObject* TLoginUser::GetServerInfo()
{
	TJSONObject *elmObject=new TJSONObject();
	elmObject->AddPair(new TJSONPair("TCPIPPort", DMServerContainer->TCPIPPort));
	elmObject->AddPair(new TJSONPair("HttpPort", DMServerContainer->HttpPort));
	elmObject->AddPair(new TJSONPair("HttpsPort", DMServerContainer->HttpsPort));
	elmObject->AddPair(new TJSONPair("DbType", DMServerContainer->DbType));
	elmObject->AddPair(new TJSONPair("PrjLevel", DMServerContainer->PrjLevel));
	elmObject->AddPair(new TJSONPair("MaxUserCount", DMServerContainer->MaxUserCount));
	elmObject->AddPair(new TJSONPair("LogUnigue", DMServerContainer->LogUnigue));
	elmObject->AddPair(new TJSONPair("RemainTimes", DMServerContainer->RemainTimes));
	elmObject->AddPair(new TJSONPair("Version", DMServerContainer->Version));
	elmObject->AddPair(new TJSONPair("SysTag", DMServerContainer->SysTag));
	elmObject->AddPair(new TJSONPair("Trial", DMServerContainer->Trial? "true":"false"));
	elmObject->AddPair(new TJSONPair("SoftDog", DMServerContainer->SoftDog? "true":"false"));
	elmObject->AddPair(new TJSONPair("TestOverTimes", DMServerContainer->TestOverTimes? "true":"false"));
	elmObject->AddPair(new TJSONPair("BeginRunTime", DMServerContainer->BeginRunTime));
	elmObject->AddPair(new TJSONPair("DevEndDate", DMServerContainer->DevEndDate));
	return elmObject;
}
//---------------------------------------------------------------------------
//���ع�����������Ϣ��Ϣ
//[TRoleAuth('admin')]
TJSONObject* TLoginUser::GetFtpSetting()
{
	TJSONObject *elmObject=new TJSONObject();
	// ftp
	elmObject->AddPair(new TJSONPair("ftpInteranetIP", DMServerContainer->ftpInteranetIP));
	elmObject->AddPair(new TJSONPair("ftpInternetIP", DMServerContainer->ftpInternetIP));
	elmObject->AddPair(new TJSONPair("ftpUserName", DMServerContainer->ftpUserName));
	elmObject->AddPair(new TJSONPair("ftpPassword", DMServerContainer->ftpPassword));
	elmObject->AddPair(new TJSONPair("ftpPort", DMServerContainer->ftpPort));
	elmObject->AddPair(new TJSONPair("ftpNeedFTP", DMServerContainer->ftpNeedFTP ? 1 : 0));
	elmObject->AddPair(new TJSONPair("ftpPassive", DMServerContainer->ftpPassive ? 1 : 0));
	elmObject->AddPair(new TJSONPair("updateurl", DMServerContainer->UpdateURL));
	return elmObject;
}
//---------------------------------------------------------------------------
//
//[TRoleAuth('admin')]
TJSONObject* TLoginUser::GetMailSetting()
{
	TJSONObject *elmObject=new TJSONObject();
	// mail
	elmObject->AddPair(new TJSONPair("mailServer", DMServerContainer->MailServer));
	elmObject->AddPair(new TJSONPair("sendEmail", DMServerContainer->ESendMail));
	elmObject->AddPair(new TJSONPair("mailUserName", DMServerContainer->MailUserName));
	elmObject->AddPair(new TJSONPair("mailPassword", DMServerContainer->MailPassword));
	elmObject->AddPair(new TJSONPair("mailPort", DMServerContainer->MailPort));
	return elmObject;
}
//---------------------------------------------------------------------------
//
//���ط������б�
//[TRoleAuth('admin')]
TJSONArray*  TLoginUser::GetWorkFlowServerList()
{
  return DMServerContainer->GetWorkFlowServerList();
}
//---------------------------------------------------------------------------
//���ط������б�
//[TRoleAuth('admin')]
TJSONArray*  TLoginUser::GetSmsServerList()
{
  return DMServerContainer->GetSmsServerList();
}
//---------------------------------------------------------------------------
//[TRoleAuth('admin')]
TJSONObject* TLoginUser::GetSmsSetting()
{
	TJSONValue * RetJson;
	bool retNotify=false;
	TList__1<String> *CallbackIDList=DMServerContainer->FSDSServer->GetAllChannelClientId(SSmsCallback);
	TJSONObject * SendJson=new TJSONObject();
	SendJson->AddPair(new TJSONPair("notificationType", "sms"));
	SendJson->AddPair(new TJSONPair("msgtype", "setting"));  //���ò���
	SendJson->AddPair(new TJSONPair("to", "smsserver"));
	for (int i=0; i < CallbackIDList->Count; i++)
	{
		String smsCallBackClientID=(String)CallbackIDList->Items[i];
		retNotify=DMServerContainer->FSDSServer->NotifyCallback(smsCallBackClientID, SSmsCallback,(TJSONObject *)SendJson->Clone(), RetJson);
		if(retNotify)//���ͳɹ������
		{
		  if(RetJson!=NULL)
			  return (TJSONObject *)RetJson;
		}
	}
	return (TJSONObject *)(new TJSONObject());
}
//---------------------------------------------------------------------------
void __fastcall	TLoginUser::NotifySendMissMessage(TLoginUserRecord *user,TADOConnection *AccConn)
{
   TSendMissMessageThread *newSendMissMessageThread=new TSendMissMessageThread(DMServerContainer->FSDSServer, user,AccConn);
}
//---------------------------------------------------------------------------