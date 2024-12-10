#ifndef TclInterp_H
#define TclInterp_H

class TclInterp {
  Tcl_Interp *_interp;
public:
  int DlshAppInit(Tcl_Interp *interp);
  TclInterp(int argc, char *argv[]);
  ~TclInterp(void);
  Tcl_Interp * interp(void);
  int eval(const char *command, std::string &resultstr);
  std::string eval(const char *command);
};

#endif