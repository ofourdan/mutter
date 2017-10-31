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

#ifndef META_KBD_A11Y_DIALOG_H
#define META_KBD_A11Y_DIALOG_H

#include <glib-object.h>
#include <gio/gio.h>

#define META_TYPE_KBD_A11Y_DIALOG (meta_kbd_a11y_dialog_get_type ())
G_DECLARE_INTERFACE (MetaKbdA11yDialog, meta_kbd_a11y_dialog,
		     META, KBD_A11Y_DIALOG, GObject)

typedef enum
{
  META_KBD_A11Y_DIALOG_RESPONSE_ON,
  META_KBD_A11Y_DIALOG_RESPONSE_OFF,
} MetaKbdA11yDialogResponse;

typedef enum
{
  META_KBD_A11Y_STICKY_KEYS,
  META_KBD_A11Y_SLOW_KEYS,
} MetaKbdA11yDialogType;

struct _MetaKbdA11yDialogInterface
{
  GTypeInterface parent_iface;

  void (* show) (MetaKbdA11yDialog *dialog);
  void (* hide) (MetaKbdA11yDialog *dialog);
};

void meta_kbd_a11y_dialog_show     (MetaKbdA11yDialog        *dialog);
void meta_kbd_a11y_dialog_hide     (MetaKbdA11yDialog        *dialog);
void meta_kbd_a11y_dialog_response (MetaKbdA11yDialog        *dialog,
                                    MetaKbdA11yDialogResponse response);

#endif /* META_KBD_A11Y_DIALOG_H */
