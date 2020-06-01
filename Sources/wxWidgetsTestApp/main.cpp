#include "DrawingControl.hpp"
#include "NE_SingleValues.hpp"
#include "NE_Debug.hpp"
#include "BI_BuiltInNodes.hpp"
#include "WXAS_wxFileIO.hpp"
#include "WXAS_ParameterDialog.hpp"
#include "WXAS_NodeEditorControl.hpp"

#include "TestAppNodes.hpp"

#include <locale>
#include <codecvt>

#include <wx/wx.h>
#include <wx/splitter.h>

class ApplicationState
{
public:
	ApplicationState () :
		currentFileName ()
	{

	}

	void ClearCurrentFileName ()
	{
		currentFileName.clear ();
	}

	void SetCurrentFileName (const std::wstring& newCurrentFileName)
	{
		currentFileName = newCurrentFileName;
	}

	bool HasCurrentFileName () const
	{
		return !currentFileName.empty ();
	}

	const std::wstring& GetCurrentFileName () const
	{
		return currentFileName;
	}

private:
	std::wstring currentFileName;
};

class MyCreateNodeCommand : public NUIE::SingleMenuCommand
{
public:
	enum class NodeType
	{
		Boolean,
		Integer,
		Number,
		IntegerIncrement,
		NumberIncrement,
		NumberDistribution,
		ListBuilder,
		Addition,
		Subtraction,
		Multiplication,
		Division,
		Color,
		Point,
		Line,
		Circle,
		Offset,
		Viewer
	};

	MyCreateNodeCommand (WXAS::NodeEditorControl* nodeEditorControl, NodeType nodeType, const std::wstring& name, const NUIE::Point& position) :
		NUIE::SingleMenuCommand (name, false),
		nodeEditorControl (nodeEditorControl),
		nodeType (nodeType),
		position (position)
	{

	}

	virtual void Do () override
	{
		nodeEditorControl->AddNode (CreateNode (nodeEditorControl->ViewToModel (position)));
	}

	NUIE::UINodePtr CreateNode (const NUIE::Point& modelPosition)
	{
		switch (nodeType) {
			case NodeType::Boolean:
				return NUIE::UINodePtr (new BI::BooleanNode (NE::String (L"Boolean"), modelPosition, true));
			case NodeType::Integer:
				return NUIE::UINodePtr (new BI::IntegerUpDownNode (NE::String (L"Integer"), modelPosition, 0, 5));
			case NodeType::Number:
				return NUIE::UINodePtr (new BI::DoubleUpDownNode (NE::String (L"Number"), modelPosition, 0.0, 5.0));
			case NodeType::IntegerIncrement:
				return NUIE::UINodePtr (new BI::IntegerIncrementedNode (NE::String (L"Integer Increment"), modelPosition));
			case NodeType::NumberIncrement:
				return NUIE::UINodePtr (new BI::DoubleIncrementedNode (NE::String (L"Number Increment"), modelPosition));
			case NodeType::NumberDistribution:
				return NUIE::UINodePtr (new BI::DoubleDistributedNode (NE::String (L"Number Distribution"), modelPosition));
			case NodeType::ListBuilder:
				return NUIE::UINodePtr (new BI::ListBuilderNode (NE::String (L"List Builder"), modelPosition));
			case NodeType::Addition:
				return NUIE::UINodePtr (new BI::AdditionNode (NE::String (L"Addition"), modelPosition));
			case NodeType::Subtraction:
				return NUIE::UINodePtr (new BI::SubtractionNode (NE::String (L"Subtraction"), modelPosition));
			case NodeType::Multiplication:
				return NUIE::UINodePtr (new BI::MultiplicationNode (NE::String (L"Multiplication"), modelPosition));
			case NodeType::Division:
				return NUIE::UINodePtr (new BI::DivisionNode (NE::String (L"Division"), modelPosition));
			case NodeType::Color:
				return NUIE::UINodePtr (new ColorNode (NE::String (L"Color"), modelPosition));
			case NodeType::Point:
				return NUIE::UINodePtr (new PointNode (NE::String (L"Point"), modelPosition));
			case NodeType::Line:
				return NUIE::UINodePtr (new LineNode (NE::String (L"Line"), modelPosition));
			case NodeType::Circle:
				return NUIE::UINodePtr (new CircleNode (NE::String (L"Circle"), modelPosition));
			case NodeType::Offset:
				return NUIE::UINodePtr (new OffsetNode (NE::String (L"Offset"), modelPosition));
			case NodeType::Viewer:
				return NUIE::UINodePtr (new BI::MultiLineViewerNode (NE::String (L"Viewer"), modelPosition, 5));
		}
		return nullptr;
	}

private:
	WXAS::NodeEditorControl*	nodeEditorControl;
	NodeType					nodeType;
	NUIE::Point					position;
};

class MyNodeEditorEventHandler : public WXAS::NodeEditorEventHandler
{
public:
	MyNodeEditorEventHandler (WXAS::NodeEditorControl* control) :
		WXAS::NodeEditorEventHandler (control)
	{
	
	}

	virtual NUIE::MenuCommandPtr OnContextMenu (const NUIE::Point& position, const NUIE::MenuCommandStructure& commands) override
	{
		NUIE::MenuCommandStructure actualCommands = commands;
		NUIE::GroupMenuCommandPtr createCommandGroup (new NUIE::GroupMenuCommand (L"Add Node"));

		NUIE::GroupMenuCommandPtr inputCommandGroup (new NUIE::GroupMenuCommand (L"Input Nodes"));
		inputCommandGroup->AddChildCommand (NUIE::MenuCommandPtr (new MyCreateNodeCommand (control, MyCreateNodeCommand::NodeType::Boolean, L"Boolean", position)));
		inputCommandGroup->AddChildCommand (NUIE::MenuCommandPtr (new MyCreateNodeCommand (control, MyCreateNodeCommand::NodeType::Integer, L"Integer", position)));
		inputCommandGroup->AddChildCommand (NUIE::MenuCommandPtr (new MyCreateNodeCommand (control, MyCreateNodeCommand::NodeType::Number, L"Number", position)));
		inputCommandGroup->AddChildCommand (NUIE::MenuCommandPtr (new MyCreateNodeCommand (control, MyCreateNodeCommand::NodeType::IntegerIncrement, L"Integer Increment", position)));
		inputCommandGroup->AddChildCommand (NUIE::MenuCommandPtr (new MyCreateNodeCommand (control, MyCreateNodeCommand::NodeType::NumberIncrement, L"Number Increment", position)));
		inputCommandGroup->AddChildCommand (NUIE::MenuCommandPtr (new MyCreateNodeCommand (control, MyCreateNodeCommand::NodeType::NumberDistribution, L"Number Distribution", position)));
		inputCommandGroup->AddChildCommand (NUIE::MenuCommandPtr (new MyCreateNodeCommand (control, MyCreateNodeCommand::NodeType::ListBuilder, L"List Builder", position)));
		createCommandGroup->AddChildCommand (inputCommandGroup);

		NUIE::GroupMenuCommandPtr arithmeticCommandGroup (new NUIE::GroupMenuCommand (L"Arithmetic Nodes"));
		arithmeticCommandGroup->AddChildCommand (NUIE::MenuCommandPtr (new MyCreateNodeCommand (control, MyCreateNodeCommand::NodeType::Addition, L"Addition", position)));
		arithmeticCommandGroup->AddChildCommand (NUIE::MenuCommandPtr (new MyCreateNodeCommand (control, MyCreateNodeCommand::NodeType::Subtraction, L"Subtraction", position)));
		arithmeticCommandGroup->AddChildCommand (NUIE::MenuCommandPtr (new MyCreateNodeCommand (control, MyCreateNodeCommand::NodeType::Multiplication, L"Multiplication", position)));
		arithmeticCommandGroup->AddChildCommand (NUIE::MenuCommandPtr (new MyCreateNodeCommand (control, MyCreateNodeCommand::NodeType::Division, L"Division", position)));
		createCommandGroup->AddChildCommand (arithmeticCommandGroup);

		NUIE::GroupMenuCommandPtr drawingCommandGroup (new NUIE::GroupMenuCommand (L"Drawing Nodes"));
		drawingCommandGroup->AddChildCommand (NUIE::MenuCommandPtr (new MyCreateNodeCommand (control, MyCreateNodeCommand::NodeType::Color, L"Color", position)));
		drawingCommandGroup->AddChildCommand (NUIE::MenuCommandPtr (new MyCreateNodeCommand (control, MyCreateNodeCommand::NodeType::Point, L"Point", position)));
		drawingCommandGroup->AddChildCommand (NUIE::MenuCommandPtr (new MyCreateNodeCommand (control, MyCreateNodeCommand::NodeType::Line, L"Line", position)));
		drawingCommandGroup->AddChildCommand (NUIE::MenuCommandPtr (new MyCreateNodeCommand (control, MyCreateNodeCommand::NodeType::Circle, L"Circle", position)));
		createCommandGroup->AddChildCommand (drawingCommandGroup);

		NUIE::GroupMenuCommandPtr transformationCommandGroup (new NUIE::GroupMenuCommand (L"Transformation Nodes"));
		transformationCommandGroup->AddChildCommand (NUIE::MenuCommandPtr (new MyCreateNodeCommand (control, MyCreateNodeCommand::NodeType::Offset, L"Offset", position)));
		createCommandGroup->AddChildCommand (transformationCommandGroup);

		NUIE::GroupMenuCommandPtr otherCommandGroup (new NUIE::GroupMenuCommand (L"Other Nodes"));
		otherCommandGroup->AddChildCommand (NUIE::MenuCommandPtr (new MyCreateNodeCommand (control, MyCreateNodeCommand::NodeType::Viewer, L"Viewer", position)));
		createCommandGroup->AddChildCommand (otherCommandGroup);

		actualCommands.AddCommand (createCommandGroup);
		return WXAS::SelectCommandFromContextMenu (control, position, actualCommands);
	}
};

class MyNodeEditorUIEnvironment : public WXAS::NodeEditorUIEnvironment
{
public:
	MyNodeEditorUIEnvironment (	WXAS::NodeEditorControl* nodeEditorControl,
								DrawingControl* drawingControl,
								NE::StringConverterPtr& stringConverter,
								NUIE::SkinParamsPtr& skinParams,
								NUIE::EventHandlerPtr& eventHandler,
								NE::EvaluationEnv& evaluationEnv) :
		WXAS::NodeEditorUIEnvironment (nodeEditorControl, stringConverter, skinParams, eventHandler, evaluationEnv),
		drawingControl (drawingControl)
	{
	}

	virtual void OnValuesRecalculated () override
	{
		drawingControl->RedrawImage ();
	}

private:
	DrawingControl* drawingControl;
};

class MyNodeEditorControl : public WXAS::NodeEditorControl
{
public:
	 MyNodeEditorControl (wxWindow *parent) :
		 WXAS::NodeEditorControl (parent)
	 {
	 
	 }

	 virtual void OnInit () override
	 {
		static const bool isStressTest = false;
		if (isStressTest) {
			static int count = 10;
			for (int i = 0; i < count; i++) {
				for (int j = 0; j < count; j++) {
					nodeEditor->AddNode (NUIE::UINodePtr (new BI::DoubleIncrementedNode (NE::String (L"Range"), NUIE::Point (i * 150, j * 150))));
				}
			}
			nodeEditor->Update ();
		} else {
			NUIE::UINodePtr startInputNode (new BI::DoubleUpDownNode (NE::String (L"Number"), NUIE::Point (70, 70), 20, 5));
			NUIE::UINodePtr stepInputNode (new BI::DoubleUpDownNode (NE::String (L"Number"), NUIE::Point (70, 180), 20, 5));
			NUIE::UINodePtr intRangeNodeX (new BI::DoubleIncrementedNode (NE::String (L"Increment"), NUIE::Point (220, 100)));
			NUIE::UINodePtr inputNodeY (new BI::DoubleUpDownNode (NE::String (L"Number"), NUIE::Point (220, 220), 20, 5));
			std::shared_ptr<PointNode> pointNode (new PointNode (NE::String (L"Point"), NUIE::Point (400, 150)));
			NUIE::UINodePtr viewerNode (new BI::MultiLineViewerNode (NE::String (L"Viewer"), NUIE::Point (600, 150), 5));

			nodeEditor->AddNode (startInputNode);
			nodeEditor->AddNode (stepInputNode);
			nodeEditor->AddNode (intRangeNodeX);
			nodeEditor->AddNode (inputNodeY);
			nodeEditor->AddNode (pointNode);
			nodeEditor->AddNode (viewerNode);

			nodeEditor->ConnectOutputSlotToInputSlot (startInputNode->GetUIOutputSlot (NE::SlotId ("out")), intRangeNodeX->GetUIInputSlot (NE::SlotId ("start")));
			nodeEditor->ConnectOutputSlotToInputSlot (stepInputNode->GetUIOutputSlot (NE::SlotId ("out")), intRangeNodeX->GetUIInputSlot (NE::SlotId ("step")));
			nodeEditor->ConnectOutputSlotToInputSlot (intRangeNodeX->GetUIOutputSlot (NE::SlotId ("out")), pointNode->GetUIInputSlot (NE::SlotId ("x")));
			nodeEditor->ConnectOutputSlotToInputSlot (inputNodeY->GetUIOutputSlot (NE::SlotId ("out")), pointNode->GetUIInputSlot (NE::SlotId ("y")));
			nodeEditor->ConnectOutputSlotToInputSlot (pointNode->GetUIOutputSlot (NE::SlotId ("point")), viewerNode->GetUIInputSlot (NE::SlotId ("in")));
			nodeEditor->Update ();
		}
	 }
};

class MenuBar : public wxMenuBar
{
public:
	enum CommandId
	{
		File_New			= 1,
		File_Open			= 2,
		File_Save			= 3,
		File_SaveAs			= 4,
		File_Exit			= 5,
		Edit_Undo			= 6,
		Edit_Redo			= 7,
		Mode_Automatic		= 8,
		Mode_Manual			= 9,
		Mode_Update			= 10,
		View_AlignToWindow	= 11,
		View_FitToWindow	= 12
	};

	MenuBar () :
		wxMenuBar ()
	{
		wxMenu* fileMenu = new wxMenu ();
		fileMenu->Append (CommandId::File_New, "New");
		fileMenu->Append (CommandId::File_Open, "Open...");
		fileMenu->Append (CommandId::File_Save, "Save...");
		fileMenu->Append (CommandId::File_SaveAs, "Save As...");
		fileMenu->AppendSeparator ();
		fileMenu->Append (CommandId::File_Exit, L"Exit");
		Append (fileMenu, L"&File");

		wxMenu* editMenu = new wxMenu ();
		editMenu->Append (CommandId::Edit_Undo, "Undo");
		editMenu->Append (CommandId::Edit_Redo, "Redo");
		Append (editMenu, L"&Edit");

		wxMenu* modeMenu = new wxMenu ();
		modeMenu->AppendRadioItem (CommandId::Mode_Automatic, "Automatic");
		modeMenu->AppendRadioItem (CommandId::Mode_Manual, "Manual");
		modeMenu->AppendSeparator ();
		modeMenu->Append (CommandId::Mode_Update, L"Update");
		Append (modeMenu, L"&Mode");

		wxMenu* viewMenu = new wxMenu ();
		viewMenu->Append (CommandId::View_AlignToWindow, "Align To Window");
		viewMenu->Append (CommandId::View_FitToWindow, "Fit To Window");
		Append (viewMenu, L"&View");
	}

	void UpdateStatus (WXAS::NodeEditorControl::UpdateMode updateMode)
	{
		if (updateMode == WXAS::NodeEditorControl::UpdateMode::Automatic) {
			FindItem (CommandId::Mode_Automatic)->Check ();
		} else if (updateMode == WXAS::NodeEditorControl::UpdateMode::Manual) {
			FindItem (CommandId::Mode_Manual)->Check ();
		} else {
			DBGBREAK ();
		}
	}
};

class MainFrame : public wxFrame
{
public:
	MainFrame (const std::shared_ptr<ResultImage>& resultImage, NE::EvaluationEnv& evaluationEnv) :
		wxFrame (NULL, wxID_ANY, L"Node Engine Test App", wxDefaultPosition, wxSize (1000, 600)),
		menuBar (new MenuBar ()),
		mainWindow (new wxSplitterWindow (this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSP_THIN_SASH | wxSP_LIVE_UPDATE)),
		drawingControl (new DrawingControl (mainWindow, resultImage)),
		nodeEditorControl (new MyNodeEditorControl (mainWindow)),
		applicationState ()
	{
		NE::StringConverterPtr stringConverter (new NE::BasicStringConverter (NE::GetDefaultStringSettings ()));
		NUIE::SkinParamsPtr skinParams (new NUIE::BasicSkinParams (NUIE::GetDefaultSkinParams ()));
		NUIE::EventHandlerPtr eventHandler (new MyNodeEditorEventHandler (nodeEditorControl));
		std::shared_ptr<WXAS::NodeEditorUIEnvironment> uiEnvironment = std::shared_ptr<WXAS::NodeEditorUIEnvironment> (
			new MyNodeEditorUIEnvironment (
				nodeEditorControl,
				drawingControl,
				stringConverter,
				skinParams,
				eventHandler,
				evaluationEnv
			)
		);

		nodeEditorControl->Init (uiEnvironment);

		SetMenuBar (menuBar);
		UpdateMenuBar ();

		CreateStatusBar ();
		UpdateStatusBar ();

		mainWindow->SetSashGravity (0.5);
		mainWindow->SetMinimumPaneSize (20);
		mainWindow->SplitVertically (nodeEditorControl, drawingControl, 700);
	}

	~MainFrame ()
	{
	
	}

	void OnCommand (wxCommandEvent& evt)
	{
		WXAS::wxFileIO fileIO;
		MenuBar::CommandId commandId = (MenuBar::CommandId) evt.GetId ();
		switch (commandId) {
			case MenuBar::CommandId::File_New:
				{
					Reset ();
				}
				break;
			case MenuBar::CommandId::File_Open:
				{
					wxFileDialog fileDialog (this, L"Open", L"", L"", L"Node Engine Files (*.ne)|*.ne", wxFD_OPEN | wxFD_FILE_MUST_EXIST);
					if (fileDialog.ShowModal () == wxID_OK) {
						std::wstring fileName = fileDialog.GetPath ().ToStdWstring ();
						drawingControl->ClearImage ();
						// TODO: handle when open fails
						if (nodeEditorControl->Open (fileName, &fileIO)) {
							applicationState.SetCurrentFileName (fileName);
						} else {
							Reset ();
						}
					}
				}
				break;
			case MenuBar::CommandId::File_Save:
				{
					wxFileDialog fileDialog (this, L"Save", L"", L"", L"Node Engine Files (*.ne)|*.ne", wxFD_SAVE);
					if (applicationState.HasCurrentFileName ()) {
						nodeEditorControl->Save (applicationState.GetCurrentFileName (), &fileIO);
					} else if (fileDialog.ShowModal () == wxID_OK) {
						std::wstring fileName = fileDialog.GetPath ().ToStdWstring ();
						nodeEditorControl->Save (fileName, &fileIO);
						applicationState.SetCurrentFileName (fileName);
					}
				}
				break;
			case MenuBar::CommandId::File_SaveAs:
				{
					wxFileDialog fileDialog (this, L"Save As", L"", L"", L"Node Engine Files (*.ne)|*.ne", wxFD_SAVE);
					if (fileDialog.ShowModal () == wxID_OK) {
						std::wstring fileName = fileDialog.GetPath ().ToStdWstring ();
						nodeEditorControl->Save (fileName, &fileIO);
						applicationState.SetCurrentFileName (fileName);
					}
				}
				break;
			case MenuBar::CommandId::File_Exit:
				{
					Close (true);
				}
				break;
			case MenuBar::CommandId::Edit_Undo:
				{
					nodeEditorControl->Undo ();
				}
				break;
			case MenuBar::CommandId::Edit_Redo:
				{
					nodeEditorControl->Redo ();
				}
				break;
			case MenuBar::CommandId::Mode_Automatic:
				{
					nodeEditorControl->SetUpdateMode (WXAS::NodeEditorControl::UpdateMode::Automatic);
				}
				break;
			case MenuBar::CommandId::Mode_Manual:
				{
					nodeEditorControl->SetUpdateMode (WXAS::NodeEditorControl::UpdateMode::Manual);
				}
				break;
			case MenuBar::CommandId::Mode_Update:
				{
					nodeEditorControl->ManualUpdate ();
				}
				break;
			case MenuBar::CommandId::View_AlignToWindow:
				{
					nodeEditorControl->AlignToWindow ();
				}
				break;
			case MenuBar::CommandId::View_FitToWindow:
				{
					nodeEditorControl->FitToWindow ();
				}
				break;
		}
		UpdateMenuBar ();
		UpdateStatusBar ();
	}

private:
	void UpdateMenuBar ()
	{
		menuBar->UpdateStatus (nodeEditorControl->GetUpdateMode ());
	}

	void UpdateStatusBar ()
	{
		std::wstring currentFileText = L"No File";
		if (applicationState.HasCurrentFileName ()) {
			currentFileText = applicationState.GetCurrentFileName ();
		}
		SetStatusText (currentFileText);
	}

	void Reset ()
	{
		nodeEditorControl->New ();
		drawingControl->ClearImage ();
		applicationState.ClearCurrentFileName ();
	}

	MenuBar*					menuBar;
	wxSplitterWindow*			mainWindow;
	DrawingControl*				drawingControl;
	WXAS::NodeEditorControl*	nodeEditorControl;
	ApplicationState			applicationState;

	DECLARE_EVENT_TABLE ()
};

BEGIN_EVENT_TABLE (MainFrame, wxFrame)
EVT_MENU (wxID_ANY, MainFrame::OnCommand)
END_EVENT_TABLE ()

class NodeEngineTestApplication : public wxApp
{
public:
	NodeEngineTestApplication () :
		resultImage (new ResultImage ()),
		evaluationData (new ResultImageEvaluationData (resultImage)),
		evaluationEnv (evaluationData),
		mainFrame (nullptr)
	{
	
	}

	virtual bool OnInit ()
	{
		mainFrame = new MainFrame (resultImage, evaluationEnv);
		mainFrame->Show (true);
		SetTopWindow (mainFrame);
		return true;
	}

private:
	std::shared_ptr<ResultImage>				resultImage;
	std::shared_ptr<ResultImageEvaluationData>	evaluationData;
	NE::EvaluationEnv							evaluationEnv;

	MainFrame*									mainFrame;
};

IMPLEMENT_APP (NodeEngineTestApplication)
