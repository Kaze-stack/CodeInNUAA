TEMPLATE = app
CONFIG += console c++11
CONFIG += sdk_no_version_check
CONFIG += precompile_header

SOURCES += \
    KA_Engine.cpp \
    KA_Scene.cpp \
    KA_Shader.cpp \
    KA_Text.cpp \
    KA_Texture.cpp \
    KA_Timer.cpp \
    KA_Widget.cpp \
    KA_Widgets/KA_BGI.cpp \
    KA_Widgets/KA_Button.cpp \
    KA_Widgets/KA_Character.cpp \
    KA_Widgets/KA_DialogBox.cpp \
    KA_Window.cpp \
    main.cpp \
    pugixml/pugixml.cpp

HEADERS += \
    KA_Engine.h \
    KA_Scene.h \
    KA_Shader.h \
    KA_Text.h \
    KA_Texture.h \
    KA_Timer.h \
    KA_Widget.h \
    KA_Widgets/KA_BGI.h \
    KA_Widgets/KA_Button.h \
    KA_Widgets/KA_Character.h \
    KA_Widgets/KA_DialogBox.h \
    KA_Widgets_All.h \
    KA_Window.h \
    pugixml/pugiconfig.hpp \
    pugixml/pugixml.hpp


#预编译头文件
PRECOMPILED_HEADER = $$PWD/pugixml/pugiconfig.hpp
PRECOMPILED_HEADER = $$PWD/pugixml/pugixml.hpp


# framewoks引用路径
INCLUDEPATH += "$$PWD/SDL2.framework/Headers"
LIBS += "-F$$PWD"
LIBS += -framework SDL2

INCLUDEPATH += "$$PWD/SDL2_image.framework/Headers"
LIBS += "-F$$PWD"
LIBS += -framework SDL2_image

INCLUDEPATH += "$$PWD/SDL2_mixer.framework/Headers"
LIBS += "-F$$PWD"
LIBS += -framework SDL2_mixer

INCLUDEPATH += "$$PWD/SDL2_ttf.framework/Headers"
LIBS += "-F$$PWD"
LIBS += -framework SDL2_ttf

INCLUDEPATH += "$$PWD/GLEW.framework/Headers"
LIBS += "-F$$PWD"
LIBS += -framework GLEW

INCLUDEPATH += "$$PWD/OpenGL.framework/Headers"
LIBS += "-F$$PWD"
LIBS += -framework OpenGL

# frameworks依赖路径
QMAKE_RPATHDIR = ../Frameworks
