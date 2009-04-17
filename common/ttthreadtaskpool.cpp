/*----------------------------------------------------------------------------*/
/* COPYRIGHT: TriTime (c) 2003/2010 / www.tritime.org                         */
/*----------------------------------------------------------------------------*/
/* PROJEKT  : TTCUT 2008                                                      */
/* FILE     : ttthreadtaskpool.cpp                                            */
/*----------------------------------------------------------------------------*/
/* AUTHOR  : b. altendorf (E-Mail: b.altendorf@tritime.de)   DATE: 01/11/2009 */
/*----------------------------------------------------------------------------*/

// ----------------------------------------------------------------------------
// TTTHREADTASKPOOL
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


#include "ttthreadtaskpool.h"
#include "ttthreadtask.h"

#include "../common/ttmessagelogger.h"
#include "../common/ttexception.h"

//TODO: exctract status args from ttprogressbar
#include "../gui/ttprogressbar.h"

#include <QThreadPool>
#include <QDebug>

//! Constructor
TTThreadTaskPool::TTThreadTaskPool() : QObject()
{
  QThreadPool::globalInstance()->setExpiryTimeout(100);

  log = TTMessageLogger::getInstance();
}

//! Destructor
TTThreadTaskPool::~TTThreadTaskPool()
{
	cleanUpQueue();
}

//! Remove all tasks from the pool
void TTThreadTaskPool::cleanUpQueue()
{
  QThreadPool::globalInstance()->waitForDone();

  QMutableListIterator<TTThreadTask*> t(mTaskQueue);
    while (t.hasNext()) {
    TTThreadTask* task = t.next();

    if (task == 0) continue;

  disconnect(task, SIGNAL(started(TTThreadTask*)),  this, SLOT(onThreadTaskStarted(TTThreadTask*)));
	disconnect(task, SIGNAL(finished(TTThreadTask*)), this, SLOT(onThreadTaskFinished(TTThreadTask*)));
	disconnect(task, SIGNAL(aborted(TTThreadTask*)),  this, SLOT(onThreadTaskAborted(TTThreadTask*)));

  disconnect(task, SIGNAL(statusReport(TTThreadTask*, int, const QString&, quint64)),
             this, SLOT(onStatusReport(TTThreadTask*, int, const QString&, quint64)));


    qDebug() << "remove task " << task->taskName() << " with UUID " << task->taskID();
    t.remove();
  }
}

//! Start the given task
void TTThreadTaskPool::start(TTThreadTask* task, bool runSyncron, int priority)
{
	connect(task, SIGNAL(started(TTThreadTask*)),  this, SLOT(onThreadTaskStarted(TTThreadTask*)));
	connect(task, SIGNAL(finished(TTThreadTask*)), this, SLOT(onThreadTaskFinished(TTThreadTask*)));
	connect(task, SIGNAL(aborted(TTThreadTask*)),  this, SLOT(onThreadTaskAborted(TTThreadTask*)));

  connect(task, SIGNAL(statusReport(TTThreadTask*, int, const QString&, quint64)),
          this, SLOT(onStatusReport(TTThreadTask*, int, const QString&, quint64)));

  if (runningTaskCount() == 0) {
    emit init();
  }

  if (!mTaskQueue.contains(task)) 
    mTaskQueue.enqueue(task);

  log->debugMsg(__FILE__, __LINE__, QString("enqueue task %1, current task count %2").
      arg(task->taskName()).
      arg(mTaskQueue.count()));
  qDebug() << "enqueue task " << (runSyncron ? "(synchron) " : "(asynchron)" ) << task->taskName() << " with UUID " << task->taskID();


  if (runSyncron)
  	task->run();
  else
  	QThreadPool::globalInstance()->start(task, priority);
}

//! Thread task started
void TTThreadTaskPool::onThreadTaskStarted(TTThreadTask* task)
{
  qDebug() << "start " << task->taskName() << " with UUID " << task->taskID() << " active threads " << runningTaskCount() << " queue count " << mTaskQueue.count();
}

//! Thread task finished
void TTThreadTaskPool::onThreadTaskFinished(TTThreadTask* task)
{
	disconnect(task, SIGNAL(started(TTThreadTask*)),  this, SLOT(onThreadTaskStarted(TTThreadTask*)));
	disconnect(task, SIGNAL(finished(TTThreadTask*)), this, SLOT(onThreadTaskFinished(TTThreadTask*)));
	disconnect(task, SIGNAL(aborted(TTThreadTask*)),  this, SLOT(onThreadTaskAborted(TTThreadTask*)));

  disconnect(task, SIGNAL(statusReport(TTThreadTask*, int, const QString&, quint64)),
             this, SLOT(onStatusReport(TTThreadTask*, int, const QString&, quint64)));


  qDebug() << "finished " << task->taskName() << " with UUID " << task->taskID() << " active threads " << runningTaskCount() << " queue count " << mTaskQueue.count();

  if (runningTaskCount() == 0) {
    cleanUpQueue();
    emit exit();
  }
}

//! Thread task aborted
void TTThreadTaskPool::onThreadTaskAborted(TTThreadTask* task)
{
  qDebug(qPrintable(QString("TTThreadTaskPool::Task %1 with uuid %2 aborted. IsRunning %3").
					arg(task->taskName()).
  				arg(task->taskID()).
          arg(task->isRunning())));

	disconnect(task, SIGNAL(started(TTThreadTask*)),  this, SLOT(onThreadTaskStarted(TTThreadTask*)));
	disconnect(task, SIGNAL(finished(TTThreadTask*)), this, SLOT(onThreadTaskFinished(TTThreadTask*)));
	disconnect(task, SIGNAL(aborted(TTThreadTask*)),  this, SLOT(onThreadTaskAborted(TTThreadTask*)));

  disconnect(task, SIGNAL(statusReport(TTThreadTask*, int, const QString&, quint64)),
             this, SLOT(onStatusReport(TTThreadTask*, int, const QString&, quint64)));

  mTaskQueue.removeAll(task);

  if (mTaskQueue.count() == 0) {
    qDebug("No more tasks in queue!");
    emit aborted();
    emit exit();
  }
  //task->setIsRunning(false);
  //qDebug() << "running count " << runningTaskCount();

	/*if (runningTaskCount() == 0) {
    //cleanUpQueue();
    emit aborted();
    emit exit();
  }*/
}

//! Task status reporting
void TTThreadTaskPool::onStatusReport(TTThreadTask* task, int state, const QString& msg, quint64 value)
{
  //qDebug() << "pool status report from task " << task->taskName() << " with UUID " << task->taskID() << " state is " << state;
  emit statusReport(task, state, msg, value);
}

/**
 * User request to abort all current operations
 */
void TTThreadTaskPool::onUserAbortRequest()
{
  qDebug() << "-----------------------------------------------------";
  qDebug() << "TTThreadTaskPool -> request to abort all tasks";

	for (int i=0; i < mTaskQueue.count(); i++) {
		TTThreadTask* task = mTaskQueue.at(i);

    if (task->isRunning())
      task->onUserAbort();
		qApp->processEvents();
	}

  qDebug() << "-----------------------------------------------------";
}

//! Calculate the total percentage progress value of all enqueued tasks
int TTThreadTaskPool::overallPercentage()
{
  mOverallStepCount  = 0;
  quint64 mTotalStepCount    = 0;

  for (int i=0; i < mTaskQueue.count(); i++) {
    TTThreadTask* task = mTaskQueue.at(i);

    if (task == 0) continue;

    mOverallStepCount += task->stepCount();
    mTotalStepCount   += task->totalSteps();
  }


  return  (mOverallStepCount > 0)
      ? (int)((double)100000.0*mOverallStepCount / (double)(100000.0*mTotalStepCount) * 1000.0)
      : 0;
}

//! Calculate the total progress time value of all enqueued tasks
QTime TTThreadTaskPool::overallTime()
{
  mOverallTotalTime.setHMS(0, 0, 0, 0);
  int totalTimeMsecs = 0;

  for (int i=0; i < mTaskQueue.count(); i++) {
    TTThreadTask* task = mTaskQueue.at(i);

    if (task == 0) continue;

    totalTimeMsecs += task->elapsedTime();
  }

  return QTime(0,0,0,0).addMSecs(totalTimeMsecs);
}

int TTThreadTaskPool::runningTaskCount()
{
  int runningCount = 0;

  for (int i=0; i<mTaskQueue.count(); i++) {
    TTThreadTask* task = mTaskQueue.at(i);
    if (task == 0) continue;
    if (task->isRunning()) runningCount++;
  }
  return runningCount;
}
