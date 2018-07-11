TEMPLATE=lib
TARGET=customcontext

CONFIG += plugin

QT += gui-private core-private quick-private qml-private

message("")

verbose:{
    message("verbose ..................: yes")
    DEFINES+=CUSTOMCONTEXT_DEBUG
} else {
    message("verbose ..................: no")
}



############################################################
#
# Rendering hooks
#

dither:{
    message("dither ...................: yes")
    DEFINES += CUSTOMCONTEXT_DITHER
    SOURCES += renderhooks/ordereddither2x2.cpp
    HEADERS += renderhooks/ordereddither2x2.h
} else {
    message("dither ...................: no")
}



############################################################
#
# Textures
#

eglgralloctexture:{
    message("eglgralloctexture ........: yes (remember: ANDROID_SDK_INCLUDE)")
    DEFINES += CUSTOMCONTEXT_EGLGRALLOCTEXTURE
    SOURCES += texture/eglgralloctexture.cpp
    HEADERS += texture/eglgralloctexture.h

    INCLUDEPATH += texture $(ANDROID_SDK_INCLUDE) $(ANDROID_SDK_INCLUDE)/android
    LIBS += -lhardware -lEGL

} else {
    message("eglgralloctexure .........: no")
}

hybristexture :{
    message("hybristexture ............: yes")
    DEFINES += CUSTOMCONTEXT_HYBRISTEXTURE
    SOURCES += texture/hybristexture.cpp
    HEADERS += texture/hybristexture.h
    INCLUDEPATH += texture
    LIBS += -lEGL

} else {
    message("hybristexture ............: no")
}

nonpreservedtexture:{
    message("nonpreservedtexture ......: yes")
    DEFINES += CUSTOMCONTEXT_NONPRESERVEDTEXTURE
    SOURCES += texture/nonpreservedtexture.cpp
    HEADERS += texture/nonpreservedtexture.h
    INCLUDEPATH += texture
} else {
    message("nonpreservedtexture ......: no")
}



############################################################
#
# Animation Drivers
#

animationdriver:{
    message("animationdriver ..........: yes")
    DEFINES += CUSTOMCONTEXT_ANIMATIONDRIVER
    SOURCES += animation/animationdriver.cpp
    HEADERS += animation/animationdriver.h
} else {
    message("animationdriver ..........: no")
}

swaplistenanimationdriver:{
    message("swaplistenanimationdriver : yes")
    DEFINES += CUSTOMCONTEXT_SWAPLISTENINGANIMATIONDRIVER
    SOURCES += animation/swaplisteninganimationdriver.cpp
    HEADERS += animation/swaplisteninganimationdriver.h
} else {
    message("swaplistenanimationdriver : no")
}



############################################################
#
# Renderers
#

simplerenderer:{
    message("simplerenderer ...........: yes")
    DEFINES += CUSTOMCONTEXT_SIMPLERENDERER
    SOURCES += renderer/simplerenderer.cpp
    HEADERS += renderer/simplerenderer.h renderer/qsgbasicshadermanager_p.h renderer/qsgbasicclipmanager_p.h
} else {
    message("simplerenderer ...........: no")
}


############################################################
#
# Other stuff
#
programbinary: {
    message("programbinary ............: yes")
    DEFINES += PROGRAM_BINARY
    SOURCES += programbinary.cpp
    LIBS += -lEGL
} else {
    message("programbinary ............: no")
}

surfaceformat:{
    message("surfaceformat ............: yes")
    DEFINES += CUSTOMCONTEXT_SURFACEFORMAT
} else {
    message("surfaceformat ............: no")
}


message("");
message("Enable the above features by adding them to the qmake config, for instance:")
message(" > qmake \"CONFIG+=verbose atlastexture dither\"");
message("");



SOURCES += \
    context.cpp \
    pluginmain.cpp

HEADERS += \
    context.h \
    pluginmain.h

OTHER_FILES += customcontext.json

target.path +=  $$[QT_INSTALL_PLUGINS]/scenegraph

files.path += $$[QT_INSTALL_PLUGINS]/scenegraph

INSTALLS += target files

!win*:DEFINES+=ENABLE_PROFILING

arm_build {
    QMAKE_CXXFLAGS += -mfp16-format=ieee -mfpu=neon
} else {
    DEFINES += DESKTOP_BUILD
}

