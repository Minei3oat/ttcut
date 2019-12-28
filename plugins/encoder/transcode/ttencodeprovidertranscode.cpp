/*----------------------------------------------------------------------------*/
/* COPYRIGHT: Minei3oat (c) 2019 / github.com/Minei3oat                       */
/*----------------------------------------------------------------------------*/
/* PROJEKT  : TTENCODEPROVIDERTRANSCODE 2019                                  */
/* FILE     : ttencodeprovidertranscode.cpp                                   */
/*----------------------------------------------------------------------------*/
/* AUTHOR  : Minei3oat                                       DATE: 12/21/2019 */
/*----------------------------------------------------------------------------*/

// ----------------------------------------------------------------------------
// TTENCODEPROVIDERTRANSCODE
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

#include "ttencodeprovidertranscode.h"

#include <QJsonDocument>
#include <QJsonObject>

/* /////////////////////////////////////////////////////////////////////////////
 * Create EncodeProvider
 */
TTEncodeProviderTranscode::TTEncodeProviderTranscode()
{
  eName                = "transcode";
  additional_arguments = "";
  encoder_path         = "transcode";
}

/* /////////////////////////////////////////////////////////////////////////////
 * Clean up used resources
 */
TTEncodeProviderTranscode::~TTEncodeProviderTranscode()
{
}



/* /////////////////////////////////////////////////////////////////////////////
 * Show a configuration dialog
 */
QString TTEncodeProviderTranscode::configDialog()
{
  //ToDo show a dialog for configuring parameters of the encoder (additional_arguments)
  return configuration();
}

/* /////////////////////////////////////////////////////////////////////////////
 * Parameter for the encoder
 */
QString TTEncodeProviderTranscode::buildCommandLine(TTEncodeParameter* enc_par)
{
  return QString("%1 -i %2 -x avi --pre_clip 0 --export_prof dvd --export_asr %3 -o %4 %5")
                .arg(encoder_path)
                .arg(enc_par->aviFileInfo().absoluteFilePath())
                .arg(enc_par->videoAspectCode())
                .arg(enc_par->mpeg2FileInfo().absoluteFilePath())
                .arg(additional_arguments);
}

/* /////////////////////////////////////////////////////////////////////////////
 * Get the configuration of this encoder
 */
QString TTEncodeProviderTranscode::configuration()
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
void TTEncodeProviderTranscode::configure(QString configuration)
{
  QJsonValue json = QJsonValue(configuration);
  additional_arguments = json["additional_arguments"].toString(additional_arguments);
  encoder_path         = json["encoderPath"].toString(encoder_path);
}
