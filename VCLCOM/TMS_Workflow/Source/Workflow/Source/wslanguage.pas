unit wsLanguage;

interface

implementation
uses wsRes;

const LanguageStr = 
'[strings]' + #13#10 +
'SAttachment=����         ' + #13#10 +
'SConfirmDeleteItems=A��ȷ��Ҫɾ��ѡ�еĽڵ���?' + #13#10 +
'SConfirmDeleteAttachments=��ȷ��Ҫɾ��Ҫ�Ƴ�ѡ�еĸ�����?' + #13#10 +
'SConfirmOpenMoreThanOneFile=����Ҫͬʱ�򿪶���ļ���?' + #13#10 +
'SCurrentWorkflow=��ǰ������' + #13#10 +
'SFileName=�ļ���' + #13#10 +
'SEnterStatusName=����״̬����' + #13#10 +
'SEnterVariableCaption=�����������' + #13#10 +
'SExpiration=����' + #13#10 +
'SStatusList=״̬�б�' + #13#10 +
'SVariables=�����б�' + #13#10 +
'STask=����' + #13#10 +
'SVariable=����' + #13#10 +
'SColumnKey=���' + #13#10 +
'SColumnName=��������' + #13#10 +
'SColumnWorkflow=������' + #13#10 +
'SColumnSubject=�������' + #13#10 +
'SColumnStatus=״̬' + #13#10 +
'SColumnUser=������' + #13#10 +
'STabFields=�ֶ�' + #13#10 +
'STabTask=����' + #13#10 +
'STabWorkflow=����ͼ' + #13#10 +
'STabLog=��־' + #13#10 +
'SConfirmDeleteWorkDef=��ȷ��Ҫɾ��ѡ�еĹ�����������?' + #13#10 +
'SConfirmCloseAndDiscard=�رմ��壬�����������޸�?' + #13#10 +
'SModified=���޸�' + #13#10 +
'SAllObjects=���ж���' + #13#10 +
'SConfirmNewDiagramAndDiscard=����������ͼ������Ŀǰ�������޸�?' + #13#10 +
'SConfirmOpenDiagramAndDiscard=��������ͼ������Ŀǰ�������޸�?' + #13#10 +
'SWorkflowCorrect=��������ͼ����ȷ��.' + #13#10 +
'SWorkflowDefinition=��������ͼ����' + #13#10 +
'STypeWorkDefName=��������Ƶ�����ͼ������' + #13#10 +
'SErrorParamNotFound=ִ�в�ѯʱ�Ҳ������� %s.' + #13#10 +
'STrueOutput=��' + #13#10 +
'SFalseOutput=��' + #13#10 +
'SBlockStart=��ʼ' + #13#10 +
'SBlockEnd=����' + #13#10 +
'SBlockTask=�������' + #13#10 +
'SBlockApproval=�����ڵ�' + #13#10 +
'SBlockScript=�ű��ڵ�' + #13#10 +
'STaskApprovalName=����' + #13#10 +
'STaskApprovalSubject=����' + #13#10 +
'STaskApprovalDescription=����׼�򲵻�.' + #13#10 +
'STaskApprovalStatusOpened=ִ��' + #13#10 +
'STaskApprovalStatusApproved=ͬ��' + #13#10 +
'STaskApprovalStatusRejected=����' + #13#10 +
'SToolbarStart=��ʼ' + #13#10 +
'SToolbarEnd=����' + #13#10 +
'SToolbarError=����������' + #13#10 +
'SToolbarSourceConnect=Դ������' + #13#10 +
'SToolbarTargetConnect=Ŀ��������' + #13#10 +
'SToolbarLineJoin=��������' + #13#10 +
'SToolbarTransition=ֱ����' + #13#10 +
'SToolbarSideTransition=������' + #13#10 +
'SToolbarArcTransition=��������' + #13#10 +
'SToolbarFork=�ֲ�' + #13#10 +
'SToolbarJoin=����' + #13#10 +
'SToolbarTask=����' + #13#10 +
'SToolbarApproval=����' + #13#10 +
'SToolbarDecision=�ж�' + #13#10 +
'SToolbarWorkflowCategory=������' + #13#10 +
'SToolbarRunWorkflow=���й�����' + #13#10 +
'SToolbarScript=�ű�' + #13#10 +
'SErrorFileAlreadyOpen=�ļ��Ѿ���.' + #13#10 +
'SConfirmUpdateModifiedAttachment=�ļ�"%s"�Ѿ��޸ģ�������µ���������?' + #13#10 +
'SWarningFieldValueRequired=��Ҫ��"%s" ��ֵ.' + #13#10 +
'SErrorInsertCannotGetKey=�� %s ����ʱ����.���ܷ�������ֵ.' + #13#10 +
'SErrorRecordNotFoundKey=�����ݿ����Ҳ���%s . ����ֵ: %s' + #13#10 +
'SErrorUpdateEmptyKey=���ܸ������� %s�����ݿ�. ������ֵ.' + #13#10 +
'SErrorDeleteEmptyKey=�����ݿ��в���ɾ�� %s . ������ֵ.' + #13#10 +
'SErrorUndefinedVariable=���� "%s" ������.' + #13#10 +
'SErrorScriptBlockCompile=����ű��ڵ����.' + #13#10 +
'SLogColOperation=����' + #13#10 +
'SLogColDate=����' + #13#10 +
'SLogColTime=ʱ��' + #13#10 +
'SLogColUserId=�û�' + #13#10 +
'SLogColInfo=��Ϣ' + #13#10 +
'STaskExpiredOn=��������� %s' + #13#10 +
'STaskLogCreate=����' + #13#10 +
'STaskLogInfo=��Ϣ' + #13#10 +
'STaskLogUpdate=����' + #13#10 +
'STaskLogStatusChange=״̬�ı�' + #13#10 +
'SStatusChangeInfo=�� "%s" �� "%s"' + #13#10 +
'SWorkInsExecutionError1=������ִ�й��̳���. �������Ѿ�����.\#13#10\#13#10������ϢΪ: "%s"'+ #13#10 +
'SWorkInsExecutionError2=%s\#13#10\#13#10����ڵ���Ϣ�������:\#13#10����: %s\#13#10����: %s\#13#10����: \#13#10"%s"' + #13#10 +
'SNextBlockUndefined=�¸��ڵ�δ����. ���ܼ���ִ��.' + #13#10 +
'STaskNotCreatedUserNotFound=����ʵ�����ܴ�������Ϊ����������("%s")������.' + #13#10 +
'' + #13#10 +
'[fApprovalEditor]' + #13#10 +
'fmApprovalEditor.Self.Caption=����' + #13#10 +
'fmApprovalEditor.tsGeneral.Caption=ͨ��' + #13#10 +
'fmApprovalEditor.lbSubject.Caption=����' + #13#10 +
'fmApprovalEditor.lbDescription.Caption=����' + #13#10 +
'fmApprovalEditor.lbAssignment.Caption=����' + #13#10 +
'fmApprovalEditor.cbMailNotification.Caption=����֪ͨ�ʼ�' + #13#10 +
'fmApprovalEditor.tsStatus.Caption=״̬' + #13#10 +
'fmApprovalEditor.lbStatusList.Caption=״̬�б�("*"��ͷ��־Ϊ���״̬' + #13#10 +
'fmApprovalEditor.tsAttachments.Caption=����' + #13#10 +
'fmApprovalEditor.tsFields.Caption=�ֶ�' + #13#10 +
'fmApprovalEditor.btCancel.Caption=ȡ��' + #13#10 +
'fmApprovalEditor.btOk.Caption=ȷ��' + #13#10 +
'fmApprovalEditor.tsExpiration.Caption=����' + #13#10 +
'' + #13#10 +
'[fAttachmentEditor]' + #13#10 +
'fmAttachmentEditor.Self.Caption=����' + #13#10 +
'fmAttachmentEditor.tsAttachments.Caption=ͨ��' + #13#10 +
'fmAttachmentEditor.btAdd.Caption=����(&A)' + #13#10 +
'fmAttachmentEditor.btDelete.Caption=ɾ��(&D)' + #13#10 +
'fmAttachmentEditor.btCancel.Caption=ȡ��' + #13#10 +
'fmAttachmentEditor.btOk.Caption=ȷ��' + #13#10 +
'' + #13#10 +
'[fAttachmentFrame]' + #13#10 +
'frAttachmentFrame.OpenDialog1.Filter=�����ļ�(*.*)|*.*' + #13#10 +
'frAttachmentFrame.OpenDialog1.Title=���븽��' + #13#10 +
'frAttachmentFrame.miOpen.Caption=(&O)�򿪸���...' + #13#10 +
'frAttachmentFrame.miInsert.Caption=(&I)��������...' + #13#10 +
'frAttachmentFrame.miRemove.Caption=(&R)�Ƴ�����' + #13#10 +
'frAttachmentFrame.acInsert.Hint=��������' + #13#10 +
'frAttachmentFrame.acRemove.Hint=�Ƴ�����' + #13#10 +
'frAttachmentFrame.acOpen.Hint=�򿪸���' + #13#10 +
'frAttachmentFrame.acLargeIcons.Caption=��ͼ��' + #13#10 +
'frAttachmentFrame.acLargeIcons.Hint=��ͼ��' + #13#10 +
'frAttachmentFrame.acSmallIcons.Caption=Сͼ��' + #13#10 +
'frAttachmentFrame.acSmallIcons.Hint=Сͼ��' + #13#10 +
'frAttachmentFrame.acList.Caption=�б�' + #13#10 +
'frAttachmentFrame.acList.Hint=�б�' + #13#10 +
'frAttachmentFrame.acReport.Caption=����' + #13#10 +
'frAttachmentFrame.acReport.Hint=����' + #13#10 +
'' + #13#10 +
'[fAttachPermissions]' + #13#10 +
'frAttachPermissions.cbShowAttachments.Caption=��ʾ����' + #13#10 +
'frAttachPermissions.cbDelAttachment.Caption=�����Ƴ�����' + #13#10 +
'frAttachPermissions.cbInsAttachment.Caption=�������븽��' + #13#10 +
'frAttachPermissions.cbEditAttachment.Caption=�����޸ĸ���' + #13#10 +
'' + #13#10 +
'[fDecisionEditor]' + #13#10 +
'fmDecisionEditor.Self.Caption=�ж�' + #13#10 +
'fmDecisionEditor.TabSheet1.Caption=ͨ��' + #13#10 +
'fmDecisionEditor.Label1.Caption=��������ʽ' + #13#10 +
'fmDecisionEditor.btCancel.Caption=ȡ��' + #13#10 +
'fmDecisionEditor.btOk.Caption=ȷ��' + #13#10 +
'' + #13#10 +
'[fExpirationFrame]' + #13#10 +
'frExpirationFrame.rbExpNever.Caption=���񲻹���' + #13#10 +
'frExpirationFrame.rbExpTerm.Caption=��������(&T):' + #13#10 +
'frExpirationFrame.cbTermType.Items0=����' + #13#10 +
'frExpirationFrame.cbTermType.Items1=����' + #13#10 +
'frExpirationFrame.cbTermType.Items2=����' + #13#10 +
'frExpirationFrame.rbExpDate.Caption=��������ʱ��:' + #13#10 +
'frExpirationFrame.rbExpCustom.Caption=����ʱ�����ʽ(&C):' + #13#10 +
'frExpirationFrame.lbStatus.Caption=����״̬(&S):' + #13#10 +
'' + #13#10 +
'[fFieldFrame]' + #13#10 +
'frFieldFrame.Label1.Caption=�ı�����:' + #13#10 +
'frFieldFrame.Label2.Caption=����������:' + #13#10 +
'frFieldFrame.cbReadOnly.Caption=ֻ��' + #13#10 +
'frFieldFrame.cbRequired.Caption=����' + #13#10 +
'frFieldFrame.btAdd.Caption=����' + #13#10 +
'frFieldFrame.btDelete.Caption=ɾ��' + #13#10 +
'' + #13#10 +
'[fStatusFrame]' + #13#10 +
'frStatusFrame.lbStatusName.Caption=״̬����:' + #13#10 +
'frStatusFrame.chCompletion.Caption=����' + #13#10 +
'frStatusFrame.chHidden.Caption=����' + #13#10 +
'frStatusFrame.btAdd.Caption=����' + #13#10 +
'frStatusFrame.btDelete.Caption=ɾ��' + #13#10 +
'' + #13#10 +
'[fscripteditor]' + #13#10 +
'fmScriptEditor.Self.Caption=�ű�' + #13#10 +
'fmScriptEditor.tsScript.Caption=�ű�����' + #13#10 +
'fmScriptEditor.btCancel.Caption=ȡ��' + #13#10 +
'fmScriptEditor.btOk.Caption=ȷ��' + #13#10 +
'' + #13#10 +
'[fTaskEditor]' + #13#10 +
'fmTaskEditor.Self.Caption=������' + #13#10 +
'fmTaskEditor.tsGeneral.Caption=ͨ��' + #13#10 +
'fmTaskEditor.Label1.Caption=����' + #13#10 +
'fmTaskEditor.Label2.Caption=����' + #13#10 +
'fmTaskEditor.Label3.Caption=����' + #13#10 +
'fmTaskEditor.cbMailNotification.Caption=����֪ͨ�ʼ�' + #13#10 +
'fmTaskEditor.TabSheet2.Caption=״̬' + #13#10 +
'fmTaskEditor.Label4.Caption=״̬�б�("*"��ͷ��־Ϊ���״̬)' + #13#10 +
'fmTaskEditor.tsAttachment.Caption=����' + #13#10 +
'fmTaskEditor.tsFields.Caption=�ֶ�' + #13#10 +
'fmTaskEditor.btAdd.Caption=(&A)����' + #13#10 +
'fmTaskEditor.btDelete.Caption=(&D)ɾ��' + #13#10 +
'fmTaskEditor.btCancel.Caption=ȡ��' + #13#10 +
'fmTaskEditor.btOk.Caption=ȷ��' + #13#10 +
'fmTaskEditor.tsExpiration.Caption=����' + #13#10 +
'' + #13#10 +
'[fTaskInstanceView]' + #13#10 +
'frTaskInstanceView.Label1.Caption=����:' + #13#10 +
'frTaskInstanceView.Label2.Caption=״̬:' + #13#10 +
'' + #13#10 +
'[ftasklist]' + #13#10 +
'fmTaskList.Self.Caption=�����б�' + #13#10 +
'fmTaskList.btSaveChanges.Caption=(&S)�����޸�' + #13#10 +
'fmTaskList.acShowOnlyOpen.Caption=ֻ��ʾִ��������' + #13#10 +
'fmTaskList.acShowAllTasks.Caption=��ʾ��������' + #13#10 +
'fmTaskList.miView.Caption=(&V)��ͼ' + #13#10 +
'' + #13#10 +
'[fTransitionEditor]' + #13#10 +
'fmTransitionEditor.Self.Caption=��ת����' + #13#10 +
'fmTransitionEditor.TabSheet1.Caption=ͨ��' + #13#10 +
'fmTransitionEditor.Label1.Caption=��������ʽ' + #13#10 +
'fmTransitionEditor.btCancel.Caption=ȡ��' + #13#10 +
'fmTransitionEditor.btOk.Caption=ȷ��' + #13#10 +
'' + #13#10 +
'[fvariableeditor]' + #13#10 +
'fmVariableEditor.Self.Caption=����' + #13#10 +
'fmVariableEditor.TabSheet1.Caption=ͨ��' + #13#10 +
'fmVariableEditor.Label1.Caption=ֵ' + #13#10 +
'fmVariableEditor.btAdd.Caption=(&A)����' + #13#10 +
'fmVariableEditor.btDelete.Caption=(&D)ɾ��' + #13#10 +
'fmVariableEditor.btCancel.Caption=ȡ��' + #13#10 +
'fmVariableEditor.btOk.Caption=ȷ��' + #13#10 +
'' + #13#10 +
'[fWorkDefManager]' + #13#10 +
'fmWorkDefManager.Self.Caption=����������' + #13#10 +
'fmWorkDefManager.btAdd.Caption=(&A)����' + #13#10 +
'fmWorkDefManager.btEdit.Caption=(&E)�޸�' + #13#10 +
'fmWorkDefManager.btDelete.Caption=(&D)ɾ��' + #13#10 +
'fmWorkDefManager.btRefresh.Caption=(&R)ˢ��' + #13#10 +
'fmWorkDefManager.btRunNew.Caption=����������' + #13#10 +
'fmWorkDefManager.acImport.Caption=(&I)���붨��...' + #13#10 +
'fmWorkDefManager.acExport.Caption=(&x)��������...' + #13#10 +
'fmWorkDefManager.OpenDialog1.Filter=���������� (*.wfd)|*.wfd' + #13#10 +
'fmWorkDefManager.OpenDialog1.Title=���빤��������' + #13#10 +
'fmWorkDefManager.SaveDialog1.Filter=���������� (*.wfd)|*.wfd' + #13#10 +
'fmWorkDefManager.SaveDialog1.Title=��������������' + #13#10 +
'' + #13#10 +
'[fWorkflowEditor]' + #13#10 +
'fmWorkflowEditor.Self.Caption=����ͼ�����' + #13#10 +
'fmWorkflowEditor.ToolBar1.Caption=������1' + #13#10 +
'fmWorkflowEditor.btCheck.Caption=�������ͼ' + #13#10 +
'fmWorkflowEditor.ToolBar2.Caption=������2' + #13#10 +
'fmWorkflowEditor.DgrColorSelector1.Hint=ͼ����ɫ' + #13#10 +
'fmWorkflowEditor.DgrGradientDirectionSelector1.Hint=����' + #13#10 +
'fmWorkflowEditor.DgrBrushStyleSelector1.Hint=�����' + #13#10 +
'fmWorkflowEditor.DgrShadowSelector1.Hint=��Ӱ' + #13#10 +
'fmWorkflowEditor.DgrPenStyleSelector1.Hint=�߷��' + #13#10 +
'fmWorkflowEditor.DgrPenColorSelector1.Hint=����ɫ' + #13#10 +
'fmWorkflowEditor.DgrPenWidthSelector1.Hint=�߿���' + #13#10 +
'fmWorkflowEditor.DgrTextColorSelector1.Hint=�ı���ɫ' + #13#10 +
'fmWorkflowEditor.DiagramToolBar2.Category=������' + #13#10 +
'fmWorkflowEditor.File1.Caption=(&F)�ļ�' + #13#10 +
'fmWorkflowEditor.Edit2.Caption=(&E�༭' + #13#10 +
'fmWorkflowEditor.Copyasimage1.Caption=����ΪͼƬ' + #13#10 +
'fmWorkflowEditor.View1.Caption=(&V)��ͼ' + #13#10 +
'fmWorkflowEditor.Zoom1.Caption=(&Z)����' + #13#10 +
'fmWorkflowEditor.Edittext1.Caption=(&O)����' + #13#10 +
'fmWorkflowEditor.Sourcearrow2.Caption=Դ��ͷ' + #13#10 +
'fmWorkflowEditor.Targetarrow2.Caption=Ŀ���ͷ' + #13#10 +
'fmWorkflowEditor.Workflow1.Caption=(&W)������' + #13#10 +
'fmWorkflowEditor.Variables1.Caption=(&V)����...' + #13#10 +
'fmWorkflowEditor.Attachments1.Caption=(&A)����...' + #13#10 +
'fmWorkflowEditor.DiagramCut1.Caption=����(&t)' + #13#10 +
'fmWorkflowEditor.DiagramCut1.Hint=����|���е�������' + #13#10 +
'fmWorkflowEditor.DiagramCopy1.Caption=(&C)����' + #13#10 +
'fmWorkflowEditor.DiagramCopy1.Hint=����|���Ƶ�������' + #13#10 +
'fmWorkflowEditor.DiagramPaste1.Caption=(&P)ճ��' + #13#10 +
'fmWorkflowEditor.DiagramPaste1.Hint=ճ��|�Ӽ�����ճ��' + #13#10 +
'fmWorkflowEditor.DiagramSelectAll1.Caption=ѡ������(&l)' + #13#10 +
'fmWorkflowEditor.DiagramSelectAll1.Hint=ѡ������|ѡ�����ж���' + #13#10 +
'fmWorkflowEditor.DiagramDelete1.Caption=(&D)ɾ��' + #13#10 +
'fmWorkflowEditor.DiagramDelete1.Hint=ɾ������|ɾ��ѡ�ж���' + #13#10 +
'fmWorkflowEditor.DiagramEditText1.Caption=�༭ �ı�(&t)' + #13#10 +
'fmWorkflowEditor.DiagramEditText1.Hint=�༭ �ı�' + #13#10 +
'fmWorkflowEditor.DiagramViewLeftRuler1.Caption=(&L)��߱��' + #13#10 +
'fmWorkflowEditor.DiagramViewLeftRuler1.Hint=��ʾ/������߱��' + #13#10 +
'fmWorkflowEditor.DiagramViewTopRuler1.Caption=(&T)�������' + #13#10 +
'fmWorkflowEditor.DiagramViewTopRuler1.Hint=��ʾ/���ض������' + #13#10 +
'fmWorkflowEditor.DiagramShapeColor1.Caption=ͼ����ɫ...' + #13#10 +
'fmWorkflowEditor.DiagramShapeColor1.Hint=ͼ����ɫ' + #13#10 +
'fmWorkflowEditor.DiagramLineColor1.Caption=����ɫ...' + #13#10 +
'fmWorkflowEditor.DiagramLineColor1.Hint=����ɫ' + #13#10 +
'fmWorkflowEditor.DiagramBlockPicture1.Caption=ͼƬ...' + #13#10 +
'fmWorkflowEditor.DiagramBlockPicture1.Hint=����ͼƬ' + #13#10 +
'fmWorkflowEditor.DiagramArrowShape1.Caption=û��' + #13#10 +
'fmWorkflowEditor.DiagramArrowShape2.Caption=ʵ��ͷ' + #13#10 +
'fmWorkflowEditor.DiagramArrowShape3.Caption=�߼�ͷ' + #13#10 +
'fmWorkflowEditor.DiagramArrowShape4.Caption=Բ��' + #13#10 +
'fmWorkflowEditor.DiagramArrowShape5.Caption=����' + #13#10 +
'fmWorkflowEditor.DiagramArrowShape6.Caption=��������' + #13#10 +
'fmWorkflowEditor.DiagramArrowShape7.Caption=û��' + #13#10 +
'fmWorkflowEditor.DiagramArrowShape8.Caption=ʵ��ͷ' + #13#10 +
'fmWorkflowEditor.DiagramArrowShape9.Caption=�߼�ͷ' + #13#10 +
'fmWorkflowEditor.DiagramArrowShape10.Caption=Բ��' + #13#10 +
'fmWorkflowEditor.DiagramArrowShape11.Caption=����' + #13#10 +
'fmWorkflowEditor.DiagramArrowShape12.Caption=��������' + #13#10 +
'fmWorkflowEditor.DiagramBringToFront1.Caption=����ǰ��' + #13#10 +
'fmWorkflowEditor.DiagramBringToFront1.Hint=����ǰ��' + #13#10 +
'fmWorkflowEditor.acNewDiagram.Caption=(&N)������ͼ...' + #13#10 +
'fmWorkflowEditor.acNewDiagram.Hint=������ͼ|����������ͼ' + #13#10 +
'fmWorkflowEditor.acOpenDiagram.Caption=(&O)������ͼ...' + #13#10 +
'fmWorkflowEditor.acOpenDiagram.Hint=������ͼ|���Ѵ�������ͼ' + #13#10 +
'fmWorkflowEditor.acSaveDiagram.Caption=(&S)��������ͼ...' + #13#10 +
'fmWorkflowEditor.acSaveDiagram.Hint=��������ͼ|���浱ǰ����ͼ' + #13#10 +
'fmWorkflowEditor.acPrintDiagram.Caption=(&P)��ӡ...' + #13#10 +
'fmWorkflowEditor.acPrintDiagram.Hint=��ӡ����ͼ|��ӡ��ǰ����ͼ' + #13#10 +
'fmWorkflowEditor.acPreviewDiagram.Caption=��ӡԤ��(&v)...' + #13#10 +
'fmWorkflowEditor.acPreviewDiagram.Hint=��ӡԤ��|��ӡԤ����ǰ����ͼ' + #13#10 +
'fmWorkflowEditor.DiagramSendToBack1.Caption=���ں���' + #13#10 +
'fmWorkflowEditor.DiagramSendToBack1.Hint=���ں���' + #13#10 +
'fmWorkflowEditor.DiagramControlFont1.Caption=����...' + #13#10 +
'fmWorkflowEditor.DiagramControlFont1.Hint=�ı��ı�����' + #13#10 +
'fmWorkflowEditor.DiagramGrid1.Caption=�׷���' + #13#10 +
'fmWorkflowEditor.DiagramGrid1.Hint=��ʾ/���ص׷���' + #13#10 +
'fmWorkflowEditor.DiagramGradientSetting1.Caption=�������� ...' + #13#10 +
'fmWorkflowEditor.DiagramGradientSetting1.Hint=ѡ�񽥱�' + #13#10 +
'fmWorkflowEditor.DiagramShadow1.Caption=��Ӱ���� ...' + #13#10 +
'fmWorkflowEditor.DiagramShadow1.Hint=��Ӱ����' + #13#10 +
'fmWorkflowEditor.DiagramUndoAction1.Caption=����' + #13#10 +
'fmWorkflowEditor.DiagramRedoAction1.Caption=����' + #13#10 +
'fmWorkflowEditor.acAutomaticNodes.Caption=(&A)�Զ�����' + #13#10 +
'fmWorkflowEditor.DiagramConnectionId1.Caption=��ת' + #13#10 +
'fmWorkflowEditor.DiagramConnectionId1.Hint=��ת' + #13#10 +
'fmWorkflowEditor.DiagramConnectionId2.Caption=����ת' + #13#10 +
'fmWorkflowEditor.DiagramConnectionId2.Hint=����ת' + #13#10 +
'fmWorkflowEditor.DiagramConnectionId3.Caption=����ת' + #13#10 +
'fmWorkflowEditor.DiagramConnectionId3.Hint=����ת' + #13#10 +
'fmWorkflowEditor.OpenDialog1.Filter=����ͼ�ļ� (*.dgr)|*.dgr|All files (*.*)|*.*' + #13#10 +
'fmWorkflowEditor.OpenDialog1.Title=ѡ������ͼ�ļ�' + #13#10 +
'fmWorkflowEditor.SaveDialog1.Filter=����ͼ�ļ� (*.dgr)|*.dgr|All files (*.*)|*.*' + #13#10 +
'fmWorkflowEditor.SaveDialog1.Title=ѡ������ͼ�ļ�' + #13#10 +
'fmWorkflowEditor.Sourcearrow1.Caption=Դ��ͷ' + #13#10 +
'fmWorkflowEditor.Targetarrow1.Caption=Ŀ���ͷ' + #13#10 +
'' + #13#10 +
'[fWorkflowEditor]' + #13#10 +
'fmRunWorkflowEditor.Self.Caption=ִ�й�����' + #13#10 +
'fmRunWorkflowEditor.tsGeneral.Caption=ͨ��' + #13#10 +
'fmRunWorkflowEditor.lbWorkflowDefinition.Caption=����������' + #13#10 +
'fmRunWorkflowEditor.lbVariableMapping.Caption=������Ӧ��' + #13#10 +
'fmRunWorkflowEditor.chWaitExecution.Caption=�ȴ�������' + #13#10 +
'fmRunWorkflowEditor.btCancel.Caption=ȡ��' + #13#10 +
'fmRunWorkflowEditor.btOk.Caption=ȷ��' + #13#10 +
'' + #13#10 +
'';

initialization
  _ResAddStrings(LanguageStr);

end.