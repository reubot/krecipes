/***************************************************************************
 *   Copyright (C) 2003 by Unai Garro                                      *
 *   ugarro@users.sourceforge.net                                          *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include <unistd.h>
#include <pwd.h>

#include <iostream>
#include <qhbox.h>
#include <qvgroupbox.h>
#include <qlayout.h>
#include <qpixmap.h>
#include <qpushbutton.h>
#include <kconfig.h>
#include <kapp.h>
#include <kstandarddirs.h>
#include <klocale.h>
#include "setupwizard.h"

SetupWizard::SetupWizard(QWidget *parent, const char *name, bool modal, WFlags f):KWizard(parent,name, modal,f)
{
welcomePage= new WelcomePage(this);
addPage(welcomePage,i18n("Welcome to Krecipes"));
permissionsSetupPage=new PermissionsSetupPage(this);
addPage(permissionsSetupPage,i18n("Database Permissions"));
serverSetupPage = new ServerSetupPage(this);
addPage(serverSetupPage,i18n("Server Settings"));
dataInitializePage= new DataInitializePage(this);
addPage(dataInitializePage,i18n("Initialize Database"));
savePage = new SavePage(this);
addPage(savePage,i18n("Finish and Save Settings"));

setFinishEnabled(savePage,true); // Enable finish button
setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
connect(finishButton(),SIGNAL(clicked()),this,SLOT(save()));
}


SetupWizard::~SetupWizard()
{

}

WelcomePage::WelcomePage(QWidget *parent):QWidget(parent)
{
QGridLayout *layout=new QGridLayout(this,1,1,0,0);
QSpacerItem *spacer_top=new QSpacerItem(10,10,QSizePolicy::Minimum, QSizePolicy::Fixed);
layout->addItem(spacer_top,0,1);
QSpacerItem *spacer_left=new QSpacerItem(10,10,QSizePolicy::Fixed, QSizePolicy::Minimum);
layout->addItem(spacer_left,1,0);
QPixmap logoPixmap (locate("data", "krecipes/pics/wizard.png"));
logo=new QLabel(this);
logo->setPixmap(logoPixmap);
logo->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
layout->addWidget(logo,1,1);

QSpacerItem *spacer_from_image=new QSpacerItem(10,10,QSizePolicy::Fixed, QSizePolicy::Minimum);
layout->addItem(spacer_from_image,1,2);

welcomeText=new QLabel(this);
welcomeText->setText(i18n("Thank you very much for choosing Krecipes.\nIt looks like this is the first time you are using it. This wizard will help you with the initial setup so that you can start using it quickly.\n\nWelcome and enjoy cooking! ;-) "));
welcomeText->setMinimumWidth(200);
welcomeText->setMaximumWidth(10000);
welcomeText->setSizePolicy(QSizePolicy::MinimumExpanding,QSizePolicy::Minimum);
welcomeText->setAlignment( int( QLabel::AlignTop |QLabel::AlignJustify  ) );
layout->addWidget(welcomeText,1,3);

}

PermissionsSetupPage::PermissionsSetupPage(QWidget *parent):QWidget(parent)
{
QGridLayout *layout=new QGridLayout(this,1,1,0,0);
QSpacerItem *spacer_top=new QSpacerItem(10,10,QSizePolicy::Minimum, QSizePolicy::Fixed);
layout->addItem(spacer_top,0,1);
QSpacerItem *spacer_left=new QSpacerItem(10,10,QSizePolicy::Fixed, QSizePolicy::Minimum);
layout->addItem(spacer_left,1,0);

// Explanation Text
permissionsText=new QLabel(this);
permissionsText->setText(i18n("This dialog will allow you to specify a MySQL account that has the necessary permissions to access the KRecipes MySQL database.<br><br><b>Most users that use Krecipes and MySQL for the first time can just leave the default parameters and press \'Next\'.</b> <br><br>If you set a MySQL root password before, or you have already permissions as normal user, click on the appropriate option. Otherwise the account 'root' will be used, with no password.<br><br>[For security reasons, we strongly encourage you to setup a MySQL root password if you have not done so yet. Just type as root: mysqladmin password <i>your_password</i>]"));
permissionsText->setMinimumWidth(200);
permissionsText->setMaximumWidth(10000);
permissionsText->setSizePolicy(QSizePolicy::MinimumExpanding,QSizePolicy::Minimum);
permissionsText->setAlignment( int( QLabel::WordBreak | QLabel::AlignTop  ) );
layout->addWidget(permissionsText,1,3);

// Logo
QPixmap permissionsSetupPixmap (locate("data", "krecipes/pics/dbpermissions.png"));
logo=new QLabel(this);
logo->setPixmap(permissionsSetupPixmap);
logo->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
layout->addMultiCellWidget(logo,1,8,1,1);

// Spacer to separate the logo
QSpacerItem *logoSpacer=new QSpacerItem(10,10,QSizePolicy::Fixed, QSizePolicy::Minimum);
layout->addItem(logoSpacer,1,2);

// "The user already has permissions" checkbox
noSetupCheckBox=new QCheckBox(i18n("I have already set the necessary permissions"),this,"noSetupCheckBox");
layout->addWidget(noSetupCheckBox,3,3);
// root checkbox
rootCheckBox=new QCheckBox(i18n("I have already set a MySQL root/admin account"),this,"rootCheckBox");
layout->addWidget(rootCheckBox,4,3);

QSpacerItem *rootInfoSpacer=new QSpacerItem(10,10,QSizePolicy::Minimum,QSizePolicy::Fixed);
layout->addItem(rootInfoSpacer,5,3);

// MySQL root/admin info
QVGroupBox *rootInfoVGBox=new QVGroupBox(this,"rootInfoVGBox"); rootInfoVGBox->setTitle(i18n("MySQL Administrator Account"));
rootInfoVGBox->setEnabled(false); // Disable by default
rootInfoVGBox->setInsideSpacing(10);
layout->addWidget(rootInfoVGBox,6,3);

// Input boxes (widgets inserted below)
QHBox *userBox=new QHBox(rootInfoVGBox); userBox->setSpacing(10);
QHBox *passBox=new QHBox(rootInfoVGBox); passBox->setSpacing(10);

// User Entry
QLabel *userLabel=new QLabel(userBox); userLabel->setText(i18n("Username:"));
userEdit=new KLineEdit(userBox);

// Password Entry
QLabel *passLabel=new QLabel(passBox); passLabel->setText(i18n("Password:"));
passEdit=new KLineEdit(passBox);
passEdit->setEchoMode(QLineEdit::Password);

// Connect Signals & slots

connect(rootCheckBox,SIGNAL(toggled(bool)),rootInfoVGBox,SLOT(setEnabled(bool)));
connect(rootCheckBox,SIGNAL(toggled(bool)),this,SLOT(rootCheckBoxChanged(bool)));
connect(noSetupCheckBox,SIGNAL(toggled(bool)),this,SLOT(noSetupCheckBoxChanged(bool)));
}

void PermissionsSetupPage::rootCheckBoxChanged(bool on)
{
if (on) noSetupCheckBox->setChecked(false); // exclude mutually the options (both can be unset)
}

bool PermissionsSetupPage::doUserSetup()
{
return (!noSetupCheckBox->isChecked());
}

bool PermissionsSetupPage::useAdmin()
{
return (rootCheckBox->isChecked());
}

void PermissionsSetupPage::getAdmin(QString &adminName,QString &adminPass)
{
adminName=userEdit->text();
adminPass=passEdit->text();
}

void PermissionsSetupPage::noSetupCheckBoxChanged(bool on)
{
if (on) rootCheckBox->setChecked(false); // exclude mutually the options (both can be unset)
}

ServerSetupPage::ServerSetupPage(QWidget *parent):QWidget(parent)
{
QGridLayout *layout=new QGridLayout(this,1,1,0,0);
QSpacerItem *spacer_top=new QSpacerItem(10,10,QSizePolicy::Minimum, QSizePolicy::Fixed);
layout->addItem(spacer_top,0,1);
QSpacerItem *spacer_left=new QSpacerItem(10,10,QSizePolicy::Fixed, QSizePolicy::Minimum);
layout->addItem(spacer_left,1,0);


// Image

QPixmap serverSetupPixmap (locate("data", "krecipes/pics/network.png"));
logo=new QLabel(this);
logo->setPixmap(serverSetupPixmap);
logo->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
layout->addMultiCellWidget(logo,1,12,1,1);

QSpacerItem *spacer_from_image=new QSpacerItem(10,10,QSizePolicy::Fixed, QSizePolicy::Minimum);
layout->addItem(spacer_from_image,1,2);


// Explanation text
serverSetupText=new QLabel(this);
serverSetupText->setText(i18n("In this dialog you can adjust the MySQL server settings. "));
serverSetupText->setMinimumWidth(200);
serverSetupText->setMaximumWidth(10000);
serverSetupText->setSizePolicy(QSizePolicy::MinimumExpanding,QSizePolicy::Minimum);
serverSetupText->setAlignment( int( QLabel::AlignTop |QLabel::AlignJustify  ) );
layout->addMultiCellWidget(serverSetupText,1,1,3,4);

// Text spacer

QSpacerItem* textSpacer = new QSpacerItem( 10,30, QSizePolicy::Minimum, QSizePolicy::Fixed );
layout->addItem(textSpacer,2,3 );


// Username Input

QLabel* usernameText=new QLabel(i18n("Username:"), this);
usernameText->setFixedSize(QSize(100,20));
usernameText->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
layout->addWidget(usernameText,3,3);

usernameEdit=new KLineEdit(this);
usernameEdit->setFixedSize(QSize(120,20));
usernameEdit->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	// get username
	uid_t userID; QString username;struct passwd *user;
	userID=getuid();user=getpwuid (userID); username=user->pw_name;

usernameEdit->setText(username);
layout->addWidget(usernameEdit,3,4);


// Spacer 1

QSpacerItem* spacerRow1 = new QSpacerItem( 10,10, QSizePolicy::Minimum, QSizePolicy::Fixed );
layout->addItem( spacerRow1,4,3 );


// Password

QLabel* passwordText=new QLabel(i18n("Password:"), this);
passwordText->setFixedSize(QSize(100,20));
passwordText->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
layout->addWidget(passwordText,5,3);

passwordEdit=new KLineEdit(this);
passwordEdit->setEchoMode(QLineEdit::Password);
passwordEdit->setFixedSize(QSize(120,20));
passwordEdit->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
layout->addWidget(passwordEdit,5,4);

// Spacer 2
QSpacerItem* spacerRow2 = new QSpacerItem( 10,10, QSizePolicy::Minimum, QSizePolicy::Fixed );
layout->addItem( spacerRow2,6,3 );



// DB Name

QLabel* dbNameText=new QLabel(i18n("Database Name:"), this);
dbNameText->setFixedSize(QSize(100,20));
dbNameText->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
layout->addWidget(dbNameText,7,3);

dbNameEdit=new KLineEdit(this);
dbNameEdit->setFixedSize(QSize(120,20));
dbNameEdit->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
dbNameEdit->setText("Krecipes");
layout->addWidget(dbNameEdit,7,4);


// Spacer 3
QSpacerItem* spacerRow3 = new QSpacerItem( 10,20, QSizePolicy::Minimum, QSizePolicy::Fixed );
layout->addItem( spacerRow3, 8,3 );


// Remote server checkbox

remoteServerCheckBox=new QCheckBox(i18n("The server is remote"),this,"remoteServerCheckBox");
layout->addWidget(remoteServerCheckBox,9,3);

// Spacer 4
QSpacerItem* spacerRow4 = new QSpacerItem( 10,10, QSizePolicy::Minimum, QSizePolicy::Fixed );
layout->addItem( spacerRow4, 10,3 );

// Server & Client Box
QGroupBox *serverSettingsGBox=new QGroupBox(this,"serverSettingsGBox"); serverSettingsGBox->setTitle(i18n("Server / Client settings"));
serverSettingsGBox->setEnabled(false); // Disable by default
serverSettingsGBox->setInsideSpacing(10);
serverSettingsGBox->setColumns(2);
layout->addMultiCellWidget(serverSettingsGBox,11,11,3,4);


// Server
QLabel* serverText=new QLabel(i18n("Server:"), serverSettingsGBox);
serverEdit=new KLineEdit(serverSettingsGBox);
serverEdit->setText("localhost");

// Client
QLabel* clientText=new QLabel(i18n("Client:"), serverSettingsGBox);
clientEdit=new KLineEdit(serverSettingsGBox);
clientEdit->setText("localhost");

// Spacer 5
QSpacerItem* spacerRow5 = new QSpacerItem( 12,12, QSizePolicy::Minimum, QSizePolicy::MinimumExpanding);
layout->addItem( spacerRow5, 12,3 );

QSpacerItem* spacerRight = new QSpacerItem( 10,10, QSizePolicy::MinimumExpanding, QSizePolicy::Fixed );
layout->addItem( spacerRight,1,5 );

// Signals & Slots
connect(remoteServerCheckBox,SIGNAL(toggled(bool)),serverSettingsGBox,SLOT(setEnabled(bool)));

}

QString ServerSetupPage::server(void)
{
return(this->serverEdit->text());
}

QString ServerSetupPage::user(void)
{
return(this->usernameEdit->text());
}

QString ServerSetupPage::password(void)
{
return(this->passwordEdit->text());
}

QString ServerSetupPage::dbName(void)
{
return(this->dbNameEdit->text());;
}

void ServerSetupPage::getServerInfo(QString &host, QString &client, QString &user, QString &pass)
{
host=serverEdit->text();
client=serverEdit->text(); //FIXME!
user=usernameEdit->text();
pass=passwordEdit->text();
}

SavePage::SavePage(QWidget *parent):QWidget(parent)
{
QGridLayout *layout=new QGridLayout(this,1,1,0,0);
QSpacerItem *spacer_top=new QSpacerItem(10,10,QSizePolicy::Minimum, QSizePolicy::Fixed);
layout->addItem(spacer_top,0,1);
QSpacerItem *spacer_left=new QSpacerItem(10,10,QSizePolicy::Fixed, QSizePolicy::Minimum);
layout->addItem(spacer_left,1,0);

QPixmap logoPixmap (locate("data", "krecipes/pics/save.png"));
logo=new QLabel(this);
logo->setPixmap(logoPixmap);
logo->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
layout->addWidget(logo,1,1);

QSpacerItem *spacer_from_image=new QSpacerItem(10,10,QSizePolicy::Fixed, QSizePolicy::Minimum);
layout->addItem(spacer_from_image,1,2);

saveText=new QLabel(this);
saveText->setText(i18n("Congratulations! All the necessary configuration setup are done. Press 'Finish' to continue, and enjoy cooking!"));
saveText->setMinimumWidth(200);
saveText->setMaximumWidth(10000);
saveText->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);

saveText->setAlignment( int( QLabel::WordBreak | QLabel::AlignVCenter) );
layout->addWidget(saveText,1,3);


}

void SetupWizard::save(void)
{
KConfig *config=kapp->config();

// Save the server data
config->setGroup("Server");
config->writeEntry("Host",serverSetupPage->server());
config->writeEntry("Username",serverSetupPage->user());
config->writeEntry("Password",serverSetupPage->password());
config->writeEntry("DBName",serverSetupPage->dbName());

// Indicate that settings were already made

config->setGroup("Wizard");
config->writeEntry( "SystemSetup",true);

}

void SetupWizard::getOptions(bool &setupUser, bool &initializeData)
{
setupUser=permissionsSetupPage->doUserSetup();
initializeData=dataInitializePage->doInitialization();

}

void SetupWizard::getAdminInfo(bool &enabled,QString &adminUser,QString &adminPass)
{
enabled=permissionsSetupPage->useAdmin();
permissionsSetupPage->getAdmin(adminUser,adminPass);

}

void SetupWizard::getServerInfo(QString &host, QString &client, QString &user, QString &pass)
{
serverSetupPage->getServerInfo(host,client,user,pass);
}

DataInitializePage::DataInitializePage(QWidget *parent):QWidget(parent)
{
QGridLayout *layout=new QGridLayout(this,1,1,0,0);
QSpacerItem *spacer_top=new QSpacerItem(10,10,QSizePolicy::Minimum, QSizePolicy::Fixed);
layout->addItem(spacer_top,0,1);
QSpacerItem *spacer_left=new QSpacerItem(10,10,QSizePolicy::Fixed, QSizePolicy::Minimum);
layout->addItem(spacer_left,1,0);

// Explanation Text
initializeText=new QLabel(this);
initializeText->setText(i18n("Krecipes comes with some delicious default recipes and useful data. <br><br>Would you like to initialize your database with those? Note that this will erase all your previous recipes if you have any. "));
initializeText->setMinimumWidth(200);
initializeText->setMaximumWidth(10000);
initializeText->setSizePolicy(QSizePolicy::MinimumExpanding,QSizePolicy::Minimum);
initializeText->setAlignment( int( QLabel::WordBreak | QLabel::AlignTop  ) );
layout->addWidget(initializeText,1,3);

// Logo
QPixmap dataInitializePixmap (locate("data", "krecipes/pics/dbpermissions.png"));
logo=new QLabel(this);
logo->setPixmap(dataInitializePixmap);
logo->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
layout->addMultiCellWidget(logo,1,8,1,1);

// Spacer to separate the logo
QSpacerItem *logoSpacer=new QSpacerItem(10,10,QSizePolicy::Fixed, QSizePolicy::Minimum);
layout->addItem(logoSpacer,1,2);

// Initialize data checkbox

QSpacerItem *rootInfoSpacer=new QSpacerItem(10,10,QSizePolicy::Minimum,QSizePolicy::Fixed);
layout->addItem(rootInfoSpacer,5,3);

initializeCheckBox=new QCheckBox(i18n("Yes please, initialize the database with the examples"),this,"initializeCheckBox");
layout->addWidget(initializeCheckBox,3,3);
}

bool DataInitializePage::doInitialization(void)
{
return (initializeCheckBox->isChecked());
}
