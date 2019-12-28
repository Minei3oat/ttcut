#/*-----------------------------------------------------------------------------*/
#/* COPYRIGHT: Minei3oat (c) 2019  (github.com/Minei3oat)                       */
#/*-----------------------------------------------------------------------------*/
#/*  FILE:   QMake Projektfile for ttencodeproviderffmpeg                       */
#/*-----------------------------------------------------------------------------*/
#/*  ABOUT:                                                                     */
#/*-----------------------------------------------------------------------------*/
#/* AUTHOR  : Minei3oat                                       DATE: 12/25/2019  */
#/*-----------------------------------------------------------------------------*/
#
#
TEMPLATE    = lib
PROJECT     = TTEncodeProviderFFmpeg
CONFIG      += qt warn_on plugin
QT          += core widgets
DIST        = ttencodeproviderffmpeg
TARGET      = ttencodeproviderffmpeg

unix {
  DEFINES += UNIX
  DEFINES -= MACX
}

macx {
  DEFINES += MACX
  DEFINES -= UNIX
}

OBJECTS_DIR = obj
MOC_DIR     = moc
UI_DIR      = ui_h
RCC_DIR     = res
#TRANSLATIONS =
#RESOURCES   =
#FORMS       =
HEADERS     += ttencodeproviderffmpeg.h\
               ../../../extern/iencodeprovider.h
SOURCES     += ttencodeproviderffmpeg.cpp
#
# -------------------------------------------------------------------------------
# TTENCODEPROVIDERFFMPEG.PRO ENDE
# -------------------------------------------------------------------------------
