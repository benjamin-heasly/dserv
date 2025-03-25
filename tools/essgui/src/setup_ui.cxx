// generated by Fast Light User Interface Designer (fluid) version 1.0402

#include "setup_ui.h"

Fl_Double_Window *main_window=(Fl_Double_Window *)0;

Fl_Output *sysname_widget=(Fl_Output *)0;

Fl_Output *sysos_widget=(Fl_Output *)0;

Fl_Tree *host_widget=(Fl_Tree *)0;

Fl_Choice *subject_widget=(Fl_Choice *)0;

static void cb_subject_widget(Fl_Choice*, void*) {
  set_subject();
}

static void cb_Go(Fl_Button*, void*) {
  esscmd("ess::start");
}

static void cb_Stop(Fl_Button*, void*) {
  esscmd("ess::stop");
}

static void cb_Reset(Fl_Button*, void*) {
  esscmd("ess::reset");
}

Fl_Output *system_status_widget=(Fl_Output *)0;

Fl_Output *obscount_widget=(Fl_Output *)0;

Fl_Box *obs_widget=(Fl_Box *)0;

Fl_Choice *system_widget=(Fl_Choice *)0;

static void cb_system_widget(Fl_Choice*, void*) {
  set_system();
}

Fl_Choice *protocol_widget=(Fl_Choice *)0;

static void cb_protocol_widget(Fl_Choice*, void*) {
  set_protocol();
}

Fl_Choice *variant_widget=(Fl_Choice *)0;

static void cb_variant_widget(Fl_Choice*, void*) {
  set_variant();
}

Fl_Scroll *options_widget=(Fl_Scroll *)0;

Fl_Scroll *settings_widget=(Fl_Scroll *)0;

EyeTouchWin *eyetouch_widget=(EyeTouchWin *)0;

Fl_Flex *sorters_widget=(Fl_Flex *)0;

Fl_Choice *sortby_1=(Fl_Choice *)0;

Fl_Choice *sortby_2=(Fl_Choice *)0;

PerfTable *perftable_widget=(PerfTable *)0;

PerfTable *general_perf_widget=(PerfTable *)0;

Fl_Group *plot_widget=(Fl_Group *)0;

CGWin *cgwin_widget=(CGWin *)0;

VirtualEye *virtual_eye_widget=(VirtualEye *)0;

VirtualJoystick *virtual_joystick_widget=(VirtualJoystick *)0;

Fl_Scroll *rmt_commands_widget=(Fl_Scroll *)0;

DGTable *stimdg_widget=(DGTable *)0;

Fl_OpDesk *opdesk_widget=(Fl_OpDesk *)0;

Fl_Tabs *editor_tabs=(Fl_Tabs *)0;

Fl_Text_Editor *system_editor=(Fl_Text_Editor *)0;

Fl_Text_Editor *protocol_editor=(Fl_Text_Editor *)0;

Fl_Text_Editor *variant_editor=(Fl_Text_Editor *)0;

Fl_Text_Editor *stim_editor=(Fl_Text_Editor *)0;

Fl_Input *FileEntry=(Fl_Input *)0;

Fl_Button *FileOpen=(Fl_Button *)0;

Fl_Button *FileClose=(Fl_Button *)0;

Fl_Button *FileSuggest=(Fl_Button *)0;

Fl_Console *output_term=(Fl_Console *)0;

Fl_Output *status_widget=(Fl_Output *)0;

Fl_Double_Window * setup_ui(int argc, char *argv[]) {
  { main_window = new Fl_Double_Window(1280, 800, "EssGUI");
    { Fl_Group* o = new Fl_Group(0, 1, 1280, 762);
      { Fl_Group* o = new Fl_Group(0, 28, 180, 640, "Systems");
        o->labelsize(18);
        o->vertical_label_margin(4);
        { Fl_Flex* o = new Fl_Flex(16, 117, 150, 32);
          o->type(1);
          { Fl_Button* o = new Fl_Button(16, 117, 150, 32, "Refresh");
            o->labelsize(16);
            o->callback((Fl_Callback*)refresh_cb);
          } // Fl_Button* o
          o->end();
        } // Fl_Flex* o
        { Fl_Group* o = new Fl_Group(2, 28, 178, 80);
          { Fl_Flex* o = new Fl_Flex(70, 46, 110, 56);
            { sysname_widget = new Fl_Output(70, 46, 110, 30, "Name:");
              sysname_widget->box(FL_NO_BOX);
              sysname_widget->labelsize(16);
              sysname_widget->textsize(16);
            } // Fl_Output* sysname_widget
            { sysos_widget = new Fl_Output(70, 76, 110, 26, "OS:");
              sysos_widget->box(FL_NO_BOX);
              sysos_widget->labelsize(16);
              sysos_widget->textsize(16);
            } // Fl_Output* sysos_widget
            o->fixed(o->child(0), 30);
            o->end();
          } // Fl_Flex* o
          o->end();
        } // Fl_Group* o
        { host_widget = new Fl_Tree(4, 152, 173, 212);
          host_widget->callback((Fl_Callback*)host_cb);
        } // Fl_Tree* host_widget
        o->end();
      } // Fl_Group* o
      { Fl_Group* o = new Fl_Group(180, 28, 315, 644, "Control");
        o->box(FL_GTK_DOWN_BOX);
        o->color(FL_DARK1);
        o->labelsize(18);
        o->vertical_label_margin(4);
        { Fl_Group* o = new Fl_Group(180, 29, 315, 283);
          { Fl_Group* o = new Fl_Group(186, 34, 306, 36);
            o->box(FL_THIN_UP_BOX);
            { Fl_Flex* o = new Fl_Flex(261, 38, 210, 28);
              { subject_widget = new Fl_Choice(261, 38, 210, 28, "Subject:");
                subject_widget->down_box(FL_BORDER_BOX);
                subject_widget->labelsize(16);
                subject_widget->textsize(16);
                subject_widget->callback((Fl_Callback*)cb_subject_widget);
                subject_widget->when(FL_WHEN_CHANGED | FL_WHEN_NOT_CHANGED);
              } // Fl_Choice* subject_widget
              o->fixed(o->child(0), 28);
              o->end();
            } // Fl_Flex* o
            o->end();
          } // Fl_Group* o
          { Fl_Group* o = new Fl_Group(186, 72, 306, 106);
            o->box(FL_THIN_UP_BOX);
            { Fl_Flex* o = new Fl_Flex(225, 79, 240, 40);
              o->type(1);
              { Fl_Button* o = new Fl_Button(225, 79, 80, 40, "Go");
                o->labelsize(18);
                o->callback((Fl_Callback*)cb_Go);
              } // Fl_Button* o
              { Fl_Button* o = new Fl_Button(305, 79, 80, 40, "Stop");
                o->labelsize(18);
                o->callback((Fl_Callback*)cb_Stop);
              } // Fl_Button* o
              { Fl_Button* o = new Fl_Button(385, 79, 80, 40, "Reset");
                o->labelsize(18);
                o->callback((Fl_Callback*)cb_Reset);
              } // Fl_Button* o
              o->end();
            } // Fl_Flex* o
            { Fl_Flex* o = new Fl_Flex(307, 125, 95, 48);
              { system_status_widget = new Fl_Output(307, 125, 95, 24, "Status:");
                system_status_widget->box(FL_NO_BOX);
                system_status_widget->labelsize(16);
                system_status_widget->textsize(16);
              } // Fl_Output* system_status_widget
              { obscount_widget = new Fl_Output(307, 149, 95, 24, "Obs:");
                obscount_widget->box(FL_FLAT_BOX);
                obscount_widget->color(FL_BACKGROUND_COLOR);
                obscount_widget->labelsize(16);
                obscount_widget->textsize(16);
              } // Fl_Output* obscount_widget
              o->end();
            } // Fl_Flex* o
            { obs_widget = new Fl_Box(231, 128, 16, 16, "obs_indicator");
              obs_widget->box(FL_FLAT_BOX);
              obs_widget->selection_color(FL_RED);
              obs_widget->labeltype(FL_NO_LABEL);
            } // Fl_Box* obs_widget
            o->end();
          } // Fl_Group* o
          { Fl_Group* o = new Fl_Group(186, 181, 306, 128);
            o->box(FL_THIN_UP_BOX);
            o->labelsize(20);
            { Fl_Flex* o = new Fl_Flex(258, 193, 210, 102);
              { system_widget = new Fl_Choice(258, 193, 210, 28, "System:");
                system_widget->down_box(FL_BORDER_BOX);
                system_widget->labelsize(16);
                system_widget->textsize(16);
                system_widget->callback((Fl_Callback*)cb_system_widget);
                system_widget->when(FL_WHEN_CHANGED | FL_WHEN_NOT_CHANGED);
              } // Fl_Choice* system_widget
              { protocol_widget = new Fl_Choice(258, 231, 210, 27, "Protocol:");
                protocol_widget->down_box(FL_BORDER_BOX);
                protocol_widget->labelsize(16);
                protocol_widget->textsize(16);
                protocol_widget->callback((Fl_Callback*)cb_protocol_widget);
                protocol_widget->when(FL_WHEN_CHANGED | FL_WHEN_NOT_CHANGED);
              } // Fl_Choice* protocol_widget
              { variant_widget = new Fl_Choice(258, 268, 210, 27, "Variant:");
                variant_widget->down_box(FL_BORDER_BOX);
                variant_widget->labelsize(16);
                variant_widget->textsize(16);
                variant_widget->callback((Fl_Callback*)cb_variant_widget);
                variant_widget->when(FL_WHEN_CHANGED | FL_WHEN_NOT_CHANGED);
              } // Fl_Choice* variant_widget
              o->gap(10);
              o->end();
            } // Fl_Flex* o
            o->end();
          } // Fl_Group* o
          o->end();
        } // Fl_Group* o
        { Fl_Group* o = new Fl_Group(186, 312, 306, 360);
          { options_widget = new Fl_Scroll(186, 335, 306, 160, "Variant Options");
            options_widget->type(7);
            options_widget->vertical_label_margin(3);
            options_widget->align(Fl_Align(FL_ALIGN_TOP_LEFT));
            options_widget->end();
          } // Fl_Scroll* options_widget
          { settings_widget = new Fl_Scroll(186, 516, 306, 150, "System Settings");
            settings_widget->type(7);
            settings_widget->vertical_label_margin(3);
            settings_widget->align(Fl_Align(FL_ALIGN_TOP_LEFT));
            settings_widget->end();
          } // Fl_Scroll* settings_widget
          o->end();
          Fl_Group::current()->resizable(o);
        } // Fl_Group* o
        o->end();
      } // Fl_Group* o
      { Fl_Group* o = new Fl_Group(499, 28, 766, 646, "Info");
        o->labelsize(18);
        o->vertical_label_margin(4);
        { Fl_Tabs* o = new Fl_Tabs(503, 32, 762, 642);
          { Fl_Group* o = new Fl_Group(504, 56, 747, 615, "behavior");
            o->labelsize(16);
            { Fl_Group* o = new Fl_Group(514, 84, 241, 264, "Behavior Monitor");
              o->box(FL_DOWN_BOX);
              { eyetouch_widget = new EyeTouchWin(521, 91, 232, 232, "EyeWindow");
                eyetouch_widget->box(FL_DOWN_FRAME);
                eyetouch_widget->color(FL_BACKGROUND_COLOR);
                eyetouch_widget->selection_color(FL_BACKGROUND_COLOR);
                eyetouch_widget->labeltype(FL_NORMAL_LABEL);
                eyetouch_widget->labelfont(0);
                eyetouch_widget->labelsize(14);
                eyetouch_widget->labelcolor(FL_FOREGROUND_COLOR);
                eyetouch_widget->align(Fl_Align(FL_ALIGN_CENTER));
                eyetouch_widget->when(FL_WHEN_RELEASE);
              } // EyeTouchWin* eyetouch_widget
              { Fl_Group* o = new Fl_Group(514, 325, 241, 20, "window status");
                o->box(FL_THIN_DOWN_FRAME);
                o->labeltype(FL_NO_LABEL);
                o->end();
              } // Fl_Group* o
              o->end();
            } // Fl_Group* o
            { Fl_Group* o = new Fl_Group(759, 420, 492, 248, "Performance");
              o->box(FL_UP_BOX);
              o->vertical_label_margin(3);
              { sorters_widget = new Fl_Flex(1070, 567, 166, 60);
                sorters_widget->hide();
                { sortby_1 = new Fl_Choice(1070, 567, 166, 25, "Sort by:");
                  sortby_1->down_box(FL_BORDER_BOX);
                  sortby_1->callback((Fl_Callback*)sortby_cb);
                } // Fl_Choice* sortby_1
                { sortby_2 = new Fl_Choice(1070, 602, 166, 25, "Sort by:");
                  sortby_2->down_box(FL_BORDER_BOX);
                  sortby_2->callback((Fl_Callback*)sortby_cb);
                } // Fl_Choice* sortby_2
                sorters_widget->gap(10);
                sorters_widget->end();
              } // Fl_Flex* sorters_widget
              { perftable_widget = new PerfTable(779, 439, 456, 120);
                perftable_widget->box(FL_NO_BOX);
                perftable_widget->color(FL_BACKGROUND_COLOR);
                perftable_widget->selection_color(FL_BACKGROUND_COLOR);
                perftable_widget->labeltype(FL_NORMAL_LABEL);
                perftable_widget->labelfont(0);
                perftable_widget->labelsize(14);
                perftable_widget->labelcolor(FL_FOREGROUND_COLOR);
                perftable_widget->align(Fl_Align(FL_ALIGN_TOP));
                perftable_widget->when(FL_WHEN_RELEASE);
                perftable_widget->end();
              } // PerfTable* perftable_widget
              { general_perf_widget = new PerfTable(779, 574, 190, 80);
                general_perf_widget->box(FL_NO_BOX);
                general_perf_widget->color(FL_BACKGROUND_COLOR);
                general_perf_widget->selection_color(FL_BACKGROUND_COLOR);
                general_perf_widget->labeltype(FL_NORMAL_LABEL);
                general_perf_widget->labelfont(0);
                general_perf_widget->labelsize(14);
                general_perf_widget->labelcolor(FL_FOREGROUND_COLOR);
                general_perf_widget->align(Fl_Align(FL_ALIGN_TOP));
                general_perf_widget->when(FL_WHEN_RELEASE);
                general_perf_widget->end();
              } // PerfTable* general_perf_widget
              o->end();
            } // Fl_Group* o
            { plot_widget = new Fl_Group(763, 84, 488, 304, "Plot Window");
              plot_widget->labeltype(FL_NO_LABEL);
              { cgwin_widget = new CGWin(763, 84, 488, 300, "cgwin");
                cgwin_widget->box(FL_NO_BOX);
                cgwin_widget->color(FL_BACKGROUND_COLOR);
                cgwin_widget->selection_color(FL_BACKGROUND_COLOR);
                cgwin_widget->labeltype(FL_NORMAL_LABEL);
                cgwin_widget->labelfont(0);
                cgwin_widget->labelsize(14);
                cgwin_widget->labelcolor(FL_FOREGROUND_COLOR);
                cgwin_widget->align(Fl_Align(FL_ALIGN_CENTER));
                cgwin_widget->when(FL_WHEN_RELEASE);
              } // CGWin* cgwin_widget
              plot_widget->end();
            } // Fl_Group* plot_widget
            { Fl_Group* o = new Fl_Group(514, 372, 245, 296, "Virtual Input");
              o->color(FL_BLACK);
              o->vertical_label_margin(3);
              { Fl_Group* o = new Fl_Group(531, 372, 208, 204, "Eye Window");
                o->box(FL_FLAT_BOX);
                o->color(FL_INACTIVE_COLOR);
                o->labeltype(FL_NO_LABEL);
                { virtual_eye_widget = new VirtualEye(619, 462, 15, 15, "label");
                  virtual_eye_widget->box(FL_NO_BOX);
                  virtual_eye_widget->color(FL_BACKGROUND_COLOR);
                  virtual_eye_widget->selection_color(FL_BACKGROUND_COLOR);
                  virtual_eye_widget->labeltype(FL_NORMAL_LABEL);
                  virtual_eye_widget->labelfont(0);
                  virtual_eye_widget->labelsize(14);
                  virtual_eye_widget->labelcolor(FL_FOREGROUND_COLOR);
                  virtual_eye_widget->callback((Fl_Callback*)virtual_eye_cb);
                  virtual_eye_widget->align(Fl_Align(FL_ALIGN_CENTER));
                  virtual_eye_widget->when(FL_WHEN_CHANGED | FL_WHEN_RELEASE_ALWAYS);
                } // VirtualEye* virtual_eye_widget
                o->end();
              } // Fl_Group* o
              { Fl_Group* o = new Fl_Group(531, 584, 80, 76, "Joystick");
                o->box(FL_FLAT_BOX);
                o->color(FL_DARK3);
                o->labeltype(FL_NO_LABEL);
                { virtual_joystick_widget = new VirtualJoystick(565, 614, 12, 12, "label");
                  virtual_joystick_widget->box(FL_NO_BOX);
                  virtual_joystick_widget->color(FL_BACKGROUND_COLOR);
                  virtual_joystick_widget->selection_color(FL_BACKGROUND_COLOR);
                  virtual_joystick_widget->labeltype(FL_NORMAL_LABEL);
                  virtual_joystick_widget->labelfont(0);
                  virtual_joystick_widget->labelsize(14);
                  virtual_joystick_widget->labelcolor(FL_FOREGROUND_COLOR);
                  virtual_joystick_widget->callback((Fl_Callback*)virtual_joystick_cb);
                  virtual_joystick_widget->align(Fl_Align(FL_ALIGN_CENTER));
                  virtual_joystick_widget->when(FL_WHEN_CHANGED | FL_WHEN_RELEASE_ALWAYS);
                } // VirtualJoystick* virtual_joystick_widget
                o->end();
              } // Fl_Group* o
              o->end();
            } // Fl_Group* o
            o->end();
          } // Fl_Group* o
          { Fl_Group* o = new Fl_Group(503, 57, 762, 611, "stim");
            o->labelsize(16);
            o->hide();
            { rmt_commands_widget = new Fl_Scroll(507, 85, 252, 360, "Stim Commands");
              rmt_commands_widget->box(FL_THIN_DOWN_BOX);
              rmt_commands_widget->end();
            } // Fl_Scroll* rmt_commands_widget
            { stimdg_widget = new DGTable(763, 85, 502, 583, "stimdg");
              stimdg_widget->box(FL_THIN_DOWN_FRAME);
              stimdg_widget->color(FL_BACKGROUND_COLOR);
              stimdg_widget->selection_color(FL_BACKGROUND_COLOR);
              stimdg_widget->labeltype(FL_NORMAL_LABEL);
              stimdg_widget->labelfont(0);
              stimdg_widget->labelsize(14);
              stimdg_widget->labelcolor(FL_FOREGROUND_COLOR);
              stimdg_widget->align(Fl_Align(FL_ALIGN_TOP));
              stimdg_widget->when(FL_WHEN_RELEASE);
              stimdg_widget->end();
            } // DGTable* stimdg_widget
            o->end();
          } // Fl_Group* o
          { Fl_Group* o = new Fl_Group(503, 57, 762, 616, "system");
            o->labelsize(16);
            o->hide();
            { opdesk_widget = new Fl_OpDesk(513, 86, 466, 580, "State System");
              opdesk_widget->box(FL_THIN_DOWN_BOX);
              opdesk_widget->color(FL_BACKGROUND_COLOR);
              opdesk_widget->selection_color(FL_BACKGROUND_COLOR);
              opdesk_widget->labeltype(FL_NORMAL_LABEL);
              opdesk_widget->labelfont(0);
              opdesk_widget->labelsize(14);
              opdesk_widget->labelcolor(FL_FOREGROUND_COLOR);
              opdesk_widget->align(Fl_Align(FL_ALIGN_TOP));
              opdesk_widget->when(FL_WHEN_RELEASE);
              opdesk_widget->end();
            } // Fl_OpDesk* opdesk_widget
            o->end();
          } // Fl_Group* o
          { Fl_Group* o = new Fl_Group(503, 57, 762, 617, "scripts");
            o->labelsize(16);
            o->hide();
            { editor_tabs = new Fl_Tabs(503, 79, 758, 595);
              { system_editor = new Fl_Text_Editor(503, 114, 747, 560, "system");
                system_editor->selection_color((Fl_Color)31);
              } // Fl_Text_Editor* system_editor
              { protocol_editor = new Fl_Text_Editor(503, 114, 747, 560, "protocol");
                protocol_editor->selection_color((Fl_Color)31);
                protocol_editor->hide();
              } // Fl_Text_Editor* protocol_editor
              { variant_editor = new Fl_Text_Editor(503, 114, 747, 550, "variant");
                variant_editor->selection_color((Fl_Color)31);
                variant_editor->hide();
              } // Fl_Text_Editor* variant_editor
              { stim_editor = new Fl_Text_Editor(503, 114, 747, 550, "stim");
                stim_editor->selection_color((Fl_Color)31);
                stim_editor->hide();
              } // Fl_Text_Editor* stim_editor
              editor_tabs->end();
            } // Fl_Tabs* editor_tabs
            o->end();
          } // Fl_Group* o
          { Fl_Group* o = new Fl_Group(503, 57, 762, 617, "data");
            o->labelsize(16);
            o->hide();
            { Fl_Group* o = new Fl_Group(513, 78, 387, 116, "Datafile");
              { Fl_Group* o = new Fl_Group(524, 88, 376, 92);
                o->box(FL_UP_FRAME);
                { FileEntry = new Fl_Input(606, 98, 278, 24, "Filename:");
                } // Fl_Input* FileEntry
                { Fl_Pack* o = new Fl_Pack(606, 136, 226, 32);
                  o->type(1);
                  { FileOpen = new Fl_Button(613, 136, 68, 32, "Open");
                    FileOpen->callback((Fl_Callback*)file_open_cb);
                  } // Fl_Button* FileOpen
                  { FileClose = new Fl_Button(685, 136, 68, 32, "Close");
                    FileClose->callback((Fl_Callback*)file_close_cb);
                  } // Fl_Button* FileClose
                  { FileSuggest = new Fl_Button(757, 136, 68, 32, "Suggest");
                    FileSuggest->callback((Fl_Callback*)file_suggest_cb);
                  } // Fl_Button* FileSuggest
                  o->end();
                } // Fl_Pack* o
                o->end();
              } // Fl_Group* o
              o->end();
            } // Fl_Group* o
            o->end();
          } // Fl_Group* o
          o->end();
        } // Fl_Tabs* o
        o->end();
        Fl_Group::current()->resizable(o);
      } // Fl_Group* o
      { Fl_Group* o = new Fl_Group(0, 674, 1279, 88);
        { output_term = new Fl_Console(0, 674, 1279, 88);
          output_term->box(FL_DOWN_FRAME);
          output_term->color(FL_BLACK);
          output_term->selection_color(FL_BACKGROUND_COLOR);
          output_term->labeltype(FL_NORMAL_LABEL);
          output_term->labelfont(0);
          output_term->labelsize(14);
          output_term->labelcolor(FL_FOREGROUND_COLOR);
          output_term->align(Fl_Align(FL_ALIGN_TOP));
          output_term->when(FL_WHEN_RELEASE);
        } // Fl_Console* output_term
        o->end();
      } // Fl_Group* o
      o->end();
      Fl_Group::current()->resizable(o);
    } // Fl_Group* o
    { Fl_Group* o = new Fl_Group(0, 768, 1280, 31);
      o->labeltype(FL_NO_LABEL);
      { status_widget = new Fl_Output(0, 769, 1280, 30);
        status_widget->labeltype(FL_NO_LABEL);
        Fl_Group::current()->resizable(status_widget);
      } // Fl_Output* status_widget
      o->end();
    } // Fl_Group* o
    main_window->end();
  } // Fl_Double_Window* main_window
  output_term->init_linenoise();
  output_term->set_callback(eval, NULL);

  return main_window;
}
