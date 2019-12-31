/*----------------------------------------------------------------------------*/
/* COPYRIGHT: Minei3oat (c) 2019 / github.com/Minei3oat                       */
/*----------------------------------------------------------------------------*/
/* PROJEKT  : TTCUT 2019                                                      */
/* FILE     : ttsrtsubtitlestream.cpp                                         */
/*----------------------------------------------------------------------------*/
/* AUTHOR  : Minei3oat                                       DATE: 12/29/2019 */
/*----------------------------------------------------------------------------*/

// ----------------------------------------------------------------------------
// TTSRTSUBTITLESTREAM
// ----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Overview
// -----------------------------------------------------------------------------
//
//                               +- TTMpegAudioStream
//             +- TTAudioStream -|
//             |                 +- TTAC3AudioStream
// TTAVStream -|
//             |
//             +- TTVideoStream - TTMpeg2VideoStream
//             |
//             +- TTSubtitleStream - TTSrtSubtitleStream
//
// -----------------------------------------------------------------------------

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

#include "ttsrtsubtitlestream.h"
#include "ttsubtitleheaderlist.h"
#include "ttavheader.h"
#include "data/ttcutparameter.h"

// /////////////////////////////////////////////////////////////////////////////
// -----------------------------------------------------------------------------
// *** TTSrtSubtitleStream: Srt subtitle stream class
// -----------------------------------------------------------------------------
// /////////////////////////////////////////////////////////////////////////////

//! Constructor with QFileInfo and start position
TTSrtSubtitleStream::TTSrtSubtitleStream(const QFileInfo &f_info)
  :TTSubtitleStream(f_info)
{
  log = TTMessageLogger::getInstance();
}

//! Destructor
TTSrtSubtitleStream::~TTSrtSubtitleStream()
{
}

//! Return the stream type
TTAVTypes::AVStreamType TTSrtSubtitleStream::streamType() const
{
  return TTAVTypes::srt_subtitle;
}

//! Returns the streams file extension
QString TTSrtSubtitleStream::streamExtension()
{
  return (stream_info != 0)
        ? stream_info->suffix().toLower()
        : ".srt";
}

//! Return the stream length as QTime
QTime TTSrtSubtitleStream::streamLengthTime()
{
  return QTime(0, 0, 0, 0);
}

//! Cut the subtitle stream
void TTSrtSubtitleStream::cut(int start, int end, TTCutParameter* cp)
{
  int index = header_list->searchTimeIndex(start);
  cp->setCutOutIndex(cp->getCutInIndex()+end-start+1);
  TTFileBuffer* stream_buffer = cp->getTargetStreamBuffer();
  int picsWritten = cp->getNumPicturesWritten();
  int offsett = cp->getCutInIndex()-start;

  while (index < header_list->count())
  {
    TTSubtitleHeader* header = (TTSubtitleHeader*)header_list->at(index);
    if (header->startMSec() > end)
      return;

    picsWritten++;
    QTime subtitleEnd = header->endMSec() <= end ? header->endTime() : QTime::fromMSecsSinceStartOfDay(end);
    QString subtitleCode = QString("%1\r\n%2 --> %3\r\n%4\r\n\r\n")
        .arg(picsWritten)
        .arg(header->startTime().addMSecs(offsett).toString("hh:mm:ss.zzz"))
        .arg(subtitleEnd.addMSecs(offsett).toString("hh:mm:ss.zzz"))
        .arg(header->text());
    stream_buffer->directWrite((quint8*)subtitleCode.toUtf8().data(), subtitleCode.length());
    cp->setNumPicturesWritten(picsWritten);
  }
}

//! Read subtitles
int TTSrtSubtitleStream::createHeaderList()
{
  header_list = new TTSubtitleHeaderList( 100 );

  QString line;
  int counter = -1;

  while (!stream_buffer->atEnd())
  {
    while (line.isEmpty())
    {
      if (stream_buffer->atEnd())
        return header_list->count();
      line  = stream_buffer->readLine("\r\n");
    }
    line = stream_buffer->readLine("\r\n").simplified();
    if (line.toInt() != counter + 1 && counter != -1)
      log->warningMsg("TTSrtSubtitleStream", QString("Subtitles in %1 missing. Reading subtitle %2, last was %3.").arg(fileName()).arg(counter).arg(line));
    counter = line.toInt();

    line = stream_buffer->readLine("\r\n").simplified();
    TTSubtitleHeader* header = new TTSubtitleHeader();
    header->setStartTime(QTime::fromString(line.left(10), "hh:mm:ss.zzz"));
    header->setEndTime(QTime::fromString(line.right(10), "hh:mm:ss.zzz"));

    QString text;
    do
    {
      line = stream_buffer->readLine("\r\n");
      text += line + "\r\n";
    }
    while (!line.isEmpty());
    header->setText(text.left(text.length()-2));

    header_list->append(header);
  }

  return header_list->count();
}

