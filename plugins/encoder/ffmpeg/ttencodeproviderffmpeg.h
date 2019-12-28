/*----------------------------------------------------------------------------*/
/* COPYRIGHT: Minei3oat (c) 2019 / github.com/Minei3oat                       */
/*----------------------------------------------------------------------------*/
/* PROJEKT  : TTENCODEPROVIDERFFMPEG 2019                                     */
/* FILE     : ttencodeproviderffmpeg.h                                        */
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

#ifndef TTENCODEPROVIDERFFMPEG_H
#define TTENCODEPROVIDERFFMPEG_H

#include "../../../extern/iencodeprovider.h"

class TTEncodeProviderFFmpeg : public QObject, public IEncodeProvider
{
  Q_OBJECT
  Q_INTERFACES(IEncodeProvider)
  Q_PLUGIN_METADATA(IID "TTCut.IEncodeProvider/1.0")

  public:
    TTEncodeProviderFFmpeg();
    ~TTEncodeProviderFFmpeg();

    QString configDialog();
    QString buildCommandLine(TTEncodeParameter* enc_par);
    QString configuration();
    void    configure(QString configuration);
    QString name() { return eName; }
    void    setName(QString name) { eName = name; }

  private:
    QString videoAspectCodeToString(int videoAspectCode);

    QString eName;
    QString additional_arguments;
    QString encoder_path;

};

#endif // TTENCODEPROVIDERFFMPEG_H
