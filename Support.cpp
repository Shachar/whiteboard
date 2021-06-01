#include "Support.h"

#include <QMenu>

void adoptMenuActions(QWidget *newParent, QAction *action) {
    if( action->isSeparator() ) {
        // Not adoping a separator
    } else if(action->menu()) {
        // Action menu
        for( auto &innerAction : action->menu()->actions() )
            adoptMenuActions(newParent, innerAction);
    } else {
        newParent->addAction(action);
    }
}
