/*
 *  scim_waitzar_setup.cpp
 *  Copyright (C) 2008 Seth N. Hetu <sorlok_reaves@yahoo.com>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 3, or (at your option)
 *  any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 USA
 */

/*
 * Modified from scim_thai_imengine_setup.cpp in scim-thai-0.1.0.
 * Copyright (C) 2006 Theppitak Karoonboonyanan <thep@linux.thai.net>
 */

#define Uses_SCIM_UTILITY
#define Uses_SCIM_CONFIG_BASE
#define Uses_SCIM_IMENGINE

#include <gtk/gtk.h>
#include <gtk/scimkeyselection.h>
#include <scim.h>
#include <WordBuilder.h>

#include "scim_waitzar_imengine.h"
#include "scim_waitzar_imengine_config.h"

//Test
/*#define ENCODING_UNICODE 1
#define ENCODING_ZAWGYI 2
#define ENCODING_WININNWA 3*/
//End Test

#include "intl.h"

using namespace scim;

#define scim_module_init waitzar_imengine_setup_LTX_scim_module_init
#define scim_module_exit waitzar_imengine_setup_LTX_scim_module_exit

#define scim_setup_module_create_ui       waitzar_imengine_setup_LTX_scim_setup_module_create_ui
#define scim_setup_module_get_category    waitzar_imengine_setup_LTX_scim_setup_module_get_category
#define scim_setup_module_get_name        waitzar_imengine_setup_LTX_scim_setup_module_get_name
#define scim_setup_module_get_description waitzar_imengine_setup_LTX_scim_setup_module_get_description
#define scim_setup_module_load_config     waitzar_imengine_setup_LTX_scim_setup_module_load_config
#define scim_setup_module_save_config     waitzar_imengine_setup_LTX_scim_setup_module_save_config
#define scim_setup_module_query_changed   waitzar_imengine_setup_LTX_scim_setup_module_query_changed

#define LIST_ICON_SIZE 20

static GtkWidget * create_setup_window ();
static void        load_config (const ConfigPointer& config);
static void        save_config (const ConfigPointer& config);
static bool        query_changed ();

// Module Interface.
extern "C" {
    void scim_module_init (void)
    {
        bindtextdomain (GETTEXT_PACKAGE, SCIM_WAITZAR_LOCALEDIR);
        bind_textdomain_codeset(GETTEXT_PACKAGE, "UTF-8");
    }

    void scim_module_exit (void)
    {
    }

    GtkWidget * scim_setup_module_create_ui (void)
    {
        return create_setup_window ();
    }

    String scim_setup_module_get_category (void)
    {
        return String ("IMEngine");
    }

    String scim_setup_module_get_name (void)
    {
        return String (_("WaitZar"));
    }

    String scim_setup_module_get_description (void)
    {
        return String (_("An IMEngine Module for Myanmar, using the WaitZar input method."));
    }

    void scim_setup_module_load_config (const ConfigPointer &config)
    {
        load_config (config);
    }

    void scim_setup_module_save_config (const ConfigPointer &config)
    {
        save_config (config);
    }

    bool scim_setup_module_query_changed ()
    {
        return query_changed ();
    }
} // extern "C"

// Internal data declaration.

// current config data
static bool   __config_burmese_numerals = false;
static int      __config_default_encoding = ENCODING_UNICODE;

// GUI status
static bool       __has_changed      = false;

//Key config widgets
static GtkWidget* __burmese_numerals_yes_radio  = 0;
static GtkWidget* __burmese_numerals_no_radio  = 0;
static GtkWidget* __output_encoding_unicode_radio  = 0;
static GtkWidget* __output_encoding_zawgyi_radio  = 0;
static GtkWidget* __output_encoding_wininnwa_radio  = 0;

//Related tooltips
#define __burmese_numerals_yes_tooltip        "Enable Burmese numerals. In other words, when the user types 0-9, convert to \u1040-\u1049."
#define __burmese_numerals_no_tooltip          "Disable Burmese numerals. In other words, ignore the keys 0-9 unless you are selecting a word."
#define __output_encoding_unicode_tooltip     "Change the default output encoding to Unicode 5.1+, which works with fonts such as Padauk, Parabaik, and Myanmar3. The default and recommended option."
#define __output_encoding_zawgyi_tooltip       "Change the default output encoding to the custom split of unicode used by the Zawgyi-One font."
#define __output_encoding_wininnwa_tooltip    "Change the default output encoding to the custom re-write of ASCII used by the Win Innwa font, and several related fonts (e.g., Win Kalaw)."

//Configuration strings for burmese_numerals
static const char* __burmese_numerals_config_string [] = {
    "yes",
    "no"
};
static const bool __burmese_numerals_config_vals [] = {
    true,
    false
};
static const int __burmese_numerals_config_opts = 2;

//Configuration strings for default_encoding
static const char* __default_encoding_config_string [] = {
    "unicode",
    "zawgyi",
    "wininnwa"
};
static const int __default_encoding_config_vals [] = {
    ENCODING_UNICODE,
    ENCODING_ZAWGYI,
    ENCODING_WININNWA
};
static const int __default_encoding_config_opts = 3;

// Declaration of internal functions.
static void setup_widget_value();

static void on_burmese_numerals_yes_radio_toggled (GtkToggleButton* togglebutton, gpointer user_data);
static void on_burmese_numerals_no_radio_toggled (GtkToggleButton* togglebutton, gpointer user_data);

static void on_output_encoding_unicode_radio_toggled (GtkToggleButton* togglebutton, gpointer user_data);
static void on_output_encoding_zawgyi_radio_toggled (GtkToggleButton* togglebutton, gpointer user_data);
static void on_output_encoding_wininnwa_radio_toggled (GtkToggleButton* togglebutton, gpointer user_data);


// Function implementations.
static GtkWidget* create_setup_window ()
{
    static GtkWidget* window = 0;

    if (!window) {
	//Re-used for each component.
        GtkWidget*   label;
        GtkWidget*   vbox;
        GtkWidget*   alignment;

	//Group our two configurable options
        GSList*      numerals_radiobutton_group = NULL;
        GSList*      encoding_radiobutton_group = NULL;

	//The tooltips for our widgets.
	GtkTooltips* tooltips;
        tooltips = gtk_tooltips_new ();

        //Create the top-level container. This will be embedded within the scim window.
        window = gtk_vbox_new (FALSE, 0);
        gtk_widget_show(window);

	//////////////////////////////////////////////////////////////////
        //burmese_numerals configuration option
	//////////////////////////////////////////////////////////////////
       label = gtk_label_new (_("Type Burmese Numerals:"));
        gtk_widget_show (label);
        gtk_box_pack_start (GTK_BOX(window), label, FALSE, FALSE, 0);
        gtk_misc_set_alignment(GTK_MISC(label), 0, 0.5);

	//Take up half the available window; pad in options from the left.
        alignment = gtk_alignment_new (0.5, 0.5, 1, 1);
        gtk_widget_show(alignment);
        gtk_container_add(GTK_CONTAINER(window), alignment);
        gtk_alignment_set_padding(GTK_ALIGNMENT(alignment), 0, 0, 12, 0);

	//Encapsulate the radio buttons into a v-box.
        vbox = gtk_vbox_new (FALSE, 0);
        gtk_widget_show (vbox);
        gtk_container_add (GTK_CONTAINER (alignment), vbox);

        //Option 1: Yes
        __burmese_numerals_yes_radio = gtk_radio_button_new_with_mnemonic (NULL, _("_Yes"));
        gtk_widget_show (__burmese_numerals_yes_radio);
        gtk_box_pack_start(GTK_BOX(vbox), __burmese_numerals_yes_radio, FALSE, FALSE, 0);
        gtk_tooltips_set_tip(tooltips, __burmese_numerals_yes_radio, _(__burmese_numerals_yes_tooltip), NULL);
        gtk_radio_button_set_group(GTK_RADIO_BUTTON (__burmese_numerals_yes_radio), numerals_radiobutton_group);
        numerals_radiobutton_group = gtk_radio_button_get_group(GTK_RADIO_BUTTON(__burmese_numerals_yes_radio));

        //Option 2: No
        __burmese_numerals_no_radio = gtk_radio_button_new_with_mnemonic(NULL, _("_No"));
        gtk_widget_show(__burmese_numerals_no_radio);
        gtk_box_pack_start(GTK_BOX (vbox), __burmese_numerals_no_radio, FALSE, FALSE, 0);
        gtk_tooltips_set_tip(tooltips, __burmese_numerals_no_radio, _(__burmese_numerals_no_tooltip), NULL);
        gtk_radio_button_set_group(GTK_RADIO_BUTTON (__burmese_numerals_no_radio), numerals_radiobutton_group);
        numerals_radiobutton_group = gtk_radio_button_get_group(GTK_RADIO_BUTTON(__burmese_numerals_no_radio));



	//////////////////////////////////////////////////////////////////
        //default_encoding configuration option
	//////////////////////////////////////////////////////////////////
        label = gtk_label_new(_("Default Output Encoding:"));
        gtk_widget_show(label);
        gtk_box_pack_start(GTK_BOX(window), label, FALSE, FALSE, 0);
        gtk_misc_set_alignment(GTK_MISC (label), 0, 0.5);

	//Take up half the available window; pad in options from the left.
        alignment = gtk_alignment_new(0.5, 0.5, 1, 1);
        gtk_widget_show(alignment);
        gtk_container_add(GTK_CONTAINER(window), alignment);
        gtk_alignment_set_padding(GTK_ALIGNMENT(alignment), 0, 0, 12, 0);

	//Encapsulate the radio buttons into a v-box.
        vbox = gtk_vbox_new(FALSE, 0);
        gtk_widget_show(vbox);
        gtk_container_add(GTK_CONTAINER(alignment), vbox);

        //Option 1: Unicode 5.1+
        __output_encoding_unicode_radio = gtk_radio_button_new_with_mnemonic(NULL, _("_Unicode 5.1+"));
        gtk_widget_show(__output_encoding_unicode_radio);
        gtk_box_pack_start(GTK_BOX (vbox), __output_encoding_unicode_radio, FALSE, FALSE, 0);
        gtk_tooltips_set_tip(tooltips, __output_encoding_unicode_radio, _(__output_encoding_unicode_tooltip), NULL);
        gtk_radio_button_set_group(GTK_RADIO_BUTTON(__output_encoding_unicode_radio), encoding_radiobutton_group);
        encoding_radiobutton_group = gtk_radio_button_get_group(GTK_RADIO_BUTTON(__output_encoding_unicode_radio));

        //Option 2: Zawgyi-One
        __output_encoding_zawgyi_radio = gtk_radio_button_new_with_mnemonic(NULL, _("_Zawgyi-One"));
        gtk_widget_show(__output_encoding_zawgyi_radio);
        gtk_box_pack_start(GTK_BOX (vbox), __output_encoding_zawgyi_radio, FALSE, FALSE, 0);
        gtk_tooltips_set_tip(tooltips, __output_encoding_zawgyi_radio, _(__output_encoding_zawgyi_tooltip), NULL);
        gtk_radio_button_set_group(GTK_RADIO_BUTTON(__output_encoding_zawgyi_radio), encoding_radiobutton_group);
        encoding_radiobutton_group = gtk_radio_button_get_group(GTK_RADIO_BUTTON(__output_encoding_zawgyi_radio));

        //Option 2: Win Innwa
        __output_encoding_wininnwa_radio = gtk_radio_button_new_with_mnemonic(NULL, _("_Win Innwa"));
        gtk_widget_show(__output_encoding_wininnwa_radio);
        gtk_box_pack_start(GTK_BOX(vbox), __output_encoding_wininnwa_radio, FALSE, FALSE, 0);
        gtk_tooltips_set_tip(tooltips, __output_encoding_wininnwa_radio, _(__output_encoding_wininnwa_tooltip), NULL);
        gtk_radio_button_set_group(GTK_RADIO_BUTTON(__output_encoding_wininnwa_radio), encoding_radiobutton_group);
        encoding_radiobutton_group = gtk_radio_button_get_group(GTK_RADIO_BUTTON(__output_encoding_wininnwa_radio));


	//////////////////////////////////////////////////////////////////
        //Connect signals for button toggling, toggle these buttons
	//////////////////////////////////////////////////////////////////

        g_signal_connect((gpointer)__burmese_numerals_yes_radio, "toggled", G_CALLBACK(on_burmese_numerals_yes_radio_toggled), NULL);
        g_signal_connect((gpointer)__burmese_numerals_no_radio, "toggled", G_CALLBACK(on_burmese_numerals_no_radio_toggled), NULL);
	g_signal_connect((gpointer)__output_encoding_unicode_radio, "toggled", G_CALLBACK(on_output_encoding_unicode_radio_toggled), NULL);
	g_signal_connect((gpointer)__output_encoding_zawgyi_radio, "toggled", G_CALLBACK(on_output_encoding_zawgyi_radio_toggled), NULL);
	g_signal_connect((gpointer)__output_encoding_wininnwa_radio, "toggled", G_CALLBACK(on_output_encoding_wininnwa_radio_toggled), NULL);
        setup_widget_value ();
    }

    return window;
}


static void setup_widget_value ()
{
    //Toggle on our burmesenumerals button
    if (__config_burmese_numerals) {
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON (__burmese_numerals_yes_radio), true);
    } else {
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON (__burmese_numerals_no_radio), true);
    }

    //Toggle on our defaultencoding button
    switch (__config_default_encoding) {
        case ENCODING_UNICODE:
	    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (__output_encoding_unicode_radio), true);
            break;
        case ENCODING_ZAWGYI:
            gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (__output_encoding_zawgyi_radio), true);
            break;
        case ENCODING_WININNWA:
            gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (__output_encoding_wininnwa_radio), true);
            break;
    }
}


void load_config (const ConfigPointer& config)
{
    //Don't parse null configurations
    if (!config)
	return;

    //Read option 1: burmese_numerals
    String str = config->read(String(SCIM_CONFIG_IMENGINE_WAITZAR_BURMESE_NUMERALS), String(__burmese_numerals_config_string[0]));
    __config_burmese_numerals = __burmese_numerals_config_vals[0];
    for (int i=0; i<__burmese_numerals_config_opts; i++) {
        if (str == __burmese_numerals_config_string[i])
            __config_burmese_numerals = __burmese_numerals_config_vals[i];
    }

    //Read option 2: default_encoding
    str = config->read(String(SCIM_CONFIG_IMENGINE_WAITZAR_DEFAULT_ENCODING), String(__default_encoding_config_string[0]));
    __config_default_encoding = __default_encoding_config_vals[0];
    for (int i=0; i<__default_encoding_config_opts; i++) {
        if (str == __default_encoding_config_string[i])
            __config_default_encoding = __default_encoding_config_vals[i];
    }

    //Reset the radio buttons
    setup_widget_value ();
}


void save_config(const ConfigPointer& config)
{
    //Don't save to a null config file.
    if (config.null())
        return;

    //Flash our file: burmese_numerals
    int id = 0;
    for (int i=0; i<__burmese_numerals_config_opts; i++) {
        if (__config_burmese_numerals == __burmese_numerals_config_vals[i])
	    id = i;
    }
    config->write(String(SCIM_CONFIG_IMENGINE_WAITZAR_BURMESE_NUMERALS), String(__burmese_numerals_config_string[id]));

    //Flash our file: default_encoding
    id = 0;
    for (int i=0; i<__default_encoding_config_opts; i++) {
        if (__config_default_encoding == __default_encoding_config_vals[i])
	    id = i;
    }
    config->write(String(SCIM_CONFIG_IMENGINE_WAITZAR_DEFAULT_ENCODING), String(__default_encoding_config_string[id]));

    //The update is complete
    __has_changed = false;
}


bool query_changed ()
{
    return __has_changed;
}


///////////////////////////////////////
//Signal handlers
///////////////////////////////////////


static void on_burmese_numerals_yes_radio_toggled(GtkToggleButton* togglebutton, gpointer user_data)
{
    if (gtk_toggle_button_get_active(togglebutton)) {
        __config_burmese_numerals = true;
        __has_changed = true;
    }
}

static void on_burmese_numerals_no_radio_toggled(GtkToggleButton* togglebutton, gpointer user_data)
{
    if (gtk_toggle_button_get_active(togglebutton)) {
        __config_burmese_numerals = false;
        __has_changed = true;
    }
}


static void on_output_encoding_unicode_radio_toggled(GtkToggleButton* togglebutton, gpointer user_data)
{
    if (gtk_toggle_button_get_active(togglebutton)) {
        __config_default_encoding = ENCODING_UNICODE;
        __has_changed = true;
    }
}

static void on_output_encoding_zawgyi_radio_toggled(GtkToggleButton* togglebutton, gpointer user_data)
{
    if (gtk_toggle_button_get_active(togglebutton)) {
        __config_default_encoding = ENCODING_ZAWGYI;
        __has_changed = true;
    }
}

static void on_output_encoding_wininnwa_radio_toggled(GtkToggleButton* togglebutton, gpointer user_data)
{
    if (gtk_toggle_button_get_active(togglebutton)) {
        __config_default_encoding = ENCODING_WININNWA;
        __has_changed = true;
    }
}
