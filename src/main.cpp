/*
 * Copyright (C) 2003 Unai Garro <ugarro@users.sourceforge.net>
 */

#include "krecipes.h"
#include <kapplication.h>
#include <kaboutdata.h>
#include <kcmdlineargs.h>
#include <klocale.h>


static const char *description =
    I18N_NOOP("The KDE Cookbook");

static const char *version = "0.6.2";

static KCmdLineOptions options[] =
{
    //{ "+[URL]", I18N_NOOP( "Document to open." ), 0 },
    { 0, 0, 0 }
};

int main(int argc, char **argv)
{
    KAboutData about("krecipes", I18N_NOOP("Krecipes"), version, description,
                     KAboutData::License_GPL, I18N_NOOP("(C) 2003 Unai Garro\n\n___________\n\n\nThis product is RecipeML compatible.\n You can get more information about this file format in:\n http://www.formatdata.com/recipeml"), 0, 0, "mizunoami44@users.sourceforge.net");
    about.addAuthor( "Unai Garro", 0, "ugarro@users.sourceforge.net" );
    about.addAuthor( "Jason Kivlighn", 0, "mizunoami44@users.sourceforge.net" );    about.addAuthor( "Cyril Bosselut", 0, "bosselut@b1project.com" );
    about.setTranslator(I18N_NOOP("INSERT YOUR NAME HERE"),I18N_NOOP("INSERT YOUR EMAIL ADDRESS"));
    KCmdLineArgs::init(argc, argv, &about);
    KCmdLineArgs::addCmdLineOptions(options);
    KApplication app;


    // see if we are starting with session management
    if (app.isRestored())
    {
        RESTORE(Krecipes);
    }
    else
    {
        // no session.. just start up normally
        KCmdLineArgs *args = KCmdLineArgs::parsedArgs();

        QApplication::flushX();
        //
        if (args->count() == 0)
        {
            Krecipes *widget = new Krecipes;
	    app.setMainWidget(widget);
            widget->show();
        }
        else
        {
            int i = 0;
            for (; i < args->count(); i++)
            {
                Krecipes *widget = new Krecipes;
		app.setMainWidget(widget);
                widget->show();
            }
        }
        args->clear();
    }

    return app.exec();
}

