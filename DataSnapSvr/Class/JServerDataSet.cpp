//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "JServerDataSet.h"
#include "BaseFunc.h"
#include "SHConst.h"
#include "XMLData.h"
#include "DBJson.hpp"
#include "FSServerContainer.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
__fastcall TJServerDataSet::TJServerDataSet(TComponent* Owner):TDMBaseServerClass(Owner)
{
  m_Active   = false; // �ر����ݼ�
  m_Status   = -1;    // -1-��δ��ʼ��,0-׼����ʼ��,1-�ѳ�ʼ��
  FSysDbType = 1;
  FAccDbType = 1;

	m_MasterDataSet = new TADODataSet(NULL);
  m_MasterDataSet->ParamCheck = false;
	m_MasterDataSet->EnableBCD  = false;
	m_MasterDataSet->LockType   = ltBatchOptimistic;

  m_DetailDataSet = new TADODataSet(NULL);
  m_DetailDataSet->ParamCheck = false;
  m_DetailDataSet->EnableBCD  = false;
  m_DetailDataSet->LockType   = ltBatchOptimistic;
  m_MasterLinkFields = new TStringList;
	m_DetailLinkFields = new TStringList;

  m_StoredProc = new TADOStoredProc(NULL);
  m_StoredProc->EnableBCD = false;

  m_BuildQuery = new TADOQuery(NULL);
  m_BuildQuery->ParamCheck = false;
	m_BuildQuery->EnableBCD  = false;

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

	m_Connection             = this->AccConnection;
	m_StoredProc->Connection = m_Connection;
	m_Query->Connection      = m_Connection;

	m_BuildConn              = this->SysConnection;
	m_BuildQuery->Connection = m_BuildConn;
	b_Query->Connection      = m_BuildConn;
	LogQuery->Connection     = m_BuildConn;

  m_MasterSQLString   = "";
  m_DetailSQLString   = "";
  m_MasterWhereString = "";
  m_DetailWhereString = "";
  m_MasterOrderString = "";
  m_DetailOrderString = "";
  m_MasterGroupString = "";
  m_DetailGroupString = "";

  m_MasterKeyFields    = "";
  m_DetailKeyFields    = "";
  m_MasterDisplayName  = "";
  m_DetailDisplayName  = "";
  m_MasterDisplayField = "";
  m_DetailDisplayName  = "";

  TLoginUserRecord *curLoginUser = GetCurLoginUser();
  if(curLoginUser)
  {
		FSysDbType = curLoginUser->SysDbType;
		FAccDbType = curLoginUser->AccDbType;
		FUserCode  = curLoginUser->UserLoginCode;
		FUserIP    = curLoginUser->IP;
		FAcckBook  = curLoginUser->AccBookID;
  }
}
//---------------------------------------------------------------------------
__fastcall TJServerDataSet::~TJServerDataSet()
{
	delete m_Query;
	delete b_Query;
	delete LogQuery;
  delete m_MasterDataSet;
	delete m_StoredProc;
  delete m_BuildQuery;
	delete m_DetailDataSet;
	delete m_MasterLinkFields;
	delete m_DetailLinkFields;
}
//---------------------------------------------------------------------------
void  TJServerDataSet::InitServerDataSet(String ClassCode)
{
	String sqlstr, sqlstr1, sendbuff, retstr;
	int index, endpos; ;
	String s1, s2;
	m_ClassCode = ClassCode;
	try
	{
		m_BuildQuery->Close();
		m_BuildQuery->SQL->Clear();
		m_BuildQuery->SQL->Add("select * from SyMiddleClass where SyMCCode='"+m_ClassCode+"'");
		m_BuildQuery->Open();
		if(m_BuildQuery->RecordCount < 1)
		{
			m_BuildQuery->Close();
			throw Exception("��δ��������Ӧ�÷���������");
		}

		m_DataSetType            = m_BuildQuery->FieldByName("SyMCType")->AsInteger;
		m_MasterSQLString        = m_BuildQuery->FieldByName("SyMCMasterSQL")->AsString.Trim();
		m_MasterFixedWhereString = m_BuildQuery->FieldByName("SyMCMasterFixCondition")->AsString.Trim();
		m_MasterWhereString      = m_BuildQuery->FieldByName("SyMCMasterCondition")->AsString.Trim();
		m_MasterFixedOrderString = m_BuildQuery->FieldByName("SyMCMasterFixOrder")->AsString.Trim();
		m_MasterOrderString      = m_BuildQuery->FieldByName("SyMCMasterOrder")->AsString.Trim();
		m_MasterFixedGroupString = m_BuildQuery->FieldByName("SyMCMasterFixGroup")->AsString.Trim();
		m_MasterGroupString      = m_BuildQuery->FieldByName("SyMCMasterGroup")->AsString.Trim();
		m_MasterKeyFields        = m_BuildQuery->FieldByName("SyMCMasterKeyFields")->AsString.Trim();

		if(m_DataSetType == 2)
		{
			m_DetailSQLString        = m_BuildQuery->FieldByName("SyMCDetailSQL")->AsString.Trim();
			m_DetailFixedWhereString = m_BuildQuery->FieldByName("SyMCDetailFixCondition")->AsString.Trim();
			m_DetailWhereString      = m_BuildQuery->FieldByName("SyMCDetailCondition")->AsString.Trim();
			m_DetailFixedOrderString = m_BuildQuery->FieldByName("SyMCDetailFixOrder")->AsString.Trim();
			m_DetailOrderString      = m_BuildQuery->FieldByName("SyMCDetailOrder")->AsString.Trim();
			m_DetailFixedGroupString = m_BuildQuery->FieldByName("SyMCDetailFixGroup")->AsString.Trim();
			m_DetailGroupString      = m_BuildQuery->FieldByName("SyMCDetailGroup")->AsString.Trim();
			m_DetailKeyFields        = m_BuildQuery->FieldByName("SyMCDetailKeyFields")->AsString.Trim();
			m_BuildQuery->Close();
			m_BuildQuery->SQL->Clear();
			m_BuildQuery->SQL->Add("select * from SyMCRelaField where SyMCRFClassCode='"+m_ClassCode+"'");
			m_BuildQuery->Open();
			if(m_BuildQuery->RecordCount < 1)
			{
				m_BuildQuery->Close();
				throw Exception("��δ�����ӱ�Ӧ�÷���������");
			}

			m_BuildQuery->First();
			while(!m_BuildQuery->Eof)
			{
				m_MasterLinkFields->Add(m_BuildQuery->FieldByName("SyRFMasterField")->AsString.Trim());
				m_DetailLinkFields->Add(m_BuildQuery->FieldByName("SyMCRFDetailField")->AsString.Trim());
				m_BuildQuery->Next();
			}

			m_BuildQuery->Close();
			m_BuildQuery->SQL->Text = " select SyDFFieldName,SyDFTitle,SyDFIsQuery,SyDFFieldType,SyMCExpandFieldSQL,SyMCExpandFieldType"
																" from SyMCDetailField left outer join SyMCExpandField on SyDFFieldType=SyMCExpandFieldCode"
																" where  SyDFClassCode='" + m_ClassCode + "'"
																" order by SyDFNo";
			m_BuildQuery->Open();
			m_BuildQuery->First();
			while(!m_BuildQuery->Eof)
			{
				m_DetailDisplayName  += m_BuildQuery->FieldByName("SyDFTitle")->AsString + "\r\n";
				m_DetailDisplayField += m_BuildQuery->FieldByName("SyDFFieldName")->AsString + "\r\n";
				m_DetailFilterFields += m_BuildQuery->FieldByName("SyDFIsQuery")->AsString + "\r\n";
				m_DetailFieldAttr    += m_BuildQuery->FieldByName("SyDFFieldType")->AsString + "\r\n";
				if(m_BuildQuery->FieldByName("SyMCExpandFieldSQL")->IsNull)
					s1 = "";
				else
					s1 = m_BuildQuery->FieldByName("SyMCExpandFieldSQL")->AsString;
				s2.SetLength(4);
				swprintf(s2.c_str(), L"%04d", s1.Length());
				m_DetailFieldExpr += s2 + s1;
				s1 = "";
				s2 = "";
				if(m_BuildQuery->FieldByName("SyMCExpandFieldType")->IsNull)
					m_DetailAttrType += "0\r\n";
				else
					m_DetailAttrType += m_BuildQuery->FieldByName("SyMCExpandFieldType")->AsString + "\r\n";
				m_BuildQuery->Next();
			}
		}

		m_BuildQuery->Close();
		m_BuildQuery->SQL->Text = " select SyMFFieldName,SyMFTitle,SyMFIsQuery,SyMFFieldType,SyMCExpandFieldSQL,SyMCExpandFieldType"
															" from SyMCMasterField left outer join SyMCExpandField on SyMFFieldType=SyMCExpandFieldCode"
															" where SyMFClassCode='" + m_ClassCode + "'"
															" order by SyMFNo";
		m_BuildQuery->Open();
		m_BuildQuery->First();
		while(!m_BuildQuery->Eof)
		{
			m_MasterDisplayName  += m_BuildQuery->FieldByName("SyMFTitle")->AsString + "\r\n";
			m_MasterDisplayField += m_BuildQuery->FieldByName("SyMFFieldName")->AsString + "\r\n";
			m_MasterFilterFields += m_BuildQuery->FieldByName("SyMFIsQuery")->AsString + "\r\n";
			m_MasterFieldAttr    += m_BuildQuery->FieldByName("SyMFFieldType")->AsString + "\r\n";
			// **�Ժ�Ҫ�ĳ����û������޹ص�ϵͳ���ݽṹ����,��������ֶ���������(SyMCExpandFieldSQL)
			if(m_BuildQuery->FieldByName("SyMCExpandFieldSQL")->IsNull)
				s1 = "";
			else
				s1 = m_BuildQuery->FieldByName("SyMCExpandFieldSQL")->AsString;
			s2.SetLength(4);
			swprintf(s2.c_str(), L"%04d", s1.Length());
			m_MasterFieldExpr += s2 + s1;
			s1 = "";
			s2 = "";
			if(m_BuildQuery->FieldByName("SyMCExpandFieldType")->IsNull)
				m_MasterAttrType += "0\r\n";
			else
				m_MasterAttrType += m_BuildQuery->FieldByName("SyMCExpandFieldType")->AsString + "\r\n";
			m_BuildQuery->Next();
		}
		m_BuildQuery->Close();

		if(m_MasterSQLString.Trim().IsEmpty())
		{
			throw Exception("����SQL�����δ��ֵ.");
		}

		if(m_DataSetType == 2)
		{
			if(m_DetailSQLString.Trim().IsEmpty())
			{
				throw Exception("�ӱ�SQL�����δ��ֵ.");
			}

			if(m_MasterLinkFields->Count == 0)
			{
				throw Exception("��δ���������ֶ�.");
			}
		}
		m_Status = 1;
	}
	catch (Exception &e)
	{
		throw Exception("��ʼ�����������ݼ�����\r\n" + e.Message);
	}
}
//---------------------------------------------------------------------------
TJSONObject*  TJServerDataSet::GetInitResult(String ClassCode)
{
	TJSONObject *JSONObject = new TJSONObject();
	try
	{
		InitServerDataSet(ClassCode);

		JSONObject->AddPair(new TJSONPair("Operate",asCreateSDataSet));                         //0
		JSONObject->AddPair(new TJSONPair("Status","s"));                                       //1
		JSONObject->AddPair(new TJSONPair("DataSetType",m_DataSetType));                        //2
		JSONObject->AddPair(new TJSONPair("MasterKeyFields",m_MasterKeyFields));                //3
		JSONObject->AddPair(new TJSONPair("MasterDisplayName",m_MasterDisplayName));            //4
		JSONObject->AddPair(new TJSONPair("MasterDisplayField",m_MasterDisplayField));          //5
		JSONObject->AddPair(new TJSONPair("MasterFilterFields",m_MasterFilterFields));          //6
		JSONObject->AddPair(new TJSONPair("MasterFieldAttr",m_MasterFieldAttr));                //7
		JSONObject->AddPair(new TJSONPair("MasterAttrType",m_MasterAttrType));                  //8
		JSONObject->AddPair(new TJSONPair("MasterFieldExpr",m_MasterFieldExpr));                //9
		JSONObject->AddPair(new TJSONPair("MasterSQLString",m_MasterSQLString));                //10
		JSONObject->AddPair(new TJSONPair("MasterFixedWhereString",m_MasterFixedWhereString));  //11
		JSONObject->AddPair(new TJSONPair("MasterFixedOrderString",m_MasterFixedOrderString));  //12
		JSONObject->AddPair(new TJSONPair("MasterFixedGroupString",m_MasterFixedGroupString));  //13

		// ��ȡ�����ṹ
		TJSONObject *MstJson = DataSetToJsonObject(m_MasterSQLString + " \r\n where 1=0");
		JSONObject->AddPair("Master",(TJSONObject*)(MstJson->Clone()));

		// ��ȡ�ӱ��ṹ
		if(m_DataSetType==2)
		{
			TJSONObject *DtlJson = DataSetToJsonObject(m_DetailSQLString + " \r\n where 1=0");
			JSONObject->AddPair("Detail",(TJSONObject*)(DtlJson->Clone()));

			JSONObject->AddPair(new TJSONPair("DetailKeyFields",m_DetailKeyFields));               //14
			JSONObject->AddPair(new TJSONPair("DetailDisplayName",m_DetailDisplayName));           //15
			JSONObject->AddPair(new TJSONPair("DetailDisplayField",m_DetailDisplayField));         //16
			JSONObject->AddPair(new TJSONPair("DetailFilterFields",m_DetailDisplayField));         //17
			JSONObject->AddPair(new TJSONPair("DetailFieldAttr",m_DetailFieldAttr));               //18
			JSONObject->AddPair(new TJSONPair("DetailAttrType",m_DetailAttrType));                 //19
			JSONObject->AddPair(new TJSONPair("DetailFieldExpr",m_DetailFieldExpr));               //20
			JSONObject->AddPair(new TJSONPair("DetailSQLString",m_DetailSQLString));               //21
			JSONObject->AddPair(new TJSONPair("DetailFixedWhereString",m_DetailFixedWhereString)); //22
			JSONObject->AddPair(new TJSONPair("DetailFixedOrderString",m_DetailFixedOrderString)); //23
			JSONObject->AddPair(new TJSONPair("DetailFixedGroupString",m_DetailFixedGroupString)); //24
			JSONObject->AddPair(new TJSONPair("MasterLinkFields",m_MasterLinkFields->Text));       //25
			JSONObject->AddPair(new TJSONPair("DetailLinkFields",m_DetailLinkFields->Text));       //26
		}
		return JSONObject;
	}
	catch(Exception &e)
	{
		return SendFailMessage(asDSError, "���ͳ�ʼ�����ݴ���\r\n" + e.Message);
	}
}
//---------------------------------------------------------------------------
TJSONObject*  TJServerDataSet::Open(TJSONObject *Json, String ClassCode)
{
	TJSONObject *JSONObject = new TJSONObject();
	try
	{
		String sMasterWhere, sDetailWhere, sMasterOrder, sDetailOrder;
		if(Json->Count > 0)
		{
			// ���������JSON����ȡ��ѯ����
			sMasterWhere = Json->GetValue("MasterWhere")->Value();
			sMasterOrder = Json->GetValue("MasterOrder")->Value();
			sDetailWhere = Json->GetValue("DetailWhere")->Value();
			sDetailOrder = Json->GetValue("DetailOrder")->Value();
		}

		// ��ȡ��ǰҵ�������Ϣ
		InitServerDataSet(ClassCode);
		if(m_MasterSQLString.Trim().IsEmpty())
			throw Exception("����SQL�����δ��ֵ.");

		String sql, sql1, RetJson;
		sql = m_MasterSQLString;
		if(!m_MasterFixedWhereString.Trim().IsEmpty())
		{
			sql += " where " + m_MasterFixedWhereString;
			if(sMasterWhere.Trim().IsEmpty())
			{
				if(!m_MasterWhereString.Trim().IsEmpty())
					sql += " and (" + m_MasterWhereString + ")";
			}
			else
			{
				sql += " and (" + sMasterWhere + ")";
			}
		}
		else
		{
			if(sMasterWhere.Trim().IsEmpty())
			{
				if(!m_MasterWhereString.Trim().IsEmpty())
					sql += " where " + m_MasterWhereString;
			}
			else
			{
				sql += " where " + sMasterWhere;
			}
		}

		if(!m_MasterFixedOrderString.Trim().IsEmpty())
		{
			sql += " Order by " + m_MasterFixedOrderString;
			if(sMasterOrder.Trim().IsEmpty())
			{
				if(!m_MasterOrderString.Trim().IsEmpty())
					sql += "," + m_MasterOrderString;
			}
			else
			{
				sql += "," + sMasterOrder;
			}
		}
		else
		{
			if(sMasterOrder.Trim().IsEmpty())
			{
				if(!m_MasterOrderString.Trim().IsEmpty())
					sql += " Order by " + m_MasterOrderString;
			}
			else
			{
				sql += " Order by " + sMasterOrder;
			}
		}
		TJSONObject *MstJson = DataSetToJsonObject(sql);
		JSONObject->AddPair("Master",(TJSONObject*)(MstJson->Clone()));

		if(m_DataSetType==2)
		{
			if(m_DetailSQLString.Trim().IsEmpty())
				throw Exception("�ӱ�SQL�����δ��ֵ.");

			if(m_MasterLinkFields->Count == 0)
				 throw Exception("��δ���������ֶ�.");

			sql1 = m_DetailSQLString;
			sql1 += " where exists(" + m_MasterSQLString;
			if(!m_MasterFixedWhereString.Trim().IsEmpty())
			{
				sql1 += " where " + m_MasterFixedWhereString;
				if(sMasterWhere.Trim().IsEmpty())
				{
					if(!m_MasterWhereString.Trim().IsEmpty())
						sql1 += " and (" + m_MasterWhereString + ")";
				}
				else
				{
					sql1 += " and (" + sMasterWhere + ")";
				}
				sql1 += " and " + BuildLinkStr() + ")";
			}
			else
			{
				if(sMasterWhere.Trim().IsEmpty())
				{
					if(!m_MasterWhereString.Trim().IsEmpty())
						sql1 += " where " + m_MasterWhereString + " and " + BuildLinkStr() + ")";
					else
						sql1 += " where " + BuildLinkStr() + ")";
				}
				else
					sql1 += " where " + sMasterWhere + " and " + BuildLinkStr() + ")";
			}

			if(!m_DetailFixedWhereString.Trim().IsEmpty())
			{
				sql1 += " and " + m_DetailFixedWhereString;
				if(sDetailWhere.Trim().IsEmpty())
				{
					if(!m_DetailWhereString.Trim().IsEmpty())
						sql1 += " and " + m_DetailWhereString;
				}
				else
					sql1 += " and " + sDetailWhere;
			}
			else
			{
				if(sDetailWhere.Trim().IsEmpty())
				{
					if(!m_DetailWhereString.Trim().IsEmpty())
						sql1 += " and " + m_DetailWhereString;
				}
				else
					sql1 += " and " + sDetailWhere;
			}

			if(!m_DetailFixedOrderString.Trim().IsEmpty())
			{
				sql1 += " order by " + m_DetailFixedOrderString;
				if(sDetailOrder.Trim().IsEmpty())
				{
					if(!m_DetailOrderString.Trim().IsEmpty())
						sql1 += "," + m_DetailOrderString;
				}
				else
					sql1 += "," + sDetailOrder;
			}
			else
			{
				if(sDetailOrder.Trim().IsEmpty())
				{
					if(!m_DetailOrderString.Trim().IsEmpty())
						sql1 += " order by " + m_DetailOrderString;
				}
				else
					sql1 += " order by " + sDetailOrder;
			}
			TJSONObject *DtlJson = DataSetToJsonObject(sql1);
			JSONObject->AddPair("Detail",(TJSONObject*)(DtlJson->Clone()));
		}
		JSONObject->AddPair("Status","s");
		return JSONObject;
	}
	catch (Exception &e)
	{
		return SendFailMessage(asDSError, "�����ݼ�����" + e.Message);
	}
}
//---------------------------------------------------------------------------
TJSONObject*  TJServerDataSet::Update(TJSONObject *Json, String ClassCode)
{
	TJSONObject *JSONObject = new TJSONObject();
	try
	{
		if(Json->Count > 0)
		{
			String sql, sParamName, sParamField, Descstr, sMasterStatus, JsonStr;
			Variant VarVal;
			TFieldType datatype;
			TParameterDirection dir;
			TParameter *FParam;

			InitServerDataSet(ClassCode);

			m_MasterDataSet->Close();
			TJSONObject *MstJson = (TJSONObject*)Json->GetValue("Master");
			JsonStr = MstJson->ToString();
			_di_ISuperObject pso = SO(JsonStr);
			TTableJson::JsonToDataSet(m_MasterDataSet,pso);

			m_Connection->BeginTrans();
			sMasterStatus = Json->GetValue("MasterStatus")->Value();
			if(sMasterStatus.ToInt() == 3) // 1-dsBrowse, 2-dsEdit, 3-dsInsert
			{
				sql = " select SyMCMasterAddProc,SyMCMAddProcParam.*,SyDataTypeForCPP"
							" from SyMiddleClass,SyMCMAddProcParam,SyDataType"
							" where SyMCCode='"+m_ClassCode+"' and SyMCMAClassCode=SyMCCode and SyDataTypeCode=SyMCMAParamDataType"
							" order by SyMCMAParamNo";
				m_BuildQuery->Close();
				m_BuildQuery->SQL->Clear();
				m_BuildQuery->SQL->Add(sql);
				m_BuildQuery->Open();
				if(m_BuildQuery->RecordCount < 1)
				{
					m_BuildQuery->Close();
					m_Connection->RollbackTrans();
					throw Exception("�������Ӵ洢������δ����.");
				}

				m_BuildQuery->First();
				m_StoredProc->Close();
				m_StoredProc->ProcedureName = m_BuildQuery->FieldByName("SyMCMasterAddProc")->AsString;
				m_StoredProc->Parameters->Clear();
				while (!m_BuildQuery->Eof)
				{
					datatype = (TFieldType)m_BuildQuery->FieldByName("SyDataTypeForCPP")->AsInteger;
					if(m_BuildQuery->FieldByName("SyMCMAParamDir")->AsInteger == 1)
						dir = pdInput;
					else if(m_BuildQuery->FieldByName("SyMCMAParamDir")->AsInteger == 2)
						dir = pdOutput;
					else
						dir = pdInputOutput;

					if(FAccDbType == 1)
						sParamName = m_BuildQuery->FieldByName("SyMCMAParamName")->AsString;
					else
						sParamName = m_BuildQuery->FieldByName("SyMCMAParamOrcName")->AsString;

					FParam = m_StoredProc->Parameters->CreateParameter(sParamName, ftVariant, dir, m_BuildQuery->FieldByName("SyMCMAParamSize")->AsInteger, NULL);
					sParamField = m_BuildQuery->FieldByName("SyMCMAParamField")->AsString;
					VarVal = m_MasterDataSet->FieldValues[sParamField];
					FParam->DataType = datatype;
					FParam->Value = VarVal;
					Descstr += VarToStr(VarVal) + ",";
					m_BuildQuery->Next();
				}

				m_StoredProc->ExecProc();
				WriteUserLog(1, Descstr);
				m_MasterDataSet->Edit();
				m_BuildQuery->First();
				while (!m_BuildQuery->Eof)
				{
					if(m_BuildQuery->FieldByName("SyMCMAParamDir")->AsInteger != 1)
					{
						sParamField = m_BuildQuery->FieldByName("SyMCMAParamField")->AsString;
						if(m_MasterDataSet->FieldByName(sParamField)->ReadOnly)
							m_MasterDataSet->FieldByName(sParamField)->ReadOnly = false;

						if(FAccDbType == 1)
							sParamName = m_BuildQuery->FieldByName("SyMCMAParamName")->AsString;
						else
							sParamName = m_BuildQuery->FieldByName("SyMCMAParamOrcName")->AsString;

						m_MasterDataSet->FieldValues[sParamField] = m_StoredProc->Parameters->ParamValues[sParamName];
					}
					m_BuildQuery->Next();
				}
				m_MasterDataSet->Post();
				m_MasterDataSet->UpdateBatch(arAll);
				String Json = TTableJson::DataSetToJson(m_MasterDataSet);
				JSONObject->AddPair("Master",(TJSONObject*)TJSONObject::ParseJSONValue(Json));
				m_MasterDataSet->Close();
				m_BuildQuery->Close();
				m_StoredProc->Close();
			}
			else if(sMasterStatus.ToInt() == 2)
			{
				TADODataSet *m_OldMasterDataSet = new TADODataSet(NULL);
				try
				{
					TJSONObject *MstJson = (TJSONObject*)Json->GetValue("OldMaster");
					JsonStr = MstJson->ToString();
					_di_ISuperObject pso = SO(JsonStr);
					TTableJson::JsonToDataSet(m_OldMasterDataSet,pso);

					sql = " select SyMCMasterEditProc,SyMCMEditProcParam.*,SyDataTypeForCPP"
								" from SyMiddleClass,SyMCMEditProcParam,SyDataType"
								" where SyMCCode='"+m_ClassCode+"' and SyMCMEClassCode=SyMCCode and SyDataTypeCode=SyMCMEParamDataType"
								" order by SyMCMEParamNo";
					m_BuildQuery->Close();
					m_BuildQuery->SQL->Clear();
					m_BuildQuery->SQL->Add(sql);
					m_BuildQuery->Open();
					if(m_BuildQuery->RecordCount < 1)
					{
						m_BuildQuery->Close();
						m_Connection->RollbackTrans();
						throw Exception("�������´洢������δ����.");
					}

					m_BuildQuery->First();
					m_StoredProc->Close();
					m_StoredProc->ProcedureName = m_BuildQuery->FieldByName("SyMCMasterEditProc")->AsString;
					m_StoredProc->Parameters->Clear();
					while (!m_BuildQuery->Eof)
					{
						datatype = (TFieldType)m_BuildQuery->FieldByName("SyDataTypeForCPP")->AsInteger;
						if(m_BuildQuery->FieldByName("SyMCMEParamDir")->AsInteger == 1)
							dir = pdInput;
						else if(m_BuildQuery->FieldByName("SyMCMEParamDir")->AsInteger == 2)
							dir = pdOutput;
						else
							dir = pdInputOutput;

						if(FAccDbType == 1)
							sParamName = m_BuildQuery->FieldByName("SyMCMEParamName")->AsString;
						else
							sParamName = m_BuildQuery->FieldByName("SyMCMEParamOrcName")->AsString;

						FParam = m_StoredProc->Parameters->CreateParameter(sParamName, ftVariant, dir, m_BuildQuery->FieldByName("SyMCMEParamSize")->AsInteger, NULL);
						FParam->DataType = datatype;
						// �ֶ�����
						sParamField = m_BuildQuery->FieldByName("SyMCMEParamField")->AsString;
						if(m_BuildQuery->FieldByName("SyMCMEParamOldValue")->AsInteger == 1)
							FParam->Value = m_OldMasterDataSet->FieldValues[sParamField];
						else
							FParam->Value = m_MasterDataSet->FieldValues[sParamField];

						Descstr += VarToStr(FParam->Value) + ",";
						m_BuildQuery->Next();
					}
					m_StoredProc->ExecProc();
					m_MasterDataSet->Edit();
					m_BuildQuery->First();
					while(!m_BuildQuery->Eof)
					{
						if(m_BuildQuery->FieldByName("SyMCMEParamDir")->AsInteger != 1)
						{
							sParamField = m_BuildQuery->FieldByName("SyMCMEParamField")->AsString;
							if(m_MasterDataSet->FieldByName(sParamField)->ReadOnly)
								m_MasterDataSet->FieldByName(sParamField)->ReadOnly = false;

							if(FAccDbType == 1)
								sParamName = m_BuildQuery->FieldByName("SyMCMEParamName")->AsString;
							else
								sParamName = m_BuildQuery->FieldByName("SyMCMEParamOrcName")->AsString;

							m_MasterDataSet->FieldValues[sParamField] = m_StoredProc->Parameters->ParamValues[sParamName];
						}
						m_BuildQuery->Next();
					}
					m_MasterDataSet->Post();
					m_MasterDataSet->UpdateBatch(arAll);
					String Json = TTableJson::DataSetToJson(m_MasterDataSet);
					JSONObject->AddPair("Master",(TJSONObject*)TJSONObject::ParseJSONValue(Json));
					m_MasterDataSet->Close();
					m_BuildQuery->Close();
					m_StoredProc->Close();
					WriteUserLog(2, Descstr);
				}
				__finally
				{
					delete m_OldMasterDataSet;
					m_OldMasterDataSet = NULL;
				}
			}

			if(m_DataSetType == 2)
			{
				m_DetailDataSet->Close();
				TJSONObject *DtlJson = (TJSONObject*)Json->GetValue("Detail");
				JsonStr = DtlJson->ToString();
				_di_ISuperObject pso = SO(JsonStr);
				TTableJson::JsonToDataSet(m_DetailDataSet,pso);

				TStringList *DtlRecStatus = new TStringList;
				TADODataSet *m_OldDetailDataSet = new TADODataSet(NULL);
				try
				{
					// ע�⣺�˴���ϸ��¼����Ŀ�������ϸ��¼״̬һһ��Ӧ
					// ���磺��ϸ��¼����Ŀ��4����Ӧ��DetailStatus����"0\r\n1\r\n2\r\n3\r\n"
					// ��ϸ����¼��״̬��1-���;3-����;2-�޸�;0-ɾ��
					DtlRecStatus->Text = Json->GetValue("DetailStatus")->Value();

					// ѭ����ϸ��¼����Ӧ����ϸOld��¼Ҳ��ѭ��
					m_DetailDataSet->First();
					while (!m_DetailDataSet->Eof)
					{
						switch (DtlRecStatus->Strings[m_DetailDataSet->RecNo-1].ToInt())
						{
							case 3: // ����
								sql = " select SyMCDetailAddProc,SyMCDAddProcParam.*,SyDataTypeForCPP"
											" from SyMiddleClass,SyMCDAddProcParam,SyDataType"
											" where SyMCCode='"+m_ClassCode+"' and SyMCDAClassCode=SyMCCode and SyDataTypeCode=SyMCDAParamDataType"
											" order by SyMCDAParamNo";
								m_BuildQuery->Close();
								m_BuildQuery->SQL->Clear();
								m_BuildQuery->SQL->Add(sql);
								m_BuildQuery->Open();
								if(m_BuildQuery->RecordCount < 1)
								{
									m_BuildQuery->Close();
									m_Connection->RollbackTrans();
									throw Exception("�ӱ����Ӵ洢������δ����.");
								}

								// ִ�������洢����
								m_BuildQuery->First();
								m_StoredProc->Close();
								m_StoredProc->ProcedureName = m_BuildQuery->FieldByName("SyMCDetailAddProc")->AsString;
								m_StoredProc->Parameters->Clear();
								while (!m_BuildQuery->Eof)
								{
									datatype = (TFieldType)m_BuildQuery->FieldByName("SyDataTypeForCPP")->AsInteger;
									if(m_BuildQuery->FieldByName("SyMCDAParamDir")->AsInteger == 1)
										dir = pdInput;
									else if(m_BuildQuery->FieldByName("SyMCDAParamDir")->AsInteger == 2)
										dir = pdOutput;
									else
										dir = pdInputOutput;

									if(FAccDbType == 1)
										sParamName = m_BuildQuery->FieldByName("SyMCDAParamName")->AsString;
									else
										sParamName = m_BuildQuery->FieldByName("SyMCDAParamOrcName")->AsString;

									FParam = m_StoredProc->Parameters->CreateParameter(sParamName, ftVariant, dir, m_BuildQuery->FieldByName("SyMCDAParamSize")->AsInteger, NULL);
									FParam->DataType = datatype;
									sParamField = m_BuildQuery->FieldByName("SyMCDAParamField")->AsString;
									FParam->Value = m_DetailDataSet->FieldValues[sParamField];
									m_BuildQuery->Next();
								}
								m_StoredProc->ExecProc();

								// �����洢���̵ķ����Ͳ�����ֵ�����磺���
								m_DetailDataSet->Edit();
								m_BuildQuery->First();
								while (!m_BuildQuery->Eof)
								{
									if(m_BuildQuery->FieldByName("SyMCDAParamDir")->AsInteger != 1)
									{
										sParamField = m_BuildQuery->FieldByName("SyMCDAParamField")->AsString;
										if(m_DetailDataSet->FieldByName(sParamField)->ReadOnly)
											m_DetailDataSet->FieldByName(sParamField)->ReadOnly = false;

										if(FAccDbType == 1)
											sParamName = m_BuildQuery->FieldByName("SyMCDAParamName")->AsString;
										else
											sParamName = m_BuildQuery->FieldByName("SyMCDAParamOrcName")->AsString;

										m_DetailDataSet->FieldValues[sParamField] = m_StoredProc->Parameters->ParamValues[sParamName];
									}
									m_BuildQuery->Next();
								}
								m_DetailDataSet->Post();

								m_BuildQuery->Close();
								m_StoredProc->Close();
								break;
							case 2: // �޸�
							{
							   TJSONObject *DtlJson = (TJSONObject*)Json->GetValue("OldDetail");
							   JsonStr = DtlJson->ToString();
							   _di_ISuperObject pso = SO(JsonStr);
							   TTableJson::JsonToDataSet(m_OldDetailDataSet,pso);
							   m_OldDetailDataSet->First();
							   sql = " select SyMCDetailEditProc,SyMCDEditProcParam.*,SyDataTypeForCPP"
											" from SyMiddleClass,SyMCDEditProcParam,SyDataType"
											" where SyMCCode='"+m_ClassCode+"' and SyMCDEClassCode=SyMCCode and SyDataTypeCode=SyMCDEParamDataType"
											" order by SyMCDEParamNo";
								m_BuildQuery->Close();
								m_BuildQuery->SQL->Clear();
								m_BuildQuery->SQL->Add(sql);
								m_BuildQuery->Open();
								if(m_BuildQuery->RecordCount < 1)
								{
									m_BuildQuery->Close();
									m_Connection->RollbackTrans();
									throw Exception("�ӱ����´洢������δ����.");
								}

								m_BuildQuery->First();
								m_StoredProc->Close();
								m_StoredProc->ProcedureName = m_BuildQuery->FieldByName("SyMCDetailEditProc")->AsString;
								m_StoredProc->Parameters->Clear();
								while (!m_BuildQuery->Eof)
								{
									datatype = (TFieldType)m_BuildQuery->FieldByName("SyDataTypeForCPP")->AsInteger;
									if(m_BuildQuery->FieldByName("SyMCDEParamDir")->AsInteger == 1)
										dir = pdInput;
									else if(m_BuildQuery->FieldByName("SyMCDEParamDir")->AsInteger == 2)
										dir = pdOutput;
									else
										dir = pdInputOutput;

									if(FAccDbType == 1)
										sParamName = m_BuildQuery->FieldByName("SyMCDEParamName")->AsString;
									else
										sParamName = m_BuildQuery->FieldByName("SyMCDEParamOrcName")->AsString;

									FParam = m_StoredProc->Parameters->CreateParameter(sParamName, ftVariant, dir, m_BuildQuery->FieldByName("SyMCDEParamSize")->AsInteger, NULL);
									FParam->DataType = datatype;

									// ��ǰ�ֶ��Ƿ�ȡ��ʷ���ݣ�һ��Ϊ���������
									sParamField = m_BuildQuery->FieldByName("SyMCDEParamField")->AsString;
									if(m_BuildQuery->FieldByName("SyMCDEParamOldValue")->AsInteger == 1)
										FParam->Value = m_OldDetailDataSet->FieldValues[sParamField];
									else
										FParam->Value = m_DetailDataSet->FieldValues[sParamField];
									m_BuildQuery->Next();
								}
								m_StoredProc->ExecProc();
								m_OldDetailDataSet->Next();

								m_DetailDataSet->Edit();
								m_BuildQuery->First();
								while (!m_BuildQuery->Eof)
								{
									if(m_BuildQuery->FieldByName("SyMCDEParamDir")->AsInteger != 1)
									{
										sParamField = m_BuildQuery->FieldByName("SyMCDEParamField")->AsString;
										if(m_DetailDataSet->FieldByName(sParamField)->ReadOnly)
											m_DetailDataSet->FieldByName(sParamField)->ReadOnly = false;

										if(FAccDbType == 1)
											sParamName = m_BuildQuery->FieldByName("SyMCDEParamName")->AsString;
										else
											sParamName = m_BuildQuery->FieldByName("SyMCDEParamOrcName")->AsString;

										m_DetailDataSet->FieldValues[sParamField] = m_StoredProc->Parameters->ParamValues[sParamName];
									}
									m_BuildQuery->Next();
								}
								m_DetailDataSet->Post();

								m_BuildQuery->Close();
								m_StoredProc->Close();
								break;
							}
							case 0: // ɾ��
								sql = " select SyMCDetailDelProc,SyMCDDelProcParam.*,SyDataTypeForCPP"
											" from SyMiddleClass,SyMCDDelProcParam,SyDataType"
											" where SyMCCode='"+m_ClassCode+"' and SyMCDDClassCode=SyMCCode and SyDataTypeCode=SyMCDDParamDataType"
											" order by SyMCDDParamNo";
								m_BuildQuery->Close();
								m_BuildQuery->SQL->Clear();
								m_BuildQuery->SQL->Add(sql);
								m_BuildQuery->Open();
								if(m_BuildQuery->RecordCount < 1)
								{
									m_BuildQuery->Close();
									m_Connection->RollbackTrans();
									 throw Exception("ɾ���洢������δ����.");
								}

								m_BuildQuery->First();
								m_StoredProc->Close();
								m_StoredProc->ProcedureName = m_BuildQuery->FieldByName("SyMCDetailDelProc")->AsString;
								m_StoredProc->Parameters->Clear();
								while (!m_BuildQuery->Eof)
								{
									datatype = (TFieldType)m_BuildQuery->FieldByName("SyDataTypeForCPP")->AsInteger;
									if(m_BuildQuery->FieldByName("SyMCDDParamDir")->AsInteger == 1)
										dir = pdInput;
									else if(m_BuildQuery->FieldByName("SyMCDDParamDir")->AsInteger == 2)
										dir = pdOutput;
									else
										dir = pdInputOutput;

									if(FAccDbType == 1)
										sParamName = m_BuildQuery->FieldByName("SyMCDDParamName")->AsString;
									else
										sParamName = m_BuildQuery->FieldByName("SyMCDDParamOrcName")->AsString;

									FParam = m_StoredProc->Parameters->CreateParameter(sParamName, ftVariant, dir, m_BuildQuery->FieldByName("SyMCDDParamSize")->AsInteger, NULL);
									FParam->DataType = datatype;
									sParamField = m_BuildQuery->FieldByName("SyMCDDParamField")->AsString;
									FParam->Value = m_DetailDataSet->FieldByName(sParamField)->OldValue;
									m_BuildQuery->Next();
								}
								m_StoredProc->ExecProc();
								m_BuildQuery->Close();
								m_StoredProc->Close();
								break;
							default:
								break;
						}
						m_DetailDataSet->Next();
					}
				}
				__finally
				{
					delete m_OldDetailDataSet;
					m_OldDetailDataSet = NULL;
					delete DtlRecStatus;
					DtlRecStatus = NULL;
				}

				// ����鹹���ֶ�û�и�ֵ�����ᱨ��E_Fail
				/*********************************************************************
				//FilterGroup�����������£�
				//FgUnassigned         ָ����ʹ���κεĹ���������������
				//fgNone               ��ȥ�κεĹ������������е����ݶ���ʾ��������Ҳ����ͨ���趨ADO��Filtered����ֵΪFalse���ﵽ
				//fgPendingRecords     ֻ��ʾ���޸Ĺ�����δ���»�����Դ�����ݻ��޸Ĺ�����δ��ȡ�����µ�����
				//fgAffectedRecords    ֻ��ʾ�ܵ���һ�θ��»�����ԴӰ�������
				//fgFetchedRecords     ֻ��ʾ��Ŀǰ���»���֮�е����ݡ���Щ����������һ�δ�����Դȡ��֮�����д����ڿͻ��˵�����
				//fgPredicate          ֻ��ʾ�ձ�ɾ��������
				//fgConflictingRecords ֻ��ʾ���޸Ĺ��������»�����Դʱ�������������
				*********************************************************************/
/*				m_DetailDataSet->FilterGroup = fgPendingRecords;
				m_DetailDataSet->Filtered = true;
				int nCount = m_DetailDataSet->RecordCount;
				m_DetailDataSet->First();
				while (!m_DetailDataSet->Eof)
				{
					if(m_DetailDataSet->RecordStatus.Contains(rsNew))
					{
						sql = " select SyMCDetailAddProc,SyMCDAddProcParam.*,SyDataTypeForCPP"
									" from SyMiddleClass,SyMCDAddProcParam,SyDataType"
									" where SyMCCode='"+m_ClassCode+"' and SyMCDAClassCode=SyMCCode and SyDataTypeCode=SyMCDAParamDataType"
									" order by SyMCDAParamNo";
						m_BuildQuery->Close();
						m_BuildQuery->SQL->Clear();
						m_BuildQuery->SQL->Add(sql);
						m_BuildQuery->Open();
						if(m_BuildQuery->RecordCount < 1)
						{
							m_BuildQuery->Close();
							m_Connection->RollbackTrans();
							throw Exception("�ӱ����Ӵ洢������δ����.");
						}

						m_BuildQuery->First();
						m_StoredProc->Close();
						m_StoredProc->ProcedureName = m_BuildQuery->FieldByName("SyMCDetailAddProc")->AsString;
						m_StoredProc->Parameters->Clear();
						while (!m_BuildQuery->Eof)
						{
							datatype = (TFieldType)m_BuildQuery->FieldByName("SyDataTypeForCPP")->AsInteger;
							if(m_BuildQuery->FieldByName("SyMCDAParamDir")->AsInteger == 1)
								dir = pdInput;
							else if(m_BuildQuery->FieldByName("SyMCDAParamDir")->AsInteger == 2)
								dir = pdOutput;
							else
								dir = pdInputOutput;

							if(FAccDbType == 1)
								FParamName = m_BuildQuery->FieldByName("SyMCDAParamName")->AsString;
							else
								FParamName = m_BuildQuery->FieldByName("SyMCDAParamOrcName")->AsString;

							FParam = m_StoredProc->Parameters->CreateParameter(FParamName, ftVariant, dir, m_BuildQuery->FieldByName("SyMCDAParamSize")->AsInteger, NULL);
							FParam->DataType = datatype;
							FParam->Value = m_DetailDataSet->FieldValues[m_BuildQuery->FieldByName("SyMCDAParamField")->AsString];
							m_BuildQuery->Next();
						}

						m_StoredProc->ExecProc();
						m_DetailDataSet->Edit();
						m_BuildQuery->First();
						while (!m_BuildQuery->Eof)
						{
							if(m_BuildQuery->FieldByName("SyMCDAParamDir")->AsInteger != 1)
							{
								if(m_DetailDataSet->FieldByName(m_BuildQuery->FieldByName("SyMCDAParamField")->AsString)->ReadOnly)
									m_DetailDataSet->FieldByName(m_BuildQuery->FieldByName("SyMCDAParamField")->AsString)->ReadOnly = false;

								if(FAccDbType == 1)
									FParamName = m_BuildQuery->FieldByName("SyMCDAParamName")->AsString;
								else
									FParamName = m_BuildQuery->FieldByName("SyMCDAParamOrcName")->AsString;

								m_DetailDataSet->FieldValues[m_BuildQuery->FieldByName("SyMCDAParamField")->AsString] = m_StoredProc->Parameters->ParamValues[FParamName];
							}
							m_BuildQuery->Next();
						}
						m_DetailDataSet->Post();
						m_BuildQuery->Close();
						m_StoredProc->Close();
					}
					else if(m_DetailDataSet->RecordStatus.Contains(rsModified))
					{
						sql = " select SyMCDetailEditProc,SyMCDEditProcParam.*,SyDataTypeForCPP"
									" from SyMiddleClass,SyMCDEditProcParam,SyDataType"
									" where SyMCCode='"+m_ClassCode+"' and SyMCDEClassCode=SyMCCode and SyDataTypeCode=SyMCDEParamDataType"
									" order by SyMCDEParamNo";
						m_BuildQuery->Close();
						m_BuildQuery->SQL->Clear();
						m_BuildQuery->SQL->Add(sql);
						m_BuildQuery->Open();
						if(m_BuildQuery->RecordCount < 1)
						{
							m_BuildQuery->Close();
							m_Connection->RollbackTrans();
							throw Exception("�ӱ����´洢������δ����.");
						}

						m_BuildQuery->First();
						m_StoredProc->Close();
						m_StoredProc->ProcedureName = m_BuildQuery->FieldByName("SyMCDetailEditProc")->AsString;
						m_StoredProc->Parameters->Clear();
						while (!m_BuildQuery->Eof)
						{
							datatype = (TFieldType)m_BuildQuery->FieldByName("SyDataTypeForCPP")->AsInteger;
							if(m_BuildQuery->FieldByName("SyMCDEParamDir")->AsInteger == 1)
								dir = pdInput;
							else if(m_BuildQuery->FieldByName("SyMCDEParamDir")->AsInteger == 2)
								dir = pdOutput;
							else
								dir = pdInputOutput;

							if(FAccDbType == 1)
								FParamName = m_BuildQuery->FieldByName("SyMCDEParamName")->AsString;
							else
								FParamName = m_BuildQuery->FieldByName("SyMCDEParamOrcName")->AsString;

							FParam = m_StoredProc->Parameters->CreateParameter(FParamName, ftVariant, dir, m_BuildQuery->FieldByName("SyMCDEParamSize")->AsInteger, NULL);
							FParam->DataType = datatype;

							if(m_BuildQuery->FieldByName("SyMCDEParamOldValue")->AsInteger == 1)
								FParam->Value = m_DetailDataSet->FieldByName(m_BuildQuery->FieldByName("SyMCDEParamField")->AsString)->OldValue;
							else
								FParam->Value = m_DetailDataSet->FieldValues[m_BuildQuery->FieldByName("SyMCDEParamField")->AsString];
							m_BuildQuery->Next();
						}

						m_StoredProc->ExecProc();
						m_DetailDataSet->Edit();
						m_BuildQuery->First();
						while (!m_BuildQuery->Eof)
						{
							if(m_BuildQuery->FieldByName("SyMCDEParamDir")->AsInteger != 1)
							{
								if(m_DetailDataSet->FieldByName(m_BuildQuery->FieldByName("SyMCDEParamField")->AsString)->ReadOnly)
									m_DetailDataSet->FieldByName(m_BuildQuery->FieldByName("SyMCDEParamField")->AsString)->ReadOnly = false;

								if(FAccDbType == 1)
									FParamName = m_BuildQuery->FieldByName("SyMCDEParamName")->AsString;
								else
									FParamName = m_BuildQuery->FieldByName("SyMCDEParamOrcName")->AsString;

								m_DetailDataSet->FieldValues[m_BuildQuery->FieldByName("SyMCDEParamField")->AsString] = m_StoredProc->Parameters->ParamValues[FParamName];
							}
							m_BuildQuery->Next();
						}
						m_DetailDataSet->Post();
						m_BuildQuery->Close();
						m_StoredProc->Close();
					}
					else if(m_DetailDataSet->RecordStatus.Contains(rsDeleted))
					{
						sql = " select SyMCDetailDelProc,SyMCDDelProcParam.*,SyDataTypeForCPP"
									" from SyMiddleClass,SyMCDDelProcParam,SyDataType"
									" where SyMCCode='"+m_ClassCode+"' and SyMCDDClassCode=SyMCCode and SyDataTypeCode=SyMCDDParamDataType"
									" order by SyMCDDParamNo";
						m_BuildQuery->Close();
						m_BuildQuery->SQL->Clear();
						m_BuildQuery->SQL->Add(sql);
						m_BuildQuery->Open();
						if(m_BuildQuery->RecordCount < 1)
						{
							m_BuildQuery->Close();
							m_Connection->RollbackTrans();
							 throw Exception("ɾ���洢������δ����.");
						}

						m_BuildQuery->First();
						m_StoredProc->Close();
						m_StoredProc->ProcedureName = m_BuildQuery->FieldByName("SyMCDetailDelProc")->AsString;
						m_StoredProc->Parameters->Clear();
						while (!m_BuildQuery->Eof)
						{
							datatype = (TFieldType)m_BuildQuery->FieldByName("SyDataTypeForCPP")->AsInteger;
							if(m_BuildQuery->FieldByName("SyMCDDParamDir")->AsInteger == 1)
								dir = pdInput;
							else if(m_BuildQuery->FieldByName("SyMCDDParamDir")->AsInteger == 2)
								dir = pdOutput;
							else
								dir = pdInputOutput;

							if(FAccDbType == 1)
								FParamName = m_BuildQuery->FieldByName("SyMCDDParamName")->AsString;
							else
								FParamName = m_BuildQuery->FieldByName("SyMCDDParamOrcName")->AsString;

							FParam = m_StoredProc->Parameters->CreateParameter(FParamName, ftVariant, dir, m_BuildQuery->FieldByName("SyMCDDParamSize")->AsInteger, NULL);
							FParam->DataType = datatype;
							FParam->Value = m_DetailDataSet->FieldByName(m_BuildQuery->FieldByName("SyMCDDParamField")->AsString)->OldValue;
							m_BuildQuery->Next();
						}
						m_StoredProc->ExecProc();
						m_BuildQuery->Close();
						m_StoredProc->Close();
					}
					m_DetailDataSet->Next();
				} */
				m_DetailDataSet->Filtered = false; // zws 20090107
				String Json = TTableJson::DataSetToJson(m_DetailDataSet);
				JSONObject->AddPair("Detail",(TJSONObject*)TJSONObject::ParseJSONValue(Json));
				m_DetailDataSet->Close();
			}
			m_BuildQuery->Close();
			JSONObject->AddPair("Status","s");
			m_Connection->CommitTrans();
		}
		return JSONObject;
	}
	catch (Exception &e)
	{
		m_Connection->RollbackTrans();
		return SendFailMessage(asDSError, "���¼�¼ʱ����" + e.Message);
	}
}
//---------------------------------------------------------------------------
TJSONObject*  TJServerDataSet::Delete(TJSONObject *Json, String ClassCode)
{
	TJSONObject *JSONObject = new TJSONObject();
	try
	{
		if(Json->Count > 0)
		{
			String sql, FParamName, DescStr;
			TFieldType datatype;
			TParameterDirection dir;
			TParameter *FParam;

			InitServerDataSet(ClassCode);

			TJSONObject *MstJson = (TJSONObject*)Json->GetValue("Master");
			m_MasterDataSet->Close();
			_di_ISuperObject pso = SO(MstJson->ToString());
			TTableJson::JsonToDataSet(m_MasterDataSet,pso);

			sql = " select SyMCMasterDelProc,SyMCMDelProcParam.*,SyDataTypeForCPP"
						" from SyMiddleClass,SyMCMDelProcParam,SyDataType"
						" where SyMCCode='"+m_ClassCode+"' and SyMCMDClassCode=SyMCCode and SyDataTypeCode=SyMCMDParamDataType"
						" order by SyMCMDParamNo";
			m_BuildQuery->Close();
			m_BuildQuery->SQL->Clear();
			m_BuildQuery->SQL->Add(sql);
			m_BuildQuery->Open();
			if(m_BuildQuery->RecordCount < 1)
			{
				m_BuildQuery->Close();
				throw Exception("ɾ���洢������δ����.");
			}

			m_BuildQuery->First();
			m_StoredProc->Close();
			m_StoredProc->ProcedureName = m_BuildQuery->FieldByName("SyMCMasterDelProc")->AsString;
			m_StoredProc->Parameters->Clear();
			while (!m_BuildQuery->Eof)
			{
				datatype = (TFieldType)m_BuildQuery->FieldByName("SyDataTypeForCPP")->AsInteger;
				if(m_BuildQuery->FieldByName("SyMCMDParamDir")->AsInteger == 1)
					dir = pdInput;
				else if(m_BuildQuery->FieldByName("SyMCMDParamDir")->AsInteger == 2)
					dir = pdOutput;
				else
					dir = pdInputOutput;

				if(FAccDbType == 1)
					FParamName = m_BuildQuery->FieldByName("SyMCMDParamName")->AsString;
				else
					FParamName = m_BuildQuery->FieldByName("SyMCMDParamOrcName")->AsString;

				FParam = m_StoredProc->Parameters->CreateParameter(FParamName, ftVariant, dir, m_BuildQuery->FieldByName("SyMCMDParamSize")->AsInteger, NULL);
				FParam->DataType = datatype;
				FParam->Value = m_MasterDataSet->FieldValues[m_BuildQuery->FieldByName("SyMCMDParamField")->AsString];
				DescStr += VarToStr(FParam->Value) + ",";
				m_BuildQuery->Next();
			}
			m_Connection->BeginTrans();
			m_StoredProc->ExecProc();
			m_Connection->CommitTrans();
			m_StoredProc->Close();
			WriteUserLog(0, DescStr);

			JSONObject->AddPair("Status","s");
		}
		return JSONObject;
	}
	catch (Exception &e)
	{
		m_Connection->RollbackTrans();
		return SendFailMessage(asDSError, "ɾ����¼ʱ����" + e.Message);
	}
}
//---------------------------------------------------------------------------
TJSONObject*  TJServerDataSet::DeleteDetail(TJSONObject *Json, String ClassCode)
{
	TJSONObject *JSONObject = new TJSONObject();
	try
	{
		if(Json->Count > 0)
		{
			String sql, FParamName, DescStr;
			TFieldType datatype;
			TParameterDirection dir;
			TParameter *FParam;

			InitServerDataSet(ClassCode);

			TJSONObject *DtlJson = (TJSONObject*)Json->GetValue("Detail");
			m_DetailDataSet->Close();
			_di_ISuperObject pso = SO(DtlJson->ToString());
			TTableJson::JsonToDataSet(m_DetailDataSet,pso);

			sql = " select SyMCDetailDelProc,SyMCDDelProcParam.*,SyDataTypeForCPP"
						" from SyMiddleClass,SyMCDDelProcParam,SyDataType"
						" where SyMCCode='"+m_ClassCode+"' and SyMCDDClassCode=SyMCCode and SyDataTypeCode=SyMCDDParamDataType"
						" order by SyMCDDParamNo";
			m_BuildQuery->Close();
			m_BuildQuery->SQL->Clear();
			m_BuildQuery->SQL->Add(sql);
			m_BuildQuery->Open();
			if(m_BuildQuery->RecordCount < 1)
			{
				m_BuildQuery->Close();
				throw Exception("ɾ���洢������δ����.");
			}

			m_BuildQuery->First();
			m_StoredProc->Close();
			m_StoredProc->ProcedureName = m_BuildQuery->FieldByName("SyMCDetailDelProc")->AsString;
			m_StoredProc->Parameters->Clear();
			while (!m_BuildQuery->Eof)
			{
				datatype = (TFieldType)m_BuildQuery->FieldByName("SyDataTypeForCPP")->AsInteger;
				if(m_BuildQuery->FieldByName("SyMCDDParamDir")->AsInteger == 1)
					dir = pdInput;
				else if(m_BuildQuery->FieldByName("SyMCDDParamDir")->AsInteger == 2)
					dir = pdOutput;
				else
					dir = pdInputOutput;

				if(FAccDbType == 1)
					FParamName = m_BuildQuery->FieldByName("SyMCDDParamName")->AsString;
				else
					FParamName = m_BuildQuery->FieldByName("SyMCDDParamOrcName")->AsString;

				FParam = m_StoredProc->Parameters->CreateParameter(FParamName, ftVariant, dir, m_BuildQuery->FieldByName("SyMCDDParamSize")->AsInteger, NULL);
				FParam->DataType = datatype;
				FParam->Value = m_DetailDataSet->FieldValues[m_BuildQuery->FieldByName("SyMCDDParamField")->AsString];
				DescStr += VarToStr(FParam->Value) + ",";
				m_BuildQuery->Next();
			}
			m_Connection->BeginTrans();
			m_StoredProc->ExecProc();
			m_Connection->CommitTrans();
			m_StoredProc->Close();
			WriteUserLog(6, DescStr);

			JSONObject->AddPair("Status","s");
		}
		return JSONObject;
	}
	catch (Exception &e)
	{
		m_Connection->RollbackTrans();
		return SendFailMessage(asDSError, "ɾ����¼ʱ����" + e.Message);
	}
}
//---------------------------------------------------------------------------
TJSONObject*  TJServerDataSet::ValidateData(TJSONObject *Json, String ClassCode)
{
	TJSONObject *JSONObject = new TJSONObject();
	try
	{
		InitServerDataSet(ClassCode);
		JSONObject->AddPair("Status","s");
		return JSONObject;
	}
	catch (Exception &e)
	{
		return SendFailMessage(asDSError, "У�����ݴ���(ϵͳ)��" + e.Message);
	}
}
//---------------------------------------------------------------------------
TJSONObject*  TJServerDataSet::CmdExec(TJSONObject *Json, String ClassCode)
{
	TJSONObject *JSONObject = new TJSONObject();
	try
	{
		String sql, LibName, LibPath, retstr, ConnPtr;
		int index;
		HINSTANCE hInst;
		typedef char*(CALLBACK * HCommandExecute)(int, int);
		HCommandExecute CommandExecute;

		InitServerDataSet(ClassCode);
		sql = " select SyMLFileName,SyMLFilePath"
					" from SyMiddleLib,SyMiddleClass"
					" where SyMCCode='"+m_ClassCode+"' and SyMCOwnCode=SyMLCode";
		m_BuildQuery->Close();
		m_BuildQuery->SQL->Text = sql;
		m_BuildQuery->Open();
		if(m_BuildQuery->RecordCount < 1)
		{
			m_BuildQuery->Close();
			throw Exception( "ҵ������δ����.");
		}

		LibName = m_BuildQuery->FieldByName("SyMLFileName")->AsString;
		LibPath = m_BuildQuery->FieldByName("SyMLFilePath")->AsString;
		m_BuildQuery->Close();
		if(!LibName.Trim().IsEmpty())
		{
			index = LibName.Pos0(".");
			if(index > -1)
				LibName = LibName.SubString0(0, index+1) + "dll";
			else
				LibName = LibName + ".dll";

			hInst = LoadLibrary(LibName.c_str());
			if(hInst != NULL)
			{
				try
				{
					CommandExecute = (HCommandExecute)GetProcAddress(hInst,"CommandExecute");
					if(CommandExecute != NULL)
					{
						retstr = CommandExecute(m_ClassCode.ToInt(),(int)JSONObject);
						if(retstr.Trim().IsEmpty())
						{
							JSONObject->AddPair("Status","s");
							FreeLibrary(hInst);
							return JSONObject;
						}
						else
						{
							FreeLibrary(hInst);
							throw Exception( retstr);
						}
					}
					else
					{
						FreeLibrary(hInst);
						throw Exception( "��ȡ���ļ�����ʧ��.");
					}
				}
				catch (Exception &e)
				{
					FreeLibrary(hInst);
					return SendFailMessage(asDSError, "ִ��ͨ���������" + e.Message);
				}
			}
			else
				throw Exception( "������ļ�:" + LibName + "ʧ��");
		}
		else
			throw Exception( "�м�㴦�����ļ�δ����.");
	}
	catch (Exception &e)
	{
		return SendFailMessage(asDSError, "ִ��ͨ���������" + e.Message);
	}
}
//---------------------------------------------------------------------------
TJSONObject*  TJServerDataSet::Check(TJSONObject *Json, String ClassCode)
{
	TJSONObject *JSONObject = new TJSONObject();
	try
	{
		if(Json->Count > 0)
		{
			String sql, FParamName, DescStr;
			TFieldType datatype;
			TParameterDirection dir;
			TParameter *FParam;

			InitServerDataSet(ClassCode);

			TJSONObject *MstJson = (TJSONObject*)Json->GetValue("Master");
			m_MasterDataSet->Close();
			_di_ISuperObject pso = SO(MstJson->ToString());
			TTableJson::JsonToDataSet(m_MasterDataSet,pso);

			sql = " select SyMCMasterCheckProc,SyMCMCheckProcParam.*,SyDataTypeForCPP"
						" from SyMiddleClass,SyMCMCheckProcParam,SyDataType"
						" where SyMCCode='"+m_ClassCode+"' and SyMCMCkClassCode=SyMCCode and SyDataTypeCode=SyMCMCkParamDataType"
						" order by SyMCMCkParamNo";
			m_BuildQuery->Close();
			m_BuildQuery->SQL->Clear();
			m_BuildQuery->SQL->Add(sql);
			m_BuildQuery->Open();
			if(m_BuildQuery->RecordCount < 1)
			{
				m_BuildQuery->Close();
				throw Exception("��˴洢������δ����.");
			}

			m_BuildQuery->First();
			m_StoredProc->Close();
			m_StoredProc->ProcedureName = m_BuildQuery->FieldByName("SyMCMasterCheckProc")->AsString;
			m_StoredProc->Parameters->Clear();
			while (!m_BuildQuery->Eof)
			{
				datatype = (TFieldType)m_BuildQuery->FieldByName("SyDataTypeForCPP")->AsInteger;
				if(m_BuildQuery->FieldByName("SyMCMCkParamDir")->AsInteger == 1)
					dir = pdInput;
				else if(m_BuildQuery->FieldByName("SyMCMCkParamDir")->AsInteger == 2)
					dir = pdOutput;
				else
					dir = pdInputOutput;

				if(FAccDbType == 1)
					FParamName = m_BuildQuery->FieldByName("SyMCMCkParamName")->AsString;
				else
					FParamName = m_BuildQuery->FieldByName("SyMCMCkParamOrcName")->AsString;

				FParam = m_StoredProc->Parameters->CreateParameter(FParamName,ftVariant, dir, m_BuildQuery->FieldByName("SyMCMCkParamSize")->AsInteger, NULL);
				FParam->DataType = datatype;
				FParam->Value = m_MasterDataSet->FieldValues[m_BuildQuery->FieldByName("SyMCMCkParamField")->AsString];
				m_BuildQuery->Next();
			}

			m_Connection->BeginTrans();
			m_StoredProc->ExecProc();
			m_MasterDataSet->Edit();
			m_BuildQuery->First();
			while (!m_BuildQuery->Eof)
			{
				if(m_BuildQuery->FieldByName("SyMCMCkParamDir")->AsInteger != 1)
				{
					if(m_MasterDataSet->FieldByName(m_BuildQuery->FieldByName("SyMCMCkParamField")->AsString)->ReadOnly)
						m_MasterDataSet->FieldByName(m_BuildQuery->FieldByName("SyMCMCkParamField")->AsString)->ReadOnly = false;

					if(FAccDbType == 1)
						FParamName = m_BuildQuery->FieldByName("SyMCMCkParamName")->AsString;
					else
						FParamName = m_BuildQuery->FieldByName("SyMCMCkParamOrcName")->AsString;

					m_MasterDataSet->FieldValues[m_BuildQuery->FieldByName("SyMCMCkParamField")->AsString] = m_StoredProc->Parameters->ParamValues[FParamName];
				}
				m_BuildQuery->Next();
			}
			m_MasterDataSet->Post();
			m_MasterDataSet->UpdateBatch(arAll);
			String Json = TTableJson::DataSetToJson(m_MasterDataSet);
			JSONObject->AddPair("Master",(TJSONObject*)TJSONObject::ParseJSONValue(Json));
			m_MasterDataSet->Close();
			m_BuildQuery->Close();
			m_Connection->CommitTrans();
			m_StoredProc->Close();

			JSONObject->AddPair("Status","s");
		}
    return JSONObject;
	}
	catch (Exception &e)
	{
		m_Connection->RollbackTrans();
		return SendFailMessage(asDSError, "������ݴ���" + e.Message);
	}
}
//---------------------------------------------------------------------------
TJSONObject*  TJServerDataSet::CloseOff(TJSONObject *Json, String ClassCode)
{
	TJSONObject *JSONObject = new TJSONObject();
	try
	{
		if(Json->Count > 0)
		{
			String sql, FParamName, DescStr;
			TFieldType datatype;
			TParameterDirection dir;
			TParameter *FParam;

			InitServerDataSet(ClassCode);

			TJSONObject *MstJson = (TJSONObject*)Json->GetValue("Master");
			m_MasterDataSet->Close();
			_di_ISuperObject pso = SO(MstJson->ToString());
			TTableJson::JsonToDataSet(m_MasterDataSet,pso);

			sql = " select SyMCMasterCloseProc,SyMCMCloseProcParam.*,SyDataTypeForCPP"
						" from SyMiddleClass,SyMCMCloseProcParam,SyDataType"
						" where SyMCCode='"+m_ClassCode+"' and SyMCMCClassCode=SyMCCode and SyDataTypeCode=SyMCMCParamDataType"
						" order by SyMCMCParamNo";
			m_BuildQuery->Close();
			m_BuildQuery->SQL->Clear();
			m_BuildQuery->SQL->Add(sql);
			m_BuildQuery->Open();
			if(m_BuildQuery->RecordCount < 1)
			{
				m_BuildQuery->Close();
				throw Exception("����洢������δ����.");
			}

			m_BuildQuery->First();
			m_StoredProc->Close();
			m_StoredProc->ProcedureName = m_BuildQuery->FieldByName("SyMCMasterCloseProc")->AsString;
			m_StoredProc->Parameters->Clear();
			while (!m_BuildQuery->Eof)
			{
				datatype = (TFieldType)m_BuildQuery->FieldByName("SyDataTypeForCPP")->AsInteger;
				if(m_BuildQuery->FieldByName("SyMCMCParamDir")->AsInteger == 1)
					dir = pdInput;
				else if(m_BuildQuery->FieldByName("SyMCMCParamDir")->AsInteger == 2)
					dir = pdOutput;
				else
					dir = pdInputOutput;

				if(FAccDbType == 1)
					FParamName = m_BuildQuery->FieldByName("SyMCMCParamName")->AsString;
				else
					FParamName = m_BuildQuery->FieldByName("SyMCMCParamOrcName")->AsString;

				FParam = m_StoredProc->Parameters->CreateParameter(FParamName,ftVariant, dir, m_BuildQuery->FieldByName("SyMCMCParamSize")->AsInteger, NULL);
				FParam->DataType = datatype;
				FParam->Value = m_MasterDataSet->FieldValues[m_BuildQuery->FieldByName("SyMCMCParamField")->AsString];
				m_BuildQuery->Next();
			}

			m_Connection->BeginTrans();
			m_StoredProc->ExecProc();
			m_MasterDataSet->Edit();
			m_BuildQuery->First();
			while (!m_BuildQuery->Eof)
			{
				if(m_BuildQuery->FieldByName("SyMCMCParamDir")->AsInteger != 1)
				{
					if(m_MasterDataSet->FieldByName(m_BuildQuery->FieldByName("SyMCMCParamField")->AsString)->ReadOnly)
						m_MasterDataSet->FieldByName(m_BuildQuery->FieldByName("SyMCMCParamField")->AsString)->ReadOnly = false;

					if(FAccDbType == 1)
						FParamName = m_BuildQuery->FieldByName("SyMCMCParamName")->AsString;
					else
						FParamName = m_BuildQuery->FieldByName("SyMCMCParamOrcName")->AsString;

					m_MasterDataSet->FieldValues[m_BuildQuery->FieldByName("SyMCMCParamField")->AsString] = m_StoredProc->Parameters->ParamValues[FParamName];
				}
				m_BuildQuery->Next();
			}
			m_MasterDataSet->Post();
			m_MasterDataSet->UpdateBatch(arAll);
			String Json = TTableJson::DataSetToJson(m_MasterDataSet);
			JSONObject->AddPair("Master",(TJSONObject*)TJSONObject::ParseJSONValue(Json));
			m_MasterDataSet->Close();
			m_BuildQuery->Close();
			m_Connection->CommitTrans();
			m_StoredProc->Close();

			JSONObject->AddPair("Status","s");
		}
		return JSONObject;
	}
	catch (Exception &e)
	{
		m_Connection->RollbackTrans();
		return SendFailMessage(asDSError, "�������ݴ���" + e.Message);
	}
}
//---------------------------------------------------------------------------
TJSONObject*  TJServerDataSet::Revoke(TJSONObject *Json, String ClassCode)
{
	TJSONObject *JSONObject = new TJSONObject();
	try
	{
		if(Json->Count > 0)
		{
			String sql, FParamName, DescStr;
			TFieldType datatype;
			TParameterDirection dir;
			TParameter *FParam;

			InitServerDataSet(ClassCode);

			TJSONObject *MstJson = (TJSONObject*)Json->GetValue("Master");
			m_MasterDataSet->Close();
			_di_ISuperObject pso = SO(MstJson->ToString());
			TTableJson::JsonToDataSet(m_MasterDataSet,pso);

			sql = " select SyMCMasterCancelProc,SyMCMCancelProcParam.*,SyDataTypeForCPP"
						" from SyMiddleClass,SyMCMCancelProcParam,SyDataType"
						" where SyMCCode='"+m_ClassCode+"' and SyMCMCanClassCode=SyMCCode and SyDataTypeCode=SyMCMCanParamDataType"
						" order by SyMCMCanParamNo";
			m_BuildQuery->Close();
			m_BuildQuery->SQL->Clear();
			m_BuildQuery->SQL->Add(sql);
			m_BuildQuery->Open();
			if(m_BuildQuery->RecordCount < 1)
			{
				m_BuildQuery->Close();
				throw Exception("���ϴ洢������δ����.");
			}

			m_BuildQuery->First();
			m_StoredProc->Close();
			m_StoredProc->ProcedureName = m_BuildQuery->FieldByName("SyMCMasterCancelProc")->AsString;
			m_StoredProc->Parameters->Clear();
			while (!m_BuildQuery->Eof)
			{
				datatype = (TFieldType)m_BuildQuery->FieldByName("SyDataTypeForCPP")->AsInteger;
				if(m_BuildQuery->FieldByName("SyMCMCanParamDir")->AsInteger == 1)
					dir = pdInput;
				else if(m_BuildQuery->FieldByName("SyMCMCanParamDir")->AsInteger == 2)
					dir = pdOutput;
				else
					dir = pdInputOutput;

				if(FAccDbType == 1)
					FParamName = m_BuildQuery->FieldByName("SyMCMCanParamName")->AsString;
				else
					FParamName = m_BuildQuery->FieldByName("SyMCMCanParamOrcName")->AsString;

				FParam = m_StoredProc->Parameters->CreateParameter(FParamName, ftVariant, dir, m_BuildQuery->FieldByName("SyMCMCanParamSize")->AsInteger, NULL);
				FParam->DataType = datatype;
				FParam->Value = m_MasterDataSet->FieldValues[m_BuildQuery->FieldByName("SyMCMCanParamField")->AsString];
				m_BuildQuery->Next();
			}

			m_Connection->BeginTrans();
			m_StoredProc->ExecProc();
			m_MasterDataSet->Edit();
			m_BuildQuery->First();
			while (!m_BuildQuery->Eof)
			{
				if(m_BuildQuery->FieldByName("SyMCMCanParamDir")->AsInteger != 1)
				{
					if(m_MasterDataSet->FieldByName(m_BuildQuery->FieldByName("SyMCMCanParamField")->AsString)->ReadOnly)
						m_MasterDataSet->FieldByName(m_BuildQuery->FieldByName("SyMCMCanParamField")->AsString)->ReadOnly = false;

					if(FAccDbType == 1)
						FParamName = m_BuildQuery->FieldByName("SyMCMCanParamName")->AsString;
					else
						FParamName = m_BuildQuery->FieldByName("SyMCMCanParamOrcName")->AsString;

					m_MasterDataSet->FieldValues[m_BuildQuery->FieldByName("SyMCMCanParamField")->AsString] = m_StoredProc->Parameters->ParamValues[FParamName];
				}
				m_BuildQuery->Next();
			}
			m_MasterDataSet->Post();
			m_MasterDataSet->UpdateBatch(arAll);
			String Json = TTableJson::DataSetToJson(m_MasterDataSet);
			JSONObject->AddPair("Master",(TJSONObject*)TJSONObject::ParseJSONValue(Json));
			m_MasterDataSet->Close();
			m_BuildQuery->Close();
			m_Connection->CommitTrans();
			m_StoredProc->Close();

			JSONObject->AddPair("Status","s");
		}
		return JSONObject;
	}
	catch (Exception &e)
	{
		m_Connection->RollbackTrans();
		return SendFailMessage(asDSError, "�������ݴ���" + e.Message);
	}
}
//---------------------------------------------------------------------------
TJSONObject*  TJServerDataSet::Tally(TJSONObject *Json, String ClassCode)
{
	TJSONObject *JSONObject = new TJSONObject();
	try
	{
		if(Json->Count > 0)
		{
			String sql, FParamName, DescStr;
			TFieldType datatype;
			TParameterDirection dir;
			TParameter *FParam;

			InitServerDataSet(ClassCode);

			TJSONObject *MstJson = (TJSONObject*)Json->GetValue("Master");
			m_MasterDataSet->Close();
			_di_ISuperObject pso = SO(MstJson->ToString());
			TTableJson::JsonToDataSet(m_MasterDataSet,pso);

			sql = " select SyMCMasterCheckOutProc,SyMCMCheckOutProcParam.*,SyDataTypeForCPP"
						" from SyMiddleClass,SyMCMCheckOutProcParam,SyDataType";
						" where SyMCCode='"+m_ClassCode+"' and SyMCMCOClassCode=SyMCCode and SyDataTypeCode=SyMCMCOParamDataType"
						" order by SyMCMCOParamNo";
			m_BuildQuery->Close();
			m_BuildQuery->SQL->Clear();
			m_BuildQuery->SQL->Add(sql);
			m_BuildQuery->Open();
			if(m_BuildQuery->RecordCount < 1)
			{
				m_BuildQuery->Close();
				throw Exception("���˴洢������δ����.");
			}

			m_BuildQuery->First();
			m_StoredProc->Close();
			m_StoredProc->ProcedureName = m_BuildQuery->FieldByName("SyMCMasterCheckOutProc")->AsString;
			m_StoredProc->Parameters->Clear();
			while (!m_BuildQuery->Eof)
			{
				datatype = (TFieldType)m_BuildQuery->FieldByName("SyDataTypeForCPP")->AsInteger;
				if(m_BuildQuery->FieldByName("SyMCMCOParamDir")->AsInteger == 1)
					dir = pdInput;
				else if(m_BuildQuery->FieldByName("SyMCMCOParamDir")->AsInteger == 2)
					dir = pdOutput;
				else
					dir = pdInputOutput;

				if(FAccDbType == 1)
					FParamName = m_BuildQuery->FieldByName("SyMCMCOParamName")->AsString;
				else
					FParamName = m_BuildQuery->FieldByName("SyMCMCOParamOrcName")->AsString;

				FParam = m_StoredProc->Parameters->CreateParameter(FParamName,ftVariant, dir, m_BuildQuery->FieldByName("SyMCMCOParamSize")->AsInteger, NULL);
				FParam->DataType = datatype;
				FParam->Value = m_MasterDataSet->FieldValues[m_BuildQuery->FieldByName("SyMCMCOParamField")->AsString];
				m_BuildQuery->Next();
			}

			m_Connection->BeginTrans();
			m_StoredProc->ExecProc();
			m_MasterDataSet->Edit();
			m_BuildQuery->First();
			while (!m_BuildQuery->Eof)
			{
				if(m_BuildQuery->FieldByName("SyMCMCOParamDir")->AsInteger != 1)
				{
					if(m_MasterDataSet->FieldByName(m_BuildQuery->FieldByName("SyMCMCOParamField")->AsString)->ReadOnly)
						m_MasterDataSet->FieldByName(m_BuildQuery->FieldByName("SyMCMCOParamField")->AsString)->ReadOnly = false;

					if(FAccDbType == 1)
						FParamName = m_BuildQuery->FieldByName("SyMCMCOParamName")->AsString;
					else
						FParamName = m_BuildQuery->FieldByName("SyMCMCOParamOrcName")->AsString;

					m_MasterDataSet->FieldValues[m_BuildQuery->FieldByName("SyMCMCOParamField")->AsString] = m_StoredProc->Parameters->ParamValues[FParamName];
				}
				m_BuildQuery->Next();
			}
			m_MasterDataSet->Post();
			m_MasterDataSet->UpdateBatch(arAll);
			String Json = TTableJson::DataSetToJson(m_MasterDataSet);
			JSONObject->AddPair("Master",(TJSONObject*)TJSONObject::ParseJSONValue(Json));
			m_MasterDataSet->Close();
			m_BuildQuery->Close();
			m_Connection->CommitTrans();
			m_StoredProc->Close();

			JSONObject->AddPair("Status","s");
		}
		return JSONObject;
	}
	catch (Exception &e)
	{
		m_Connection->RollbackTrans();
		return SendFailMessage(asDSError, "�������ݴ���" + e.Message);
	}
}
//---------------------------------------------------------------------------
TJSONObject*  TJServerDataSet::ExecSQL(TJSONObject *Json)
{
	TJSONObject *JSONObject = new TJSONObject();
	try
	{
		if(Json->Count > 0)
		{
			String sql = Json->GetValue("SQLStr")->Value();

			if(sql.Trim().IsEmpty())
				throw Exception( "ִ�������δ��ֵ.");

			int affectedrows;
			TExecuteOptions eos;
			eos << eoExecuteNoRecords;
			m_Connection->Execute(sql, affectedrows, eos);
			WriteUserLog(12, sql);

			JSONObject->AddPair("Status","s");
		}
		return JSONObject;
	}
	catch (Exception &e)
	{
		return SendFailMessage(asDSError, "ִ��SQL������" + e.Message);
	}
}
//---------------------------------------------------------------------------
TJSONObject*  TJServerDataSet::QueryOpen(String SQLText)
{
	TJSONObject *JSONObject = new TJSONObject();
	try
	{
		if(SQLText.Trim().IsEmpty())
			throw Exception( "ִ�������δ��ֵ.");

		TJSONObject *MstJson = DataSetToJsonObject(SQLText);
		JSONObject->AddPair("Master",(TJSONObject*)(MstJson->Clone()));
		JSONObject->AddPair("Status","s");
		return JSONObject;
	}
	catch (Exception &e)
	{
		return SendFailMessage(asQError, "�����ݼ�����" + e.Message);
	}
}
//---------------------------------------------------------------------------
TJSONObject*  TJServerDataSet::QueryExecSQL(String SQLText)
{
	TJSONObject *JSONObject = new TJSONObject();
	try
	{
		if(SQLText.Trim().IsEmpty())
			throw Exception( "ִ�������δ��ֵ.");

		String sql = SQLText;
		int affectedrows;
		TExecuteOptions eos;
		eos << eoExecuteNoRecords;
		m_Connection->Execute(sql, affectedrows, eos);
		WriteUserLog(12, sql);

		JSONObject->AddPair("Status","s");
		JSONObject->AddPair("RowsAffected",affectedrows);
		return JSONObject;
	}
	catch (Exception &e)
	{
		return SendFailMessage(asQError, "ִ��SQL������" + e.Message);
	}
}
//---------------------------------------------------------------------------
// ִ��ϵͳ��
//---------------------------------------------------------------------------
TJSONObject*  TJServerDataSet::QuerySysOpen(String SQLText)
{
	TJSONObject *JSONObject = new TJSONObject();
	try
	{
		if(SQLText.Trim().IsEmpty())
			throw Exception( "ִ�������δ��ֵ.");

		TJSONObject *MstJson = DataSetSysToJsonObject(SQLText);
		JSONObject->AddPair("Master",(TJSONObject*)(MstJson->Clone()));
		JSONObject->AddPair("Status","s");
		return JSONObject;
	}
	catch (Exception &e)
	{
		return SendFailMessage(asQError, "�����ݼ�����" + e.Message);
	}
}
//---------------------------------------------------------------------------
TJSONObject*  TJServerDataSet::QuerySysExecSQL(String SQLText)
{
	TJSONObject *JSONObject = new TJSONObject();
	try
	{
		if(SQLText.Trim().IsEmpty())
			throw Exception( "ִ�������δ��ֵ.");

		String sql = SQLText;
		int affectedrows;
		TExecuteOptions eos;
		eos << eoExecuteNoRecords;
		m_BuildConn->Execute(sql, affectedrows, eos);
		WriteUserLog(12, sql);

		JSONObject->AddPair("Status","s");
		JSONObject->AddPair("RowsAffected",affectedrows);
		return JSONObject;
	}
	catch (Exception &e)
	{
		return SendFailMessage(asQError, "ִ��SQL������" + e.Message);
	}
}
//---------------------------------------------------------------------------
TJSONObject*  TJServerDataSet::SendErrorMessage(int errortype, int errorcode, String errormessage)
{
	TJSONObject *JSONObject = new TJSONObject();
	try
	{
		JSONObject->AddPair("Operate",asDSError);
		JSONObject->AddPair("Status","f");
		JSONObject->AddPair("ErrorType",errortype);
		JSONObject->AddPair("ErrorCode",errorcode);
		JSONObject->AddPair("ErrorMessage",errormessage);
		return JSONObject;
	}
	catch (Exception &e)
	{
		return JSONObject;
	}
}
// ---------------------------------------------------------------------------
TJSONObject*  TJServerDataSet::SendFailMessage(int operate, String errormessage)
{
	TJSONObject *JSONObject = new TJSONObject();
	try
	{
		JSONObject->AddPair("Operate",operate);
		JSONObject->AddPair("Status","f");
		JSONObject->AddPair("ErrorMessage",errormessage);
		return JSONObject;
	}
	catch (Exception &e)
	{
		return SendErrorMessage(100, 100, "����ʧ����Ϣ����\r\n" + e.Message);
	}
}
// ---------------------------------------------------------------------------
void  TJServerDataSet::WriteUserLog(int type, String Desc)
{
  DMServerContainer->NotifyWriteLog(FUserCode,FUserIP,FAcckBook,m_ClassCode,type,Desc);
//	try
//	{
//		if(Desc.Length()>250)
//			Desc.SetLength(250);
//		String InsertString = " Insert into SyUserOpLog(SyULogUserCode,SyULogIP,SyULogClassCode,SyULogOperType,SyULogAccbook,SyULogDesc)"
//													" Values ('"+FUserCode+"','"+FUserIP+"','"+m_ClassCode+"',"+IntToStr(type)+",'"+FAcckBook+"',"+QuotedStr(Desc)+")";
//		LogQuery->Close();
//		LogQuery->SQL->Text = InsertString;
//		LogQuery->ExecSQL();
//	}
//	catch (...)
//	{
//	}
}
// ---------------------------------------------------------------------------
String  TJServerDataSet::BuildLinkFields()
{
	String s;
	if (m_DataSetType == 1) return "";
	for (int i = 0; i < m_MasterLinkFields->Count; i++)
	{
		s += m_MasterLinkFields->Strings[i] + "=" + m_DetailLinkFields->Strings[i] + "\t";
	}
	return s;
}
// ---------------------------------------------------------------------------
void  TJServerDataSet::ExtractLinkFields(String value)
{
	String s;
	int index;
	if (m_DataSetType == 1) return;
	index = value.Pos0("=");
	m_MasterLinkFields->Clear();
	m_DetailLinkFields->Clear();
	while (index > -1)
	{
		s = value.SubString0(0, index ).Trim();
		m_MasterLinkFields->Add(s);
		value.Delete0(0, index+1);
		index = value.Pos0("\t");
		s = value.SubString0(0, index);
		m_DetailLinkFields->Add(s);
		value.Delete0(0, index+1);
		index = value.Pos0("=");
	}
}
// ---------------------------------------------------------------------------
String  TJServerDataSet::BuildLinkStr()
{
	String s;
	if (m_DataSetType == 1)	return "";
	for (int i = 0; i < m_MasterLinkFields->Count; i++)
	{
		s += m_MasterLinkFields->Strings[i] + "=" + m_DetailLinkFields->Strings[i];
		if (i < m_MasterLinkFields->Count - 1)
			s += " and ";
	}
	return s;
}
// ---------------------------------------------------------------------------
void  TJServerDataSet::BuildDetailFilter()
{
	String filterstr;
	int ftype;
	for (int i = 0; i < m_MasterLinkFields->Count; i++)
	{
		ftype = CheckFieldType
		(m_DetailDataSet->FieldByName(m_DetailLinkFields->Strings[i]));
		if (ftype == 1)
			filterstr += m_DetailLinkFields->Strings[i] + "='" +
			m_MasterDataSet->FieldByName(m_MasterLinkFields->Strings[i])->AsString + "'";
		else
			filterstr += m_DetailLinkFields->Strings[i] + "=" +
			m_MasterDataSet->FieldByName(m_MasterLinkFields->Strings[i])->AsString;

		if (i < m_MasterLinkFields->Count - 1)
			filterstr += " and ";
	}
	m_DetailDataSet->Filtered = false;
	m_DetailDataSet->Filter = filterstr;
	m_DetailDataSet->Filtered = true;
}
// ---------------------------------------------------------------------------
int  TJServerDataSet::CheckFieldType(TField *Field)
{
	switch (Field->DataType)
	{
		case ftString:
		case ftDate:
		case ftTime:
		case ftDateTime:
		case ftMemo:
		case ftFixedChar:
		case ftWideString:
			return 1;
		case ftSmallint:
		case ftInteger:
		case ftBoolean:
		case ftBytes:
		case ftWord:
		case ftFloat:
		case ftCurrency:
		case ftBCD:
		case ftAutoInc:
		case ftLargeint:
			return 2;
		default:
			return 0;
	}
}
// ---------------------------------------------------------------------------