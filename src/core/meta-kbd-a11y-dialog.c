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

#include "config.h"

#include "meta/meta-kbd-a11y-dialog.h"
#include "meta/meta-enum-types.h"

enum
{
  RESPONSE,
  LAST_SIGNAL
};

static guint kbd_a11y_dialog_signals[LAST_SIGNAL] = { 0, };

G_DEFINE_INTERFACE (MetaKbdA11yDialog, meta_kbd_a11y_dialog, G_TYPE_OBJECT)

static void
meta_kbd_a11y_dialog_default_init (MetaKbdA11yDialogInterface *iface)
{
  g_object_interface_install_property (iface,
                                       g_param_spec_enum ("type",
                                                          "Setting Type",
                                                          "Keyboard accessibility setting",
                                                           META_TYPE_KBD_A11Y_DIALOG_TYPE,
                                                           META_KBD_A11Y_SLOW_KEYS,
                                                           G_PARAM_READWRITE));
  g_object_interface_install_property (iface,
                                       g_param_spec_boolean ("enabled",
                                                             "Setting enabled",
                                                             "Setting enabled",
                                                             TRUE,
                                                             G_PARAM_READWRITE));

  kbd_a11y_dialog_signals[RESPONSE] =
    g_signal_new ("response",
                  G_TYPE_FROM_INTERFACE (iface),
                  G_SIGNAL_RUN_LAST | G_SIGNAL_ACTION,
                  0,
                  NULL, NULL, NULL,
                  G_TYPE_NONE, 1, META_TYPE_KBD_A11Y_DIALOG_RESPONSE);
}

/**
 * meta_kbd_a11y_dialog_show:
 * @dialog: a #MetaKbdA11yDialog
 *
 * Shows the inhibit shortcuts dialog.
 **/
void
meta_kbd_a11y_dialog_show (MetaKbdA11yDialog *dialog)
{
  MetaKbdA11yDialogInterface *iface;

  g_return_if_fail (META_IS_KBD_A11Y_DIALOG (dialog));

  iface = META_KBD_A11Y_DIALOG_GET_IFACE (dialog);
  iface->show (dialog);
}

/**
 * meta_kbd_a11y_dialog_hide:
 * @dialog: a #MetaKbdA11yDialog
 *
 * Hides the inhibit shortcuts dialog.
 **/
void
meta_kbd_a11y_dialog_hide (MetaKbdA11yDialog *dialog)
{
  MetaKbdA11yDialogInterface *iface;

  g_return_if_fail (META_IS_KBD_A11Y_DIALOG (dialog));

  iface = META_KBD_A11Y_DIALOG_GET_IFACE (dialog);
  iface->hide (dialog);
}

/**
 * meta_kbd_a11y_dialog_response:
 * @dialog: a #MetaKbdA11yDialog
 * @response: a #MetaKbdA11yDialogResponse
 *
 * Responds and closes the dialog. To be called by #MetaKbdA11yDialog
 * implementations.
 **/
void
meta_kbd_a11y_dialog_response (MetaKbdA11yDialog         *dialog,
                               MetaKbdA11yDialogResponse  response)
{
  g_signal_emit (dialog, kbd_a11y_dialog_signals[RESPONSE], 0, response);
  meta_kbd_a11y_dialog_hide (dialog);
}
