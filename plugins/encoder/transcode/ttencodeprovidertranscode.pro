#/*-----------------------------------------------------------------------------*/
#/* COPYRIGHT: Minei3oat (c) 2019  (github.com/Minei3oat)                       */
#/*-----------------------------------------------------------------------------*/
#/*  FILE:   QMake Projektfile for ttencodeprovidertranscode                    */
#/*-----------------------------------------------------------------------------*/
#/*  ABOUT:                                                                     */
#/*-----------------------------------------------------------------------------*/
#/* AUTHOR  : Minei3oat                                       DATE: 12/25/2019  */
#/*-----------------------------------------------------------------------------*/
#
#
TEMPLATE    = lib
PROJECT     = TTEncodeProviderTranscode
CONFIG      += qt warn_on plugin
QT          += core widgets
DIST        = ttencodeprovidertranscode
TARGET      = ttencodeprovidertranscode

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
HEADERS     += ttencodeprovidertranscode.h\
               ../../../extern/iencodeprovider.h
SOURCES     += ttencodeprovidertranscode.cpp
#
# -------------------------------------------------------------------------------
# TTENCODEPROVIDERTRANSCODE.PRO ENDE
# -------------------------------------------------------------------------------
