/*
 * Copyright (C) 2017 Red Hat
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef META_KBD_A11Y_DIALOG_DEFAULT_H
#define META_KBD_A11Y_DIALOG_DEFAULT_H

#include <meta/meta-kbd-a11y-dialog.h>

#define META_TYPE_KBD_A11Y_DIALOG_DEFAULT (meta_kbd_a11y_dialog_default_get_type ())
G_DECLARE_FINAL_TYPE (MetaKbdA11yDialogDefault,
                      meta_kbd_a11y_dialog_default,
                      META, KBD_A11Y_DIALOG_DEFAULT,
                      GObject)

MetaKbdA11yDialog * meta_kbd_a11y_dialog_default_new (MetaKbdA11yDialogType type,
                                                      gboolean              enabled);


#endif /* META_KBD_A11Y_DIALOG_DEFAULT_H */
