/*----------------------------------------------------------------------------*/
/* COPYRIGHT: Minei3oat (c) 2019 / github.com/Minei3oat                       */
/*----------------------------------------------------------------------------*/
/* PROJEKT  : TTCUT 2019                                                      */
/* FILE     : iencodeprovider.h                                              */
/*----------------------------------------------------------------------------*/
/* AUTHOR  : Minei3oat                                       DATE: 12/21/2019 */
/*----------------------------------------------------------------------------*/

// ----------------------------------------------------------------------------
// IENCODEPROVIDER
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

#ifndef IENCODEPROVIDER_H
#define IENCODEPROVIDER_H

#include "ttencodeparameter.h"

#include <QtPlugin>

class IEncodeProvider
{
  public:
    virtual QString configDialog() = 0;
    virtual QString buildCommandLine(TTEncodeParameter* enc_par) = 0;
    virtual QString configuration() = 0;
    virtual void    configure(QString configuration) = 0;
    virtual QString name() = 0;
    virtual void setName(QString name) = 0;
};
Q_DECLARE_INTERFACE(IEncodeProvider, "TTCut.IEncodeProvider/1.0");

#endif // IENCODEPROVIDER_H
