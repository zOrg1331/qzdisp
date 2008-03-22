TEMPLATE = app
TARGET += 
QT           += opengl
DEPENDPATH += .
INCLUDEPATH += .
INCLUDEPATH += ../includes/
INCLUDEPATH += ../includes/muParser/

# Input
HEADERS += disp_setup.h \
           glwidget4.h \
	   mainwindow.h \
	   parser.h \
	   result.h \
	   wave_setup.h \
	   drawdisparea.h
# my headers
HEADERS += ../includes/knz_fft_cpp.h \
           ../includes/translate.h \
	   ../includes/zchart.h \
   	   ../includes/zframes.h
# muParser headers
HEADERS += ../includes/muParser/muParserBase.h \
	   ../includes/muParser/muParserBytecode.h \
   	   ../includes/muParser/muParserCallback.h \
   	   ../includes/muParser/muParserDef.h \
   	   ../includes/muParser/muParserError.h \
  	   ../includes/muParser/muParserFixes.h \
   	   ../includes/muParser/muParser.h \
  	   ../includes/muParser/muParserInt.h \
   	   ../includes/muParser/muParserStack.h \
   	   ../includes/muParser/muParserToken.h \
  	   ../includes/muParser/muParserTokenReader.h
SOURCES += disp_setup.cpp \
           glwidget4.cpp \
           main.cpp \
           mainwindow.cpp \
           parser.cpp \
           result.cpp \
           wave_setup.cpp \
	   drawdisparea.cpp
# my sources
SOURCES += ../includes/knz_fft_cpp.cpp \
	   ../includes/zchart.cpp \
	   ../includes/zframes.cpp
# muParsers sources
SOURCES += ../includes/muParser/muParserBase.cpp \
 	   ../includes/muParser/muParserBytecode.cpp \
  	   ../includes/muParser/muParserCallback.cpp \
   	   ../includes/muParser/muParser.cpp \
   	   ../includes/muParser/muParserError.cpp \
   	   ../includes/muParser/muParserInt.cpp \
   	   ../includes/muParser/muParserTokenReader.cpp
