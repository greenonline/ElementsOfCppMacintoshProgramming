# Elements of C++ Macintosh Programming Notes

### Notes

#### General C++ notes for MPW
Use MPW 3.5 as 3.2.3 does not have

- Tools:SC
- Tools:SCpp
- Interfaces(&Libraries):Interfaces:CIncludes:iostreams.h

#### When compiling `TDoc.cp`
However, 3.5 does not have Interfaces:CIncludes:SysEqu.h whereas 3.2.3 does (as does Think C v6 and Think C v7.0 (but not Symantec C++ 8.6?) - fixed by copying over!


In MPW 3.5 for the SFReply error, use StandardFile.h

Another error:
```none
const short rMenuBarID = 128;         /* application's menu bar */
                       ^
File "AppDocMenus.h"; line 7 #Error: ';' expected
#-----------------------
```
Solution/Workaround: Just use #define instead.

Actually, the issue went away, in the end, and was (probably) due to a typo in `TDoc.h` or `TDoc.cp`


#### Illegal cast (`Str63` to `Str255`)

Error

```none
        err = FSOpen( (Str255)reply->fName, reply->vRefNum, &refnum);
                                          ^
File "TDoc.cp"; line 318 #Error: illegal cast
from: (unsigned char *)
to  : (unsigned char [256])
#-----------------------
```
Workaround, just remove the casting.

```none
  //OSErr err = FSOpen((Str255)reply->fName, reply->vRefNum, &refnum); 
  OSErr err = FSOpen(reply->fName, reply->vRefNum, &refnum); 
  switch(err) {
    case fnfErr: // file not found, create it
      //err = Create((Str255)reply->fName, reply->vRefNum, fCreator, GetDocType());
      err = Create(reply->fName, reply->vRefNum, fCreator, GetDocType());
      if(err == noErr) {
        //err = FSOpen( (Str255)reply->fName, reply->vRefNum, &refnum);
        err = FSOpen( reply->fName, reply->vRefNum, &refnum);
```

However, from `StandardFile.h` `SFReply` , the `fNAME` field is a `StrFileName`, which is a `Str63`.

From `Files.h`, the first argument to `FSOpen()` is a  `ConstStr255Param`. So, how does one cast between `Str63` and `Str255` (or `ConstStr255Param`)?

Interesting notes on `Str63` and `Str255`:
 - MacTech - [MACINTOSH DEVELOPER TECHNICAL SUPPORT](https://preserve.mactech.com/articles/develop/issue_13/104-117_Q_As.html)

- [Steve's Tech Talk : The Good Old Days Weren't That Good](https://www.atalasoft.com/cs/blogs/stevehawley/archive/2010/07/20/the-good-old-days-weren-t-that-good.aspx)

- [](https://preserve.mactech.com/articles/mactech/Vol.15/15.01/Ultra-GroovyLStringClass/index.html)



### DisposHandle() to DisposeHandle()
### GetMHandle() to GetMenuHandle()

### GetItem() to GetMenuItemText()

### Generate new make files

Use the menu option to create a new makefile as the makefiles in the book don't seem to match MPW 3.5

### Commented out `#pragma segment` lines

They gave an error (TODO: Check this???)

However, there is a `'seg!'` resource in the file `MAPictView.r`

```none
/*
Printing to the LaserWriter is the time when the most temporary memory is in use. We need the segments in use at that time
*/ 
resource 'seg ! ' {256, purgeable){
  {
    "GWriteFile";
    "GClipboard";
    "GNonRes";
    "GFile";
    "GSelCommand";
    "GTerminate";
    "GClose";
    "GDoCommand";
  }
};
```


### p427-430 missing in PDF
End of PictView.rsr.c.r and .sade, .r files?

### Note the use of include lines in `MAPictView.r` without the `#`.