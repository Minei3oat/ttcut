/*----------------------------------------------------------------------------*/
/* COPYRIGHT: Minei3oat (c) 2019 / github.com/Minei3oat                       */
/*----------------------------------------------------------------------------*/
/* PROJEKT  : TTENCODEPROVIDERFFMPEG 2019                                     */
/* FILE     : ttencodeproviderffmpeg.cpp                                      */
/*----------------------------------------------------------------------------*/
/* AUTHOR  : Minei3oat                                       DATE: 12/21/2019 */
/*----------------------------------------------------------------------------*/

// ----------------------------------------------------------------------------
// TTENCODEPROVIDERFFMPEG
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

#include "ttencodeproviderffmpeg.h"

#include <QJsonDocument>
#include <QJsonObject>

/* /////////////////////////////////////////////////////////////////////////////
 * Create EncodeProvider
 */
TTEncodeProviderFFmpeg::TTEncodeProviderFFmpeg()
{
  eName                = QString("FFmpeg");
  additional_arguments = QString("");
  encoder_path         = QString("ffmpeg");
}

/* /////////////////////////////////////////////////////////////////////////////
 * Clean up used resources
 */
TTEncodeProviderFFmpeg::~TTEncodeProviderFFmpeg()
{
}



/* /////////////////////////////////////////////////////////////////////////////
 * Show a configuration dialog
 */
QString TTEncodeProviderFFmpeg::configDialog()
{
  //ToDo show a dialog for configuring parameters of the encoder (additional_arguments)
  return configuration();
}

/* /////////////////////////////////////////////////////////////////////////////
 * Parameter for the encoder
 */
QString TTEncodeProviderFFmpeg::buildCommandLine(TTEncodeParameter* enc_par)
{
  QString command = QString("%1 -i %2 -y -target dvd -f mpeg2video -flags cgop")
                           .arg(encoder_path)
                           .arg(enc_par->aviFileInfo().absoluteFilePath());
  if (enc_par->videoInterlaced())
  {
    command += "+ilme -top %1";
    command = command.arg(enc_par->videoTopFieldFirst());
  }
  command += " -sc_threshold 1000000000 -bf 2 -vf setdar=%1 %2 %3";
  command = command.arg(videoAspectCodeToString(enc_par->videoAspectCode()))
                   .arg(additional_arguments)
                   .arg(enc_par->mpeg2FileInfo().absoluteFilePath());
  return command;
}

QString TTEncodeProviderFFmpeg::videoAspectCodeToString(int videoAspectCode)
{
  QString szTemp;

  if ( videoAspectCode == 1 ) szTemp = "1/1";
  if ( videoAspectCode == 2 ) szTemp = "4/3";
  if ( videoAspectCode == 3 ) szTemp = "16/9";
  if ( videoAspectCode == 4 ) szTemp = "2.21/1";

  return szTemp;
}

/* /////////////////////////////////////////////////////////////////////////////
 * Get the configuration of this encoder
 */
QString TTEncodeProviderFFmpeg::configuration()
{
  QJsonObject json = QJsonObject();
  json["additional_arguments"] = additional_arguments;
  json["encoderPath"]          = encoder_path;
  QJsonDocument doc = QJsonDocument(json);
  return doc.toJson(QJsonDocument::Compact);
}

/* /////////////////////////////////////////////////////////////////////////////
 * Configure the encoder
 */
void TTEncodeProviderFFmpeg::configure(QString configuration)
{
  QJsonValue json = QJsonValue(configuration);
  additional_arguments = json["additional_arguments"].toString(additional_arguments);
  encoder_path         = json["encoderPath"].toString(encoder_path);
}
