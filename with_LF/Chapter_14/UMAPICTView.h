///////////////////////////////////////////////////////////////////
//
// UMAPICTView.h
// This is the interface of the application objects for the
// MAPictView program
//
//© 1990 Dan Weston, All Rights Reserved
// 
///////////////////////////////////////////////////////////////////

class TPICTViewApp: public TApplication {
  public:
    // Initialize the Application
    virtual pascal void IPICTViewApp(void);

    // Launches a TPICTDocument
    virtual pascal struct TDocument *DoMakeDocument(CmdNumber
itsCmdNumber);

    // disable the new menu item
    virtual pascal void DoSetupMenus(void);

    // Prevents empty document on laun~h
    virtual pascal void OpenNew(CmdNumber itsCmdNumber);
};

class TPICTDocument: public TDocument {
  public: 
    Handle fPICTData; // The PICT owned by the document
    Handle fPICTHeader; // header for PICT file


    // Initialization and freeing
    virtual pascal void IPICTDocument(void);
    virtual pascal void Free(void);

    // disable Save and SaveAs menu items 
    virtual pascal void DoSetupMenus(void);

    // read the file
    virtual pascal void DoRead(short aRefNum,Boolean rsrcExists,
Boolean forPrinting);

    // Making views and windows
    virtual pascal void DoMakeViews(Boolean forPrinting);

    // Inspecting
    virtual pascal void Fields(pascal void (*DoToField) (
                          StringPtr fieldName,
                          Ptr fieldAddr,
                          short fieldType,
                          void *DoToField_StaticLink),
                          void *DoToField_StaticLink);
};

class TPICTView:public TView {
  public:
    // drawing and sizing
    virtual pascal void CalcMinSize(VPoint *minSize);
    virtual pascal void Draw(Rect *area);
};