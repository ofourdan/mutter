/*
 * Copyright (C) 2001, 2002 Havoc Pennington
 * Copyright (C) 2004 Elijah Newren
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

#define _XOPEN_SOURCE /* for kill() */

#include <config.h>
#include "util-private.h"
#include "window-private.h"
#include "meta-kbd-a11y-default-private.h"

#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

typedef struct _MetaKbdA11yDialogDefaultPrivate MetaKbdA11yDialogDefaultPrivate;

struct _MetaKbdA11yDialogDefault
{
  GObject parent_instance;
  MetaKbdA11yDialogType type;
  gboolean enabled;
  int dialog_pid;
  guint child_watch_id;
};

enum {
  PROP_0,
  PROP_TYPE,
  PROP_ENABLED,
  N_PROPS
};

static void meta_kbd_a11y_dialog_iface_init (MetaKbdA11yDialogInterface *iface);

G_DEFINE_TYPE_WITH_CODE (MetaKbdA11yDialogDefault, meta_kbd_a11y_dialog_default,
                         G_TYPE_OBJECT,
                         G_IMPLEMENT_INTERFACE (META_TYPE_KBD_A11Y_DIALOG,
                                                meta_kbd_a11y_dialog_iface_init))

static void
dialog_exited (GPid     pid,
               int      status,
               gpointer user_data)
{
  MetaKbdA11yDialogDefault *dialog = user_data;

  dialog->dialog_pid = -1;

  if (g_spawn_check_exit_status (status, NULL))
    g_signal_emit_by_name (dialog, "response", META_KBD_A11Y_DIALOG_RESPONSE_ON);
  else
    g_signal_emit_by_name (dialog, "response", META_KBD_A11Y_DIALOG_RESPONSE_OFF);
}

static void
meta_kbd_a11y_dialog_default_show (MetaKbdA11yDialog *dialog)
{
  MetaKbdA11yDialogDefault *dialog_default = META_KBD_A11Y_DIALOG_DEFAULT (dialog);
  const char *message;
  gchar *content;
  GPid dialog_pid;

  if (dialog_default->dialog_pid >= 0)
    return;

  if (dialog_default->type == META_KBD_A11Y_SLOW_KEYS)
    message = dialog_default->enabled ? _("Slow Keys Turned On")
                                      : _("Slow Keys Turned Off");
  else /* dialog_default->type == META_KBD_A11Y_STICKY_KEYS */
    message = dialog_default->enabled ? _("Sticky Keys Turned On")
                                      : _("Sticky Keys Turned Off");

  content = g_strdup_printf ("<b>%s</b>", message);
  dialog_pid =
    meta_show_dialog ("--question",
                      content, NULL,
                      NULL,
                      dialog_default->enabled ? _("Leave On") : _("Turn On"),
                      dialog_default->enabled ? _("Turn Off") : _("Leave Off"),
                      "preferences-desktop-accessibility-symbolic",
                      None, NULL, NULL);
  g_free (content);

  dialog_default->dialog_pid = dialog_pid;
  g_child_watch_add (dialog_pid, dialog_exited, dialog);
}

static void
meta_kbd_a11y_dialog_default_hide (MetaKbdA11yDialog *dialog)
{
  MetaKbdA11yDialogDefault *dialog_default;

  dialog_default = META_KBD_A11Y_DIALOG_DEFAULT (dialog);

  if (dialog_default->child_watch_id)
    {
      g_source_remove (dialog_default->child_watch_id);
      dialog_default->child_watch_id = 0;
    }

  if (dialog_default->dialog_pid > -1)
    {
      kill (dialog_default->dialog_pid, SIGTERM);
      dialog_default->dialog_pid = -1;
    }
}

static void
meta_kbd_a11y_dialog_iface_init (MetaKbdA11yDialogInterface *iface)
{
  iface->show = meta_kbd_a11y_dialog_default_show;
  iface->hide = meta_kbd_a11y_dialog_default_hide;
}

static void
meta_kbd_a11y_dialog_default_finalize (GObject *object)
{
  MetaKbdA11yDialogDefault *dialog;

  dialog = META_KBD_A11Y_DIALOG_DEFAULT (object);

  if (dialog->child_watch_id)
    g_source_remove (dialog->child_watch_id);

  if (dialog->dialog_pid > -1)
    {
      kill (dialog->dialog_pid, SIGKILL);
      dialog->dialog_pid = -1;
    }

  G_OBJECT_CLASS (meta_kbd_a11y_dialog_default_parent_class)->finalize (object);
}

static void
meta_kbd_a11y_dialog_default_set_property (GObject      *object,
                                           guint         prop_id,
                                           const GValue *value,
                                           GParamSpec   *pspec)
{
  MetaKbdA11yDialogDefault *dialog;

  dialog = META_KBD_A11Y_DIALOG_DEFAULT (object);

  switch (prop_id)
    {
    case PROP_TYPE:
      dialog->type = g_value_get_enum (value);
      break;
    case PROP_ENABLED:
      dialog->enabled = g_value_get_boolean (value);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
    }
}

static void
meta_kbd_a11y_dialog_default_get_property (GObject    *object,
                                           guint       prop_id,
                                           GValue     *value,
                                           GParamSpec *pspec)
{
  MetaKbdA11yDialogDefault *dialog;

  dialog = META_KBD_A11Y_DIALOG_DEFAULT (object);

  switch (prop_id)
    {
    case PROP_TYPE:
      g_value_set_enum (value, dialog->type);
      break;
    case PROP_ENABLED:
      g_value_set_boolean (value, dialog->enabled);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
    }
}

static void
meta_kbd_a11y_dialog_default_class_init (MetaKbdA11yDialogDefaultClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);

  object_class->finalize = meta_kbd_a11y_dialog_default_finalize;
  object_class->set_property = meta_kbd_a11y_dialog_default_set_property;
  object_class->get_property = meta_kbd_a11y_dialog_default_get_property;

  g_object_class_override_property (object_class, PROP_TYPE, "type");
  g_object_class_override_property (object_class, PROP_ENABLED, "enabled");
}

static void
meta_kbd_a11y_dialog_default_init (MetaKbdA11yDialogDefault *dialog)
{
  dialog->dialog_pid = -1;
}

MetaKbdA11yDialog *
meta_kbd_a11y_dialog_default_new (MetaKbdA11yDialogType type,
                                  gboolean              enabled)
{
  return g_object_new (META_TYPE_KBD_A11Y_DIALOG_DEFAULT,
                       "type", type,
                       "enabled", enabled,
                       NULL);
}
