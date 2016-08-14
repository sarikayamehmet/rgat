#pragma once
#include "stdafx.h"
#include "GUIStructs.h"
#include <Agui/Agui.hpp>
#include <Agui/Backends/Allegro5/Allegro5.hpp>
#include "Agui\Widgets\DropDown\DropDown.hpp"
#include "Agui\Widgets\Label\Label.hpp"
#include "Agui\Widgets\RadioButton\RadioButton.hpp"
#include "Agui\Widgets\Frame\Frame.hpp"
#include "Agui\FlowLayout.hpp"
#include "Agui\Widgets\TextField\TextField.hpp"

#define DIFF_INFOLABEL_X_OFFSET 25

class AnimControls {
public:
	AnimControls(agui::Gui *widgets, VISSTATE *cState);
	void setAnimEnabled(bool enabled);
	bool isEnabled() { return enableState; }
	void update(thread_graph_data *graph);
	void notifyAnimFinished();
private:
	agui::FlowLayout *controlsLayout = NULL;
	agui::Button *connectBtn = NULL;
	agui::Button *backJumpBtn = NULL;
	agui::Button *backStepBtn = NULL;
	agui::Button *forwardStepBtn = NULL;
	agui::Button *forwardJumpBtn = NULL;
	agui::Button *playBtn = NULL;
	agui::TextField *stepText = NULL;
	agui::Font *btnFont;
	agui::Label *stepsLabel;
	bool enableState = true;
	agui::Gui *guiwidgets;
	VISSTATE *clientState;
};

class ComparisonBox {
public:
	ComparisonBox(agui::Gui *widgets, VISSTATE *clientState);
	agui::RadioButton *firstDiffLabel;
	agui::RadioButton *secondDiffLabel;
	//todo make class for this
	agui::Frame *diffFrame = NULL;
	agui::Font *diffFont;
	
	agui::Button *diffBtn;

	int getSelectedDiff();
	void setDiffGraph(thread_graph_data *graph);
	thread_graph_data *get_graph(int idx);

private:
	agui::Label *graph1Info = 0;
	agui::Label *graph1Path = 0;
	agui::Label *graph2Info = 0;
	agui::Label *graph2Path = 0;
	thread_graph_data *graph1 = 0;
	thread_graph_data *graph2 = 0;
};

class TraceVisGUI {

public:
	TraceVisGUI(VISSTATE *cstate) { clientState = cstate; }
	void widgetSetup();
	agui::Allegro5Input *inputHandler() { return widgetInputHandler; }
	agui::DropDown *dropdown() { return dropDownWidget; }
	void updateRenderWidgets(thread_graph_data *graph);
	void doLogic() { widgets->logic(); }
	void setActivePID(int PID);
	void addPID(int PID);
	void showHideDiffFrame();
	void processEvent(ALLEGRO_EVENT *ev) 
		{ widgetInputHandler->processEvent(*ev); 
			widgets->logic();
		}

	ComparisonBox *diffWindow = NULL;
	AnimControls *controlWindow = NULL;

protected:

	agui::Gui *widgets;
	VISSTATE *clientState;
	agui::Allegro5Graphics *widgetGraphicsHandler;
	agui::Label *pidDropLabel;
	agui::DropDown *dropDownWidget;
	agui::Allegro5Input *widgetInputHandler;

	
};

class DropDownListener : public agui::ActionListener
{
public:
	DropDownListener(VISSTATE *state) { clientState = state; }
	virtual void actionPerformed(const agui::ActionEvent &evt)
	{
		int PID = std::stoi(evt.getSource()->getText());
		if (PID != clientState->activePid->PID)
			clientState->newPID = PID;
	}
private:
	VISSTATE *clientState;
};

class CompareButtonListener : public agui::ActionListener
{
public:
	CompareButtonListener(VISSTATE *state) { clientState = state; }
	virtual void actionPerformed(const agui::ActionEvent &evt)
	{
		clientState->modes.diff = DIFF_SELECTED;
	}
private:
	VISSTATE *clientState;
};


class RadioButtonListener : public agui::ActionListener
{
public:
	RadioButtonListener(VISSTATE *state, agui::RadioButton *s1, agui::RadioButton *s2);
	virtual void actionPerformed(const agui::ActionEvent &evt)
	{
		if (evt.getSource() == source1)
			source2->setChecked(!source1->getRadioButtonState());
		else {
			if (evt.getSource() == source2)
				source1->setChecked(!source2->getRadioButtonState());
		}
	}
private:
	VISSTATE *clientState;
	agui::RadioButton *source1;
	agui::RadioButton *source2;
};

ALLEGRO_DISPLAY* displaySetup();
void updateTitle(ALLEGRO_DISPLAY *display, TITLE *title);
void updateTitle_Mouse(ALLEGRO_DISPLAY *display, TITLE *title, int x, int y);
void updateTitle_Zoom(ALLEGRO_DISPLAY *display, TITLE *title, float zoom);
void updateTitle_FPS(ALLEGRO_DISPLAY *display, TITLE *title, int FPS, double FPSMax);
void updateTitle_NumPrimitives(ALLEGRO_DISPLAY *display, VISSTATE *clientState, int verts, int edges);
void updateTitle_dbg(ALLEGRO_DISPLAY *display, TITLE *title, char *msg);
void display_activeGraph_summary(int x, int y, ALLEGRO_FONT *font, VISSTATE *clientState);

int controlSetup();
GUI_DATA *init_GUI_Colours();
ALLEGRO_EVENT_SOURCE * create_menu(ALLEGRO_DISPLAY *display);
void cleanup_for_exit(ALLEGRO_DISPLAY *display);