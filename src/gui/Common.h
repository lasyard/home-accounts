#ifndef _GUI_COMMON_H_
#define _GUI_COMMON_H_

#define DECLARE_TM()      static const wxChar *const TM;
#define IMPLEMENT_TM(cls) const wxChar *const cls::TM = wxS(#cls);

#endif /* _GUI_COMMON_H_ */
