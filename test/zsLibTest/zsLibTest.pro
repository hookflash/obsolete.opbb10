APP_NAME = zsLibTest

CONFIG += qt warn_on cascades10

LIBS += -L../../../../op/hookflash-core/projects/bb/bbzsLib/Device-Debug -lbbzslib
LIBS += -L../../../../op/hookflash-libs/bbBoost/bin.v2/libs/date_time/build/qcc/debug/architecture-arm/link-static/target-os-qnxnto/threading-multi -lboost_date_time
LIBS += -L../../../../op/hookflash-libs/bbBoost/bin.v2/libs/thread/build/qcc/debug/architecture-arm/link-static/target-os-qnxnto/threading-multi -lboost_thread
LIBS += -L../../../../op/hookflash-libs/bbBoost/bin.v2/libs/system/build/qcc/debug/architecture-arm/link-static/target-os-qnxnto/threading-multi -lboost_system
LIBS += -lcamapi -laudio_manager -lasound -lcurl -lsocket -lbps -lm -lc -lcpp -lstdc++ -ljpeg -lGLESv2 -lslog2

include(config.pri)
