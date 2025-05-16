INCLUDEPATH += /home/junlin/opensource/linux-6.14.6/include  /usr/include/

DEFINES +=MODULE
QMAKE_CFLAGS += -std=c99 # 正确的方式指定C标准


SOURCES += \
    i2c.c
