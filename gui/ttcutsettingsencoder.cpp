/*----------------------------------------------------------------------------*/
/* COPYRIGHT: TriTime (c) 2003/2008 / ttcut.tritime.org                       */
/*----------------------------------------------------------------------------*/
/* PROJEKT  : TTCUT 2005                                                      */
/* FILE     : ttcutsettingsenocder.cpp                                        */
/*----------------------------------------------------------------------------*/
/* AUTHOR  : b. altendorf (E-Mail: b.altendorf@tritime.de)   DATE: 02/26/2006 */
/*----------------------------------------------------------------------------*/

// ----------------------------------------------------------------------------
// *** TTCUTSETTINGSENCODER
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

#include "ttcutsettingsencoder.h"

#include "common/ttcut.h"
#include "extern/iencodeprovider.h"

#include <QFileDialog>
#include <QPluginLoader>

  
TTCutSettingsEncoder::TTCutSettingsEncoder(QWidget* parent)
:QWidget(parent)
{
  setupUi(this);

  connect(pbAddEncoder,        SIGNAL(clicked()),         SLOT(onAddEncoder()));
  connect(pbRemoveEncoder,     SIGNAL(clicked()),         SLOT(onRemoveEncoder()));
  connect(pbConfigureEncoder,  SIGNAL(clicked()),         SLOT(onConfigureEncoder()));
}

void TTCutSettingsEncoder::setTitle(__attribute__((unused))const QString& title)
{
}

void TTCutSettingsEncoder::setEncoderList()
{
  while (cbEncoderProg->count() > 0)
  {
    cbEncoderProg->removeItem(0);
  }
  cbEncoderProg->addItems(TTCut::encoderList->keys());
}

void TTCutSettingsEncoder::setTabData()
{
  cbEncodingMode->setChecked( TTCut::encoderMode );

  // fill cbEncoderProg
  setEncoderList();
  cbEncoderProg->setCurrentText(TTCut::encoderProg);
}

void TTCutSettingsEncoder::getTabData()
{
  TTCut::encoderMode = cbEncodingMode->isChecked();
  TTCut::encoderProg = cbEncoderProg->currentText();
  for (QString key : TTCut::encoderList->keys()) {
    int index = cbEncoderProg->findText(key);
    if (index == -1)
    {
      TTCut::encoderList->remove(key);
    }
  }
}

void TTCutSettingsEncoder::onConfigureEncoder()
{
  qobject_cast<IEncodeProvider*>(TTCut::encoderList->value(cbEncoderProg->currentText())->instance())->configDialog();
}

void TTCutSettingsEncoder::onAddEncoder()
{
  QString fileName = QFileDialog::getOpenFileName(this, tr("Add Encoder"));
  if (fileName.isEmpty())
  {
  }
  else
  {
    QPluginLoader* loader = new QPluginLoader(fileName);
    IEncodeProvider* encoder = qobject_cast<IEncodeProvider*>(loader->instance());
    if (encoder == NULL)
    {
      QString error = loader->errorString();
      int test = 1;
    }
    else
    {
      QString pluginName = encoder->name();
      TTCut::encoderList->insert(pluginName, loader);
      setEncoderList();
    }
  }
}

void TTCutSettingsEncoder::onRemoveEncoder()
{
  cbEncoderProg->removeItem(cbEncoderProg->currentIndex());
}
