unit fSponsorEditor;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  ComCtrls, StdCtrls, wsClasses, wsBlocks, fAttachPermissions, fFieldFrame,
  wsMain, wsDiagram, fStatusFrame, fExpirationFrame, fAssignedUserFrame;

type
  TfmSponsorEditor = class(TForm)
    PageControl1: TPageControl;
    tsGeneral: TTabSheet;
    lbSubject: TLabel;
    edSubject: TEdit;
    lbDescription: TLabel;
    mmComments: TMemo;
    tsStatus: TTabSheet;
    btCancel: TButton;
    btOk: TButton;
    cbMailNotification: TCheckBox;
    tsAttachments: TTabSheet;
    frPermissions: TfrAttachPermissions;
    tsFields: TTabSheet;
    frFields: TfrFieldFrame;
    tsExpiration: TTabSheet;
    frStatus: TfrStatusFrame;
    frExpiration: TfrExpirationFrame;
    lStepNo: TLabel;
    cbStepDefin: TEdit;
    cbCanPrint: TCheckBox;
    procedure btOkClick(Sender: TObject);
    procedure btCancelClick(Sender: TObject);
    procedure PropertyChanged(Sender: TObject);
    procedure FormCreate(Sender: TObject);
    procedure FormActivate(Sender: TObject);
    procedure cbStepDefinKeyPress(Sender: TObject; var Key: Char);
  private                     
    FDiagram: TCustomWorkflowDiagram;
    FTask: TTaskDefinition;
    FUpdatingControls: integer;
    FWorkflowStudio: TCustomWorkflowStudio;
    procedure TaskToInterface;
    procedure LoadTaskInEditors(ATask: TTaskDefinition);
    procedure SaveEditorsInTask(ATask: TTaskDefinition);
    procedure BeginUpdateControls;
    procedure EndUpdateControls;
    procedure SetControlsEnabled(AEnabled: boolean);
    procedure Localize;
  protected
    procedure Loaded; override;
  public
    function EditSponsorDefinition(ABlock: TWorkflowSponsorBlock; ADiagram: TCustomWorkflowDiagram): boolean;
    constructor Create(AOwner: TComponent; AWorkflowStudio: TCustomWorkflowStudio); reintroduce;
    destructor Destroy; override;
  end;

var
  fmSponsorEditor: TfmSponsorEditor;

implementation

uses
  wsRes;

{$R *.DFM}

{ TForm1 }

function TfmSponsorEditor.EditSponsorDefinition(ABlock: TWorkflowSponsorBlock; ADiagram: TCustomWorkflowDiagram): boolean;
begin
  FTask.Assign(ABlock.Task);
  FDiagram := ADiagram;
  frPermissions.OnPropertyChanged := PropertyChanged;
  frExpiration.OnPropertyChanged := PropertyChanged;
  TaskToInterface;
  result := ShowModal = mrOk;
  if result then
  begin
    frFields.SaveEditorsInTask(FTask);
    ABlock.Task.Assign(FTask);
  end;
end;

procedure TfmSponsorEditor.btOkClick(Sender: TObject);
begin
  ModalResult := mrOk;
end;

procedure TfmSponsorEditor.cbStepDefinKeyPress(Sender: TObject; var Key: Char);
begin
      if (Key<>#8) and (Key<>#13) and ((Key<#48) or (Key>#57)) then
            Key:=#0; //��������
end;

procedure TfmSponsorEditor.btCancelClick(Sender: TObject);
begin
  ModalResult := mrCancel;
end;

procedure TfmSponsorEditor.TaskToInterface;
begin
  LoadTaskInEditors(FTask);
end;

procedure TfmSponsorEditor.LoadTaskInEditors(ATask: TTaskDefinition);
var
TaskUser: TTaskAssignedUser;
begin
  if ATask <> nil then
  begin
    BeginUpdateControls;
    try
      edSubject.Text := ATask.Subject;
      mmComments.Lines.Text := ATask.Description;
      TaskUser :=ATask.Users[0];
      cbStepDefin.Text := IntToStr(TaskUser.StepDefin);
      cbCanPrint.Checked :=TaskUser.CanPrint;
      cbMailNotification.Checked := ATask.MailNotification;
      frPermissions.LoadTaskInEditors(ATask);
      frFields.LoadTaskInEditors(ATask, FDiagram);
      frStatus.LoadTaskStatusList(ATask);
      frExpiration.LoadTaskInEditors(ATask);
      SetControlsEnabled(true);
    finally
      EndUpdateControls;
    end;
  end;
end;

procedure TfmSponsorEditor.SaveEditorsInTask(ATask: TTaskDefinition);
var
  TaskUser: TTaskAssignedUser;
begin
  if ATask <> nil then
  begin
    ATask.Subject := edSubject.Text;
    ATask.Description := mmComments.Lines.Text;
    TaskUser :=ATask.Users[0];
    TaskUser.StepDefin:=StrToInt(cbStepDefin.Text);
    TaskUser.CanPrint:=cbCanPrint.Checked ;
    ATask.MailNotification := cbMailNotification.Checked;
    frPermissions.SaveEditorsInTask(ATask);
    frFields.SaveEditorsInTask(ATask);
    frExpiration.SaveEditorsInTask(ATask);
  end;
end;

procedure TfmSponsorEditor.PropertyChanged(Sender: TObject);
begin
  if (FUpdatingControls = 0) then
    SaveEditorsInTask(FTask);
end;

constructor TfmSponsorEditor.Create(AOwner: TComponent; AWorkflowStudio: TCustomWorkflowStudio);
begin
  inherited Create(AOwner);
  if AWorkflowStudio <> nil then
    FWorkflowStudio := AWorkflowStudio
  else
    raise Exception.CreateFmt('WorkflowStudio parameter not specified in %s.Create.', [ClassName]);

  FTask := TTaskDefinition.Create(nil);
  //FWorkflowStudio.UserManager.FillAssignmentList(edAssignment.Items);
end;

destructor TfmSponsorEditor.Destroy;
begin
  FTask.Free;
  inherited;
end;

procedure TfmSponsorEditor.BeginUpdateControls;
begin
  inc(FUpdatingControls);
end;

procedure TfmSponsorEditor.EndUpdateControls;
begin
  dec(FUpdatingControls);
end;

procedure TfmSponsorEditor.SetControlsEnabled(AEnabled: boolean);
begin
  edSubject.Enabled := AEnabled;
  mmComments.Enabled := AEnabled;
  cbMailNotification.Enabled := AEnabled;
end;

procedure TfmSponsorEditor.FormCreate(Sender: TObject);
begin
  PageControl1.ActivePage := tsGeneral;
  ActiveControl := edSubject;
end;

procedure TfmSponsorEditor.Localize;
begin
  Self.Caption := '������';
  tsGeneral.Caption := _str('fmApprovalEditor.tsGeneral.Caption');
  lbSubject.Caption := _str('fmApprovalEditor.lbSubject.Caption');
  lbDescription.Caption := _str('fmApprovalEditor.lbDescription.Caption');
  //lbAssignment.Caption := _str('fmApprovalEditor.lbAssignment.Caption');
  cbMailNotification.Caption := _str('fmApprovalEditor.cbMailNotification.Caption');
  tsStatus.Caption := _str('fmApprovalEditor.tsStatus.Caption');
  tsAttachments.Caption := _str('fmApprovalEditor.tsAttachments.Caption');
  tsFields.Caption := _str('fmApprovalEditor.tsFields.Caption');
  btCancel.Caption := _str('fmApprovalEditor.btCancel.Caption');
  btOk.Caption := _str('fmApprovalEditor.btOk.Caption');
  tsExpiration.Caption := _str('fmApprovalEditor.tsExpiration.Caption');
end;

procedure TfmSponsorEditor.FormActivate(Sender: TObject);
begin
  edSubject.SetFocus;
end;

procedure TfmSponsorEditor.Loaded;
begin
  inherited;
  Localize;
end;

end.
