#ifndef WXAS_NODEEDITORCONTROL_HPP
#define WXAS_NODEEDITORCONTROL_HPP

#include "NUIE_NodeEditor.hpp"
#include "WXAS_ControlUtilities.hpp"

#include <wx/wx.h>
#include <memory>

namespace WXAS
{

class NodeEditorControl;

class NodeEditorEventHandler : public NUIE::EventHandler
{
public:
	NodeEditorEventHandler (NodeEditorControl* control);
	virtual ~NodeEditorEventHandler ();

	virtual NUIE::MenuCommandPtr OnContextMenu (const NUIE::Point& position, const NUIE::MenuCommandStructure& commands) override;
	virtual NUIE::MenuCommandPtr OnContextMenu (const NUIE::Point& position, const NUIE::UINodePtr& uiNode, const NUIE::MenuCommandStructure& commands) override;
	virtual NUIE::MenuCommandPtr OnContextMenu (const NUIE::Point& position, const NUIE::UIOutputSlotConstPtr& outputSlot, const NUIE::MenuCommandStructure& commands) override;
	virtual NUIE::MenuCommandPtr OnContextMenu (const NUIE::Point& position, const NUIE::UIInputSlotConstPtr& inputSlot, const NUIE::MenuCommandStructure& commands) override;
	virtual NUIE::MenuCommandPtr OnContextMenu (const NUIE::Point& position, const NUIE::UINodeGroupPtr& group, const NUIE::MenuCommandStructure& commands) override;
	virtual void OnDoubleClick (const NUIE::Point& position, NUIE::MouseButton mouseButton) override;
	virtual bool OnParameterSettings (NUIE::ParameterInterfacePtr paramInterface, const NUIE::UINodePtr& uiNode);
	virtual bool OnParameterSettings (NUIE::ParameterInterfacePtr paramInterface, const NUIE::UINodeGroupPtr& uiGroup);

protected:
	NodeEditorControl* control;
};

class NodeEditorUIEnvironment : public NUIE::NodeUIEnvironment
{
public:
	NodeEditorUIEnvironment	(	NodeEditorControl* nodeEditorControl,
								NE::StringConverterPtr& stringConverter,
								NUIE::SkinParamsPtr& skinParams,
								NUIE::EventHandlerPtr& eventHandler,
								NE::EvaluationEnv& evaluationEnv);
	virtual ~NodeEditorUIEnvironment ();

	void								OnPaint (wxPanel* panel, wxPaintEvent& evt);
	void								OnResize (int width, int height);

	virtual const NE::StringConverter&	GetStringConverter () override;
	virtual const NUIE::SkinParams&		GetSkinParams () override;
	virtual NUIE::DrawingContext&		GetDrawingContext () override;
	virtual double						GetWindowScale () override;
	virtual NE::EvaluationEnv&			GetEvaluationEnv () override;
	virtual void						OnEvaluationBegin () override;
	virtual void						OnEvaluationEnd () override;
	virtual void						OnValuesRecalculated () override;
	virtual void						OnRedrawRequested () override;
	virtual NUIE::EventHandler&			GetEventHandler () override;
	virtual NUIE::ClipboardHandler&		GetClipboardHandler () override;
	virtual double						GetMouseMoveMinOffset () override;

private:
	NodeEditorControl*								nodeEditorControl;
	NE::EvaluationEnv&								evaluationEnv;

	NE::StringConverterPtr							stringConverter;
	NUIE::SkinParamsPtr								skinParams;
	NUIE::EventHandlerPtr							eventHandler;
	NUIE::ClipboardHandlerPtr						clipboardHandler;
	std::shared_ptr<NUIE::NativeDrawingContext>		drawingContext;
};

class NodeEditorControl : public wxPanel
{
public:
	enum class UpdateMode
	{
		Automatic,
		Manual
	};

	NodeEditorControl (wxWindow *parent);
	virtual ~NodeEditorControl ();

	void						Init (std::shared_ptr<NodeEditorUIEnvironment>& editorUIEnvironment);
	virtual void				OnInit ();

	void						OnPaint (wxPaintEvent& evt);
	void						OnResize (wxSizeEvent& evt);
	void						OnMouseCaptureLost (wxMouseCaptureLostEvent& evt);

	void						OnLeftButtonDown (wxMouseEvent& evt);
	void						OnLeftButtonUp (wxMouseEvent& evt);
	void						OnLeftButtonDoubleClick (wxMouseEvent& evt);

	void						OnRightButtonDown (wxMouseEvent& evt);
	void						OnRightButtonUp (wxMouseEvent& evt);
	void						OnRightButtonDoubleClick (wxMouseEvent& evt);

	void						OnMiddleButtonDown (wxMouseEvent& evt);
	void						OnMiddleButtonUp (wxMouseEvent& evt);
	void						OnMiddleButtonDoubleClick (wxMouseEvent& evt);

	void						OnMouseMove (wxMouseEvent& evt);
	void						OnMouseWheel (wxMouseEvent& evt);

	void						OnKeyDown (wxKeyEvent& evt);

	UpdateMode					GetUpdateMode () const;
	void						SetUpdateMode (UpdateMode mode) const;
	void						ManualUpdate ();

	void						AddNode (const NUIE::UINodePtr& uiNode);
	NUIE::Point					ViewToModel (const NUIE::Point& viewPoint) const;
	void						AlignToWindow ();
	void						FitToWindow ();

	const NE::NodeCollection&	GetSelectedNodes () const;
	void						SetSelectedNodes (const NE::NodeCollection& newSelectedNodes);

	void						New ();
	bool						Open (const std::wstring& fileName);
	bool						Open (NE::InputStream& inputStream);
	bool						Save (const std::wstring& fileName);
	bool						Save (NE::OutputStream& outputStream) const;
	bool						NeedToSave () const;
	void						Undo ();
	void						Redo ();

protected:
	MouseCaptureHandler							captureHandler;
	std::shared_ptr<NodeEditorUIEnvironment>	uiEnvironment;
	std::shared_ptr<NUIE::NodeEditor>			nodeEditor;

	DECLARE_EVENT_TABLE ()
};

}

#endif
