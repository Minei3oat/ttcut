/*----------------------------------------------------------------------------*/
/* COPYRIGHT: TriTime (c) 2003/2010 / ttcut.tritime.org                       */
/*----------------------------------------------------------------------------*/
/* PROJEKT  : TTCUT 2008                                                      */
/* FILE     : ttnavigation.h                                                  */
/*----------------------------------------------------------------------------*/
/* AUTHOR  : b. altendorf (E-Mail: b.altendorf@tritime.de)   DATE: 12/07/2008 */
/*----------------------------------------------------------------------------*/

// ----------------------------------------------------------------------------
// TTCUTFRAMENAVIGATION
// ----------------------------------------------------------------------------

/*----------------------------------------------------------------------------*/
/* This program is free software; you can redistribute it and/or modify it    */
/* under the terms of the GNU General Public License as published by the Free */
/* Software Foundation;                                                       */
/* either version 2 of the License, or (at your option) any later version.    */
/*                                                                            */
/* This program is distributed in the hope that it will be useful, but WITHOUT*/
/* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or      */
/* FITNESS FOR A PARTICULAR PURPOSE.                                          */
/* See the GNU General Public License for more details.                       */
/*                                                                            */
/* You should have received a copy of the GNU General Public License along    */
/* with this program; if not, write to the Free Software Foundation,          */
/* Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307, USA.              */
/*----------------------------------------------------------------------------*/

#ifndef TTCUTFRAMENAVIGATION_H
#define TTCUTFRAMENAVIGATION_H

#include "ui_ttcutframenavigationwidget.h"

#include "../common/ttmessagelogger.h"
#include "../data/ttcutlist.h"
#include "../avstream/ttavstream.h"

class TTAVItem;

class TTCutFrameNavigation : public QWidget, Ui::TTCutFrameNavigationWidget
{
  Q_OBJECT

  public:
    TTCutFrameNavigation(QWidget* parent=0);

    //void setTitle ( const QString & title );

    void controlEnabled( bool enabled );
    void checkCutPosition(TTAVItem* avData);
    void keyPressEvent(QKeyEvent* e);

  public slots:
    void onPrevIFrame();
    void onNextIFrame();
    void onPrevPFrame();
    void onNextPFrame();
    void onPrevBFrame();
    void onNextBFrame();
    void onSetCutIn();
    void onSetCutOut();
    void onGotoCutIn();
    void onGotoCutOut();
    void onAddCutRange();

    void onEditCut(const TTCutItem& cutData);

  signals:
    void prevIFrame();
    void nextIFrame();
    void prevPFrame();
    void nextPFrame();
    void prevBFrame();
    void nextBFrame();
    void setCutIn(int);
    void setCutOut(int);
    void gotoCutIn(int);
    void gotoCutOut(int);
    void addCutRange(int, int);
    void moveNumSteps(int);
    void moveToHome();
    void moveToEnd();

  protected:

  private:
    TTMessageLogger* log;
    TTCutItem* editCutData;
    bool    isControlEnabled;
    bool    isEditCut;
    bool    isCutInPosition;
    bool    isCutOutPosition;
    int     currentPosition;
    int     currentFrameType;
    QString currentTime;
    int     cutInPosition;
    int     cutOutPosition;
    int     markerPosition;
};

#endif
