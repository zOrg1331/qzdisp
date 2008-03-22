RESOURCES     = disp_tut.qrc
QT           += opengl
TEMPLATE      = lib
CONFIG       += plugin
INCLUDEPATH  += ../
INCLUDEPATH  += ../includes
INCLUDEPATH  += ../includes/muParser

# Main headers
HEADERS       = disp_tut.h \
		less1.h \
		less2.h \
		less3.h \
                less4.h \
                less5.h \
                glwidget4.h \
                glwidget5.h \
		drawdisparea.h \
		parser.h
# my headers
HEADERS	     +=	../includes/translate.h \
                ../includes/xcrypt.h \
		../includes/knz_fft_cpp.h \
		../includes/zchart.h \
		../includes/zframes.h
# muParser headers
HEADERS      += ../includes/muParser/muParserBase.h \
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
# main sources
SOURCES       = disp_tut.cpp \
		less1.cpp \
		less2.cpp \
		less3.cpp \
                less4.cpp \
                less5.cpp \
                glwidget4.cpp \
                glwidget5.cpp \
		drawdisparea.cpp \
		parser.cpp
# my sources
SOURCES      +=	../includes/knz_fft_cpp.cpp \
		../includes/zchart.cpp \
		../includes/zframes.cpp
# muParsers sources
SOURCES      += ../includes/muParser/muParserBase.cpp \
		../includes/muParser/muParserBytecode.cpp \
		../includes/muParser/muParserCallback.cpp \
		../includes/muParser/muParser.cpp \
		../includes/muParser/muParserError.cpp \
		../includes/muParser/muParserInt.cpp \
		../includes/muParser/muParserTokenReader.cpp

TARGET        = disp_tut
DESTDIR       = ../s_edu/plugins/

SHARED_FOLDER = ./shared

include($$SHARED_FOLDER/shared.pri)

contains(TEMPLATE,lib) {
CONFIG(release) {
unix:TARGET = $$member(TARGET, 0)
else:TARGET = $$member(TARGET, 0)
  }
}

# install
target.path = ../s_edu/plugins/
sources.files = $$SOURCES $$HEADERS $$RESOURCES $$FORMS disp_tut.pro
sources.path = ../tutorials/disp_tut/
INSTALLS += target sources

